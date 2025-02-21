#version 410

layout(location = 0) in vec2 in_position;

uniform float aspectRatio;
uniform mat4 view_projection_matrix;

void main()
{
    
    vec2 position = in_position;
    position.x=position.x/aspectRatio;
    position.x=position.x;
    position.y=position.y;
    gl_Position = view_projection_matrix * vec4(in_position, 0., 1.);
}