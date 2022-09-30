#include "Function.hpp"
#include"Shader.hpp"
#include<vector>
#include<cmath>

using namespace std;

const pair<float, float> window_size{800,800};

int main() {

    glfwInit();                                                           // initialize glfw options
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *window {glfwCreateWindow(window_size.first, window_size.second, "my_opengl", nullptr, nullptr)};           // create glfw window

    int screen_width{}, screen_height{};
    glfwGetFramebufferSize(window, &screen_width, &screen_height);

    if(!window){
        cout << "failed to create glfw window" << endl;
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){                                // use glad to load opengl functions
        cout << "glad initialization failed" << endl;
        return -1;
    }

    glViewport(0, 0, screen_width, screen_height);                                         // create oprngl viewport
    glfwSetFramebufferSizeCallback(window, frame_buffer_size_callback);

    float vertices[]{                                // 2 * 3 * 6 = 36 vertices for a cube (creative feature - 3D CUBE)
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

         -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
         -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    int texture_wood{create_texture("container.jpeg", true)};            // create two textures
    int texture_face{create_texture("jie.png", false)};

    unsigned int VAO{}, VBO{};                        // generate VAO, VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);                             // bind VAO, VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);                        // set VBO, VAO
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    Shader shader_program{"vertex_shader.txt", "fragment_shader.txt"};
    glEnable(GL_DEPTH_TEST);
    
    float x_velocity{0.01};                                   // speed for x-axis translation
    vector<pair<glm::vec3, float>> particle_info{};           // live particle collection
    bool active{};
    float size{0.25};               // default size(800 * (0.25 / 2) = 100px)
    glm::vec3 spawn_loc = glm::vec3(0.0f, 0.0f, 0.0f);
    float x_degree{};
    
    while(!glfwWindowShouldClose(window)){                           // display window program until quit (render loop start here)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        process_input(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader_program.use();
        
        glActiveTexture(GL_TEXTURE0);                               //(creative feature) bind textures
        glBindTexture(GL_TEXTURE_2D, texture_wood);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture_face);
        shader_program.set_fragment_uniform("texture_1", 0);
        shader_program.set_fragment_uniform("texture_2", 1);


        for(auto &item: particle_info){                            // display every live particle
            glm::mat4 model = glm::mat4(1.0f);                               // set model matrix
            model = glm::translate(model, item.first);
            model = glm::scale(model, glm::vec3(size, size, size));
            model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
            shader_program.set_model_matrix(model);
            
            glm::mat4 view = glm::mat4(1.0f);                                              // set view matrix
            view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
            shader_program.set_view_matrix(view);
            
            glm::mat4 proj = glm::mat4(1.0f);                                                                    // set projection matrix
            proj = glm::perspective(glm::radians(45.0f), window_size.first / window_size.second, 0.1f, 100.0f);
            shader_program.set_projection_matrix(proj);
            
            glBindVertexArray(VAO);                                         // render cube
            glDrawArrays(GL_TRIANGLES, 0, 36);
            
//            item.first.x += item.second;
            glm::mat4 temp = glm::mat4(1.0f);                                                  // define translation for next frame
            temp = glm::rotate(temp, glm::radians(x_degree), glm::vec3(0.0f, 0.0f, 1.0f));
            glm::vec4 translation_vec = glm::vec4(item.second, 0.0f, 0.0f, 1);
            translation_vec = temp * translation_vec;
            item.first.x += translation_vec.x;
            item.first.y += translation_vec.y;
            item.first.z += translation_vec.z;
        }
        
        if(glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)                // input lock
            active = true;
        
                                                                             // create particle on user input
        if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && active){
            glm::vec3 particle_location = spawn_loc;
            particle_info.push_back(pair<glm::vec3, float>{particle_location, x_velocity});
            if(particle_info.size() > 60)
                particle_info.erase(particle_info.begin());
            active = false;
        }
        
        if(glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS){  // increase size
            if(size < 1)
                size += 0.25;
            active = false;
        }
        
        if(glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS ){            // decrease size
            if(size > 0.25)
                size -= 0.25;
            active = false;
        }
        
        if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){              // increase speed
            if(x_velocity < 0.02)
                x_velocity += 0.01;
            active = false;
        }
        
        if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){              // decrease speed
            if(x_velocity > 0.01)
                x_velocity -= 0.01;
            active = false;
        }
        
        if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){              // move emiter up
            spawn_loc.y += 0.025;
            active = false;
        }
        
        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){              // move emiter left
            spawn_loc.x -= 0.025;
            active = false;
        }
        
        if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){              // move emiter down
            spawn_loc.y -= 0.025;
            active = false;
        }
        
        if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){              // move emiter right
            spawn_loc.x += 0.025;
            active = false;
        }
        
        if(glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS){           // reset all particle attribute to default
            spawn_loc = glm::vec3(0.0f, 0.0f, 0.0f);
            x_velocity = 0.01;
            size = 0.25;
            x_degree = 0;
            active = false;
        }
        
        if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){           // emitter rotate left
            x_degree += 45;
            if(x_degree > 360)
                x_degree -= 360;
            active = false;
        }
        
        if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){           // emitter rotate right
            x_degree -= 45;
            if(x_degree < 0)
                x_degree += 360;
            active = false;
        }
        

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}











