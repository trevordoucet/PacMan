#version 150

in vec4 vPos;
in  vec2 vTexCoord;
out vec2 texCoord;

uniform mat4 M;


void
main()
{
  gl_Position = M*vPos;
  texCoord  = vTexCoord;
}
