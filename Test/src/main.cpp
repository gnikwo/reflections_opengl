#include <iostream>
#include <string>

// Includes GLM
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>

#include <Lore.h>
#include <Light.h>
#include <Cube.h>
#include <Plan.h>

#include "Diamond.h"

using namespace std;
using namespace glm;
using namespace LORE;

int main(int argc, char** argv)
{
    cout << "===== INIT =====" << endl;

    LORE::Window* window = LORE::Lore::init(); // Initializes OpenGL context and creates a Window
    if(!window)
    {
        cout << "Error during OpenGL context initialization." << endl;
        Lore::unload();
        return -1;
    }

    string file = "./Objects/empty.gltf";
    if(argc >= 2)
    {
        file = argv[1];
    }

    cout << "===Importing===" << endl;
    Scene* default_scene = Lore::load(file);
    cout << "===Finished importing===" << endl;
    cout << "Scene_ " << default_scene << endl;
    Camera* camera = Lore::getCamera("Camera");
    if(!camera)
    {
        cout << "Could'nt find Camera, creating one..." << endl;
        camera = Lore::createCamera("Camera");
        camera->setPosition(vec3(5, 5, 5));
        camera->setScene(default_scene);
    }
	camera = Lore::createCamera("Camera");
	camera->setPosition(vec3(-1, 0, 1));
	camera->setScene(default_scene);
    window->setCamera(camera);

    Lore::listCameras();

    Material* mat_reflections = Lore::createMaterial("true_reflections");
    mat_reflections->load();
    Shader* shader_reflections = Lore::createShader("true_reflections");
    shader_reflections->load();
    mat_reflections->setShader(shader_reflections);
    mat_reflections->setCulling(false);

    auto diamond = new Plan();
    diamond->load();
    diamond->setMaterial(mat_reflections);
    
    auto obj = Lore::createObject("cube");
    obj->setMesh(diamond);
    default_scene->addChild(obj);
    
    auto obj_ = Lore::createObject("cube");
    obj_->setMesh(diamond);
    default_scene->addChild(obj_);

    obj_->setPosition(vec3(2,0,2));


    float a = sqrt(2.0)/2.0, b = 0, c = -sqrt(2.0)/2.0;
    vec3 mirrorNormal = vec3(a, b, c);
    mat4 reflection;
    vec3 cameraPos;
    mat_reflections->addCustomVec3Uniform("cameraPos", &cameraPos);
    mat_reflections->addCustomMat4Uniform("reflection", &reflection);
    reflection = mat4(
            vec4(1-2*a*a, -2*a*b, -2*a*c, 0), 
            vec4(-2*a*b, 1-2*b*b, 0-2*b*c, 0), 
            vec4(-2*a*c, -2*b*c, 1-2*c*c,0), 
            vec4(0,0,0,1));
    mat_reflections->addCustomVec3Uniform("mirrorNormal", &mirrorNormal);
    

    bool display_world = true;

    //----------

    auto controller = new Controller(); // a Controller to bind the ESCAPE key to the Window

    controller->bind(GLFW_KEY_ESCAPE, [&window](double x, double y) {
        window->close();
    });
    
    controller->bind(GLFW_KEY_W, [&window](double x, double y) {
        window->getCamera()->forward();
    });
    controller->bind(GLFW_KEY_S, [&window](double x, double y) {
        window->getCamera()->backward();
    });
    controller->bind(GLFW_KEY_A, [&window](double x, double y) {
        window->getCamera()->left();
    });
    controller->bind(GLFW_KEY_D, [&window](double x, double y) {
        window->getCamera()->right();
    });
    controller->bind(GLFW_KEY_SPACE, [&window](double x, double y) {
        window->getCamera()->up();
    });
    controller->bind(GLFW_KEY_LEFT_SHIFT, [&window](double x, double y) {
        window->getCamera()->down();
    });

    controller->setMouseEvent([&window](double x, double y, double dx, double dy){
        window->getCamera()->orienter(dx, dy);
    });
    
	controller->bind(GLFW_KEY_T, [&window](double x, double y) {
		window->setCamera(Lore::getCamera("Camera"));
    });
    
	controller->bind(GLFW_KEY_G, [&window](double x, double y) {
		window->setCamera(Lore::getCamera("Camera_1"));
    });
	
    controller->bind(GLFW_KEY_P, [&display_world](double x, double y) {
            display_world = !display_world;
    });

    float time = 0;

    //===================================
    cout << "===== RENDER =====" << endl;

    while (!window->shouldClose())
    {
        int start = window->startFrame(); // Begin the frame render process

        controller->check(window); // Checks all bindings for the Window and execute the fonction if it matches
        cameraPos = camera->getPosition();
        time += 0.01;
        Light::lightPosition = vec3(sin(time)*6, 3, cos(time)*6);
        //obj->setPosition(Light::lightPosition);
        quat myQuat = quat(vec3(0,-time,0));
        //obj->setRotation(myQuat);
        

        diamond->setMaterial(mat_reflections);
        
        glClearColor(0.1, 0.1, 0.1, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        window->render(); //

        if(display_world)
        {
            diamond->setMaterial(Lore::getMaterial("basic"));

            window->render(); //
        }
        window->endFrame(start); // End the frame render process and display the image on the window
    }

    //================================
    cout << "===== END =====" << endl;

    delete controller;
    delete diamond;

    Lore::unload(); // Unload all dictionnaries, and unload OpenGL context

    cout << "===== SUCCESSFULLY ENDED =====" << endl;
    return 0;
}
