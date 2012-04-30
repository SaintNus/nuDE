/*!
 * \file nuVertexBuffer.h
 * \brief Vertex buffer.
 * \author Nus
 * \date 2012/03/23 12:28
 */

#ifndef __NUVERTEXBUFFER_H__
#define __NUVERTEXBUFFER_H__

#include "nuGResource.h"

class nuVertexBuffer : public nuGResource
{
  DECLARE_TYPE_INFO;
  friend class nuGResManager;
  friend nude::Handle< class nuVertexBuffer >;

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

  struct VertexArray {
    ui32 index;
    i32 size;
    ATTRIBUTE_TYPE type;
    bool normalized;
    size_t offset;
    VertexArray() {}
    VertexArray(ui32 i_index,
                i32 i_size,
                ATTRIBUTE_TYPE i_type,
                bool i_normalized,
                size_t i_offset)
        : index(i_index),
          size(i_size),
          type(i_type),
          normalized(i_normalized),
          offset(i_offset)
    {
      // None...
    }
                    
  };

private:
  enum EXTENSION_FLAG {
    UPDATE_VERTEX_ARRAY = 1 << 0,
  };

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

  void* mpBuffer;
  size_t mSize;
  GLuint mVertexBufferID;
  GLuint mVertexArrayID;
  size_t mUpdateSize;

  static const ui32 MAX_VERTEX_ATTRIBUTE = 16;
  VertexArray mVertexArray[MAX_VERTEX_ATTRIBUTE];
  ui32 mVertexArrayNum;
  GLsizei mStride;

  void update(void);

  void releaseBuffer(void) {
    if(mpBuffer) {
      nude::Dealloc(mpBuffer);
      mpBuffer = nullptr;
    }
  }

  void setUpdateVertexArray(bool update) {
    ui32 ext = getExtension();
    if(update) {
      ext |= UPDATE_VERTEX_ARRAY;
      setExtension(ext);
    } else {
      ext &= ~UPDATE_VERTEX_ARRAY;
      setExtension(ext);
    }
  }

  bool isUpdateVertexArray(void) const {
    return (getExtension() & UPDATE_VERTEX_ARRAY) ? true : false;
  }

  nuVertexBuffer();

  nuVertexBuffer(size_t size, nuGResource::RESOURCE_USAGE usage);
  ~nuVertexBuffer();

public:
  void* getBuffer(void) const {
    return mpBuffer;
  }

  size_t getSize(void) const {
    return mSize;
  }

  void commit(size_t size) {
    mUpdateSize = size;
    setUpdate(true);
  }

  void bind(void) const {
    if(mVertexArrayID != 0)
      glBindVertexArray(mVertexArrayID);
    else
      glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
  }

  void beginArrayDeclaration(size_t stride) {
    setUpdateVertexArray(false);
    mStride = static_cast< GLsizei >(stride);
    mVertexArrayNum = 0;
  }

  void declare(const VertexArray& attr) {
    mVertexArray[mVertexArrayNum] = attr;
    mVertexArrayNum++;
  }

  void endArrayDeclaration(void) {
    setUpdateVertexArray(true);
  }

};

namespace nude {
  typedef Handle< nuVertexBuffer > VertexBuffer;
}

#endif
