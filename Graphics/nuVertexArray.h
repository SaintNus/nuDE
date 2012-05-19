/*!
 * \file nuVertexArray.h
 * \brief Vertex array.
 * \author Nus
 * \date 2012/05/05 17:53
 */

#ifndef __NUVERTEXARRAY_H__
#define __NUVERTEXARRAY_H__

#include "nuGResource.h"

class nuVertexArray : public nuGResource
{
  DECLARE_TYPE_INFO;
  friend class nuGResManager;
  friend class nuRenderGL;
  friend nude::Handle< class nuVertexArray >;

public:
  enum ATTRIBUTE_TYPE {
    INT_8 = 0,
    UNSIGNED_INT_8,
    INT_16,
    UNSIGNED_INT_16,
    INT_32,
    UNSIGNED_INT_32,
    FLOAT,
    HALF,
    DOUBLE,
  };

  struct Array {
    i32 size;
    ATTRIBUTE_TYPE type;
    bool normalized;
    size_t offset;
    size_t stride;
    Array() {}
    Array(i32 i_size,
          ATTRIBUTE_TYPE i_type,
          bool i_normalized,
          size_t i_stride,
          size_t i_offset)
        : size(i_size),
          type(i_type),
          normalized(i_normalized),
          stride(i_stride),
          offset(i_offset)
    {
      // None...
    }
    ~Array() {}
  };

private:                    
  static const ui32 MAX_VERTEX_ATTRIBUTE = 16;

  GLuint mHandle;
  GLuint mVertexBuffer;
  Array* mpArray;
  ui32 mArrayNum;

  nuVertexArray();
  ~nuVertexArray();

  void update(void);

  GLenum getAttributeType(ATTRIBUTE_TYPE type) const {
    const GLenum attr_table[] = {
      GL_BYTE,
      GL_UNSIGNED_BYTE,
      GL_SHORT,
      GL_UNSIGNED_SHORT,
      GL_INT,
      GL_UNSIGNED_INT,
      GL_FLOAT,
      GL_HALF_FLOAT,
      GL_DOUBLE,
    };

    return attr_table[type];
  }

  GLuint getVertexHandle(void) const {
    return mVertexBuffer;
  }

  void reset(GLuint vertex_id);

public:
  GLuint getHandle() const {
    return mHandle;
  }

  void begin(ui32 array_num);
  void declare(ui32 index, const Array& array);
  void end(void);

};

namespace nude {
  typedef Handle< nuVertexArray > VertexArray;
}

#endif
