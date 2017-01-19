/*
 * Copyright 2001 Borland Software Corporation, Inc.
 *
 * GENERATED CODE --- DO NOT EDIT
 * 
 */

#ifndef __CCMCommon_idl___client
#define __CCMCommon_idl___client
#include "vpre.h"

#ifndef _corba_h_
# include "corba.h"
#endif   // _corba_h_

namespace Components {
  typedef CORBA::ULong FailureReason;
  typedef CORBA::ULong_out FailureReason_out;

  class  InvalidConfiguration : public CORBA_UserException {
   public:

    #if defined(MSVCNEWDLL_BUG)
      void *operator new(size_t ts);
      void *operator new(size_t ts, char*, int) {return operator new(ts);}
      void operator delete(void *p);
    #endif // MSVCNEWDLL_BUG

    InvalidConfiguration();
    virtual ~InvalidConfiguration();

    
    static CORBA::Exception *_factory() { return new InvalidConfiguration(); }
    virtual const CORBA_Exception::Description& _desc() const;
    static InvalidConfiguration* _downcast(CORBA::Exception *_exc);
    static const InvalidConfiguration* _downcast(const CORBA::Exception *_exc);
    CORBA::Exception *_deep_copy() { return new InvalidConfiguration(*this); }
  #ifdef AIXV3
    void _raise() const { VISTHROW_INST((InvalidConfiguration*)this) }
  #else
    void _raise() const { VISTHROW_INST(this) }
  #endif

    friend void _internal_pretty_print(VISostream&, const InvalidConfiguration&);
    void _write(VISostream&) const;
    void _copy(VISistream&);

    friend VISistream& _marshal_out(VISistream& _strm, InvalidConfiguration& _e);
    static const CORBA_Exception::Description _description;
  };

  typedef char* FeatureName;
  typedef CORBA::String_out FeatureName_out;
  typedef CORBA::String_var FeatureName_var;

  class  NameList;
  typedef NameList* NameList_ptr;

  class  NameList : private VISResource {
  private:
    char* *_contents;
    CORBA::ULong _count;
    CORBA::ULong _num_allocated;
    CORBA::Boolean _release_flag;

  public:
    #if defined(MSVCNEWDLL_BUG)
      void *operator new(size_t ts);
      void *operator new(size_t ts, char*, int) {return operator new(ts);}
      void operator delete(void *p);
    #endif // MSVCNEWDLL_BUG

    static char* *allocbuf(CORBA::ULong _nelems);
    static void freebuf(char** _data);
    static void freebuf_elems(char* *_data, CORBA::ULong _nelems);

    class  Sequence_NameList {
      friend class NameList;
       private:
      CORBA::Boolean _release_flag;
      char* &_ptr;

      void release() {
        if (_release_flag) {
          CORBA::string_free(_ptr);
          _ptr = (char *) NULL;
        }
      }

   public:
      Sequence_NameList(char* &_p, CORBA::Boolean _rel)
       : _ptr(_p), _release_flag(_rel) {}

      Sequence_NameList(const Sequence_NameList& _seq)
       : _ptr(_seq._ptr), _release_flag(_seq._release_flag) {}

      Sequence_NameList& operator=(char* _p) {
        if (_ptr != _p) {
          release();
          _ptr = _p;
        }
        return *this;
      }

      Sequence_NameList& operator=(const Sequence_NameList& _p) {
        if (_ptr != _p._ptr) {
          if (_release_flag) {
            release();
            _ptr = CORBA::string_dup(_p._ptr);
          } else
            _ptr = _p._ptr;
        }
        return *this;
      }

      operator const char* () const { return _ptr; }
      operator char* () const { return _ptr; }
    };

    NameList(CORBA::ULong _max = 0);
    NameList(CORBA::ULong max, CORBA::ULong length, char* *_data,
             CORBA::Boolean release = (CORBA::Boolean)0);
    NameList(const NameList&);
    virtual ~NameList();

    NameList& operator=(const NameList&);
    NameList& operator=(const NameList* _obj) {
      assert(_obj);
      return operator=(*_obj);
    }

    CORBA::ULong maximum() const { return _num_allocated; }

    void length(CORBA::ULong _len);
    CORBA::ULong length() const { return _count; }

