#ifndef MODEL_H
#define MODEL_H

#include <vector>

#include <GL/glew.h>

// Includes GLM
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Material.h"

#ifndef BUFFER_OFFSET

#define BUFFER_OFFSET(offset) ((char*)NULL + (offset))

#endif

namespace LORE
{

    class Mesh
    {

        public:

            Mesh();
            ~Mesh();

            void load();

            int getVertexCount();
            int getVerticesSize();
            void setVertices(std::vector<glm::vec3> vertices) { m__vertices = vertices; };
            std::vector<glm::vec3> getVertices();
            float* getVerticesFloat();

            void setMaterial(Material* mat) { m__material = mat; };
            Material* getMaterial() { return m__material;  };

        private:


            Material* m__material;

        protected:

            std::vector<glm::vec3> m__vertices;

            static float* vec3ToFloat(std::vector<glm::vec3>);

    };

}

#endif