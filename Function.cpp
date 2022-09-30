#include "Function.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include"stb_image.hpp"

void frame_buffer_size_callback(GLFWwindow *window, int width, int height){              // callback in case user resize window
    glViewport(0, 0, width, height);
};



void process_input(GLFWwindow *window){                                    // take user entered esc input to close window
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}



unsigned int set_basic_shader_program(){                                       // set vertex and fragment shader and link them in shader program
                                                                                       // create and compile vertex shader
    const char *vertex_shader_source = "#version 330 core\n"
    "layout (location = 0) in vec3 a_pos;\n"
    "layout (location = 1) in vec3 a_color;\n"
    "out vec3 our_color;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(a_pos, 1.0);\n"
    "   our_color = a_color;\n"
    "}\0";
    
    unsigned int vertex_shader{glCreateShader(GL_VERTEX_SHADER)};
    glShaderSource(vertex_shader, 1, &vertex_shader_source, nullptr);
    glCompileShader(vertex_shader);
    
    int vertex_success_compile{};                                                         // check if vertex shader successful compiled
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &vertex_success_compile);
    if(!vertex_success_compile)
        cout << "vertex shader compilation failed" << endl;
    
                                                                                // create and compile fragment shader
    const char *fragment_shader_source = "                \n\
    #version 330 core                                     \n\
    out vec4 frag_color;                                  \n\
    in vec3 our_color;                                    \n\
    void main(){                                          \n\
    frag_color = vec4(our_color, 1.0);                   \n\
    }\0";
    
    unsigned int fragment_shader{glCreateShader(GL_FRAGMENT_SHADER)};
    glShaderSource(fragment_shader, 1, &fragment_shader_source, nullptr);
    glCompileShader(fragment_shader);
    
    int fragment_success_compile{};                                                         // check if fragment shader successful compiled
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &fragment_success_compile);
    if(!fragment_success_compile)
        cout << "fragmet shader complie failed" << endl;

    unsigned int shader_program{glCreateProgram()};                                 // attach shader to program and link shaders
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    
    int program_successful_linked{};                                                      // check if shaders are successful linked
    glGetProgramiv(shader_program, GL_LINK_STATUS, &program_successful_linked);
    if(!program_successful_linked)
        cout << "failed to link shaders in program" << endl;
                                                                     //delete used shaders
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    
    return shader_program;
}



int create_texture(string image_file, bool is_rgb){                                                    // create, bind texture
    unsigned int texture{};
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);              // set repeat pattern (s,t)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);      // set filter method (min,mag)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    int width{}, height{}, nr_channels{};                                          // load image to texture
    const char* image_source = image_file.c_str();
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data {stbi_load(image_source, &width, &height, &nr_channels, 0)};
    if(data){
        if(is_rgb)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        else
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }else
        cout <<  stbi_failure_reason() << endl;
    stbi_image_free(data);
    return texture;
}
