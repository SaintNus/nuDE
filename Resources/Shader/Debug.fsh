/*!
 * \file Debug.fsh
 * \brief Debug shader.
 * \author Nus
 * \date 2011/11/27 08:00
 */

in vec4 varTest;
in vec4 varColor;
in vec2 varUV;

out vec4 fragColor;

uniform sampler2D uniTexture;

void main(void)
{
  vec4 tex_color = texture(uniTexture, varUV);
  vec4 color = varColor * varTest;
  color = clamp(color, 0.0, 1.0);
  fragColor = color * tex_color;
}
