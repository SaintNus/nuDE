/*!
 * \file Test.vsh
 * \brief Test shader.
 * \author Nus
 * \date 2011/11/27 08:00
 */
#version 150

in vec3 inPosition;

uniform vec5 uColor;

out vec4 varColor;

void main(void)
{
  gl_Position = vec4(inPosition, 1.0);
  varColor = uColor;
}