    void replace(CORBA::ULong _max, CORBA::ULong _len, char* *_data,
                 CORBA::Boolean _release = (CORBA::Boolean)0);

    char* *get_buffer(CORBA::Boolean orphan = (CORBA::Boolean) 0);
    const char* const * get_buffer() const;

    Sequence_NameList operator[](CORBA::ULong _index);
    const Sequence_NameList operator[](CORBA::ULong _index) const;

    static NameList *_duplicate(NameList* _ptr) {
      if (_ptr) _ptr->_ref();
      return _ptr;
    }

    static void _release(NameList *_ptr) {
      if (_ptr && (_ptr->_deref() == 0L)) 
        delete _ptr;
    }

    friend VISostream& _marshal_in(VISostream&, const NameList&);
    friend VISistream& _marshal_out(VISistream&, NameList&);

    friend VISostream& _marshal_in(VISostream& _strm, const NameList* _obj);
    friend VISistream& _marshal_out(VISistream& _strm, NameList_ptr& _obj);

    friend void _internal_pretty_print(VISostream&, const NameList&);

    friend Ostream& _marshal_in(Ostream&, const NameList&);
    friend Istream& _marshal_out(Istream&, NameList&);
    friend Istream& _marshal_out(Istream&, NameList_ptr&);
  };

  class  NameList_out;

  class  NameList_var {
    friend class NameList_out;
     private:
    NameList *_ptr;
     public:
    NameList_var() : _ptr((NameList*)NULL) {}
    NameList_var(NameList *_p) : _ptr(_p) {}
    NameList_var(const NameList_var& _var) :_ptr(NameList::_duplicate(_var._ptr)){}
    virtual ~NameList_var() { NameList::_release(_ptr); }
    NameList_var& operator=(NameList* _p) {
      NameList::_release(_ptr);
      _ptr = _p;
      return *this;
    }

    NameList_var& operator=(const NameList_var& _var) {
      NameList::_release(_ptr);
      _ptr = NameList::_duplicate(_var._ptr);
      return *this;
    }
    operator NameList*() const { return _ptr; }

    NameList* operator->() { return _ptr; }
    const NameList* operator->() const { return _ptr; }

    operator NameList&() { return *_ptr; }
    operator const NameList&() const { return *_ptr; }

    const NameList& in() const { return *_ptr; }
    NameList& inout() { return *_ptr; }
    NameList_ptr & out() {
      NameList::_release(_ptr);
      _ptr = (NameList *)NULL;
      return _ptr;
    }

    NameList* _retn() {
      NameList* _tmp_ptr;
      _tmp_ptr = _ptr;
      _ptr = (NameList *)NULL;
      return _tmp_ptr;
    }
    friend VISostream& _marshal_in(VISostream&, const NameList_var&);
    friend VISistream& _marshal_out(VISistream&, NameList_var&);

    NameList::Sequence_NameList operator[](CORBA::ULong _index);
    const NameList::Sequence_NameList operator[](CORBA::ULong _index) const;

  };
  class  NameList_out {
   private:
    NameList_ptr& _ptr;
    static NameList* _nil() { return (NameList*)NULL; }

