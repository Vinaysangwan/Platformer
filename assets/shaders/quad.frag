#version 430 core

// Inputs
layout (location = 0) in vec2 inTexCoords;

// Outputs
layout (location = 0) out vec4 outColor;

// uniforms
uniform sampler2D uTextureSampler;

void main()
{
  vec4 textureColor = texelFetch(uTextureSampler, ivec2(inTexCoords), 0);
  if (textureColor.a < 0.01)
    discard;
  
  outColor = textureColor;
}
