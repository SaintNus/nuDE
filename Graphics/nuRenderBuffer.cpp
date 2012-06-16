/*!
 * \file nuRenderBuffer.cpp
 * \brief Render buffer.
 * \author Nus
 * \date 2012/06/03 22:29
 */

#include "GraphicsInclude.h"
#include "nuRenderBuffer.h"

IMPLEMENT_TYPE_INFO_INST(nuRenderBuffer, nuGResource, nullptr);

nuRenderBuffer::nuRenderBuffer()
    : nuGResource(nuGResource::RENDER_BUFFER, nuGResource::STATIC_RESOURCE),
      mHandle(0),
      mInternalFormat(nude::RENDERBUFFER_RGBA8),
      mWidth(0),
      mHeight(0)
{
  // None...
}

nuRenderBuffer::~nuRenderBuffer()
{
  if(mHandle) {
    CHECK_GL_ERROR(glDeleteRenderbuffers(1, &mHandle));
    mHandle = 0;
  }
}

void nuRenderBuffer::update(void)
{
  if(!isInitialized()) {
    if(mHandle == 0) {
      CHECK_GL_ERROR(glGenRenderbuffers(1, &mHandle));
      NU_ASSERT(mHandle != 0, "Cannot generate render buffer.\n");
    }

    CHECK_GL_ERROR(glBindRenderbuffer(GL_RENDERBUFFER, mHandle));
    glRenderbufferStorage(GL_RENDERBUFFER, mInternalFormat, mWidth, mHeight);

    setInitialized(true);
  }
}
