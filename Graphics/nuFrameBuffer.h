/*!
 * \file nuFrameBuffer.h
 * \brief Render buffer.
 * \author Nus
 * \date 2012/02/09 00:54
 */

#ifndef __NUFRAMEBUFFER_H__
#define __NUFRAMEBUFFER_H__

#include "nuGResource.h"
#include "nuTexture.h"
#include "nuRenderBuffer.h"

class nuFrameBuffer : public nuGResource
{
  DECLARE_TYPE_INFO;
  friend nude::Handle< nuFrameBuffer >;

public:
  enum TARGET {
    DRAW_BUFFER = 0,
    READ_BUFFER,
    FRAME_BUFFER
  };

  enum ATTACHMENT {
    COLOR_0 = 0,
    COLOR_1,
    COLOR_2,
    COLOR_3,
    COLOR_4,
    COLOR_5,
    COLOR_6,
    COLOR_7,
    COLOR_8,
    MAX_COLOR_ATTACHMENT,
    DEPTH = MAX_COLOR_ATTACHMENT,
    STENCIL,
    MAX_ATTACHMENT,
  };

private:
  struct Attachment {
    nude::GraphicResource attachment;
    TARGET target;
    ATTACHMENT type;
    Attachment() {}
    ~Attachment() {}
  };

  struct AttachmentList {
    Attachment* attachments;
    ui32 attachment_num;
    ui32 size;

    AttachmentList(ui32 sz)
        : attachment_num(0),
          size(sz)
    {
      attachments = new Attachment[size];
    }

    ~AttachmentList() {
      if(attachments) {
        delete[] attachments;
        attachments = nullptr;
      }
    }
  };

  GLuint mHandle;
  AttachmentList* mpCurrentAttachmentList;
  AttachmentList* mpAttachmentList;

  GLenum getFrameBufferAttachment(ATTACHMENT attachment) const {
    const GLenum gl_attachment[] = {
      GL_COLOR_ATTACHMENT0,
      GL_COLOR_ATTACHMENT1,
      GL_COLOR_ATTACHMENT2,
      GL_COLOR_ATTACHMENT3,
      GL_COLOR_ATTACHMENT4,
      GL_COLOR_ATTACHMENT5,
      GL_COLOR_ATTACHMENT6,
      GL_COLOR_ATTACHMENT7,
      GL_COLOR_ATTACHMENT8,
      GL_DEPTH_ATTACHMENT,
      GL_STENCIL_ATTACHMENT,
    };
    return gl_attachment[attachment];
  };

  GLenum getFrameBufferTarget(TARGET target) const {
    const GLenum gl_target[] = {
      GL_READ_FRAMEBUFFER,
      GL_DRAW_FRAMEBUFFER,
      GL_FRAMEBUFFER,
    };
    return gl_target[target];
  }

  void update(void);

  nuFrameBuffer();
  ~nuFrameBuffer();

public:
  GLuint getHandle(void) const {
    return mHandle;
  }

  void beginInitialize(ui32 num);
  void attach(TARGET target, ATTACHMENT attachment, nude::Texture& texture);
  void attach(TARGET target, ATTACHMENT attachment, nude::RenderBuffer& render_buffer);
  void endInitialize(void);

};

namespace nude {
  typedef nude::Handle< nuFrameBuffer > FrameBuffer;
}

#endif
