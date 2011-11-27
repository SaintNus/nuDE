/*!
 * \file Debug.vsh
 * \brief Debug shader.
 * \author Nus
 * \date 2011/11/27 08:00
 */

attribute vec4 inPosition;
attribute vec4 inColor;

varying vec4 varColor;

void main(void)
{
  gl_Position = inPosition;
  varColor = inColor;
}
