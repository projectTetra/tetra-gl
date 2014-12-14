#version 130

uniform mat4 projection;

in vec2 vVertex;
in vec2 vTexCoords;

out vec2 vVaryingTexCoords;

void main()
{
  vVaryingTexCoords = vTexCoords;
  gl_Position = projection * vec4( vVertex.x, vVertex.y, 0.0f, 1.0f );
}
