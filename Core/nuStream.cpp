/*!
 * \file nuStream.cpp
 * \brief Stream class.
 * \author Nus
 * \date 2012/02/04 23:18
 */

#include "CoreInclude.h"
#include "nuStream.h"

void* nuStream::create(void* p_buffer, size_t size)
{
  close();
  MemoryStreamer* p_mem = new MemoryStreamer(p_buffer, size);
  if(p_mem) {
    mpStreamer = p_mem;
    return p_mem->getBuffer();
  }
  return nullptr;
}

void* nuStream::create(size_t size)
{
  close();
  MemoryStreamer* p_mem = new MemoryStreamer(size);
  if(p_mem) {
    mpStreamer = p_mem;
    return p_mem->getBuffer();
  }
  return nullptr;
}

nude::STREAM_ERROR nuStream::create(ccstr file_path, nude::FILE_ATTRIBUTE attr)
{
  close();
  FileStreamer* p_file = new FileStreamer(file_path, attr);
  if(p_file) {
    mpStreamer = p_file;
    return p_file->isOpened() ? nude::SERROR_NONE : nude::SERROR_CANNOT_OPEN;
  }
  return nude::SERROR_CRITICAL;
}

nude::STREAM_ERROR nuStream::create(wccstr file_path, nude::FILE_ATTRIBUTE attr)
{
  close();
  FileStreamer* p_file = new FileStreamer(file_path, attr);
  if(p_file) {
    mpStreamer = p_file;
    return p_file->isOpened() ? nude::SERROR_NONE : nude::SERROR_CANNOT_OPEN;
  }
  return nude::SERROR_CRITICAL;
}
