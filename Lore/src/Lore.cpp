#include "Lore.h"

#include <iostream>

using namespace std;
using namespace LORE;

Library<LORE::Window>* Lore::m__windowLibrary = new Library<Window>();
Library<Scene>* Lore::m__sceneLibrary = new Library<Scene>();
Library<Camera>* Lore::m__cameraLibrary = new Library<Camera>();
Library<Node>* Lore::m__nodeLibrary = new Library<Node>();
Library<Mesh>* Lore::m__meshLibrary = new Library<Mesh>();
Library<Material>* Lore::m__materialLibrary = new Library<Material>();
Library<Texture>* Lore::m__textureLibrary = new Library<Texture>();
Library<Shader>* Lore::m__shaderLibrary = new Library<Shader>();

Lore::Lore()
{


}


Lore::~Lore()
{



}


void Lore::error_callback(int error, const char* description)
{
	cout << "[Window] error_callback() :" << description << "\n";
}

/**
 * Initialize Glfw and Glew and returns a Window
 * @return returns a window
 **/
LORE::Window* Lore::init()
{

	cout << "[Lore] init" << endl;

	glfwSetErrorCallback(error_callback);

	if (!glfwInit()) {

		std::cout << "Failed to initialize GLFW\n";
		return nullptr;

	}

	//=====================

    LORE::Window* w = Lore::createWindow("Window_1", "Test");
	w->load();

	//=====================

	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); //OpenGL 4.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

	// On initialise GLEW
	GLenum initialisationGLEW(glewInit());

	// Si l'initialisation a �chou� :
	if (initialisationGLEW != GLEW_OK)
	{

		// On affiche l'erreur gr�ce � la fonction : glewGetErrorString(GLenum code)
		std::cout << "Erreur d'initialisation de GLEW : " << glewGetErrorString(initialisationGLEW) << std::endl;

		glfwTerminate();
		exit(EXIT_FAILURE);

	}

    Material* default_material = Lore::createMaterial("default");
    default_material->load();
    Shader* default_shader = Lore::createShader("default");
    default_shader->load();
    Shader* default_shader_120 = Lore::createShader("default-120");
    default_shader_120->load();

    default_material->setShader(default_shader_120);
    
    Material* mat_basic = Lore::createMaterial("basic");
    mat_basic->load();
    mat_basic->setCulling(false);
    Shader* shader_basic = Lore::createShader("basic");
    shader_basic->load();
    mat_basic->setShader(shader_basic);
    
    Material* mat_environmentMap = Lore::createMaterial("environmentMap");
    mat_environmentMap->load();
    Shader* shader_environmentMap = Lore::createShader("environmentMap");
    shader_environmentMap->load();
    mat_environmentMap->setShader(shader_environmentMap);
    mat_environmentMap->setCulling(false);
    
	return w;
}


void Lore::unload()
{
	Lore::m__windowLibrary->flush();
	Lore::m__sceneLibrary->flush();
	Lore::m__cameraLibrary->flush();
	Lore::m__nodeLibrary->flush();
	Lore::m__meshLibrary->flush();
	Lore::m__materialLibrary->flush();
	Lore::m__textureLibrary->flush();
	Lore::m__shaderLibrary->flush();
	glfwTerminate();
}


bool Lore::importScene(string id, Scene* scene)
{
	Lore::m__sceneLibrary->add(id, scene);
	return true;
}


bool Lore::importNode(string id, Node* node)
{

    cout << "[Lore]: importNode" << endl;
    Lore::m__nodeLibrary->add(id, node);
	return true;
}


bool Lore::importMesh(string id, Mesh* mesh)
{
    cout << "[Lore]: importMesh" << endl;
	Lore::m__meshLibrary->add(id, mesh);
    return true;
}


LORE::Window* Lore::createWindow(string id, string name)
{
    LORE::Window* w = new Window(name);
	Lore::m__windowLibrary->add(id, w);
	return w;
}


Scene* Lore::createScene(string id)
{
    Scene* s = new Scene();
    Lore::m__sceneLibrary->add(id, s);
    return s;
}


Node* Lore::createNode(string id)
{
    Node* n = new Node();
    Lore::m__nodeLibrary->add(id, n);
    return n;
}


Object* Lore::createObject(string id)
{
    Object* o = new Object();
    Lore::m__nodeLibrary->add(id, o);
    return o;
}


Camera* Lore::createCamera(string id)
{
    Camera* c = new Camera();
    Lore::m__cameraLibrary->add(id, c);
    return c;
}


Mesh* Lore::createMesh(string id)
{
    Mesh* o = new Mesh();
    Lore::m__meshLibrary->add(id, o);
    return o;
}


Material* Lore::createMaterial(string id)
{
	Material* m = new Material();
	Lore::m__materialLibrary->add(id, m);
	return m;
}


Texture* Lore::createTexture(string id)
{
	Texture* t = new Texture(id);
	Lore::m__textureLibrary->add(id, t);
	return t;
}


Shader* Lore::createShader(string id)
{
	Shader* s = new Shader(id);
	Lore::m__shaderLibrary->add(id, s);
	return s;
}


Scene* Lore::load(string path)
{
    Importer importer(path);
    return importer.import();
}


Scene* Lore::getScene(string id)
{
    return m__sceneLibrary->get(id);
}


Camera* Lore::getCamera(string id)
{
    return m__cameraLibrary->get(id);
}

void Lore::listCameras()
{
    m__cameraLibrary->list();
}


Node* Lore::getNode(string id)
{
    return m__nodeLibrary->get(id);
}


Mesh* Lore::getMesh(string id)
{
    return m__meshLibrary->get(id);
}


Texture* Lore::getTexture(string id)
{
    return m__textureLibrary->get(id);
}


Material* Lore::getMaterial(string id)
{
    return m__materialLibrary->get(id);
}


Shader* Lore::getShader(string id)
{
    return m__shaderLibrary->get(id);
}
