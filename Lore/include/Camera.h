#ifndef CAMERA_H
#define CAMERA_H

#ifndef PI
#define PI 3.14159265359
#endif


#include <GL/glew.h>
#include <SOIL.h>

// Includes GLM
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Node.h"

#include "Scene.h"
#include "Texture.h"
#include "Mesh.h"

namespace LORE
{

    class Camera: public Node
    {

        public:
            Camera(int width = 768, int height = 768, glm::vec3 position = glm::vec3(0, 0, 0), glm::vec3 pointCible = glm::vec3(1, 0, 0), glm::vec3 axeVertical = glm::vec3(0, 1, 0), float sensibilite = 0.04, float vitesse = 0.001);
            virtual ~Camera();

            void move(glm::vec3 pos);
            void deplacer(glm::vec3 direction = glm::vec3(0, 0, 0));
            void forward();
            void backward();
            void left();
            void right();
            void up();
            void down();
            void orienter(double xRel, double yRel);

            virtual void render();
            virtual void render(Node* renderer, glm::mat4 projection, glm::mat4 view, glm::mat4 model);


            virtual bool isEmpty() { return false; };
            virtual bool isObject() { return false; };
            virtual bool isCamera() { return true; };

            Scene* getScene(){ return m__scene; };
            void setScene(Scene* scene){ m__scene = scene; };

            float getSensibilite() const;
            float getVitesse() const;

            bool getFly() const;

            void setSensibilite(float sensibilite);
            void setVitesse(float vitesse);

            void setPointcible(glm::vec3 pointCible);

            void setBackgroundColor(glm::vec3 color) { m__backgroundColor = color; };

            void setRatio(double ratio) { m__ratio = ratio; updatePerspective(); };
            void setFar(double limit) { m__far = limit; updatePerspective(); };
            void setNear(double limit) { m__near = limit; updatePerspective(); };

            double getRatio() { return m__ratio; };
            double getFar() { return m__far; };
            double getNear() { return m__near; };

            GLuint getEnvironmentMap() { return m__environmentMap; };

        private:

            glm::mat4 getView();
            void updatePerspective();

            Scene* m__scene;

            glm::vec3 m_pointCible;

            glm::vec3 m_axeVertical;
            glm::vec3 m_deplacementLateral;
            glm::vec3 m_deplacement;

            double m_phi;
            double m_theta;
            glm::vec3 m_orientation;

            float m_sensibilite;
            float m_vitesse;

            bool m_fly;

            glm::mat4 m__projection;

            double m__ratio;
            double m__far;
            double m__near;

            glm::vec3 m__backgroundColor;

            GLuint m__fbo;
            GLuint m__rbo;
            Texture* m__render;
            GLuint m__environmentMap;
            Mesh* m__environmentCube;

    };

}

#endif
