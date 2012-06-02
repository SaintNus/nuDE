/*!
 * \file DrawString.vsh
 * \brief Draw string.
 * \author Nus
 * \date 2011/11/27 08:00
 */

in int inVertexIndex;

out vec2 varUV;

uniform vec2 uniPosOffset;
uniform vec2 uniPosition[4];
uniform vec2 uniUV[4];
uniform vec4 uniViewport;

void main(void)
{
  vec2 vpos = uniPosOffset + uniPosition[inVertexIndex];
  vec4 pos = vec4((vpos * uniViewport.zw * vec2(2.0)) - vec2(1.0),
                  0.0,
                  1.0);
  gl_Position = pos;
  varUV = uniUV[inVertexIndex];
}
