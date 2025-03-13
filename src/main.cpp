#include "opengl-framework/opengl-framework.hpp" // Inclue la librairie qui va nous servir à faire du rendu
#include "iostream"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

auto load_mesh(std::filesystem::path const& path) -> gl::Mesh
{
    // On lit le fichier avec tinyobj
    auto reader = tinyobj::ObjReader{};
    reader.ParseFromFile(gl::make_absolute_path(path).string(), {});

    if (!reader.Error().empty())
        throw std::runtime_error("Failed to read 3D model:\n" + reader.Error());
    if (!reader.Warning().empty())
        std::cout << "Warning while reading 3D model:\n" + reader.Warning();

    // On met tous les attributs dans un tableau
    auto vertices = std::vector<float>{};
    for (auto const& shape : reader.GetShapes())
    {
        for (auto const& idx : shape.mesh.indices)
        {
            // Position
            vertices.push_back(reader.GetAttrib().vertices[3 * idx.vertex_index + 0]);
            vertices.push_back(reader.GetAttrib().vertices[3 * idx.vertex_index + 2]);
            vertices.push_back(reader.GetAttrib().vertices[3 * idx.vertex_index + 1]);

            // UV
            vertices.push_back(reader.GetAttrib().texcoords[2 * idx.texcoord_index + 0]);
            vertices.push_back(reader.GetAttrib().texcoords[2 * idx.texcoord_index + 1]);

            // Normale
            vertices.push_back(reader.GetAttrib().normals[3 * idx.normal_index + 0]);
            vertices.push_back(reader.GetAttrib().normals[3 * idx.normal_index + 2]);
            vertices.push_back(reader.GetAttrib().normals[3 * idx.normal_index + 1]);
        };
    }
    
    // TODO créer et return un gl::mesh, qui utilisera le tableau `vertices` en tant que `data` pour son vertex buffer.
    // Attention, il faudra bien spécifier le layout pour qu'il corresponde à l'ordre des attributs dans le tableau `vertices`.
    return gl::Mesh{{
        .vertex_buffers = {{
            .layout = {gl::VertexAttribute::Position3D{0}, gl::VertexAttribute::UV{1},gl::VertexAttribute::Normal3D{2}},
            .data   = vertices,
        }},
    }};
    
}

