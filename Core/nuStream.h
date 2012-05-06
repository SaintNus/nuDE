/*!
 * \file nuStream.h
 * \brief Stream class.
 * \author Nus
 * \date 2012/02/04 23:18
 */

#ifndef __NUSTREAM_H__
#define __NUSTREAM_H__

#include "nuType.h"
#include "nuOS.h"
#include "nuOSType.h"

class nuStream
{
public:
  enum STREAM_TYPE {
    FILE_STREAM = 0,
    MEMORY_STREAM,
  };

private:
  class Streamer {
    STREAM_TYPE mType;
    Streamer();

  public:
    Streamer(STREAM_TYPE type)
        : mType(type)
    {
      // None...
    }
    virtual ~Streamer() {}

    STREAM_TYPE getType(void) const {
      return mType;
    }

    virtual nude::STREAM_ERROR seekBegin(ui64 seek_offset) const = 0;
    virtual nude::STREAM_ERROR seekEnd(ui64 seek_offset) const = 0;
    virtual nude::STREAM_ERROR seek(i64 seek_offset) const = 0;

    virtual size_t read(void* p_buffer, size_t size) const = 0;
    virtual size_t write(const void* p_buffer, size_t size) = 0;

    virtual size_t getSize(void) const = 0;
    virtual size_t getRemainSize(void) const = 0;
  };

  class FileStreamer : public Streamer {
    nuFile mFile;

    FileStreamer();

  public:
    FileStreamer(ccstr file_path, nude::FILE_ATTRIBUTE attr)
        : Streamer(FILE_STREAM),
          mFile(attr, file_path)
    {
      // None...
    }

    FileStreamer(wccstr file_path, nude::FILE_ATTRIBUTE attr)
        : Streamer(FILE_STREAM),
          mFile(attr, file_path)
    {
      // None...
    }

    ~FileStreamer() {}

    bool isOpened(void) const {
      return mFile.isOpened();
    }

    nude::STREAM_ERROR seekBegin(ui64 seek_offset) const {
      if(mFile.isOpened())
        return nude::SERROR_CANNOT_OPEN;

      FileStreamer& file_stream = *const_cast< FileStreamer* >(this);
      nude::FILE_ERROR err = file_stream.mFile.seek(static_cast< i64 >(seek_offset), nude::FSEEK_SET);
      if(err != nude::FERROR_NONE)
        return nude::SERROR_INVALID_OPERATION;

      return nude::SERROR_NONE;
    }

    nude::STREAM_ERROR seekEnd(ui64 seek_offset) const {
      if(mFile.isOpened())
        return nude::SERROR_CANNOT_OPEN;

      FileStreamer& file_stream = *const_cast< FileStreamer* >(this);
      nude::FILE_ERROR err = file_stream.mFile.seek(static_cast< i64 >(seek_offset), nude::FSEEK_END);
      if(err != nude::FERROR_NONE)
        return nude::SERROR_INVALID_OPERATION;

      return nude::SERROR_NONE;
    }

    nude::STREAM_ERROR seek(i64 seek_offset) const {
      if(mFile.isOpened())
        return nude::SERROR_CANNOT_OPEN;

      FileStreamer& file_stream = *const_cast< FileStreamer* >(this);
      nude::FILE_ERROR err = file_stream.mFile.seek(static_cast< i64 >(seek_offset), nude::FSEEK_SET);
      if(err != nude::FERROR_NONE)
        return nude::SERROR_INVALID_OPERATION;

      return nude::SERROR_NONE;
    }

    size_t read(void* p_buffer, size_t size) const {
      if(mFile.isOpened())
        return nude::SERROR_CANNOT_OPEN;

      FileStreamer& file_stream = *const_cast< FileStreamer* >(this);
      return file_stream.mFile.read(p_buffer, size);
    }

    size_t write(const void* p_buffer, size_t size) {
      if(mFile.isOpened())
        return nude::SERROR_CANNOT_OPEN;
      return mFile.write(p_buffer, size);
    }

    size_t getSize(void) const {
      return mFile.getSize();
    }

    size_t getRemainSize(void) const {
      i64 pos = mFile.tell();
      return mFile.getSize() - pos;
    }

  };

  class MemoryStreamer : public Streamer {
    void* mpMemory;
    size_t mSize;
    ui8* mpCurrent;
    size_t mRemainSize;

    union {
      ui32 mAttribute;
      struct {
        ui32 mInternalMem: 1;
        ui32 mReserved: 31;
      };
    };

    MemoryStreamer();

  public:
    MemoryStreamer(void* p_memory, size_t size)
        : Streamer(MEMORY_STREAM),
          mpMemory(p_memory),
          mSize(size),
          mAttribute(0),
          mpCurrent(nullptr),
          mRemainSize(0)
    {
      if(!mpMemory || size == 0) {
        mpMemory = nullptr;
        mSize = 0;
      } else {
        mpCurrent = static_cast< ui8* >(mpMemory);
        mRemainSize = mSize;
      }
    }

    MemoryStreamer(size_t size)
        : Streamer(MEMORY_STREAM),
          mpMemory(nullptr),
          mSize(0),
          mAttribute(0),
          mpCurrent(nullptr),
          mRemainSize(0)
    {
      size_t sz = size > 0 ? size : 1;
      mpMemory = nude::Alloc(sz);
      if(mpMemory) {
        mSize = size;
        mpCurrent = static_cast< ui8* >(mpMemory);
        mRemainSize = mSize;
        mInternalMem = 1;
      }
    }

    ~MemoryStreamer() {
      if(mInternalMem == 1 && mpMemory) {
        nude::Dealloc(mpMemory);
        mpMemory = 0;
      }
    }

    void* getBuffer(void) const {
      return mpMemory;
    }

