#version 120

varying vec4 position;
varying vec3 mirrorNormal_V;
varying vec4 projected_position;

varying vec4 projected_n1;
varying vec4 projected_n2;
varying vec4 projected_n3;

varying vec3 color_V;
varying float vertex_distance;

uniform vec3 cameraPos;
uniform vec3 u_lightPosition;
uniform samplerCube u_environmentMap;

vec2 Barycentric(vec2 n1, vec2 n2, vec2 n3, vec2 pos)
{
    vec2 v0 = n2.xy - n1.xy;
    vec2 v1 = n3.xy - n1.xy;
    vec2 v2 = pos.xy - n1.xy;
    float d00 = dot(v0, v0);
    float d01 = dot(v0, v1);
    float d11 = dot(v1, v1);
    float d20 = dot(v2, v0);
    float d21 = dot(v2, v1);
    float denom = d00 * d11 - d01 * d01;
    float a = (d11 * d20 - d01 * d21) / denom;
    float b = (d00 * d21 - d01 * d20) / denom;
    return vec2(a, b);
}


void main()
{
    vec3 color = vec3(1.0, 0.0, 0.0);

    // ==== Barycenter ====
    vec2 center = Barycentric(projected_n1.xy / projected_n1.w, projected_n2.xy / projected_n2.w, projected_n3.xy / projected_n3.w, projected_position.xy / projected_position.w);
    if(center.x == 0 || center.y == 0)
        color = vec3(1.0, 0.0, 0.0);
    else if(center.x < 0 && center.y < 0)
        color = vec3(1.0, 0.0, 0.0);
    else if(center.x > 0 && center.y > 0)
        color = vec3(1.0, 0.0, 0.0);
    else
        discard;
    
/*    vec2 diff = center; 
    if(sqrt(diff.x * diff.x + diff.y * diff.y) > 0.5)
        discard;
*/
    // =================

    if(vertex_distance > 0)
        discard;

    //color = color_V;

    gl_FragColor = vec4(color, 1.0);
}
