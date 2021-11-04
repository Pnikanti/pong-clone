#include <iostream>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <glew.h>
#include <glfw3.h>
#include "graphics.h"
#include "context.h"
#include "gameObject.h"
#include "shader.h"

void GraphicsComponent::Draw(GameObject& object) {}

TriangleGraphicsComponent::TriangleGraphicsComponent() 
	: triangle(Primitives::Triangle())
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle.vertices), triangle.vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void TriangleGraphicsComponent::Draw(GameObject& object)
{
	unsigned int shader = OpenGLContext::shaders["PaddleShader"];

	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(object.position, 0.0f))
		* glm::scale(glm::mat4(1.0f), glm::vec3(object.size, 1.0f));

	unsigned int modelUniform = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(model));

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}

QuadGraphicsComponent::QuadGraphicsComponent()
	: quad(Primitives::Quad())
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad.vertices), quad.vertices, GL_STATIC_DRAW);
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quad.indices), quad.indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void QuadGraphicsComponent::Draw(GameObject& object)
{
	unsigned int shader = OpenGLContext::shaders["PaddleShader"];
	glUseProgram(shader);

	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(object.position, 0.0f))
		* glm::scale(glm::mat4(1.0f), glm::vec3(object.size, 1.0f));

	unsigned int modelUniform = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(model));

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

CircleGraphicsComponent::CircleGraphicsComponent()
	: quad(Primitives::Quad())
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad.vertices), quad.vertices, GL_STATIC_DRAW);
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quad.indices), quad.indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}
void CircleGraphicsComponent::Draw(GameObject& object)
{
	unsigned int shader = OpenGLContext::shaders["BallShader"];
	glUseProgram(shader);

	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(object.position, 0.0f))
		* glm::scale(glm::mat4(1.0f), glm::vec3(object.size, 1.0f));

	unsigned int modelUniform = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(model));

	unsigned int positionUniform = glGetUniformLocation(shader, "position");
	glUniform2fv(positionUniform, 1, glm::value_ptr(object.position));

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}