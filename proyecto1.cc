#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <stb_image.h>
#include <shader_s.h>
#include <ctime>
#include <sys\timeb.h> 

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void create_circle(float ver[], unsigned int ind[], float r, float x, float y);
void drawBackground(int vertexColorLocation, unsigned int transformLoc, unsigned int VAO[]);
void drawLeafs(int vertexColorLocation, unsigned int transformLoc, unsigned int VAO[], float colores[], int state, float inc);
void drawCircles(int vertexColorLocation, unsigned int transformLoc, unsigned int VAO[], float colores[]);
void drawWood(int vertexColorLocation, unsigned int transformLoc, unsigned int VAO[]);
void moveCircles(int vertexColorLocation, unsigned int transformLoc, unsigned int VAO[], float colores[], float inc, int state);
void moveBackCircles(int vertexColorLocation, unsigned int transformLoc, unsigned int VAO[], float colores[], float inc, int state);// settings
void getWater(int vertexColorLocation, unsigned int transformLoc, unsigned int VAO[], float colores[],int state);
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;
unsigned int VAO[10], VBO[10], EBO[10];

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
    Shader ShaderTexture("4.1.texture.vs", "4.1.texture.fs");
    float verticesTexture[] = {
        // positions          // colors           // texture coords
         1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 

    };
    unsigned int indicesTexture[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float verticesBg[] = {
        0.0f, 0.0f, 0.0f, //0
        1.5f, 0.0f, 0.0f,
        0.0f, 1.5f, 0.0f, //2
        0.7f, 0.7f, 0.0f, //3
        0.7f, 0.0f, 0.0f, //4
        0.0f, 0.0f, 0.0f, //5
        0.0f, 0.7f, 0.0f,//6
        0.6f, 0.6f, 0.0f,//7
        0.6f, 0.0f, 0.0f,//8  
        0.0f, 0.6f, 0.0f,//9
        0.5f, 0.5f, 0.0f,//10
        0.5f, 0.0f, 0.0f,//11
        0.0f, 0.5f, 0.0f,//12
        0.66f,0.7f,0.0f,
        0.7f,0.66f,0.0f
    };
    unsigned int indicesBg[] = {  // note that we start from 0!
        0, 1, 2,
        3, 4, 5,
        3, 5, 6,
        5, 7, 8,
        5, 7, 9,
        5, 10, 11,
        5, 10, 12,
        3,13,14,
    };

    float verticesSpikes[] = {
        0.0f, 0.17f, 0.0f, //0
        0.17f, 0.0f, 0.0f, //1
        0.3f, 0.3f, 0.0f,

        0.0f, -0.17f, 0.0f, //3
        0.17f, 0.0f, 0.0f, //4
        0.3f, -0.3f, 0.0f,

        0.0f, -0.17f, 0.0f, //6
        -0.17f, 0.0f, 0.0f, //7
        -0.3f, -0.3f, 0.0f,

        0.0f, 0.17f, 0.0f, //9
        -0.17f, 0.0f, 0.0f, //10
        -0.3f, 0.3f, 0.0f,
    };
    unsigned int indicesSpikes[] = {  // note that we start from 0!
        0, 1, 2,
        3, 4, 5,
        6, 7, 8,
        9, 10, 11
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
    create_circle(vertices_circleBlueIn, indices_circleBlueIn, 0.2, 0.0, 0.0);
    create_circle(vertices_circleBlueOut, indices_circleBlueOut, 0.25, 0.0, 0.0);
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
        indices_red_tail[(x + 1) * 3] = x;
        indices_red_tail[(x + 1) * 3 + 1] = x + 1;
        indices_red_tail[(x + 1) * 3 + 2] = x + 2;
    }
    indices_red_tail[0] = 69;
    indices_red_tail[1] = 70;
    indices_red_tail[2] = 71;

    glGenVertexArrays(1, &VAO[1]);
    glGenBuffers(1, &VBO[1]);
    glGenBuffers(1, &EBO[1]);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO[1]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesBg), verticesBg, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesBg), indicesBg, GL_STATIC_DRAW);

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
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_circleBlack), vertices_circleBlack, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[2]);
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
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_circleWhite), indices_circleWhite, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glGenVertexArrays(1, &VAO[3]);
    glGenBuffers(1, &VBO[3]);
    glGenBuffers(1, &EBO[3]);
    glBindVertexArray(VAO[3]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_circleYellow), vertices_circleYellow, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_circleYellow), indices_circleYellow, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glGenVertexArrays(1, &VAO[4]);
    glGenBuffers(1, &VBO[4]);
    glGenBuffers(1, &EBO[4]);
    glBindVertexArray(VAO[4]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_circleBlueOut), vertices_circleBlueOut, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[4]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_circleBlueOut), indices_circleBlueOut, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glGenVertexArrays(1, &VAO[5]);
    glGenBuffers(1, &VBO[5]);
    glGenBuffers(1, &EBO[5]);
    glBindVertexArray(VAO[5]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[5]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_circleBlueIn), vertices_circleBlueIn, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[5]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_circleBlueIn), indices_circleBlueIn, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glGenVertexArrays(1, &VAO[6]);
    glGenBuffers(1, &VBO[6]);
    glGenBuffers(1, &EBO[6]);
    glBindVertexArray(VAO[6]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[6]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_red), vertices_red, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[6]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_red), indices_red, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glGenVertexArrays(1, &VAO[7]);
    glGenBuffers(1, &VBO[7]);
    glGenBuffers(1, &EBO[7]);
    glBindVertexArray(VAO[7]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[7]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_red_tail), vertices_red_tail, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[7]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_red_tail), indices_red_tail, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glGenVertexArrays(1, &VAO[8]);
    glGenBuffers(1, &VBO[8]);
    glGenBuffers(1, &EBO[8]);
    glBindVertexArray(VAO[8]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[8]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesSpikes), verticesSpikes, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[8]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesSpikes), indicesSpikes, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glGenVertexArrays(1, &VAO[9]);
    glGenBuffers(1, &VBO[9]);
    glGenBuffers(1, &EBO[9]);
    glBindVertexArray(VAO[9]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[9]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesTexture), verticesTexture, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[9]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesTexture), indicesTexture, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    // load and create a texture 
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
        // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set  texture wrapping to GL_REPEAT(default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    unsigned char* data = stbi_load("C:/Graficas/OpenGL/images/water.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
            GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);    glBindVertexArray(0);


    // render loop
    // ----------

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    int vertexColorLocation = glGetUniformLocation(ShaderNormal.ID, "ourColor");
    unsigned int transformLoc = glGetUniformLocation(ShaderNormal.ID, "transform");
    struct timeb start, end;
    unsigned short elapse = 0, t1, t2;
    float inc = 0;
    int state = 0;
    ftime(&start);
    t1 = start.millitm;
    float color_hoja[] = {
        0.0f,1.0f,0.0f,1.0f,
    };
    float color_agua[] = {
        1.0f,1.0f,1.0f,1.0f,
    };
    glm::mat4 trans = glm::mat4(1.0f);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        ftime(&end);
        t2 = end.millitm;
        elapse = t2 - t1;
        if (elapse > 1) {
            t1 = t2;
            inc = inc + 0.1; //cada que el incremento sube 50 son 8 segundos
            glClearColor(1.0f, 0.898f, 0.8f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            ShaderTexture.use();
            glBindVertexArray(VAO[9]);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            ShaderNormal.use();
            // render
            // ------
            drawBackground(vertexColorLocation, transformLoc, VAO);
            if (inc < 16.5) { 
                color_hoja[0] = color_hoja[0] + 0.006;
                drawCircles(vertexColorLocation, transformLoc, VAO, color_agua);
                drawWood(vertexColorLocation, transformLoc, VAO);
                drawLeafs(vertexColorLocation, transformLoc, VAO, color_hoja, state, inc);
            }
            else if (inc < 33) {
                color_hoja[1] = color_hoja[1] - 0.006;
                drawCircles(vertexColorLocation, transformLoc, VAO, color_agua);
                drawWood(vertexColorLocation, transformLoc, VAO);
                drawLeafs(vertexColorLocation, transformLoc, VAO, color_hoja, state, inc);
            }
            else if (inc < 50) {
                state = 1;
                color_hoja[3] = color_hoja[3] - 0.006;
                drawCircles(vertexColorLocation, transformLoc, VAO, color_agua);
                drawWood(vertexColorLocation, transformLoc, VAO);
                drawLeafs(vertexColorLocation, transformLoc, VAO, color_hoja, state, inc);
            }
            else if (inc < 85) {
                drawWood(vertexColorLocation, transformLoc, VAO);
                moveCircles(vertexColorLocation, transformLoc, VAO, color_agua, inc,state);
            }
            
            else if (inc < 120) {
                color_agua[0] = color_agua[0] - 0.003;
                drawWood(vertexColorLocation, transformLoc, VAO);
                getWater(vertexColorLocation, transformLoc, VAO, color_agua,state);
            }
            else if (inc < 155) {
                drawWood(vertexColorLocation, transformLoc, VAO);
                moveBackCircles(vertexColorLocation, transformLoc, VAO, color_agua, inc,state);
            }
            else if (inc < 160) {
                state = 2;
                drawWood(vertexColorLocation, transformLoc, VAO);
                moveCircles(vertexColorLocation, transformLoc, VAO, color_agua, inc,state);
            }
            else if (inc < 185) {
                state = 2;
                color_hoja[0] = 0.0f;
                color_hoja[1] = 1.0f;
                color_hoja[3] = 1.0f;
                drawWood(vertexColorLocation, transformLoc, VAO);
                drawLeafs(vertexColorLocation, transformLoc, VAO, color_hoja, state, inc);
                color_agua[0] = color_agua[0] + 0.001;
                getWater(vertexColorLocation, transformLoc, VAO, color_agua,state);
            }
            else if (inc < 195) {
                state = 3;
                drawWood(vertexColorLocation, transformLoc, VAO);
                moveBackCircles(vertexColorLocation, transformLoc, VAO, color_agua, inc, state);
                drawLeafs(vertexColorLocation, transformLoc, VAO, color_hoja, state, inc);
            }
            else if (inc < 210) {
                state = 4;
                drawWood(vertexColorLocation, transformLoc, VAO);
                drawLeafs(vertexColorLocation, transformLoc, VAO, color_hoja, state, inc);
                color_agua[0] = color_agua[0] + 0.001;
                getWater(vertexColorLocation, transformLoc, VAO, color_agua, state);
            }
            else if (inc < 220) {
                state = 5;
                drawWood(vertexColorLocation, transformLoc, VAO);
                moveBackCircles(vertexColorLocation, transformLoc, VAO, color_agua, inc, state);
                drawLeafs(vertexColorLocation, transformLoc, VAO, color_hoja, state, inc);
            }
            else if (inc < 235) {
                state = 6;
                drawWood(vertexColorLocation, transformLoc, VAO);
                drawLeafs(vertexColorLocation, transformLoc, VAO, color_hoja, state, inc);
                color_agua[0] = color_agua[0] + 0.001;
                getWater(vertexColorLocation, transformLoc, VAO, color_agua, state);
            }
            else if (inc < 245) {
                state = 7;
                drawWood(vertexColorLocation, transformLoc, VAO);
                moveBackCircles(vertexColorLocation, transformLoc, VAO, color_agua, inc, state);
                drawLeafs(vertexColorLocation, transformLoc, VAO, color_hoja, state, inc);
            }
            else if (inc < 260) {
                state = 8;
                drawWood(vertexColorLocation, transformLoc, VAO);
                drawLeafs(vertexColorLocation, transformLoc, VAO, color_hoja, state, inc);
                color_agua[0] = color_agua[0] + 0.001;
                getWater(vertexColorLocation, transformLoc, VAO, color_agua, state);
            }
            else {
                state = 0;
                drawWood(vertexColorLocation, transformLoc, VAO);
                drawLeafs(vertexColorLocation, transformLoc, VAO, color_hoja, state, inc);
                drawCircles(vertexColorLocation, transformLoc, VAO, color_agua);
            }
            
            
            // -------------------------------------------------------------------------------
            glfwSwapBuffers(window);

        }
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

void drawBackground(int vertexColorLocation, unsigned int transformLoc, unsigned int VAO[]) {
    float angle;
    glBindVertexArray(VAO[2]); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    for (size_t i = 0; i < 4; i++)
    {
        angle = 90.0 * (float)i;
        transform = glm::rotate(transform, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
        glBindVertexArray(VAO[1]);
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
        glUniform4f(vertexColorLocation, 1.0f, 1.0f, 1.0f, 1.0f);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        glUniform4f(vertexColorLocation, 0.0f, 0.0f, 0.0f, 1.0f);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(3 * sizeof(unsigned int)));
        glUniform4f(vertexColorLocation, 1.0f, 1.0f, 1.0f, 1.0f);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(21 * sizeof(unsigned int)));
    };
    for (size_t i = 0; i < 4; i++)
    {
        angle = 90.0 * (float)i;
        transform = glm::rotate(transform, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));

        glBindVertexArray(VAO[2]);
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
        glBindVertexArray(VAO[1]);
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
        glUniform4f(vertexColorLocation, 1.0f, 1.0f, 1.0f, 1.0f);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(9 * sizeof(unsigned int)));
    };
    for (size_t i = 0; i < 4; i++)
    {
        angle = 90.0 * (float)i;
        transform = glm::rotate(transform, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
        glBindVertexArray(VAO[3]);
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
        glUniform4f(vertexColorLocation, 0.82f, 0.69f, 0.63f, 1.0f);
        glDrawElements(GL_TRIANGLES, 149, GL_UNSIGNED_INT, 0);
    };

    for (size_t i = 0; i < 4; i++)
    {
        angle = 90.0 * (float)i;
        transform = glm::rotate(transform, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
        glBindVertexArray(VAO[1]);
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
        glUniform4f(vertexColorLocation, 0.82f, 0.69f, 0.63f, 1.0f);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(15 * sizeof(unsigned int)));
    };


}

