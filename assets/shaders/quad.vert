#version 430 core

// Inputs
layout (location = 0) in vec2 inPos;
layout (location = 1) in vec2 inSize;
layout (location = 2) in vec2 inAtlasOffset;
layout (location = 3) in vec2 inSpriteSize;

// Outputs
layout (location = 0) out vec2 outTexCoords;

// Uniforms
uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;

void main()
{
  // Vertices
  vec2 vertex[6] = {
    inPos,                                          // Left Top
    vec2(inPos.x, inPos.y + inSize.y),              // Left Bottom
    vec2(inPos.x + inSize.x, inPos.y),              // Right Top

    vec2(inPos.x + inSize.x, inPos.y),              // Right Top
    vec2(inPos.x, inPos.y + inSize.y),              // Left Bottom
    vec2(inPos.x + inSize.x, inPos.y + inSize.y)    // Right Bottom
  };

  // TextureCoords
  vec2 textureCoords[6] = {
    vec2(inAtlasOffset),                                                                  // Left Top
    vec2(inAtlasOffset.x, inAtlasOffset.y + inSpriteSize.y),                              // Left Bottom
    vec2(inAtlasOffset.x + inSpriteSize.x, inAtlasOffset.y),                              // Right Top

    vec2(inAtlasOffset.x + inSpriteSize.x, inAtlasOffset.y),                              // Right Top
    vec2(inAtlasOffset.x, inAtlasOffset.y + inSpriteSize.y),                              // Left Bottom
    vec2(inAtlasOffset.x + inSpriteSize.x, inAtlasOffset.y + inSpriteSize.y)              // Right Bottom
  };
  
  gl_Position = uProjectionMatrix * uViewMatrix * vec4(vertex[gl_VertexID], 1.0, 1.0);

  outTexCoords = textureCoords[gl_VertexID];
}
