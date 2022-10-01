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

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;


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
    Shader ShaderTexture("4.1.texture.vs", "4.1.texture.fs");
    Shader ShaderNormal("shaderNormal.vs", "shaderNormal.fs");
    

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
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
    float vertices[] = {
        1.0f, 1.0f, 0.0f,//A 0
        0.0f, 1.0f, 0.0f,//B 1
        1.0f, 0.0f, 0.0f,//C 2 
        0.0f, 0.0f, 0.0f,//D 3
        0.7f, 0.7f, 0.0f,//E 4
        1.0f, 0.7f, 0.0f,//F 5
        0.7f, 1.0f, 0.0f,//G 6
        0.3f, 0.3f, 0.0f,//H 7
        0.2f, 0.2f, 0.0f,//I  8
        0.0f, 0.45f, 0.0f,//J 9
        0.45f, 0.0f, 0.0f,//K 10
        0.3f, 0.7f, 0.0f,//L 11
        0.7f, 0.3f, 0.0f,//M 12
        0.7f, 0.0f, 0.0f,//N 13
        0.0f, 0.7f, 0.0f,//O 14
        0.6f, 0.0f, 0.0f,//P 15
        0.0f, 0.6f, 0.0f,//Q 16

    };

    unsigned int indices[] = {  // note that we start from 0!
        0,4,5,
        0,4,6,
        2,4,5,
        1,4,6,
        7,12,13,
        7,11,14
    };

    float vertices_circle[150];
    unsigned int indices_circle[147];
    vertices_circle[0] = 0.2f;
    vertices_circle[1] = 0.2f;
    vertices_circle[2] = 0.0f;
    for (int j = 1; j < 50; j++)
    {
        vertices_circle[j * 3] = 0.2f + (0.16 * cos(j * 3.1416 * 2 / 49));
        vertices_circle[j * 3 + 1] = 0.2f + (0.16 * sin(j * 3.1416 * 2 / 49));
        vertices_circle[j * 3 + 2] = 0.0f;
    };

    for (int k = 0; k < 49; k++)
    {
        indices_circle[3 * k] = 0;
        indices_circle[3 * k + 1] = k + 1;
        indices_circle[3 * k + 2] = k + 2;
    }
    indices_circle[144] = 0;
    indices_circle[145] = 49;
    indices_circle[146] = 1;

    float vertices_ellipse1[150];
    float vertices_ellipse2[150];
    float x, y, pi;
    pi = 3.1416;
    vertices_ellipse1[0] = 0.45f;
    vertices_ellipse1[1] = 0.0f;
    vertices_ellipse1[2] = 0.0f;
    vertices_ellipse2[0] = 0.0f;
    vertices_ellipse2[1] = 0.45f;
    vertices_ellipse2[2] = 0.0f;
    for (int j = 1; j < 50; j++)
    {
        x = 0.45f + (0.55 * cos(j * pi * 2 / 160));
        y = 0.0f + (0.2 * sin(j * pi * 2 / 160));
        vertices_ellipse1[j * 3] = x;
        vertices_ellipse1[j * 3 + 1] = y;
        vertices_ellipse1[j * 3 + 2] = 0.0f;

        vertices_ellipse2[j * 3] = y;
        vertices_ellipse2[j * 3 + 1] = x;
        vertices_ellipse2[j * 3 + 2] = 0.0f;
    };

    unsigned int VAO[5], VBO[5], EBO[5];
    glGenVertexArrays(1, &VAO[1]);
    glGenBuffers(1, &VBO[1]);
    glGenBuffers(1, &EBO[1]);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO[1]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    glGenVertexArrays(1, &VAO[2]);
    glGenBuffers(1, &VBO[2]);
    glGenBuffers(1, &EBO[2]);
    glBindVertexArray(VAO[2]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_circle), vertices_circle, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[2]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_circle), indices_circle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glGenVertexArrays(1, &VAO[3]);
    glGenBuffers(1, &VBO[3]);
    glGenBuffers(1, &EBO[3]);
    glBindVertexArray(VAO[3]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_ellipse1), vertices_ellipse1, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_circle), indices_circle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glGenVertexArrays(1, &VAO[4]);
    glGenBuffers(1, &VBO[4]);
    glGenBuffers(1, &EBO[4]);
    glBindVertexArray(VAO[4]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_ellipse2), vertices_ellipse2, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[4]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_circle), indices_circle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glGenVertexArrays(1, &VAO[0]);
    glGenBuffers(1, &VBO[0]);
    glGenBuffers(1, &EBO[0]);
    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesTexture), verticesTexture, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
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
    unsigned char* data = stbi_load("C:/Graficas/OpenGL/images/textura1.png", &width, &height, &nrChannels, 0);
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

    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // ----------
    float angle;
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ShaderTexture.use();
        glBindVertexArray(VAO[0]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        ShaderNormal.use();
        glBindVertexArray(VAO[1]); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glm::mat4 transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        unsigned int transformLoc = glGetUniformLocation(ShaderNormal.ID, "transform");
        int vertexColorLocation = glGetUniformLocation(ShaderNormal.ID, "ourColor");
        //Pintamos un cuadrante y lo rotamos para que se repita en los 4
        for (size_t i = 0; i < 4; i++)
        {
            angle = 90.0 * (float)i;
            transform = glm::rotate(transform, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));

            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
            glUniform4f(vertexColorLocation, 0.0f, 1.0f, 0.0f, 1.0f);
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
            glUniform4f(vertexColorLocation, 1.0f, 0.0f, 0.0f, 1.0f);
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(3 * sizeof(unsigned int)));
            glUniform4f(vertexColorLocation, 1.0f, 1.0f, 0.0f, 1.0f);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(6 * sizeof(unsigned int)));
            glUniform4f(vertexColorLocation, 1.0f, 0.1f, 1.0f, 1.0f);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(12 * sizeof(unsigned int)));
        };
        glBindVertexArray(0);
        for (size_t m = 0; m < 5; m++)
        {
            glBindVertexArray(VAO[3]);
            angle = 90.0 * (float)m;
            transform = glm::rotate(transform, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

            glUniform4f(vertexColorLocation, 0.8f, 0.4f, 0.7f, 1.0f);
            glDrawElements(GL_TRIANGLES, 147, GL_UNSIGNED_INT, 0);
            transform = glm::rotate(transform, glm::radians(45.0f), glm::vec3(0.0, 0.0, 1.0));
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
            glDrawElements(GL_TRIANGLES, 147, GL_UNSIGNED_INT, 0);

            glBindVertexArray(0);
            glBindVertexArray(VAO[4]);
            glUniform4f(vertexColorLocation, 0.8f, 0.3f, 0.5f, 1.0f);
            glDrawElements(GL_TRIANGLES, 147, GL_UNSIGNED_INT, 0);
            transform = glm::rotate(transform, glm::radians(45.0f), glm::vec3(0.0, 0.0, 1.0));
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
            glDrawElements(GL_TRIANGLES, 147, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
            transform = glm::mat4(1.0f);
        };


        glBindVertexArray(VAO[2]);
        for (size_t l = 0; l < 4; l++)
        {
            angle = 90.0 * (float)l;
            transform = glm::rotate(transform, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));


            glUniform4f(vertexColorLocation, 0.5f, 0.1f, 1.0f, 1.0f);
            glDrawElements(GL_TRIANGLES, 147, GL_UNSIGNED_INT, 0);
            transform = glm::translate(transform, glm::vec3(-0.2f, 0.25f, 0.0f));
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
            glUniform4f(vertexColorLocation, 0.5f, 0.1f, 0.0f, 1.0f);
            glDrawElements(GL_TRIANGLES, 147, GL_UNSIGNED_INT, 0);
            transform = glm::translate(transform, glm::vec3(0.2f, -0.25f, 0.0f));
            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

            transform = glm::mat4(1.0f);
        };

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
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
    glDeleteProgram(ShaderTexture.ID);


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
