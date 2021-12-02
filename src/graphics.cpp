#include <glew/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image/stb_image.h>
#include "graphics.h"
#include "context.h"
#include "entity.h"
#include "log.h"

namespace OpenGL
{
	void GraphicsComponent::Draw(Entity& entity) {}
	void GraphicsComponent::SetShader(std::string& shaderName)
	{
		shader = Context::Shaders[shaderName];
	}

	QuadComponent::QuadComponent()
		: quad(Quad()), vertexArray(0), vertexBuffer(0), elementBuffer(0)
	{
		LOGGER_TRACE("QuadComponent constructor called");
		glGenVertexArrays(1, &vertexArray);
		glBindVertexArray(vertexArray);

		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quad.vertices), quad.vertices, GL_STATIC_DRAW);

		glGenBuffers(1, &elementBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quad.indices), quad.indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		shader = Context::Shaders["BasicShader"];
	}
	QuadComponent::~QuadComponent()
	{
		LOGGER_TRACE("QuadComponent destructor called");
		glBindVertexArray(0);
		glDeleteBuffers(1, &vertexBuffer);
		glDeleteBuffers(1, &elementBuffer);
		glDeleteVertexArrays(1, &vertexArray);
	}

	void QuadComponent::Draw(Entity& entity)
	{
		glUseProgram(shader);
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(entity.GetPosition(), 0.0f))
			* glm::rotate(glm::mat4(1.0f), entity.GetRotationRadians(), {0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), glm::vec3(entity.GetSize(), 1.0f));

		unsigned int uModel = glGetUniformLocation(shader, "model");
		glUniformMatrix4fv(uModel, 1, GL_FALSE, glm::value_ptr(model));

		unsigned int uColor = glGetUniformLocation(shader, "color");
		glUniform3fv(uColor, 1, glm::value_ptr(entity.Color));

		glBindVertexArray(vertexArray);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	TexturedQuadComponent::TexturedQuadComponent(const char* textureFilePath)
		: texturedQuad(TexturedQuad()), vertexArray(0), vertexBuffer(0), elementBuffer(0)
	{
		int width, height, nrChannels;

		LOGGER_TRACE("TexturedQuadComponent constructor called");

		glGenVertexArrays(1, &vertexArray);
		glBindVertexArray(vertexArray);

		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(texturedQuad.vertices), texturedQuad.vertices, GL_STATIC_DRAW);

		glGenBuffers(1, &elementBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(texturedQuad.indices), texturedQuad.indices, GL_STATIC_DRAW);

		// positions
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// textures
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		unsigned char* data = stbi_load(textureFilePath, &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
			LOGGER_ERROR("Failed to load texture: \"{0}\"", textureFilePath);

		stbi_image_free(data);
		shader = Context::Shaders["TexturedShader"];
	}
	TexturedQuadComponent::~TexturedQuadComponent()
	{
		LOGGER_TRACE("TexturedQuadComponent destructor called");
		glBindVertexArray(0);
		glDeleteTextures(1, &texture);
		glDeleteBuffers(1, &vertexBuffer);
		glDeleteBuffers(1, &elementBuffer);
		glDeleteVertexArrays(1, &vertexArray);
	}

	void TexturedQuadComponent::Draw(Entity& entity)
	{
		glUseProgram(shader);
		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(entity.GetPosition(), 0.0f))
			* glm::rotate(glm::mat4(1.0f), entity.GetRotationRadians(), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), glm::vec3(entity.GetSize(), 1.0f));

		unsigned int uModel = glGetUniformLocation(shader, "model");
		glUniformMatrix4fv(uModel, 1, GL_FALSE, glm::value_ptr(model));

		unsigned int uColor = glGetUniformLocation(shader, "color");
		glUniform3fv(uColor, 1, glm::value_ptr(entity.Color));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		glBindVertexArray(vertexArray);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}