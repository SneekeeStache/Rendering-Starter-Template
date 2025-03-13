#version 410

//out vec4 out_color;
out vec4 out_render_color;
in vec3 vertex_position;
in vec2 uv;
uniform sampler2D render_texture;
uniform vec4 overlay_color;

void main()
{
    //out_color = vec4(0.2, 0.8, 0.4, 1.);
    //out_color = vec4(vertex_position, 1.);
    //out_color = vec4(uv.x, uv.y, 0., 1.);

    
    vec4 render_color = texture(render_texture, uv);
    //overlay_color = vec4(0.2, 0.8, 0.4, 0.1);
    float grayscale = dot(render_color.rgb, vec3(0.299, 0.587, 0.114));
    //out_render_color = vec4(vec3(grayscale), render_color.a);
    out_render_color = vec4(render_color.r, 0.0, 0.0, render_color.a);
}

    