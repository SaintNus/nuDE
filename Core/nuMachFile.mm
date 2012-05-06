/*!
 * \file nuMachFile.mm
 * \brief File handler for MacOSX.
 * \author Nus
 * \date 2011/09/19 18:59
 */

#include "nuMachFile.h"

// Open a file.
nude::FILE_ERROR nuMachFile::open(nude::FILE_ATTRIBUTE attr, ccstr name)
{
  nude::FILE_ERROR ret = nude::FERROR_NONE;

  i32 protocol_id = -1;
  NSString* path = nil;

  ccstr protocol[] = {
    "res://",
    "home://",
    "file://",
  };
  
  const i32 sz = static_cast< i32 >(sizeof(protocol) / sizeof(ccstr));

  for(i32 ii = 0; ii < sz; ii++) {
    size_t len = strlen(protocol[ii]);
    if(strncmp(protocol[ii], name, len) == 0) {
      protocol_id = ii;
      break;
    }
  }

  switch(protocol_id) {
  case 0:
    {
      ccstr f_name = name + strlen(protocol[protocol_id]);
      NSBundle* bundle = [NSBundle mainBundle];
      NSString* name_str = [[NSString alloc] initWithCString: f_name encoding: NSUTF8StringEncoding];
      path = [[NSString alloc] initWithFormat: @"%@/%@", [bundle resourcePath], name_str];
      [name_str release];
    }
    break;
  case 1:
    {
      ccstr f_name = name + strlen(protocol[protocol_id]);
      NSString* name_str = [[NSString alloc] initWithCString: f_name encoding: NSUTF8StringEncoding];
      NSString* t_path = [[NSString alloc] initWithFormat: @"~/%@", name_str];
      path = [[t_path stringByExpandingTildeInPath] retain];
      [name_str release];
      [t_path release];
    }
    break;
  case 2:
    {
      ccstr f_name = name + strlen(protocol[protocol_id]);
      path = [[NSString alloc] initWithCString: f_name encoding: NSUTF8StringEncoding];
    }
    break;
  default:
    path = [[NSString alloc] initWithCString: name encoding: NSUTF8StringEncoding];
  }

  close();

  mAttribute = attr;

  switch(mAttribute) {
  case nude::FATTR_READ:
    {
      NSFileHandle* fh = [NSFileHandle fileHandleForReadingAtPath: path];
      if(!fh) {
        ret = nude::FERROR_CANNOT_OPEN;
      } else {
        [fh retain];
        mFileSize = [fh seekToEndOfFile];
        [fh seekToFileOffset: 0];
        mFileHandle = fh;
      }
    }
    break;
  case nude::FATTR_WRITE:
    {
      NSFileHandle* fh = [NSFileHandle fileHandleForWritingAtPath: path];
      if(!fh) {
        ret = nude::FERROR_CANNOT_OPEN;
      } else {
        [fh retain];
        mFileHandle = fh;
      }
    }
    break;
  case nude::FATTR_UPDATE:
    {
      NSFileHandle* fh = [NSFileHandle fileHandleForUpdatingAtPath: path];
      if(!fh) {
        ret = nude::FERROR_CANNOT_OPEN;
      } else {
        [fh retain];
        mFileSize = [fh seekToEndOfFile];
        [fh seekToFileOffset: 0];
        mFileHandle = fh;
      }
    }
    break;
  default:
    ret = nude::FERROR_CRITICAL;
  }

  [path release];

  return ret;
}

// Open a file.
nude::FILE_ERROR nuMachFile::open(nude::FILE_ATTRIBUTE attr, wccstr name)
{
  c8 buffer[PATH_MAX];

  wcstombs(buffer, name, PATH_MAX);
  buffer[PATH_MAX - 1] = 0x00;
  
  return open(attr, buffer);
}

// Close a file.
nude::FILE_ERROR nuMachFile::close(void)
{
  if(mFileHandle) {
    NSFileHandle* fh = static_cast< NSFileHandle* >(mFileHandle);

    [fh release];
    mFileHandle = nullptr;
    mFileSize = 0;

    return nude::FERROR_NONE;
  }

  return nude::FERROR_INVALID_OPERATION;
}

// Read file content.
size_t nuMachFile::read(void* ptr, size_t bytes)
{
  if(!mFileHandle || ptr == nullptr)
    return 0;

  if(mAttribute != nude::FATTR_WRITE) {
    NSFileHandle* fh = static_cast< NSFileHandle* >(mFileHandle);
    NSData* data = [fh readDataOfLength: bytes];
    size_t length = [data length];
    if(length > 0) {
      memcpy(ptr, [data bytes], length);
      return length;
    }
  }

  return 0;
}

// Write file content.
size_t nuMachFile::write(const void* ptr, size_t bytes)
{
  if(!mFileHandle || ptr == nullptr || bytes == 0)
    return 0;

  if(mAttribute == nude::FATTR_WRITE) {
    NSFileHandle* fh = static_cast< NSFileHandle* >(mFileHandle);
    NSData* data = [NSData dataWithBytesNoCopy: const_cast< void* >(ptr) length: bytes freeWhenDone: NO];
    [fh writeData: data];
    return bytes;
  }

  return 0;
}

// Seek file.
nude::FILE_ERROR nuMachFile::seek(i64 pos, nude::FILE_SEEK whence)
{
  if(!mFileHandle)
    return nude::FERROR_INVALID_OPERATION;

  switch(whence) {
  case nude::FSEEK_CURRENT:
    {
      NSFileHandle* fh = static_cast< NSFileHandle* >(mFileHandle);
      i64 cpos = [fh offsetInFile] + pos;
      [fh seekToFileOffset: cpos];
    }
    break;
  case nude::FSEEK_END:
    {
      NSFileHandle* fh = static_cast< NSFileHandle* >(mFileHandle);
      i64 cpos = [fh seekToEndOfFile] + pos;
      [fh seekToFileOffset: cpos];
    }
    break;
  case nude::FSEEK_SET:
    {
      NSFileHandle* fh = static_cast< NSFileHandle* >(mFileHandle);
      [fh seekToFileOffset: pos];
    }
    break;
  default:
    return nude::FERROR_CRITICAL;
  }

  return nude::FERROR_NONE;
}

// Tell file position.
i64 nuMachFile::tell(void) const
{
  if(!mFileHandle)
    return 0;
  NSFileHandle* fh = static_cast< NSFileHandle* >(mFileHandle);
  return [fh offsetInFile];
}
