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
    tag.priority = mCurrentPriority;
    tag.command = p_clear;
    p_clear->type = CLEAR;
    p_clear->data.clear_bit = clear_bit;
    p_clear->data.clear_color = color.rgba;
    p_clear->data.depth_value = depth;
  }
}

void nuGContext::begin(ui32 tag_num, Tag* p_tag)
{
  NU_ASSERT_C(mpTag == nullptr);

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
}

void nuGContext::end(void)
{
  sortTag();
  mpTag = nullptr;
  mTagNum = 0;
  mCurrentTag = 0;
}

void nuGContext::sortTag(void)
{

}
