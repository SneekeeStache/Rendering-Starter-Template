#version 410
uniform float movementPositonX;
uniform float movementPositonY;
layout(location = 0) in vec2 in_position;

void main() {
    vec2 position = in_position;
    position.x=position.x+movementPositonX;
    position.y=position.y+movementPositonY;
    gl_Position= vec4(position,0.,1.);
}