#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <glew.h>
#include <glfw3.h>
#include "graphics.h"
#include "gameObject.h"
#include "shader.h"

void GraphicsComponent::Update(GameObject& object) {}

TriangleGraphicsComponent::TriangleGraphicsComponent()
{
	triangle = Primitives::Triangle();
}

void TriangleGraphicsComponent::Update(GameObject& object)
{
	triangle.Draw(object.position);
}