#include "Shader.hpp"

Shader::Shader(string vertex_shader_file, string fragment_shader_file){                    // constructor
    ifstream vertex_file{vertex_shader_file};
    ifstream fragment_file{fragment_shader_file};
    
    if(!(vertex_file && fragment_file)){
        cout << "shader file not found" << endl;
        return;
    }
    
    stringstream vertex_stream{}, fragment_stream{};
    vertex_stream << vertex_file.rdbuf();
    fragment_stream << fragment_file.rdbuf();
    
    string vertex_shader_code{vertex_stream.str()};
    string fragment_shader_code{fragment_stream.str()};
    
    const char* vertex_shader_source{vertex_shader_code.c_str()};
    const char* fragment_shader_source{fragment_shader_code.c_str()};
    
    unsigned int vertex_shader{glCreateShader(GL_VERTEX_SHADER)};                        // compile vertex shader
    glShaderSource(vertex_shader, 1, &vertex_shader_source, nullptr);
    glCompileShader(vertex_shader);
    
    int vertex_success_compile{};                                                          // check if vertex shader successful compiled
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &vertex_success_compile);
    if(!vertex_success_compile)
        cout << "vertex shader compilation failed" << endl;
    
    unsigned int fragment_shader{glCreateShader(GL_FRAGMENT_SHADER)};                   // compile fragment shader
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
    this -> id = shader_program;
    
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    
}



void Shader::use() const{                              // use method
    glUseProgram(this -> id);
}



void Shader::set_x(float offset){                                          // set uniform x_offset
    int offset_location {glGetUniformLocation(this -> id, "x_offset")};
    glUniform1f(offset_location, offset);
}



void Shader::set_fragment_uniform(string uniform_name, int uniform_value){                // set fragment uniform
    const char *name_source{uniform_name.c_str()};
    glUniform1i(glGetUniformLocation(this -> id, name_source), uniform_value);
}


void Shader::set_model_matrix(glm::mat4 matrix){
    int model_loc {glGetUniformLocation(this -> id, "model")};
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::set_view_matrix(glm::mat4 matrix){
    int view_loc {glGetUniformLocation(this -> id, "view")};
    glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(matrix));
}
void Shader::set_projection_matrix(glm::mat4 matrix){
    int proj_loc {glGetUniformLocation(this -> id, "projection")};
    glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(matrix));
}
