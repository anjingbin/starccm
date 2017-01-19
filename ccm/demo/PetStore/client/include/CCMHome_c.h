/*
 * Copyright 2001 Borland Software Corporation, Inc.
 *
 * GENERATED CODE --- DO NOT EDIT
 * 
 */

#ifndef __CCMHome_idl___client
#define __CCMHome_idl___client
#include "vpre.h"

#ifndef _corba_h_
# include "corba.h"
#endif   // _corba_h_
#ifndef __CCMCommon_idl___client
# include "CCMCommon_c.h"
#endif
namespace Components {
  #ifndef Components_CCMObject_var_
  #define Components_CCMObject_var_

  class  CCMObject;

  typedef CCMObject* CCMObject_ptr;
  typedef CCMObject_ptr CCMObjectRef;
   VISistream& _marshal_out(VISistream&, CCMObject_ptr&);
   VISostream& _marshal_in(VISostream&, const CCMObject_ptr);
  class  CCMObject_out;

  class  CCMObject_var : public CORBA::_var {
  friend class CCMObject_out;
   private:
    CCMObject_ptr _ptr;

   public:
    CCMObject_var();
    CCMObject_var(CCMObject_ptr);
    CCMObject_var(const CCMObject_var &);
    virtual ~CCMObject_var();

    static CCMObject_ptr _duplicate(CCMObject_ptr);
    static void _release(CCMObject_ptr);

    CCMObject_var& operator=(const CCMObject_var& _var);
    CCMObject_var& operator=(CCMObject_ptr);

    operator CCMObject*() const { return _ptr; }
    CCMObject* operator->() const { return _ptr; }

    CCMObject_ptr in() const { return _ptr; }
    CCMObject_ptr& inout() { return _ptr; }
    CCMObject_ptr& out();
    CCMObject_ptr _retn();

    friend VISostream& _marshal_in(VISostream&, const CCMObject_var&);
    friend VISistream& _marshal_out(VISistream&, CCMObject_var&);

    friend Istream& _marshal_out(Istream&, CCMObject_var&);
    friend Ostream& _marshal_in(Ostream&, const CCMObject_var&);
  };
  class  CCMObject_out {
   private:
    CCMObject_ptr& _ptr;
    static CCMObject* _nil() { return (CCMObject*)NULL; }

