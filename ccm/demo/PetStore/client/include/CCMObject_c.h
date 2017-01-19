/*
 * Copyright 2001 Borland Software Corporation, Inc.
 *
 * GENERATED CODE --- DO NOT EDIT
 * 
 */

#ifndef __CCMObject_idl___client
#define __CCMObject_idl___client
#include "vpre.h"

#ifndef _corba_h_
# include "corba.h"
#endif   // _corba_h_
#ifndef __CCMHome_idl___client
# include "CCMHome_c.h"
#endif
namespace Components {
  class  InvalidName : public CORBA_UserException {
   public:

    #if defined(MSVCNEWDLL_BUG)
      void *operator new(size_t ts);
      void *operator new(size_t ts, char*, int) {return operator new(ts);}
      void operator delete(void *p);
    #endif // MSVCNEWDLL_BUG

    InvalidName();
    virtual ~InvalidName();

    
    static CORBA::Exception *_factory() { return new InvalidName(); }
    virtual const CORBA_Exception::Description& _desc() const;
    static InvalidName* _downcast(CORBA::Exception *_exc);
    static const InvalidName* _downcast(const CORBA::Exception *_exc);
    CORBA::Exception *_deep_copy() { return new InvalidName(*this); }
  #ifdef AIXV3
    void _raise() const { VISTHROW_INST((InvalidName*)this) }
  #else
    void _raise() const { VISTHROW_INST(this) }
  #endif

    friend void _internal_pretty_print(VISostream&, const InvalidName&);
    void _write(VISostream&) const;
    void _copy(VISistream&);

    friend VISistream& _marshal_out(VISistream& _strm, InvalidName& _e);
    static const CORBA_Exception::Description _description;
  };

  #ifndef Components_PortDescription_var_
  #define Components_PortDescription_var_

  class  PortDescription;

  typedef PortDescription* PortDescription_ptr;

   VISistream& _marshal_out(VISistream&, PortDescription_ptr&);
   VISostream& _marshal_in(VISostream&, const PortDescription_ptr);

  class  PortDescription_out;
  class  PortDescription_var: public CORBA::_var {
    friend class PortDescription_out;
     public:
    PortDescription_var();
    PortDescription_var(PortDescription_ptr );
    PortDescription_var(const PortDescription_var &);

    virtual ~PortDescription_var();

    PortDescription_var& operator=(PortDescription_ptr);
    void operator=(const PortDescription_var & _v);

    static void _add_ref(PortDescription_ptr );
    static void _remove_ref(PortDescription_ptr );

    operator PortDescription*() const { return _ptr; }
    PortDescription* operator->() { return _ptr; }
    const PortDescription* operator->() const { return _ptr; }

    PortDescription_ptr in () const { return _ptr; }
    PortDescription_ptr& inout () { return _ptr; }
    PortDescription_ptr& out();
    PortDescription_ptr _retn();

    friend VISistream& _marshal_out(VISistream& _strm, PortDescription_var& _var);
    friend VISostream& _marshal_in(VISostream& _strm, const PortDescription_var& _var);

    friend Istream& _marshal_out(Istream&, PortDescription_var&);
    friend Ostream& _marshal_in(Ostream&, const PortDescription_var&);
     private:
    PortDescription_ptr _ptr;
  };
  class  PortDescription_out {
   private:
    PortDescription_ptr& _ptr;
    static PortDescription* _nil() { return (PortDescription*)NULL; }

