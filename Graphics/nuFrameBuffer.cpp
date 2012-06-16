/*!
 * \file nuFrameBuffer.cpp
 * \brief Frame buffer.
 * \author Nus
 * \date 2012/02/09 00:54
 */

#include "GraphicsInclude.h"
#include "nuFrameBuffer.h"

IMPLEMENT_TYPE_INFO_INST(nuFrameBuffer, nuGResource, nullptr);

nuFrameBuffer::nuFrameBuffer()
    : nuGResource(nuGResource::FRAME_BUFFER, nuGResource::STATIC_RESOURCE),
      mHandle(0),
      mpCurrentAttachmentList(nullptr),
      mpAttachmentList(nullptr)
{
  // None...
}

nuFrameBuffer::~nuFrameBuffer()
{
  if(mHandle) {
    CHECK_GL_ERROR(glDeleteFramebuffers(1, &mHandle));
    mHandle = 0;
  }

  if(mpAttachmentList) {
    delete[] mpAttachmentList;
    mpAttachmentList = nullptr;
  }

  if(mpCurrentAttachmentList) {
    delete[] mpCurrentAttachmentList;
    mpCurrentAttachmentList = nullptr;
  }
}

void nuFrameBuffer::update(void)
{
  if(!isInitialized()) {
    CHECK_GL_ERROR(glGenFramebuffers(1, &mHandle));
    NU_ASSERT(mHandle != 0, "Error creating frame buffer.\n");
  }

  if(mpAttachmentList) {
    if(mpCurrentAttachmentList)
      delete mpCurrentAttachmentList;

    mpCurrentAttachmentList = mpAttachmentList;
    mpAttachmentList = nullptr;

    CHECK_GL_ERROR(glBindFramebuffer(GL_FRAMEBUFFER, mHandle));
    for(ui32 ui = 0; ui < mpCurrentAttachmentList->attachment_num; ui++) {
      Attachment att = mpCurrentAttachmentList->attachments[ui];
      switch(att.attachment->getType()) {
      case nuGResource::TEXTURE:
        {
          GLenum target = getFrameBufferTarget(att.target);
          GLenum attachment = getFrameBufferAttachment(att.type);
          GLuint handle = att.attachment.cast< nuTexture >()->getHandle();
          CHECK_GL_ERROR(glFramebufferTexture(target, attachment, handle, 0));
        }
        break;
      case nuGResource::RENDER_BUFFER:
        {
          GLenum target = getFrameBufferTarget(att.target);
          GLenum attachment = getFrameBufferAttachment(att.type);
          GLuint handle = att.attachment.cast< nuRenderBuffer >()->getHandle();
          CHECK_GL_ERROR(glFramebufferRenderbuffer(target, attachment, GL_RENDERBUFFER, handle));
        }
        break;
      default:
        NU_ASSERT_C(false);
      }
    }
  }
}

void nuFrameBuffer::beginInitialize(ui32 num)
{
  if(mpAttachmentList) {
    if(mpAttachmentList->size < num) {
      delete mpAttachmentList;
      mpAttachmentList = new AttachmentList(num);
      NU_ASSERT_C(mpAttachmentList != nullptr);
    } else {
      for(ui32 ui = 0; ui < mpAttachmentList->size; ui++) {
        mpAttachmentList->attachments[ui].attachment.release();
      }
      mpAttachmentList->attachment_num = 0;
    }
  } else {
    mpAttachmentList = new AttachmentList(num);
    NU_ASSERT_C(mpAttachmentList != nullptr);
  }
}

void nuFrameBuffer::endInitialize(void)
{
  if(mpAttachmentList->attachment_num > 0)
    setUpdate(true);
}

void nuFrameBuffer::attach(TARGET target, ATTACHMENT attachment, nude::Texture& texture)
{
  if(mpAttachmentList && mpAttachmentList->attachment_num < mpAttachmentList->size) {
    Attachment& att = mpAttachmentList->attachments[mpAttachmentList->attachment_num];
    mpAttachmentList->attachment_num++;
    att.attachment = &texture;
    att.target = target;
    att.type = attachment;
  }
}

void nuFrameBuffer::attach(TARGET target, ATTACHMENT attachment, nude::RenderBuffer& render_buffer)
{
  if(mpAttachmentList && mpAttachmentList->attachment_num < mpAttachmentList->size) {
    Attachment& att = mpAttachmentList->attachments[mpAttachmentList->attachment_num];
    mpAttachmentList->attachment_num++;
    att.attachment = &render_buffer;
    att.target = target;
    att.type = attachment;
  }
}
