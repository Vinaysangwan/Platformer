#version 430 core

// Inputs
layout (location = 0) in vec2 inTexCoords;

// Outputs
layout (location = 0) out vec4 outColor;

// Uniforms
uniform sampler2D uTextureSampler;
uniform ivec2 uSpriteSize;
uniform ivec2 uAtlasOffset;

void main()
{
  ivec2 pixelCoords = uAtlasOffset + ivec2(inTexCoords * uSpriteSize);
  vec4 textureColor = texelFetch(uTextureSampler, pixelCoords, 0);
  if (textureColor.a < 0.01)
    discard;
  
  outColor = textureColor;
}