void drawLeafs(int vertexColorLocation, unsigned int transformLoc, unsigned int VAO[], float colores[], int state, float inc) {
    float angle;
    glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glBindVertexArray(VAO[6]);
    glUniform4f(vertexColorLocation, colores[0], colores[1], colores[2], colores[3]);
    for (size_t i = 0; i < 4; i++)
    {
        transform = glm::mat4(1.0f);
        angle = 90.0 * (float)i;
        switch (i)
        {
        case 0:
            if (state == 0) {
                break;
            }
            if (state == 1) {
                transform = glm::translate(transform, glm::vec3((inc / 250) - 0.1, 0.0f, 0.0f));
            }
            else if (state < 8) {
                transform = glm::scale(transform, glm::vec3(0.0f, 0.0f, 0.0f));
            }
            if (state == 8) {
                transform = glm::translate(transform, glm::vec3(0.56f, 0.07f, 0.0f));
                transform = glm::scale(transform, glm::vec3((inc / 25) - 9.8, (inc / 25) - 9.8, 0.0f));
                transform = glm::translate(transform, glm::vec3(-0.56f, -0.07f, 0.0f));
            }
            break;
        case 1:
            if (state == 0) {
                break;
            }
            if (state == 1) {
                transform = glm::translate(transform, glm::vec3(0.0f, (inc / 250) - 0.1, 0.0f));
            }
            if (state == 2) {
                transform = glm::translate(transform, glm::vec3(-0.07f, 0.56f, 0.0f));
                transform = glm::scale(transform, glm::vec3((inc / 25) - 6.4, (inc / 25) - 6.4, 0.0f));
                transform = glm::translate(transform, glm::vec3(0.07f, -0.56f, 0.0f));
            }
            break;
        case 2:
            if (state == 0) {
                break;
            }
            if (state == 1) {
                transform = glm::translate(transform, glm::vec3((-inc / 250) + 0.1, 0.0f, 0.0f));
            }
            else if (state < 4) {
                transform = glm::scale(transform, glm::vec3(0.0f, 0.0f, 0.0f));
            }
            if (state == 4) {
                transform = glm::translate(transform, glm::vec3(-0.56f, -0.07f, 0.0f));
                transform = glm::scale(transform, glm::vec3((inc / 25) - 7.8, (inc / 25) - 7.8, 0.0f));
                transform = glm::translate(transform, glm::vec3(0.56f, 0.07f, 0.0f));
            }

            break;
        case 3:
            if (state == 0) {
                break;
            }
            if (state == 1) {
                transform = glm::translate(transform, glm::vec3(0.0f, (-inc / 250) + 0.1, 0.0f));
            }
            else if (state < 6) {
                transform = glm::scale(transform, glm::vec3(0.0f, 0.0f, 0.0f));
            }
            if (state == 6) {
                transform = glm::translate(transform, glm::vec3(0.07f, -0.56f, 0.0f));
                transform = glm::scale(transform, glm::vec3((inc / 25) - 8.8, (inc / 25) - 8.8, 0.0f));
                transform = glm::translate(transform, glm::vec3(-0.07f, 0.56f, 0.0f));
            }
            break;
        default:
            break;
        }
        transform = glm::rotate(transform, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
        glDrawElements(GL_TRIANGLES, 149, GL_UNSIGNED_INT, 0);
        transform = glm::mat4(1.0f);
        switch (i)
        {
        case 0:
            if (state == 0) {
                break;
            }
            if (state == 1) {
                transform = glm::translate(transform, glm::vec3((-inc / 250) + 0.1, 0.0f, 0.0f));
            }
            if (state == 2) {
                transform = glm::translate(transform, glm::vec3(-0.56f, 0.07f, 0.0f));
                transform = glm::scale(transform, glm::vec3((inc / 25) - 6.4, (inc / 25) - 6.4, 0.0f));
                transform = glm::translate(transform, glm::vec3(0.56f, -0.07f, 0.0f));
            }
            break;
        case 1:
            if (state == 0) {
                break;
            }
            if (state == 1) {
                transform = glm::translate(transform, glm::vec3(0.0f, (-inc / 250) + 0.1, 0.0f));
            }
            else if (state < 4) {
                transform = glm::scale(transform, glm::vec3(0.0f, 0.0f, 0.0f));
            }
            if (state == 4) {
                transform = glm::translate(transform, glm::vec3(-0.07f, -0.56f, 0.0f));
                transform = glm::scale(transform, glm::vec3((inc / 25) - 7.8, (inc / 25) - 7.8, 0.0f));
                transform = glm::translate(transform, glm::vec3(0.07f, 0.56f, 0.0f));
            }
            break;
        case 2:
            if (state == 0) {
                break;
            }
            if (state == 1) {
                transform = glm::translate(transform, glm::vec3((inc / 250) - 0.1, 0.0f, 0.0f));
            }
            else if (state < 6) {
                transform = glm::scale(transform, glm::vec3(0.0f, 0.0f, 0.0f));
            }
            if (state == 6) {
                transform = glm::translate(transform, glm::vec3(0.56f, -0.07f, 0.0f));
                transform = glm::scale(transform, glm::vec3((inc / 25) - 8.8, (inc / 25) - 8.8, 0.0f));
                transform = glm::translate(transform, glm::vec3(-0.56f, 0.07f, 0.0f));
            }

            break;
        case 3:
            if (state == 0) {
                break;
            }
            if (state == 1) {
                transform = glm::translate(transform, glm::vec3(0.0f, (inc / 250) - 0.1, 0.0f));
            }
            else if (state < 8) {
                transform = glm::scale(transform, glm::vec3(0.0f, 0.0f, 0.0f));
            }
            if (state == 8) {
                transform = glm::translate(transform, glm::vec3(0.07f, 0.56f, 0.0f));
                transform = glm::scale(transform, glm::vec3((inc / 25) - 9.8, (inc / 25) - 9.8, 0.0f));
                transform = glm::translate(transform, glm::vec3(-0.07f, -0.56f, 0.0f));
            }
            break;
        default:
            break;
        }
        transform = glm::rotate(transform, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
        transform = glm::scale(transform, glm::vec3(-1.0, 1.0, 0.0));
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
        glDrawElements(GL_TRIANGLES, 149, GL_UNSIGNED_INT, 0);

    };
}

void drawWood(int vertexColorLocation, unsigned int transformLoc, unsigned int VAO[]) {
    float angle;
    glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glBindVertexArray(VAO[7]);
    for (size_t i = 0; i < 4; i++)
    {
        angle = 90.0 * (float)i;
        transform = glm::rotate(transform, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
        glUniform4f(vertexColorLocation, 0.525, 0.16, 0.047, 1.0f);//color cafe 
        glDrawElements(GL_TRIANGLES, 192, GL_UNSIGNED_INT, 0);
        transform = glm::scale(transform, glm::vec3(-1.0, 1.0, 0.0));
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
        glDrawElements(GL_TRIANGLES, 192, GL_UNSIGNED_INT, 0);
    };
}

void drawCircles(int vertexColorLocation, unsigned int transformLoc, unsigned int VAO[], float colores[]) {
    float angle;
    glm::mat4 transform = glm::mat4(1.0f);
    for (size_t i = 0; i < 4; i++)
    {
        angle = 90.0 * (float)i;
        transform = glm::rotate(transform, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
        glBindVertexArray(VAO[8]);
        glUniform4f(vertexColorLocation, 0.0f, 0.0f, 0.0f, 1.0f);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

    };
    glBindVertexArray(VAO[4]);
    glUniform4f(vertexColorLocation, 0.38f, 0.69f, 0.72f, 1.0f);
    glDrawElements(GL_TRIANGLES, 149, GL_UNSIGNED_INT, 0);
    glBindVertexArray(VAO[5]);
    glUniform4f(vertexColorLocation, colores[0], colores[1], colores[2], colores[3]);
    glDrawElements(GL_TRIANGLES, 149, GL_UNSIGNED_INT, 0);
}

void getWater(int vertexColorLocation, unsigned int transformLoc, unsigned int VAO[], float colores[],int state) {

    glm::mat4 transform = glm::mat4(1.0f);
    float coordx, coordy,angle;
    switch (state)
    {
    case 1:
        coordx = 0.7;
        coordy = 0.7;
        angle = 0.0;
        break;
    case 2: 
        coordx = -0.1;
        coordy = 0.1;
        angle = 90.0;
        break;
    case 4:
        coordx = -0.1;
        coordy = -0.1;
        angle = 180.0;
        break;
    case 6:
        coordx = 0.1;
        coordy = -0.1;
        angle = 270.0;
        break;
    case 8:
        coordx = 0.1;
        coordy = 0.1;
        angle = 0.0;
        break;
    default:
        break;
    }
    transform = glm::translate(transform, glm::vec3(coordx, coordy, 0.0f));
    transform = glm::rotate(transform, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
    glBindVertexArray(VAO[8]);
    glUniform4f(vertexColorLocation, 0.0f, 0.0f, 0.0f, 1.0f);
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
    glBindVertexArray(VAO[4]);
    glUniform4f(vertexColorLocation, 0.38f, 0.69f, 0.72f, 1.0f);
    glDrawElements(GL_TRIANGLES, 149, GL_UNSIGNED_INT, 0);

    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
    glBindVertexArray(VAO[5]);
    glUniform4f(vertexColorLocation, colores[0], colores[1], colores[2], colores[3]);
    glDrawElements(GL_TRIANGLES, 149, GL_UNSIGNED_INT, 0);

    glBindVertexArray(VAO[8]);
    glUniform4f(vertexColorLocation, colores[0], colores[1], colores[2], colores[3]);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

    glBindVertexArray(VAO[5]);
    glUniform4f(vertexColorLocation, colores[0], colores[1], colores[2], colores[3]);
    glDrawElements(GL_TRIANGLES, 149, GL_UNSIGNED_INT, 0);
}

void moveBackCircles(int vertexColorLocation, unsigned int transformLoc, unsigned int VAO[], float colores[], float inc,int state) {
    glm::mat4 transform = glm::mat4(1.0f);
    float offset = 3.1;
    float angle = 0.0;
    switch (state)
    {
    case 3:
        offset = 3.8;
        transform = glm::translate(transform, glm::vec3(-0.1f, (-inc / 50) + offset, 0.0f));
        break;
    case 5:
        offset = 4.3;
        transform = glm::translate(transform, glm::vec3((inc / 50) - offset, -0.1f, 0.0f));
        break;
    case 7:
        offset = 4.8;
        transform = glm::translate(transform, glm::vec3(0.1f, (inc / 50) - offset, 0.0f));
        break;
    default:
        transform = glm::translate(transform, glm::vec3((-inc / 50) + offset, (-inc / 50) + offset, 0.0f));
        break;
    }

    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
    glBindVertexArray(VAO[8]);
    glUniform4f(vertexColorLocation, 0.0f, 0.0f, 0.0f, 1.0f);
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
    glBindVertexArray(VAO[4]);
    glUniform4f(vertexColorLocation, 0.38f, 0.69f, 0.72f, 1.0f);
    glDrawElements(GL_TRIANGLES, 149, GL_UNSIGNED_INT, 0);

    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
    glBindVertexArray(VAO[5]);
    glUniform4f(vertexColorLocation, colores[0], colores[1], colores[2], colores[3]);
    glDrawElements(GL_TRIANGLES, 149, GL_UNSIGNED_INT, 0);
}
void moveCircles(int vertexColorLocation, unsigned int transformLoc, unsigned int VAO[], float colores[], float inc, int state) {
    glm::mat4 transform = glm::mat4(1.0f);
    float offset = 1.0;
    switch (state)
    {
    case 2:
        offset = 3.1;
        transform = glm::translate(transform, glm::vec3((-inc / 50) + offset, 0.0f, 0.0f));
        transform = glm::translate(transform, glm::vec3(0.0f, (inc / 50) - offset, 0.0f));
        break;
    default:
        transform = glm::translate(transform, glm::vec3((inc / 50) - offset, 0.0f, 0.0f));
        transform = glm::translate(transform, glm::vec3(0.0f, (inc / 50) - offset, 0.0f));
        break;
    }
    
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
    glBindVertexArray(VAO[8]);
    glUniform4f(vertexColorLocation, 0.0f, 0.0f, 0.0f, 1.0f);
    glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
    glBindVertexArray(VAO[4]);
    glUniform4f(vertexColorLocation, 0.38f, 0.69f, 0.72f, 1.0f);
    glDrawElements(GL_TRIANGLES, 149, GL_UNSIGNED_INT, 0);

    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
    glBindVertexArray(VAO[5]);
    glUniform4f(vertexColorLocation, colores[0], colores[1], colores[2], colores[3]);
    glDrawElements(GL_TRIANGLES, 149, GL_UNSIGNED_INT, 0);
}
