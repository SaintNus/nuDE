/*!
 * \file nuShaderList.h
 * \brief Shader list resource.
 * \author Nus
 * \date 2012/05/05 23:12
 */

#ifndef __NUSHADERLIST_H__
#define __NUSHADERLIST_H__

#include "nuShaderInfo.h"

class nuShaderList : public nuResource
{
  DECLARE_TYPE_INFO;
  friend nude::Handle< nuShaderList >;
  friend class nuGResManager;

public:
  struct Data {
    union {
      ui32 attribute;
      struct {
        ui32 initialized: 1;
        ui32 reserved: 31;
      };
    };
    void* vsh_data;
    size_t vsh_size;
    GLuint vsh_id;
    void* fsh_data;
    size_t fsh_size;
    GLuint fsh_id;
  };

private:
  Data mData[nude::ProgramList_Num];

  nuShaderList();
  ~nuShaderList();

  ccstr getExt(void) const {
    return "shlst";
  }

  ERROR_CODE setup(const nuStream& stream);

  void compileData(Data& data);
  GLuint compileShader(GLenum shader, const GLchar** sh_data, GLint sh_size);

public:
  const Data& getData(ui32 index) const {
    if(mData[index].initialized == 0) {
      Data& data = const_cast< Data& >(mData[index]);
      const_cast< nuShaderList* >(this)->compileData(data);
    }
    return mData[index];
  }

};

namespace nude {
  typedef Handle< nuShaderList > ShaderList;
}

#endif
