/*
 * Copyright 2001 Borland Software Corporation, Inc.
 *
 * GENERATED CODE --- DO NOT EDIT
 * 
 */

#ifndef __HomeFinder_idl___client
#define __HomeFinder_idl___client
#include "vpre.h"

#ifndef _corba_h_
# include "corba.h"
#endif   // _corba_h_
#ifndef __CCMHome_idl___client
# include "CCMHome_c.h"
#endif
namespace Components {
  class  HomeNotFound : public CORBA_UserException {
   public:

    #if defined(MSVCNEWDLL_BUG)
      void *operator new(size_t ts);
      void *operator new(size_t ts, char*, int) {return operator new(ts);}
      void operator delete(void *p);
    #endif // MSVCNEWDLL_BUG

    HomeNotFound();
    virtual ~HomeNotFound();

    
    static CORBA::Exception *_factory() { return new HomeNotFound(); }
    virtual const CORBA_Exception::Description& _desc() const;
    static HomeNotFound* _downcast(CORBA::Exception *_exc);
    static const HomeNotFound* _downcast(const CORBA::Exception *_exc);
    CORBA::Exception *_deep_copy() { return new HomeNotFound(*this); }
  #ifdef AIXV3
    void _raise() const { VISTHROW_INST((HomeNotFound*)this) }
  #else
    void _raise() const { VISTHROW_INST(this) }
  #endif

    friend void _internal_pretty_print(VISostream&, const HomeNotFound&);
    void _write(VISostream&) const;
    void _copy(VISistream&);

    friend VISistream& _marshal_out(VISistream& _strm, HomeNotFound& _e);
    static const CORBA_Exception::Description _description;
  };

  #ifndef Components_HomeFinder_var_
  #define Components_HomeFinder_var_

  class  HomeFinder;

  typedef HomeFinder* HomeFinder_ptr;
  typedef HomeFinder_ptr HomeFinderRef;
   VISistream& _marshal_out(VISistream&, HomeFinder_ptr&);
   VISostream& _marshal_in(VISostream&, const HomeFinder_ptr);
  class  HomeFinder_out;

  class  HomeFinder_var : public CORBA::_var {
  friend class HomeFinder_out;
   private:
    HomeFinder_ptr _ptr;

   public:
    HomeFinder_var();
    HomeFinder_var(HomeFinder_ptr);
    HomeFinder_var(const HomeFinder_var &);
    virtual ~HomeFinder_var();

    static HomeFinder_ptr _duplicate(HomeFinder_ptr);
    static void _release(HomeFinder_ptr);

    HomeFinder_var& operator=(const HomeFinder_var& _var);
    HomeFinder_var& operator=(HomeFinder_ptr);

    operator HomeFinder*() const { return _ptr; }
    HomeFinder* operator->() const { return _ptr; }

    HomeFinder_ptr in() const { return _ptr; }
    HomeFinder_ptr& inout() { return _ptr; }
    HomeFinder_ptr& out();
    HomeFinder_ptr _retn();

    friend VISostream& _marshal_in(VISostream&, const HomeFinder_var&);
    friend VISistream& _marshal_out(VISistream&, HomeFinder_var&);

    friend Istream& _marshal_out(Istream&, HomeFinder_var&);
    friend Ostream& _marshal_in(Ostream&, const HomeFinder_var&);
  };
  class  HomeFinder_out {
   private:
    HomeFinder_ptr& _ptr;
    static HomeFinder* _nil() { return (HomeFinder*)NULL; }

