#include "opengl-framework/opengl-framework.hpp" // Inclue la librairie qui va nous servir à faire du rendu
#include "iostream"
int main()
{
    float initialPositionX = 0;
    float initialPositionY = 0;
    bool reverseX = false;
    bool reverseY = false;
    bool dvd = false;

    // Initialisation
    gl::init("TPs de Rendering"); // On crée une fenêtre et on choisit son nom
    gl::maximize_window();        // On peut la maximiser si on veut

    

    auto const DVDShader = gl::Shader{{
        .vertex = gl::ShaderSource::File{"res/DVD.glsl"},
        .fragment = gl::ShaderSource::File{"res/fragment.glsl"},
    }};

    auto const Shader = gl::Shader{{
        .vertex = gl::ShaderSource::File{"res/vertex.glsl"},
        .fragment = gl::ShaderSource::File{"res/fragment.glsl"},
    }};

    auto const triangle_mesh = gl::Mesh{{
        .vertex_buffers = {{
            .layout = {gl::VertexAttribute::Position2D{0 /*Index de l'attribut dans le shader, on en reparle juste après*/}},
            .data = {
                -0.5f, -0.5f, // Position2D du 1er sommet
                +1.f, -1.f,   // Position2D du 2ème sommet
                0.f, +1.f,    // Position2D du 3ème sommet
            },
        }},
    }};
    auto const rectangle_mesh = gl::Mesh{{
        .vertex_buffers = {{
            .layout = {gl::VertexAttribute::Position2D{0}},
            .data = {
                -0.5f, -0.5f, // Position2D du 1er sommet
                +0.5f, -0.5f, // Position2D du 2ème sommet
                +0.5f, +0.5f, // Position2D du 3ème sommet
                -0.5f, +0.5f  // Position2D du 4ème sommet
            },
        }},
        .index_buffer = {
            0, 1, 2, // Indices du premier triangle : on utilise le 1er, 2ème et 3ème sommet
            0, 2, 3  // Indices du deuxième triangle : on utilise le 1er, 3ème et 4ème sommet
        },
    }};

    while (gl::window_is_open())
    {
        // Rendu à chaque frame
        glClearColor(1.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);
        glEnable(GL_BLEND);
        glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA, GL_ONE); // On peut configurer l'équation qui mélange deux couleurs, comme pour faire différents blend mode dans Photoshop. Cette équation-ci donne le blending "normal" entre pixels transparents.

        // gl::bind_default_shader(); // On a besoin qu'un shader soit bind (i.e. "actif") avant de draw(). On en reparle dans la section d'après.
        // triangle_mesh.draw(); // C'est ce qu'on appelle un "draw call" : on envoie l'instruction à la carte graphique de dessiner notre mesh.

        // gl::bind_default_shader();

        if (dvd)
        {
            // dvd shader
            if (initialPositionX >= 0.5f)
            {
                reverseX = true;
            }
            else if (initialPositionX <= -0.5)
            {
                reverseX = false;
            }
            if (initialPositionY >= 0.5f)
            {
                reverseY = true;
            }
            else if (initialPositionY <= -0.5)
            {
                reverseY = false;
            }

            if (reverseX)
            {
                initialPositionX = initialPositionX - 0.0002f;
            }
            else
            {
                initialPositionX = initialPositionX + 0.0002f;
            }

            if (reverseY)
            {
                initialPositionY = initialPositionY - 0.0004f;
            }
            else
            {
                initialPositionY = initialPositionY + 0.0004f;
            }
            DVDShader.bind(); // accroche le shader fait main au lieu du default shader

            DVDShader.set_uniform("movementPositonX", initialPositionX);
            DVDShader.set_uniform("movementPositonY", initialPositionY);

            rectangle_mesh.draw();

        }else{
            
            Shader.bind();
            Shader.set_uniform("aspectRatio",gl::framebuffer_aspect_ratio());
            Shader.set_uniform("test",gl::time_in_seconds());
            rectangle_mesh.draw();

            std::cout << gl::time_in_seconds()<< std::endl;
        }

        

        
    }
}