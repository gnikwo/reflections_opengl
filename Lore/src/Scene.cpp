#include "Scene.h"

#include <iostream>

#include "Camera.h"

using namespace std;
using namespace glm;
using namespace LORE;

Scene::Scene():	Node(),
                m__paused(false),
                m__environmentMap(),
                m__lights()
{
	cout << this << " [Scene] constructor" << endl;
}


Scene::~Scene()
{
	cout << this << " [ModelRender] destructor" << endl;
}


void Scene::renderLights()
{



}

