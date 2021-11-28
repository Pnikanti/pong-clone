#pragma once

#include <string>

class Entity;

namespace OpenGL
{
	struct Quad {
		float vertices[12] = {
			 0.5f,  0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			-0.5f, -0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};
		unsigned int indices[6] = {
			0, 1, 3,
			1, 2, 3
		};
	};

	class GraphicsComponent
	{
	public:
		virtual ~GraphicsComponent() = default;
		virtual void Draw(Entity& entity);
		void SetShader(std::string& shaderName);
	protected:
		unsigned int shader;
	};

	class QuadComponent : public GraphicsComponent
	{
	public:
		QuadComponent();
		~QuadComponent();
		void Draw(Entity& entity) override;
	private:
		Quad quad;
		unsigned int vertexArray;
		unsigned int vertexBuffer;
		unsigned int elementBuffer;
	};
	class CircleComponent : public GraphicsComponent
	{
	public:
		CircleComponent();
		~CircleComponent();
		void Draw(Entity& entity) override;
	private:
		Quad quad;
		unsigned int vertexArray;
		unsigned int vertexBuffer;
		unsigned int elementBuffer;
	};
}