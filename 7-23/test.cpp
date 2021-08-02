
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
using namespace std;

#define STRINGIZE(x) #x
#define SHADER(shader) "" STRINGIZE(shader)

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window= glfwCreateWindow(800,600,"OpenGL",nullptr,nullptr);

	//opengl 运行模式单线程 要调用opengl api是在单一线程内进行
	//多个线程同时调用不被允许
	//绑定这个window与当前main函数的线程
	glfwMakeContextCurrent(window);

	//初始化glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}

	//顶点数据

	float vertex1[] =
	{
		//为了适配封装的VAO方法 UV坐标暂时使用三维的
		// positions          // colors           // texture coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,0.0f,   // top right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,0.0f,  // bottom right
	   -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,0.0f,   // bottom left
	   -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,0.0f   // top left 
	};

	unsigned int indices[] = {
	   0, 1, 3, // first triangle
	   1, 2, 3  // second triangle
	};
	VAO *vao1 = new VAO();

	vector<int> layout = { 0,1,2 };
	vao1->AddVertex3D(vertex1, 4, layout);
	vao1->SetIndexData(indices,6);

	Shader *shader = new Shader("vert.vs", "fragment.fs");

	int width, height, nrChannels;

	unsigned char* data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);

	GLuint texId = 0;
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);

	//纹理包装以及过滤模式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		cout << "Failed to load texture" << endl;
	}
	stbi_image_free(data);
	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);//1字节对齐

	glViewport(0, 0, 800, 600);  //指定opengl渲染窗口位置 以及大小
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  //窗口大小改变 回调执行framebuffer_size_callback 重新指定渲染视口
	//绘制
	while (!glfwWindowShouldClose(window))   //不关闭一直绘制
	{
		//清屏指定颜色
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		shader->use();
		
		//纹理单元 0-31 编号
		//传入纹理单元号0
		//GLuint texLoc = glGetUniformLocation(program->programId, "tex");
		//激活0号位单元
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texId);
		//glUniform1i(texLoc, 0);

		vao1->Draw();

		processInput(window); //监听输入事件 按Escape 关闭窗口
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//释放
	glfwTerminate();
	delete vao1;
	delete shader;
	return 0;
}