    void operator=(const NameList_out&);
    void operator=(const NameList_var&);
     public:
    NameList_out(const NameList_out& _o) : _ptr(_o._ptr) {}
    NameList_out(NameList_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    NameList_out(NameList_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    NameList_out& operator=(NameList* _p) {
      _ptr = _p;
      return *this;
    }
    operator NameList_ptr&() { return _ptr; }
    NameList_ptr& ptr() { return _ptr; }
  };
  class  WrongComponentType : public CORBA_UserException {
   public:

    #if defined(MSVCNEWDLL_BUG)
      void *operator new(size_t ts);
      void *operator new(size_t ts, char*, int) {return operator new(ts);}
      void operator delete(void *p);
    #endif // MSVCNEWDLL_BUG

    WrongComponentType();
    virtual ~WrongComponentType();

    
    static CORBA::Exception *_factory() { return new WrongComponentType(); }
    virtual const CORBA_Exception::Description& _desc() const;
    static WrongComponentType* _downcast(CORBA::Exception *_exc);
    static const WrongComponentType* _downcast(const CORBA::Exception *_exc);
    CORBA::Exception *_deep_copy() { return new WrongComponentType(*this); }
  #ifdef AIXV3
    void _raise() const { VISTHROW_INST((WrongComponentType*)this) }
  #else
    void _raise() const { VISTHROW_INST(this) }
  #endif

    friend void _internal_pretty_print(VISostream&, const WrongComponentType&);
    void _write(VISostream&) const;
    void _copy(VISistream&);

    friend VISistream& _marshal_out(VISistream& _strm, WrongComponentType& _e);
    static const CORBA_Exception::Description _description;
  };

  class  DuplicateKeyValue : public CORBA_UserException {
   public:

    #if defined(MSVCNEWDLL_BUG)
      void *operator new(size_t ts);
      void *operator new(size_t ts, char*, int) {return operator new(ts);}
      void operator delete(void *p);
    #endif // MSVCNEWDLL_BUG

    DuplicateKeyValue();
    virtual ~DuplicateKeyValue();

    
    static CORBA::Exception *_factory() { return new DuplicateKeyValue(); }
    virtual const CORBA_Exception::Description& _desc() const;
    static DuplicateKeyValue* _downcast(CORBA::Exception *_exc);
    static const DuplicateKeyValue* _downcast(const CORBA::Exception *_exc);
    CORBA::Exception *_deep_copy() { return new DuplicateKeyValue(*this); }
  #ifdef AIXV3
    void _raise() const { VISTHROW_INST((DuplicateKeyValue*)this) }
  #else
    void _raise() const { VISTHROW_INST(this) }
  #endif

    friend void _internal_pretty_print(VISostream&, const DuplicateKeyValue&);
    void _write(VISostream&) const;
    void _copy(VISistream&);

    friend VISistream& _marshal_out(VISistream& _strm, DuplicateKeyValue& _e);
    static const CORBA_Exception::Description _description;
  };

  class  InvalidKey : public CORBA_UserException {
   public:

    #if defined(MSVCNEWDLL_BUG)
      void *operator new(size_t ts);
      void *operator new(size_t ts, char*, int) {return operator new(ts);}
      void operator delete(void *p);
    #endif // MSVCNEWDLL_BUG

    InvalidKey();
    virtual ~InvalidKey();

    
    static CORBA::Exception *_factory() { return new InvalidKey(); }
    virtual const CORBA_Exception::Description& _desc() const;
    static InvalidKey* _downcast(CORBA::Exception *_exc);
    static const InvalidKey* _downcast(const CORBA::Exception *_exc);
    CORBA::Exception *_deep_copy() { return new InvalidKey(*this); }
  #ifdef AIXV3
    void _raise() const { VISTHROW_INST((InvalidKey*)this) }
  #else
    void _raise() const { VISTHROW_INST(this) }
  #endif

    friend void _internal_pretty_print(VISostream&, const InvalidKey&);
    void _write(VISostream&) const;
    void _copy(VISistream&);

    friend VISistream& _marshal_out(VISistream& _strm, InvalidKey& _e);
    static const CORBA_Exception::Description _description;
  };

  class  UnknownKeyValue : public CORBA_UserException {
   public:

    #if defined(MSVCNEWDLL_BUG)
      void *operator new(size_t ts);
      void *operator new(size_t ts, char*, int) {return operator new(ts);}
      void operator delete(void *p);
    #endif // MSVCNEWDLL_BUG

    UnknownKeyValue();
    virtual ~UnknownKeyValue();

    
    static CORBA::Exception *_factory() { return new UnknownKeyValue(); }
    virtual const CORBA_Exception::Description& _desc() const;
    static UnknownKeyValue* _downcast(CORBA::Exception *_exc);
    static const UnknownKeyValue* _downcast(const CORBA::Exception *_exc);
    CORBA::Exception *_deep_copy() { return new UnknownKeyValue(*this); }
  #ifdef AIXV3
    void _raise() const { VISTHROW_INST((UnknownKeyValue*)this) }
  #else
    void _raise() const { VISTHROW_INST(this) }
  #endif

    friend void _internal_pretty_print(VISostream&, const UnknownKeyValue&);
    void _write(VISostream&) const;
    void _copy(VISistream&);

    friend VISistream& _marshal_out(VISistream& _strm, UnknownKeyValue& _e);
    static const CORBA_Exception::Description _description;
  };

  class  FinderFailure : public CORBA_UserException {
   public:

    #if defined(MSVCNEWDLL_BUG)
      void *operator new(size_t ts);
      void *operator new(size_t ts, char*, int) {return operator new(ts);}
      void operator delete(void *p);
    #endif // MSVCNEWDLL_BUG

    FinderFailure();
    FinderFailure( CORBA::ULong _reason );
    virtual ~FinderFailure();

    CORBA::ULong reason;
    static CORBA::Exception *_factory() { return new FinderFailure(); }
    virtual const CORBA_Exception::Description& _desc() const;
    static FinderFailure* _downcast(CORBA::Exception *_exc);
    static const FinderFailure* _downcast(const CORBA::Exception *_exc);
    CORBA::Exception *_deep_copy() { return new FinderFailure(*this); }
  #ifdef AIXV3
    void _raise() const { VISTHROW_INST((FinderFailure*)this) }
  #else
    void _raise() const { VISTHROW_INST(this) }
  #endif

    friend void _internal_pretty_print(VISostream&, const FinderFailure&);
    void _write(VISostream&) const;
    void _copy(VISistream&);

    friend VISistream& _marshal_out(VISistream& _strm, FinderFailure& _e);
    static const CORBA_Exception::Description _description;
  };

  #ifndef Components_PrimaryKeyBase_var_
  #define Components_PrimaryKeyBase_var_

  class  PrimaryKeyBase;

  typedef PrimaryKeyBase* PrimaryKeyBase_ptr;

   VISistream& _marshal_out(VISistream&, PrimaryKeyBase_ptr&);
   VISostream& _marshal_in(VISostream&, const PrimaryKeyBase_ptr);

  class  PrimaryKeyBase_out;
  class  PrimaryKeyBase_var: public CORBA::_var {
    friend class PrimaryKeyBase_out;
     public:
    PrimaryKeyBase_var();
    PrimaryKeyBase_var(PrimaryKeyBase_ptr );
    PrimaryKeyBase_var(const PrimaryKeyBase_var &);

    virtual ~PrimaryKeyBase_var();

    PrimaryKeyBase_var& operator=(PrimaryKeyBase_ptr);
    void operator=(const PrimaryKeyBase_var & _v);

    static void _add_ref(PrimaryKeyBase_ptr );
    static void _remove_ref(PrimaryKeyBase_ptr );

    operator PrimaryKeyBase*() const { return _ptr; }
    PrimaryKeyBase* operator->() { return _ptr; }
    const PrimaryKeyBase* operator->() const { return _ptr; }

    PrimaryKeyBase_ptr in () const { return _ptr; }
    PrimaryKeyBase_ptr& inout () { return _ptr; }
    PrimaryKeyBase_ptr& out();
    PrimaryKeyBase_ptr _retn();

    friend VISistream& _marshal_out(VISistream& _strm, PrimaryKeyBase_var& _var);
    friend VISostream& _marshal_in(VISostream& _strm, const PrimaryKeyBase_var& _var);

    friend Istream& _marshal_out(Istream&, PrimaryKeyBase_var&);
    friend Ostream& _marshal_in(Ostream&, const PrimaryKeyBase_var&);
     private:
    PrimaryKeyBase_ptr _ptr;
  };
  class  PrimaryKeyBase_out {
   private:
    PrimaryKeyBase_ptr& _ptr;
    static PrimaryKeyBase* _nil() { return (PrimaryKeyBase*)NULL; }

    void operator=(const PrimaryKeyBase_out&);
    void operator=(const PrimaryKeyBase_var&);
     public:
    PrimaryKeyBase_out(const PrimaryKeyBase_out& _o) : _ptr(_o._ptr) {}
    PrimaryKeyBase_out(PrimaryKeyBase_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    PrimaryKeyBase_out(PrimaryKeyBase_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    PrimaryKeyBase_out& operator=(PrimaryKeyBase* _p) {
      _ptr = _p;
      return *this;
    }
    operator PrimaryKeyBase_ptr&() { return _ptr; }
    PrimaryKeyBase_ptr& ptr() { return _ptr; }
    PrimaryKeyBase* operator->() { return _ptr; }
  };

  #endif	// Components_PrimaryKeyBase_var_
  // idl valuetype: Components::PrimaryKeyBase
  class  PrimaryKeyBase  : public virtual CORBA::ValueBase  {
    public:
      friend VISostream& _marshal_in(VISostream& _strm, const PrimaryKeyBase_ptr _obj);
      friend VISistream& _marshal_out(VISistream& _strm, PrimaryKeyBase_ptr& _obj);
      friend Ostream& _marshal_in(Ostream& _strm, const PrimaryKeyBase_ptr _obj);
      friend Istream& _marshal_out(Istream& _strm, PrimaryKeyBase_ptr& _obj);
      friend void _internal_pretty_print(VISostream& _strm, const PrimaryKeyBase& _obj);
      friend void _internal_pretty_print(VISostream& _strm, const PrimaryKeyBase* _obj);

      static PrimaryKeyBase* _downcast(CORBA::ValueBase* vbptr);
      static const VISValueInfo& _stat_info;
      static const VISValueInfo& _info();

      virtual const VISValueInfo& _type_info() const;
      virtual void* _safe_downcast(const VISValueInfo&) const;

      #if defined(MSVCNEWDLL_BUG)
        void *operator new(size_t ts);
        void *operator new(size_t ts, char*, int) {return operator new(ts);}
        void operator delete(void *p);
      #endif // MSVCNEWDLL_BUG

      virtual void _write_state(VISostream& _ostrm);
      virtual void _read_state(VISistream& _strm);
      protected:
       PrimaryKeyBase() {}
        virtual ~PrimaryKeyBase() {}

      private:
        void operator=(const PrimaryKeyBase&);
  };
  class PrimaryKeyBase_init : public CORBA::ValueFactoryBase {
  };
  #ifndef Components_Cookie_var_
  #define Components_Cookie_var_

  class  Cookie;

  typedef Cookie* Cookie_ptr;

   VISistream& _marshal_out(VISistream&, Cookie_ptr&);
   VISostream& _marshal_in(VISostream&, const Cookie_ptr);

  class  Cookie_out;
  class  Cookie_var: public CORBA::_var {
    friend class Cookie_out;
     public:
    Cookie_var();
    Cookie_var(Cookie_ptr );
    Cookie_var(const Cookie_var &);

    virtual ~Cookie_var();

    Cookie_var& operator=(Cookie_ptr);
    void operator=(const Cookie_var & _v);

    static void _add_ref(Cookie_ptr );
    static void _remove_ref(Cookie_ptr );

    operator Cookie*() const { return _ptr; }
    Cookie* operator->() { return _ptr; }
    const Cookie* operator->() const { return _ptr; }

    Cookie_ptr in () const { return _ptr; }
    Cookie_ptr& inout () { return _ptr; }
    Cookie_ptr& out();
    Cookie_ptr _retn();

    friend VISistream& _marshal_out(VISistream& _strm, Cookie_var& _var);
    friend VISostream& _marshal_in(VISostream& _strm, const Cookie_var& _var);

    friend Istream& _marshal_out(Istream&, Cookie_var&);
    friend Ostream& _marshal_in(Ostream&, const Cookie_var&);
     private:
    Cookie_ptr _ptr;
  };
  class  Cookie_out {
   private:
    Cookie_ptr& _ptr;
    static Cookie* _nil() { return (Cookie*)NULL; }

    void operator=(const Cookie_out&);
    void operator=(const Cookie_var&);
     public:
    Cookie_out(const Cookie_out& _o) : _ptr(_o._ptr) {}
    Cookie_out(Cookie_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    Cookie_out(Cookie_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    Cookie_out& operator=(Cookie* _p) {
      _ptr = _p;
      return *this;
    }
    operator Cookie_ptr&() { return _ptr; }
    Cookie_ptr& ptr() { return _ptr; }
    Cookie* operator->() { return _ptr; }
  };

  #endif	// Components_Cookie_var_
  // idl valuetype: Components::Cookie
  class  Cookie  : public virtual CORBA::ValueBase  {
    public:
      friend VISostream& _marshal_in(VISostream& _strm, const Cookie_ptr _obj);
      friend VISistream& _marshal_out(VISistream& _strm, Cookie_ptr& _obj);
      friend Ostream& _marshal_in(Ostream& _strm, const Cookie_ptr _obj);
      friend Istream& _marshal_out(Istream& _strm, Cookie_ptr& _obj);
      friend void _internal_pretty_print(VISostream& _strm, const Cookie& _obj);
      friend void _internal_pretty_print(VISostream& _strm, const Cookie* _obj);

      static Cookie* _downcast(CORBA::ValueBase* vbptr);
      static const VISValueInfo& _stat_info;
      static const VISValueInfo& _info();

      virtual const VISValueInfo& _type_info() const;
      virtual void* _safe_downcast(const VISValueInfo&) const;

      #if defined(MSVCNEWDLL_BUG)
        void *operator new(size_t ts);
        void *operator new(size_t ts, char*, int) {return operator new(ts);}
        void operator delete(void *p);
      #endif // MSVCNEWDLL_BUG

      virtual void _write_state(VISostream& _ostrm);
      virtual void _read_state(VISistream& _strm);
      protected:
       Cookie() {}
        virtual ~Cookie() {}
      virtual void cookieValue(const ::CORBA::OctetSequence& _cookieValue) = 0;
      virtual const ::CORBA::OctetSequence& cookieValue() const = 0;
      virtual ::CORBA::OctetSequence& cookieValue() = 0;

      private:
        void operator=(const Cookie&);
  };
  class  OBV_Cookie: public virtual Cookie  {
    public:
    protected:
      OBV_Cookie();
      virtual ~OBV_Cookie();
      virtual void cookieValue(const ::CORBA::OctetSequence& _cookieValue) {
        _obv_cookieValue = _cookieValue;
      }
      virtual const ::CORBA::OctetSequence& cookieValue() const {
        return _obv_cookieValue;
      }
      virtual ::CORBA::OctetSequence& cookieValue() {
        return _obv_cookieValue;
      }

      OBV_Cookie(const ::CORBA::OctetSequence& _cookieValue) {
                _obv_cookieValue = _cookieValue;
  
      }
      ::CORBA::OctetSequence _obv_cookieValue;
  };
  class Cookie_init : public CORBA::ValueFactoryBase {
  };
  enum CCMExceptionReason {
     SYSTEM_ERROR, 
     CREATE_ERROR, 
     REMOVE_ERROR, 
     DUPLICATE_KEY, 
     FIND_ERROR, 
     OBJECT_NOT_FOUND, 
     NO_SUCH_ENTITY
  };

  typedef CCMExceptionReason& CCMExceptionReason_out;

   VISostream& _marshal_in(VISostream& _strm, const CCMExceptionReason _enum_type);
   VISistream& _marshal_out(VISistream& _strm, CCMExceptionReason& _enum_type);

   Ostream& _marshal_in(Ostream&, const CCMExceptionReason&);
   Istream& _marshal_out(Istream&, CCMExceptionReason&);

   void _internal_pretty_print(VISostream& _strm, CCMExceptionReason _val);

  class  CCMException : public CORBA_UserException {
   public:

    #if defined(MSVCNEWDLL_BUG)
      void *operator new(size_t ts);
      void *operator new(size_t ts, char*, int) {return operator new(ts);}
      void operator delete(void *p);
    #endif // MSVCNEWDLL_BUG

    CCMException();
    CCMException( CCMExceptionReason _reason );
    virtual ~CCMException();

    CCMExceptionReason reason;
    static CORBA::Exception *_factory() { return new CCMException(); }
    virtual const CORBA_Exception::Description& _desc() const;
    static CCMException* _downcast(CORBA::Exception *_exc);
    static const CCMException* _downcast(const CORBA::Exception *_exc);
    CORBA::Exception *_deep_copy() { return new CCMException(*this); }
  #ifdef AIXV3
    void _raise() const { VISTHROW_INST((CCMException*)this) }
  #else
    void _raise() const { VISTHROW_INST(this) }
  #endif

    friend void _internal_pretty_print(VISostream&, const CCMException&);
    void _write(VISostream&) const;
    void _copy(VISistream&);

    friend VISistream& _marshal_out(VISistream& _strm, CCMException& _e);
    static const CORBA_Exception::Description _description;
  };

}
inline Ostream& operator<<(Ostream& s, const Components::PrimaryKeyBase_ptr v) {
  return Components::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::InvalidKey& v) {
  return Components::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const Components::Cookie_ptr v) {
  return Components::_marshal_in(s, v);
}
inline Ostream& operator<<(Ostream& s, const Components::NameList& v) {
  return Components::_marshal_in(s, v);
}
inline void _pretty_print(VISostream& s, const Components::PrimaryKeyBase& v) {
  Components::_internal_pretty_print(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::WrongComponentType& v) {
  return Components::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, Components::NameList& v) {
  return Components::_marshal_out(s, v);
}
inline void _pretty_print(VISostream& s, Components::CCMExceptionReason v) {
  Components::_internal_pretty_print(s, v);
}
inline void _pretty_print(VISostream& s, const Components::PrimaryKeyBase* v) {
  Components::_internal_pretty_print(s, v);
}
inline Istream& operator>>(Istream& s, Components::CCMExceptionReason& v) {
  return Components::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, Components::NameList_ptr& v) {
  return Components::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::Cookie_ptr& v) {
  return Components::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, Components::PrimaryKeyBase_ptr& v) {
  return Components::_marshal_out(s, v);
}
inline void _pretty_print(VISostream& s, const Components::InvalidKey& v) {
  Components::_internal_pretty_print(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::Cookie_var& v) {
  return Components::_marshal_out(s, v);
}
inline void _pretty_print(VISostream& s, const Components::Cookie& v) {
  Components::_internal_pretty_print(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::CCMExceptionReason& v) {
  return Components::_marshal_out(s, v);
}
inline void _pretty_print(VISostream& s, const Components::WrongComponentType& v) {
  Components::_internal_pretty_print(s, v);
}
inline VISostream& operator<<(VISostream& s, const Components::PrimaryKeyBase_ptr v) {
  return Components::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, Components::PrimaryKeyBase_var& v) {
  return Components::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::PrimaryKeyBase_ptr& v) {
  return Components::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::UnknownKeyValue& v) {
  return Components::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const Components::NameList_var& v) {
  return Components::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const Components::CCMExceptionReason v) {
  return Components::_marshal_in(s, v);
}
inline void _pretty_print(VISostream& s, const Components::Cookie* v) {
  Components::_internal_pretty_print(s, v);
}
inline Ostream& operator<<(Ostream& s, const Components::Cookie_var& v) {
  return Components::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::PrimaryKeyBase_var& v) {
  return Components::_marshal_out(s, v);
}
inline Ostream& operator<<(Ostream& s, const Components::CCMExceptionReason& v) {
  return Components::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, Components::Cookie_ptr& v) {
  return Components::_marshal_out(s, v);
}
inline void _pretty_print(VISostream& s, const Components::DuplicateKeyValue& v) {
  Components::_internal_pretty_print(s, v);
}
inline void _pretty_print(VISostream& s, const Components::UnknownKeyValue& v) {
  Components::_internal_pretty_print(s, v);
}
inline void _pretty_print(VISostream& s, const Components::NameList& v) {
  Components::_internal_pretty_print(s, v);
}
inline void _pretty_print(VISostream& s, const Components::FinderFailure& v) {
  Components::_internal_pretty_print(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::DuplicateKeyValue& v) {
  return Components::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const Components::Cookie_var& v) {
  return Components::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, Components::Cookie_var& v) {
  return Components::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::CCMException& v) {
  return Components::_marshal_out(s, v);
}
inline Ostream& operator<<(Ostream& s, const Components::PrimaryKeyBase_var& v) {
  return Components::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::InvalidConfiguration& v) {
  return Components::_marshal_out(s, v);
}
inline void _pretty_print(VISostream& s, const Components::InvalidConfiguration& v) {
  Components::_internal_pretty_print(s, v);
}
inline Ostream& operator<<(Ostream& s, const Components::Cookie_ptr v) {
  return Components::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::NameList& v) {
  return Components::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const Components::NameList& v) {
  return Components::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::NameList_ptr& v) {
  return Components::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const Components::PrimaryKeyBase_var& v) {
  return Components::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::FinderFailure& v) {
  return Components::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const Components::NameList* v) {
  return Components::_marshal_in(s, v);
}
inline void _pretty_print(VISostream& s, const Components::CCMException& v) {
  Components::_internal_pretty_print(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::NameList_var& v) {
  return Components::_marshal_out(s, v);
}

#include "vpost.h"
#endif // __CCMCommon_idl___client
