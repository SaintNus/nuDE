/*!
 * \file nude.h
 * \brief Nude includer.
 * \author Nus
 * \date 2011/09/11 22:07
 */

#ifndef __NUDE_H__
#define __NUDE_H__

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include <assert.h>

#include <cstring>
#include <new>

#include <QuartzCore/CVDisplayLink.h>

#include <OpenGL/OpenGL.h>
#include <OpenGL/gl3.h>

#include "nuType.h"
#include "nuDebug.h"
#include "nuCRC.h"
#include "nuMemory.h"
#include "nuObject.h"
#include "nuTypeInfo.h"

#include "nuFile.h"

#ifdef __OBJC__

#import "nuMachAppDelegate.h"
#import "nuMachWindow.h"
#import "nuMachView.h"

#endif

#include "nuMachFile.h"

#endif
