#version 130

uniform mat4 projection;

in vec3 vVertex;

void main()
{
  gl_Position = projection * vec4( vVertex, 1.0f );
}