    void operator=(const CCMObject_out&);
    void operator=(const CCMObject_var&);
     public:
    CCMObject_out(const CCMObject_out& _o) : _ptr(_o._ptr) {}
    CCMObject_out(CCMObject_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    CCMObject_out(CCMObject_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    CCMObject_out& operator=(CCMObject* _p) {
      _ptr = _p;
      return *this;
    }
    operator CCMObject_ptr&() { return _ptr; }
    CCMObject_ptr& ptr() { return _ptr; }
    CCMObject* operator->() { return _ptr; }
  };

  #endif // Components_CCMObject_var_
  class  CreateFailure : public CORBA_UserException {
   public:

    #if defined(MSVCNEWDLL_BUG)
      void *operator new(size_t ts);
      void *operator new(size_t ts, char*, int) {return operator new(ts);}
      void operator delete(void *p);
    #endif // MSVCNEWDLL_BUG

    CreateFailure();
    CreateFailure( CORBA::ULong _reason );
    virtual ~CreateFailure();

    CORBA::ULong reason;
    static CORBA::Exception *_factory() { return new CreateFailure(); }
    virtual const CORBA_Exception::Description& _desc() const;
    static CreateFailure* _downcast(CORBA::Exception *_exc);
    static const CreateFailure* _downcast(const CORBA::Exception *_exc);
    CORBA::Exception *_deep_copy() { return new CreateFailure(*this); }
  #ifdef AIXV3
    void _raise() const { VISTHROW_INST((CreateFailure*)this) }
  #else
    void _raise() const { VISTHROW_INST(this) }
  #endif

    friend void _internal_pretty_print(VISostream&, const CreateFailure&);
    void _write(VISostream&) const;
    void _copy(VISistream&);

    friend VISistream& _marshal_out(VISistream& _strm, CreateFailure& _e);
    static const CORBA_Exception::Description _description;
  };

  class  RemoveFailure : public CORBA_UserException {
   public:

    #if defined(MSVCNEWDLL_BUG)
      void *operator new(size_t ts);
      void *operator new(size_t ts, char*, int) {return operator new(ts);}
      void operator delete(void *p);
    #endif // MSVCNEWDLL_BUG

    RemoveFailure();
    RemoveFailure( CORBA::ULong _reason );
    virtual ~RemoveFailure();

    CORBA::ULong reason;
    static CORBA::Exception *_factory() { return new RemoveFailure(); }
    virtual const CORBA_Exception::Description& _desc() const;
    static RemoveFailure* _downcast(CORBA::Exception *_exc);
    static const RemoveFailure* _downcast(const CORBA::Exception *_exc);
    CORBA::Exception *_deep_copy() { return new RemoveFailure(*this); }
  #ifdef AIXV3
    void _raise() const { VISTHROW_INST((RemoveFailure*)this) }
  #else
    void _raise() const { VISTHROW_INST(this) }
  #endif

    friend void _internal_pretty_print(VISostream&, const RemoveFailure&);
    void _write(VISostream&) const;
    void _copy(VISistream&);

    friend VISistream& _marshal_out(VISistream& _strm, RemoveFailure& _e);
    static const CORBA_Exception::Description _description;
  };

  #ifndef Components_CCMHome_var_
  #define Components_CCMHome_var_

  class  CCMHome;

  typedef CCMHome* CCMHome_ptr;
  typedef CCMHome_ptr CCMHomeRef;
   VISistream& _marshal_out(VISistream&, CCMHome_ptr&);
   VISostream& _marshal_in(VISostream&, const CCMHome_ptr);
  class  CCMHome_out;

  class  CCMHome_var : public CORBA::_var {
  friend class CCMHome_out;
   private:
    CCMHome_ptr _ptr;

   public:
    CCMHome_var();
    CCMHome_var(CCMHome_ptr);
    CCMHome_var(const CCMHome_var &);
    virtual ~CCMHome_var();

    static CCMHome_ptr _duplicate(CCMHome_ptr);
    static void _release(CCMHome_ptr);

    CCMHome_var& operator=(const CCMHome_var& _var);
    CCMHome_var& operator=(CCMHome_ptr);

    operator CCMHome*() const { return _ptr; }
    CCMHome* operator->() const { return _ptr; }

    CCMHome_ptr in() const { return _ptr; }
    CCMHome_ptr& inout() { return _ptr; }
    CCMHome_ptr& out();
    CCMHome_ptr _retn();

    friend VISostream& _marshal_in(VISostream&, const CCMHome_var&);
    friend VISistream& _marshal_out(VISistream&, CCMHome_var&);

    friend Istream& _marshal_out(Istream&, CCMHome_var&);
    friend Ostream& _marshal_in(Ostream&, const CCMHome_var&);
  };
  class  CCMHome_out {
   private:
    CCMHome_ptr& _ptr;
    static CCMHome* _nil() { return (CCMHome*)NULL; }

    void operator=(const CCMHome_out&);
    void operator=(const CCMHome_var&);
     public:
    CCMHome_out(const CCMHome_out& _o) : _ptr(_o._ptr) {}
    CCMHome_out(CCMHome_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    CCMHome_out(CCMHome_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    CCMHome_out& operator=(CCMHome* _p) {
      _ptr = _p;
      return *this;
    }
    operator CCMHome_ptr&() { return _ptr; }
    CCMHome_ptr& ptr() { return _ptr; }
    CCMHome* operator->() { return _ptr; }
  };

  #endif // Components_CCMHome_var_

  // idl interface: Components::CCMHome
  class  CCMHome : public virtual CORBA_Object {
   private:
    static const CORBA::TypeInfo _class_info;
    void operator=(const CCMHome&) {}

    protected:
    CCMHome() {}
    CCMHome(const CCMHome&) {}

   public:
    virtual ~CCMHome() {}
    static  const CORBA::TypeInfo *_desc();
    virtual const CORBA::TypeInfo *_type_info() const;
    virtual void *_safe_narrow(const CORBA::TypeInfo& ) const;
    static CORBA::Object*_factory();
    CCMHome_ptr _this();
    static CCMHome_ptr _duplicate(CCMHome_ptr _obj) {
      if (_obj) _obj->_ref();
      return _obj;
    }
    static CCMHome_ptr _nil() { return (CCMHome_ptr)NULL; }
    static CCMHome_ptr _narrow(CORBA::Object* _obj);
    static CCMHome_ptr _clone(CCMHome_ptr _obj) {
      CORBA::Object_var _obj_var(CORBA_Object::_clone(_obj));
      #if defined(_HPCC_BUG)
        return _narrow(_obj_var.operator CORBA::Object_ptr());
      #else
        return _narrow(_obj_var);
      #endif  // _HPCC_BUG
    }
    static CCMHome_ptr _bind(const char *_object_name = (const char*)NULL,
                             const char *_host_name = (const char*)NULL,
                             const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                             CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    static CCMHome_ptr _bind(const char *_poa_name,
                             const CORBA::OctetSequence& _id,
                             const char *_host_name = (const char*)NULL,
                             const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                             CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    virtual ::CORBA::IRObject_ptr get_component_def();
    virtual ::CORBA::IRObject_ptr get_home_def();
    virtual void remove_component(CCMObject_ptr _comp);

    friend VISostream& _marshal_in(VISostream& _strm, const CCMHome_ptr _obj);
    friend VISistream& _marshal_out(VISistream& _strm, CCMHome_ptr& _obj);
    friend Ostream& _marshal_in(Ostream& _strm, const CCMHome_ptr _obj);
    friend Istream& _marshal_out(Istream& _strm, CCMHome_ptr& _obj);
  };

  class CCMHome_ops;
  typedef CCMHome_ops* CCMHome_ops_ptr;

  class  CCMHome_ops  {
   public:
    CCMHome_ops() {}
    virtual ~CCMHome_ops () {}

    virtual ::CORBA::IRObject_ptr get_component_def() = 0;
    virtual ::CORBA::IRObject_ptr get_home_def() = 0;
    virtual void remove_component(CCMObject_ptr _comp) = 0;

    static const VISOps_Info *_desc();
    static CCMHome_ops_ptr _nil() { return (CCMHome_ops_ptr)NULL; }
    static CCMHome_ops_ptr _downcast(PortableServer::ServantBase* _servant);
  protected:
    static const VISOps_Info _ops_info;
  };

  #ifndef Components_KeylessCCMHome_var_
  #define Components_KeylessCCMHome_var_

  class  KeylessCCMHome;

  typedef KeylessCCMHome* KeylessCCMHome_ptr;
  typedef KeylessCCMHome_ptr KeylessCCMHomeRef;
   VISistream& _marshal_out(VISistream&, KeylessCCMHome_ptr&);
   VISostream& _marshal_in(VISostream&, const KeylessCCMHome_ptr);
  class  KeylessCCMHome_out;

  class  KeylessCCMHome_var : public CORBA::_var {
  friend class KeylessCCMHome_out;
   private:
    KeylessCCMHome_ptr _ptr;

   public:
    KeylessCCMHome_var();
    KeylessCCMHome_var(KeylessCCMHome_ptr);
    KeylessCCMHome_var(const KeylessCCMHome_var &);
    virtual ~KeylessCCMHome_var();

    static KeylessCCMHome_ptr _duplicate(KeylessCCMHome_ptr);
    static void _release(KeylessCCMHome_ptr);

    KeylessCCMHome_var& operator=(const KeylessCCMHome_var& _var);
    KeylessCCMHome_var& operator=(KeylessCCMHome_ptr);

    operator KeylessCCMHome*() const { return _ptr; }
    KeylessCCMHome* operator->() const { return _ptr; }

    KeylessCCMHome_ptr in() const { return _ptr; }
    KeylessCCMHome_ptr& inout() { return _ptr; }
    KeylessCCMHome_ptr& out();
    KeylessCCMHome_ptr _retn();

    friend VISostream& _marshal_in(VISostream&, const KeylessCCMHome_var&);
    friend VISistream& _marshal_out(VISistream&, KeylessCCMHome_var&);

    friend Istream& _marshal_out(Istream&, KeylessCCMHome_var&);
    friend Ostream& _marshal_in(Ostream&, const KeylessCCMHome_var&);
  };
  class  KeylessCCMHome_out {
   private:
    KeylessCCMHome_ptr& _ptr;
    static KeylessCCMHome* _nil() { return (KeylessCCMHome*)NULL; }

    void operator=(const KeylessCCMHome_out&);
    void operator=(const KeylessCCMHome_var&);
     public:
    KeylessCCMHome_out(const KeylessCCMHome_out& _o) : _ptr(_o._ptr) {}
    KeylessCCMHome_out(KeylessCCMHome_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    KeylessCCMHome_out(KeylessCCMHome_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    KeylessCCMHome_out& operator=(KeylessCCMHome* _p) {
      _ptr = _p;
      return *this;
    }
    operator KeylessCCMHome_ptr&() { return _ptr; }
    KeylessCCMHome_ptr& ptr() { return _ptr; }
    KeylessCCMHome* operator->() { return _ptr; }
  };

  #endif // Components_KeylessCCMHome_var_

  // idl interface: Components::KeylessCCMHome
  class  KeylessCCMHome : public virtual CORBA_Object {
   private:
    static const CORBA::TypeInfo _class_info;
    void operator=(const KeylessCCMHome&) {}

    protected:
    KeylessCCMHome() {}
    KeylessCCMHome(const KeylessCCMHome&) {}

   public:
    virtual ~KeylessCCMHome() {}
    static  const CORBA::TypeInfo *_desc();
    virtual const CORBA::TypeInfo *_type_info() const;
    virtual void *_safe_narrow(const CORBA::TypeInfo& ) const;
    static CORBA::Object*_factory();
    KeylessCCMHome_ptr _this();
    static KeylessCCMHome_ptr _duplicate(KeylessCCMHome_ptr _obj) {
      if (_obj) _obj->_ref();
      return _obj;
    }
    static KeylessCCMHome_ptr _nil() { return (KeylessCCMHome_ptr)NULL; }
    static KeylessCCMHome_ptr _narrow(CORBA::Object* _obj);
    static KeylessCCMHome_ptr _clone(KeylessCCMHome_ptr _obj) {
      CORBA::Object_var _obj_var(CORBA_Object::_clone(_obj));
      #if defined(_HPCC_BUG)
        return _narrow(_obj_var.operator CORBA::Object_ptr());
      #else
        return _narrow(_obj_var);
      #endif  // _HPCC_BUG
    }
    static KeylessCCMHome_ptr _bind(const char *_object_name = (const char*)NULL,
                                    const char *_host_name = (const char*)NULL,
                                    const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                                    CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    static KeylessCCMHome_ptr _bind(const char *_poa_name,
                                    const CORBA::OctetSequence& _id,
                                    const char *_host_name = (const char*)NULL,
                                    const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                                    CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    virtual CCMObject_ptr create_component();

    friend VISostream& _marshal_in(VISostream& _strm, const KeylessCCMHome_ptr _obj);
    friend VISistream& _marshal_out(VISistream& _strm, KeylessCCMHome_ptr& _obj);
    friend Ostream& _marshal_in(Ostream& _strm, const KeylessCCMHome_ptr _obj);
    friend Istream& _marshal_out(Istream& _strm, KeylessCCMHome_ptr& _obj);
  };

  class KeylessCCMHome_ops;
  typedef KeylessCCMHome_ops* KeylessCCMHome_ops_ptr;

  class  KeylessCCMHome_ops  {
   public:
    KeylessCCMHome_ops() {}
    virtual ~KeylessCCMHome_ops () {}

    virtual CCMObject_ptr create_component() = 0;

    static const VISOps_Info *_desc();
    static KeylessCCMHome_ops_ptr _nil() { return (KeylessCCMHome_ops_ptr)NULL; }
    static KeylessCCMHome_ops_ptr _downcast(PortableServer::ServantBase* _servant);
  protected:
    static const VISOps_Info _ops_info;
  };

}
inline VISostream& operator<<(VISostream& s, const Components::CCMObject_var& v) {
  return Components::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const Components::CCMHome_ptr v) {
  return Components::_marshal_in(s, v);
}
inline Ostream& operator<<(Ostream& s, const Components::CCMHome_var& v) {
  return Components::_marshal_in(s, v);
}
inline Ostream& operator<<(Ostream& s, const Components::CCMObject_var& v) {
  return Components::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, Components::CCMObject_var& v) {
  return Components::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const Components::CCMHome_var& v) {
  return Components::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::CCMObject_var& v) {
  return Components::_marshal_out(s, v);
}
inline Ostream& operator<<(Ostream& s, const Components::CCMHome_ptr v) {
  return Components::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::CCMObject_ptr& v) {
  return Components::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::CreateFailure& v) {
  return Components::_marshal_out(s, v);
}
inline Ostream& operator<<(Ostream& s, const Components::KeylessCCMHome_var& v) {
  return Components::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, Components::KeylessCCMHome_var& v) {
  return Components::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::KeylessCCMHome_var& v) {
  return Components::_marshal_out(s, v);
}
inline void _pretty_print(VISostream& s, const Components::CreateFailure& v) {
  Components::_internal_pretty_print(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::CCMHome_var& v) {
  return Components::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, Components::KeylessCCMHome_ptr& v) {
  return Components::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, Components::CCMHome_ptr& v) {
  return Components::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::CCMHome_ptr& v) {
  return Components::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::RemoveFailure& v) {
  return Components::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const Components::KeylessCCMHome_var& v) {
  return Components::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const Components::CCMObject_ptr v) {
  return Components::_marshal_in(s, v);
}
inline void _pretty_print(VISostream& s, const Components::RemoveFailure& v) {
  Components::_internal_pretty_print(s, v);
}
inline Ostream& operator<<(Ostream& s, const Components::KeylessCCMHome_ptr v) {
  return Components::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const Components::KeylessCCMHome_ptr v) {
  return Components::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, Components::CCMHome_var& v) {
  return Components::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::KeylessCCMHome_ptr& v) {
  return Components::_marshal_out(s, v);
}

#include "vpost.h"
#endif // __CCMHome_idl___client
