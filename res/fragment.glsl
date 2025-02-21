#version 410

out vec4 out_color;
in vec3 vertex_position;
uniform vec2 uv;
uniform vec2 positionTexture;
uniform sampler2D textureCustom;

void main() {
    vec4 texture_color = texture(textureCustom, vec2(vertex_position.x+positionTexture.x,vertex_position.y+positionTexture.y)*uv);
    out_color = texture_color;
}