int main()
{
    
    // Initialisation
    gl::init("TPs de Rendering"); // On crée une fenêtre et on choisit son nom
    gl::maximize_window(); // On peut la maximiser si on veut

    auto camera = gl::Camera{};
    //gl::set_events_callbacks({camera.events_callbacks()});
    gl::set_events_callbacks({
        camera.events_callbacks(),
        {
            .on_mouse_pressed = [&](gl::MousePressedEvent const& e) {
                std::cout << "Mouse pressed at " << e.position.x << " " << e.position.y << '\n';
            },
        },
    });

    glEnable(GL_DEPTH_TEST);

    auto const shader = gl::Shader{{
        .vertex   = gl::ShaderSource::File{"res/vertex.glsl"},
        .fragment = gl::ShaderSource::File{"res/fragment.glsl"},
    }};

    auto const render_shader = gl::Shader{{
        .vertex   = gl::ShaderSource::File{"res/QuadShaderVertex.glsl"},
        .fragment = gl::ShaderSource::File{"res/QuadShaderFragment.glsl"},
    }};

    auto const uv_mesh = gl::Mesh{{
        .vertex_buffers = {{
            .layout = {gl::VertexAttribute::Position2D{0}, gl::VertexAttribute::UV{1}},
            .data   = {
                -1.0f, -1.0f, 0.8f, 0.8f, // Position2D du 1er sommet
                +1.0f, -1.0f, +0.9f, 0.8f, // Position2D du 2ème sommet
                +1.0f, +1.0f, +0.9f, +0.9f, // Position2D du 3ème sommet
                -1.0f, +1.0f, 0.8f, +0.9f  // Position2D du 4ème sommet
            },
        }},
        .index_buffer   = {
            0, 1, 2, // Indices du premier triangle : on utilise le 1er, 2ème et 3ème sommet
            0, 2, 3  // Indices du deuxième triangle : on utilise le 1er, 3ème et 4ème sommet
        },
    }};

    auto const render_mesh = gl::Mesh{{
        .vertex_buffers = {{
            .layout = {gl::VertexAttribute::Position2D{2}, gl::VertexAttribute::UV{1}},
            .data   = {
                -1.0f, -1.0f, 0.0f, 0.0f, // Position2D du 1er sommet
                +1.0f, -1.0f, +1.0f, 0.0f, // Position2D du 2ème sommet
                +1.0f, +1.0f, +1.0f, +1.0f, // Position2D du 3ème sommet
                -1.0f, +1.0f, 0.0f, +1.0f  // Position2D du 4ème sommet
            },
        }},
        .index_buffer   = {
            0, 1, 2, // Indices du premier triangle : on utilise le 1er, 2ème et 3ème sommet
            0, 2, 3  // Indices du deuxième triangle : on utilise le 1er, 3ème et 4ème sommet
        },
    }};

    auto const texture = gl::Texture{
        gl::TextureSource::File{ // Peut être un fichier, ou directement un tableau de pixels
            .path           = "res/pierre.png",
            .flip_y         = true, // Il n'y a pas de convention universelle sur la direction de l'axe Y. Les fichiers (.png, .jpeg) utilisent souvent une direction différente de celle attendue par OpenGL. Ce booléen flip_y est là pour inverser la texture si jamais elle n'apparaît pas dans le bon sens.
            .texture_format = gl::InternalFormat::RGBA8, // Format dans lequel la texture sera stockée. On pourrait par exemple utiliser RGBA16 si on voulait 16 bits par canal de couleur au lieu de 8. (Mais ça ne sert à rien dans notre cas car notre fichier ne contient que 8 bits par canal, donc on ne gagnerait pas de précision). On pourrait aussi stocker en RGB8 si on ne voulait pas de canal alpha. On utilise aussi parfois des textures avec un seul canal (R8) pour des usages spécifiques.
        },
        gl::TextureOptions{
            .minification_filter  = gl::Filter::NearestNeighbour, // Comment on va moyenner les pixels quand on voit l'image de loin ?
            .magnification_filter = gl::Filter::NearestNeighbour, // Comment on va interpoler entre les pixels quand on zoom dans l'image ?
            .wrap_x               = gl::Wrap::Repeat,   // Quelle couleur va-t-on lire si jamais on essaye de lire en dehors de la texture ?
            .wrap_y               = gl::Wrap::Repeat,   // Idem, mais sur l'axe Y. En général on met le même wrap mode sur les deux axes.
        }
    };

    auto render_target = gl::RenderTarget{gl::RenderTarget_Descriptor{
        .width          = gl::framebuffer_width_in_pixels(),
        .height         = gl::framebuffer_height_in_pixels(),
        .color_textures = {
            gl::ColorAttachment_Descriptor{
                .format  = gl::InternalFormat_Color::RGBA8,
                .options = {
                    .minification_filter  = gl::Filter::NearestNeighbour, // On va toujours afficher la texture à la taille de l'écran,
                    .magnification_filter = gl::Filter::NearestNeighbour, // donc les filtres n'auront pas d'effet. Tant qu'à faire on choisit le moins coûteux.
                    .wrap_x               = gl::Wrap::ClampToEdge,
                    .wrap_y               = gl::Wrap::ClampToEdge,
                },
            },
        },
        .depth_stencil_texture = gl::DepthStencilAttachment_Descriptor{
            .format  = gl::InternalFormat_DepthStencil::Depth32F,
            .options = {
                .minification_filter  = gl::Filter::NearestNeighbour,
                .magnification_filter = gl::Filter::NearestNeighbour,
                .wrap_x               = gl::Wrap::ClampToEdge,
                .wrap_y               = gl::Wrap::ClampToEdge,
            },
        },
    }};

    gl::set_events_callbacks({
        camera.events_callbacks(),
        {.on_framebuffer_resized = [&](gl::FramebufferResizedEvent const& e) {
            if(e.width_in_pixels != 0 && e.height_in_pixels != 0) // OpenGL crash si on tente de faire une render target avec une taille de 0
                render_target.resize(e.width_in_pixels, e.height_in_pixels);
        }},
    });

    auto const cube_mesh = gl::Mesh{{
        .vertex_buffers = {{
            .layout = {gl::VertexAttribute::Position3D{0}, gl::VertexAttribute::UV{1}},
            .data   = {
                -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,// Position3D du 1er sommet
                +0.5f, -0.5f, -0.5f,  1.0f, 0.0f,// Position3D du 2ème sommet
                +0.5f, +0.5f, -0.5f,  1.0f, 1.0f,// Position3D du 3ème sommet
                -0.5f, +0.5f, -0.5f,  0.0f, 1.0f,// Position3D du 4ème sommet
                -0.5f, -0.5f, +0.5f,  0.0f, 0.0f,// Position3D du 5ème sommet
                +0.5f, -0.5f, +0.5f,  1.0f, 0.0f,// Position3D du 6ème sommet
                +0.5f, +0.5f, +0.5f,  1.0f, 1.0f,// Position3D du 7ème sommet
                -0.5f, +0.5f, +0.5f,  0.0f, 1.0f,// Position3D du 8ème sommet
            },
        }},
        .index_buffer   = {
            0, 1, 2, // Indices du premier triangle : on utilise le 1er, 2ème et 3ème sommet
            0, 2, 3,  // Indices du deuxième triangle : on utilise le 1er, 3ème et 4ème sommet
            4, 5, 6,
            4, 6, 7,
            1, 2, 6,
            1, 5, 6,
            0, 1, 5,
            0, 4, 5,
            3, 0, 4,
            3, 7, 4,
            2, 3, 7,
            2, 6, 7
        },
    }};
    auto const CustomMesh = load_mesh("res/fourareen.obj");
    

    while (gl::window_is_open())
    {
        // Rendu à chaque frame
        glm::mat4 const view_matrix = camera.view_matrix();
        glm::mat4 const projection_matrix = glm::infinitePerspective(1.f /*field of view in radians*/, gl::framebuffer_aspect_ratio() /*aspect ratio*/, 0.001f /*near plane*/);

        glm::mat4 const view_projection_matrix = projection_matrix * view_matrix;
        
        glm::mat4 const rotation = glm::rotate(glm::mat4{1.f}, gl::time_in_seconds() /*angle de la rotation*/, glm::vec3{0.f, 1.f, 0.f} /* axe autour duquel on tourne */);
        glm::mat4 const translation = glm::translate(glm::mat4{1.f}, glm::vec3{0.f, 1.f, 0.f} /* déplacement */);   

        glm::mat4 const model_matrix = translation * rotation; 
        glm::mat4 const model_view_projection_matrix = view_projection_matrix * model_matrix;

        glClearColor(0.6f, 0.2f, 0.5f, 1.f); // Choisis la couleur à utiliser. Les paramètres sont R, G, B, A avec des valeurs qui vont de 0 à 1
        glClear(GL_COLOR_BUFFER_BIT); // Exécute concrètement l'action d'appliquer sur tout l'écran la couleur choisie au-dessus
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Vient remplacer glClear(GL_COLOR_BUFFER_BIT);
        
        shader.bind(); // On a besoin qu'un shader soit bind (i.e. "actif") avant de draw(). On en reparle dans la section d'après.
        shader.set_uniform("model_matrix",model_matrix);
        shader.set_uniform("view_projection_matrix", model_view_projection_matrix);
        shader.set_uniform("aspect_ratio",gl::framebuffer_aspect_ratio());
        shader.set_uniform("time", gl::time_in_seconds());
        shader.set_uniform("my_texture", texture);
        shader.set_uniform("light_direction",glm::normalize(glm::vec3(0.0, 0.0, -1)));
        shader.set_uniform("positionPointLight",glm::vec3(1,0,0));
        //uv_mesh.draw(); // C'est ce qu'on appelle un "draw call" : on envoie l'instruction à la carte graphique de dessiner notre mesh.
        //cube_mesh.draw();
        

        render_target.render([&]() {
            glClearColor(1.f, 0.f, 0.f, 1.f); // Dessine du rouge, non pas à l'écran, mais sur notre render target
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            CustomMesh.draw();
        });
        
        render_shader.bind();
        render_shader.set_uniform("render_texture", render_target.color_texture(0));
        render_mesh.draw();
    }
}
