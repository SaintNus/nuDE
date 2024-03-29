/*!
 * \file Settings.h
 * \brief Command line settings.
 * \author Nus
 * \date 2011/12/25 21:15
 */

#ifndef __SETTINGS_H__
#define __SETTINGS_H__

class Settings
{
private:
  const char* mpOutputName;
  const char* mpInputName;
  const char* mpIntermediateDir;
  const char* mpNamespace;
  const char* mpVersion;
  bool mValid;
  bool mForceWrite;
  bool mVerbose;
  bool mVersion;
  bool mHelp;
  bool mDebug;

  Settings();

public:
  Settings(int argc, const char** argv);
  ~Settings();

  const char* getOutputName(void) const {
    return mpOutputName;
  }

  const char* getInputName(void) const {
    return mpInputName;
  }

  const char* getIntermediateDir(void) const {
    return mpIntermediateDir;
  }

  const char* getNamespace(void) const {
    return mpNamespace;
  }

  const char* getVersion(void) const {
    return mpVersion;
  }

  bool isValid(void) const {
    return mValid;
  }

  bool isForceWrite(void) const {
    return mForceWrite;
  }

  bool isVerbose(void) const {
    return mVerbose;
  }

  bool isVersion(void) const {
    return mVersion;
  }

  bool isHelp(void) const {
    return mHelp;
  }

  bool isDebug(void) const {
    return mDebug;
  }

};

#endif
