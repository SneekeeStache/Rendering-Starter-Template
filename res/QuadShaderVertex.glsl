#version 410
layout(location = 0) in vec2 position;  // Position du sommet
layout(location = 1) in vec2 uv;        // Coordonnées UV

out vec2 texCoords;
uniform float aspectRatio;

void main() {
    texCoords = uv;
    gl_Position = vec4(position.x/aspectRatio,position.y, 0.0, 1.0);
}