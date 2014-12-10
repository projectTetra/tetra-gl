#version 130

uniform sampler2D tex;

in vec2 vVaryingTexCoords;

out vec4 vFragColor;

void main()
{
  vFragColor = texture( tex, vVaryingTexCoords );
}
