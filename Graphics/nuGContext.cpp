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

  mBuffer.reset();
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

  Tag* p_dest = mpTempTag;
  Tag* p_src = mpTag;

  while(inc < mCurrentTag) {
    l = 0;
    r = inc;
    l_max = r - 1;
    r_max = (l_max + inc < mCurrentTag) ? l_max + inc : mCurrentTag - 1;
    curr = 0;

    while(curr < mCurrentTag) {
      while(l <= l_max && r <= r_max) {
        if(p_src[r].mPriority.value < p_src[l].mPriority.value)
          p_dest[curr] = p_src[r++];
        else
          p_dest[curr] = p_src[l++];
        curr++;
      }

      while(r <= r_max)
        p_dest[curr++] = p_src[r++];
      while (l <= l_max)
        p_dest[curr++] = p_src[l++];

      l = r;
      r += inc;
      l_max = r - 1;
      r_max = (l_max + inc < mCurrentTag) ? l_max + inc : mCurrentTag - 1;
    }

    inc *= 2;

    if(inc < mCurrentTag) {
      Tag* p_tmp = p_src;
      p_src = p_dest;
      p_dest = p_tmp;
    }
  }

  if(p_dest != mpTag)
    memcpy(mpTag, mpTempTag, sizeof(Tag) * mCurrentTag);
}

void nuGContext::createTagList(TagList& tag_list, SortTagContext* ctx, ui32 ctx_num)
{
  ui32 tag_num = 0;
  for(ui32 ui = 0; ui < ctx_num; ui++) {
    tag_num += ctx[ui].mTagNum;
  }

  if(tag_num == 0) {
    tag_list.mTagNum = 0;
    return;
  }

  tag_list.reserve(tag_num);
  tag_list.mTagNum = tag_num;

  for(ui32 ui = 0; ui < tag_list.mTagNum; ui++) {
    ui32 max_val = 0xffffffff;
    i32 min_idx = -1;
    for(ui32 uj = 0; uj < ctx_num; uj++) {
      if(ctx[uj].mTagNum == 0)
        continue;
      i32 idx = ctx[uj].mpContext->mCurrentTag - ctx[uj].mTagNum;
      if(max_val > ctx[uj].mpContext->mpTag[idx].mPriority.value) {
        max_val = ctx[uj].mpContext->mpTag[idx].mPriority.value;
        min_idx = uj;
      }
    }

    if(min_idx >= 0) {
      i32 idx = ctx[min_idx].mpContext->mCurrentTag - ctx[min_idx].mTagNum;
      ui32 prio = ctx[min_idx].mpContext->mpTag[idx].mPriority.value;

      ctx[min_idx].mTagNum--;
      tag_list.mpTagList[ui] = ctx[min_idx].mpContext->mpTag[idx];
      
      {
        if(ctx[min_idx].mTagNum > 0) {
          idx = ctx[min_idx].mpContext->mCurrentTag - ctx[min_idx].mTagNum;
          while(prio == ctx[min_idx].mpContext->mpTag[idx].mPriority.value) {
            ui++;
            tag_list.mpTagList[ui] = ctx[min_idx].mpContext->mpTag[idx];
            ctx[min_idx].mTagNum--;
            if(ctx[min_idx].mTagNum == 0)
              break;
            idx = ctx[min_idx].mpContext->mCurrentTag - ctx[min_idx].mTagNum;
          }
        }
      }
    }
  }
}
