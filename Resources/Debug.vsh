/*!
 * \file Debug.vsh
 * \brief Debug shader.
 * \author Nus
 * \date 2011/11/27 08:00
 */

in vec3 inPosition;
in vec4 inColor;

out vec4 varColor;

void main(void)
{
  gl_Position = vec4(inPosition, 1.0);
  varColor = inColor;
}
