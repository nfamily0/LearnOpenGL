#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//���� ���̴� �ڵ� �ۼ�
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";


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

	//�������̴� ����
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//�������̴��� �ҽ��ڵ� ����
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	//�������̴� ������
	glCompileShader(vertexShader);

	//������ ���� ����
	int  success;
	char infoLog[512]; //���� �޼����� ������ ���⿡ ������
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//�÷��׸�Ʈ ���̴� ����
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//�ҽ��ڵ带 �÷��׸�Ʈ �� ����
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	//������
	glCompileShader(fragmentShader);
	//������ ���� ���� äũ
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//���̴� �� ������ ���α׷� ID ����
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	//���̴��� ������
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	//shader linking
	glLinkProgram(shaderProgram);

	//linking ���� ���� äũ
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	//����� ���̴� ����
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//�ﰢ�� ���� ����
	float vertices[] = {
		 0.5f,  0.5f, 0.0f,  // ���� ���
		 0.5f, -0.5f, 0.0f,  // ���� �ϴ�
		-0.5f, -0.5f, 0.0f,  // ���� �ϴ�
		-0.5f,  0.5f, 0.0f   // ���� ���
	};
	unsigned int indices[] = {  // 0���� �����Ѵٴ� ���� ����ϼ���!
	0, 1, 3,   // ù ��° �ﰢ��
	1, 2, 3    // �� ��° �ﰢ��
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
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		

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