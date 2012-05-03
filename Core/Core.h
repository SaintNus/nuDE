/*!
 * \file Core.h
 * \brief Core includer.
 * \author Nus
 * \date 2012/02/06 00:58
 */

#ifndef __CORE_H__
#define __CORE_H__

#include <cstddef>
#include <stdint.h>
#include <cstdio>
#include <cstdarg>
#include <cstdlib>

#include <cassert>

#include <cstring>
#include <new>

#include <algorithm>
#include <cmath>

#include <wchar.h>
#include <vector>
#include <list>

#include <libkern/OSAtomic.h>

#ifdef __OBJC__

#import <Cocoa/Cocoa.h>

#endif

#include "nuType.h"
#include "nuDebug.h"
#include "nuCRC.h"
#include "nuMemory.h"
#include "nuObject.h"
#include "nuTypeInfo.h"

#include "nuFile.h"
#include "nuThread.h"
#include "nuOSType.h"

#include "nuResource.h"
#include "nuSingleton.h"

#include "nuThreadPool.h"

#include "nuColor.h"
#include "nuHandle.h"
#include "nuRingBuffer.h"

#endif
