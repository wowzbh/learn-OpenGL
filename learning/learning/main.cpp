#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
/*
	framebuffer 帧缓存器
	viewport 视口
	context 上下文
	Rendering 渲染
	vertexShader 顶点着色器
	fragmentShader 片段着色器
	VAO(vertex array object): 顶点数组对象
	VBO(vertex buffer object): 顶点缓冲对象
	EOB(element buffer object) 
	&IBO(index budffer object): 索引缓冲对象
*/

//init viewport and viewport size changed with window size change
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
//control of input
void processInput(GLFWwindow *window);


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//set vertexShader source code
const char *vertexShaderSource = "#version 330 core \n"
"layout (location = 0) in vec3 aPos; \n"
"void main() \n"
"{ \n"
"gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0); \n"
"} \0";

//set fragmentShader source code 
const char *fragmentShaderSource = "#version 330 core \n"
"out vec4 FragColor; \n"
"void main() \n"
"{ \n"
"FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f); \n"
"} \0";

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

	//build and compile shader program
	//-------------

	//vertex shader
	//using ID to create vertex shader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//binding source code on shader
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	//compile
	glCompileShader(vertexShader);
	//check compile error
	int success;
	char infolog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infolog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infolog << std::endl;
	}

	//fragment shader similar with vertex shader
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource,NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infolog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infolog << std::endl;
	}

	//link shader
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();  //return a reference to the created object
	glAttachShader(shaderProgram,vertexShader);  //addition vertexShader 
	glAttachShader(shaderProgram, fragmentShader);  //addition fragmentShader 
	glLinkProgram(shaderProgram);   //link these
	//check error
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infolog);
		std::cout << "ERROR::PROGROM::COMPILATION_FAILED\n" << infolog << std::endl;
	}
	//delete shader after link
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	//set up vertex data and configure vertex attributes
	float vertices[] = {
	-0.5f, -0.5f, 0.0f, //left
	0.5f, -0.5f, 0.0f, //right
	0.0f, 0.5f, 0.0f //top
	};
	//set up VAO & VBO
	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//bind the Vertex Array Object first, then bind and set vertex buffer(s), 
	//and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//copy the vertices to buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//set attributes of vertex
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	//i do not understand why need these code
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//render loop
	//if glfwWindowShouldClose is true exit loop
	while (!glfwWindowShouldClose(window))
	{
		//input 
		processInput(window);

		//Rendering
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//draw triangle
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//Swap color buffer
		//check events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
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