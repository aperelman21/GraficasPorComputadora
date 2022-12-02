#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader_m.h>
#include <shader_s.h>
#include <shader.h>
#include <camera.h>
#include <iostream>
#include <fstream>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(const char* path);
void drawSpikes(unsigned int VAO, Shader lightingShader, glm::vec3 PyramidPositions[]);
void drawCorners(unsigned int VAO, Shader lightingShader, glm::vec3 CornerPositions[]);
void drawBorders(unsigned int VAO, Shader lightingShader, glm::vec3 BorderPositions[]);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightingShader("6.multiple_lights.vs", "6.multiple_lights.fs");
    Shader lightCubeShader("6.light_cube.vs", "6.light_cube.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float cubeVertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };
    float planeVertices[] = {
        // positions          //Normal Vector  // texture Coords 
         5.0f, -0.5f,  5.0f,  0.0f,0.0f,1.0f,  2.0f, 0.0f,
        -5.0f, -0.5f,  5.0f,  0.0f,0.0f,1.0f,  0.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f,0.0f,1.0f,  0.0f, 2.0f,

         5.0f, -0.5f,  5.0f,  0.0f,0.0f,1.0f,  2.0f, 0.0f,
        -5.0f, -0.5f, -5.0f,  0.0f,0.0f,1.0f,  0.0f, 2.0f,
         5.0f, -0.5f, -5.0f,  0.0f,0.0f,1.0f,  2.0f, 2.0f
    };

    float pyramidVertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        0.0f, 1.0f, 0.0f,  -0.9486832980505138, 0.31622776601683794,0.0f,
        -0.5f, -0.5f, -0.5f,-0.9486832980505138, 0.31622776601683794,0.0f,
        -0.5f, -0.5f, 0.5f,-0.9486832980505138, 0.31622776601683794,0.0f,

        0.0f, 1.0f, 0.0f,  0.0f, 0.31622776601683794,  0.9486832980505138,
        -0.5f, -0.5f, 0.5f,0.0f, 0.31622776601683794,  0.9486832980505138,
        0.5f, -0.5f, 0.5f,0.0f, 0.31622776601683794,  0.9486832980505138,
        
        0.0f, 1.0f, 0.0f,  0.9486832980505138, 0.31622776601683794,0.0f,
        0.5f, -0.5f, 0.5f,0.9486832980505138, 0.31622776601683794,0.0f,
        0.5f, -0.5f, -0.5f,0.9486832980505138, 0.31622776601683794,0.0f,
        
        0.0f, 1.0f, 0.0f, 0.0f, 0.31622776601683794,  -0.9486832980505138,
        0.5f, -0.5f, -0.5f,0.0f, 0.31622776601683794,  -0.9486832980505138,
        -0.5f, -0.5f,-0.5f, 0.0f, 0.31622776601683794,  -0.9486832980505138,
    };

    glm::vec3 PyramidPositions[] = {
        glm::vec3(0.5f,  0.5f,  0.5f),
        glm::vec3(-0.5f,  0.5f,  0.5f),
        glm::vec3(0.5f,  0.5f,  -0.5f),
        glm::vec3(-0.5f,  0.5f,  -0.5f),
        
    };
    
    // positions of the point lights
    glm::vec3 pointLightPositions[] = {
        glm::vec3(0.7f,  0.2f,  2.0f),
        glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3(0.0f,  0.0f, -3.0f)
    };

    float cornerVertices[] = {
        0.0f, -0.5f, 0.0f,  0.0f, -1.0f,  0.0f,//f
        1.0f, -0.5f, 0.0f,  0.0f, -1.0f,  0.0f,//d
        0.0f, -0.5f, 1.0f,  0.0f, -1.0f,  0.0f,//e

        0.0f, 0.5f, 0.0f,  0.0f, 1.0f,  0.0f,//c
        1.0f, 0.5f, 0.0f,  0.0f, 1.0f,  0.0f,//a
        0.0f, 0.5f, 1.0f,  0.0f, 1.0f,  0.0f,//b

        0.0f, 0.5f, 0.0f,  1.0f, 0.0f,  0.0f,//c
        1.0f, 0.5f, 0.0f,  1.0f, 0.0f,  0.0f,//a
        1.0f, -0.5f, 0.0f,  1.0f, 0.0f,  0.0f,//d
        0.0f, -0.5f, 0.0f,  1.0f, 0.0f,  0.0f,//f
        1.0f, -0.5f, 0.0f,  1.0f, 0.0f,  0.0f,//d
        0.0f, 0.5f, 0.0f,  1.0f, 0.0f,  0.0f,//c

        0.0f, -0.5f, 1.0f,  0.0f, 0.0f,  -1.0f,//e
        0.0f, 0.5f, 0.0f,  0.0f, 0.0f,  -1.0f,//c
        0.0f, 0.5f, 1.0f,  0.0f, 0.0f,  -1.0f,//b
        0.0f, -0.5f, 0.0f,  0.0f, 0.0f,  -1.0f,//f
        0.0f, -0.5f, 1.0f,  0.0f, 0.0f,  -1.0f,//e
        0.0f, 0.5f, 0.0f,  0.0f, 0.0f,  -1.0f,//c

        1.0f, 0.5f, 0.0f,  0.7071067811865475, 0.0f,  0.7071067811865475,//a
        0.0f, 0.5f, 1.0f,  0.7071067811865475, 0.0f,  0.7071067811865475,//b
        0.0f, -0.5f, 1.0f,  0.7071067811865475, 0.0f,  0.7071067811865475,//e
        1.0f, -0.5f, 0.0f,  0.7071067811865475, 0.0f,  0.7071067811865475,//d
        0.0f, -0.5f, 1.0f,  0.7071067811865475, 0.0f,  0.7071067811865475,//e
        1.0f, 0.5f, 0.0f,  0.7071067811865475, 0.0f,  0.7071067811865475,//a
    };

    glm::vec3 CornerPositions[] = {
        
        glm::vec3(-2.0f,  0.0f,  -2.0f),
        glm::vec3(-2.0,  0.0f,  2.0f),
        glm::vec3(2.0f,  0.0f,  2.0f),
        glm::vec3(2.0f,  0.0f,  -2.0f),

    };

    float borderVertices[] = {

        //parte de arriba
        2.0f, -0.0f, 0.0f,   0.0f,1.0f,0.0f, //Fh
        1.8873f,-0.0f,0.5076f,   0.0f,1.0f,0.0f,//Hh
        1.6978f, -0.0f, 0.0f,   0.0f,1.0f,0.0f, //Lh
        1.5947f, -0.0f, 0.4176f,   0.0f,1.0f,0.0f, //Ph
        1.6978f, -0.0f, 0.0f,   0.0f,1.0f,0.0f, //Lh
        1.8873f,-0.0f,0.5076f,   0.0f,1.0f,0.0f,//Hh

        1.8873f,-0.0f,0.5076f,   0.0f,1.0f,0.0f,//Hh
        1.4881f, -0.0f, 0.983f,   0.0f,1.0f,0.0f, //Ih
        1.5947f, -0.0f, 0.4176f,   0.0f,1.0f,0.0f, //Ph
        1.2816f, -0.0f, 0.7576f,   0.0f,1.0f,0.0f, //Qh
        1.5947f, -0.0f, 0.4176f,   0.0f,1.0f,0.0f, //Ph
        1.4881f, -0.0f, 0.983f,   0.0f,1.0f,0.0f, //Ih

        1.4881f, -0.0f, 0.983f,   0.0f,1.0f,0.0f, //Ih
        1.0f, -0.0f, 1.1857f,   0.0f,1.0f,0.0f, //Nh
        1.2816f, -0.0f, 0.7576f,   0.0f,1.0f,0.0f, //Qh
        0.798f, -0.0f, 0.8978f,   0.0f,1.0f,0.0f, //Mh
        1.2816f, -0.0f, 0.7576f,   0.0f,1.0f,0.0f, //Qh
        1.0f, -0.0f, 1.1857f,   0.0f,1.0f,0.0f, //Nh

        0.814f, -0.0f, 1.1999f,   0.0f,1.0f,0.0f, //Jh
        0.798f, -0.0f, 0.8978f,   0.0f,1.0f,0.0f, //Mh
        1.0f, -0.0f, 1.1857f,   0.0f,1.0f,0.0f, //Nh

        1.0f, -0.0f, 1.1857f,   0.0f,1.0f,0.0f, //Nh
        1.0f, -0.0f, 1.5f,   0.0f,1.0f,0.0f, //Oh
        0.798f, -0.0f, 1.5027f,   0.0f,1.0f,0.0f, //Kh
        0.798f, -0.0f, 1.5027f,   0.0f,1.0f,0.0f, //Kh
        0.814f, -0.0f, 1.1999f,   0.0f,1.0f,0.0f, //Jh
        1.0f, 0.0f, 1.1857f,   0.0f,1.0f,0.0f, //Nh
        //lado externo

        2.0f, -0.5f, 0.0f,    0.9762f,0.0f,0.21665f, //F
        1.8873f,-0.5f,0.5076f,   0.9762f,0.0f,0.21665f,//H
        2.0f, 0.0f, 0.0f,   0.9762f,0.0f,0.21665f, //Fh
        1.8873f,0.0f,0.5076f, 0.9762f, 0.0f, 0.21665f,//Hh
        1.8873f,-0.5f,0.5076f, 0.9762f, 0.0f, 0.21665f,//H
        2.0f, 0.0f, 0.0f, 0.9762f, 0.0f, 0.21665f, //Fh

        1.8873f, -0.5f, 0.5076f, 0.7658f, 0.0f, 0.1497f,//H
        1.4881f, -0.5f, 0.983f, 0.7658f, 0.0f, 0.1497f, //I
        1.8873f, 0.0f, 0.5076f, 0.7658f, 0.0f, 0.1497f,//Hh
        1.4881f, 0.0f, 0.983f, 0.7658f, 0.0f, 0.1497f, //Ih
        1.4881f, -0.5f, 0.983f, 0.7658f, 0.0f, 0.1497f, //I
        1.8873f, 0.0f, 0.5076f, 0.7658f, 0.0f, 0.1497f,//Hh

        1.4881f, -0.5f, 0.983f,   0.373f,0.0f,0.9277f, //I
        1.0f, -0.5f, 1.1857f,   0.373f,0.0f,0.9277f, //N
        1.4881f, 0.0f, 0.983f,   0.373f,0.0f,0.9277f, //Ih
        1.0f, 0.0f, 1.1857f,   0.373f,0.0f,0.9277f, //Nh
        1.0f, -0.5f, 1.1857f,   0.373f,0.0f,0.9277f, //N
        1.4881f, 0.0f, 0.983f,   0.373f,0.0f,0.9277f, //Ih

        1.0f, -0.5f, 1.1857f,   1.0f,0.0f,0.0f, //N
        1.0f, -0.5f, 1.5f,   1.0f,0.0f,0.0f, //O
        1.0f, 0.0f, 1.1857f,   1.0f,0.0f,0.0f, //Nh
        1.0f, 0.0f, 1.5f,   1.0f,0.0f,0.0f, //Oh
        1.0f, -0.5f, 1.5f,   1.0f,0.0f,0.0f, //O
        1.0f, 0.0f, 1.1857f,   1.0f,0.0f,0.0f, //Nh

        1.0f, -0.5f, 1.5f,   0.0f,0.0f,1.0f, //O
        0.798f, -0.5f, 1.5027f,   0.0f,0.0f,1.0f, //K
        1.0f, 0.0f, 1.5f,   0.0f,0.0f,1.0f, //Oh
        0.798f, 0.0f, 1.5027f,   0.0f,0.0f,1.0f, //Kh
        0.798f, -0.5f, 1.5027f,   0.0f,0.0f,1.0f, //K
        1.0f, 0.0f, 1.5f,   0.0f,0.0f,1.0f, //Oh

        //lado interno
        0.798f, -0.5f, 0.8978f,   -0.373f,0.0f,-0.9277f, //M
        1.2816f, -0.5f, 0.7576f,   -0.373f,0.0f,-0.9277f, //Q
        0.798f, 0.0f, 0.8978f,   -0.373f,0.0f,-0.9277f, //Mh
        1.2816f, 0.0f, 0.7576f, -0.373f, 0.0f, -0.9277f, //Qh
        1.2816f, -0.5f, 0.7576f, -0.373f, 0.0f, -0.9277f, //Q
        0.798f, 0.0f, 0.8978f, -0.373f, 0.0f, -0.9277f, //Mh

        1.2816f, -0.5f, 0.7576f, -0.7658f, 0.0f, -0.1497f, //Q
        1.5947f, -0.5f, 0.4176f, -0.7658f, 0.0f, -0.1497f, //P
        1.2816f, 0.0f, 0.7576f, -0.7658f, 0.0f, -0.1497f, //Qh
        1.5947f, 0.0f, 0.4176f, -0.7658f, 0.0f, -0.1497f, //Ph
        1.5947f, -0.5f, 0.4176f, -0.7658f, 0.0f, -0.1497f, //P
        1.2816f, 0.0f, 0.7576f, -0.7658f, 0.0f, -0.1497f, //Qh

        1.5947f, -0.5f, 0.4176f, -0.9762f, 0.0f, -0.21665f, //P
        1.6978f, -0.5f, 0.0f, -0.9762f, 0.0f, -0.21665f, //L
        1.5947f, 0.0f, 0.4176f, -0.9762f, 0.0f, -0.21665f, //Ph
        1.6978f, 0.0f, 0.0f, -0.9762f, 0.0f, -0.21665f, //Lh
        1.6978f, -0.5f, 0.0f, -0.9762f, 0.0f, -0.21665f, //L
        1.5947f, 0.0f, 0.4176f, -0.9762f, 0.0f, -0.21665f, //Ph


    };

    glm::vec3 BorderPositions[] = {

        glm::vec3(0.25f,  0.0f,  0.0f),
        glm::vec3(0.0f,  0.0f,  -0.25f),
        glm::vec3(-0.25f,  0.0f,  0.0f),
        glm::vec3(0.0f,  0.0f,  0.25f),
        glm::vec3(-0.25f,  0.0f,  0.0f),
        glm::vec3(0.0f,  0.0f,  0.25f),
        glm::vec3(0.25f,  0.0f,  0.0f),
        glm::vec3(0.0f,  0.0f,  -0.25f),
        

    };
    // first, configure the cube's VAO (and VBO)
    unsigned int cubeVBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);

    unsigned int planeVAO, planeVBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);


    unsigned int piramydVAO, piramydVBO;
    glGenVertexArrays(1, &piramydVAO);
    glGenBuffers(1, &piramydVBO);
    glBindVertexArray(piramydVAO);
    glBindBuffer(GL_ARRAY_BUFFER, piramydVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidVertices), &pyramidVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    unsigned int cornerVAO, cornerVBO;
    glGenVertexArrays(1, &cornerVAO);
    glGenBuffers(1, &cornerVBO);
    glBindVertexArray(cornerVAO);
    glBindBuffer(GL_ARRAY_BUFFER, cornerVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cornerVertices), &cornerVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    unsigned int borderVAO, borderVBO;
    glGenVertexArrays(1, &borderVAO);
    glGenBuffers(1, &borderVBO);
    glBindVertexArray(borderVAO);
    glBindBuffer(GL_ARRAY_BUFFER, borderVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(borderVertices), &borderVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
  

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

 

    // shader configuration
    // --------------------
    lightingShader.use();
    lightingShader.setInt("material.diffuse", 0);
    lightingShader.setInt("material.specular", 1);


    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.Position);
        lightingShader.setVec3("material.ambient", 0.0f, 0.0f, 0.1f);
        lightingShader.setVec3("material.diffuse", 0.0f, 0.0f, 0.1f);
        lightingShader.setVec3("material.specular", 0.0f, 0.0f, 0.0f); // specular lighting doesn't have full effect on this object's material
        lightingShader.setFloat("material.shininess", 32.0f);

        /*
           Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
           the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
           by defining light types as classes and set their values in there, or by using a more efficient uniform approach
           by using 'Uniform buffer objects', but that is something we'll discuss in the 'Advanced GLSL' tutorial.
        */
        // directional light
        lightingShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        lightingShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        lightingShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
        // point light 1
        lightingShader.setVec3("pointLights[0].position", pointLightPositions[0]);
        lightingShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        lightingShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("pointLights[0].constant", 1.0f);
        lightingShader.setFloat("pointLights[0].linear", 0.09);
        lightingShader.setFloat("pointLights[0].quadratic", 0.032);
        // point light 2
        lightingShader.setVec3("pointLights[1].position", pointLightPositions[1]);
        lightingShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        lightingShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("pointLights[1].constant", 1.0f);
        lightingShader.setFloat("pointLights[1].linear", 0.09);
        lightingShader.setFloat("pointLights[1].quadratic", 0.032);
        // point light 3
        lightingShader.setVec3("pointLights[2].position", pointLightPositions[2]);
        lightingShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
        lightingShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("pointLights[2].constant", 1.0f);
        lightingShader.setFloat("pointLights[2].linear", 0.09);
        lightingShader.setFloat("pointLights[2].quadratic", 0.032);
        // point light 4
        lightingShader.setVec3("pointLights[3].position", pointLightPositions[3]);
        lightingShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
        lightingShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
        lightingShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("pointLights[3].constant", 1.0f);
        lightingShader.setFloat("pointLights[3].linear", 0.09);
        lightingShader.setFloat("pointLights[3].quadratic", 0.032);
        // spotLight
        lightingShader.setVec3("spotLight.position", camera.Position);
        lightingShader.setVec3("spotLight.direction", camera.Front);
        lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        lightingShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        lightingShader.setFloat("spotLight.constant", 1.0f);
        lightingShader.setFloat("spotLight.linear", 0.09);
        lightingShader.setFloat("spotLight.quadratic", 0.032);
        lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        lightingShader.setMat4("model", model);

        

        glBindVertexArray(planeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        drawCorners(cornerVAO, lightingShader, CornerPositions);
        drawSpikes(piramydVAO,lightingShader,PyramidPositions);
        drawBorders(borderVAO, lightingShader,BorderPositions);

        // also draw the lamp object(s)
        lightCubeShader.use();
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);

        // we now draw as many light bulbs as we have point lights.
        glBindVertexArray(lightCubeVAO);
        for (unsigned int i = 0; i < 4; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
            lightCubeShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &cubeVBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

void drawSpikes(unsigned int VAO,Shader lightingShader, glm::vec3 PyramidPositions[]) {
    glBindVertexArray(VAO);
    lightingShader.use();
    lightingShader.setVec3("viewPos", camera.Position);
    lightingShader.setVec3("material.ambient", 0.05f, 0.05f, 0.0f);
    lightingShader.setVec3("material.diffuse", 0.5f, 0.5f, 0.04f);
    lightingShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f); // specular lighting doesn't have full effect on this object's material
    lightingShader.setFloat("material.shininess", 32.0f);
    glm::vec3 anglePositions[] = {
        glm::vec3(1.0f, 0.0f, -1.0f),
        glm::vec3(1.0f, 0.0f, 1.0f),
        glm::vec3(-1.0f, 0.0f, -1.0f),
        glm::vec3(-1.0f, 0.0f, 1.0f),
    };
    for (size_t i = 0; i < 4; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, PyramidPositions[i]);
        model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));
        float angle = 90.0f;
        model = glm::rotate(model, glm::radians(angle), anglePositions[i]);
        lightingShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 18);
    }
    
    
}

