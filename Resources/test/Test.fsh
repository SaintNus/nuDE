/*!
 * \file Test.fsh
 * \brief Test shader.
 * \author Nus
 * \date 2011/11/27 08:00
 */
#version 150

in vec4 varColor;

uniform Test
{
  vec4 adder;
} ubTest;

out vec4 fragColor;

void main(void)
{
  fragColor = varColor + ubTest.adder;
}
