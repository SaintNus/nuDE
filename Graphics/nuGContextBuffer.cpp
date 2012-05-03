/*!
 * \file nuGContextBuffer.cpp
 * \brief Buffer for graphic context.
 * \author Nus
 * \date 2012/05/02 19:35
 */

#include "nuGContextBuffer.h"

nuGContextBuffer::nuGContextBuffer(size_t rb_size)
    : mRingBuffer(rb_size)
{

}

nuGContextBuffer::~nuGContextBuffer()
{

}

void initialize(size_t rb_size);
