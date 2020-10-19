#include <iostream>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <algorithm>
#include <iterator>

const float sphereRadius = 1.5f;

struct Ray
{
	glm::vec3 pos;
	glm::vec3 dir;
};

float SphereDistance(float eye, float centre, float sphereRadius)
{
	float dist = centre - eye;
	return dist - sphereRadius;
}

float DistanceToScene(glm::vec3 p)
{
	float pRemained = glm::length(p) - 0.3f;
	if (p.y < pRemained)
	{
		return p.y;
	}
	else
	{
		return pRemained;
	}
}

Ray CreateCameraRay(glm::vec2 uv, glm::vec3 camPos, glm::vec3 LookAt, float zoom)
{
	Ray newRay;
	glm::vec3 f = glm::normalize(LookAt - camPos);
	glm::vec3 r = cross(glm::vec3(0.0f, 1.0f, 0.0f), f);
	glm::vec3 u = glm::cross(f, r);
	glm::vec3 c = camPos + f * zoom;
	glm::vec3 i = c + uv.x * r + uv.y * u;
	glm::vec3 dir = i - camPos;
	newRay.pos = camPos;
	//std::cout << "x: " << newRay.pos.x << " y: " << newRay.pos.y << " z: " << newRay.pos.z << std::endl;
	newRay.dir = normalize(dir);
	return newRay;
}

glm::vec3 estimateNormal(glm::vec3 p)
{
	const float EPS = std::numeric_limits<float>::epsilon();
	float xPl = DistanceToScene(glm::vec3(p.x + EPS, p.y, p.z));
	float xMi = DistanceToScene(glm::vec3(p.x - EPS, p.y, p.z));
	float yPl = DistanceToScene(glm::vec3(p.x, p.y + EPS, p.z));
	float yMi = DistanceToScene(glm::vec3(p.x, p.y - EPS, p.z));
	float zPl = DistanceToScene(glm::vec3(p.x, p.y, p.z + EPS));
	float zMi = DistanceToScene(glm::vec3(p.x, p.y, p.z - EPS));
	float xDiff = xPl - xMi;
	float yDiff = yPl - yMi;
	float zDiff = zPl - zMi;
	return normalize(glm::vec3(xDiff, yDiff, zDiff));
}




int main()
{
	GLFWwindow* window;
	Ray* ray; 


	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);


	Ray r = CreateCameraRay(glm::vec2(0.5f, 0.5f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 1.f), 1.f);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		glBegin(GL_TRIANGLES);
		glVertex2f(0.0f, 1.f);
		glVertex2f(-1.f, -1.f);
		glVertex2f(1.f, -1.f);
		glEnd();

		glBegin(GL_LINES);
		glVertex2f(0.5f, 0.5f);
		glVertex2f(-0.5f, 0.7f);
		glEnd();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;

}