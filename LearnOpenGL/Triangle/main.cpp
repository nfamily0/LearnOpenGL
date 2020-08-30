#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//정점 쉐이더 코드 작성
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


//뷰포트 변경시 작동하는 콜백함수
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// 입력값을 받아서 처리
void processInput(GLFWwindow* window);
//쉐이더 컴파일 코드
unsigned int CompileShader(const char* vertextShaderSource, const char* fragmentShaderSource);

int main() {
	//glfw 초기화
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	//glfw window 생성
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	//뷰포트 설정
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//GLAD 초기화
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	unsigned int orangeTriangle = CompileShader(vertexShaderSource, fragmentShaderSource);
	unsigned int greenTriangle = CompileShader(vertexShaderSource, fragmentShaderSource2);


	//삼각형 정점 설정
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
	//정점버퍼의 GPU메모리상의 ID생성
	glGenVertexArrays(2, VAO);
	glGenBuffers(2, VBO);

	//정점 Array Buffer 바인드
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

		//gl배경색
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		//삼각형 그리기
		glUseProgram(orangeTriangle);
		glBindVertexArray(VAO[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);


		glUseProgram(greenTriangle);
		glBindVertexArray(VAO[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}



	//사용한 버퍼 버퍼들 삭제
	glDeleteVertexArrays(2, VAO);
	glDeleteBuffers(2, VBO);

	//glfw 자원 정리
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
	//정점쉐이더 생성
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//정점쉐이더에 소스코드 연결
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	//정점쉐이더 컴파일
	glCompileShader(vertexShader);

	//컴파일 성공 여부
	int  success;
	char infoLog[512]; //에러 메세지가 생길경우 여기에 생성됨
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//플래그먼트 쉐이더 생성
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//소스코드를 플레그먼트 에 연결
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	//컴파일
	glCompileShader(fragmentShader);
	//컴파일 성공 여부 채크
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//쉐이더 를 연결할 프로그램 ID 생성
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	//쉐이더를 붙혀줌
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	//shader linking
	glLinkProgram(shaderProgram);

	//linking 성공 여부 채크
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	//사용한 쉐이더 삭제
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}