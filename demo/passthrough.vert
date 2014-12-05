#version 130

in vec2 vVertex;

void main()
{
  gl_Position = vec4( vVertex, 0.0f, 1.0f );
}
