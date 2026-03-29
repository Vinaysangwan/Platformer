#version 430 core

// Inputs
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec2 inTexCoords;

// Outputs
layout (location = 0) out vec2 outTexCoords;

// Uniforms

void main()
{
  gl_Position = vec4(inPos, 1.0);

  outTexCoords = inTexCoords;
}
