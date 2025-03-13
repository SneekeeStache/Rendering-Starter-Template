#version 410

layout(location = 1) in vec2 uv_position;
layout(location = 2) in vec2 render_position;

uniform float aspect_ratio;
uniform float time;
uniform mat4 view_projection_matrix;
out vec3 vertex_position;
out vec2 uv;

void main()
{
    //vec3 position = in_position;
    uv = uv_position;
    //vertex_position = in_position;
    vec2 render_pos = render_position;

    //position.x += 0.4;
    //position.y += 0.4; 
    //position.x /= aspect_ratio;
    //position.y /= aspect_ratio;
    //position.x /= aspect_ratio;
    //position.x += 0.1 * time;
    //position.y += 0.1 * time;

    //gl_Position = vec4(position, 1.);
    //gl_Position = vec4(position, 1.);
    //gl_Position = view_projection_matrix * vec4(in_position,  1.);
    gl_Position = vec4(render_pos, 0., 1.);
}