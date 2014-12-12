#version 130

uniform sampler2D tex;
uniform vec4 vTextColor;

in vec2 vVaryingTexCoords;

out vec4 vFragColor;

void main()
{
  vFragColor =
    vTextColor *
    vec4( 1.0f, 1.0f, 1.0f, texture( tex, vVaryingTexCoords ).r );
}
