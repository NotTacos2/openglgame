#include "display.h"
#include <GL/glew.h>
#include <iostream>
#include "camera.h"
#include "global.h"

Display::Display(int width, int height, const std::string& title) {
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	m_window = SDL_CreateWindow(title.c_str(), width, height, SDL_WINDOW_OPENGL);
	m_glContext = SDL_GL_CreateContext(m_window);

	GLenum status = glewInit();

	if (status != GLEW_OK) {
		std::cerr << "Glew failed to init!" << std::endl;
	}
	m_isClosed = false;

	glEnable(GL_DEPTH_TEST);

	glDisable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

Display::~Display() {
	SDL_GL_DestroyContext(m_glContext);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

void Display::Clear(float r, float g, float b, float a) {
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool Display::IsClosed() {
	return m_isClosed;
}

void Display::Update() {
	const bool* keys = SDL_GetKeyboardState(nullptr);
	SDL_GL_SwapWindow(m_window);
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_EVENT_QUIT)
			m_isClosed = true;

		SDL_SetWindowTitle(
			m_window,
			("Teapot Collector - Score: " + std::to_string(score)).c_str()
		);
	}
}