#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader.h>
#include <camera.h>

#include "teapot_loader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// object drawing
void drawLShape(unsigned int VAO, unsigned int nVert, bool bSolid, bool bColor, glm::mat4 model_curr, Shader shader);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 1.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Jieun Lee", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile our shader zprogram
	// ------------------------------------
	Shader cubeShader("j11.cube.vs", "j11.cube.fs");
	Shader teapotShader1("14.2.teapot.vs", "14.2.teapot.fs");

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// position           // normal
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	// cube VAO and VBO
	unsigned int cubeVBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindVertexArray(cubeVAO);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)3);
	//glEnableVertexAttribArray(1);

	// teapot VAO and VBO
	std::vector <float> data;
	Teapot teapot("teapot.vbo", data, 8);

	unsigned int teapotVBO, teapotVAO;
	glGenVertexArrays(1, &teapotVAO);
	glGenBuffers(1, &teapotVBO);
	glBindBuffer(GL_ARRAY_BUFFER, teapotVBO);
	glBufferData(GL_ARRAY_BUFFER, teapot.nVertNum * teapot.nVertFloats * sizeof(float), &data[0], GL_STATIC_DRAW);

	glBindVertexArray(teapotVAO);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, teapot.nVertFloats * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texCoord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, teapot.nVertFloats * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// normal attribute
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, teapot.nVertFloats * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);


	// set keyframes
	// keyframe 0
	/*glm::mat4 model0 = glm::mat4(1.0f);
	//model0 = glm::rotate(model0, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//glm::quat q0 = glm::quat(cos(glm::radians(90.0f / 2)), 0.0f, sin(glm::radians(90.0f / 2)), 0.0f);
	glm::quat q0 = glm::angleAxis(glm::radians(90.f), glm::vec3(0.f, 1.f, 0.f));

	// keyframe 1
	glm::mat4 model1 = glm::mat4(1.0f);
	model1 = glm::rotate(model1, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	model1 = glm::rotate(model1, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	model1 = glm::rotate(model1, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::quat q1 = glm::quat_cast(model1);*/

	//keyframe0
	glm::mat4 model0 = glm::mat4(1.0f);
	glm::mat4 translate0 = glm::translate(model0, glm::vec3(1.0f, 1.0f, 0.0f));
	glm::quat q0 = glm::angleAxis(glm::radians(90.f), glm::vec3(0.f, 1.f, 0.f));
	glm::mat4 rotate0 = glm::mat4_cast(q0);
	glm::mat4 scale0 = glm::scale(model0, glm::vec3(1.0f, 1.0f, 1.0f));
	model0 = translate0 * rotate0 * scale0;

	//keyFrame1
	glm::mat4 model1 = glm::mat4(1.0f);
	glm::mat4 translate1 = glm::translate(model1, glm::vec3(0.0f, 1.0f, 1.0f));
	glm::quat q1 = glm::angleAxis(glm::radians(45.f), glm::vec3(-1.f, 1.f, 0.f));
	glm::mat4 rotate1 = glm::mat4_cast(q1);
	glm::mat4 scale1 = glm::scale(model1, glm::vec3(2.0f, 2.0f, 2.0f));
	model1 = translate1 * rotate1 * scale1;

	//keyFrame2
	glm::mat4 model2 = glm::mat4(1.0f);
	glm::mat4 translate2 = glm::translate(model2, glm::vec3(-1.0f, 1.0f, 0.0f));
	glm::quat q2 = glm::angleAxis(glm::radians(160.f), glm::vec3(0.f, 0.f, 1.f));
	glm::mat4 rotate2 = glm::mat4_cast(q2);
	glm::mat4 scale2 = glm::scale(model2, glm::vec3(1.0f, 1.0f, 1.0f));
	model2 = translate2 * rotate2 * scale2;

	//keyFrame3
	glm::mat4 model3 = glm::mat4(1.0f);
	glm::mat4 translate3 = glm::translate(model3, glm::vec3(1.0f, 0.0f, 1.0f));
	glm::quat q3 = glm::angleAxis(glm::radians(120.f), glm::vec3(1.f, 0.f, 0.5f));
	glm::mat4 rotate3 = glm::mat4_cast(q3);
	glm::mat4 scale3 = glm::scale(model3, glm::vec3(3.0f, 3.0f, 3.0f));
	model3 = translate3 * rotate3 * scale3;

	//keyFrame4
	glm::mat4 model4 = glm::mat4(1.0f);
	glm::mat4 translate4 = glm::translate(model4, glm::vec3(2.0f, 2.0f, -2.0f));
	glm::quat q4 = glm::angleAxis(glm::radians(60.f), glm::vec3(0.f, 1.f, 0.5f));
	glm::mat4 rotate4 = glm::mat4_cast(q4);
	glm::mat4 scale4 = glm::scale(model4, glm::vec3(0.5f, 0.5f, 0.5f));
	model4 = translate4 * rotate4 * scale4;

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();


		// draw cubes
		cubeShader.use();
		// view/projection transformations
		cubeShader.setMat4("projection", projection);
		cubeShader.setMat4("view", view);


		
		// L shape animation 

		// initial orientation
		glm::mat4 model = glm::mat4(1.0f);

		// keyframe 0
		/*model0 = glm::mat4_cast(q0);
		drawLShape(cubeVAO, 36, false, true, model0, cubeShader);

		// keyframe 1
		model1 = glm::mat4_cast(q1);
		drawLShape(cubeVAO, 36, false, true, model1, cubeShader);*/

		// quaternion interpolation using slerp
		/*static float t = 0.0f;
		glm::quat q = glm::mix(q0, q1, t);
		model = glm::mat4_cast(q);
		drawLShape(cubeVAO, 36, true, true, model, cubeShader);
		t = t + 0.01f;
		if (t > 1.0) t = 0.0f;*/



		// also draw the lamp object
		cubeShader.setVec3("objectColor", 1.0f, 1.0f, 1.0f);
		// world transformation
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
		cubeShader.setMat4("model", model);
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);


		teapotShader1.use();
		// view/projection transformations
		teapotShader1.setMat4("projection", projection);
		teapotShader1.setMat4("view", view);

		glm::vec3 lightColor(1.0, 1.0, 1.0);;
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); // decrease the influence 
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influenc
		teapotShader1.setVec3("light.ambient", ambientColor);
		teapotShader1.setVec3("light.diffuse", diffuseColor);
		teapotShader1.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		teapotShader1.setVec3("light.position", lightPos);
		// material properties
		teapotShader1.setVec3("material.ambient", 0.7f, 0.5f, 0.3f);
		teapotShader1.setVec3("material.diffuse", 0.7f, 0.5f, 0.3f);
		teapotShader1.setVec3("material.specular", 0.5f, 0.5f, 0.5f); // specular lighting doesn't have full effect on this object's material
		teapotShader1.setFloat("material.shininess", 30.0f);
		// world transformation
		model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));


		glm::mat4 mix0;
		glm::mat4 mix1;
		glm::mat4 mix2;
		glm::mat4 mix3;
		static float t = 0.0f;
		if (t >= 0.0f && t < 1.0f) {
			mix0 = glm::mix(model0, model1, t);
			teapotShader1.setMat4("model", mix0);
		}
		else if (t >= 1.0f && t < 2.0f) {
			mix1 = glm::mix(model1, model2, t - 1.0f);
			teapotShader1.setMat4("model", mix1);
		}
		else if (t > 2.0f && t < 3.0f) {
			mix2 = glm::mix(model2, model3, t - 2.0f);
			teapotShader1.setMat4("model", mix2);
		}
		else if (t > 3.0f && t < 4.0f) {
			mix3 = glm::mix(model3, model4, t - 3.0f);
			teapotShader1.setMat4("model", mix3);
		}
		t = t + 0.01f;
		if (t > 4.0) t = 0.0f; 
		

		
		// render the teapot
		glBindVertexArray(teapotVAO);
		glDrawArrays(GL_TRIANGLES, 0, teapot.nVertNum);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &cubeVAO);
	//glDeleteVertexArrays(1, &teapotVAO);
	glDeleteBuffers(1, &cubeVBO);
	//glDeleteBuffers(1, &teapotVBO);

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// draw L shape
void drawLShape(unsigned int VAO, unsigned int nVert, bool bSolid, bool bColor, glm::mat4 model_curr, Shader shader)
{
	glm::vec3 color1 = glm::vec3(1.0f, 0.0f, 1.0f);
	glm::vec3 color2 = glm::vec3(0.0f, 1.0f, 1.0f);
	if (!bColor)
	{
		color1 = glm::vec3(0.7f, 0.7f, 0.7f);
		color2 = glm::vec3(0.7f, 0.7f, 0.7f);
	}

	// draw the magenta cube
	shader.setVec3("objectColor", color1);
	// world transformation
	glm::mat4 model = model_curr;
	model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 0.05f, 0.05f));
	shader.setMat4("model", model);
	glBindVertexArray(VAO);
	if (bSolid)
		glDrawArrays(GL_TRIANGLES, 0, nVert);
	else
		glDrawArrays(GL_LINE_LOOP, 0, nVert);


	// draw the cyan cube
	shader.setVec3("objectColor", color2);
	// world transformation
	model = model_curr;
	model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
	model = glm::scale(model, glm::vec3(0.05f, 1.0f, 0.05f));
	shader.setMat4("model", model);
	glBindVertexArray(VAO);
	if (bSolid)
		glDrawArrays(GL_TRIANGLES, 0, nVert);
	else
		glDrawArrays(GL_LINE_LOOP, 0, nVert);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}
