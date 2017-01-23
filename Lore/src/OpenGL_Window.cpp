//#include "stdafx.h"
#include "OpenGL_Window.h"

#include <iostream>

#include <chrono>
#include <thread>

using namespace std;
using namespace glm;

OpenGL_Window::OpenGL_Window(std::string tittle, int width, int height, int fps): m__window(),
																		m__title(tittle),
																		m__width(width),
																		m__height(height),
																		m__frameRate(int(1000/fps)),
																		m__scene(),
																		m__camera()
{



}


OpenGL_Window::~OpenGL_Window()
{

	cout << this << " [Window] destructor" << endl;
	if(m__scene)
		delete m__scene;
	if(m__camera)
		delete m__camera;

}


void OpenGL_Window::error_callback(int error, const char* description)
{
	cout << "[Window] error_callback() :" << description << "\n";
}


void OpenGL_Window::window_size_callback(GLFWwindow* window, int width, int height)
{

	cout << "[Window] window_size_callback" << endl;

}


void OpenGL_Window::window_focus_callback(GLFWwindow* window, int state)
{

	cout << "[Window] window_focus_callback" << endl;

	//static_cast<Window*>(glfwGetWindowUserPointer(window))->m__paused = !state;

}

void OpenGL_Window::window_close_callback(GLFWwindow* window)
{

	cout << "[Window] window_close_callback" << endl;

	OpenGL_Window* w = static_cast<OpenGL_Window*>(glfwGetWindowUserPointer(window));

	w->close();

}

void OpenGL_Window::mouse_move_callback(GLFWwindow* window, double x, double y)
{

	//cout << "[Window] mouse_move_callback" << endl;

}


void OpenGL_Window::load()
{

    cout << this << " [OpenGL_Window] load" << endl;

	// Cr�ation de la fen�tre
	m__window = glfwCreateWindow(m__width, m__height, m__title.c_str(), NULL, NULL);
	if (!m__window)
	{

		std::cout << "Erreur lors de la creation de la fenetre" << std::endl;

		glfwTerminate();
		exit(EXIT_FAILURE);

	}


	//glfwSetKeyCallback(m__window, OpenGL_Window::key_callback);

	glfwMakeContextCurrent(m__window);

	glfwSetInputMode(m__window, GLFW_STICKY_KEYS, 1);

	glfwSetWindowUserPointer(m__window, this);
	glfwSetWindowSizeCallback(m__window, window_size_callback);
	glfwSetWindowFocusCallback(m__window, window_focus_callback);
	glfwSetWindowCloseCallback(m__window, window_close_callback);
	glfwSetCursorPosCallback(m__window, mouse_move_callback);

}


void OpenGL_Window::close()
{

	glfwSetWindowShouldClose(m__window, GL_TRUE);

}

int OpenGL_Window::shouldClose()
{
	return glfwWindowShouldClose(m__window);
}


int OpenGL_Window::startFrame()
{

	int startTime = int(glfwGetTime() * 1000);



	return startTime;

}


void OpenGL_Window::endFrame(int startTime)
{

	glfwSwapBuffers(m__window);

	glfwPollEvents();

	int endTime = int(glfwGetTime() * 1000);
	int elapsedTime = endTime - startTime;

	//cout << elapsedTime << endl;

	if(elapsedTime < m__frameRate)
		this_thread::sleep_for(std::chrono::milliseconds(m__frameRate - elapsedTime));

}


void OpenGL_Window::render()
{

	glfwMakeContextCurrent(m__window);
	
	if(m__camera)
		m__camera->render();

}