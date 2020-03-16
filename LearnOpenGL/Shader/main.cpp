#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shader.h"


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;



//뷰포트 변경시 작동하는 콜백함수
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// 입력값을 받아서 처리
void processInput(GLFWwindow* window);


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


	Shader progame("./basic.vert", "./basic.frag");


	//삼각형 정점 설정
	float vertices[] = {
		 0.0f,  0.5f, 0.0f,  // 우측 상단
		0.5f, -0.5f, 0.0f,  // 우측 하단
		-0.5f, -0.5f, 0.0f,  // 좌측 하단
	};
	unsigned int indices[] = {  // 0부터 시작한다는 것을 명심하세요!
	0, 1, 2,   // 첫 번째 삼각형
	};

	//Vertex Buffer object(VBO),Vertex Array Object(VAO)
	unsigned int VBO, VAO, EBO;

	//GPU메모리상의 ID생성
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	//바인드
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//정점속성 설정
	//1. 매개변수 : shader 소스코드의 (location = 0)에 대응하는 정점속성 생성
	//2. 정점 크기 지정 vec3이므로 3
	//3. 데이터 타입
	//4. 정규화 여부
	//5. stride 설정 정정버퍼의 비트단위 크기 지정
	//6. 정점 시작 위치 (void*)로 형변환 해주어야 함
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);


	//render loop
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		//gl배경색
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//삼각형 그리기
		progame.use();
		float timeValue = glfwGetTime();
		float greenValue = sin(timeValue) / 2.0f + 0.5f; //항상 양수
		int vertexColorLocation = glGetUniformLocation(progame.ID, "ourColor");
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}



	//사용한 버퍼 버퍼들 삭제
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

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

