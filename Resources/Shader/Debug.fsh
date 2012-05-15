/*!
 * \file Debug.fsh
 * \brief Debug shader.
 * \author Nus
 * \date 2011/11/27 08:00
 */

in vec4 varTest;
in vec4 varColor;

out vec4 fragColor;

void main(void)
{
  fragColor = varColor * varTest;
}
