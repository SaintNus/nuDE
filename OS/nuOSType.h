/*!
 * \file nuOSType.h
 * \brief OS types.
 * \author Nus
 * \date 2011/11/26 22:28
 */

#ifndef __NUOSTYPE_H__
#define __NUOSTYPE_H__

#include "nuFile.h"
#include "nuMachFile.h"
#include "nuThread.h"
#include "nuMachThread.h"
#include "nuMutex.h"
#include "nuMachMutex.h"
#include "nuAtomic.h"
#include "nuMachAtomic.h"

//! \brief Define template specialization for file.
typedef nude::File< nuMachFile > nuFile;

//! \brief Define template specialization for thread.
typedef nude::Thread< nuMachThread > nuThread;

//! \brief Define template specialization for mutex.
typedef nude::Mutex< nuMachMutex > nuMutex;

//! \brief Define template specialization for recursive mutex.
typedef nude::Mutex< nuMachRecursiveMutex > nuRecursiveMutex;

//! \brief Define template specialization for spin lock.
typedef nude::Mutex< nuMachSpinLock > nuSpinLock;

//! \brief Define template specialization for atomic.
typedef nude::Atomic< nuMachAtomic > nuAtomic;

#endif
