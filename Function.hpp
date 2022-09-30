#pragma once
#include <iostream>
#include "glad.h"
#include <GLFW/glfw3.h>


using namespace std;

void frame_buffer_size_callback(GLFWwindow *window, int width, int height);           // callback in case user resize window

void process_input(GLFWwindow *window);                            // take user entered esc key input to close window

unsigned int set_basic_shader_program();       // set vertex and fragment shader and link them in shader program

int create_texture(string image_file, bool is_rgb);           // create and set texture

