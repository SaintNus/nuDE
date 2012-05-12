/*!
 * \file nuProgramManager.h
 * \brief GLSL program manager.
 * \author Nus
 * \date 2012/05/05 23:12
 */

#ifndef __NUPROGRAMMANAGER_H__
#define __NUPROGRAMMANAGER_H__

class nuProgramManager : public nuObject
{
  DECLARE_TYPE_INFO;

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
    typedef std::vector< Data > DataTable;
    typedef DataTable::iterator DataTableIterator;
    typedef DataTable::const_iterator DataTableConstIterator;

    DataTable mData;

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

public:
  nuProgramManager();
  ~nuProgramManager();

};

#endif
