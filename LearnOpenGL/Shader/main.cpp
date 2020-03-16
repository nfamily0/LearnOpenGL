#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;



//����Ʈ ����� �۵��ϴ� �ݹ��Լ�
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// �Է°��� �޾Ƽ� ó��
void processInput(GLFWwindow* window);


int main() {
	//glfw �ʱ�ȭ
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	//glfw window ����
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	//����Ʈ ����
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//GLAD �ʱ�ȭ
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	Shader progame("./basic.vert", "./basic.frag");


	//�ﰢ�� ���� ����
	float vertices[] = {
		 0.0f,  0.5f, 0.0f,  // ���� ���
		0.5f, -0.5f, 0.0f,  // ���� �ϴ�
		-0.5f, -0.5f, 0.0f,  // ���� �ϴ�
	};
	unsigned int indices[] = {  // 0���� �����Ѵٴ� ���� ����ϼ���!
	0, 1, 2,   // ù ��° �ﰢ��
	};

	//Vertex Buffer object(VBO),Vertex Array Object(VAO)
	unsigned int VBO, VAO, EBO;

	//GPU�޸𸮻��� ID����
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	//���ε�
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//�����Ӽ� ����
	//1. �Ű����� : shader �ҽ��ڵ��� (location = 0)�� �����ϴ� �����Ӽ� ����
	//2. ���� ũ�� ���� vec3�̹Ƿ� 3
	//3. ������ Ÿ��
	//4. ����ȭ ����
	//5. stride ���� ���������� ��Ʈ���� ũ�� ����
	//6. ���� ���� ��ġ (void*)�� ����ȯ ���־�� ��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);


	//render loop
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		//gl����
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//�ﰢ�� �׸���
		progame.use();
		float timeValue = glfwGetTime();
		float greenValue = sin(timeValue) / 2.0f + 0.5f; //�׻� ���
		int vertexColorLocation = glGetUniformLocation(progame.ID, "ourColor");
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}



	//����� ���� ���۵� ����
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	//glfw �ڿ� ����
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

