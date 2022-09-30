#pragma once
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include "glad.h"
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


using namespace std;

class Shader{
private:
    unsigned int id;
    
public:
    Shader(string vertex_shader_file, string fragment_shader_file);                  // constructor
    void use() const;                // use method
    void set_x(float offset);           // set uniform method
    void set_fragment_uniform(string uniform_name, int uniform_value);             // set fragment uniform
    unsigned int get_id() {return this -> id;}
    void set_model_matrix(glm::mat4 matrix);
    void set_view_matrix(glm::mat4 matrix);
    void set_projection_matrix(glm::mat4 matrix);
};
