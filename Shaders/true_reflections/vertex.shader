#version 120

attribute vec3 in_Vertex;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;
uniform mat4 reflection;

uniform vec3 cameraPos;
uniform vec3 mirrorNormal;

varying vec4 vertex_position;
varying vec3 mirrorNormal_V;
varying vec4 projected_position;

varying vec4 projected_n1;
varying vec4 projected_n2;
varying vec4 projected_n3;

varying vec3 color_V;
varying float vertex_distance;


void main()
{

    vec4 worldSpace = reflection * model * vec4(in_Vertex, 1.0);
    vec4 cameraSpace = view * worldSpace; 
    vec4 screenSpace = projection * cameraSpace;
    gl_Position = screenSpace;

    projected_position = screenSpace;
    

    color_V = vec3(0.0, 0.0, 1.0);
    float distance_camera = dot(mirrorNormal, cameraPos.xyz);
    if(distance_camera > 0){
        color_V = vec3(1.0, 1.0, 0.0);
        mirrorNormal_V = mirrorNormal;
    }else{
        color_V = vec3(0.0, 1.0, 0.0);
        mirrorNormal_V = -mirrorNormal;
    }
    

    // ===== REFRACTION ====
    //vec3 incidence = worldSpace.xyz - cameraPos;
    //vec3 rotation_plan = cross(incidence, normal);



    vertex_distance = dot(mirrorNormal_V, worldSpace.xyz);


    // ==== N1 N2 N3 ====
    vec3 n1 = vec3(-0.5,  0.5, -0.5);
    vec3 n2 = vec3(-0.5, -0.5, -0.5);
    vec3 n3 = vec3( 0.5, -0.5,  0.5);

    projected_n1 = (projection * view * vec4(n1, 1));
    projected_n2 = (projection * view * vec4(n2, 1));
    projected_n3 = (projection * view * vec4(n3, 1));
    // ==================
    
}
