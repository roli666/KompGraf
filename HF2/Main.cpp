#include <glfw3.h>
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include "bevgrafmath2017.h"
#include "Nurbs.h"

#define GLFW_DLL

Nurbs* spline = new Nurbs(10);
GLFWcursor* handCursor = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
double mouseX;
double mouseY;

void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	mouseX = xpos;
	mouseY = ypos;
	glfwSetCursor(window, NULL);
	if (spline->MouseOverPoint(xpos, ypos))
	{
		glfwSetCursor(window, handCursor);
	}
	if (spline->grabbedPoint != nullptr)
	{
		spline->MoveGrabbedPoint(xpos,ypos);
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	std::cout << "Clicked on x:" << mouseX << " y:" << mouseY << std::endl;

	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		spline->grabbedPoint = nullptr;
		if (!spline->MouseOverPoint(mouseX, mouseY))
			spline->AddPoint(mouseX, mouseY);
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		if (spline->MouseOverPoint(mouseX, mouseY))
			spline->SetGrabbedPoint(mouseX, mouseY);
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
	{
		spline->RemovePoint(mouseX, mouseY);
	}
}

int main(void)
{

#pragma region GLFWSETUP

	GLFWwindow* window;

	char WindowTitle[] = "Aww shit, here we go again...";
	int WindowWidth = 800;
	int WindowHeight = 600;

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

	std::mt19937_64 generator;
	std::uniform_real_distribution<float> distribution(0, 1);

	//float roll[3] = { distribution(generator),distribution(generator),distribution(generator) };
	float roll[3] = { 0,0,0 };
	int rollNum = 0;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, WindowWidth, WindowHeight, 0, -1, 1);
	glEnable(GL_POINT_SMOOTH);
	glShadeModel(GL_FLAT);
	glClearColor(1, 1, 1, 1);

	while (!glfwWindowShouldClose(window))
	{
		glfwGetFramebufferSize(window, &WindowWidth, &WindowHeight);
		glClear(GL_COLOR_BUFFER_BIT);

#pragma region Disco_Region
		if (roll[rollNum] <= 1)
		{
			roll[rollNum] = roll[rollNum] + 0.10f;
			roll[(rollNum - 1) % 3] = roll[(rollNum - 1) % 3] - 0.10f;
		}
		else
			rollNum++;

		if (roll[2] > 1)
		{
			for (int i = 0; i < 3; i++)
			{
				roll[i] = 0;
			}
			rollNum = 0;
		}
		glColor3f(roll[0], roll[1], roll[2]);
#pragma endregion

		spline->DrawPoints();
		spline->DrawSpline(3);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}