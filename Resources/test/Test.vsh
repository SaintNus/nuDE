/*!
 * \file Test.vsh
 * \brief Test shader.
 * \author Nus
 * \date 2011/11/27 08:00
 */

in vec3 inPosition;

struct Color
{
  vec4 value;
};
uniform Color uColor;

out vec4 varColor;

void main(void)
{
  gl_Position = vec4(inPosition, 1.0);
  varColor = uColor.value;
}
