/*!
 * \file nuTypeInfo.h
 * \brief Type information.
 * \author Nus
 * \date 2010/04/15 23:40
 */

#ifndef __NUTYPEINFO_H__
#define __NUTYPEINFO_H__

/*!
 * \class nuTypeInfo
 * \brief Type information.
 */
class nuTypeInfo
{
public:
  //! \brief Type.
  enum Type {
    ROOT = 0,
    DEFAULT,
    ABSTRACT,
  };

  //! \brief Constructor.
  nuTypeInfo(ccstr class_name, nuTypeInfo* type_info);
  //! \brief Default destructor.
  virtual ~nuTypeInfo() {
    // None...
  }

  //! \brief Get class name.
  ccstr getName(void) const {
    return mName;
  }

  //! \brief Get parent.
  const nuTypeInfo* getParent(void) const {
    return mpSuperClass;
  }
  //! \brief Get child.
  const nuTypeInfo* getChild(void) const {
    return mpSubClass;
  }

  //! \brief Get sibling.
  const nuTypeInfo* getSibling(void) const {
    return mpNextClass;
  }

  //! \brief Get size.
  virtual size_t getSize(void) const = 0;
  //! \brief Get type.
  virtual Type getType(void) const = 0;

  //! \brief Create instance
  virtual class nuObject* createInstance(void) const = 0;

  //! \brief Search by name.
  static const nuTypeInfo* searchByName(ccstr class_name) {
    return mManager.searchByName(class_name);
  }

#if !NDEBUG
  //! \brief Print all type infos.
  static void listAll(void);
#endif

  //! \brief Is derived from.
  bool isDerivedFrom(ccstr parent_name) const;
  //! \brief Is derived from.
  bool isDerivedFrom(const nuTypeInfo& parent) const;

protected:
  ccstr mName;                    //!< Name.
  ui32 mID;                       //!< ID.

  nuTypeInfo* mpSubClass;         //!< Sub-class.
  nuTypeInfo* mpSuperClass;       //!< Super class.
  nuTypeInfo* mpNextClass;        //!< Next class.

  nuTypeInfo* mpNextTypeInfo;     //!< Hash table.

  //! \brief Register type info.
  void registerTypeInfo(void) {
    mManager.registerTypeInfo(this);
  }

#if !NDEBUG
  //! \brief Print all type infos.
  static void printTree(const nuTypeInfo* type_info, ui32 lv);
#endif

private:
  static const ui32 MAX_TABLE = 32; //!< Max. table entry.

  /*!
   * \class TypeInfoMgr
   * \brief Type info manager.
   */
  static class TypeInfoMgr
  {
  public:
    //! \brief Default constructor.
    TypeInfoMgr() {
      // None...
    }
    //! \brief Default destructor.
    ~TypeInfoMgr() {
      // None...
    }

    //! \brief Get root.
    const nuTypeInfo* getRoot(void) const {
      return mpRoot;
    }

    //! \brief Search for type info.
    const nuTypeInfo* searchByName(ccstr class_name);

    //! \brief Register type info.
    void registerTypeInfo(nuTypeInfo* type_info);

  private:
    nuTypeInfo* mpRoot;           //!< Root.
    nuTypeInfo* mTable[MAX_TABLE]; //!< Hash table.

    //! \brief CRC to hash address.
    ui32 hashAddr(ui32 crc) {
      ui32 ret = crc;
     // Get 5 most significant bits.
      ret >>= 27;
      return ret;
    }

  } mManager;    //!< Manager.

  //! \brief Default constructor.
  //! \warning Do not use.
  nuTypeInfo();

  //! \brief Operator new.
  //! \warning Do not use.
  void* operator new(size_t sz);
  //! \brief Operator new.
  //! \warning Do not use.
  void* operator new[](size_t sz);
  
  //! \brief Is kind of.
  bool internalIsDerivedFrom(ui32 id) const {
    const nuTypeInfo* p_curr = this;
    while(p_curr) {
      if(p_curr->mID == id)
        return true;
      p_curr = p_curr->mpSuperClass;
    }
    return false;
  }

};



#define DECLARE_TYPE_INFO \
  private: \
    class PrivateTypeInfo : public nuTypeInfo { \
    public: \
      PrivateTypeInfo() __attribute__((used)); \
      ~PrivateTypeInfo() __attribute__((used)); \
      size_t getSize(void) const __attribute__((used, always_inline)); \
      Type getType(void) const __attribute__((used, always_inline)); \
      nuObject* createInstance(void) const __attribute__((used, always_inline)); \
    }; \
  protected: \
    static PrivateTypeInfo mTypeInfo __attribute__((used)); \
  public: \
    static const nuTypeInfo& TypeInfo(void) __attribute__((used, always_inline)); \
  private: \

#define IMPLEMENT_TYPE_INFO(_class, _super) \
  _class::PrivateTypeInfo::PrivateTypeInfo() : nuTypeInfo(#_class, &_super::mTypeInfo) {} \
  _class::PrivateTypeInfo::~PrivateTypeInfo() {} \
  size_t _class::PrivateTypeInfo::getSize(void) const { return sizeof(_class); } \
  nuTypeInfo::Type _class::PrivateTypeInfo::getType(void) const { return nuTypeInfo::DEFAULT; } \
  nuObject* _class::PrivateTypeInfo::createInstance(void) const { return new _class; } \
  _class::PrivateTypeInfo _class::mTypeInfo; \
  const nuTypeInfo& _class::TypeInfo(void) { return mTypeInfo; }

#define IMPLEMENT_TYPE_INFO_INST(_class, _super, _new) \
  _class::PrivateTypeInfo::PrivateTypeInfo() : nuTypeInfo(#_class, &_super::mTypeInfo) {} \
  _class::PrivateTypeInfo::~PrivateTypeInfo() {} \
  size_t _class::PrivateTypeInfo::getSize(void) const { return sizeof(_class); } \
  nuTypeInfo::Type _class::PrivateTypeInfo::getType(void) const { return nuTypeInfo::DEFAULT; } \
  nuObject* _class::PrivateTypeInfo::createInstance(void) const { return _new; } \
  _class::PrivateTypeInfo _class::mTypeInfo; \
  const nuTypeInfo& _class::TypeInfo(void) { return mTypeInfo; }

#define IMPLEMENT_TYPE_INFO_ABSTRACT(_class, _super) \
  _class::PrivateTypeInfo::PrivateTypeInfo() : nuTypeInfo(#_class, &_super::mTypeInfo) {} \
  _class::PrivateTypeInfo::~PrivateTypeInfo() {} \
  size_t _class::PrivateTypeInfo::getSize(void) const { return sizeof(_class); } \
  nuTypeInfo::Type _class::PrivateTypeInfo::getType(void) const { return nuTypeInfo::ABSTRACT; } \
  nuObject* _class::PrivateTypeInfo::createInstance(void) const { return nullptr; } \
  _class::PrivateTypeInfo _class::mTypeInfo; \
  const nuTypeInfo& _class::TypeInfo(void) { return mTypeInfo; }



#endif