    nude::STREAM_ERROR seekBegin(ui64 seek_offset) const {
      if(!mpMemory)
        return nude::SERROR_CRITICAL;

      ui64 seek_sz = seek_offset <= mSize ? seek_offset : mSize;
      MemoryStreamer& mem_stream = *const_cast< MemoryStreamer* >(this);
      mem_stream.mpCurrent = static_cast< ui8* >(mpMemory);
      mem_stream.mpCurrent += seek_sz;
      mem_stream.mRemainSize = mSize - seek_sz;

      return nude::SERROR_NONE;
    }

    nude::STREAM_ERROR seekEnd(ui64 seek_offset) const {
      if(!mpMemory)
        return nude::SERROR_CRITICAL;

      ui64 seek_sz = seek_offset <= mSize ? seek_offset : mSize;
      seek_sz = mSize - seek_sz;
      MemoryStreamer& mem_stream = *const_cast< MemoryStreamer* >(this);
      mem_stream.mpCurrent = static_cast< ui8* >(mpMemory);
      mem_stream.mpCurrent += seek_sz;
      mem_stream.mRemainSize = mSize - seek_sz;

      return nude::SERROR_NONE;
    }

    nude::STREAM_ERROR seek(i64 seek_offset) const {
      if(!mpMemory)
        return nude::SERROR_CRITICAL;

      if(seek_offset > 0) {
        ui64 useek_ofs = static_cast< ui64 >(seek_offset);
        ui64 seek_sz = useek_ofs <= mRemainSize ? useek_ofs : mRemainSize;
        MemoryStreamer& mem_stream = *const_cast< MemoryStreamer* >(this);

        mem_stream.mpCurrent += seek_sz;
        mem_stream.mRemainSize -= seek_sz;

        return nude::SERROR_NONE;
      } else if(seek_offset < 0) {
        size_t remain = mSize - mRemainSize;
        ui64 useek_ofs = static_cast< ui64 >(seek_offset * -1);
        ui64 seek_sz = useek_ofs <= remain ? useek_ofs : remain;
        MemoryStreamer& mem_stream = *const_cast< MemoryStreamer* >(this);

        mem_stream.mpCurrent -= seek_sz;
        mem_stream.mRemainSize += seek_sz;

        return nude::SERROR_NONE;
      }

      return nude::SERROR_INVALID_OPERATION;
    }

    size_t read(void* p_buffer, size_t size) const {
      if(!mpMemory)
        return nude::SERROR_CRITICAL;

      ui64 data_sz = size <= mRemainSize ? size : mRemainSize;
      memcpy(p_buffer, mpCurrent, data_sz);

      MemoryStreamer& mem_stream = *const_cast< MemoryStreamer* >(this);
      mem_stream.mpCurrent += data_sz;
      mem_stream.mRemainSize -= data_sz;

      return data_sz;
    }

    size_t write(const void* p_buffer, size_t size) {
      if(!mpMemory)
        return nude::SERROR_CRITICAL;

      ui64 data_sz = size <= mRemainSize ? size : mRemainSize;
      memcpy(mpCurrent, p_buffer, data_sz);

      mpCurrent += data_sz;
      mRemainSize -= data_sz;

      return data_sz;
    }

    size_t getSize(void) const {
      return mSize;
    }

    size_t getRemainSize(void) const {
      return mRemainSize;
    }

  };

  Streamer* mpStreamer;

public:
  nuStream()
      : mpStreamer(nullptr)
  {
    // None...
  }

  ~nuStream() {
    close();
  }

  void* create(void* p_buffer, size_t size);
  void* create(size_t size);

  nude::STREAM_ERROR create(ccstr file_path, nude::FILE_ATTRIBUTE attr);
  nude::STREAM_ERROR create(wccstr file_path, nude::FILE_ATTRIBUTE attr);

  nude::STREAM_ERROR rewind(void) const {
    return mpStreamer->seekBegin(0);
  }

  nude::STREAM_ERROR seekBegin(ui64 seek_offset) const {
    if(!mpStreamer)
      return nude::SERROR_CRITICAL;
    return mpStreamer->seekBegin(seek_offset);
  }

  nude::STREAM_ERROR seekEnd(ui64 seek_offset) const {
    if(!mpStreamer)
      return nude::SERROR_CRITICAL;
    return mpStreamer->seekEnd(seek_offset);
  }

  nude::STREAM_ERROR seek(i64 seek_offset) const {
    if(!mpStreamer)
      return nude::SERROR_CRITICAL;
    return mpStreamer->seek(seek_offset);
  }

  size_t read(void* p_buffer, size_t size) const {
    if(!mpStreamer)
      return nude::SERROR_CRITICAL;
    return mpStreamer->read(p_buffer, size);
  }

  template< class T >
  size_t read(T* p_buffer, ui32 num = 1) const {
    if(!mpStreamer)
      return nude::SERROR_CRITICAL;
    return mpStreamer->read(p_buffer, sizeof(T) * num);
  }

  size_t write(const void* p_buffer, size_t size) {
    if(!mpStreamer)
      return nude::SERROR_CRITICAL;
    return mpStreamer->write(p_buffer, size);
  }

  template< class T >
  size_t write(const T* p_buffer, ui32 num = 1) {
    if(!mpStreamer)
      return nude::SERROR_CRITICAL;
    return mpStreamer->write(p_buffer, sizeof(T) * num);
  }

  size_t getSize(void) const {
    if(!mpStreamer)
      return 0;
    return mpStreamer->getSize();
  }

  size_t getRemainSize(void) const {
    if(!mpStreamer)
      return 0;
    return mpStreamer->getRemainSize();
  }

  void close(void) {
    if(mpStreamer) {
      delete mpStreamer;
      mpStreamer = nullptr;
    }
  }

};

#endif
