#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <array>
#include <iostream>

unsigned int buildShaderProgram();

void rectangle(
    unsigned int VAO,
    std::array<float, 3> const &topleft,
    std::array<float, 3> const &topright,
    std::array<float, 3> const &bottomleft,
    std::array<float, 3> const &bottomright
);

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

    unsigned int shaderProgram, VAOs[4];

    shaderProgram = buildShaderProgram();

    glGenVertexArrays(4, VAOs);

    rectangle(VAOs[0], { -0.4f, 1.0f, 0.0f }, { -0.3f, 1.0f, 0.0f }, { -0.4f, -1.0f, 0.0f }, { -0.3f, -1.0f, 0.0f });
    rectangle(VAOs[1], { 0.3f, 1.0f, 0.0f }, { 0.4f, 1.0f, 0.0f }, { 0.3f, -1.0f, 0.0f }, { 0.4f, -1.0f, 0.0f });
    rectangle(VAOs[2], { -1.0, 0.4f, 0.0f }, { 1.0f, 0.4f, 0.0f }, { -1.0f, 0.3f, 0.0f }, { 1.0f, 0.3f, 0.0f });
    rectangle(VAOs[3], { -1.0, -0.3f, 0.0f }, { 1.0f, -0.3f, 0.0f }, { -1.0f, -0.4f, 0.0f }, { 1.0f, -0.4f, 0.0f });

    while(!glfwWindowShouldClose(window))
    {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glBindVertexArray(VAOs[0]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindVertexArray(VAOs[1]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindVertexArray(VAOs[2]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindVertexArray(VAOs[3]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(4, VAOs);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\n\0";

const char *fragtmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);\n"
    "}\n\0";

unsigned int buildVertexShader()
{
    unsigned int shader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(shader, 1, &vertexShaderSource, NULL);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    return shader;
}

unsigned int buildFragmentShader()
{
    unsigned int shader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(shader, 1, &fragtmentShaderSource, NULL);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    return shader;
}

unsigned int buildShaderProgram()
{
    unsigned int vertexShader, fragmentShader, program;

    vertexShader = buildVertexShader();
    fragmentShader = buildFragmentShader();
    program = glCreateProgram();

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

void rectangle(
    unsigned int VAO,
    std::array<float, 3> const &topleft,
    std::array<float, 3> const &topright,
    std::array<float, 3> const &bottomleft,
    std::array<float, 3> const &bottomright
) {
    float vertices[4 * 3];

    std::copy(topleft.data(), topleft.data() + 3, vertices);
    std::copy(topright.data(), topright.data() + 3, vertices + 3);
    std::copy(bottomleft.data(), bottomleft.data() + 3, vertices + 6);
    std::copy(bottomright.data(), bottomright.data() + 3, vertices + 9);

    unsigned int indices[] = {
        0, 1, 2,
        1, 2, 3
    };

    unsigned int VBO, EBO;

    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

static void glfwError(int id, const char* description)
{
    std::cout << description << std::endl;
}
