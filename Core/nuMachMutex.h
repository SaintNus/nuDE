/*!
 * \file nuMachMutex.h
 * \brief Mutex for MacOSX.
 * \author Nus
 * \date 2011/11/26 23:15
 */

#ifndef __NUMACHMUTEX_H__
#define __NUMACHMUTEX_H__

class nuMachMutex
{
public:
  nuMachMutex();
  ~nuMachMutex();
  
  void lock(void);
  void unlock(void);
  bool trylock(void);

private:
  void* mpMutex;

};

class nuMachRecursiveMutex
{
public:
  nuMachRecursiveMutex();
  ~nuMachRecursiveMutex();
  
  void lock(void);
  void unlock(void);
  bool trylock(void);

private:
  void* mpRecursiveMutex;

};

class nuMachSpinLock
{
public:
  nuMachSpinLock();
  ~nuMachSpinLock();
  
  void lock(void);
  void unlock(void);
  bool trylock(void);

private:
  volatile OSSpinLock mSpinLock;

};

#endif
