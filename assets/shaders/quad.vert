#version 430 core

// Inputs
layout (location = 0) in vec3 inPos;

// Outputs

// Uniforms

void main()
{
  gl_Position = vec4(inPos, 1.0);
}
