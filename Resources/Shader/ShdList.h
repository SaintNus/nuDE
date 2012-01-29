/*
 * Shader list information header.
 * This file is automatically generated, do not edit.
 */

#ifndef __Generated_ShdList_h__
#define __Generated_ShdList_h__

#ifdef __cplusplus
namespace glslp
{
#endif

  // Program list.
  enum ProgramList {
    Program_Debug,
    Program_Test,
    ProgramList_Num,
  };

  /*
   * Attributes.
   */

  enum DebugAttribute {
    Debug_inPosition,
    Debug_inColor,
    DebugAttribute_Num,
  };

  enum TestAttribute {
    Test_inPosition,
    TestAttribute_Num,
  };

  /*
   * Uniforms.
   */

  enum TestUniform {
    Test_uColor_value_0_,
    Test_Test_adder,
    TestUniform_Num,
  };

  /*
   * Uniform blocks.
   */

  enum TestUniformBlock {
    Test_Test,
    TestUniformBlock_Num,
  };

  /*
   * Structures.
   */

  struct Variable {
    const char* type;
    GLenum type;
    GLint size;
  };

  struct Program {
    const char* vsh_file; // Vertex shader file name.
    const char* fsh_file; // Fragment shader file name.
    const Variable* attributes; // Program attributes.
    const Variable* uniforms; // Program uniforms.
    const char** uniform_blocks; // Uniform block names.
  };

  /*
   * Constants.
   */

  const Variable DebugAttributes[DebugAttribute_Num] = {
    { "inPosition", static_cast< GLenum >(35665), 1 },
    { "inColor", static_cast< GLenum >(35666), 1 },
  };

  const Variable TestAttributes[TestAttribute_Num] = {
    { "inPosition", static_cast< GLenum >(35665), 1 },
  };

  const Variable TestUniforms[TestUniform_Num] = {
    { "uColor.value[0]", static_cast< GLenum >(5126), 4 },
    { "Test.adder", static_cast< GLenum >(35666), 1 },
  };

  const char* TestUniformBlocks[TestUniformBlock_Num] = {
    "Test",
  };

  const Program GLSLPrograms[ProgramList_Num] = {
    {
      "Debug.vsh",
      "Debug.fsh",
      DebugAttributes,
      NULL,
      NULL,
    },
    {
      "test/Test.vsh",
      "test/Test.fsh",
      TestAttributes,
      TestUniforms,
      TestUniformBlocks,
    },
  };

}

#ifdef __cplusplus
}
#endif

#endif