    void operator=(const PortDescription_out&);
    void operator=(const PortDescription_var&);
     public:
    PortDescription_out(const PortDescription_out& _o) : _ptr(_o._ptr) {}
    PortDescription_out(PortDescription_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    PortDescription_out(PortDescription_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    PortDescription_out& operator=(PortDescription* _p) {
      _ptr = _p;
      return *this;
    }
    operator PortDescription_ptr&() { return _ptr; }
    PortDescription_ptr& ptr() { return _ptr; }
    PortDescription* operator->() { return _ptr; }
  };

  #endif	// Components_PortDescription_var_
  // idl valuetype: Components::PortDescription
  class  PortDescription  : public virtual CORBA::ValueBase  {
    public:
      friend VISostream& _marshal_in(VISostream& _strm, const PortDescription_ptr _obj);
      friend VISistream& _marshal_out(VISistream& _strm, PortDescription_ptr& _obj);
      friend Ostream& _marshal_in(Ostream& _strm, const PortDescription_ptr _obj);
      friend Istream& _marshal_out(Istream& _strm, PortDescription_ptr& _obj);
      friend void _internal_pretty_print(VISostream& _strm, const PortDescription& _obj);
      friend void _internal_pretty_print(VISostream& _strm, const PortDescription* _obj);

      static PortDescription* _downcast(CORBA::ValueBase* vbptr);
      virtual void name(char* _name) = 0;
      virtual void name(const char* _name) = 0;
      virtual void name(const CORBA::String_var& _name) = 0;
      virtual const char* name() const = 0;
      virtual void type_id(char* _type_id) = 0;
      virtual void type_id(const char* _type_id) = 0;
      virtual void type_id(const CORBA::String_var& _type_id) = 0;
      virtual const char* type_id() const = 0;
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
       PortDescription() {}
        virtual ~PortDescription() {}

      private:
        void operator=(const PortDescription&);
  };
  class  OBV_PortDescription: public virtual PortDescription  {
    public:
      virtual void name(char* _name) {
        _obv_name = _name;
      }

      virtual void name(const char* _name) {
        _obv_name = CORBA::string_dup(_name);
      }

      virtual void name(const CORBA::String_var& _name) {
        _obv_name = CORBA::string_dup(_name);
      }

      virtual const char* name() const {
        return _obv_name;
      }
      virtual void type_id(char* _type_id) {
        _obv_type_id = _type_id;
      }

      virtual void type_id(const char* _type_id) {
        _obv_type_id = CORBA::string_dup(_type_id);
      }

      virtual void type_id(const CORBA::String_var& _type_id) {
        _obv_type_id = CORBA::string_dup(_type_id);
      }

      virtual const char* type_id() const {
        return _obv_type_id;
      }
    protected:
      OBV_PortDescription();
      virtual ~OBV_PortDescription();

      OBV_PortDescription(const char* _name, 
                          const char* _type_id) {
                _obv_name = CORBA::string_dup(_name);
 
                _obv_type_id = CORBA::string_dup(_type_id);
  
      }
      CORBA::String_var _obv_name;
      CORBA::String_var _obv_type_id;
  };
  class PortDescription_init : public CORBA::ValueFactoryBase {
  };
  #ifndef Components_FacetDescription_var_
  #define Components_FacetDescription_var_

  class  FacetDescription;

  typedef FacetDescription* FacetDescription_ptr;

   VISistream& _marshal_out(VISistream&, FacetDescription_ptr&);
   VISostream& _marshal_in(VISostream&, const FacetDescription_ptr);

  class  FacetDescription_out;
  class  FacetDescription_var: public CORBA::_var {
    friend class FacetDescription_out;
     public:
    FacetDescription_var();
    FacetDescription_var(FacetDescription_ptr );
    FacetDescription_var(const FacetDescription_var &);

    virtual ~FacetDescription_var();

    FacetDescription_var& operator=(FacetDescription_ptr);
    void operator=(const FacetDescription_var & _v);

    static void _add_ref(FacetDescription_ptr );
    static void _remove_ref(FacetDescription_ptr );

    operator FacetDescription*() const { return _ptr; }
    FacetDescription* operator->() { return _ptr; }
    const FacetDescription* operator->() const { return _ptr; }

    FacetDescription_ptr in () const { return _ptr; }
    FacetDescription_ptr& inout () { return _ptr; }
    FacetDescription_ptr& out();
    FacetDescription_ptr _retn();

    friend VISistream& _marshal_out(VISistream& _strm, FacetDescription_var& _var);
    friend VISostream& _marshal_in(VISostream& _strm, const FacetDescription_var& _var);

    friend Istream& _marshal_out(Istream&, FacetDescription_var&);
    friend Ostream& _marshal_in(Ostream&, const FacetDescription_var&);
     private:
    FacetDescription_ptr _ptr;
  };
  class  FacetDescription_out {
   private:
    FacetDescription_ptr& _ptr;
    static FacetDescription* _nil() { return (FacetDescription*)NULL; }

    void operator=(const FacetDescription_out&);
    void operator=(const FacetDescription_var&);
     public:
    FacetDescription_out(const FacetDescription_out& _o) : _ptr(_o._ptr) {}
    FacetDescription_out(FacetDescription_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    FacetDescription_out(FacetDescription_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    FacetDescription_out& operator=(FacetDescription* _p) {
      _ptr = _p;
      return *this;
    }
    operator FacetDescription_ptr&() { return _ptr; }
    FacetDescription_ptr& ptr() { return _ptr; }
    FacetDescription* operator->() { return _ptr; }
  };

  #endif	// Components_FacetDescription_var_
  // idl valuetype: Components::FacetDescription
  class  FacetDescription  : public virtual PortDescription  {
    public:
      friend VISostream& _marshal_in(VISostream& _strm, const FacetDescription_ptr _obj);
      friend VISistream& _marshal_out(VISistream& _strm, FacetDescription_ptr& _obj);
      friend Ostream& _marshal_in(Ostream& _strm, const FacetDescription_ptr _obj);
      friend Istream& _marshal_out(Istream& _strm, FacetDescription_ptr& _obj);
      friend void _internal_pretty_print(VISostream& _strm, const FacetDescription& _obj);
      friend void _internal_pretty_print(VISostream& _strm, const FacetDescription* _obj);

      static FacetDescription* _downcast(CORBA::ValueBase* vbptr);
      virtual void facet_ref(const CORBA::Object_ptr _facet_ref) = 0;
      virtual const CORBA::Object_ptr facet_ref() const = 0;
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
       FacetDescription() {}
        virtual ~FacetDescription() {}

      private:
        void operator=(const FacetDescription&);
  };
  class  OBV_FacetDescription: public virtual FacetDescription,
                               public virtual OBV_PortDescription  {
    public:
      virtual void facet_ref(const CORBA::Object_ptr _facet_ref) {
        _obv_facet_ref = CORBA::Object::_duplicate(_facet_ref);
      }
      virtual const CORBA::Object_ptr facet_ref() const {
        return _obv_facet_ref;
      }
    protected:
      OBV_FacetDescription();
      virtual ~OBV_FacetDescription();

      OBV_FacetDescription(const CORBA::Object_ptr _facet_ref) {
                _obv_facet_ref = CORBA::Object::_duplicate(_facet_ref);
  
      }
      CORBA::Object_var _obv_facet_ref;
  };
  class FacetDescription_init : public CORBA::ValueFactoryBase {
  };
  class  FacetDescriptions;
  typedef FacetDescriptions* FacetDescriptions_ptr;

  class  FacetDescriptions : private VISResource {
  private:
    FacetDescription_ptr *_contents;
    CORBA::ULong _count;
    CORBA::ULong _num_allocated;
    CORBA::Boolean _release_flag;

  public:
    #if defined(MSVCNEWDLL_BUG)
      void *operator new(size_t ts);
      void *operator new(size_t ts, char*, int) {return operator new(ts);}
      void operator delete(void *p);
    #endif // MSVCNEWDLL_BUG

    static FacetDescription_ptr *allocbuf(CORBA::ULong _nelems);
    static void freebuf(FacetDescription_ptr* _data);
    static void freebuf_elems(FacetDescription_ptr *_data, CORBA::ULong _nelems);

    class  Sequence_FacetDescriptions {
      friend class FacetDescriptions;
       private:
      CORBA::Boolean _release_flag;
      FacetDescription_ptr &_ptr;

      void release() {
        if (_release_flag) {
          CORBA::remove_ref(_ptr);
          _ptr = (Components::FacetDescription *)NULL;
        }
      }

   public:
      Sequence_FacetDescriptions(FacetDescription_ptr &_p, CORBA::Boolean _rel)
       : _ptr(_p), _release_flag(_rel) {}

      Sequence_FacetDescriptions(const Sequence_FacetDescriptions& _seq)
       : _ptr(_seq._ptr), _release_flag(_seq._release_flag) {}

      Sequence_FacetDescriptions& operator=(FacetDescription_ptr _p) {
        if (_ptr != _p) {
          release();
          _ptr = _p;
        }
        return *this;
      }

      Sequence_FacetDescriptions& operator=(const Sequence_FacetDescriptions& _p) {
        if (_ptr != _p._ptr) {
          if (_release_flag) {
            release();
            ::Components::FacetDescription_var::_add_ref(_p._ptr);
            _ptr = _p._ptr;
          } else
            _ptr = _p._ptr;
        }
        return *this;
      }

      operator FacetDescription_ptr () const { return _ptr; }
      FacetDescription_ptr operator ->() const { return _ptr; }
    };

    FacetDescriptions(CORBA::ULong _max = 0);
    FacetDescriptions(CORBA::ULong max, CORBA::ULong length,
                      FacetDescription_ptr *_data,
                      CORBA::Boolean release = (CORBA::Boolean)0);
    FacetDescriptions(const FacetDescriptions&);
    virtual ~FacetDescriptions();

    FacetDescriptions& operator=(const FacetDescriptions&);
    FacetDescriptions& operator=(const FacetDescriptions* _obj) {
      assert(_obj);
      return operator=(*_obj);
    }

    CORBA::ULong maximum() const { return _num_allocated; }

    void length(CORBA::ULong _len);
    CORBA::ULong length() const { return _count; }

    void replace(CORBA::ULong _max, CORBA::ULong _len,
                 FacetDescription_ptr *_data,
                 CORBA::Boolean _release = (CORBA::Boolean)0);

    FacetDescription_ptr *get_buffer(CORBA::Boolean orphan = (CORBA::Boolean) 0);
    const FacetDescription_ptr* get_buffer() const;

    Sequence_FacetDescriptions operator[](CORBA::ULong _index);
    const Sequence_FacetDescriptions operator[](CORBA::ULong _index) const;

    static FacetDescriptions *_duplicate(FacetDescriptions* _ptr) {
      if (_ptr) _ptr->_ref();
      return _ptr;
    }

    static void _release(FacetDescriptions *_ptr) {
      if (_ptr && (_ptr->_deref() == 0L)) 
        delete _ptr;
    }

    friend VISostream& _marshal_in(VISostream&, const FacetDescriptions&);
    friend VISistream& _marshal_out(VISistream&, FacetDescriptions&);

    friend VISostream& _marshal_in(VISostream& _strm, const FacetDescriptions* _obj);
    friend VISistream& _marshal_out(VISistream& _strm, FacetDescriptions_ptr& _obj);

    friend void _internal_pretty_print(VISostream&, const FacetDescriptions&);

    friend Ostream& _marshal_in(Ostream&, const FacetDescriptions&);
    friend Istream& _marshal_out(Istream&, FacetDescriptions&);
    friend Istream& _marshal_out(Istream&, FacetDescriptions_ptr&);
  };

  class  FacetDescriptions_out;

  class  FacetDescriptions_var {
    friend class FacetDescriptions_out;
     private:
    FacetDescriptions *_ptr;
     public:
    FacetDescriptions_var() : _ptr((FacetDescriptions*)NULL) {}
    FacetDescriptions_var(FacetDescriptions *_p) : _ptr(_p) {}
    FacetDescriptions_var(const FacetDescriptions_var& _var) :_ptr(FacetDescriptions::_duplicate(_var._ptr)){}
    virtual ~FacetDescriptions_var() { FacetDescriptions::_release(_ptr); }
    FacetDescriptions_var& operator=(FacetDescriptions* _p) {
      FacetDescriptions::_release(_ptr);
      _ptr = _p;
      return *this;
    }

    FacetDescriptions_var& operator=(const FacetDescriptions_var& _var) {
      FacetDescriptions::_release(_ptr);
      _ptr = FacetDescriptions::_duplicate(_var._ptr);
      return *this;
    }
    operator FacetDescriptions*() const { return _ptr; }

    FacetDescriptions* operator->() { return _ptr; }
    const FacetDescriptions* operator->() const { return _ptr; }

    operator FacetDescriptions&() { return *_ptr; }
    operator const FacetDescriptions&() const { return *_ptr; }

    const FacetDescriptions& in() const { return *_ptr; }
    FacetDescriptions& inout() { return *_ptr; }
    FacetDescriptions_ptr & out() {
      FacetDescriptions::_release(_ptr);
      _ptr = (FacetDescriptions *)NULL;
      return _ptr;
    }

    FacetDescriptions* _retn() {
      FacetDescriptions* _tmp_ptr;
      _tmp_ptr = _ptr;
      _ptr = (FacetDescriptions *)NULL;
      return _tmp_ptr;
    }
    friend VISostream& _marshal_in(VISostream&, const FacetDescriptions_var&);
    friend VISistream& _marshal_out(VISistream&, FacetDescriptions_var&);

    FacetDescriptions::Sequence_FacetDescriptions operator[](CORBA::ULong _index);
    const FacetDescriptions::Sequence_FacetDescriptions operator[](CORBA::ULong _index) const;

  };
  class  FacetDescriptions_out {
   private:
    FacetDescriptions_ptr& _ptr;
    static FacetDescriptions* _nil() { return (FacetDescriptions*)NULL; }

    void operator=(const FacetDescriptions_out&);
    void operator=(const FacetDescriptions_var&);
     public:
    FacetDescriptions_out(const FacetDescriptions_out& _o) : _ptr(_o._ptr) {}
    FacetDescriptions_out(FacetDescriptions_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    FacetDescriptions_out(FacetDescriptions_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    FacetDescriptions_out& operator=(FacetDescriptions* _p) {
      _ptr = _p;
      return *this;
    }
    operator FacetDescriptions_ptr&() { return _ptr; }
    FacetDescriptions_ptr& ptr() { return _ptr; }
  };
  #ifndef Components_Navigation_var_
  #define Components_Navigation_var_

  class  Navigation;

  typedef Navigation* Navigation_ptr;
  typedef Navigation_ptr NavigationRef;
   VISistream& _marshal_out(VISistream&, Navigation_ptr&);
   VISostream& _marshal_in(VISostream&, const Navigation_ptr);
  class  Navigation_out;

  class  Navigation_var : public CORBA::_var {
  friend class Navigation_out;
   private:
    Navigation_ptr _ptr;

   public:
    Navigation_var();
    Navigation_var(Navigation_ptr);
    Navigation_var(const Navigation_var &);
    virtual ~Navigation_var();

    static Navigation_ptr _duplicate(Navigation_ptr);
    static void _release(Navigation_ptr);

    Navigation_var& operator=(const Navigation_var& _var);
    Navigation_var& operator=(Navigation_ptr);

    operator Navigation*() const { return _ptr; }
    Navigation* operator->() const { return _ptr; }

    Navigation_ptr in() const { return _ptr; }
    Navigation_ptr& inout() { return _ptr; }
    Navigation_ptr& out();
    Navigation_ptr _retn();

    friend VISostream& _marshal_in(VISostream&, const Navigation_var&);
    friend VISistream& _marshal_out(VISistream&, Navigation_var&);

    friend Istream& _marshal_out(Istream&, Navigation_var&);
    friend Ostream& _marshal_in(Ostream&, const Navigation_var&);
  };
  class  Navigation_out {
   private:
    Navigation_ptr& _ptr;
    static Navigation* _nil() { return (Navigation*)NULL; }

    void operator=(const Navigation_out&);
    void operator=(const Navigation_var&);
     public:
    Navigation_out(const Navigation_out& _o) : _ptr(_o._ptr) {}
    Navigation_out(Navigation_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    Navigation_out(Navigation_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    Navigation_out& operator=(Navigation* _p) {
      _ptr = _p;
      return *this;
    }
    operator Navigation_ptr&() { return _ptr; }
    Navigation_ptr& ptr() { return _ptr; }
    Navigation* operator->() { return _ptr; }
  };

  #endif // Components_Navigation_var_

  // idl interface: Components::Navigation
  class  Navigation : public virtual CORBA_Object {
   private:
    static const CORBA::TypeInfo _class_info;
    void operator=(const Navigation&) {}

    protected:
    Navigation() {}
    Navigation(const Navigation&) {}

   public:
    virtual ~Navigation() {}
    static  const CORBA::TypeInfo *_desc();
    virtual const CORBA::TypeInfo *_type_info() const;
    virtual void *_safe_narrow(const CORBA::TypeInfo& ) const;
    static CORBA::Object*_factory();
    Navigation_ptr _this();
    static Navigation_ptr _duplicate(Navigation_ptr _obj) {
      if (_obj) _obj->_ref();
      return _obj;
    }
    static Navigation_ptr _nil() { return (Navigation_ptr)NULL; }
    static Navigation_ptr _narrow(CORBA::Object* _obj);
    static Navigation_ptr _clone(Navigation_ptr _obj) {
      CORBA::Object_var _obj_var(CORBA_Object::_clone(_obj));
      #if defined(_HPCC_BUG)
        return _narrow(_obj_var.operator CORBA::Object_ptr());
      #else
        return _narrow(_obj_var);
      #endif  // _HPCC_BUG
    }
    static Navigation_ptr _bind(const char *_object_name = (const char*)NULL,
                                const char *_host_name = (const char*)NULL,
                                const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                                CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    static Navigation_ptr _bind(const char *_poa_name,
                                const CORBA::OctetSequence& _id,
                                const char *_host_name = (const char*)NULL,
                                const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                                CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    virtual CORBA::Object_ptr provide_facet(const char* _name);
    virtual FacetDescriptions* get_all_facets();
    virtual FacetDescriptions* get_named_facets(const NameList& _names);
    virtual CORBA::Boolean same_component(CORBA::Object_ptr _object_ref);

    friend VISostream& _marshal_in(VISostream& _strm, const Navigation_ptr _obj);
    friend VISistream& _marshal_out(VISistream& _strm, Navigation_ptr& _obj);
    friend Ostream& _marshal_in(Ostream& _strm, const Navigation_ptr _obj);
    friend Istream& _marshal_out(Istream& _strm, Navigation_ptr& _obj);
  };

  class Navigation_ops;
  typedef Navigation_ops* Navigation_ops_ptr;

  class  Navigation_ops  {
   public:
    Navigation_ops() {}
    virtual ~Navigation_ops () {}

    virtual CORBA::Object_ptr provide_facet(const char* _name) = 0;
    virtual FacetDescriptions* get_all_facets() = 0;
    virtual FacetDescriptions* get_named_facets(const NameList& _names) = 0;
    virtual CORBA::Boolean same_component(CORBA::Object_ptr _object_ref) = 0;

    static const VISOps_Info *_desc();
    static Navigation_ops_ptr _nil() { return (Navigation_ops_ptr)NULL; }
    static Navigation_ops_ptr _downcast(PortableServer::ServantBase* _servant);
  protected:
    static const VISOps_Info _ops_info;
  };

  class  InvalidConnection : public CORBA_UserException {
   public:

    #if defined(MSVCNEWDLL_BUG)
      void *operator new(size_t ts);
      void *operator new(size_t ts, char*, int) {return operator new(ts);}
      void operator delete(void *p);
    #endif // MSVCNEWDLL_BUG

    InvalidConnection();
    virtual ~InvalidConnection();

    
    static CORBA::Exception *_factory() { return new InvalidConnection(); }
    virtual const CORBA_Exception::Description& _desc() const;
    static InvalidConnection* _downcast(CORBA::Exception *_exc);
    static const InvalidConnection* _downcast(const CORBA::Exception *_exc);
    CORBA::Exception *_deep_copy() { return new InvalidConnection(*this); }
  #ifdef AIXV3
    void _raise() const { VISTHROW_INST((InvalidConnection*)this) }
  #else
    void _raise() const { VISTHROW_INST(this) }
  #endif

    friend void _internal_pretty_print(VISostream&, const InvalidConnection&);
    void _write(VISostream&) const;
    void _copy(VISistream&);

    friend VISistream& _marshal_out(VISistream& _strm, InvalidConnection& _e);
    static const CORBA_Exception::Description _description;
  };

  class  AlreadyConnected : public CORBA_UserException {
   public:

    #if defined(MSVCNEWDLL_BUG)
      void *operator new(size_t ts);
      void *operator new(size_t ts, char*, int) {return operator new(ts);}
      void operator delete(void *p);
    #endif // MSVCNEWDLL_BUG

    AlreadyConnected();
    virtual ~AlreadyConnected();

    
    static CORBA::Exception *_factory() { return new AlreadyConnected(); }
    virtual const CORBA_Exception::Description& _desc() const;
    static AlreadyConnected* _downcast(CORBA::Exception *_exc);
    static const AlreadyConnected* _downcast(const CORBA::Exception *_exc);
    CORBA::Exception *_deep_copy() { return new AlreadyConnected(*this); }
  #ifdef AIXV3
    void _raise() const { VISTHROW_INST((AlreadyConnected*)this) }
  #else
    void _raise() const { VISTHROW_INST(this) }
  #endif

    friend void _internal_pretty_print(VISostream&, const AlreadyConnected&);
    void _write(VISostream&) const;
    void _copy(VISistream&);

    friend VISistream& _marshal_out(VISistream& _strm, AlreadyConnected& _e);
    static const CORBA_Exception::Description _description;
  };

  class  ExceededConnectionLimit : public CORBA_UserException {
   public:

    #if defined(MSVCNEWDLL_BUG)
      void *operator new(size_t ts);
      void *operator new(size_t ts, char*, int) {return operator new(ts);}
      void operator delete(void *p);
    #endif // MSVCNEWDLL_BUG

    ExceededConnectionLimit();
    virtual ~ExceededConnectionLimit();

    
    static CORBA::Exception *_factory() { return new ExceededConnectionLimit(); }
    virtual const CORBA_Exception::Description& _desc() const;
    static ExceededConnectionLimit* _downcast(CORBA::Exception *_exc);
    static const ExceededConnectionLimit* _downcast(const CORBA::Exception *_exc);
    CORBA::Exception *_deep_copy() { return new ExceededConnectionLimit(*this); }
  #ifdef AIXV3
    void _raise() const { VISTHROW_INST((ExceededConnectionLimit*)this) }
  #else
    void _raise() const { VISTHROW_INST(this) }
  #endif

    friend void _internal_pretty_print(VISostream&, const ExceededConnectionLimit&);
    void _write(VISostream&) const;
    void _copy(VISistream&);

    friend VISistream& _marshal_out(VISistream& _strm, ExceededConnectionLimit& _e);
    static const CORBA_Exception::Description _description;
  };

  class  CookieRequired : public CORBA_UserException {
   public:

    #if defined(MSVCNEWDLL_BUG)
      void *operator new(size_t ts);
      void *operator new(size_t ts, char*, int) {return operator new(ts);}
      void operator delete(void *p);
    #endif // MSVCNEWDLL_BUG

    CookieRequired();
    virtual ~CookieRequired();

    
    static CORBA::Exception *_factory() { return new CookieRequired(); }
    virtual const CORBA_Exception::Description& _desc() const;
    static CookieRequired* _downcast(CORBA::Exception *_exc);
    static const CookieRequired* _downcast(const CORBA::Exception *_exc);
    CORBA::Exception *_deep_copy() { return new CookieRequired(*this); }
  #ifdef AIXV3
    void _raise() const { VISTHROW_INST((CookieRequired*)this) }
  #else
    void _raise() const { VISTHROW_INST(this) }
  #endif

    friend void _internal_pretty_print(VISostream&, const CookieRequired&);
    void _write(VISostream&) const;
    void _copy(VISistream&);

    friend VISistream& _marshal_out(VISistream& _strm, CookieRequired& _e);
    static const CORBA_Exception::Description _description;
  };

  class  NoConnection : public CORBA_UserException {
   public:

    #if defined(MSVCNEWDLL_BUG)
      void *operator new(size_t ts);
      void *operator new(size_t ts, char*, int) {return operator new(ts);}
      void operator delete(void *p);
    #endif // MSVCNEWDLL_BUG

    NoConnection();
    virtual ~NoConnection();

    
    static CORBA::Exception *_factory() { return new NoConnection(); }
    virtual const CORBA_Exception::Description& _desc() const;
    static NoConnection* _downcast(CORBA::Exception *_exc);
    static const NoConnection* _downcast(const CORBA::Exception *_exc);
    CORBA::Exception *_deep_copy() { return new NoConnection(*this); }
  #ifdef AIXV3
    void _raise() const { VISTHROW_INST((NoConnection*)this) }
  #else
    void _raise() const { VISTHROW_INST(this) }
  #endif

    friend void _internal_pretty_print(VISostream&, const NoConnection&);
    void _write(VISostream&) const;
    void _copy(VISistream&);

    friend VISistream& _marshal_out(VISistream& _strm, NoConnection& _e);
    static const CORBA_Exception::Description _description;
  };

  #ifndef Components_ConnectionDescription_var_
  #define Components_ConnectionDescription_var_

  class  ConnectionDescription;

  typedef ConnectionDescription* ConnectionDescription_ptr;

   VISistream& _marshal_out(VISistream&, ConnectionDescription_ptr&);
   VISostream& _marshal_in(VISostream&, const ConnectionDescription_ptr);

  class  ConnectionDescription_out;
  class  ConnectionDescription_var: public CORBA::_var {
    friend class ConnectionDescription_out;
     public:
    ConnectionDescription_var();
    ConnectionDescription_var(ConnectionDescription_ptr );
    ConnectionDescription_var(const ConnectionDescription_var &);

    virtual ~ConnectionDescription_var();

    ConnectionDescription_var& operator=(ConnectionDescription_ptr);
    void operator=(const ConnectionDescription_var & _v);

    static void _add_ref(ConnectionDescription_ptr );
    static void _remove_ref(ConnectionDescription_ptr );

    operator ConnectionDescription*() const { return _ptr; }
    ConnectionDescription* operator->() { return _ptr; }
    const ConnectionDescription* operator->() const { return _ptr; }

    ConnectionDescription_ptr in () const { return _ptr; }
    ConnectionDescription_ptr& inout () { return _ptr; }
    ConnectionDescription_ptr& out();
    ConnectionDescription_ptr _retn();

    friend VISistream& _marshal_out(VISistream& _strm, ConnectionDescription_var& _var);
    friend VISostream& _marshal_in(VISostream& _strm, const ConnectionDescription_var& _var);

    friend Istream& _marshal_out(Istream&, ConnectionDescription_var&);
    friend Ostream& _marshal_in(Ostream&, const ConnectionDescription_var&);
     private:
    ConnectionDescription_ptr _ptr;
  };
  class  ConnectionDescription_out {
   private:
    ConnectionDescription_ptr& _ptr;
    static ConnectionDescription* _nil() { return (ConnectionDescription*)NULL; }

    void operator=(const ConnectionDescription_out&);
    void operator=(const ConnectionDescription_var&);
     public:
    ConnectionDescription_out(const ConnectionDescription_out& _o) : _ptr(_o._ptr) {}
    ConnectionDescription_out(ConnectionDescription_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    ConnectionDescription_out(ConnectionDescription_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    ConnectionDescription_out& operator=(ConnectionDescription* _p) {
      _ptr = _p;
      return *this;
    }
    operator ConnectionDescription_ptr&() { return _ptr; }
    ConnectionDescription_ptr& ptr() { return _ptr; }
    ConnectionDescription* operator->() { return _ptr; }
  };

  #endif	// Components_ConnectionDescription_var_
  // idl valuetype: Components::ConnectionDescription
  class  ConnectionDescription  : public virtual CORBA::ValueBase  {
    public:
      friend VISostream& _marshal_in(VISostream& _strm, const ConnectionDescription_ptr _obj);
      friend VISistream& _marshal_out(VISistream& _strm, ConnectionDescription_ptr& _obj);
      friend Ostream& _marshal_in(Ostream& _strm, const ConnectionDescription_ptr _obj);
      friend Istream& _marshal_out(Istream& _strm, ConnectionDescription_ptr& _obj);
      friend void _internal_pretty_print(VISostream& _strm, const ConnectionDescription& _obj);
      friend void _internal_pretty_print(VISostream& _strm, const ConnectionDescription* _obj);

      static ConnectionDescription* _downcast(CORBA::ValueBase* vbptr);
      virtual void ck(Cookie_ptr _ck) = 0;
      virtual Cookie_ptr ck() const = 0;
      virtual void objref(const CORBA::Object_ptr _objref) = 0;
      virtual const CORBA::Object_ptr objref() const = 0;
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
       ConnectionDescription() {}
        virtual ~ConnectionDescription() {}

      private:
        void operator=(const ConnectionDescription&);
  };
  class  OBV_ConnectionDescription: public virtual ConnectionDescription  {
    public:
      virtual void ck(Cookie_ptr _ck) {
        ::Components::Cookie_var::_add_ref(_ck);
        _obv_ck = _ck;
      }
      virtual Cookie_ptr ck() const {
        return _obv_ck;
      }
      virtual void objref(const CORBA::Object_ptr _objref) {
        _obv_objref = CORBA::Object::_duplicate(_objref);
      }
      virtual const CORBA::Object_ptr objref() const {
        return _obv_objref;
      }
    protected:
      OBV_ConnectionDescription();
      virtual ~OBV_ConnectionDescription();

      OBV_ConnectionDescription(Cookie_ptr _ck, 
                                const CORBA::Object_ptr _objref) {
                ::Components::Cookie_var::_add_ref(_ck);
        _obv_ck = _ck;
 
                _obv_objref = CORBA::Object::_duplicate(_objref);
  
      }
      Cookie_var _obv_ck;
      CORBA::Object_var _obv_objref;
  };
  class ConnectionDescription_init : public CORBA::ValueFactoryBase {
  };
  class  ConnectionDescriptions;
  typedef ConnectionDescriptions* ConnectionDescriptions_ptr;

  class  ConnectionDescriptions : private VISResource {
  private:
    ConnectionDescription_ptr *_contents;
    CORBA::ULong _count;
    CORBA::ULong _num_allocated;
    CORBA::Boolean _release_flag;

  public:
    #if defined(MSVCNEWDLL_BUG)
      void *operator new(size_t ts);
      void *operator new(size_t ts, char*, int) {return operator new(ts);}
      void operator delete(void *p);
    #endif // MSVCNEWDLL_BUG

    static ConnectionDescription_ptr *allocbuf(CORBA::ULong _nelems);
    static void freebuf(ConnectionDescription_ptr* _data);
    static void freebuf_elems(ConnectionDescription_ptr *_data, CORBA::ULong _nelems);

    class  Sequence_ConnectionDescriptions {
      friend class ConnectionDescriptions;
       private:
      CORBA::Boolean _release_flag;
      ConnectionDescription_ptr &_ptr;

      void release() {
        if (_release_flag) {
          CORBA::remove_ref(_ptr);
          _ptr = (Components::ConnectionDescription *)NULL;
        }
      }

   public:
      Sequence_ConnectionDescriptions(ConnectionDescription_ptr &_p,
                                      CORBA::Boolean _rel)
       : _ptr(_p), _release_flag(_rel) {}

      Sequence_ConnectionDescriptions(const Sequence_ConnectionDescriptions& _seq)
       : _ptr(_seq._ptr), _release_flag(_seq._release_flag) {}

      Sequence_ConnectionDescriptions& operator=(ConnectionDescription_ptr _p) {
        if (_ptr != _p) {
          release();
          _ptr = _p;
        }
        return *this;
      }

      Sequence_ConnectionDescriptions& operator=(const Sequence_ConnectionDescriptions& _p) {
        if (_ptr != _p._ptr) {
          if (_release_flag) {
            release();
            ::Components::ConnectionDescription_var::_add_ref(_p._ptr);
            _ptr = _p._ptr;
          } else
            _ptr = _p._ptr;
        }
        return *this;
      }

      operator ConnectionDescription_ptr () const { return _ptr; }
      ConnectionDescription_ptr operator ->() const { return _ptr; }
    };

    ConnectionDescriptions(CORBA::ULong _max = 0);
    ConnectionDescriptions(CORBA::ULong max, CORBA::ULong length,
                           ConnectionDescription_ptr *_data,
                           CORBA::Boolean release = (CORBA::Boolean)0);
    ConnectionDescriptions(const ConnectionDescriptions&);
    virtual ~ConnectionDescriptions();

    ConnectionDescriptions& operator=(const ConnectionDescriptions&);
    ConnectionDescriptions& operator=(const ConnectionDescriptions* _obj) {
      assert(_obj);
      return operator=(*_obj);
    }

    CORBA::ULong maximum() const { return _num_allocated; }

    void length(CORBA::ULong _len);
    CORBA::ULong length() const { return _count; }

    void replace(CORBA::ULong _max, CORBA::ULong _len,
                 ConnectionDescription_ptr *_data,
                 CORBA::Boolean _release = (CORBA::Boolean)0);

    ConnectionDescription_ptr *get_buffer(CORBA::Boolean orphan = (CORBA::Boolean) 0);
    const ConnectionDescription_ptr* get_buffer() const;

    Sequence_ConnectionDescriptions operator[](CORBA::ULong _index);
    const Sequence_ConnectionDescriptions operator[](CORBA::ULong _index) const;

    static ConnectionDescriptions *_duplicate(ConnectionDescriptions* _ptr) {
      if (_ptr) _ptr->_ref();
      return _ptr;
    }

    static void _release(ConnectionDescriptions *_ptr) {
      if (_ptr && (_ptr->_deref() == 0L)) 
        delete _ptr;
    }

    friend VISostream& _marshal_in(VISostream&, const ConnectionDescriptions&);
    friend VISistream& _marshal_out(VISistream&, ConnectionDescriptions&);

    friend VISostream& _marshal_in(VISostream& _strm, const ConnectionDescriptions* _obj);
    friend VISistream& _marshal_out(VISistream& _strm, ConnectionDescriptions_ptr& _obj);

    friend void _internal_pretty_print(VISostream&, const ConnectionDescriptions&);

    friend Ostream& _marshal_in(Ostream&, const ConnectionDescriptions&);
    friend Istream& _marshal_out(Istream&, ConnectionDescriptions&);
    friend Istream& _marshal_out(Istream&, ConnectionDescriptions_ptr&);
  };

  class  ConnectionDescriptions_out;

  class  ConnectionDescriptions_var {
    friend class ConnectionDescriptions_out;
     private:
    ConnectionDescriptions *_ptr;
     public:
    ConnectionDescriptions_var() : _ptr((ConnectionDescriptions*)NULL) {}
    ConnectionDescriptions_var(ConnectionDescriptions *_p) : _ptr(_p) {}
    ConnectionDescriptions_var(const ConnectionDescriptions_var& _var) :_ptr(ConnectionDescriptions::_duplicate(_var._ptr)){}
    virtual ~ConnectionDescriptions_var() { ConnectionDescriptions::_release(_ptr); }
    ConnectionDescriptions_var& operator=(ConnectionDescriptions* _p) {
      ConnectionDescriptions::_release(_ptr);
      _ptr = _p;
      return *this;
    }

    ConnectionDescriptions_var& operator=(const ConnectionDescriptions_var& _var) {
      ConnectionDescriptions::_release(_ptr);
      _ptr = ConnectionDescriptions::_duplicate(_var._ptr);
      return *this;
    }
    operator ConnectionDescriptions*() const { return _ptr; }

    ConnectionDescriptions* operator->() { return _ptr; }
    const ConnectionDescriptions* operator->() const { return _ptr; }

    operator ConnectionDescriptions&() { return *_ptr; }
    operator const ConnectionDescriptions&() const { return *_ptr; }

    const ConnectionDescriptions& in() const { return *_ptr; }
    ConnectionDescriptions& inout() { return *_ptr; }
    ConnectionDescriptions_ptr & out() {
      ConnectionDescriptions::_release(_ptr);
      _ptr = (ConnectionDescriptions *)NULL;
      return _ptr;
    }

    ConnectionDescriptions* _retn() {
      ConnectionDescriptions* _tmp_ptr;
      _tmp_ptr = _ptr;
      _ptr = (ConnectionDescriptions *)NULL;
      return _tmp_ptr;
    }
    friend VISostream& _marshal_in(VISostream&, const ConnectionDescriptions_var&);
    friend VISistream& _marshal_out(VISistream&, ConnectionDescriptions_var&);

    ConnectionDescriptions::Sequence_ConnectionDescriptions operator[](CORBA::ULong _index);
    const ConnectionDescriptions::Sequence_ConnectionDescriptions operator[](CORBA::ULong _index) const;

  };
  class  ConnectionDescriptions_out {
   private:
    ConnectionDescriptions_ptr& _ptr;
    static ConnectionDescriptions* _nil() { return (ConnectionDescriptions*)NULL; }

    void operator=(const ConnectionDescriptions_out&);
    void operator=(const ConnectionDescriptions_var&);
     public:
    ConnectionDescriptions_out(const ConnectionDescriptions_out& _o) : _ptr(_o._ptr) {}
    ConnectionDescriptions_out(ConnectionDescriptions_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    ConnectionDescriptions_out(ConnectionDescriptions_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    ConnectionDescriptions_out& operator=(ConnectionDescriptions* _p) {
      _ptr = _p;
      return *this;
    }
    operator ConnectionDescriptions_ptr&() { return _ptr; }
    ConnectionDescriptions_ptr& ptr() { return _ptr; }
  };
  #ifndef Components_ReceptacleDescription_var_
  #define Components_ReceptacleDescription_var_

  class  ReceptacleDescription;

  typedef ReceptacleDescription* ReceptacleDescription_ptr;

   VISistream& _marshal_out(VISistream&, ReceptacleDescription_ptr&);
   VISostream& _marshal_in(VISostream&, const ReceptacleDescription_ptr);

  class  ReceptacleDescription_out;
  class  ReceptacleDescription_var: public CORBA::_var {
    friend class ReceptacleDescription_out;
     public:
    ReceptacleDescription_var();
    ReceptacleDescription_var(ReceptacleDescription_ptr );
    ReceptacleDescription_var(const ReceptacleDescription_var &);

    virtual ~ReceptacleDescription_var();

    ReceptacleDescription_var& operator=(ReceptacleDescription_ptr);
    void operator=(const ReceptacleDescription_var & _v);

    static void _add_ref(ReceptacleDescription_ptr );
    static void _remove_ref(ReceptacleDescription_ptr );

    operator ReceptacleDescription*() const { return _ptr; }
    ReceptacleDescription* operator->() { return _ptr; }
    const ReceptacleDescription* operator->() const { return _ptr; }

    ReceptacleDescription_ptr in () const { return _ptr; }
    ReceptacleDescription_ptr& inout () { return _ptr; }
    ReceptacleDescription_ptr& out();
    ReceptacleDescription_ptr _retn();

    friend VISistream& _marshal_out(VISistream& _strm, ReceptacleDescription_var& _var);
    friend VISostream& _marshal_in(VISostream& _strm, const ReceptacleDescription_var& _var);

    friend Istream& _marshal_out(Istream&, ReceptacleDescription_var&);
    friend Ostream& _marshal_in(Ostream&, const ReceptacleDescription_var&);
     private:
    ReceptacleDescription_ptr _ptr;
  };
  class  ReceptacleDescription_out {
   private:
    ReceptacleDescription_ptr& _ptr;
    static ReceptacleDescription* _nil() { return (ReceptacleDescription*)NULL; }

    void operator=(const ReceptacleDescription_out&);
    void operator=(const ReceptacleDescription_var&);
     public:
    ReceptacleDescription_out(const ReceptacleDescription_out& _o) : _ptr(_o._ptr) {}
    ReceptacleDescription_out(ReceptacleDescription_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    ReceptacleDescription_out(ReceptacleDescription_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    ReceptacleDescription_out& operator=(ReceptacleDescription* _p) {
      _ptr = _p;
      return *this;
    }
    operator ReceptacleDescription_ptr&() { return _ptr; }
    ReceptacleDescription_ptr& ptr() { return _ptr; }
    ReceptacleDescription* operator->() { return _ptr; }
  };

  #endif	// Components_ReceptacleDescription_var_
  // idl valuetype: Components::ReceptacleDescription
  class  ReceptacleDescription  : public virtual PortDescription  {
    public:
      friend VISostream& _marshal_in(VISostream& _strm, const ReceptacleDescription_ptr _obj);
      friend VISistream& _marshal_out(VISistream& _strm, ReceptacleDescription_ptr& _obj);
      friend Ostream& _marshal_in(Ostream& _strm, const ReceptacleDescription_ptr _obj);
      friend Istream& _marshal_out(Istream& _strm, ReceptacleDescription_ptr& _obj);
      friend void _internal_pretty_print(VISostream& _strm, const ReceptacleDescription& _obj);
      friend void _internal_pretty_print(VISostream& _strm, const ReceptacleDescription* _obj);

      static ReceptacleDescription* _downcast(CORBA::ValueBase* vbptr);
      virtual void is_multiple(const CORBA::Boolean _is_multiple) = 0;
      virtual const CORBA::Boolean is_multiple() const = 0;

      virtual void connections(const ConnectionDescriptions& _connections) = 0;
      virtual const ConnectionDescriptions& connections() const = 0;
      virtual ConnectionDescriptions& connections() = 0;
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
       ReceptacleDescription() {}
        virtual ~ReceptacleDescription() {}

      private:
        void operator=(const ReceptacleDescription&);
  };
  class  OBV_ReceptacleDescription: public virtual ReceptacleDescription,
                                    public virtual OBV_PortDescription  {
    public:
      virtual void is_multiple(const CORBA::Boolean _is_multiple) {
        _obv_is_multiple = _is_multiple;
      }
      virtual const CORBA::Boolean is_multiple() const {
        return _obv_is_multiple;
      }

      virtual void connections(const ConnectionDescriptions& _connections) {
        _obv_connections = _connections;
      }
      virtual const ConnectionDescriptions& connections() const {
        return _obv_connections;
      }
      virtual ConnectionDescriptions& connections() {
        return _obv_connections;
      }
    protected:
      OBV_ReceptacleDescription();
      virtual ~OBV_ReceptacleDescription();

      OBV_ReceptacleDescription(const CORBA::Boolean _is_multiple, 
                                const ConnectionDescriptions& _connections) {
                _obv_is_multiple = _is_multiple;
 
                _obv_connections = _connections;
  
      }
      CORBA::Boolean _obv_is_multiple;
      ConnectionDescriptions _obv_connections;
  };
  class ReceptacleDescription_init : public CORBA::ValueFactoryBase {
  };
  class  ReceptacleDescriptions;
  typedef ReceptacleDescriptions* ReceptacleDescriptions_ptr;

  class  ReceptacleDescriptions : private VISResource {
  private:
    ReceptacleDescription_ptr *_contents;
    CORBA::ULong _count;
    CORBA::ULong _num_allocated;
    CORBA::Boolean _release_flag;

  public:
    #if defined(MSVCNEWDLL_BUG)
      void *operator new(size_t ts);
      void *operator new(size_t ts, char*, int) {return operator new(ts);}
      void operator delete(void *p);
    #endif // MSVCNEWDLL_BUG

    static ReceptacleDescription_ptr *allocbuf(CORBA::ULong _nelems);
    static void freebuf(ReceptacleDescription_ptr* _data);
    static void freebuf_elems(ReceptacleDescription_ptr *_data, CORBA::ULong _nelems);

    class  Sequence_ReceptacleDescriptions {
      friend class ReceptacleDescriptions;
       private:
      CORBA::Boolean _release_flag;
      ReceptacleDescription_ptr &_ptr;

      void release() {
        if (_release_flag) {
          CORBA::remove_ref(_ptr);
          _ptr = (Components::ReceptacleDescription *)NULL;
        }
      }

   public:
      Sequence_ReceptacleDescriptions(ReceptacleDescription_ptr &_p,
                                      CORBA::Boolean _rel)
       : _ptr(_p), _release_flag(_rel) {}

      Sequence_ReceptacleDescriptions(const Sequence_ReceptacleDescriptions& _seq)
       : _ptr(_seq._ptr), _release_flag(_seq._release_flag) {}

      Sequence_ReceptacleDescriptions& operator=(ReceptacleDescription_ptr _p) {
        if (_ptr != _p) {
          release();
          _ptr = _p;
        }
        return *this;
      }

      Sequence_ReceptacleDescriptions& operator=(const Sequence_ReceptacleDescriptions& _p) {
        if (_ptr != _p._ptr) {
          if (_release_flag) {
            release();
            ::Components::ReceptacleDescription_var::_add_ref(_p._ptr);
            _ptr = _p._ptr;
          } else
            _ptr = _p._ptr;
        }
        return *this;
      }

      operator ReceptacleDescription_ptr () const { return _ptr; }
      ReceptacleDescription_ptr operator ->() const { return _ptr; }
    };

    ReceptacleDescriptions(CORBA::ULong _max = 0);
    ReceptacleDescriptions(CORBA::ULong max, CORBA::ULong length,
                           ReceptacleDescription_ptr *_data,
                           CORBA::Boolean release = (CORBA::Boolean)0);
    ReceptacleDescriptions(const ReceptacleDescriptions&);
    virtual ~ReceptacleDescriptions();

    ReceptacleDescriptions& operator=(const ReceptacleDescriptions&);
    ReceptacleDescriptions& operator=(const ReceptacleDescriptions* _obj) {
      assert(_obj);
      return operator=(*_obj);
    }

    CORBA::ULong maximum() const { return _num_allocated; }

    void length(CORBA::ULong _len);
    CORBA::ULong length() const { return _count; }

    void replace(CORBA::ULong _max, CORBA::ULong _len,
                 ReceptacleDescription_ptr *_data,
                 CORBA::Boolean _release = (CORBA::Boolean)0);

    ReceptacleDescription_ptr *get_buffer(CORBA::Boolean orphan = (CORBA::Boolean) 0);
    const ReceptacleDescription_ptr* get_buffer() const;

    Sequence_ReceptacleDescriptions operator[](CORBA::ULong _index);
    const Sequence_ReceptacleDescriptions operator[](CORBA::ULong _index) const;

    static ReceptacleDescriptions *_duplicate(ReceptacleDescriptions* _ptr) {
      if (_ptr) _ptr->_ref();
      return _ptr;
    }

    static void _release(ReceptacleDescriptions *_ptr) {
      if (_ptr && (_ptr->_deref() == 0L)) 
        delete _ptr;
    }

    friend VISostream& _marshal_in(VISostream&, const ReceptacleDescriptions&);
    friend VISistream& _marshal_out(VISistream&, ReceptacleDescriptions&);

    friend VISostream& _marshal_in(VISostream& _strm, const ReceptacleDescriptions* _obj);
    friend VISistream& _marshal_out(VISistream& _strm, ReceptacleDescriptions_ptr& _obj);

    friend void _internal_pretty_print(VISostream&, const ReceptacleDescriptions&);

    friend Ostream& _marshal_in(Ostream&, const ReceptacleDescriptions&);
    friend Istream& _marshal_out(Istream&, ReceptacleDescriptions&);
    friend Istream& _marshal_out(Istream&, ReceptacleDescriptions_ptr&);
  };

  class  ReceptacleDescriptions_out;

  class  ReceptacleDescriptions_var {
    friend class ReceptacleDescriptions_out;
     private:
    ReceptacleDescriptions *_ptr;
     public:
    ReceptacleDescriptions_var() : _ptr((ReceptacleDescriptions*)NULL) {}
    ReceptacleDescriptions_var(ReceptacleDescriptions *_p) : _ptr(_p) {}
    ReceptacleDescriptions_var(const ReceptacleDescriptions_var& _var) :_ptr(ReceptacleDescriptions::_duplicate(_var._ptr)){}
    virtual ~ReceptacleDescriptions_var() { ReceptacleDescriptions::_release(_ptr); }
    ReceptacleDescriptions_var& operator=(ReceptacleDescriptions* _p) {
      ReceptacleDescriptions::_release(_ptr);
      _ptr = _p;
      return *this;
    }

    ReceptacleDescriptions_var& operator=(const ReceptacleDescriptions_var& _var) {
      ReceptacleDescriptions::_release(_ptr);
      _ptr = ReceptacleDescriptions::_duplicate(_var._ptr);
      return *this;
    }
    operator ReceptacleDescriptions*() const { return _ptr; }

    ReceptacleDescriptions* operator->() { return _ptr; }
    const ReceptacleDescriptions* operator->() const { return _ptr; }

    operator ReceptacleDescriptions&() { return *_ptr; }
    operator const ReceptacleDescriptions&() const { return *_ptr; }

    const ReceptacleDescriptions& in() const { return *_ptr; }
    ReceptacleDescriptions& inout() { return *_ptr; }
    ReceptacleDescriptions_ptr & out() {
      ReceptacleDescriptions::_release(_ptr);
      _ptr = (ReceptacleDescriptions *)NULL;
      return _ptr;
    }

    ReceptacleDescriptions* _retn() {
      ReceptacleDescriptions* _tmp_ptr;
      _tmp_ptr = _ptr;
      _ptr = (ReceptacleDescriptions *)NULL;
      return _tmp_ptr;
    }
    friend VISostream& _marshal_in(VISostream&, const ReceptacleDescriptions_var&);
    friend VISistream& _marshal_out(VISistream&, ReceptacleDescriptions_var&);

    ReceptacleDescriptions::Sequence_ReceptacleDescriptions operator[](CORBA::ULong _index);
    const ReceptacleDescriptions::Sequence_ReceptacleDescriptions operator[](CORBA::ULong _index) const;

  };
  class  ReceptacleDescriptions_out {
   private:
    ReceptacleDescriptions_ptr& _ptr;
    static ReceptacleDescriptions* _nil() { return (ReceptacleDescriptions*)NULL; }

    void operator=(const ReceptacleDescriptions_out&);
    void operator=(const ReceptacleDescriptions_var&);
     public:
    ReceptacleDescriptions_out(const ReceptacleDescriptions_out& _o) : _ptr(_o._ptr) {}
    ReceptacleDescriptions_out(ReceptacleDescriptions_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    ReceptacleDescriptions_out(ReceptacleDescriptions_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    ReceptacleDescriptions_out& operator=(ReceptacleDescriptions* _p) {
      _ptr = _p;
      return *this;
    }
    operator ReceptacleDescriptions_ptr&() { return _ptr; }
    ReceptacleDescriptions_ptr& ptr() { return _ptr; }
  };
  #ifndef Components_Receptacles_var_
  #define Components_Receptacles_var_

  class  Receptacles;

  typedef Receptacles* Receptacles_ptr;
  typedef Receptacles_ptr ReceptaclesRef;
   VISistream& _marshal_out(VISistream&, Receptacles_ptr&);
   VISostream& _marshal_in(VISostream&, const Receptacles_ptr);
  class  Receptacles_out;

  class  Receptacles_var : public CORBA::_var {
  friend class Receptacles_out;
   private:
    Receptacles_ptr _ptr;

   public:
    Receptacles_var();
    Receptacles_var(Receptacles_ptr);
    Receptacles_var(const Receptacles_var &);
    virtual ~Receptacles_var();

    static Receptacles_ptr _duplicate(Receptacles_ptr);
    static void _release(Receptacles_ptr);

    Receptacles_var& operator=(const Receptacles_var& _var);
    Receptacles_var& operator=(Receptacles_ptr);

    operator Receptacles*() const { return _ptr; }
    Receptacles* operator->() const { return _ptr; }

    Receptacles_ptr in() const { return _ptr; }
    Receptacles_ptr& inout() { return _ptr; }
    Receptacles_ptr& out();
    Receptacles_ptr _retn();

    friend VISostream& _marshal_in(VISostream&, const Receptacles_var&);
    friend VISistream& _marshal_out(VISistream&, Receptacles_var&);

    friend Istream& _marshal_out(Istream&, Receptacles_var&);
    friend Ostream& _marshal_in(Ostream&, const Receptacles_var&);
  };
  class  Receptacles_out {
   private:
    Receptacles_ptr& _ptr;
    static Receptacles* _nil() { return (Receptacles*)NULL; }

    void operator=(const Receptacles_out&);
    void operator=(const Receptacles_var&);
     public:
    Receptacles_out(const Receptacles_out& _o) : _ptr(_o._ptr) {}
    Receptacles_out(Receptacles_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    Receptacles_out(Receptacles_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    Receptacles_out& operator=(Receptacles* _p) {
      _ptr = _p;
      return *this;
    }
    operator Receptacles_ptr&() { return _ptr; }
    Receptacles_ptr& ptr() { return _ptr; }
    Receptacles* operator->() { return _ptr; }
  };

  #endif // Components_Receptacles_var_

  // idl interface: Components::Receptacles
  class  Receptacles : public virtual CORBA_Object {
   private:
    static const CORBA::TypeInfo _class_info;
    void operator=(const Receptacles&) {}

    protected:
    Receptacles() {}
    Receptacles(const Receptacles&) {}

   public:
    virtual ~Receptacles() {}
    static  const CORBA::TypeInfo *_desc();
    virtual const CORBA::TypeInfo *_type_info() const;
    virtual void *_safe_narrow(const CORBA::TypeInfo& ) const;
    static CORBA::Object*_factory();
    Receptacles_ptr _this();
    static Receptacles_ptr _duplicate(Receptacles_ptr _obj) {
      if (_obj) _obj->_ref();
      return _obj;
    }
    static Receptacles_ptr _nil() { return (Receptacles_ptr)NULL; }
    static Receptacles_ptr _narrow(CORBA::Object* _obj);
    static Receptacles_ptr _clone(Receptacles_ptr _obj) {
      CORBA::Object_var _obj_var(CORBA_Object::_clone(_obj));
      #if defined(_HPCC_BUG)
        return _narrow(_obj_var.operator CORBA::Object_ptr());
      #else
        return _narrow(_obj_var);
      #endif  // _HPCC_BUG
    }
    static Receptacles_ptr _bind(const char *_object_name = (const char*)NULL,
                                 const char *_host_name = (const char*)NULL,
                                 const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                                 CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    static Receptacles_ptr _bind(const char *_poa_name,
                                 const CORBA::OctetSequence& _id,
                                 const char *_host_name = (const char*)NULL,
                                 const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                                 CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    virtual Cookie_ptr connect(const char* _name,
                               CORBA::Object_ptr _connection);
    virtual void disconnect(const char* _name, Cookie_ptr _ck);
    virtual ConnectionDescriptions* get_connections(const char* _name);
    virtual ReceptacleDescriptions* get_all_receptacles();
    virtual ReceptacleDescriptions* get_named_receptacles(const NameList& _names);

    friend VISostream& _marshal_in(VISostream& _strm, const Receptacles_ptr _obj);
    friend VISistream& _marshal_out(VISistream& _strm, Receptacles_ptr& _obj);
    friend Ostream& _marshal_in(Ostream& _strm, const Receptacles_ptr _obj);
    friend Istream& _marshal_out(Istream& _strm, Receptacles_ptr& _obj);
  };

  class Receptacles_ops;
  typedef Receptacles_ops* Receptacles_ops_ptr;

  class  Receptacles_ops  {
   public:
    Receptacles_ops() {}
    virtual ~Receptacles_ops () {}

    virtual Cookie_ptr connect(const char* _name,
                               CORBA::Object_ptr _connection) = 0;
    virtual void disconnect(const char* _name, Cookie_ptr _ck) = 0;
    virtual ConnectionDescriptions* get_connections(const char* _name) = 0;
    virtual ReceptacleDescriptions* get_all_receptacles() = 0;
    virtual ReceptacleDescriptions* get_named_receptacles(const NameList& _names) = 0;

    static const VISOps_Info *_desc();
    static Receptacles_ops_ptr _nil() { return (Receptacles_ops_ptr)NULL; }
    static Receptacles_ops_ptr _downcast(PortableServer::ServantBase* _servant);
  protected:
    static const VISOps_Info _ops_info;
  };

  class  BadEventType : public CORBA_UserException {
   public:

    #if defined(MSVCNEWDLL_BUG)
      void *operator new(size_t ts);
      void *operator new(size_t ts, char*, int) {return operator new(ts);}
      void operator delete(void *p);
    #endif // MSVCNEWDLL_BUG

    BadEventType();
    BadEventType( const char* _expected_event_type );
    virtual ~BadEventType();

    CORBA::String_var expected_event_type;
    static CORBA::Exception *_factory() { return new BadEventType(); }
    virtual const CORBA_Exception::Description& _desc() const;
    static BadEventType* _downcast(CORBA::Exception *_exc);
    static const BadEventType* _downcast(const CORBA::Exception *_exc);
    CORBA::Exception *_deep_copy() { return new BadEventType(*this); }
  #ifdef AIXV3
    void _raise() const { VISTHROW_INST((BadEventType*)this) }
  #else
    void _raise() const { VISTHROW_INST(this) }
  #endif

    friend void _internal_pretty_print(VISostream&, const BadEventType&);
    void _write(VISostream&) const;
    void _copy(VISistream&);

    friend VISistream& _marshal_out(VISistream& _strm, BadEventType& _e);
    static const CORBA_Exception::Description _description;
  };

  #ifndef Components_EventBase_var_
  #define Components_EventBase_var_

  class  EventBase;

  typedef EventBase* EventBase_ptr;

   VISistream& _marshal_out(VISistream&, EventBase_ptr&);
   VISostream& _marshal_in(VISostream&, const EventBase_ptr);

  class  EventBase_out;
  class  EventBase_var: public CORBA::_var {
    friend class EventBase_out;
     public:
    EventBase_var();
    EventBase_var(EventBase_ptr );
    EventBase_var(const EventBase_var &);

    virtual ~EventBase_var();

    EventBase_var& operator=(EventBase_ptr);
    void operator=(const EventBase_var & _v);

    static void _add_ref(EventBase_ptr );
    static void _remove_ref(EventBase_ptr );

    operator EventBase*() const { return _ptr; }
    EventBase* operator->() { return _ptr; }
    const EventBase* operator->() const { return _ptr; }

    EventBase_ptr in () const { return _ptr; }
    EventBase_ptr& inout () { return _ptr; }
    EventBase_ptr& out();
    EventBase_ptr _retn();

    friend VISistream& _marshal_out(VISistream& _strm, EventBase_var& _var);
    friend VISostream& _marshal_in(VISostream& _strm, const EventBase_var& _var);

    friend Istream& _marshal_out(Istream&, EventBase_var&);
    friend Ostream& _marshal_in(Ostream&, const EventBase_var&);
     private:
    EventBase_ptr _ptr;
  };
  class  EventBase_out {
   private:
    EventBase_ptr& _ptr;
    static EventBase* _nil() { return (EventBase*)NULL; }

    void operator=(const EventBase_out&);
    void operator=(const EventBase_var&);
     public:
    EventBase_out(const EventBase_out& _o) : _ptr(_o._ptr) {}
    EventBase_out(EventBase_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    EventBase_out(EventBase_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    EventBase_out& operator=(EventBase* _p) {
      _ptr = _p;
      return *this;
    }
    operator EventBase_ptr&() { return _ptr; }
    EventBase_ptr& ptr() { return _ptr; }
    EventBase* operator->() { return _ptr; }
  };

  #endif	// Components_EventBase_var_
  // idl valuetype: Components::EventBase
  class  EventBase  : public virtual CORBA::ValueBase  {
    public:
      friend VISostream& _marshal_in(VISostream& _strm, const EventBase_ptr _obj);
      friend VISistream& _marshal_out(VISistream& _strm, EventBase_ptr& _obj);
      friend Ostream& _marshal_in(Ostream& _strm, const EventBase_ptr _obj);
      friend Istream& _marshal_out(Istream& _strm, EventBase_ptr& _obj);
      friend void _internal_pretty_print(VISostream& _strm, const EventBase& _obj);
      friend void _internal_pretty_print(VISostream& _strm, const EventBase* _obj);

      static EventBase* _downcast(CORBA::ValueBase* vbptr);
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
       EventBase() {}
        virtual ~EventBase() {}

      private:
        void operator=(const EventBase&);
  };
  class EventBase_init : public CORBA::ValueFactoryBase {
  };
  #ifndef Components_EventConsumerBase_var_
  #define Components_EventConsumerBase_var_

  class  EventConsumerBase;

  typedef EventConsumerBase* EventConsumerBase_ptr;
  typedef EventConsumerBase_ptr EventConsumerBaseRef;
   VISistream& _marshal_out(VISistream&, EventConsumerBase_ptr&);
   VISostream& _marshal_in(VISostream&, const EventConsumerBase_ptr);
  class  EventConsumerBase_out;

  class  EventConsumerBase_var : public CORBA::_var {
  friend class EventConsumerBase_out;
   private:
    EventConsumerBase_ptr _ptr;

   public:
    EventConsumerBase_var();
    EventConsumerBase_var(EventConsumerBase_ptr);
    EventConsumerBase_var(const EventConsumerBase_var &);
    virtual ~EventConsumerBase_var();

    static EventConsumerBase_ptr _duplicate(EventConsumerBase_ptr);
    static void _release(EventConsumerBase_ptr);

    EventConsumerBase_var& operator=(const EventConsumerBase_var& _var);
    EventConsumerBase_var& operator=(EventConsumerBase_ptr);

    operator EventConsumerBase*() const { return _ptr; }
    EventConsumerBase* operator->() const { return _ptr; }

    EventConsumerBase_ptr in() const { return _ptr; }
    EventConsumerBase_ptr& inout() { return _ptr; }
    EventConsumerBase_ptr& out();
    EventConsumerBase_ptr _retn();

    friend VISostream& _marshal_in(VISostream&, const EventConsumerBase_var&);
    friend VISistream& _marshal_out(VISistream&, EventConsumerBase_var&);

    friend Istream& _marshal_out(Istream&, EventConsumerBase_var&);
    friend Ostream& _marshal_in(Ostream&, const EventConsumerBase_var&);
  };
  class  EventConsumerBase_out {
   private:
    EventConsumerBase_ptr& _ptr;
    static EventConsumerBase* _nil() { return (EventConsumerBase*)NULL; }

    void operator=(const EventConsumerBase_out&);
    void operator=(const EventConsumerBase_var&);
     public:
    EventConsumerBase_out(const EventConsumerBase_out& _o) : _ptr(_o._ptr) {}
    EventConsumerBase_out(EventConsumerBase_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    EventConsumerBase_out(EventConsumerBase_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    EventConsumerBase_out& operator=(EventConsumerBase* _p) {
      _ptr = _p;
      return *this;
    }
    operator EventConsumerBase_ptr&() { return _ptr; }
    EventConsumerBase_ptr& ptr() { return _ptr; }
    EventConsumerBase* operator->() { return _ptr; }
  };

  #endif // Components_EventConsumerBase_var_

  // idl interface: Components::EventConsumerBase
  class  EventConsumerBase : public virtual CORBA_Object {
   private:
    static const CORBA::TypeInfo _class_info;
    void operator=(const EventConsumerBase&) {}

    protected:
    EventConsumerBase() {}
    EventConsumerBase(const EventConsumerBase&) {}

   public:
    virtual ~EventConsumerBase() {}
    static  const CORBA::TypeInfo *_desc();
    virtual const CORBA::TypeInfo *_type_info() const;
    virtual void *_safe_narrow(const CORBA::TypeInfo& ) const;
    static CORBA::Object*_factory();
    EventConsumerBase_ptr _this();
    static EventConsumerBase_ptr _duplicate(EventConsumerBase_ptr _obj) {
      if (_obj) _obj->_ref();
      return _obj;
    }
    static EventConsumerBase_ptr _nil() { return (EventConsumerBase_ptr)NULL; }
    static EventConsumerBase_ptr _narrow(CORBA::Object* _obj);
    static EventConsumerBase_ptr _clone(EventConsumerBase_ptr _obj) {
      CORBA::Object_var _obj_var(CORBA_Object::_clone(_obj));
      #if defined(_HPCC_BUG)
        return _narrow(_obj_var.operator CORBA::Object_ptr());
      #else
        return _narrow(_obj_var);
      #endif  // _HPCC_BUG
    }
    static EventConsumerBase_ptr _bind(const char *_object_name = (const char*)NULL,
                                       const char *_host_name = (const char*)NULL,
                                       const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                                       CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    static EventConsumerBase_ptr _bind(const char *_poa_name,
                                       const CORBA::OctetSequence& _id,
                                       const char *_host_name = (const char*)NULL,
                                       const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                                       CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    virtual void push_event(EventBase_ptr _evt);

    friend VISostream& _marshal_in(VISostream& _strm, const EventConsumerBase_ptr _obj);
    friend VISistream& _marshal_out(VISistream& _strm, EventConsumerBase_ptr& _obj);
    friend Ostream& _marshal_in(Ostream& _strm, const EventConsumerBase_ptr _obj);
    friend Istream& _marshal_out(Istream& _strm, EventConsumerBase_ptr& _obj);
  };

  class EventConsumerBase_ops;
  typedef EventConsumerBase_ops* EventConsumerBase_ops_ptr;

  class  EventConsumerBase_ops  {
   public:
    EventConsumerBase_ops() {}
    virtual ~EventConsumerBase_ops () {}

    virtual void push_event(EventBase_ptr _evt) = 0;

    static const VISOps_Info *_desc();
    static EventConsumerBase_ops_ptr _nil() { return (EventConsumerBase_ops_ptr)NULL; }
    static EventConsumerBase_ops_ptr _downcast(PortableServer::ServantBase* _servant);
  protected:
    static const VISOps_Info _ops_info;
  };

  #ifndef Components_ConsumerDescription_var_
  #define Components_ConsumerDescription_var_

  class  ConsumerDescription;

  typedef ConsumerDescription* ConsumerDescription_ptr;

   VISistream& _marshal_out(VISistream&, ConsumerDescription_ptr&);
   VISostream& _marshal_in(VISostream&, const ConsumerDescription_ptr);

  class  ConsumerDescription_out;
  class  ConsumerDescription_var: public CORBA::_var {
    friend class ConsumerDescription_out;
     public:
    ConsumerDescription_var();
    ConsumerDescription_var(ConsumerDescription_ptr );
    ConsumerDescription_var(const ConsumerDescription_var &);

    virtual ~ConsumerDescription_var();

    ConsumerDescription_var& operator=(ConsumerDescription_ptr);
    void operator=(const ConsumerDescription_var & _v);

    static void _add_ref(ConsumerDescription_ptr );
    static void _remove_ref(ConsumerDescription_ptr );

    operator ConsumerDescription*() const { return _ptr; }
    ConsumerDescription* operator->() { return _ptr; }
    const ConsumerDescription* operator->() const { return _ptr; }

    ConsumerDescription_ptr in () const { return _ptr; }
    ConsumerDescription_ptr& inout () { return _ptr; }
    ConsumerDescription_ptr& out();
    ConsumerDescription_ptr _retn();

    friend VISistream& _marshal_out(VISistream& _strm, ConsumerDescription_var& _var);
    friend VISostream& _marshal_in(VISostream& _strm, const ConsumerDescription_var& _var);

    friend Istream& _marshal_out(Istream&, ConsumerDescription_var&);
    friend Ostream& _marshal_in(Ostream&, const ConsumerDescription_var&);
     private:
    ConsumerDescription_ptr _ptr;
  };
  class  ConsumerDescription_out {
   private:
    ConsumerDescription_ptr& _ptr;
    static ConsumerDescription* _nil() { return (ConsumerDescription*)NULL; }

    void operator=(const ConsumerDescription_out&);
    void operator=(const ConsumerDescription_var&);
     public:
    ConsumerDescription_out(const ConsumerDescription_out& _o) : _ptr(_o._ptr) {}
    ConsumerDescription_out(ConsumerDescription_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    ConsumerDescription_out(ConsumerDescription_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    ConsumerDescription_out& operator=(ConsumerDescription* _p) {
      _ptr = _p;
      return *this;
    }
    operator ConsumerDescription_ptr&() { return _ptr; }
    ConsumerDescription_ptr& ptr() { return _ptr; }
    ConsumerDescription* operator->() { return _ptr; }
  };

  #endif	// Components_ConsumerDescription_var_
  // idl valuetype: Components::ConsumerDescription
  class  ConsumerDescription  : public virtual PortDescription  {
    public:
      friend VISostream& _marshal_in(VISostream& _strm, const ConsumerDescription_ptr _obj);
      friend VISistream& _marshal_out(VISistream& _strm, ConsumerDescription_ptr& _obj);
      friend Ostream& _marshal_in(Ostream& _strm, const ConsumerDescription_ptr _obj);
      friend Istream& _marshal_out(Istream& _strm, ConsumerDescription_ptr& _obj);
      friend void _internal_pretty_print(VISostream& _strm, const ConsumerDescription& _obj);
      friend void _internal_pretty_print(VISostream& _strm, const ConsumerDescription* _obj);

      static ConsumerDescription* _downcast(CORBA::ValueBase* vbptr);
      virtual void consumer(EventConsumerBase_ptr _consumer) = 0;
      virtual EventConsumerBase_ptr consumer() const = 0;
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
       ConsumerDescription() {}
        virtual ~ConsumerDescription() {}

      private:
        void operator=(const ConsumerDescription&);
  };
  class  OBV_ConsumerDescription: public virtual ConsumerDescription,
                                  public virtual OBV_PortDescription  {
    public:
      virtual void consumer(EventConsumerBase_ptr _consumer) {
        _obv_consumer = ::Components::EventConsumerBase_var::_duplicate(_consumer);
      }
      virtual EventConsumerBase_ptr consumer() const {
        return _obv_consumer;
      }
    protected:
      OBV_ConsumerDescription();
      virtual ~OBV_ConsumerDescription();

      OBV_ConsumerDescription(EventConsumerBase_ptr _consumer) {
                _obv_consumer = ::Components::EventConsumerBase_var::_duplicate(_consumer);
  
      }
      EventConsumerBase_var _obv_consumer;
  };
  class ConsumerDescription_init : public CORBA::ValueFactoryBase {
  };
  class  ConsumerDescriptions;
  typedef ConsumerDescriptions* ConsumerDescriptions_ptr;

  class  ConsumerDescriptions : private VISResource {
  private:
    ConsumerDescription_ptr *_contents;
    CORBA::ULong _count;
    CORBA::ULong _num_allocated;
    CORBA::Boolean _release_flag;

  public:
    #if defined(MSVCNEWDLL_BUG)
      void *operator new(size_t ts);
      void *operator new(size_t ts, char*, int) {return operator new(ts);}
      void operator delete(void *p);
    #endif // MSVCNEWDLL_BUG

    static ConsumerDescription_ptr *allocbuf(CORBA::ULong _nelems);
    static void freebuf(ConsumerDescription_ptr* _data);
    static void freebuf_elems(ConsumerDescription_ptr *_data, CORBA::ULong _nelems);

    class  Sequence_ConsumerDescriptions {
      friend class ConsumerDescriptions;
       private:
      CORBA::Boolean _release_flag;
      ConsumerDescription_ptr &_ptr;

      void release() {
        if (_release_flag) {
          CORBA::remove_ref(_ptr);
          _ptr = (Components::ConsumerDescription *)NULL;
        }
      }

   public:
      Sequence_ConsumerDescriptions(ConsumerDescription_ptr &_p,
                                    CORBA::Boolean _rel)
       : _ptr(_p), _release_flag(_rel) {}

      Sequence_ConsumerDescriptions(const Sequence_ConsumerDescriptions& _seq)
       : _ptr(_seq._ptr), _release_flag(_seq._release_flag) {}

      Sequence_ConsumerDescriptions& operator=(ConsumerDescription_ptr _p) {
        if (_ptr != _p) {
          release();
          _ptr = _p;
        }
        return *this;
      }

      Sequence_ConsumerDescriptions& operator=(const Sequence_ConsumerDescriptions& _p) {
        if (_ptr != _p._ptr) {
          if (_release_flag) {
            release();
            ::Components::ConsumerDescription_var::_add_ref(_p._ptr);
            _ptr = _p._ptr;
          } else
            _ptr = _p._ptr;
        }
        return *this;
      }

      operator ConsumerDescription_ptr () const { return _ptr; }
      ConsumerDescription_ptr operator ->() const { return _ptr; }
    };

    ConsumerDescriptions(CORBA::ULong _max = 0);
    ConsumerDescriptions(CORBA::ULong max, CORBA::ULong length,
                         ConsumerDescription_ptr *_data,
                         CORBA::Boolean release = (CORBA::Boolean)0);
    ConsumerDescriptions(const ConsumerDescriptions&);
    virtual ~ConsumerDescriptions();

    ConsumerDescriptions& operator=(const ConsumerDescriptions&);
    ConsumerDescriptions& operator=(const ConsumerDescriptions* _obj) {
      assert(_obj);
      return operator=(*_obj);
    }

    CORBA::ULong maximum() const { return _num_allocated; }

    void length(CORBA::ULong _len);
    CORBA::ULong length() const { return _count; }

    void replace(CORBA::ULong _max, CORBA::ULong _len,
                 ConsumerDescription_ptr *_data,
                 CORBA::Boolean _release = (CORBA::Boolean)0);

    ConsumerDescription_ptr *get_buffer(CORBA::Boolean orphan = (CORBA::Boolean) 0);
    const ConsumerDescription_ptr* get_buffer() const;

    Sequence_ConsumerDescriptions operator[](CORBA::ULong _index);
    const Sequence_ConsumerDescriptions operator[](CORBA::ULong _index) const;

    static ConsumerDescriptions *_duplicate(ConsumerDescriptions* _ptr) {
      if (_ptr) _ptr->_ref();
      return _ptr;
    }

    static void _release(ConsumerDescriptions *_ptr) {
      if (_ptr && (_ptr->_deref() == 0L)) 
        delete _ptr;
    }

    friend VISostream& _marshal_in(VISostream&, const ConsumerDescriptions&);
    friend VISistream& _marshal_out(VISistream&, ConsumerDescriptions&);

    friend VISostream& _marshal_in(VISostream& _strm, const ConsumerDescriptions* _obj);
    friend VISistream& _marshal_out(VISistream& _strm, ConsumerDescriptions_ptr& _obj);

    friend void _internal_pretty_print(VISostream&, const ConsumerDescriptions&);

    friend Ostream& _marshal_in(Ostream&, const ConsumerDescriptions&);
    friend Istream& _marshal_out(Istream&, ConsumerDescriptions&);
    friend Istream& _marshal_out(Istream&, ConsumerDescriptions_ptr&);
  };

  class  ConsumerDescriptions_out;

  class  ConsumerDescriptions_var {
    friend class ConsumerDescriptions_out;
     private:
    ConsumerDescriptions *_ptr;
     public:
    ConsumerDescriptions_var() : _ptr((ConsumerDescriptions*)NULL) {}
    ConsumerDescriptions_var(ConsumerDescriptions *_p) : _ptr(_p) {}
    ConsumerDescriptions_var(const ConsumerDescriptions_var& _var) :_ptr(ConsumerDescriptions::_duplicate(_var._ptr)){}
    virtual ~ConsumerDescriptions_var() { ConsumerDescriptions::_release(_ptr); }
    ConsumerDescriptions_var& operator=(ConsumerDescriptions* _p) {
      ConsumerDescriptions::_release(_ptr);
      _ptr = _p;
      return *this;
    }

    ConsumerDescriptions_var& operator=(const ConsumerDescriptions_var& _var) {
      ConsumerDescriptions::_release(_ptr);
      _ptr = ConsumerDescriptions::_duplicate(_var._ptr);
      return *this;
    }
    operator ConsumerDescriptions*() const { return _ptr; }

    ConsumerDescriptions* operator->() { return _ptr; }
    const ConsumerDescriptions* operator->() const { return _ptr; }

    operator ConsumerDescriptions&() { return *_ptr; }
    operator const ConsumerDescriptions&() const { return *_ptr; }

    const ConsumerDescriptions& in() const { return *_ptr; }
    ConsumerDescriptions& inout() { return *_ptr; }
    ConsumerDescriptions_ptr & out() {
      ConsumerDescriptions::_release(_ptr);
      _ptr = (ConsumerDescriptions *)NULL;
      return _ptr;
    }

    ConsumerDescriptions* _retn() {
      ConsumerDescriptions* _tmp_ptr;
      _tmp_ptr = _ptr;
      _ptr = (ConsumerDescriptions *)NULL;
      return _tmp_ptr;
    }
    friend VISostream& _marshal_in(VISostream&, const ConsumerDescriptions_var&);
    friend VISistream& _marshal_out(VISistream&, ConsumerDescriptions_var&);

    ConsumerDescriptions::Sequence_ConsumerDescriptions operator[](CORBA::ULong _index);
    const ConsumerDescriptions::Sequence_ConsumerDescriptions operator[](CORBA::ULong _index) const;

  };
  class  ConsumerDescriptions_out {
   private:
    ConsumerDescriptions_ptr& _ptr;
    static ConsumerDescriptions* _nil() { return (ConsumerDescriptions*)NULL; }

    void operator=(const ConsumerDescriptions_out&);
    void operator=(const ConsumerDescriptions_var&);
     public:
    ConsumerDescriptions_out(const ConsumerDescriptions_out& _o) : _ptr(_o._ptr) {}
    ConsumerDescriptions_out(ConsumerDescriptions_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    ConsumerDescriptions_out(ConsumerDescriptions_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    ConsumerDescriptions_out& operator=(ConsumerDescriptions* _p) {
      _ptr = _p;
      return *this;
    }
    operator ConsumerDescriptions_ptr&() { return _ptr; }
    ConsumerDescriptions_ptr& ptr() { return _ptr; }
  };
  #ifndef Components_EmitterDescription_var_
  #define Components_EmitterDescription_var_

  class  EmitterDescription;

  typedef EmitterDescription* EmitterDescription_ptr;

   VISistream& _marshal_out(VISistream&, EmitterDescription_ptr&);
   VISostream& _marshal_in(VISostream&, const EmitterDescription_ptr);

  class  EmitterDescription_out;
  class  EmitterDescription_var: public CORBA::_var {
    friend class EmitterDescription_out;
     public:
    EmitterDescription_var();
    EmitterDescription_var(EmitterDescription_ptr );
    EmitterDescription_var(const EmitterDescription_var &);

    virtual ~EmitterDescription_var();

    EmitterDescription_var& operator=(EmitterDescription_ptr);
    void operator=(const EmitterDescription_var & _v);

    static void _add_ref(EmitterDescription_ptr );
    static void _remove_ref(EmitterDescription_ptr );

    operator EmitterDescription*() const { return _ptr; }
    EmitterDescription* operator->() { return _ptr; }
    const EmitterDescription* operator->() const { return _ptr; }

    EmitterDescription_ptr in () const { return _ptr; }
    EmitterDescription_ptr& inout () { return _ptr; }
    EmitterDescription_ptr& out();
    EmitterDescription_ptr _retn();

    friend VISistream& _marshal_out(VISistream& _strm, EmitterDescription_var& _var);
    friend VISostream& _marshal_in(VISostream& _strm, const EmitterDescription_var& _var);

    friend Istream& _marshal_out(Istream&, EmitterDescription_var&);
    friend Ostream& _marshal_in(Ostream&, const EmitterDescription_var&);
     private:
    EmitterDescription_ptr _ptr;
  };
  class  EmitterDescription_out {
   private:
    EmitterDescription_ptr& _ptr;
    static EmitterDescription* _nil() { return (EmitterDescription*)NULL; }

    void operator=(const EmitterDescription_out&);
    void operator=(const EmitterDescription_var&);
     public:
    EmitterDescription_out(const EmitterDescription_out& _o) : _ptr(_o._ptr) {}
    EmitterDescription_out(EmitterDescription_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    EmitterDescription_out(EmitterDescription_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    EmitterDescription_out& operator=(EmitterDescription* _p) {
      _ptr = _p;
      return *this;
    }
    operator EmitterDescription_ptr&() { return _ptr; }
    EmitterDescription_ptr& ptr() { return _ptr; }
    EmitterDescription* operator->() { return _ptr; }
  };

  #endif	// Components_EmitterDescription_var_
  // idl valuetype: Components::EmitterDescription
  class  EmitterDescription  : public virtual PortDescription  {
    public:
      friend VISostream& _marshal_in(VISostream& _strm, const EmitterDescription_ptr _obj);
      friend VISistream& _marshal_out(VISistream& _strm, EmitterDescription_ptr& _obj);
      friend Ostream& _marshal_in(Ostream& _strm, const EmitterDescription_ptr _obj);
      friend Istream& _marshal_out(Istream& _strm, EmitterDescription_ptr& _obj);
      friend void _internal_pretty_print(VISostream& _strm, const EmitterDescription& _obj);
      friend void _internal_pretty_print(VISostream& _strm, const EmitterDescription* _obj);

      static EmitterDescription* _downcast(CORBA::ValueBase* vbptr);
      virtual void consumer(EventConsumerBase_ptr _consumer) = 0;
      virtual EventConsumerBase_ptr consumer() const = 0;
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
       EmitterDescription() {}
        virtual ~EmitterDescription() {}

      private:
        void operator=(const EmitterDescription&);
  };
  class  OBV_EmitterDescription: public virtual EmitterDescription,
                                 public virtual OBV_PortDescription  {
    public:
      virtual void consumer(EventConsumerBase_ptr _consumer) {
        _obv_consumer = ::Components::EventConsumerBase_var::_duplicate(_consumer);
      }
      virtual EventConsumerBase_ptr consumer() const {
        return _obv_consumer;
      }
    protected:
      OBV_EmitterDescription();
      virtual ~OBV_EmitterDescription();

      OBV_EmitterDescription(EventConsumerBase_ptr _consumer) {
                _obv_consumer = ::Components::EventConsumerBase_var::_duplicate(_consumer);
  
      }
      EventConsumerBase_var _obv_consumer;
  };
  class EmitterDescription_init : public CORBA::ValueFactoryBase {
  };
  class  EmitterDescriptions;
  typedef EmitterDescriptions* EmitterDescriptions_ptr;

  class  EmitterDescriptions : private VISResource {
  private:
    EmitterDescription_ptr *_contents;
    CORBA::ULong _count;
    CORBA::ULong _num_allocated;
    CORBA::Boolean _release_flag;

  public:
    #if defined(MSVCNEWDLL_BUG)
      void *operator new(size_t ts);
      void *operator new(size_t ts, char*, int) {return operator new(ts);}
      void operator delete(void *p);
    #endif // MSVCNEWDLL_BUG

    static EmitterDescription_ptr *allocbuf(CORBA::ULong _nelems);
    static void freebuf(EmitterDescription_ptr* _data);
    static void freebuf_elems(EmitterDescription_ptr *_data, CORBA::ULong _nelems);

    class  Sequence_EmitterDescriptions {
      friend class EmitterDescriptions;
       private:
      CORBA::Boolean _release_flag;
      EmitterDescription_ptr &_ptr;

      void release() {
        if (_release_flag) {
          CORBA::remove_ref(_ptr);
          _ptr = (Components::EmitterDescription *)NULL;
        }
      }

   public:
      Sequence_EmitterDescriptions(EmitterDescription_ptr &_p,
                                   CORBA::Boolean _rel)
       : _ptr(_p), _release_flag(_rel) {}

      Sequence_EmitterDescriptions(const Sequence_EmitterDescriptions& _seq)
       : _ptr(_seq._ptr), _release_flag(_seq._release_flag) {}

      Sequence_EmitterDescriptions& operator=(EmitterDescription_ptr _p) {
        if (_ptr != _p) {
          release();
          _ptr = _p;
        }
        return *this;
      }

      Sequence_EmitterDescriptions& operator=(const Sequence_EmitterDescriptions& _p) {
        if (_ptr != _p._ptr) {
          if (_release_flag) {
            release();
            ::Components::EmitterDescription_var::_add_ref(_p._ptr);
            _ptr = _p._ptr;
          } else
            _ptr = _p._ptr;
        }
        return *this;
      }

      operator EmitterDescription_ptr () const { return _ptr; }
      EmitterDescription_ptr operator ->() const { return _ptr; }
    };

    EmitterDescriptions(CORBA::ULong _max = 0);
    EmitterDescriptions(CORBA::ULong max, CORBA::ULong length,
                        EmitterDescription_ptr *_data,
                        CORBA::Boolean release = (CORBA::Boolean)0);
    EmitterDescriptions(const EmitterDescriptions&);
    virtual ~EmitterDescriptions();

    EmitterDescriptions& operator=(const EmitterDescriptions&);
    EmitterDescriptions& operator=(const EmitterDescriptions* _obj) {
      assert(_obj);
      return operator=(*_obj);
    }

    CORBA::ULong maximum() const { return _num_allocated; }

    void length(CORBA::ULong _len);
    CORBA::ULong length() const { return _count; }

    void replace(CORBA::ULong _max, CORBA::ULong _len,
                 EmitterDescription_ptr *_data,
                 CORBA::Boolean _release = (CORBA::Boolean)0);

    EmitterDescription_ptr *get_buffer(CORBA::Boolean orphan = (CORBA::Boolean) 0);
    const EmitterDescription_ptr* get_buffer() const;

    Sequence_EmitterDescriptions operator[](CORBA::ULong _index);
    const Sequence_EmitterDescriptions operator[](CORBA::ULong _index) const;

    static EmitterDescriptions *_duplicate(EmitterDescriptions* _ptr) {
      if (_ptr) _ptr->_ref();
      return _ptr;
    }

    static void _release(EmitterDescriptions *_ptr) {
      if (_ptr && (_ptr->_deref() == 0L)) 
        delete _ptr;
    }

    friend VISostream& _marshal_in(VISostream&, const EmitterDescriptions&);
    friend VISistream& _marshal_out(VISistream&, EmitterDescriptions&);

    friend VISostream& _marshal_in(VISostream& _strm, const EmitterDescriptions* _obj);
    friend VISistream& _marshal_out(VISistream& _strm, EmitterDescriptions_ptr& _obj);

    friend void _internal_pretty_print(VISostream&, const EmitterDescriptions&);

    friend Ostream& _marshal_in(Ostream&, const EmitterDescriptions&);
    friend Istream& _marshal_out(Istream&, EmitterDescriptions&);
    friend Istream& _marshal_out(Istream&, EmitterDescriptions_ptr&);
  };

  class  EmitterDescriptions_out;

  class  EmitterDescriptions_var {
    friend class EmitterDescriptions_out;
     private:
    EmitterDescriptions *_ptr;
     public:
    EmitterDescriptions_var() : _ptr((EmitterDescriptions*)NULL) {}
    EmitterDescriptions_var(EmitterDescriptions *_p) : _ptr(_p) {}
    EmitterDescriptions_var(const EmitterDescriptions_var& _var) :_ptr(EmitterDescriptions::_duplicate(_var._ptr)){}
    virtual ~EmitterDescriptions_var() { EmitterDescriptions::_release(_ptr); }
    EmitterDescriptions_var& operator=(EmitterDescriptions* _p) {
      EmitterDescriptions::_release(_ptr);
      _ptr = _p;
      return *this;
    }

    EmitterDescriptions_var& operator=(const EmitterDescriptions_var& _var) {
      EmitterDescriptions::_release(_ptr);
      _ptr = EmitterDescriptions::_duplicate(_var._ptr);
      return *this;
    }
    operator EmitterDescriptions*() const { return _ptr; }

    EmitterDescriptions* operator->() { return _ptr; }
    const EmitterDescriptions* operator->() const { return _ptr; }

    operator EmitterDescriptions&() { return *_ptr; }
    operator const EmitterDescriptions&() const { return *_ptr; }

    const EmitterDescriptions& in() const { return *_ptr; }
    EmitterDescriptions& inout() { return *_ptr; }
    EmitterDescriptions_ptr & out() {
      EmitterDescriptions::_release(_ptr);
      _ptr = (EmitterDescriptions *)NULL;
      return _ptr;
    }

    EmitterDescriptions* _retn() {
      EmitterDescriptions* _tmp_ptr;
      _tmp_ptr = _ptr;
      _ptr = (EmitterDescriptions *)NULL;
      return _tmp_ptr;
    }
    friend VISostream& _marshal_in(VISostream&, const EmitterDescriptions_var&);
    friend VISistream& _marshal_out(VISistream&, EmitterDescriptions_var&);

    EmitterDescriptions::Sequence_EmitterDescriptions operator[](CORBA::ULong _index);
    const EmitterDescriptions::Sequence_EmitterDescriptions operator[](CORBA::ULong _index) const;

  };
  class  EmitterDescriptions_out {
   private:
    EmitterDescriptions_ptr& _ptr;
    static EmitterDescriptions* _nil() { return (EmitterDescriptions*)NULL; }

    void operator=(const EmitterDescriptions_out&);
    void operator=(const EmitterDescriptions_var&);
     public:
    EmitterDescriptions_out(const EmitterDescriptions_out& _o) : _ptr(_o._ptr) {}
    EmitterDescriptions_out(EmitterDescriptions_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    EmitterDescriptions_out(EmitterDescriptions_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    EmitterDescriptions_out& operator=(EmitterDescriptions* _p) {
      _ptr = _p;
      return *this;
    }
    operator EmitterDescriptions_ptr&() { return _ptr; }
    EmitterDescriptions_ptr& ptr() { return _ptr; }
  };
  #ifndef Components_SubscriberDescription_var_
  #define Components_SubscriberDescription_var_

  class  SubscriberDescription;

  typedef SubscriberDescription* SubscriberDescription_ptr;

   VISistream& _marshal_out(VISistream&, SubscriberDescription_ptr&);
   VISostream& _marshal_in(VISostream&, const SubscriberDescription_ptr);

  class  SubscriberDescription_out;
  class  SubscriberDescription_var: public CORBA::_var {
    friend class SubscriberDescription_out;
     public:
    SubscriberDescription_var();
    SubscriberDescription_var(SubscriberDescription_ptr );
    SubscriberDescription_var(const SubscriberDescription_var &);

    virtual ~SubscriberDescription_var();

    SubscriberDescription_var& operator=(SubscriberDescription_ptr);
    void operator=(const SubscriberDescription_var & _v);

    static void _add_ref(SubscriberDescription_ptr );
    static void _remove_ref(SubscriberDescription_ptr );

    operator SubscriberDescription*() const { return _ptr; }
    SubscriberDescription* operator->() { return _ptr; }
    const SubscriberDescription* operator->() const { return _ptr; }

    SubscriberDescription_ptr in () const { return _ptr; }
    SubscriberDescription_ptr& inout () { return _ptr; }
    SubscriberDescription_ptr& out();
    SubscriberDescription_ptr _retn();

    friend VISistream& _marshal_out(VISistream& _strm, SubscriberDescription_var& _var);
    friend VISostream& _marshal_in(VISostream& _strm, const SubscriberDescription_var& _var);

    friend Istream& _marshal_out(Istream&, SubscriberDescription_var&);
    friend Ostream& _marshal_in(Ostream&, const SubscriberDescription_var&);
     private:
    SubscriberDescription_ptr _ptr;
  };
  class  SubscriberDescription_out {
   private:
    SubscriberDescription_ptr& _ptr;
    static SubscriberDescription* _nil() { return (SubscriberDescription*)NULL; }

    void operator=(const SubscriberDescription_out&);
    void operator=(const SubscriberDescription_var&);
     public:
    SubscriberDescription_out(const SubscriberDescription_out& _o) : _ptr(_o._ptr) {}
    SubscriberDescription_out(SubscriberDescription_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    SubscriberDescription_out(SubscriberDescription_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    SubscriberDescription_out& operator=(SubscriberDescription* _p) {
      _ptr = _p;
      return *this;
    }
    operator SubscriberDescription_ptr&() { return _ptr; }
    SubscriberDescription_ptr& ptr() { return _ptr; }
    SubscriberDescription* operator->() { return _ptr; }
  };

  #endif	// Components_SubscriberDescription_var_
  // idl valuetype: Components::SubscriberDescription
  class  SubscriberDescription  : public virtual CORBA::ValueBase  {
    public:
      friend VISostream& _marshal_in(VISostream& _strm, const SubscriberDescription_ptr _obj);
      friend VISistream& _marshal_out(VISistream& _strm, SubscriberDescription_ptr& _obj);
      friend Ostream& _marshal_in(Ostream& _strm, const SubscriberDescription_ptr _obj);
      friend Istream& _marshal_out(Istream& _strm, SubscriberDescription_ptr& _obj);
      friend void _internal_pretty_print(VISostream& _strm, const SubscriberDescription& _obj);
      friend void _internal_pretty_print(VISostream& _strm, const SubscriberDescription* _obj);

      static SubscriberDescription* _downcast(CORBA::ValueBase* vbptr);
      virtual void ck(Cookie_ptr _ck) = 0;
      virtual Cookie_ptr ck() const = 0;
      virtual void consumer(EventConsumerBase_ptr _consumer) = 0;
      virtual EventConsumerBase_ptr consumer() const = 0;
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
       SubscriberDescription() {}
        virtual ~SubscriberDescription() {}

      private:
        void operator=(const SubscriberDescription&);
  };
  class  OBV_SubscriberDescription: public virtual SubscriberDescription  {
    public:
      virtual void ck(Cookie_ptr _ck) {
        ::Components::Cookie_var::_add_ref(_ck);
        _obv_ck = _ck;
      }
      virtual Cookie_ptr ck() const {
        return _obv_ck;
      }
      virtual void consumer(EventConsumerBase_ptr _consumer) {
        _obv_consumer = ::Components::EventConsumerBase_var::_duplicate(_consumer);
      }
      virtual EventConsumerBase_ptr consumer() const {
        return _obv_consumer;
      }
    protected:
      OBV_SubscriberDescription();
      virtual ~OBV_SubscriberDescription();

      OBV_SubscriberDescription(Cookie_ptr _ck, 
                                EventConsumerBase_ptr _consumer) {
                ::Components::Cookie_var::_add_ref(_ck);
        _obv_ck = _ck;
 
                _obv_consumer = ::Components::EventConsumerBase_var::_duplicate(_consumer);
  
      }
      Cookie_var _obv_ck;
      EventConsumerBase_var _obv_consumer;
  };
  class SubscriberDescription_init : public CORBA::ValueFactoryBase {
  };
  class  SubscriberDescriptions;
  typedef SubscriberDescriptions* SubscriberDescriptions_ptr;

  class  SubscriberDescriptions : private VISResource {
  private:
    SubscriberDescription_ptr *_contents;
    CORBA::ULong _count;
    CORBA::ULong _num_allocated;
    CORBA::Boolean _release_flag;

  public:
    #if defined(MSVCNEWDLL_BUG)
      void *operator new(size_t ts);
      void *operator new(size_t ts, char*, int) {return operator new(ts);}
      void operator delete(void *p);
    #endif // MSVCNEWDLL_BUG

    static SubscriberDescription_ptr *allocbuf(CORBA::ULong _nelems);
    static void freebuf(SubscriberDescription_ptr* _data);
    static void freebuf_elems(SubscriberDescription_ptr *_data, CORBA::ULong _nelems);

    class  Sequence_SubscriberDescriptions {
      friend class SubscriberDescriptions;
       private:
      CORBA::Boolean _release_flag;
      SubscriberDescription_ptr &_ptr;

      void release() {
        if (_release_flag) {
          CORBA::remove_ref(_ptr);
          _ptr = (Components::SubscriberDescription *)NULL;
        }
      }

   public:
      Sequence_SubscriberDescriptions(SubscriberDescription_ptr &_p,
                                      CORBA::Boolean _rel)
       : _ptr(_p), _release_flag(_rel) {}

      Sequence_SubscriberDescriptions(const Sequence_SubscriberDescriptions& _seq)
       : _ptr(_seq._ptr), _release_flag(_seq._release_flag) {}

      Sequence_SubscriberDescriptions& operator=(SubscriberDescription_ptr _p) {
        if (_ptr != _p) {
          release();
          _ptr = _p;
        }
        return *this;
      }

      Sequence_SubscriberDescriptions& operator=(const Sequence_SubscriberDescriptions& _p) {
        if (_ptr != _p._ptr) {
          if (_release_flag) {
            release();
            ::Components::SubscriberDescription_var::_add_ref(_p._ptr);
            _ptr = _p._ptr;
          } else
            _ptr = _p._ptr;
        }
        return *this;
      }

      operator SubscriberDescription_ptr () const { return _ptr; }
      SubscriberDescription_ptr operator ->() const { return _ptr; }
    };

    SubscriberDescriptions(CORBA::ULong _max = 0);
    SubscriberDescriptions(CORBA::ULong max, CORBA::ULong length,
                           SubscriberDescription_ptr *_data,
                           CORBA::Boolean release = (CORBA::Boolean)0);
    SubscriberDescriptions(const SubscriberDescriptions&);
    virtual ~SubscriberDescriptions();

    SubscriberDescriptions& operator=(const SubscriberDescriptions&);
    SubscriberDescriptions& operator=(const SubscriberDescriptions* _obj) {
      assert(_obj);
      return operator=(*_obj);
    }

    CORBA::ULong maximum() const { return _num_allocated; }

    void length(CORBA::ULong _len);
    CORBA::ULong length() const { return _count; }

    void replace(CORBA::ULong _max, CORBA::ULong _len,
                 SubscriberDescription_ptr *_data,
                 CORBA::Boolean _release = (CORBA::Boolean)0);

    SubscriberDescription_ptr *get_buffer(CORBA::Boolean orphan = (CORBA::Boolean) 0);
    const SubscriberDescription_ptr* get_buffer() const;

    Sequence_SubscriberDescriptions operator[](CORBA::ULong _index);
    const Sequence_SubscriberDescriptions operator[](CORBA::ULong _index) const;

    static SubscriberDescriptions *_duplicate(SubscriberDescriptions* _ptr) {
      if (_ptr) _ptr->_ref();
      return _ptr;
    }

    static void _release(SubscriberDescriptions *_ptr) {
      if (_ptr && (_ptr->_deref() == 0L)) 
        delete _ptr;
    }

    friend VISostream& _marshal_in(VISostream&, const SubscriberDescriptions&);
    friend VISistream& _marshal_out(VISistream&, SubscriberDescriptions&);

    friend VISostream& _marshal_in(VISostream& _strm, const SubscriberDescriptions* _obj);
    friend VISistream& _marshal_out(VISistream& _strm, SubscriberDescriptions_ptr& _obj);

    friend void _internal_pretty_print(VISostream&, const SubscriberDescriptions&);

    friend Ostream& _marshal_in(Ostream&, const SubscriberDescriptions&);
    friend Istream& _marshal_out(Istream&, SubscriberDescriptions&);
    friend Istream& _marshal_out(Istream&, SubscriberDescriptions_ptr&);
  };

  class  SubscriberDescriptions_out;

  class  SubscriberDescriptions_var {
    friend class SubscriberDescriptions_out;
     private:
    SubscriberDescriptions *_ptr;
     public:
    SubscriberDescriptions_var() : _ptr((SubscriberDescriptions*)NULL) {}
    SubscriberDescriptions_var(SubscriberDescriptions *_p) : _ptr(_p) {}
    SubscriberDescriptions_var(const SubscriberDescriptions_var& _var) :_ptr(SubscriberDescriptions::_duplicate(_var._ptr)){}
    virtual ~SubscriberDescriptions_var() { SubscriberDescriptions::_release(_ptr); }
    SubscriberDescriptions_var& operator=(SubscriberDescriptions* _p) {
      SubscriberDescriptions::_release(_ptr);
      _ptr = _p;
      return *this;
    }

    SubscriberDescriptions_var& operator=(const SubscriberDescriptions_var& _var) {
      SubscriberDescriptions::_release(_ptr);
      _ptr = SubscriberDescriptions::_duplicate(_var._ptr);
      return *this;
    }
    operator SubscriberDescriptions*() const { return _ptr; }

    SubscriberDescriptions* operator->() { return _ptr; }
    const SubscriberDescriptions* operator->() const { return _ptr; }

    operator SubscriberDescriptions&() { return *_ptr; }
    operator const SubscriberDescriptions&() const { return *_ptr; }

    const SubscriberDescriptions& in() const { return *_ptr; }
    SubscriberDescriptions& inout() { return *_ptr; }
    SubscriberDescriptions_ptr & out() {
      SubscriberDescriptions::_release(_ptr);
      _ptr = (SubscriberDescriptions *)NULL;
      return _ptr;
    }

    SubscriberDescriptions* _retn() {
      SubscriberDescriptions* _tmp_ptr;
      _tmp_ptr = _ptr;
      _ptr = (SubscriberDescriptions *)NULL;
      return _tmp_ptr;
    }
    friend VISostream& _marshal_in(VISostream&, const SubscriberDescriptions_var&);
    friend VISistream& _marshal_out(VISistream&, SubscriberDescriptions_var&);

    SubscriberDescriptions::Sequence_SubscriberDescriptions operator[](CORBA::ULong _index);
    const SubscriberDescriptions::Sequence_SubscriberDescriptions operator[](CORBA::ULong _index) const;

  };
  class  SubscriberDescriptions_out {
   private:
    SubscriberDescriptions_ptr& _ptr;
    static SubscriberDescriptions* _nil() { return (SubscriberDescriptions*)NULL; }

    void operator=(const SubscriberDescriptions_out&);
    void operator=(const SubscriberDescriptions_var&);
     public:
    SubscriberDescriptions_out(const SubscriberDescriptions_out& _o) : _ptr(_o._ptr) {}
    SubscriberDescriptions_out(SubscriberDescriptions_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    SubscriberDescriptions_out(SubscriberDescriptions_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    SubscriberDescriptions_out& operator=(SubscriberDescriptions* _p) {
      _ptr = _p;
      return *this;
    }
    operator SubscriberDescriptions_ptr&() { return _ptr; }
    SubscriberDescriptions_ptr& ptr() { return _ptr; }
  };
  #ifndef Components_PublisherDescription_var_
  #define Components_PublisherDescription_var_

  class  PublisherDescription;

  typedef PublisherDescription* PublisherDescription_ptr;

   VISistream& _marshal_out(VISistream&, PublisherDescription_ptr&);
   VISostream& _marshal_in(VISostream&, const PublisherDescription_ptr);

  class  PublisherDescription_out;
  class  PublisherDescription_var: public CORBA::_var {
    friend class PublisherDescription_out;
     public:
    PublisherDescription_var();
    PublisherDescription_var(PublisherDescription_ptr );
    PublisherDescription_var(const PublisherDescription_var &);

    virtual ~PublisherDescription_var();

    PublisherDescription_var& operator=(PublisherDescription_ptr);
    void operator=(const PublisherDescription_var & _v);

    static void _add_ref(PublisherDescription_ptr );
    static void _remove_ref(PublisherDescription_ptr );

    operator PublisherDescription*() const { return _ptr; }
    PublisherDescription* operator->() { return _ptr; }
    const PublisherDescription* operator->() const { return _ptr; }

    PublisherDescription_ptr in () const { return _ptr; }
    PublisherDescription_ptr& inout () { return _ptr; }
    PublisherDescription_ptr& out();
    PublisherDescription_ptr _retn();

    friend VISistream& _marshal_out(VISistream& _strm, PublisherDescription_var& _var);
    friend VISostream& _marshal_in(VISostream& _strm, const PublisherDescription_var& _var);

    friend Istream& _marshal_out(Istream&, PublisherDescription_var&);
    friend Ostream& _marshal_in(Ostream&, const PublisherDescription_var&);
     private:
    PublisherDescription_ptr _ptr;
  };
  class  PublisherDescription_out {
   private:
    PublisherDescription_ptr& _ptr;
    static PublisherDescription* _nil() { return (PublisherDescription*)NULL; }

    void operator=(const PublisherDescription_out&);
    void operator=(const PublisherDescription_var&);
     public:
    PublisherDescription_out(const PublisherDescription_out& _o) : _ptr(_o._ptr) {}
    PublisherDescription_out(PublisherDescription_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    PublisherDescription_out(PublisherDescription_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    PublisherDescription_out& operator=(PublisherDescription* _p) {
      _ptr = _p;
      return *this;
    }
    operator PublisherDescription_ptr&() { return _ptr; }
    PublisherDescription_ptr& ptr() { return _ptr; }
    PublisherDescription* operator->() { return _ptr; }
  };

  #endif	// Components_PublisherDescription_var_
  // idl valuetype: Components::PublisherDescription
  class  PublisherDescription  : public virtual PortDescription  {
    public:
      friend VISostream& _marshal_in(VISostream& _strm, const PublisherDescription_ptr _obj);
      friend VISistream& _marshal_out(VISistream& _strm, PublisherDescription_ptr& _obj);
      friend Ostream& _marshal_in(Ostream& _strm, const PublisherDescription_ptr _obj);
      friend Istream& _marshal_out(Istream& _strm, PublisherDescription_ptr& _obj);
      friend void _internal_pretty_print(VISostream& _strm, const PublisherDescription& _obj);
      friend void _internal_pretty_print(VISostream& _strm, const PublisherDescription* _obj);

      static PublisherDescription* _downcast(CORBA::ValueBase* vbptr);
      virtual void consumers(const SubscriberDescriptions& _consumers) = 0;
      virtual const SubscriberDescriptions& consumers() const = 0;
      virtual SubscriberDescriptions& consumers() = 0;
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
       PublisherDescription() {}
        virtual ~PublisherDescription() {}

      private:
        void operator=(const PublisherDescription&);
  };
  class  OBV_PublisherDescription: public virtual PublisherDescription,
                                   public virtual OBV_PortDescription  {
    public:
      virtual void consumers(const SubscriberDescriptions& _consumers) {
        _obv_consumers = _consumers;
      }
      virtual const SubscriberDescriptions& consumers() const {
        return _obv_consumers;
      }
      virtual SubscriberDescriptions& consumers() {
        return _obv_consumers;
      }
    protected:
      OBV_PublisherDescription();
      virtual ~OBV_PublisherDescription();

      OBV_PublisherDescription(const SubscriberDescriptions& _consumers) {
                _obv_consumers = _consumers;
  
      }
      SubscriberDescriptions _obv_consumers;
  };
  class PublisherDescription_init : public CORBA::ValueFactoryBase {
  };
  class  PublisherDescriptions;
  typedef PublisherDescriptions* PublisherDescriptions_ptr;

  class  PublisherDescriptions : private VISResource {
  private:
    PublisherDescription_ptr *_contents;
    CORBA::ULong _count;
    CORBA::ULong _num_allocated;
    CORBA::Boolean _release_flag;

  public:
    #if defined(MSVCNEWDLL_BUG)
      void *operator new(size_t ts);
      void *operator new(size_t ts, char*, int) {return operator new(ts);}
      void operator delete(void *p);
    #endif // MSVCNEWDLL_BUG

    static PublisherDescription_ptr *allocbuf(CORBA::ULong _nelems);
    static void freebuf(PublisherDescription_ptr* _data);
    static void freebuf_elems(PublisherDescription_ptr *_data, CORBA::ULong _nelems);

    class  Sequence_PublisherDescriptions {
      friend class PublisherDescriptions;
       private:
      CORBA::Boolean _release_flag;
      PublisherDescription_ptr &_ptr;

      void release() {
        if (_release_flag) {
          CORBA::remove_ref(_ptr);
          _ptr = (Components::PublisherDescription *)NULL;
        }
      }

   public:
      Sequence_PublisherDescriptions(PublisherDescription_ptr &_p,
                                     CORBA::Boolean _rel)
       : _ptr(_p), _release_flag(_rel) {}

      Sequence_PublisherDescriptions(const Sequence_PublisherDescriptions& _seq)
       : _ptr(_seq._ptr), _release_flag(_seq._release_flag) {}

      Sequence_PublisherDescriptions& operator=(PublisherDescription_ptr _p) {
        if (_ptr != _p) {
          release();
          _ptr = _p;
        }
        return *this;
      }

      Sequence_PublisherDescriptions& operator=(const Sequence_PublisherDescriptions& _p) {
        if (_ptr != _p._ptr) {
          if (_release_flag) {
            release();
            ::Components::PublisherDescription_var::_add_ref(_p._ptr);
            _ptr = _p._ptr;
          } else
            _ptr = _p._ptr;
        }
        return *this;
      }

      operator PublisherDescription_ptr () const { return _ptr; }
      PublisherDescription_ptr operator ->() const { return _ptr; }
    };

    PublisherDescriptions(CORBA::ULong _max = 0);
    PublisherDescriptions(CORBA::ULong max, CORBA::ULong length,
                          PublisherDescription_ptr *_data,
                          CORBA::Boolean release = (CORBA::Boolean)0);
    PublisherDescriptions(const PublisherDescriptions&);
    virtual ~PublisherDescriptions();

    PublisherDescriptions& operator=(const PublisherDescriptions&);
    PublisherDescriptions& operator=(const PublisherDescriptions* _obj) {
      assert(_obj);
      return operator=(*_obj);
    }

    CORBA::ULong maximum() const { return _num_allocated; }

    void length(CORBA::ULong _len);
    CORBA::ULong length() const { return _count; }

    void replace(CORBA::ULong _max, CORBA::ULong _len,
                 PublisherDescription_ptr *_data,
                 CORBA::Boolean _release = (CORBA::Boolean)0);

    PublisherDescription_ptr *get_buffer(CORBA::Boolean orphan = (CORBA::Boolean) 0);
    const PublisherDescription_ptr* get_buffer() const;

    Sequence_PublisherDescriptions operator[](CORBA::ULong _index);
    const Sequence_PublisherDescriptions operator[](CORBA::ULong _index) const;

    static PublisherDescriptions *_duplicate(PublisherDescriptions* _ptr) {
      if (_ptr) _ptr->_ref();
      return _ptr;
    }

    static void _release(PublisherDescriptions *_ptr) {
      if (_ptr && (_ptr->_deref() == 0L)) 
        delete _ptr;
    }

    friend VISostream& _marshal_in(VISostream&, const PublisherDescriptions&);
    friend VISistream& _marshal_out(VISistream&, PublisherDescriptions&);

    friend VISostream& _marshal_in(VISostream& _strm, const PublisherDescriptions* _obj);
    friend VISistream& _marshal_out(VISistream& _strm, PublisherDescriptions_ptr& _obj);

    friend void _internal_pretty_print(VISostream&, const PublisherDescriptions&);

    friend Ostream& _marshal_in(Ostream&, const PublisherDescriptions&);
    friend Istream& _marshal_out(Istream&, PublisherDescriptions&);
    friend Istream& _marshal_out(Istream&, PublisherDescriptions_ptr&);
  };

  class  PublisherDescriptions_out;

  class  PublisherDescriptions_var {
    friend class PublisherDescriptions_out;
     private:
    PublisherDescriptions *_ptr;
     public:
    PublisherDescriptions_var() : _ptr((PublisherDescriptions*)NULL) {}
    PublisherDescriptions_var(PublisherDescriptions *_p) : _ptr(_p) {}
    PublisherDescriptions_var(const PublisherDescriptions_var& _var) :_ptr(PublisherDescriptions::_duplicate(_var._ptr)){}
    virtual ~PublisherDescriptions_var() { PublisherDescriptions::_release(_ptr); }
    PublisherDescriptions_var& operator=(PublisherDescriptions* _p) {
      PublisherDescriptions::_release(_ptr);
      _ptr = _p;
      return *this;
    }

    PublisherDescriptions_var& operator=(const PublisherDescriptions_var& _var) {
      PublisherDescriptions::_release(_ptr);
      _ptr = PublisherDescriptions::_duplicate(_var._ptr);
      return *this;
    }
    operator PublisherDescriptions*() const { return _ptr; }

    PublisherDescriptions* operator->() { return _ptr; }
    const PublisherDescriptions* operator->() const { return _ptr; }

    operator PublisherDescriptions&() { return *_ptr; }
    operator const PublisherDescriptions&() const { return *_ptr; }

    const PublisherDescriptions& in() const { return *_ptr; }
    PublisherDescriptions& inout() { return *_ptr; }
    PublisherDescriptions_ptr & out() {
      PublisherDescriptions::_release(_ptr);
      _ptr = (PublisherDescriptions *)NULL;
      return _ptr;
    }

    PublisherDescriptions* _retn() {
      PublisherDescriptions* _tmp_ptr;
      _tmp_ptr = _ptr;
      _ptr = (PublisherDescriptions *)NULL;
      return _tmp_ptr;
    }
    friend VISostream& _marshal_in(VISostream&, const PublisherDescriptions_var&);
    friend VISistream& _marshal_out(VISistream&, PublisherDescriptions_var&);

    PublisherDescriptions::Sequence_PublisherDescriptions operator[](CORBA::ULong _index);
    const PublisherDescriptions::Sequence_PublisherDescriptions operator[](CORBA::ULong _index) const;

  };
  class  PublisherDescriptions_out {
   private:
    PublisherDescriptions_ptr& _ptr;
    static PublisherDescriptions* _nil() { return (PublisherDescriptions*)NULL; }

    void operator=(const PublisherDescriptions_out&);
    void operator=(const PublisherDescriptions_var&);
     public:
    PublisherDescriptions_out(const PublisherDescriptions_out& _o) : _ptr(_o._ptr) {}
    PublisherDescriptions_out(PublisherDescriptions_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    PublisherDescriptions_out(PublisherDescriptions_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    PublisherDescriptions_out& operator=(PublisherDescriptions* _p) {
      _ptr = _p;
      return *this;
    }
    operator PublisherDescriptions_ptr&() { return _ptr; }
    PublisherDescriptions_ptr& ptr() { return _ptr; }
  };
  #ifndef Components_Events_var_
  #define Components_Events_var_

  class  Events;

  typedef Events* Events_ptr;
  typedef Events_ptr EventsRef;
   VISistream& _marshal_out(VISistream&, Events_ptr&);
   VISostream& _marshal_in(VISostream&, const Events_ptr);
  class  Events_out;

  class  Events_var : public CORBA::_var {
  friend class Events_out;
   private:
    Events_ptr _ptr;

   public:
    Events_var();
    Events_var(Events_ptr);
    Events_var(const Events_var &);
    virtual ~Events_var();

    static Events_ptr _duplicate(Events_ptr);
    static void _release(Events_ptr);

    Events_var& operator=(const Events_var& _var);
    Events_var& operator=(Events_ptr);

    operator Events*() const { return _ptr; }
    Events* operator->() const { return _ptr; }

    Events_ptr in() const { return _ptr; }
    Events_ptr& inout() { return _ptr; }
    Events_ptr& out();
    Events_ptr _retn();

    friend VISostream& _marshal_in(VISostream&, const Events_var&);
    friend VISistream& _marshal_out(VISistream&, Events_var&);

    friend Istream& _marshal_out(Istream&, Events_var&);
    friend Ostream& _marshal_in(Ostream&, const Events_var&);
  };
  class  Events_out {
   private:
    Events_ptr& _ptr;
    static Events* _nil() { return (Events*)NULL; }

    void operator=(const Events_out&);
    void operator=(const Events_var&);
     public:
    Events_out(const Events_out& _o) : _ptr(_o._ptr) {}
    Events_out(Events_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    Events_out(Events_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    Events_out& operator=(Events* _p) {
      _ptr = _p;
      return *this;
    }
    operator Events_ptr&() { return _ptr; }
    Events_ptr& ptr() { return _ptr; }
    Events* operator->() { return _ptr; }
  };

  #endif // Components_Events_var_

  // idl interface: Components::Events
  class  Events : public virtual CORBA_Object {
   private:
    static const CORBA::TypeInfo _class_info;
    void operator=(const Events&) {}

    protected:
    Events() {}
    Events(const Events&) {}

   public:
    virtual ~Events() {}
    static  const CORBA::TypeInfo *_desc();
    virtual const CORBA::TypeInfo *_type_info() const;
    virtual void *_safe_narrow(const CORBA::TypeInfo& ) const;
    static CORBA::Object*_factory();
    Events_ptr _this();
    static Events_ptr _duplicate(Events_ptr _obj) {
      if (_obj) _obj->_ref();
      return _obj;
    }
    static Events_ptr _nil() { return (Events_ptr)NULL; }
    static Events_ptr _narrow(CORBA::Object* _obj);
    static Events_ptr _clone(Events_ptr _obj) {
      CORBA::Object_var _obj_var(CORBA_Object::_clone(_obj));
      #if defined(_HPCC_BUG)
        return _narrow(_obj_var.operator CORBA::Object_ptr());
      #else
        return _narrow(_obj_var);
      #endif  // _HPCC_BUG
    }
    static Events_ptr _bind(const char *_object_name = (const char*)NULL,
                            const char *_host_name = (const char*)NULL,
                            const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                            CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    static Events_ptr _bind(const char *_poa_name,
                            const CORBA::OctetSequence& _id,
                            const char *_host_name = (const char*)NULL,
                            const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                            CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    virtual EventConsumerBase_ptr get_consumer(const char* _sink_name);
    virtual Cookie_ptr subscribe(const char* _publisher_name,
                                 EventConsumerBase_ptr _subscriber);
    virtual void unsubscribe(const char* _publisher_name, Cookie_ptr _ck);
    virtual void connect_consumer(const char* _emitter_name,
                                  EventConsumerBase_ptr _consumer);
    virtual EventConsumerBase_ptr disconnect_consumer(const char* _source_name);
    virtual ConsumerDescriptions* get_all_consumers();
    virtual ConsumerDescriptions* get_named_consumers(const NameList& _names);
    virtual EmitterDescriptions* get_all_emitters();
    virtual EmitterDescriptions* get_named_emitters(const NameList& _names);
    virtual PublisherDescriptions* get_all_publishers();
    virtual PublisherDescriptions* get_named_publishers(const NameList& _names);

    friend VISostream& _marshal_in(VISostream& _strm, const Events_ptr _obj);
    friend VISistream& _marshal_out(VISistream& _strm, Events_ptr& _obj);
    friend Ostream& _marshal_in(Ostream& _strm, const Events_ptr _obj);
    friend Istream& _marshal_out(Istream& _strm, Events_ptr& _obj);
  };

  class Events_ops;
  typedef Events_ops* Events_ops_ptr;

  class  Events_ops  {
   public:
    Events_ops() {}
    virtual ~Events_ops () {}

    virtual EventConsumerBase_ptr get_consumer(const char* _sink_name) = 0;
    virtual Cookie_ptr subscribe(const char* _publisher_name,
                                 EventConsumerBase_ptr _subscriber) = 0;
    virtual void unsubscribe(const char* _publisher_name, Cookie_ptr _ck) = 0;
    virtual void connect_consumer(const char* _emitter_name,
                                  EventConsumerBase_ptr _consumer) = 0;
    virtual EventConsumerBase_ptr disconnect_consumer(const char* _source_name) = 0;
    virtual ConsumerDescriptions* get_all_consumers() = 0;
    virtual ConsumerDescriptions* get_named_consumers(const NameList& _names) = 0;
    virtual EmitterDescriptions* get_all_emitters() = 0;
    virtual EmitterDescriptions* get_named_emitters(const NameList& _names) = 0;
    virtual PublisherDescriptions* get_all_publishers() = 0;
    virtual PublisherDescriptions* get_named_publishers(const NameList& _names) = 0;

    static const VISOps_Info *_desc();
    static Events_ops_ptr _nil() { return (Events_ops_ptr)NULL; }
    static Events_ops_ptr _downcast(PortableServer::ServantBase* _servant);
  protected:
    static const VISOps_Info _ops_info;
  };

  class  NoKeyAvailable : public CORBA_UserException {
   public:

    #if defined(MSVCNEWDLL_BUG)
      void *operator new(size_t ts);
      void *operator new(size_t ts, char*, int) {return operator new(ts);}
      void operator delete(void *p);
    #endif // MSVCNEWDLL_BUG

    NoKeyAvailable();
    virtual ~NoKeyAvailable();

    
    static CORBA::Exception *_factory() { return new NoKeyAvailable(); }
    virtual const CORBA_Exception::Description& _desc() const;
    static NoKeyAvailable* _downcast(CORBA::Exception *_exc);
    static const NoKeyAvailable* _downcast(const CORBA::Exception *_exc);
    CORBA::Exception *_deep_copy() { return new NoKeyAvailable(*this); }
  #ifdef AIXV3
    void _raise() const { VISTHROW_INST((NoKeyAvailable*)this) }
  #else
    void _raise() const { VISTHROW_INST(this) }
  #endif

    friend void _internal_pretty_print(VISostream&, const NoKeyAvailable&);
    void _write(VISostream&) const;
    void _copy(VISistream&);

    friend VISistream& _marshal_out(VISistream& _strm, NoKeyAvailable& _e);
    static const CORBA_Exception::Description _description;
  };

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
  #ifndef Components_ComponentPortDescription_var_
  #define Components_ComponentPortDescription_var_

  class  ComponentPortDescription;

  typedef ComponentPortDescription* ComponentPortDescription_ptr;

   VISistream& _marshal_out(VISistream&, ComponentPortDescription_ptr&);
   VISostream& _marshal_in(VISostream&, const ComponentPortDescription_ptr);

  class  ComponentPortDescription_out;
  class  ComponentPortDescription_var: public CORBA::_var {
    friend class ComponentPortDescription_out;
     public:
    ComponentPortDescription_var();
    ComponentPortDescription_var(ComponentPortDescription_ptr );
    ComponentPortDescription_var(const ComponentPortDescription_var &);

    virtual ~ComponentPortDescription_var();

    ComponentPortDescription_var& operator=(ComponentPortDescription_ptr);
    void operator=(const ComponentPortDescription_var & _v);

    static void _add_ref(ComponentPortDescription_ptr );
    static void _remove_ref(ComponentPortDescription_ptr );

    operator ComponentPortDescription*() const { return _ptr; }
    ComponentPortDescription* operator->() { return _ptr; }
    const ComponentPortDescription* operator->() const { return _ptr; }

    ComponentPortDescription_ptr in () const { return _ptr; }
    ComponentPortDescription_ptr& inout () { return _ptr; }
    ComponentPortDescription_ptr& out();
    ComponentPortDescription_ptr _retn();

    friend VISistream& _marshal_out(VISistream& _strm, ComponentPortDescription_var& _var);
    friend VISostream& _marshal_in(VISostream& _strm, const ComponentPortDescription_var& _var);

    friend Istream& _marshal_out(Istream&, ComponentPortDescription_var&);
    friend Ostream& _marshal_in(Ostream&, const ComponentPortDescription_var&);
     private:
    ComponentPortDescription_ptr _ptr;
  };
  class  ComponentPortDescription_out {
   private:
    ComponentPortDescription_ptr& _ptr;
    static ComponentPortDescription* _nil() { return (ComponentPortDescription*)NULL; }

    void operator=(const ComponentPortDescription_out&);
    void operator=(const ComponentPortDescription_var&);
     public:
    ComponentPortDescription_out(const ComponentPortDescription_out& _o) : _ptr(_o._ptr) {}
    ComponentPortDescription_out(ComponentPortDescription_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    ComponentPortDescription_out(ComponentPortDescription_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    ComponentPortDescription_out& operator=(ComponentPortDescription* _p) {
      _ptr = _p;
      return *this;
    }
    operator ComponentPortDescription_ptr&() { return _ptr; }
    ComponentPortDescription_ptr& ptr() { return _ptr; }
    ComponentPortDescription* operator->() { return _ptr; }
  };

  #endif	// Components_ComponentPortDescription_var_
  // idl valuetype: Components::ComponentPortDescription
  class  ComponentPortDescription  : public virtual CORBA::ValueBase  {
    public:
      friend VISostream& _marshal_in(VISostream& _strm, const ComponentPortDescription_ptr _obj);
      friend VISistream& _marshal_out(VISistream& _strm, ComponentPortDescription_ptr& _obj);
      friend Ostream& _marshal_in(Ostream& _strm, const ComponentPortDescription_ptr _obj);
      friend Istream& _marshal_out(Istream& _strm, ComponentPortDescription_ptr& _obj);
      friend void _internal_pretty_print(VISostream& _strm, const ComponentPortDescription& _obj);
      friend void _internal_pretty_print(VISostream& _strm, const ComponentPortDescription* _obj);

      static ComponentPortDescription* _downcast(CORBA::ValueBase* vbptr);
      virtual void facets(const FacetDescriptions& _facets) = 0;
      virtual const FacetDescriptions& facets() const = 0;
      virtual FacetDescriptions& facets() = 0;
      virtual void receptacles(const ReceptacleDescriptions& _receptacles) = 0;
      virtual const ReceptacleDescriptions& receptacles() const = 0;
      virtual ReceptacleDescriptions& receptacles() = 0;
      virtual void consumers(const ConsumerDescriptions& _consumers) = 0;
      virtual const ConsumerDescriptions& consumers() const = 0;
      virtual ConsumerDescriptions& consumers() = 0;
      virtual void emitters(const EmitterDescriptions& _emitters) = 0;
      virtual const EmitterDescriptions& emitters() const = 0;
      virtual EmitterDescriptions& emitters() = 0;
      virtual void publishers(const PublisherDescriptions& _publishers) = 0;
      virtual const PublisherDescriptions& publishers() const = 0;
      virtual PublisherDescriptions& publishers() = 0;
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
       ComponentPortDescription() {}
        virtual ~ComponentPortDescription() {}

      private:
        void operator=(const ComponentPortDescription&);
  };
  class  OBV_ComponentPortDescription: public virtual ComponentPortDescription  {
    public:
      virtual void facets(const FacetDescriptions& _facets) {
        _obv_facets = _facets;
      }
      virtual const FacetDescriptions& facets() const {
        return _obv_facets;
      }
      virtual FacetDescriptions& facets() {
        return _obv_facets;
      }
      virtual void receptacles(const ReceptacleDescriptions& _receptacles) {
        _obv_receptacles = _receptacles;
      }
      virtual const ReceptacleDescriptions& receptacles() const {
        return _obv_receptacles;
      }
      virtual ReceptacleDescriptions& receptacles() {
        return _obv_receptacles;
      }
      virtual void consumers(const ConsumerDescriptions& _consumers) {
        _obv_consumers = _consumers;
      }
      virtual const ConsumerDescriptions& consumers() const {
        return _obv_consumers;
      }
      virtual ConsumerDescriptions& consumers() {
        return _obv_consumers;
      }
      virtual void emitters(const EmitterDescriptions& _emitters) {
        _obv_emitters = _emitters;
      }
      virtual const EmitterDescriptions& emitters() const {
        return _obv_emitters;
      }
      virtual EmitterDescriptions& emitters() {
        return _obv_emitters;
      }
      virtual void publishers(const PublisherDescriptions& _publishers) {
        _obv_publishers = _publishers;
      }
      virtual const PublisherDescriptions& publishers() const {
        return _obv_publishers;
      }
      virtual PublisherDescriptions& publishers() {
        return _obv_publishers;
      }
    protected:
      OBV_ComponentPortDescription();
      virtual ~OBV_ComponentPortDescription();

      OBV_ComponentPortDescription(const FacetDescriptions& _facets, 
                                   const ReceptacleDescriptions& _receptacles, 
                                   const ConsumerDescriptions& _consumers, 
                                   const EmitterDescriptions& _emitters, 
                                   const PublisherDescriptions& _publishers) {
                _obv_facets = _facets;
 
                _obv_receptacles = _receptacles;
 
                _obv_consumers = _consumers;
 
                _obv_emitters = _emitters;
 
                _obv_publishers = _publishers;
  
      }
      FacetDescriptions _obv_facets;
      ReceptacleDescriptions _obv_receptacles;
      ConsumerDescriptions _obv_consumers;
      EmitterDescriptions _obv_emitters;
      PublisherDescriptions _obv_publishers;
  };
  class ComponentPortDescription_init : public CORBA::ValueFactoryBase {
  };

  // idl interface: Components::CCMObject
  class  CCMObject : public virtual Navigation, public virtual Receptacles, public virtual Events, public virtual CORBA_Object {
   private:
    static const CORBA::TypeInfo _class_info;
    void operator=(const CCMObject&) {}

    protected:
    CCMObject() {}
    CCMObject(const CCMObject&) {}

   public:
    virtual ~CCMObject() {}
    static  const CORBA::TypeInfo *_desc();
    virtual const CORBA::TypeInfo *_type_info() const;
    virtual void *_safe_narrow(const CORBA::TypeInfo& ) const;
    static CORBA::Object*_factory();
    CCMObject_ptr _this();
    static CCMObject_ptr _duplicate(CCMObject_ptr _obj) {
      if (_obj) _obj->_ref();
      return _obj;
    }
    static CCMObject_ptr _nil() { return (CCMObject_ptr)NULL; }
    static CCMObject_ptr _narrow(CORBA::Object* _obj);
    static CCMObject_ptr _clone(CCMObject_ptr _obj) {
      CORBA::Object_var _obj_var(CORBA_Object::_clone(_obj));
      #if defined(_HPCC_BUG)
        return _narrow(_obj_var.operator CORBA::Object_ptr());
      #else
        return _narrow(_obj_var);
      #endif  // _HPCC_BUG
    }
    static CCMObject_ptr _bind(const char *_object_name = (const char*)NULL,
                               const char *_host_name = (const char*)NULL,
                               const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                               CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    static CCMObject_ptr _bind(const char *_poa_name,
                               const CORBA::OctetSequence& _id,
                               const char *_host_name = (const char*)NULL,
                               const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                               CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    virtual ::CORBA::IRObject_ptr get_component_def();
    virtual CCMHome_ptr get_ccm_home();
    virtual PrimaryKeyBase_ptr get_primary_key();
    virtual void configuration_complete();
    virtual void remove();
    virtual ComponentPortDescription_ptr get_all_ports();

    friend VISostream& _marshal_in(VISostream& _strm, const CCMObject_ptr _obj);
    friend VISistream& _marshal_out(VISistream& _strm, CCMObject_ptr& _obj);
    friend Ostream& _marshal_in(Ostream& _strm, const CCMObject_ptr _obj);
    friend Istream& _marshal_out(Istream& _strm, CCMObject_ptr& _obj);
  };

  class CCMObject_ops;
  typedef CCMObject_ops* CCMObject_ops_ptr;

  class  CCMObject_ops  {
   public:
    CCMObject_ops() {}
    virtual ~CCMObject_ops () {}

    virtual ::CORBA::IRObject_ptr get_component_def() = 0;
    virtual CCMHome_ptr get_ccm_home() = 0;
    virtual PrimaryKeyBase_ptr get_primary_key() = 0;
    virtual void configuration_complete() = 0;
    virtual void remove() = 0;
    virtual ComponentPortDescription_ptr get_all_ports() = 0;

    static const VISOps_Info *_desc();
    static CCMObject_ops_ptr _nil() { return (CCMObject_ops_ptr)NULL; }
    static CCMObject_ops_ptr _downcast(PortableServer::ServantBase* _servant);
  protected:
    static const VISOps_Info _ops_info;
  };

}
inline Istream& operator>>(Istream& s, Components::FacetDescription_var& v) {
  return Components::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, Components::EventConsumerBase_var& v) {
  return Components::_marshal_out(s, v);
}
inline void _pretty_print(VISostream& s, const Components::EventBase* v) {
  Components::_internal_pretty_print(s, v);
}
inline VISostream& operator<<(VISostream& s, const Components::ComponentPortDescription_var& v) {
  return Components::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::ReceptacleDescription_var& v) {
  return Components::_marshal_out(s, v);
}
inline Ostream& operator<<(Ostream& s, const Components::ComponentPortDescription_var& v) {
  return Components::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::SubscriberDescription_ptr& v) {
  return Components::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const Components::ConsumerDescription_ptr v) {
  return Components::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::Receptacles_ptr& v) {
  return Components::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::PublisherDescription_ptr& v) {
  return Components::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::ReceptacleDescriptions_ptr& v) {
  return Components::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, Components::ComponentPortDescription_ptr& v) {
  return Components::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::EmitterDescription_ptr& v) {
  return Components::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, Components::PortDescription_ptr& v) {
  return Components::_marshal_out(s, v);
}
inline void _pretty_print(VISostream& s, const Components::SubscriberDescription& v) {
  Components::_internal_pretty_print(s, v);
}
inline VISostream& operator<<(VISostream& s, const Components::EmitterDescription_ptr v) {
  return Components::_marshal_in(s, v);
}
inline void _pretty_print(VISostream& s, const Components::ConsumerDescriptions& v) {
  Components::_internal_pretty_print(s, v);
}
inline Istream& operator>>(Istream& s, Components::CCMObject_ptr& v) {
  return Components::_marshal_out(s, v);
}
inline void _pretty_print(VISostream& s, const Components::EventBase& v) {
  Components::_internal_pretty_print(s, v);
}
inline Istream& operator>>(Istream& s, Components::Events_var& v) {
  return Components::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::PortDescription_var& v) {
  return Components::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, Components::SubscriberDescription_var& v) {
  return Components::_marshal_out(s, v);
}
inline void _pretty_print(VISostream& s, const Components::SubscriberDescription* v) {
  Components::_internal_pretty_print(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::ComponentPortDescription_ptr& v) {
  return Components::_marshal_out(s, v);
}
/*inline Ostream& operator<<(Ostream& s, const Components::CCMObject_var& v) {
  return Components::_marshal_in(s, v);
}*/
inline void _pretty_print(VISostream& s, const Components::InvalidName& v) {
  Components::_internal_pretty_print(s, v);
}
inline void _pretty_print(VISostream& s, const Components::PortDescription& v) {
  Components::_internal_pretty_print(s, v);
}
inline void _pretty_print(VISostream& s, const Components::NoKeyAvailable& v) {
  Components::_internal_pretty_print(s, v);
}
inline Ostream& operator<<(Ostream& s, const Components::ConnectionDescriptions& v) {
  return Components::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::ConnectionDescriptions_var& v) {
  return Components::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const Components::SubscriberDescriptions_var& v) {
  return Components::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::Navigation_ptr& v) {
  return Components::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, Components::ConnectionDescription_ptr& v) {
  return Components::_marshal_out(s, v);
}
inline void _pretty_print(VISostream& s, const Components::PortDescription* v) {
  Components::_internal_pretty_print(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::AlreadyConnected& v) {
  return Components::_marshal_out(s, v);
}
inline void _pretty_print(VISostream& s, const Components::FacetDescriptions& v) {
  Components::_internal_pretty_print(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::ConsumerDescriptions_var& v) {
  return Components::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const Components::EventConsumerBase_ptr v) {
  return Components::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, Components::EventBase_var& v) {
  return Components::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const Components::SubscriberDescription_ptr v) {
  return Components::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, Components::Receptacles_ptr& v) {
  return Components::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::ConsumerDescription_ptr& v) {
  return Components::_marshal_out(s, v);
}
inline Ostream& operator<<(Ostream& s, const Components::SubscriberDescriptions& v) {
  return Components::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const Components::FacetDescriptions& v) {
  return Components::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::InvalidConnection& v) {
  return Components::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, Components::ReceptacleDescriptions& v) {
  return Components::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const Components::FacetDescriptions_var& v) {
  return Components::_marshal_in(s, v);
}
inline Ostream& operator<<(Ostream& s, const Components::PublisherDescriptions& v) {
  return Components::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const Components::FacetDescriptions* v) {
  return Components::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::ConsumerDescriptions& v) {
  return Components::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const Components::FacetDescription_var& v) {
  return Components::_marshal_in(s, v);
}
inline Ostream& operator<<(Ostream& s, const Components::ReceptacleDescription_ptr v) {
  return Components::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::SubscriberDescription_var& v) {
  return Components::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::EmitterDescriptions& v) {
  return Components::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, Components::ConsumerDescriptions_ptr& v) {
  return Components::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::PublisherDescription_var& v) {
  return Components::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::ReceptacleDescriptions_var& v) {
  return Components::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const Components::PortDescription_var& v) {
  return Components::_marshal_in(s, v);
}
/*inline VISostream& operator<<(VISostream& s, const Components::CCMObject_ptr v) {
  return Components::_marshal_in(s, v);
} */
inline Istream& operator>>(Istream& s, Components::PortDescription_var& v) {
  return Components::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::ExceededConnectionLimit& v) {
  return Components::_marshal_out(s, v);
}
inline Ostream& operator<<(Ostream& s, const Components::ReceptacleDescription_var& v) {
  return Components::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::Events_ptr& v) {
  return Components::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const Components::EventBase_ptr v) {
  return Components::_marshal_in(s, v);
}
inline Ostream& operator<<(Ostream& s, const Components::ConsumerDescription_var& v) {
  return Components::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::ConnectionDescription_ptr& v) {
  return Components::_marshal_out(s, v);
}
/*inline Istream& operator>>(Istream& s, Components::CCMObject_var& v) {
  return Components::_marshal_out(s, v);
} */
inline Ostream& operator<<(Ostream& s, const Components::ConnectionDescription_ptr v) {
  return Components::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, Components::Navigation_var& v) {
  return Components::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::FacetDescriptions_var& v) {
  return Components::_marshal_out(s, v);
}
inline Ostream& operator<<(Ostream& s, const Components::Navigation_ptr v) {
  return Components::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, Components::PublisherDescriptions_ptr& v) {
  return Components::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::FacetDescription_ptr& v) {
  return Components::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::EmitterDescriptions_ptr& v) {
  return Components::_marshal_out(s, v);
}
inline Ostream& operator<<(Ostream& s, const Components::Events_var& v) {
  return Components::_marshal_in(s, v);
}
inline Ostream& operator<<(Ostream& s, const Components::EventBase_ptr v) {
  return Components::_marshal_in(s, v);
}
inline Ostream& operator<<(Ostream& s, const Components::ConsumerDescriptions& v) {
  return Components::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const Components::EmitterDescriptions& v) {
  return Components::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const Components::FacetDescription_ptr v) {
  return Components::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const Components::PublisherDescriptions& v) {
  return Components::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, Components::PublisherDescription_ptr& v) {
  return Components::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::Receptacles_var& v) {
  return Components::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const Components::ConnectionDescription_var& v) {
  return Components::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, Components::ComponentPortDescription_var& v) {
  return Components::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::EmitterDescription_var& v) {
  return Components::_marshal_out(s, v);
}
inline void _pretty_print(VISostream& s, const Components::ReceptacleDescription& v) {
  Components::_internal_pretty_print(s, v);
}
inline VISostream& operator<<(VISostream& s, const Components::EmitterDescriptions* v) {
  return Components::_marshal_in(s, v);
}
inline void _pretty_print(VISostream& s, const Components::ReceptacleDescriptions& v) {
  Components::_internal_pretty_print(s, v);
}
inline VISostream& operator<<(VISostream& s, const Components::EmitterDescription_var& v) {
  return Components::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const Components::PublisherDescriptions* v) {
  return Components::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, Components::Receptacles_var& v) {
  return Components::_marshal_out(s, v);
}
inline Ostream& operator<<(Ostream& s, const Components::CCMObject_ptr v) {
  return Components::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::ConsumerDescription_var& v) {
  return Components::_marshal_out(s, v);
}
inline void _pretty_print(VISostream& s, const Components::ReceptacleDescription* v) {
  Components::_internal_pretty_print(s, v);
}
inline Istream& operator>>(Istream& s, Components::FacetDescriptions_ptr& v) {
  return Components::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, Components::SubscriberDescriptions_ptr& v) {
  return Components::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::ComponentPortDescription_var& v) {
  return Components::_marshal_out(s, v);
}
inline Ostream& operator<<(Ostream& s, const Components::ConsumerDescription_ptr v) {
  return Components::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, Components::EmitterDescriptions& v) {
  return Components::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::PublisherDescriptions& v) {
  return Components::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const Components::PublisherDescriptions_var& v) {
  return Components::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::SubscriberDescriptions_ptr& v) {
  return Components::_marshal_out(s, v);
}
inline void _pretty_print(VISostream& s, const Components::FacetDescription& v) {
  Components::_internal_pretty_print(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::Navigation_var& v) {
  return Components::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const Components::PublisherDescription_ptr v) {
  return Components::_marshal_in(s, v);
}
inline void _pretty_print(VISostream& s, const Components::CookieRequired& v) {
  Components::_internal_pretty_print(s, v);
}
inline Istream& operator>>(Istream& s, Components::ConnectionDescription_var& v) {
  return Components::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::PublisherDescriptions_ptr& v) {
  return Components::_marshal_out(s, v);
}
inline void _pretty_print(VISostream& s, const Components::EmitterDescription& v) {
  Components::_internal_pretty_print(s, v);
}
inline Istream& operator>>(Istream& s, Components::SubscriberDescriptions& v) {
  return Components::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const Components::EventBase_var& v) {
  return Components::_marshal_in(s, v);
}
inline void _pretty_print(VISostream& s, const Components::FacetDescription* v) {
  Components::_internal_pretty_print(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::EventConsumerBase_ptr& v) {
  return Components::_marshal_out(s, v);
}
inline void _pretty_print(VISostream& s, const Components::ConnectionDescription& v) {
  Components::_internal_pretty_print(s, v);
}
inline Ostream& operator<<(Ostream& s, const Components::PortDescription_var& v) {
  return Components::_marshal_in(s, v);
}
inline Ostream& operator<<(Ostream& s, const Components::EventConsumerBase_var& v) {
  return Components::_marshal_in(s, v);
}
inline void _pretty_print(VISostream& s, const Components::InvalidConnection& v) {
  Components::_internal_pretty_print(s, v);
}
inline Ostream& operator<<(Ostream& s, const Components::SubscriberDescription_var& v) {
  return Components::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, Components::ConsumerDescription_ptr& v) {
  return Components::_marshal_out(s, v);
}
inline void _pretty_print(VISostream& s, const Components::EmitterDescription* v) {
  Components::_internal_pretty_print(s, v);
}
inline Istream& operator>>(Istream& s, Components::ConnectionDescriptions& v) {
  return Components::_marshal_out(s, v);
}
inline Ostream& operator<<(Ostream& s, const Components::PublisherDescription_var& v) {
  return Components::_marshal_in(s, v);
}
inline void _pretty_print(VISostream& s, const Components::ConnectionDescription* v) {
  Components::_internal_pretty_print(s, v);
}
inline Ostream& operator<<(Ostream& s, const Components::Navigation_var& v) {
  return Components::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::ReceptacleDescriptions& v) {
  return Components::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::EmitterDescriptions_var& v) {
  return Components::_marshal_out(s, v);
}
inline Ostream& operator<<(Ostream& s, const Components::EventConsumerBase_ptr v) {
  return Components::_marshal_in(s, v);
}
inline Ostream& operator<<(Ostream& s, const Components::EmitterDescriptions& v) {
  return Components::_marshal_in(s, v);
}
inline Ostream& operator<<(Ostream& s, const Components::FacetDescription_ptr v) {
  return Components::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const Components::PortDescription_ptr v) {
  return Components::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::EventBase_ptr& v) {
  return Components::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::Events_var& v) {
  return Components::_marshal_out(s, v);
}
/*inline VISostream& operator<<(VISostream& s, const Components::CCMObject_var& v) {
  return Components::_marshal_in(s, v);
} */
inline VISistream& operator>>(VISistream& s, Components::ConnectionDescription_var& v) {
  return Components::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::NoConnection& v) {
  return Components::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const Components::EventConsumerBase_var& v) {
  return Components::_marshal_in(s, v);
}
inline void _pretty_print(VISostream& s, const Components::ExceededConnectionLimit& v) {
  Components::_internal_pretty_print(s, v);
}
inline Ostream& operator<<(Ostream& s, const Components::Receptacles_var& v) {
  return Components::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::FacetDescription_var& v) {
  return Components::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, Components::EmitterDescriptions_ptr& v) {
  return Components::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const Components::ReceptacleDescriptions& v) {
  return Components::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const Components::ReceptacleDescription_ptr v) {
  return Components::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const Components::Navigation_var& v) {
  return Components::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, Components::PublisherDescription_var& v) {
  return Components::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const Components::ReceptacleDescriptions* v) {
  return Components::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::NoKeyAvailable& v) {
  return Components::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::PublisherDescriptions_var& v) {
  return Components::_marshal_out(s, v);
}
inline void _pretty_print(VISostream& s, const Components::ConsumerDescription& v) {
  Components::_internal_pretty_print(s, v);
}
inline void _pretty_print(VISostream& s, const Components::NoConnection& v) {
  Components::_internal_pretty_print(s, v);
}
inline VISostream& operator<<(VISostream& s, const Components::ConnectionDescriptions_var& v) {
  return Components::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::EventConsumerBase_var& v) {
  return Components::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const Components::ConnectionDescription_ptr v) {
  return Components::_marshal_in(s, v);
}
inline void _pretty_print(VISostream& s, const Components::PublisherDescriptions& v) {
  Components::_internal_pretty_print(s, v);
}
inline Istream& operator>>(Istream& s, Components::ConsumerDescriptions& v) {
  return Components::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::BadEventType& v) {
  return Components::_marshal_out(s, v);
}
inline Ostream& operator<<(Ostream& s, const Components::FacetDescription_var& v) {
  return Components::_marshal_in(s, v);
}
inline void _pretty_print(VISostream& s, const Components::ConsumerDescription* v) {
  Components::_internal_pretty_print(s, v);
}
inline void _pretty_print(VISostream& s, const Components::SubscriberDescriptions& v) {
  Components::_internal_pretty_print(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::SubscriberDescriptions_var& v) {
  return Components::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, Components::ReceptacleDescription_ptr& v) {
  return Components::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, Components::FacetDescriptions& v) {
  return Components::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const Components::Receptacles_var& v) {
  return Components::_marshal_in(s, v);
}
inline Ostream& operator<<(Ostream& s, const Components::ConnectionDescription_var& v) {
  return Components::_marshal_in(s, v);
}
inline void _pretty_print(VISostream& s, const Components::ConnectionDescriptions& v) {
  Components::_internal_pretty_print(s, v);
}
inline Ostream& operator<<(Ostream& s, const Components::EmitterDescription_var& v) {
  return Components::_marshal_in(s, v);
}
inline Ostream& operator<<(Ostream& s, const Components::Receptacles_ptr v) {
  return Components::_marshal_in(s, v);
}
inline Ostream& operator<<(Ostream& s, const Components::PortDescription_ptr v) {
  return Components::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const Components::ConsumerDescription_var& v) {
  return Components::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, Components::EventConsumerBase_ptr& v) {
  return Components::_marshal_out(s, v);
}
/*inline VISistream& operator>>(VISistream& s, Components::CCMObject_ptr& v) {
  return Components::_marshal_out(s, v);
} */
inline Istream& operator>>(Istream& s, Components::ConsumerDescription_var& v) {
  return Components::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const Components::ReceptacleDescriptions_var& v) {
  return Components::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const Components::PublisherDescription_var& v) {
  return Components::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, Components::EmitterDescription_ptr& v) {
  return Components::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const Components::ConsumerDescriptions_var& v) {
  return Components::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, Components::PublisherDescriptions& v) {
  return Components::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::InvalidName& v) {
  return Components::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const Components::ReceptacleDescription_var& v) {
  return Components::_marshal_in(s, v);
}
inline void _pretty_print(VISostream& s, const Components::ComponentPortDescription& v) {
  Components::_internal_pretty_print(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::SubscriberDescriptions& v) {
  return Components::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, Components::FacetDescription_ptr& v) {
  return Components::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::EventBase_var& v) {
  return Components::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const Components::SubscriberDescriptions& v) {
  return Components::_marshal_in(s, v);
}
inline void _pretty_print(VISostream& s, const Components::BadEventType& v) {
  Components::_internal_pretty_print(s, v);
}
inline VISostream& operator<<(VISostream& s, const Components::Receptacles_ptr v) {
  return Components::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const Components::Navigation_ptr v) {
  return Components::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::ReceptacleDescription_ptr& v) {
  return Components::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const Components::ComponentPortDescription_ptr v) {
  return Components::_marshal_in(s, v);
}
inline void _pretty_print(VISostream& s, const Components::EmitterDescriptions& v) {
  Components::_internal_pretty_print(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::ConnectionDescriptions& v) {
  return Components::_marshal_out(s, v);
}
inline void _pretty_print(VISostream& s, const Components::ComponentPortDescription* v) {
  Components::_internal_pretty_print(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::CookieRequired& v) {
  return Components::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const Components::EmitterDescriptions_var& v) {
  return Components::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const Components::ConnectionDescriptions& v) {
  return Components::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const Components::SubscriberDescriptions* v) {
  return Components::_marshal_in(s, v);
}
inline Ostream& operator<<(Ostream& s, const Components::ReceptacleDescriptions& v) {
  return Components::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const Components::ConsumerDescriptions& v) {
  return Components::_marshal_in(s, v);
}
inline Ostream& operator<<(Ostream& s, const Components::Events_ptr v) {
  return Components::_marshal_in(s, v);
}
inline Ostream& operator<<(Ostream& s, const Components::EmitterDescription_ptr v) {
  return Components::_marshal_in(s, v);
}
inline Ostream& operator<<(Ostream& s, const Components::SubscriberDescription_ptr v) {
  return Components::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::FacetDescriptions& v) {
  return Components::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, Components::Events_ptr& v) {
  return Components::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const Components::ConnectionDescriptions* v) {
  return Components::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::PortDescription_ptr& v) {
  return Components::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const Components::ConsumerDescriptions* v) {
  return Components::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, Components::ReceptacleDescription_var& v) {
  return Components::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, Components::ConnectionDescriptions_ptr& v) {
  return Components::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, Components::SubscriberDescription_ptr& v) {
  return Components::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::ConnectionDescriptions_ptr& v) {
  return Components::_marshal_out(s, v);
}
inline void _pretty_print(VISostream& s, const Components::PublisherDescription& v) {
  Components::_internal_pretty_print(s, v);
}
/*inline VISistream& operator>>(VISistream& s, Components::CCMObject_var& v) {
  return Components::_marshal_out(s, v);
} */
inline Ostream& operator<<(Ostream& s, const Components::FacetDescriptions& v) {
  return Components::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::ConsumerDescriptions_ptr& v) {
  return Components::_marshal_out(s, v);
}
inline void _pretty_print(VISostream& s, const Components::PublisherDescription* v) {
  Components::_internal_pretty_print(s, v);
}
inline Ostream& operator<<(Ostream& s, const Components::PublisherDescription_ptr v) {
  return Components::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, Components::EventBase_ptr& v) {
  return Components::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, Components::EmitterDescription_var& v) {
  return Components::_marshal_out(s, v);
}
inline Ostream& operator<<(Ostream& s, const Components::EventBase_var& v) {
  return Components::_marshal_in(s, v);
}
inline Ostream& operator<<(Ostream& s, const Components::ComponentPortDescription_ptr v) {
  return Components::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, Components::Navigation_ptr& v) {
  return Components::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, Components::FacetDescriptions_ptr& v) {
  return Components::_marshal_out(s, v);
}
inline void _pretty_print(VISostream& s, const Components::AlreadyConnected& v) {
  Components::_internal_pretty_print(s, v);
}
inline VISostream& operator<<(VISostream& s, const Components::SubscriberDescription_var& v) {
  return Components::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, Components::ReceptacleDescriptions_ptr& v) {
  return Components::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const Components::Events_var& v) {
  return Components::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const Components::Events_ptr v) {
  return Components::_marshal_in(s, v);
}

#include "vpost.h"
#endif // __CCMObject_idl___client
