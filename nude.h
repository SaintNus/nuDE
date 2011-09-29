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

#include <assert.h>

#include <cstring>
#include <new>

#include <QuartzCore/CVDisplayLink.h>

#include <OpenGL/OpenGL.h>
#include <OpenGL/gl3.h>

#include "Core/nuType.h"
#include "Core/nuDebug.h"
#include "Core/nuCRC.h"
#include "Core/nuMemory.h"
#include "Core/nuObject.h"
#include "Core/nuTypeInfo.h"

#include "Core/nuFile.h"

#ifdef __OBJC__

#import "OS/nuMachAppDelegate.h"
#import "OS/nuMachWindow.h"
#import "OS/nuMachView.h"

#endif

#endif
