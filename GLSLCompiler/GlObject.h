/*!
 * \file GlObject.h
 * \brief GL object.
 * \author Nus
 * \date 2012/01/01 20:55
 */

#ifndef __GLOBJECT_H__
#define __GLOBJECT_H__

class GlObject
{
private:
  CGLContextObj mContext;

public:
  GlObject();
  ~GlObject();

  GLint compileShader(GLenum shader_type, void* p_buffer, size_t size);
  GLint linkProgram(GLint vs_id, GLint fs_id);

  void deleteShader(GLint shd_id);
  void deleteProgram(GLint prog_id);

};

#endif
