#version 150

in vec4 vPos;

uniform mat4 M;


void
main()
{
  gl_Position = M*vPos;
}
