#version 410

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 uv_position;
layout(location = 2) in vec3 normal_position;

uniform float aspect_ratio;
uniform float time;
uniform mat4 model_matrix;
uniform mat4 view_projection_matrix;
out vec3 vertex_position;
out vec2 uv;
out vec3 normal;


void main()
{
    vec3 position = in_position;
    uv = uv_position;
    vertex_position = in_position;

    //position.x += 0.4;
    //position.y += 0.4; 
    //position.x /= aspect_ratio;
    //position.y /= aspect_ratio;
    normal= vec3(model_matrix*vec4(normal_position,0));
    position.x /= aspect_ratio;
    //position.x += 0.1 * time;
    //position.y += 0.1 * time;
    //gl_Position = vec4(position, 1.);
    gl_Position = vec4(position, 1.);
    gl_Position = view_projection_matrix * vec4(in_position,  1.);
}