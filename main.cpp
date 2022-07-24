#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void buildShaderProgram(unsigned int *program);

void triangle();

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

static void glfwError(int id, const char* description);

int main()
{
    glfwSetErrorCallback(&glfwError);

    glfwInit();

    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    unsigned int shaderProgram, VAO, VBO;

    buildShaderProgram(&shaderProgram);

    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    triangle();

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    while(!glfwWindowShouldClose(window))
    {
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragtmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\0";

void buildVertexShader(unsigned int *shader)
{
    *shader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(*shader, 1, &vertexShaderSource, NULL);
    glCompileShader(*shader);

    int success;
    char infoLog[512];
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(*shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
}

void buildFragmentShader(unsigned int *shader)
{
    *shader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(*shader, 1, &fragtmentShaderSource, NULL);
    glCompileShader(*shader);

    int success;
    char infoLog[512];
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(*shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
}

void buildShaderProgram(unsigned int *program)
{
    unsigned int vertexShader, fragmentShader;
    buildVertexShader(&vertexShader);
    buildFragmentShader(&fragmentShader);

    *program = glCreateProgram();

    glAttachShader(*program, vertexShader);
    glAttachShader(*program, fragmentShader);
    glLinkProgram(*program);

    int success;
    char infoLog[512];
    glGetProgramiv(*program, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetProgramInfoLog(*program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void triangle()
{
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.00f,
        0.0f, 0.5f, 0.0f
    };

    unsigned int VBO;
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

static void glfwError(int id, const char* description)
{
    std::cout << description << std::endl;
}
