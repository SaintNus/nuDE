/*!
 * \file Settings.cpp
 * \brief Command line settings.
 * \author Nus
 * \date 2011/12/25 21:15
 */

#include "Settings.h"

Settings::Settings(int argc, const char** argv)
    : mpOutputName("ShdList"),
      mpInputName(nullptr),
      mValid(false),
      mForceWrite(false)
{
  for(int ii = 1; ii < argc; ii++) {
    const char* arg_str = argv[ii];
    if(strncmp(arg_str, "--help", strlen("--help")) == 0) {
      mValid = false;
      break;
    } else if(strncmp(arg_str, "--output-file=", strlen("--output-file=")) == 0) {
      mpOutputName = arg_str + strlen("--output-file=");
      if(*mpOutputName == '\0')
        mpOutputName = nullptr;
    } else if(strncmp(arg_str, "--force-write", strlen("--force-write")) == 0) {
      mForceWrite = true;
    } else {
      mpInputName = arg_str;
      mValid = true;
    }
  }
}

Settings::~Settings()
{
  // None...
}
