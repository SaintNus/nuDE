/*!
 * \file nuProgramManager.h
 * \brief GLSL program manager.
 * \author Nus
 * \date 2012/05/05 23:12
 */

#ifndef __NUPROGRAMMANAGER_H__
#define __NUPROGRAMMANAGER_H__

#include "nuShaderInfo.h"

class nuProgramManager : public nuObject
{
  DECLARE_TYPE_INFO;

public:
  class ShaderProgram {
    friend nuProgramManager;
    friend class nuRenderGL;

    GLuint mVshID;
    GLuint mFshID;
    GLuint mProgID;

    volatile i32 mRefCount;
    volatile i64 mFrameID;

    struct UniformBinding {
      GLuint location;
      union {
        GLuint binding;
        void* data;
      };
    };

    UniformBinding* mpUniform;
    UniformBinding* mpUniformBlock;

    void createUniform(ui32 size) {
      if(!mpUniform)
        mpUniform = new UniformBinding[size];
    }

    void createUniformBlock(ui32 size) {
      if(!mpUniformBlock)
        mpUniformBlock = new UniformBinding[size];
    }

  public:
    ShaderProgram()
        : mVshID(0),
          mFshID(0),
          mProgID(0),
          mRefCount(0),
          mFrameID(0),
          mpUniform(nullptr),
          mpUniformBlock(nullptr)
    {
      // None...
    }
    ~ShaderProgram() {
      if(mpUniform) {
        delete[] mpUniform;
        mpUniform = nullptr;
      }
      if(mpUniformBlock) {
        delete[] mpUniformBlock;
        mpUniformBlock = nullptr;
      }
    }
    GLuint getVshID(void) const {
      return mVshID;
    }
    GLuint getFshID(void) const {
      return mFshID;
    }
    GLuint getProgID(void) const {
      return mProgID;
    }
  };

private:
  class ShaderList : public nuResource {
    DECLARE_TYPE_INFO;
    friend nude::Handle< ShaderList >;

  public:
    struct Data {
      void* vsh_data;
      size_t vsh_size;
      void* fsh_data;
      size_t fsh_size;
    };

  private:
    Data mData[nude::ProgramList_Num];

    ShaderList();
    ~ShaderList();

    ccstr getExt(void) const {
      return "shlst";
    }

    ERROR_CODE setup(const nuStream& stream);

  public:
    const Data& getData(ui32 index) const {
      return mData[index];
    }

  };

  typedef nude::Handle< ShaderList > ShdListHandle;
  ShdListHandle mShdListHandle;
  ShaderProgram mShaderProgram[nude::ProgramList_Num];

  GLuint compileShader(GLenum shader, const GLchar** p_data, GLint size);

public:
  nuProgramManager();
  ~nuProgramManager();

  void initializeResource(nuResourceManager& resource_mgr, ccstr file_path);
  void terminateResource(void);

  void update(i64 frame_id);

  void protect(ShaderProgram& program, ui64 frame_id) {
    i64 curr = program.mFrameID;
    i64 res = frame_id;
    while(curr < res) {
      bool ret = nuAtomic::casBarrier(curr, res, &program.mFrameID);
      if(!ret)
        curr = program.mFrameID;
      else
        break;
    }
  }
};

#endif
