#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
/*
	framebuffer 帧缓存器
	viewport 视口
	context 上下文
	Rendering 渲染
*/

//init viewport and viewport size changed with window size change
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
//control of input
void processInput(GLFWwindow *window);


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
	//init a window
	glfwInit();
	//set version 3.3 --版本
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//set Core-profile --核心模式
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//instantiate window object
	GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "learnOpengl", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();//end
		return -1;
	}
	//set context
	glfwMakeContextCurrent(window);
	//init viewport
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//glad: load all OpenGl function pointers
	//init GLAD
	//glfwGetProcAddress : load the address of the openGL function that about system 
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//render loop
	//if glfwWindowShouldClose is true exit loop
	while (!glfwWindowShouldClose(window))
	{
		//input 
		processInput(window);

		//Rendering
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Swap color buffer
		//check enents
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
	//press esc button to close window
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}