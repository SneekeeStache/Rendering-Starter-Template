#version 410

layout(location = 0) in vec2 in_position;

uniform float aspectRatio;
uniform float test;

void main()
{
    vec2 position = in_position;
    position.x=position.x/aspectRatio;
    position.x=position.x+test;
    position.y=position.y;
    gl_Position= vec4(position,0.,1.);
}