#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <stb_image.h>
#include <shader_s.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void create_buffers(unsigned int vao, unsigned int vbo, unsigned int ebo, float vertices[], unsigned int indices[]);
void create_circle(float ver[], unsigned int ind[], float r, float x, float y);
void drawBackground(Shader shader, unsigned int VAO[]);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;
unsigned int VAO[11], VBO[11], EBO[11];

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

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    // build and compile our shader program
    Shader ShaderNormal("shaderNormal.vs", "shaderNormal.fs");


    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float verticesBlack[] = {
        0.5f, 1.0f, 0.0f, //0
        1.0f, 1.0f, 0.0f,
        1.0f, 0.5f, 0.0f, //2
        0.7f, 0.7f, 0.0f, //3
        0.7f, 0.0f, 0.0f, //4
        0.0f, 0.0f, 0.0f, //5
        0.0f, 0.7f, 0.0f, //6
    };
    unsigned int indicesBlack[] = {  // note that we start from 0!
        0, 1, 2,
        3, 4, 5,
        3, 5, 6
    };

    float verticesWhite[] = {
        0.6f, 0.6f, 0.0f, //0
        0.6f, 0.0f, 0.0f, //1
        0.0f, 0.0f, 0.0f, //2
        0.0f, 0.6f, 0.0f, //3
    };
    unsigned int indicesWhite[] = {  // note that we start from 0!
        0, 1, 2,
        0, 2, 3
    };

    float verticesYellow[] = {
        0.5f, 0.5f, 0.0f, //0
        0.5f, 0.0f, 0.0f, //1
        0.0f, 0.0f, 0.0f, //2
        0.0f, 0.5f, 0.0f, //3
    };
    unsigned int indicesYellow[] = {  // note that we start from 0!
        0, 1, 2,
        0, 2, 3
    };


    float vertices_circleBlack[150];
    unsigned int indices_circleBlack[147];
    float vertices_circleWhite[150];
    unsigned int indices_circleWhite[147];
    float vertices_circleYellow[150];
    unsigned int indices_circleYellow[147];
    float vertices_circleBlueOut[150];
    unsigned int indices_circleBlueOut[147];
    float vertices_circleBlueIn[150];
    unsigned int indices_circleBlueIn[147];
    float vertices_red[150];
    unsigned int indices_red[147];

    create_circle(vertices_circleBlack, indices_circleBlack, 0.50, 0.6, 0.0);
    create_circle(vertices_circleWhite, indices_circleWhite, 0.38, 0.6, 0.0);
    create_circle(vertices_circleYellow, indices_circleYellow, 0.26, 0.6, 0.0);
    create_circle(vertices_circleBlueIn, indices_circleBlueIn, 0.15, 0.0, 0.0);
    create_circle(vertices_circleBlueOut, indices_circleBlueOut, 0.2, 0.0, 0.0);
    create_circle(vertices_red, indices_red, 0.07, 0.56, 0.13);

    float vertices_red_tail[] = {
        0.5f,0.5f,0.0f,
        0.388f,0.384,0.0f,
        0.5f,0.45f,0.0f,
        0.3853f,0.3692f,0.0f,
        0.4852f,0.4255f,0.0f,
        0.3831f,0.3539f,0.0f,
        0.4719f,0.4017f,0.0f,
        0.3813f,0.3385f,0.0f,
        0.4608f,0.3819f,0.0f,
        0.3801f,0.3217f,0.0f,
        0.4523f,0.3602f,0.0f,
        0.3792f,0.3032f,0.0f,
        0.4447f,0.3412f,0.0f,
        0.3800f,0.2870f,0.0f,
        0.4394f,0.3256f,0.0f,
        0.3800f,0.2840f,0.0f,
        0.4370f,0.3155f,0.0f,
        0.3800f,0.2785f,0.0f,
        0.4350f,0.3031f,0.0f,
        0.3804f,0.2709f,0.0f,
        0.4333f,0.2921f,0.0f,
        0.3809f,0.2626f,0.0f,
        0.4316f,0.2817f,0.0f,
        0.3817f,0.2558f,0.0f,
        0.4308f,0.2738f,0.0f,
        0.3828f,0.2493f,0.0f,
        0.4309f,0.2663f,0.0f,
        0.3843f,0.244f,0.0f,
        0.431f,0.2579f,0.0f,
        0.386f,0.2368f,0.0f,
        0.4316f,0.2492f,0.0f,
        0.388f,0.2311f,0.0f,
        0.4321f,0.2422f,0.0f,
        0.3900f,0.2229f,0.0f,
        0.4337f,0.2328f,0.0f,
        0.3932f,0.2141f,0.0f,
        0.4349f,0.2240f,0.0f,
        0.3965f,0.2068f,0.0f,
        0.4378f,0.2143f,0.0f,
        0.3998f,0.1965f,0.0f,
        0.4413f,0.2035f,0.0f,
        0.4031f,0.1860f,0.0f,
        0.4473f,0.1928f,0.0f,
        0.4062f,0.1767f,0.0f,
        0.4516f,0.1831f,0.0f,
        0.4106f,0.1680f,0.0f,
        0.4580f,0.1744f,0.0f,
        0.4159f,0.1585f,0.0f,
        0.4649f,0.1661f,0.0f,
        0.4217f,0.1490f,0.0f,
        0.4719f,0.1583f,0.0f,
        0.4293f,0.1387f,0.0f,
        0.4785f,0.1524f,0.0f,
        0.4396f,0.1260f,0.0f,
        0.4844f,0.1491f,0.0f,
        0.4522f,0.1109f,0.0f,
        0.4991f,0.1434f,0.0f,
        0.4698f,0.0980f,0.0f,
        0.5138f,0.1422f,0.0f,
        0.4854f,0.0888f,0.0f,
        0.5208f,0.1416f,0.0f,
        0.5038f,0.0819f,0.0f,
        0.5290f,0.1413f,0.0f,
        0.5174f,0.0767f,0.0f,
        0.5413f,0.07288f,0.0f,
        0.5658f,0.0713f,0.0f,
        0.5927f,0.711f,0.0f,
        0.6130f,0.0786f,0.0f,
        0.6333f,0.0941f,0.0f,
        0.3873f,0.3839f,0.0f,
        0.4994f,0.4493f,0.0f,
        0.4562f,0.4984f,0.0f,

    };
    unsigned int indices_red_tail[195];
    for (int x = 0; x < 63; x++)
    {
        indices_red_tail[(x+1) * 3] = x;
        indices_red_tail[(x + 1) * 3 + 1] = x + 1;
        indices_red_tail[(x + 1) * 3 + 2] = x + 2;
    }
    indices_red_tail[0] = 69;
    indices_red_tail[1] = 70;
    indices_red_tail[2] = 71;
    /*create_buffers(VAO[0], VBO[0], EBO[0],vertices_circleWhite,indices_circleWhite );
    create_buffers(VAO[1], VBO[1], EBO[1], verticesBlack, indicesBlack);
    create_buffers(VAO[2], VBO[2], EBO[2], verticesWhite, indicesWhite);
    create_buffers(VAO[3], VBO[3], EBO[3], verticesYellow, indicesYellow);
    create_buffers(VAO[4], VBO[4], EBO[4], vertices_circleBlack, indices_circleBlack);
    create_buffers(VAO[5], VBO[5], EBO[5], vertices_circleYellow, indices_circleYellow);
    create_buffers(VAO[6], VBO[6], EBO[6], vertices_circleBlueOut, indices_circleBlueOut);
    create_buffers(VAO[7], VBO[7], EBO[7], vertices_circleBlueIn, indices_circleBlueIn);
    create_buffers(VAO[8], VBO[8], EBO[8], vertices_red, indices_red);
    create_buffers(VAO[9], VBO[9], EBO[9], vertices_red_tail, indices_red_tail);*/

    glGenVertexArrays(1, &VAO[1]);
    glGenBuffers(1, &VBO[1]);
    glGenBuffers(1, &EBO[1]);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO[1]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesBlack), verticesBlack, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesBlack), indicesBlack, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);
    //We repeat this for each buffer
    glGenVertexArrays(1, &VAO[2]);
    glGenBuffers(1, &VBO[2]);
    glGenBuffers(1, &EBO[2]);
    glBindVertexArray(VAO[2]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesWhite), verticesWhite, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesWhite), indicesWhite, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glGenVertexArrays(1, &VAO[3]);
    glGenBuffers(1, &VBO[3]);
    glGenBuffers(1, &EBO[3]);
    glBindVertexArray(VAO[3]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesYellow), verticesYellow, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesYellow), indicesYellow, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glGenVertexArrays(1, &VAO[4]);
    glGenBuffers(1, &VBO[4]);
    glGenBuffers(1, &EBO[4]);
    glBindVertexArray(VAO[4]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_circleBlack), vertices_circleBlack, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[4]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_circleBlack), indices_circleBlack, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glGenVertexArrays(1, &VAO[0]);
    glGenBuffers(1, &VBO[0]);
    glGenBuffers(1, &EBO[0]);
    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_circleWhite), vertices_circleWhite, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[4]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_circleWhite), indices_circleWhite, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glGenVertexArrays(1, &VAO[5]);
    glGenBuffers(1, &VBO[5]);
    glGenBuffers(1, &EBO[5]);
    glBindVertexArray(VAO[5]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[5]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_circleYellow), vertices_circleYellow, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[5]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_circleYellow), indices_circleYellow, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glGenVertexArrays(1, &VAO[6]);
    glGenBuffers(1, &VBO[6]);
    glGenBuffers(1, &EBO[6]);
    glBindVertexArray(VAO[6]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[6]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_circleBlueOut), vertices_circleBlueOut, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[6]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_circleBlueOut), indices_circleBlueOut, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glGenVertexArrays(1, &VAO[7]);
    glGenBuffers(1, &VBO[7]);
    glGenBuffers(1, &EBO[7]);
    glBindVertexArray(VAO[7]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[7]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_circleBlueIn), vertices_circleBlueIn, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[7]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_circleBlueIn), indices_circleBlueIn, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glGenVertexArrays(1, &VAO[8]);
    glGenBuffers(1, &VBO[8]);
    glGenBuffers(1, &EBO[8]);
    glBindVertexArray(VAO[8]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[8]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_red), vertices_red, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[8]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_red), indices_red, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glGenVertexArrays(1, &VAO[9]);
    glGenBuffers(1, &VBO[9]);
    glGenBuffers(1, &EBO[9]);
    glBindVertexArray(VAO[9]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[9]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_red_tail), vertices_red_tail, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[9]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_red_tail), indices_red_tail, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // ----------
    //float angle;
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);
        ShaderNormal.use();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // render
        // ------
        drawBackground(ShaderNormal, VAO);
        int vertexColorLocation = glGetUniformLocation(ShaderNormal.ID, "ourColor");
        glBindVertexArray(VAO[6]);
        glUniform4f(vertexColorLocation, 0.38f, 0.69f, 0.72f, 1.0f);
        glDrawElements(GL_TRIANGLES, 149, GL_UNSIGNED_INT, 0);
        glBindVertexArray(VAO[7]);
        glUniform4f(vertexColorLocation, 0.3f, 0.51f, 0.6f, 1.0f);
        glDrawElements(GL_TRIANGLES, 149, GL_UNSIGNED_INT, 0);

        float angle;
        glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        unsigned int transformLoc = glGetUniformLocation(ShaderNormal.ID, "transform");
        for (size_t i = 0; i < 4; i++)
        {
            angle = 90.0 * (float)i;
            transform = glm::rotate(transform, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
            glBindVertexArray(VAO[9]);
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
            glUniform4f(vertexColorLocation, 1.0f, 0.0f, 0.0f, 1.0f);
            glDrawElements(GL_TRIANGLES, 192, GL_UNSIGNED_INT, 0);
            glBindVertexArray(VAO[8]);
            glUniform4f(vertexColorLocation, 1.0f, 0.0f, 0.0f, 1.0f);
            glDrawElements(GL_TRIANGLES, 149, GL_UNSIGNED_INT, 0);
            transform = glm::scale(transform, glm::vec3(-1.0, 1.0, 0.0));
            glBindVertexArray(VAO[9]);
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
            glUniform4f(vertexColorLocation, 1.0f, 0.0f, 0.0f, 1.0f);
            glDrawElements(GL_TRIANGLES, 192, GL_UNSIGNED_INT, 0);
            glBindVertexArray(VAO[8]);
            glUniform4f(vertexColorLocation, 1.0f, 0.0f, 0.0f, 1.0f);
            glDrawElements(GL_TRIANGLES, 149, GL_UNSIGNED_INT, 0);
            transform = glm::mat4(1.0f);//regresamos a la matriz identidad
        };

        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO[1]);
    glDeleteBuffers(1, &VBO[1]);
    glDeleteBuffers(1, &EBO[1]);
    glDeleteVertexArrays(1, &VAO[2]);
    glDeleteBuffers(1, &VBO[2]);
    glDeleteBuffers(1, &EBO[2]);
    glDeleteProgram(ShaderNormal.ID);


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
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void create_buffers(unsigned int vao, unsigned int vbo, unsigned int ebo, float vertices[], unsigned int indices[]) {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
void create_circle(float ver[], unsigned int ind[], float r, float x, float y) {
    ver[0] = x;
    ver[1] = y;
    ver[2] = 0.0f;
    for (int j = 1; j < 50; j++)//calculate 49 points to make a circle
    {
        ver[j * 3] = x + (r * cos(j * 3.1416 * 2 / (49)));
        ver[j * 3 + 1] = y + (r * sin(j * 3.1416 * 2 / (49)));
        ver[j * 3 + 2] = 0.0f;
    };
    for (int k = 0; k < 49; k++)//similar to a fan triangle.
    {
        ind[3 * k] = 0;
        ind[3 * k + 1] = k + 1;
        ind[3 * k + 2] = k + 2;
    }
    ind[144] = 0;
    ind[145] = 49;
    ind[146] = 1;
}

void drawBackground(Shader shader, unsigned int VAO[]) {
    float angle;
    glBindVertexArray(VAO[4]); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    unsigned int transformLoc = glGetUniformLocation(shader.ID, "transform");
    int vertexColorLocation = glGetUniformLocation(shader.ID, "ourColor");
    for (size_t i = 0; i < 4; i++)
    {
        angle = 90.0 * (float)i;
        transform = glm::rotate(transform, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
        glBindVertexArray(VAO[1]);
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
        glUniform4f(vertexColorLocation, 0.0f, 0.0f, 0.0f, 1.0f);
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
    };
    for (size_t i = 0; i < 4; i++)
    {
        angle = 90.0 * (float)i;
        transform = glm::rotate(transform, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));

        glBindVertexArray(VAO[4]);
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
        glUniform4f(vertexColorLocation, 0.0f, 0.0f, 0.0f, 1.0f);
        glDrawElements(GL_TRIANGLES, 149, GL_UNSIGNED_INT, 0);
    };
    for (size_t i = 0; i < 4; i++)
    {
        angle = 90.0 * (float)i;
        transform = glm::rotate(transform, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
        glBindVertexArray(VAO[0]);
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
        glUniform4f(vertexColorLocation, 1.0f, 1.0f, 1.0f, 1.0f);
        glDrawElements(GL_TRIANGLES, 149, GL_UNSIGNED_INT, 0);
    };
    for (size_t i = 0; i < 4; i++)
    {
        angle = 90.0 * (float)i;
        transform = glm::rotate(transform, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
        glBindVertexArray(VAO[2]);
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
        glUniform4f(vertexColorLocation, 1.0f, 1.0f, 1.0f, 1.0f);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    };
    for (size_t i = 0; i < 4; i++)
    {
        angle = 90.0 * (float)i;
        transform = glm::rotate(transform, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
        glBindVertexArray(VAO[5]);
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
        glUniform4f(vertexColorLocation, 0.82f, 0.69f, 0.63f, 1.0f);
        glDrawElements(GL_TRIANGLES, 149, GL_UNSIGNED_INT, 0);
    };

    for (size_t i = 0; i < 4; i++)
    {
        angle = 90.0 * (float)i;
        transform = glm::rotate(transform, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
        glBindVertexArray(VAO[3]);
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
        glUniform4f(vertexColorLocation, 0.82f, 0.69f, 0.63f, 1.0f);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    };

}
