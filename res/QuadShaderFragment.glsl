#version 410

out vec4 out_color;
in vec3 vertex_position;
uniform sampler2D TextureTarget;

void main() {
    vec4 texture_color = texture(TextureTarget, vec2(vertex_position.x+2,vertex_position.y+2)*2);
    out_color = texture_color;
}