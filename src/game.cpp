#include "game.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

static void glfwError(int id, const char* description);

Game::Game()
{
    this->shader = Shader();
    this->shader.Compile();
}
