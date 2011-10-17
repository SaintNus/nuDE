/*!
 * \file nuMachFile.mm
 * \brief File handler for MacOSX.
 * \author Nus
 * \date 2011/09/19 18:59
 */

#include "nuMachFile.h"

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

  for(i32 ii = 0; ii < sizeof(protocol) / sizeof(ccstr); ii++) {
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
      path = [[NSString alloc] initWithFormat: @"%@/@s", [bundle resourcePath], f_name];
    }
    break;
  case 1:
    {
      ccstr f_name = name + strlen(protocol[protocol_id]);
      path = [[NSString alloc] initWithFormat: @"~/@s", f_name];
    }
    break;
  case 2:
    {
      ccstr f_name = name + strlen(protocol[protocol_id]);
      path = [[NSString alloc] initWithCString: f_name encoding: NSASCIIStringEncoding];
    }
    break;
  default:
    path = [[NSString alloc] initWithCString: name encoding: NSASCIIStringEncoding];
  }

  if(mFileHandle) {
    NSFileHandle* fh = static_cast< NSFileHandle* >(mFileHandle);
    [fh release];
    mFileHandle = NULL;
    mFileSize = 0;
  }

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

nude::FILE_ERROR nuMachFile::close(void)
{
  if(mFileHandle) {
    NSFileHandle* fh = static_cast< NSFileHandle* >(mFileHandle);

    [fh release];
    mFileHandle = NULL;
    mFileSize = 0;

    return nude::FERROR_NONE;
  }

  return nude::FERROR_INVALID_OPERATION;
}

size_t nuMachFile::read(void* ptr, size_t bytes)
{
  if(!mFileHandle)
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

size_t nuMachFile::write(void* ptr, size_t bytes)
{
  if(!mFileHandle || bytes == 0)
    return 0;

  if(mAttribute == nude::FATTR_WRITE) {
    NSFileHandle* fh = static_cast< NSFileHandle* >(mFileHandle);
    NSData* data = [NSData dataWithBytesNoCopy: ptr length: bytes freeWhenDone: NO];
    [fh writeData: data];
    return bytes;
  }

  return 0;
}

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

i64 nuMachFile::tell(void) const
{
  if(!mFileHandle)
    return 0;
  NSFileHandle* fh = static_cast< NSFileHandle* >(mFileHandle);
  return [fh offsetInFile];
}
