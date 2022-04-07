#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Mesh.h"
#include "Shader.h"
#include "Window.h"

const float toRadians = 3.141592665f / 180.f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

// Vertex Shader
static const char* vShader = "Shaders/shader.vert";

// Fragment Shader
static const char* fShader = "Shaders/shader.frag";

void CreateObjects() {
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		-1.f, -1.f, -1.f,
		0.f, -1.f, 1.f,
		1.f, -1.f, -1.f,
		0.f, 1.f, 0.f
	};

	Mesh* obj = new Mesh();
	obj->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj);
	obj = new Mesh();
	obj->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj);
}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

int main(int argc, char** argv) 
{
	mainWindow = Window(800, 600);
	mainWindow.Initialize();
	CreateObjects();
	CreateShaders();

	GLuint uniformProjection = 0, uniformView = 0, uniformModel = 0;
	glm::mat4 projection = glm::perspective(45.f, (GLfloat)mainWindow.getBufferWidth() / (GLfloat)mainWindow.getBufferHeight(), 0.1f, 100.f);
	glm::mat4 view(1.0f);

	// Loop until window closed
	while (!mainWindow.getShouldClose()) {
		// Get & handle user input events
		glfwPollEvents();

		// Clear window
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();

		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.f, 0.f, -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		meshList[0]->RenderMesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.f, 1.f, -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		meshList[1]->RenderMesh();

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}