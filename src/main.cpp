#include <string>

#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

void ErrorCallback(int, const char* err_str)
{
	spdlog::error("GLFW Error: {}", err_str);
}

int main(int argc, char *argv[])
{
	if (!glfwInit())
	{
		const char* msg;
		int err = glfwGetError(&msg);
		spdlog::error("Could not initialize glew. Error: " + std::to_string(err) + msg);
		return EXIT_FAILURE;
	}
	glfwSetErrorCallback(ErrorCallback);

	/* config */
	GLFWwindow* window = glfwCreateWindow(1366, 768, "hello world!",
										  nullptr, nullptr);
	if (!window)
	{
		spdlog::error("Could not create window.");
		glfwTerminate();
		return EXIT_FAILURE;
	}
	glfwMakeContextCurrent(window);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return EXIT_SUCCESS;
}
