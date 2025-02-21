#include "opengl-framework/opengl-framework.hpp" // Inclue la librairie qui va nous servir à faire du rendu
#include "iostream"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"



int main()
{
    
    // Initialisation
    gl::init("TPs de Rendering"); // On crée une fenêtre et on choisit son nom
    gl::maximize_window(); // On peut la maximiser si on veut
    
    auto camera = gl::Camera{};
    gl::set_events_callbacks({
        camera.events_callbacks(),
        {
            .on_mouse_pressed = [&](gl::MousePressedEvent const& e) {
                std::cout << "Mouse pressed at " << e.position.x << " " << e.position.y << '\n';
            },
        },
    
    });


    auto const rectangle_mesh = gl::Mesh{{
        .vertex_buffers = {{
            .layout = {gl::VertexAttribute::Position2D{0}},
            .data   = {
                -0.5f, -0.5f, // Position2D du 1er sommet
                +0.5f, -0.5f, // Position2D du 2ème sommet
                +0.5f, +0.5f, // Position2D du 3ème sommet
                -0.5f, +0.5f  // Position2D du 4ème sommet
            },
        }},
        .index_buffer   = {
            0, 1, 2, // Indices du premier triangle : on utilise le 1er, 2ème et 3ème sommet
            0, 2, 3  // Indices du deuxième triangle : on utilise le 1er, 3ème et 4ème sommet
        },
    }};

    auto const shader = gl::Shader{{
        .vertex   = gl::ShaderSource::File{"res/vertex.glsl"},
        .fragment = gl::ShaderSource::File{"res/fragment.glsl"},
    }};

    while (gl::window_is_open())
    {
        // Rendu à chaque frame
        
        glClearColor(0.f, 0.f, 1.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);
        glm::mat4 const view_matrix = camera.view_matrix();

        glm::mat4 const projection_matrix = glm::infinitePerspective(1.f /*field of view in radians*/, gl::framebuffer_aspect_ratio() /*aspect ratio*/, 0.001f /*near plane*/);
        glm::mat4 const rotation = glm::rotate(glm::mat4{1.f}, gl::time_in_seconds() /*angle de la rotation*/, glm::vec3{0.f, 0.f, 1.f} /* axe autour duquel on tourne */);
        glm::mat4 const translation = glm::translate(glm::mat4{1.f}, glm::vec3{0.f, 1.f, 0.f} /* déplacement */);    

        //glm::mat4 const view_projection_matrix = projection_matrix*view_matrix;
        glm::mat4 const view_projection_matrix_Tran_Rota = projection_matrix*view_matrix*(translation*rotation);

        shader.bind();
        shader.set_uniform("aspectRatio",gl::framebuffer_aspect_ratio());
        shader.set_uniform("view_projection_matrix",view_projection_matrix_Tran_Rota);
        rectangle_mesh.draw();
        
    }
}