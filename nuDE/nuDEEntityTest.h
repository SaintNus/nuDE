/*!
 * \file nuDEEntityTest.h
 * \brief Entity test.
 * \author Nus
 * \date 2012/05/01 23:08
 */

#ifndef __NUDEENTITYTEST_H__
#define __NUDEENTITYTEST_H__

class nuDEEntityTest : public nuEntity
{
  DECLARE_TYPE_INFO;

  void setup(nuGSetupContext& setup);
  void update(void);
  void draw(nuGContext& context);

  f32 mPosX;
  f32 mDir;
  bool mInit;

  nude::VertexBuffer mVertexBuffer;
  nude::ElementBuffer mElementBuffer;
  nude::VertexArray mVertexArray;
  nude::ShaderProgram mShaderProgram;

public:
  nuDEEntityTest();
  ~nuDEEntityTest();

};

#endif
