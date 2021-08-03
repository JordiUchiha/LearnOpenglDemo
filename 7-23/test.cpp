
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

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

	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,0.0f
	};

	glm::vec3 cubePositions[] = {
			glm::vec3(0.0f,  0.0f,  0.0f),
			glm::vec3(2.0f,  5.0f, -15.0f),
			glm::vec3(-1.5f, -2.2f, -2.5f),
			glm::vec3(-3.8f, -2.0f, -12.3f),
			glm::vec3(2.4f, -0.4f, -3.5f),
			glm::vec3(-1.7f,  3.0f, -7.5f),
			glm::vec3(1.3f, -2.0f, -2.5f),
			glm::vec3(1.5f,  2.0f, -2.5f),
			glm::vec3(1.5f,  0.2f, -1.5f),
			glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	unsigned int indices[] = {
	   0, 1, 3, // first triangle
	   1, 2, 3  // second triangle
	};
	VAO *vao1 = new VAO();

	vector<int> layout = { 0,1 };
	vao1->AddVertex3D(vertices, 36, layout);
	//vao1->SetIndexData(indices,6);

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

	GLuint texId2 = 0;
	glGenTextures(1, &texId2);
	glBindTexture(GL_TEXTURE_2D, texId2);
	//纹理包装以及过滤模式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		cout << "Failed to load texture" << endl;
	}
	stbi_image_free(data);
	//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);//1字节对齐

	shader->use();
	shader->setInt("ourTexture", 0);
	shader->setInt("otherTexture", 1);
	glViewport(0, 0, 800, 600);  //指定opengl渲染窗口位置 以及大小
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_DEPTH_TEST);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  //窗口大小改变 回调执行framebuffer_size_callback 重新指定渲染视口
	//绘制
	while (!glfwWindowShouldClose(window))   //不关闭一直绘制
	{
		//清屏指定颜色
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//纹理单元 0-31 编号
		//传入纹理单元号0
		//GLuint texLoc = glGetUniformLocation(program->programId, "tex");
		//激活0号位单元
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texId);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texId2);

		//矩阵变换

		//投影矩阵
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.0f), 800.f / 600.f, 0.1f, 100.f);

		shader->use();
		//构建相机矩阵
		const float radius = 10.0f;
		float camX = sin(glfwGetTime())*radius;
		float camZ = cos(glfwGetTime())*radius;
		glm::mat4 view;
		view = glm::lookAt(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
		shader->setMat4("view", view);
		shader->setMat4("projection", projection);
		//unsigned int transformLoc = glGetUniformLocation(shader->ID, "transform");
		//glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
		//glUniform1i(texLoc, 0);
		vao1->BindVAO();
		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			shader->setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

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

