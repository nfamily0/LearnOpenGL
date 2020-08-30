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

const char* fragmentShaderSource2 = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.2f, 0.5f, 0.4f, 1.0f);\n"
"}\n\0";


//����Ʈ ����� �۵��ϴ� �ݹ��Լ�
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// �Է°��� �޾Ƽ� ó��
void processInput(GLFWwindow* window);
//���̴� ������ �ڵ�
unsigned int CompileShader(const char* vertextShaderSource, const char* fragmentShaderSource);

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


	unsigned int orangeTriangle = CompileShader(vertexShaderSource, fragmentShaderSource);
	unsigned int greenTriangle = CompileShader(vertexShaderSource, fragmentShaderSource2);


	//�ﰢ�� ���� ����
	float vertices[] = {
		-0.9f, -0.5f, 0.0f,  // left 
		-0.0f, -0.5f, 0.0f,  // right
		-0.45f, 0.5f, 0.0f,  // top 
	};

	float vertice2[] = {
		// second triangle
		 0.0f, 0.5f, 0.0f,  // left
		 0.9f, 0.5f, 0.0f,  // right
		 0.45f,-0.5f, 0.0f   // top 
	};

	//Vertex Buffer object(VBO),Vertex Array Object(VAO)
	unsigned int VBO[2], VAO[2];
	//���������� GPU�޸𸮻��� ID����
	glGenVertexArrays(2, VAO);
	glGenBuffers(2, VBO);

	//���� Array Buffer ���ε�
	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertice2), vertice2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);



	//render loop
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		//gl����
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		//�ﰢ�� �׸���
		glUseProgram(orangeTriangle);
		glBindVertexArray(VAO[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);


		glUseProgram(greenTriangle);
		glBindVertexArray(VAO[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}



	//����� ���� ���۵� ����
	glDeleteVertexArrays(2, VAO);
	glDeleteBuffers(2, VBO);

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

unsigned int CompileShader(const char* vertextShaderSource,const char* fragmentShaderSource) {
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

	return shaderProgram;
}