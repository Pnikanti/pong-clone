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

GraphicsComponent::GraphicsComponent() 
	: size(glm::vec2(1.0f)), model(glm::mat4(1.0f)) {}

void GraphicsComponent::Draw(GameObject& object) {}

TriangleGraphicsComponent::TriangleGraphicsComponent() 
	: GraphicsComponent(), triangle(Primitives::Triangle()), size(glm::vec2(1.0f)), model(glm::mat4(1.0f))
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle.vertices), triangle.vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	model = glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));
}

void TriangleGraphicsComponent::Draw(GameObject& object)
{
	std::cout << "object pos: " << object.position.x << ", " << object.position.y << std::endl;
	glUseProgram(OpenGLContext::shader);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(object.position, 0.0f));
	unsigned int modelUniform = glGetUniformLocation(OpenGLContext::shader, "model");
	glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(model));

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}