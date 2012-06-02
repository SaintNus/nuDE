/*!
 * \file DrawString.vsh
 * \brief Draw string.
 * \author Nus
 * \date 2011/11/27 08:00
 */

in vec2 varUV;

out vec4 fragColor;

uniform sampler2DRect uniStringTex;
uniform vec4 uniColor;

void main(void)
{
  vec4 color = texture(uniStringTex, varUV);
  fragColor = vec4(uniColor.xyz, color.a);
}
