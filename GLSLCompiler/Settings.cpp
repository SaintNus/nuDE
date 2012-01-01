/*!
 * \file Settings.cpp
 * \brief Command line settings.
 * \author Nus
 * \date 2011/12/25 21:15
 */

#include "Settings.h"

#define COMPARE_CSTR(_arg, _str) strncmp(_arg, _str, strlen(_str))

Settings::Settings(int argc, const char** argv)
    : mpOutputName("ShdList"),
      mpInputName(nullptr),
      mValid(false),
      mForceWrite(false),
      mVerbose(false),
      mVersion(false),
      mHelp(false),
      mDebug(false)
{
  for(int ii = 1; ii < argc; ii++) {
    const char* arg_str = argv[ii];
    if(COMPARE_CSTR(arg_str, "--help") == 0) {
      mHelp = true;
      break;
    } else if(COMPARE_CSTR(arg_str, "--version") == 0) {
      mVersion = true;
      break;
    } else if(COMPARE_CSTR(arg_str, "--output-file=") == 0) {
      const char* str = arg_str + strlen("--output-file=");
      if(*str != '\0')
        mpOutputName = str;
    } else if(COMPARE_CSTR(arg_str, "--force-write") == 0) {
      mForceWrite = true;
    } else if(COMPARE_CSTR(arg_str, "--verbose") == 0) {
      mVerbose = true;
    } else if(COMPARE_CSTR(arg_str, "--debug") == 0) {
      mVerbose = true;
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
