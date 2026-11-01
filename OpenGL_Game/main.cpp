#include "display.h"
#include <GL/glew.h>
#include "shader.h"
#include "mesh.h"
#include "texture.h"
#include "transform.h"
#include "camera.h"
#include <iostream>
#include "global.h"

int main() {
	Display display(800, 600, "Teapot Collector");

	Vertex vertices[] = { Vertex(glm::vec3(-0.5, -0.5, 0), glm::vec2(0.0, 0.0)),
						  Vertex(glm::vec3(0, 0.5, 0), glm::vec2(0.5, 1.0)),
						  Vertex(glm::vec3(0.5, -0.5, 0), glm::vec2(1.0, 0.0)), };

	Vertex floorVertices[] = {
		Vertex(glm::vec3(-5, 0, -5), glm::vec2(0,0)),
		Vertex(glm::vec3(5, 0, -5), glm::vec2(1,0)),
		Vertex(glm::vec3(5, 0,  5), glm::vec2(1,1)),
		Vertex(glm::vec3(-5, 0,  5), glm::vec2(0,1))
	};

	unsigned int floorindices[] = {
		0, 1, 2,
		0, 2, 3
	};

	Mesh floor(floorVertices, 4, floorindices, 6);


	unsigned int indices[] = { 0, 1, 2 };

	Mesh mesh(vertices, sizeof(vertices) / sizeof(vertices[0]), indices, sizeof(indices) / sizeof(indices[0]));
	Mesh mesh2("./assets/brrmonkey.obj");
	Mesh mesh3("./assets/teapot.obj");
	Shader shader("./assets/shaderTest");
	Texture texture("./assets/bricks.jpg");
	Camera camera(glm::vec3(0, 0.5, -6), 70.0f, (float)800 / (float)600, 0.01f, 1000.0f);
	Transform transform;
	Transform floortransform;
	Transform teapottransform;

	float counter = 0.0f;

	floortransform.GetPos().y = 0;
	transform.GetPos().y = 0;
	transform.GetPos().z = 0;
	teapottransform.GetPos().y = 0.1;
	teapottransform.GetPos().z = 1;
	teapottransform.SetScale(glm::vec3(0.1f));

	while (!display.IsClosed()) {
		const bool* keys = SDL_GetKeyboardState(nullptr);
		if (keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_A]) {
			transform.GetRot().y += 0.05;
		}
		if (keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_D]) {
			transform.GetRot().y -= 0.05;
		}
		if (keys[SDL_SCANCODE_UP] || keys[SDL_SCANCODE_W]) {
			transform.GetPos().x += sin(transform.GetRot().y) * 0.05;
			transform.GetPos().z += cos(transform.GetRot().y) * 0.05;
		}
		if (keys[SDL_SCANCODE_DOWN] || keys[SDL_SCANCODE_S]) {
			transform.GetPos().x -= sin(transform.GetRot().y) * 0.05;
			transform.GetPos().z -= cos(transform.GetRot().y) * 0.05;
		}
		if (keys[SDL_SCANCODE_R]) {
			transform.GetPos().x = 0;
			transform.GetPos().y = 0;
			transform.GetPos().z = 0;

			transform.GetRot().x = 0;
			transform.GetRot().y = 0;
			transform.GetRot().z = 0;
		}

		float dx = transform.GetPos().x - teapottransform.GetPos().x;
		float dz = transform.GetPos().z - teapottransform.GetPos().z;

		float distanceSquared = dx * dx + dz * dz;

		if (distanceSquared < 1.0f)
		{
			score += 1;

			float newlocX = (rand() % 10 - 5);
			float newlocZ = (rand() % 10 - 5);

			if (newlocX > 2) newlocX = 2;
			if (newlocX < -2) newlocX = -2;
			if (newlocZ > 2) newlocZ = 2;
			if (newlocZ < -2) newlocZ = -2;

			teapottransform.GetPos().x = newlocX;
			teapottransform.GetPos().z = newlocZ;
		}

		display.Clear(0.0f, 0.15f, 0.3f, 1.0f);
		shader.Bind();
		texture.Bind(0);
		shader.Update(floortransform, camera);
		floor.Draw();

		shader.Update(transform, camera);
		mesh2.Draw();
		shader.Update(teapottransform, camera);
		mesh3.Draw();
		display.Update();
		counter += 0.01f;
	}

	return 0;
}