#include <glfw3.h>
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include "bevgrafmath2017.h"
#include "Triangle.h"
#include "BezierSurface.h"
#define GLFW_DLL

GLFWcursor* handCursor = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
int WindowWidth = 800;
int WindowHeight = 600;
double mouseX;
double mouseY;
mat4 w2v, projection, coordTrans;
GLfloat center = 5.00;
vec2 windowSize = { 1, 1 };
vec2 windowPosition = { 0, 0 };
vec2 viewportSize = { (float)WindowWidth / 2, (float)WindowWidth / 2 };
vec2 viewportPosition = { (float)WindowHeight / 2, (float)WindowHeight / 2.5f };
vec3 camera, Xn, Yn, Zn, up = vec3(0.0f, 0.0f, 1.0f);
GLfloat uCam = 2.5f, vCam = 0.5f, rCam = 3.0f;

BezierSurface* bs = new BezierSurface();

void Camera(void) {

	camera = vec3(rCam * cos(uCam), rCam * sin(uCam), vCam);

	Zn = normalize(camera);
	Xn = normalize(cross(Zn, up));
	Yn = normalize(cross(Zn, Xn));

	coordTrans = coordinateTransform(camera, Xn, Yn, Zn);
}

void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
		bs->showControlNet = true;
	if (key == GLFW_KEY_W)
		vCam -= 0.02;
	if (key == GLFW_KEY_S)
		vCam += 0.02;
	if (key == GLFW_KEY_A)
		uCam -= 0.02;
	if (key == GLFW_KEY_D)
		uCam += 0.02;
	if (key == GLFW_KEY_KP_ADD)
		center += 10;
	if (key == GLFW_KEY_KP_SUBTRACT)
		center -= 10;
	if (key == GLFW_KEY_F)
		rCam += 0.02;
	if (key == GLFW_KEY_G)
		rCam -= 0.02;
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	mouseX = xpos;
	mouseY = ypos;
	glfwSetCursor(window, NULL);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	std::cout << "Clicked on x:" << mouseX << " y:" << mouseY << std::endl;

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{

	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{

	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
	{

	}
}

void Initialize(void)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, WindowWidth, WindowHeight, 0, -1, 1);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_SCISSOR_TEST);
	glShadeModel(GL_FLAT);
	glLineWidth(1.0);
	glPointSize(10.0);
	glClearDepth(1.0);
	glClearColor(1, 1, 1, 1);
	glScissor(0, 1, WindowWidth/2, WindowHeight/2);
	w2v = windowToViewport3(windowPosition, windowSize, viewportPosition, viewportSize);
}

int main(void)
{

#pragma region GLFWSETUP

	GLFWwindow* window;

	char WindowTitle[] = "Aww shit, here we go again...";

	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	window = glfwCreateWindow(WindowWidth, WindowHeight, WindowTitle, NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwSetKeyCallback(window, key_callback);
	glfwSetErrorCallback(error_callback);
	glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

#pragma endregion
#pragma region DiscoInit
	std::mt19937_64 generator;
	std::uniform_real_distribution<float> distribution(0, 1);
	float roll[3] = { 0,0,0 };
	int rollNum = 0;
#pragma endregion

	while (!glfwWindowShouldClose(window))
	{
		glfwGetFramebufferSize(window, &WindowWidth, &WindowHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//#pragma region Disco_Region
//		if (roll[rollNum] <= 1)
//		{
//			roll[rollNum] = roll[rollNum] + 0.10f;
//			roll[(rollNum - 1) % 3] = roll[(rollNum - 1) % 3] - 0.10f;
//		}
//		else
//			rollNum++;
//
//		if (roll[2] > 1)
//		{
//			for (int i = 0; i < 3; i++)
//			{
//				roll[i] = 0;
//			}
//			rollNum = 0;
//		}
//		glColor3f(roll[0], roll[1], roll[2]);
//#pragma endregion

		projection = perspective(center);

		Camera();

		bs->CalculateTriangles();
		bs->TransformTriangles(coordTrans);
		bs->TransformControlPoints(coordTrans);
		bs->SortTriangles();

		bs->Draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}