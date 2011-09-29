/*!
 * \file nuMachFile.h
 * \brief File handler for MacOSX.
 * \author Nus
 * \date 2011/09/19 18:59
 */

#ifndef __NUMACHFILE_H__
#define __NUMACHFILE_H__

class nuMachFile
{
public:
  nuMachFile();
  ~nuMachFile();

private:
  void* mFileHandler;
  
};

typedef nude::File< nuMachFile > nuFile;

#endif
