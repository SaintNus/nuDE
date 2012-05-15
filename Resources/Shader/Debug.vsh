/*!
 * \file Debug.vsh
 * \brief Debug shader.
 * \author Nus
 * \date 2011/11/27 08:00
 */

in vec3 inPosition;
in vec4 inColor;

out vec4 varTest;
out vec4 varColor;

uniform vec4 uniTest;
uniform vec4 uniColor[3];

void main(void)
{
  gl_Position = vec4(inPosition, 1.0);
  varTest = uniTest;
  varColor = inColor * vec4(uniColor[0].x, uniColor[1].y, uniColor[2].z, 1.0);
}