    void operator=(const HomeFinder_out&);
    void operator=(const HomeFinder_var&);
     public:
    HomeFinder_out(const HomeFinder_out& _o) : _ptr(_o._ptr) {}
    HomeFinder_out(HomeFinder_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    HomeFinder_out(HomeFinder_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    HomeFinder_out& operator=(HomeFinder* _p) {
      _ptr = _p;
      return *this;
    }
    operator HomeFinder_ptr&() { return _ptr; }
    HomeFinder_ptr& ptr() { return _ptr; }
    HomeFinder* operator->() { return _ptr; }
  };

  #endif // Components_HomeFinder_var_

  // idl interface: Components::HomeFinder
  class  HomeFinder : public virtual CORBA_Object {
   private:
    static const CORBA::TypeInfo _class_info;
    void operator=(const HomeFinder&) {}

    protected:
    HomeFinder() {}
    HomeFinder(const HomeFinder&) {}

   public:
    virtual ~HomeFinder() {}
    static  const CORBA::TypeInfo *_desc();
    virtual const CORBA::TypeInfo *_type_info() const;
    virtual void *_safe_narrow(const CORBA::TypeInfo& ) const;
    static CORBA::Object*_factory();
    HomeFinder_ptr _this();
    static HomeFinder_ptr _duplicate(HomeFinder_ptr _obj) {
      if (_obj) _obj->_ref();
      return _obj;
    }
    static HomeFinder_ptr _nil() { return (HomeFinder_ptr)NULL; }
    static HomeFinder_ptr _narrow(CORBA::Object* _obj);
    static HomeFinder_ptr _clone(HomeFinder_ptr _obj) {
      CORBA::Object_var _obj_var(CORBA_Object::_clone(_obj));
      #if defined(_HPCC_BUG)
        return _narrow(_obj_var.operator CORBA::Object_ptr());
      #else
        return _narrow(_obj_var);
      #endif  // _HPCC_BUG
    }
    static HomeFinder_ptr _bind(const char *_object_name = (const char*)NULL,
                                const char *_host_name = (const char*)NULL,
                                const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                                CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    static HomeFinder_ptr _bind(const char *_poa_name,
                                const CORBA::OctetSequence& _id,
                                const char *_host_name = (const char*)NULL,
                                const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                                CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    virtual CCMHome_ptr find_home_by_component_type(const char* _comp_repid);
    virtual CCMHome_ptr find_home_by_home_type(const char* _home_repid);
    virtual CCMHome_ptr find_home_by_name(const char* _home_name);

    friend VISostream& _marshal_in(VISostream& _strm, const HomeFinder_ptr _obj);
    friend VISistream& _marshal_out(VISistream& _strm, HomeFinder_ptr& _obj);
    friend Ostream& _marshal_in(Ostream& _strm, const HomeFinder_ptr _obj);
    friend Istream& _marshal_out(Istream& _strm, HomeFinder_ptr& _obj);
  };

  class HomeFinder_ops;
  typedef HomeFinder_ops* HomeFinder_ops_ptr;

  class  HomeFinder_ops  {
   public:
    HomeFinder_ops() {}
    virtual ~HomeFinder_ops () {}

    virtual CCMHome_ptr find_home_by_component_type(const char* _comp_repid) = 0;
    virtual CCMHome_ptr find_home_by_home_type(const char* _home_repid) = 0;
    virtual CCMHome_ptr find_home_by_name(const char* _home_name) = 0;

    static const VISOps_Info *_desc();
    static HomeFinder_ops_ptr _nil() { return (HomeFinder_ops_ptr)NULL; }
    static HomeFinder_ops_ptr _downcast(PortableServer::ServantBase* _servant);
  protected:
    static const VISOps_Info _ops_info;
  };

}
inline Ostream& operator<<(Ostream& s, const Components::HomeFinder_var& v) {
  return Components::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::HomeNotFound& v) {
  return Components::_marshal_out(s, v);
}
inline Ostream& operator<<(Ostream& s, const Components::HomeFinder_ptr v) {
  return Components::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const Components::HomeFinder_ptr v) {
  return Components::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, Components::HomeFinder_var& v) {
  return Components::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::HomeFinder_var& v) {
  return Components::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, Components::HomeFinder_ptr& v) {
  return Components::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::HomeFinder_ptr& v) {
  return Components::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const Components::HomeFinder_var& v) {
  return Components::_marshal_in(s, v);
}
inline void _pretty_print(VISostream& s, const Components::HomeNotFound& v) {
  Components::_internal_pretty_print(s, v);
}

#include "vpost.h"
#endif // __HomeFinder_idl___client
