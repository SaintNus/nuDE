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

  static GLuint compileShader(GLenum shader_type, void* p_buffer, size_t size);
  static GLuint linkProgram(GLuint vs_id, GLuint fs_id);

  static void deleteShader(GLuint shd_id);
  static void deleteProgram(GLuint prog_id);

};

#endif