void drawCorners(unsigned int VAO, Shader lightingShader, glm::vec3 CornerPositions[]) {
    glBindVertexArray(VAO);
    lightingShader.use();
    lightingShader.setVec3("viewPos", camera.Position);
    lightingShader.setVec3("material.ambient", 0.4f, 0.1f, 0.1f);
    lightingShader.setVec3("material.diffuse", 0.4f, 0.1f, 0.1f);
    lightingShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f); // specular lighting doesn't have full effect on this object's material
    lightingShader.setFloat("material.shininess", 32.0f);
    for (size_t i = 0; i < 4; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, CornerPositions[i]);
        
        float angle = 90.0f*i;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
        lightingShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 24);
    }
}

void drawBorders(unsigned int VAO, Shader lightingShader, glm::vec3 BorderPositions[]) {
    glBindVertexArray(VAO);
    lightingShader.use();
    lightingShader.setVec3("viewPos", camera.Position);
    lightingShader.setVec3("material.ambient", 0.0f, 0.8f, 0.1f);
    lightingShader.setVec3("material.diffuse", 0.0f, 0.8f, 0.1f);
    lightingShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f); // specular lighting doesn't have full effect on this object's material
    lightingShader.setFloat("material.shininess", 32.0f);
    for (size_t i = 0; i < 4; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, BorderPositions[i]);
        float angle = 90.0f * i;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
        lightingShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 75);
        model = glm::mat4(1.0f);
        model = glm::translate(model, BorderPositions[4+i]);
        model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(-1.0f, 1.0f, 1.0f));
        lightingShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 75);
    }



}
