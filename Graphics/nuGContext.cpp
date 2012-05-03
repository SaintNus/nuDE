/*!
 * \file nuGContext.cpp
 * \brief Graphic context.
 * \author Nus
 * \date 2011/10/30 21:49
 */

#include "nuGContext.h"

nuGContext::nuGContext(nuGContextBuffer& ctx_buffer)
    : mFrameID(0),
      mBuffer(ctx_buffer),
      mpTag(nullptr),
      mTagNum(0),
      mCurrentTag(0),
      mpTempTag(nullptr),
      mTempTagNum(0)
{
  mCurrentPriority.value = 0;
}

nuGContext::~nuGContext()
{
  // None...
}

void nuGContext::clear(ui32 clear_bit, const nuColor& color, f32 depth)
{
  NU_ASSERT_C(mpTag != nullptr);
  DrawCmd< Clear >* p_clear = mBuffer.allocBuffer< DrawCmd< Clear > >();
  if(p_clear) {
    Tag& tag = mpTag[mCurrentTag];
    mCurrentTag++;
    tag.mPriority = mCurrentPriority;
    tag.mpCommand = p_clear;
    p_clear->type = CLEAR;
    p_clear->data.clear_bit = clear_bit;
    p_clear->data.clear_color = color.rgba;
    p_clear->data.depth_value = depth;
  }
}

void nuGContext::begin(i64 frame_id, Tag* p_tag, ui32 tag_num)
{
  if(mTempTagNum < tag_num) {
    if(mpTempTag) {
      delete[] mpTempTag;
      mpTempTag = nullptr;
    }

    ui32 temp_tag_num = tag_num / EXPAND_TEMP_TAG_NUM;
    temp_tag_num = (temp_tag_num + 1) * EXPAND_TEMP_TAG_NUM;

    mTempTagNum = temp_tag_num;
    mpTempTag = new Tag[mTempTagNum];
    mCurrentPriority.value = 0;

    NU_ASSERT(mpTempTag != nullptr, "Cannot allocate temporary tag.\n");
  }

  mpTag = p_tag;
  mTagNum = tag_num;
  mCurrentTag = 0;
  mFrameID = frame_id;
}

void nuGContext::end(void)
{
  if(mCurrentTag > 1)
    sortTag();
}

void nuGContext::sortTag(void)
{
  ui32 inc = 1;
  ui32 l, l_max;
  ui32 r, r_max;
  ui32 curr;

  Tag* p_tmp = mpTempTag;

  while(inc < mCurrentTag) {
    l = 0;
    r = inc;
    l_max = r - 1;
    r_max = (l_max + inc < mCurrentTag) ? l_max + inc : mCurrentTag - 1;
    curr = 0;

    while(curr < mCurrentTag) {
      while(l <= l_max && r <= r_max) {
        if(mpTag[r].mPriority.value < mpTag[l].mPriority.value)
          p_tmp[curr] = mpTag[r++];
        else
          p_tmp[curr] = mpTag[l++];
        curr++;
      }

      while(r <= r_max)
        p_tmp[curr++] = mpTag[r++];
      while (l <= l_max)
        p_tmp[curr++] = mpTag[l++];

      l = r;
      r += inc;
      l_max = r - 1;
      r_max = (l_max + inc < mCurrentTag) ? l_max + inc : mCurrentTag - 1;
    }

    inc *= 2;

    memcpy(mpTag, mpTempTag, sizeof(Tag) * mCurrentTag);
  }
}
