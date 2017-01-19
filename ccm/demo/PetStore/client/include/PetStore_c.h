/*
 * Copyright 2001 Borland Software Corporation, Inc.
 *
 * GENERATED CODE --- DO NOT EDIT
 * 
 */

#ifndef __PetStore_idl___client
#define __PetStore_idl___client
#include "vpre.h"

#ifndef _corba_h_
# include "corba.h"
#endif   // _corba_h_
#ifndef __CCMCommon_idl___client
# include "CCMCommon_c.h"
#endif
#ifndef __CCMObject_idl___client
# include "CCMObject_c.h"
#endif
#ifndef __CCMHome_idl___client
# include "CCMHome_c.h"
#endif
namespace PetStore {
  class  ItemPriceList;
  typedef ItemPriceList* ItemPriceList_ptr;

  class  ItemPriceList : private VISResource {
  private:
    CORBA::ULong *_contents;
    CORBA::ULong _count;
    CORBA::ULong _num_allocated;
    CORBA::Boolean _release_flag;

  public:
    #if defined(MSVCNEWDLL_BUG)
      void *operator new(size_t ts);
      void *operator new(size_t ts, char*, int) {return operator new(ts);}
      void operator delete(void *p);
    #endif // MSVCNEWDLL_BUG

    static CORBA::ULong *allocbuf(CORBA::ULong _nelems);
    static void freebuf(CORBA::ULong* _data);

    ItemPriceList(CORBA::ULong _max = 0);
    ItemPriceList(CORBA::ULong max, CORBA::ULong length, CORBA::ULong *_data,
                  CORBA::Boolean release = (CORBA::Boolean)0);
    ItemPriceList(const ItemPriceList&);
    virtual ~ItemPriceList();

    ItemPriceList& operator=(const ItemPriceList&);
    ItemPriceList& operator=(const ItemPriceList* _obj) {
      assert(_obj);
      return operator=(*_obj);
    }

    CORBA::ULong maximum() const { return _num_allocated; }

    void length(CORBA::ULong _len);
    CORBA::ULong length() const { return _count; }

    void replace(CORBA::ULong _max, CORBA::ULong _len, CORBA::ULong *_data,
                 CORBA::Boolean _release = (CORBA::Boolean)0);

    CORBA::ULong *get_buffer(CORBA::Boolean orphan = (CORBA::Boolean) 0);
    const CORBA::ULong* get_buffer() const;

    CORBA::ULong& operator[](CORBA::ULong _index);
    const CORBA::ULong& operator[](CORBA::ULong _index) const;

    static ItemPriceList *_duplicate(ItemPriceList* _ptr) {
      if (_ptr) _ptr->_ref();
      return _ptr;
    }

    static void _release(ItemPriceList *_ptr) {
      if (_ptr && (_ptr->_deref() == 0L)) 
        delete _ptr;
    }

    friend VISostream& _marshal_in(VISostream&, const ItemPriceList&);
    friend VISistream& _marshal_out(VISistream&, ItemPriceList&);

    friend VISostream& _marshal_in(VISostream& _strm, const ItemPriceList* _obj);
    friend VISistream& _marshal_out(VISistream& _strm, ItemPriceList_ptr& _obj);

    friend void _internal_pretty_print(VISostream&, const ItemPriceList&);

    friend Ostream& _marshal_in(Ostream&, const ItemPriceList&);
    friend Istream& _marshal_out(Istream&, ItemPriceList&);
    friend Istream& _marshal_out(Istream&, ItemPriceList_ptr&);
  };

  class  ItemPriceList_out;

  class  ItemPriceList_var {
    friend class ItemPriceList_out;
     private:
    ItemPriceList *_ptr;
     public:
    ItemPriceList_var() : _ptr((ItemPriceList*)NULL) {}
    ItemPriceList_var(ItemPriceList *_p) : _ptr(_p) {}
    ItemPriceList_var(const ItemPriceList_var& _var) :_ptr(ItemPriceList::_duplicate(_var._ptr)){}
    virtual ~ItemPriceList_var() { ItemPriceList::_release(_ptr); }
    ItemPriceList_var& operator=(ItemPriceList* _p) {
      ItemPriceList::_release(_ptr);
      _ptr = _p;
      return *this;
    }

    ItemPriceList_var& operator=(const ItemPriceList_var& _var) {
      ItemPriceList::_release(_ptr);
      _ptr = ItemPriceList::_duplicate(_var._ptr);
      return *this;
    }
    operator ItemPriceList*() const { return _ptr; }

    ItemPriceList* operator->() { return _ptr; }
    const ItemPriceList* operator->() const { return _ptr; }

    operator ItemPriceList&() { return *_ptr; }
    operator const ItemPriceList&() const { return *_ptr; }

    const ItemPriceList& in() const { return *_ptr; }
    ItemPriceList& inout() { return *_ptr; }
    ItemPriceList_ptr & out() {
      ItemPriceList::_release(_ptr);
      _ptr = (ItemPriceList *)NULL;
      return _ptr;
    }

    ItemPriceList* _retn() {
      ItemPriceList* _tmp_ptr;
      _tmp_ptr = _ptr;
      _ptr = (ItemPriceList *)NULL;
      return _tmp_ptr;
    }
    friend VISostream& _marshal_in(VISostream&, const ItemPriceList_var&);
    friend VISistream& _marshal_out(VISistream&, ItemPriceList_var&);

    CORBA::ULong& operator[](CORBA::ULong _index);
    const CORBA::ULong& operator[](CORBA::ULong _index) const;

  };
  class  ItemPriceList_out {
   private:
    ItemPriceList_ptr& _ptr;
    static ItemPriceList* _nil() { return (ItemPriceList*)NULL; }

    void operator=(const ItemPriceList_out&);
    void operator=(const ItemPriceList_var&);
     public:
    ItemPriceList_out(const ItemPriceList_out& _o) : _ptr(_o._ptr) {}
    ItemPriceList_out(ItemPriceList_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    ItemPriceList_out(ItemPriceList_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    ItemPriceList_out& operator=(ItemPriceList* _p) {
      _ptr = _p;
      return *this;
    }
    operator ItemPriceList_ptr&() { return _ptr; }
    ItemPriceList_ptr& ptr() { return _ptr; }
  };
  #ifndef PetStore_Calculate_var_
  #define PetStore_Calculate_var_

  class  Calculate;

  typedef Calculate* Calculate_ptr;
  typedef Calculate_ptr CalculateRef;
   VISistream& _marshal_out(VISistream&, Calculate_ptr&);
   VISostream& _marshal_in(VISostream&, const Calculate_ptr);
  class  Calculate_out;

  class  Calculate_var : public CORBA::_var {
  friend class Calculate_out;
   private:
    Calculate_ptr _ptr;

   public:
    Calculate_var();
    Calculate_var(Calculate_ptr);
    Calculate_var(const Calculate_var &);
    virtual ~Calculate_var();

    static Calculate_ptr _duplicate(Calculate_ptr);
    static void _release(Calculate_ptr);

    Calculate_var& operator=(const Calculate_var& _var);
    Calculate_var& operator=(Calculate_ptr);

    operator Calculate*() const { return _ptr; }
    Calculate* operator->() const { return _ptr; }

    Calculate_ptr in() const { return _ptr; }
    Calculate_ptr& inout() { return _ptr; }
    Calculate_ptr& out();
    Calculate_ptr _retn();

    friend VISostream& _marshal_in(VISostream&, const Calculate_var&);
    friend VISistream& _marshal_out(VISistream&, Calculate_var&);

    friend Istream& _marshal_out(Istream&, Calculate_var&);
    friend Ostream& _marshal_in(Ostream&, const Calculate_var&);
  };
  class  Calculate_out {
   private:
    Calculate_ptr& _ptr;
    static Calculate* _nil() { return (Calculate*)NULL; }

    void operator=(const Calculate_out&);
    void operator=(const Calculate_var&);
     public:
    Calculate_out(const Calculate_out& _o) : _ptr(_o._ptr) {}
    Calculate_out(Calculate_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    Calculate_out(Calculate_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    Calculate_out& operator=(Calculate* _p) {
      _ptr = _p;
      return *this;
    }
    operator Calculate_ptr&() { return _ptr; }
    Calculate_ptr& ptr() { return _ptr; }
    Calculate* operator->() { return _ptr; }
  };

  #endif // PetStore_Calculate_var_

  // idl interface: PetStore::Calculate
  class  Calculate : public virtual CORBA_Object {
   private:
    static const CORBA::TypeInfo _class_info;
    void operator=(const Calculate&) {}

    protected:
    Calculate() {}
    Calculate(const Calculate&) {}

   public:
    virtual ~Calculate() {}
    static  const CORBA::TypeInfo *_desc();
    virtual const CORBA::TypeInfo *_type_info() const;
    virtual void *_safe_narrow(const CORBA::TypeInfo& ) const;
    static CORBA::Object*_factory();
    Calculate_ptr _this();
    static Calculate_ptr _duplicate(Calculate_ptr _obj) {
      if (_obj) _obj->_ref();
      return _obj;
    }
    static Calculate_ptr _nil() { return (Calculate_ptr)NULL; }
    static Calculate_ptr _narrow(CORBA::Object* _obj);
    static Calculate_ptr _clone(Calculate_ptr _obj) {
      CORBA::Object_var _obj_var(CORBA_Object::_clone(_obj));
      #if defined(_HPCC_BUG)
        return _narrow(_obj_var.operator CORBA::Object_ptr());
      #else
        return _narrow(_obj_var);
      #endif  // _HPCC_BUG
    }
    static Calculate_ptr _bind(const char *_object_name = (const char*)NULL,
                               const char *_host_name = (const char*)NULL,
                               const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                               CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    static Calculate_ptr _bind(const char *_poa_name,
                               const CORBA::OctetSequence& _id,
                               const char *_host_name = (const char*)NULL,
                               const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                               CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    virtual CORBA::ULong multiply(CORBA::ULong _unitPrice,
                                  CORBA::ULong _quantity);
    virtual CORBA::ULong add(const ItemPriceList& _itemList);

    friend VISostream& _marshal_in(VISostream& _strm, const Calculate_ptr _obj);
    friend VISistream& _marshal_out(VISistream& _strm, Calculate_ptr& _obj);
    friend Ostream& _marshal_in(Ostream& _strm, const Calculate_ptr _obj);
    friend Istream& _marshal_out(Istream& _strm, Calculate_ptr& _obj);
  };

  class Calculate_ops;
  typedef Calculate_ops* Calculate_ops_ptr;

  class  Calculate_ops  {
   public:
    Calculate_ops() {}
    virtual ~Calculate_ops () {}

    virtual CORBA::ULong multiply(CORBA::ULong _unitPrice,
                                  CORBA::ULong _quantity) = 0;
    virtual CORBA::ULong add(const ItemPriceList& _itemList) = 0;

    static const VISOps_Info *_desc();
    static Calculate_ops_ptr _nil() { return (Calculate_ops_ptr)NULL; }
    static Calculate_ops_ptr _downcast(PortableServer::ServantBase* _servant);
  protected:
    static const VISOps_Info _ops_info;
  };

  #ifndef PetStore_Pricer_var_
  #define PetStore_Pricer_var_

  class  Pricer;

  typedef Pricer* Pricer_ptr;
  typedef Pricer_ptr PricerRef;
   VISistream& _marshal_out(VISistream&, Pricer_ptr&);
   VISostream& _marshal_in(VISostream&, const Pricer_ptr);
  class  Pricer_out;

  class  Pricer_var : public CORBA::_var {
  friend class Pricer_out;
   private:
    Pricer_ptr _ptr;

   public:
    Pricer_var();
    Pricer_var(Pricer_ptr);
    Pricer_var(const Pricer_var &);
    virtual ~Pricer_var();

    static Pricer_ptr _duplicate(Pricer_ptr);
    static void _release(Pricer_ptr);

    Pricer_var& operator=(const Pricer_var& _var);
    Pricer_var& operator=(Pricer_ptr);

    operator Pricer*() const { return _ptr; }
    Pricer* operator->() const { return _ptr; }

    Pricer_ptr in() const { return _ptr; }
    Pricer_ptr& inout() { return _ptr; }
    Pricer_ptr& out();
    Pricer_ptr _retn();

    friend VISostream& _marshal_in(VISostream&, const Pricer_var&);
    friend VISistream& _marshal_out(VISistream&, Pricer_var&);

    friend Istream& _marshal_out(Istream&, Pricer_var&);
    friend Ostream& _marshal_in(Ostream&, const Pricer_var&);
  };
  class  Pricer_out {
   private:
    Pricer_ptr& _ptr;
    static Pricer* _nil() { return (Pricer*)NULL; }

    void operator=(const Pricer_out&);
    void operator=(const Pricer_var&);
     public:
    Pricer_out(const Pricer_out& _o) : _ptr(_o._ptr) {}
    Pricer_out(Pricer_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    Pricer_out(Pricer_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    Pricer_out& operator=(Pricer* _p) {
      _ptr = _p;
      return *this;
    }
    operator Pricer_ptr&() { return _ptr; }
    Pricer_ptr& ptr() { return _ptr; }
    Pricer* operator->() { return _ptr; }
  };

  #endif // PetStore_Pricer_var_

  // idl interface: PetStore::Pricer
  class  Pricer : public virtual Calculate, public virtual ::Components::CCMObject, public virtual CORBA_Object {
   private:
    static const CORBA::TypeInfo _class_info;
    void operator=(const Pricer&) {}

    protected:
    Pricer() {}
    Pricer(const Pricer&) {}

   public:
    virtual ~Pricer() {}
    static  const CORBA::TypeInfo *_desc();
    virtual const CORBA::TypeInfo *_type_info() const;
    virtual void *_safe_narrow(const CORBA::TypeInfo& ) const;
    static CORBA::Object*_factory();
    Pricer_ptr _this();
    static Pricer_ptr _duplicate(Pricer_ptr _obj) {
      if (_obj) _obj->_ref();
      return _obj;
    }
    static Pricer_ptr _nil() { return (Pricer_ptr)NULL; }
    static Pricer_ptr _narrow(CORBA::Object* _obj);
    static Pricer_ptr _clone(Pricer_ptr _obj) {
      CORBA::Object_var _obj_var(CORBA_Object::_clone(_obj));
      #if defined(_HPCC_BUG)
        return _narrow(_obj_var.operator CORBA::Object_ptr());
      #else
        return _narrow(_obj_var);
      #endif  // _HPCC_BUG
    }
    static Pricer_ptr _bind(const char *_object_name = (const char*)NULL,
                            const char *_host_name = (const char*)NULL,
                            const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                            CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    static Pricer_ptr _bind(const char *_poa_name,
                            const CORBA::OctetSequence& _id,
                            const char *_host_name = (const char*)NULL,
                            const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                            CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    friend VISostream& _marshal_in(VISostream& _strm, const Pricer_ptr _obj);
    friend VISistream& _marshal_out(VISistream& _strm, Pricer_ptr& _obj);
    friend Ostream& _marshal_in(Ostream& _strm, const Pricer_ptr _obj);
    friend Istream& _marshal_out(Istream& _strm, Pricer_ptr& _obj);
  };

  class Pricer_ops;
  typedef Pricer_ops* Pricer_ops_ptr;

  class  Pricer_ops  {
   public:
    Pricer_ops() {}
    virtual ~Pricer_ops () {}

    static const VISOps_Info *_desc();
    static Pricer_ops_ptr _nil() { return (Pricer_ops_ptr)NULL; }
    static Pricer_ops_ptr _downcast(PortableServer::ServantBase* _servant);
  protected:
    static const VISOps_Info _ops_info;
  };

  #ifndef PetStore_KeyID_var_
  #define PetStore_KeyID_var_

  class  KeyID;

  typedef KeyID* KeyID_ptr;

   VISistream& _marshal_out(VISistream&, KeyID_ptr&);
   VISostream& _marshal_in(VISostream&, const KeyID_ptr);

  class  KeyID_out;
  class  KeyID_var: public CORBA::_var {
    friend class KeyID_out;
     public:
    KeyID_var();
    KeyID_var(KeyID_ptr );
    KeyID_var(const KeyID_var &);

    virtual ~KeyID_var();

    KeyID_var& operator=(KeyID_ptr);
    void operator=(const KeyID_var & _v);

    static void _add_ref(KeyID_ptr );
    static void _remove_ref(KeyID_ptr );

    operator KeyID*() const { return _ptr; }
    KeyID* operator->() { return _ptr; }
    const KeyID* operator->() const { return _ptr; }

    KeyID_ptr in () const { return _ptr; }
    KeyID_ptr& inout () { return _ptr; }
    KeyID_ptr& out();
    KeyID_ptr _retn();

    friend VISistream& _marshal_out(VISistream& _strm, KeyID_var& _var);
    friend VISostream& _marshal_in(VISostream& _strm, const KeyID_var& _var);

    friend Istream& _marshal_out(Istream&, KeyID_var&);
    friend Ostream& _marshal_in(Ostream&, const KeyID_var&);
     private:
    KeyID_ptr _ptr;
  };
  class  KeyID_out {
   private:
    KeyID_ptr& _ptr;
    static KeyID* _nil() { return (KeyID*)NULL; }

    void operator=(const KeyID_out&);
    void operator=(const KeyID_var&);
     public:
    KeyID_out(const KeyID_out& _o) : _ptr(_o._ptr) {}
    KeyID_out(KeyID_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    KeyID_out(KeyID_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    KeyID_out& operator=(KeyID* _p) {
      _ptr = _p;
      return *this;
    }
    operator KeyID_ptr&() { return _ptr; }
    KeyID_ptr& ptr() { return _ptr; }
    KeyID* operator->() { return _ptr; }
  };

  #endif	// PetStore_KeyID_var_
  // idl valuetype: PetStore::KeyID
  class  KeyID  : public virtual CORBA::ValueBase,
                  public virtual ::Components::PrimaryKeyBase   {
    public:
      friend VISostream& _marshal_in(VISostream& _strm, const KeyID_ptr _obj);
      friend VISistream& _marshal_out(VISistream& _strm, KeyID_ptr& _obj);
      friend Ostream& _marshal_in(Ostream& _strm, const KeyID_ptr _obj);
      friend Istream& _marshal_out(Istream& _strm, KeyID_ptr& _obj);
      friend void _internal_pretty_print(VISostream& _strm, const KeyID& _obj);
      friend void _internal_pretty_print(VISostream& _strm, const KeyID* _obj);

      static KeyID* _downcast(CORBA::ValueBase* vbptr);
      virtual void id(char* _id) = 0;
      virtual void id(const char* _id) = 0;
      virtual void id(const CORBA::String_var& _id) = 0;
      virtual const char* id() const = 0;
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
       KeyID() {}
        virtual ~KeyID() {}

      private:
        void operator=(const KeyID&);
  };
  class  OBV_KeyID: public virtual KeyID  {
    public:
      virtual void id(char* _id) {
        _obv_id = _id;
      }

      virtual void id(const char* _id) {
        _obv_id = CORBA::string_dup(_id);
      }

      virtual void id(const CORBA::String_var& _id) {
        _obv_id = CORBA::string_dup(_id);
      }

      virtual const char* id() const {
        return _obv_id;
      }
    protected:
      OBV_KeyID();
      virtual ~OBV_KeyID();

      OBV_KeyID(const char* _id) {
                _obv_id = CORBA::string_dup(_id);
  
      }
      CORBA::String_var _obv_id;
  };
  class KeyID_init : public CORBA::ValueFactoryBase {
  };
  #ifndef PetStore_Product_var_
  #define PetStore_Product_var_

  class  Product;

  typedef Product* Product_ptr;
  typedef Product_ptr ProductRef;
   VISistream& _marshal_out(VISistream&, Product_ptr&);
   VISostream& _marshal_in(VISostream&, const Product_ptr);
  class  Product_out;

  class  Product_var : public CORBA::_var {
  friend class Product_out;
   private:
    Product_ptr _ptr;

   public:
    Product_var();
    Product_var(Product_ptr);
    Product_var(const Product_var &);
    virtual ~Product_var();

    static Product_ptr _duplicate(Product_ptr);
    static void _release(Product_ptr);

    Product_var& operator=(const Product_var& _var);
    Product_var& operator=(Product_ptr);

    operator Product*() const { return _ptr; }
    Product* operator->() const { return _ptr; }

    Product_ptr in() const { return _ptr; }
    Product_ptr& inout() { return _ptr; }
    Product_ptr& out();
    Product_ptr _retn();

    friend VISostream& _marshal_in(VISostream&, const Product_var&);
    friend VISistream& _marshal_out(VISistream&, Product_var&);

    friend Istream& _marshal_out(Istream&, Product_var&);
    friend Ostream& _marshal_in(Ostream&, const Product_var&);
  };
  class  Product_out {
   private:
    Product_ptr& _ptr;
    static Product* _nil() { return (Product*)NULL; }

    void operator=(const Product_out&);
    void operator=(const Product_var&);
     public:
    Product_out(const Product_out& _o) : _ptr(_o._ptr) {}
    Product_out(Product_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    Product_out(Product_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    Product_out& operator=(Product* _p) {
      _ptr = _p;
      return *this;
    }
    operator Product_ptr&() { return _ptr; }
    Product_ptr& ptr() { return _ptr; }
    Product* operator->() { return _ptr; }
  };

  #endif // PetStore_Product_var_

  // idl interface: PetStore::Product
  class  Product : public virtual ::Components::CCMObject, public virtual CORBA_Object {
   private:
    static const CORBA::TypeInfo _class_info;
    void operator=(const Product&) {}

    protected:
    Product() {}
    Product(const Product&) {}

   public:
    virtual ~Product() {}
    static  const CORBA::TypeInfo *_desc();
    virtual const CORBA::TypeInfo *_type_info() const;
    virtual void *_safe_narrow(const CORBA::TypeInfo& ) const;
    static CORBA::Object*_factory();
    Product_ptr _this();
    static Product_ptr _duplicate(Product_ptr _obj) {
      if (_obj) _obj->_ref();
      return _obj;
    }
    static Product_ptr _nil() { return (Product_ptr)NULL; }
    static Product_ptr _narrow(CORBA::Object* _obj);
    static Product_ptr _clone(Product_ptr _obj) {
      CORBA::Object_var _obj_var(CORBA_Object::_clone(_obj));
      #if defined(_HPCC_BUG)
        return _narrow(_obj_var.operator CORBA::Object_ptr());
      #else
        return _narrow(_obj_var);
      #endif  // _HPCC_BUG
    }
    static Product_ptr _bind(const char *_object_name = (const char*)NULL,
                             const char *_host_name = (const char*)NULL,
                             const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                             CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    static Product_ptr _bind(const char *_poa_name,
                             const CORBA::OctetSequence& _id,
                             const char *_host_name = (const char*)NULL,
                             const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                             CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    virtual KeyID_ptr product_id();
    virtual char* product_name();
    virtual void product_name(const char* _product_name);
    virtual char* product_picture();
    virtual void product_picture(const char* _product_picture);
    virtual char* product_category();
    virtual void product_category(const char* _product_category);
    virtual CORBA::ULong product_price();
    virtual void product_price(CORBA::ULong _product_price);
    virtual CORBA::ULong product_qty();
    virtual void product_qty(CORBA::ULong _product_qty);
    virtual char* product_description();
    virtual void product_description(const char* _product_description);

    friend VISostream& _marshal_in(VISostream& _strm, const Product_ptr _obj);
    friend VISistream& _marshal_out(VISistream& _strm, Product_ptr& _obj);
    friend Ostream& _marshal_in(Ostream& _strm, const Product_ptr _obj);
    friend Istream& _marshal_out(Istream& _strm, Product_ptr& _obj);
  };

  class Product_ops;
  typedef Product_ops* Product_ops_ptr;

  class  Product_ops  {
   public:
    Product_ops() {}
    virtual ~Product_ops () {}

    virtual KeyID_ptr product_id() = 0;
    virtual char* product_name() = 0;
    virtual void product_name(const char* _product_name) = 0;
    virtual char* product_picture() = 0;
    virtual void product_picture(const char* _product_picture) = 0;
    virtual char* product_category() = 0;
    virtual void product_category(const char* _product_category) = 0;
    virtual CORBA::ULong product_price() = 0;
    virtual void product_price(CORBA::ULong _product_price) = 0;
    virtual CORBA::ULong product_qty() = 0;
    virtual void product_qty(CORBA::ULong _product_qty) = 0;
    virtual char* product_description() = 0;
    virtual void product_description(const char* _product_description) = 0;

    static const VISOps_Info *_desc();
    static Product_ops_ptr _nil() { return (Product_ops_ptr)NULL; }
    static Product_ops_ptr _downcast(PortableServer::ServantBase* _servant);
  protected:
    static const VISOps_Info _ops_info;
  };

  #ifndef PetStore_OrderItem_var_
  #define PetStore_OrderItem_var_

  class  OrderItem;

  typedef OrderItem* OrderItem_ptr;
  typedef OrderItem_ptr OrderItemRef;
   VISistream& _marshal_out(VISistream&, OrderItem_ptr&);
   VISostream& _marshal_in(VISostream&, const OrderItem_ptr);
  class  OrderItem_out;

  class  OrderItem_var : public CORBA::_var {
  friend class OrderItem_out;
   private:
    OrderItem_ptr _ptr;

   public:
    OrderItem_var();
    OrderItem_var(OrderItem_ptr);
    OrderItem_var(const OrderItem_var &);
    virtual ~OrderItem_var();

    static OrderItem_ptr _duplicate(OrderItem_ptr);
    static void _release(OrderItem_ptr);

    OrderItem_var& operator=(const OrderItem_var& _var);
    OrderItem_var& operator=(OrderItem_ptr);

    operator OrderItem*() const { return _ptr; }
    OrderItem* operator->() const { return _ptr; }

    OrderItem_ptr in() const { return _ptr; }
    OrderItem_ptr& inout() { return _ptr; }
    OrderItem_ptr& out();
    OrderItem_ptr _retn();

    friend VISostream& _marshal_in(VISostream&, const OrderItem_var&);
    friend VISistream& _marshal_out(VISistream&, OrderItem_var&);

    friend Istream& _marshal_out(Istream&, OrderItem_var&);
    friend Ostream& _marshal_in(Ostream&, const OrderItem_var&);
  };
  class  OrderItem_out {
   private:
    OrderItem_ptr& _ptr;
    static OrderItem* _nil() { return (OrderItem*)NULL; }

    void operator=(const OrderItem_out&);
    void operator=(const OrderItem_var&);
     public:
    OrderItem_out(const OrderItem_out& _o) : _ptr(_o._ptr) {}
    OrderItem_out(OrderItem_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    OrderItem_out(OrderItem_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    OrderItem_out& operator=(OrderItem* _p) {
      _ptr = _p;
      return *this;
    }
    operator OrderItem_ptr&() { return _ptr; }
    OrderItem_ptr& ptr() { return _ptr; }
    OrderItem* operator->() { return _ptr; }
  };

  #endif // PetStore_OrderItem_var_

  // idl interface: PetStore::OrderItem
  class  OrderItem : public virtual ::Components::CCMObject, public virtual CORBA_Object {
   private:
    static const CORBA::TypeInfo _class_info;
    void operator=(const OrderItem&) {}

    protected:
    OrderItem() {}
    OrderItem(const OrderItem&) {}

   public:
    virtual ~OrderItem() {}
    static  const CORBA::TypeInfo *_desc();
    virtual const CORBA::TypeInfo *_type_info() const;
    virtual void *_safe_narrow(const CORBA::TypeInfo& ) const;
    static CORBA::Object*_factory();
    OrderItem_ptr _this();
    static OrderItem_ptr _duplicate(OrderItem_ptr _obj) {
      if (_obj) _obj->_ref();
      return _obj;
    }
    static OrderItem_ptr _nil() { return (OrderItem_ptr)NULL; }
    static OrderItem_ptr _narrow(CORBA::Object* _obj);
    static OrderItem_ptr _clone(OrderItem_ptr _obj) {
      CORBA::Object_var _obj_var(CORBA_Object::_clone(_obj));
      #if defined(_HPCC_BUG)
        return _narrow(_obj_var.operator CORBA::Object_ptr());
      #else
        return _narrow(_obj_var);
      #endif  // _HPCC_BUG
    }
    static OrderItem_ptr _bind(const char *_object_name = (const char*)NULL,
                               const char *_host_name = (const char*)NULL,
                               const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                               CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    static OrderItem_ptr _bind(const char *_poa_name,
                               const CORBA::OctetSequence& _id,
                               const char *_host_name = (const char*)NULL,
                               const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                               CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    virtual KeyID_ptr orderItem_id();
    virtual char* product_id();
    virtual void product_id(const char* _product_id);
    virtual char* product_name();
    virtual void product_name(const char* _product_name);
    virtual CORBA::ULong quantity();
    virtual void quantity(CORBA::ULong _quantity);
    virtual CORBA::ULong unit_price();
    virtual void unit_price(CORBA::ULong _unit_price);
    virtual CORBA::ULong sub_total();
    virtual void sub_total(CORBA::ULong _sub_total);

    friend VISostream& _marshal_in(VISostream& _strm, const OrderItem_ptr _obj);
    friend VISistream& _marshal_out(VISistream& _strm, OrderItem_ptr& _obj);
    friend Ostream& _marshal_in(Ostream& _strm, const OrderItem_ptr _obj);
    friend Istream& _marshal_out(Istream& _strm, OrderItem_ptr& _obj);
  };

  class OrderItem_ops;
  typedef OrderItem_ops* OrderItem_ops_ptr;

  class  OrderItem_ops  {
   public:
    OrderItem_ops() {}
    virtual ~OrderItem_ops () {}

    virtual KeyID_ptr orderItem_id() = 0;
    virtual char* product_id() = 0;
    virtual void product_id(const char* _product_id) = 0;
    virtual char* product_name() = 0;
    virtual void product_name(const char* _product_name) = 0;
    virtual CORBA::ULong quantity() = 0;
    virtual void quantity(CORBA::ULong _quantity) = 0;
    virtual CORBA::ULong unit_price() = 0;
    virtual void unit_price(CORBA::ULong _unit_price) = 0;
    virtual CORBA::ULong sub_total() = 0;
    virtual void sub_total(CORBA::ULong _sub_total) = 0;

    static const VISOps_Info *_desc();
    static OrderItem_ops_ptr _nil() { return (OrderItem_ops_ptr)NULL; }
    static OrderItem_ops_ptr _downcast(PortableServer::ServantBase* _servant);
  protected:
    static const VISOps_Info _ops_info;
  };

  #ifndef PetStore_OrderOp_var_
  #define PetStore_OrderOp_var_

  class  OrderOp;

  typedef OrderOp* OrderOp_ptr;
  typedef OrderOp_ptr OrderOpRef;
   VISistream& _marshal_out(VISistream&, OrderOp_ptr&);
   VISostream& _marshal_in(VISostream&, const OrderOp_ptr);
  class  OrderOp_out;

  class  OrderOp_var : public CORBA::_var {
  friend class OrderOp_out;
   private:
    OrderOp_ptr _ptr;

   public:
    OrderOp_var();
    OrderOp_var(OrderOp_ptr);
    OrderOp_var(const OrderOp_var &);
    virtual ~OrderOp_var();

    static OrderOp_ptr _duplicate(OrderOp_ptr);
    static void _release(OrderOp_ptr);

    OrderOp_var& operator=(const OrderOp_var& _var);
    OrderOp_var& operator=(OrderOp_ptr);

    operator OrderOp*() const { return _ptr; }
    OrderOp* operator->() const { return _ptr; }

    OrderOp_ptr in() const { return _ptr; }
    OrderOp_ptr& inout() { return _ptr; }
    OrderOp_ptr& out();
    OrderOp_ptr _retn();

    friend VISostream& _marshal_in(VISostream&, const OrderOp_var&);
    friend VISistream& _marshal_out(VISistream&, OrderOp_var&);

    friend Istream& _marshal_out(Istream&, OrderOp_var&);
    friend Ostream& _marshal_in(Ostream&, const OrderOp_var&);
  };
  class  OrderOp_out {
   private:
    OrderOp_ptr& _ptr;
    static OrderOp* _nil() { return (OrderOp*)NULL; }

    void operator=(const OrderOp_out&);
    void operator=(const OrderOp_var&);
     public:
    OrderOp_out(const OrderOp_out& _o) : _ptr(_o._ptr) {}
    OrderOp_out(OrderOp_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    OrderOp_out(OrderOp_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    OrderOp_out& operator=(OrderOp* _p) {
      _ptr = _p;
      return *this;
    }
    operator OrderOp_ptr&() { return _ptr; }
    OrderOp_ptr& ptr() { return _ptr; }
    OrderOp* operator->() { return _ptr; }
  };

  #endif // PetStore_OrderOp_var_

  // idl interface: PetStore::OrderOp
  class  OrderOp : public virtual CORBA_Object {
   private:
    static const CORBA::TypeInfo _class_info;
    void operator=(const OrderOp&) {}

    protected:
    OrderOp() {}
    OrderOp(const OrderOp&) {}

   public:
    virtual ~OrderOp() {}
    static  const CORBA::TypeInfo *_desc();
    virtual const CORBA::TypeInfo *_type_info() const;
    virtual void *_safe_narrow(const CORBA::TypeInfo& ) const;
    static CORBA::Object*_factory();
    OrderOp_ptr _this();
    static OrderOp_ptr _duplicate(OrderOp_ptr _obj) {
      if (_obj) _obj->_ref();
      return _obj;
    }
    static OrderOp_ptr _nil() { return (OrderOp_ptr)NULL; }
    static OrderOp_ptr _narrow(CORBA::Object* _obj);
    static OrderOp_ptr _clone(OrderOp_ptr _obj) {
      CORBA::Object_var _obj_var(CORBA_Object::_clone(_obj));
      #if defined(_HPCC_BUG)
        return _narrow(_obj_var.operator CORBA::Object_ptr());
      #else
        return _narrow(_obj_var);
      #endif  // _HPCC_BUG
    }
    static OrderOp_ptr _bind(const char *_object_name = (const char*)NULL,
                             const char *_host_name = (const char*)NULL,
                             const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                             CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    static OrderOp_ptr _bind(const char *_poa_name,
                             const CORBA::OctetSequence& _id,
                             const char *_host_name = (const char*)NULL,
                             const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                             CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    virtual KeyID_ptr addItem(const char* _product_id,
                              const char* _product_name,
                              CORBA::ULong _quantity,
                              CORBA::ULong _unit_price,
                              CORBA::ULong _sub_total);
    virtual void delItem(KeyID_ptr _ID);

    friend VISostream& _marshal_in(VISostream& _strm, const OrderOp_ptr _obj);
    friend VISistream& _marshal_out(VISistream& _strm, OrderOp_ptr& _obj);
    friend Ostream& _marshal_in(Ostream& _strm, const OrderOp_ptr _obj);
    friend Istream& _marshal_out(Istream& _strm, OrderOp_ptr& _obj);
  };

  class OrderOp_ops;
  typedef OrderOp_ops* OrderOp_ops_ptr;

  class  OrderOp_ops  {
   public:
    OrderOp_ops() {}
    virtual ~OrderOp_ops () {}

    virtual KeyID_ptr addItem(const char* _product_id,
                              const char* _product_name,
                              CORBA::ULong _quantity,
                              CORBA::ULong _unit_price,
                              CORBA::ULong _sub_total) = 0;
    virtual void delItem(KeyID_ptr _ID) = 0;

    static const VISOps_Info *_desc();
    static OrderOp_ops_ptr _nil() { return (OrderOp_ops_ptr)NULL; }
    static OrderOp_ops_ptr _downcast(PortableServer::ServantBase* _servant);
  protected:
    static const VISOps_Info _ops_info;
  };

  // idl struct: PetStore::OrderItemInfo
  struct  OrderItemInfo {

    CORBA::String_var orderItem_id;
    CORBA::String_var product_id;
    CORBA::String_var product_name;
    CORBA::ULong quantity;
    CORBA::ULong unit_price;
    CORBA::ULong sub_total;
    #if defined(MSVCNEWDLL_BUG)
      void *operator new(size_t ts);
      void *operator new(size_t ts, char*, int) {return operator new(ts);}
      void operator delete(void *p);
    #endif // MSVCNEWDLL_BUG
    friend VISostream& _marshal_in(VISostream&, const OrderItemInfo&);
    friend VISistream& _marshal_out(VISistream&, OrderItemInfo&);

    friend VISostream& _marshal_in(VISostream& _strm, const OrderItemInfo* _obj);
    friend VISistream& _marshal_out(VISistream& _strm, OrderItemInfo*& _obj);

    friend void _internal_pretty_print(VISostream&, const OrderItemInfo&);
    friend Ostream& _marshal_in(Ostream&, const OrderItemInfo&);
    friend Istream& _marshal_out(Istream&, OrderItemInfo&);
    friend Istream& _marshal_out(Istream&, OrderItemInfo*&);
  };
  typedef OrderItemInfo * OrderItemInfo_ptr;

  class  OrderItemInfo_out;

  class  OrderItemInfo_var {
    friend class OrderItemInfo_out;
  private:
    OrderItemInfo_ptr _ptr;
  public:
    OrderItemInfo_var() : _ptr((OrderItemInfo*)NULL) {}
    OrderItemInfo_var(OrderItemInfo *_p) : _ptr(_p) {}

    OrderItemInfo_var(const OrderItemInfo_var& _var) {
      _ptr = _var._ptr ? (new OrderItemInfo(*_var._ptr)) : (OrderItemInfo*)NULL;
    }

    virtual ~OrderItemInfo_var() { delete _ptr; }
    OrderItemInfo_var& operator=(OrderItemInfo* _p);
    OrderItemInfo_var& operator=(const OrderItemInfo_var& _var);
    operator OrderItemInfo*() const { return _ptr; }

    OrderItemInfo* operator->() { return _ptr; }
    const OrderItemInfo* operator->() const { return _ptr; }

    operator OrderItemInfo&() { return *_ptr; }
    operator const OrderItemInfo&() const { return *_ptr; }

    const OrderItemInfo& in() const { return *_ptr; }
    OrderItemInfo& inout() { return *_ptr; }
    OrderItemInfo_ptr& out() {
      delete _ptr;
      _ptr = (OrderItemInfo_ptr) NULL;
      return _ptr;
    }

    OrderItemInfo_ptr _retn() {
      OrderItemInfo_ptr temp_ptr;
      temp_ptr = _ptr;
      _ptr = (OrderItemInfo_ptr) NULL;
      return temp_ptr;
    }
    friend VISostream& _marshal_in(VISostream&, const OrderItemInfo_var&);
    friend VISistream& _marshal_out(VISistream&, OrderItemInfo_var&);
  };
  class  OrderItemInfo_out {
   private:
    OrderItemInfo_ptr& _ptr;
    static OrderItemInfo* _nil() { return (OrderItemInfo*)NULL; }

    void operator=(const OrderItemInfo_out&);
    void operator=(const OrderItemInfo_var&);
     public:
    OrderItemInfo_out(const OrderItemInfo_out& _o) : _ptr(_o._ptr) {}
    OrderItemInfo_out(OrderItemInfo_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    OrderItemInfo_out(OrderItemInfo_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    OrderItemInfo_out& operator=(OrderItemInfo* _p) {
      _ptr = _p;
      return *this;
    }
    operator OrderItemInfo_ptr&() { return _ptr; }
    OrderItemInfo_ptr& ptr() { return _ptr; }
    OrderItemInfo* operator->() { return _ptr; }
  };
  class  OrderItemInfoList;
  typedef OrderItemInfoList* OrderItemInfoList_ptr;

  class  OrderItemInfoList : private VISResource {
  private:
    OrderItemInfo *_contents;
    CORBA::ULong _count;
    CORBA::ULong _num_allocated;
    CORBA::Boolean _release_flag;

  public:
    #if defined(MSVCNEWDLL_BUG)
      void *operator new(size_t ts);
      void *operator new(size_t ts, char*, int) {return operator new(ts);}
      void operator delete(void *p);
    #endif // MSVCNEWDLL_BUG

    static OrderItemInfo *allocbuf(CORBA::ULong _nelems);
    static void freebuf(OrderItemInfo* _data);

    OrderItemInfoList(CORBA::ULong _max = 0);
    OrderItemInfoList(CORBA::ULong max, CORBA::ULong length,
                      OrderItemInfo *_data,
                      CORBA::Boolean release = (CORBA::Boolean)0);
    OrderItemInfoList(const OrderItemInfoList&);
    virtual ~OrderItemInfoList();

    OrderItemInfoList& operator=(const OrderItemInfoList&);
    OrderItemInfoList& operator=(const OrderItemInfoList* _obj) {
      assert(_obj);
      return operator=(*_obj);
    }

    CORBA::ULong maximum() const { return _num_allocated; }

    void length(CORBA::ULong _len);
    CORBA::ULong length() const { return _count; }

    void replace(CORBA::ULong _max, CORBA::ULong _len, OrderItemInfo *_data,
                 CORBA::Boolean _release = (CORBA::Boolean)0);

    OrderItemInfo *get_buffer(CORBA::Boolean orphan = (CORBA::Boolean) 0);
    const OrderItemInfo* get_buffer() const;

    OrderItemInfo& operator[](CORBA::ULong _index);
    const OrderItemInfo& operator[](CORBA::ULong _index) const;

    static OrderItemInfoList *_duplicate(OrderItemInfoList* _ptr) {
      if (_ptr) _ptr->_ref();
      return _ptr;
    }

    static void _release(OrderItemInfoList *_ptr) {
      if (_ptr && (_ptr->_deref() == 0L)) 
        delete _ptr;
    }

    friend VISostream& _marshal_in(VISostream&, const OrderItemInfoList&);
    friend VISistream& _marshal_out(VISistream&, OrderItemInfoList&);

    friend VISostream& _marshal_in(VISostream& _strm, const OrderItemInfoList* _obj);
    friend VISistream& _marshal_out(VISistream& _strm, OrderItemInfoList_ptr& _obj);

    friend void _internal_pretty_print(VISostream&, const OrderItemInfoList&);

    friend Ostream& _marshal_in(Ostream&, const OrderItemInfoList&);
    friend Istream& _marshal_out(Istream&, OrderItemInfoList&);
    friend Istream& _marshal_out(Istream&, OrderItemInfoList_ptr&);
  };

  class  OrderItemInfoList_out;

  class  OrderItemInfoList_var {
    friend class OrderItemInfoList_out;
     private:
    OrderItemInfoList *_ptr;
     public:
    OrderItemInfoList_var() : _ptr((OrderItemInfoList*)NULL) {}
    OrderItemInfoList_var(OrderItemInfoList *_p) : _ptr(_p) {}
    OrderItemInfoList_var(const OrderItemInfoList_var& _var) :_ptr(OrderItemInfoList::_duplicate(_var._ptr)){}
    virtual ~OrderItemInfoList_var() { OrderItemInfoList::_release(_ptr); }
    OrderItemInfoList_var& operator=(OrderItemInfoList* _p) {
      OrderItemInfoList::_release(_ptr);
      _ptr = _p;
      return *this;
    }

    OrderItemInfoList_var& operator=(const OrderItemInfoList_var& _var) {
      OrderItemInfoList::_release(_ptr);
      _ptr = OrderItemInfoList::_duplicate(_var._ptr);
      return *this;
    }
    operator OrderItemInfoList*() const { return _ptr; }

    OrderItemInfoList* operator->() { return _ptr; }
    const OrderItemInfoList* operator->() const { return _ptr; }

    operator OrderItemInfoList&() { return *_ptr; }
    operator const OrderItemInfoList&() const { return *_ptr; }

    const OrderItemInfoList& in() const { return *_ptr; }
    OrderItemInfoList& inout() { return *_ptr; }
    OrderItemInfoList_ptr & out() {
      OrderItemInfoList::_release(_ptr);
      _ptr = (OrderItemInfoList *)NULL;
      return _ptr;
    }

    OrderItemInfoList* _retn() {
      OrderItemInfoList* _tmp_ptr;
      _tmp_ptr = _ptr;
      _ptr = (OrderItemInfoList *)NULL;
      return _tmp_ptr;
    }
    friend VISostream& _marshal_in(VISostream&, const OrderItemInfoList_var&);
    friend VISistream& _marshal_out(VISistream&, OrderItemInfoList_var&);

    OrderItemInfo& operator[](CORBA::ULong _index);
    const OrderItemInfo& operator[](CORBA::ULong _index) const;

  };
  class  OrderItemInfoList_out {
   private:
    OrderItemInfoList_ptr& _ptr;
    static OrderItemInfoList* _nil() { return (OrderItemInfoList*)NULL; }

    void operator=(const OrderItemInfoList_out&);
    void operator=(const OrderItemInfoList_var&);
     public:
    OrderItemInfoList_out(const OrderItemInfoList_out& _o) : _ptr(_o._ptr) {}
    OrderItemInfoList_out(OrderItemInfoList_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    OrderItemInfoList_out(OrderItemInfoList_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    OrderItemInfoList_out& operator=(OrderItemInfoList* _p) {
      _ptr = _p;
      return *this;
    }
    operator OrderItemInfoList_ptr&() { return _ptr; }
    OrderItemInfoList_ptr& ptr() { return _ptr; }
  };
  #ifndef PetStore_OrderItemHomeExplicit_var_
  #define PetStore_OrderItemHomeExplicit_var_

  class  OrderItemHomeExplicit;

  typedef OrderItemHomeExplicit* OrderItemHomeExplicit_ptr;
  typedef OrderItemHomeExplicit_ptr OrderItemHomeExplicitRef;
   VISistream& _marshal_out(VISistream&, OrderItemHomeExplicit_ptr&);
   VISostream& _marshal_in(VISostream&, const OrderItemHomeExplicit_ptr);
  class  OrderItemHomeExplicit_out;

  class  OrderItemHomeExplicit_var : public CORBA::_var {
  friend class OrderItemHomeExplicit_out;
   private:
    OrderItemHomeExplicit_ptr _ptr;

   public:
    OrderItemHomeExplicit_var();
    OrderItemHomeExplicit_var(OrderItemHomeExplicit_ptr);
    OrderItemHomeExplicit_var(const OrderItemHomeExplicit_var &);
    virtual ~OrderItemHomeExplicit_var();

    static OrderItemHomeExplicit_ptr _duplicate(OrderItemHomeExplicit_ptr);
    static void _release(OrderItemHomeExplicit_ptr);

    OrderItemHomeExplicit_var& operator=(const OrderItemHomeExplicit_var& _var);
    OrderItemHomeExplicit_var& operator=(OrderItemHomeExplicit_ptr);

    operator OrderItemHomeExplicit*() const { return _ptr; }
    OrderItemHomeExplicit* operator->() const { return _ptr; }

    OrderItemHomeExplicit_ptr in() const { return _ptr; }
    OrderItemHomeExplicit_ptr& inout() { return _ptr; }
    OrderItemHomeExplicit_ptr& out();
    OrderItemHomeExplicit_ptr _retn();

    friend VISostream& _marshal_in(VISostream&, const OrderItemHomeExplicit_var&);
    friend VISistream& _marshal_out(VISistream&, OrderItemHomeExplicit_var&);

    friend Istream& _marshal_out(Istream&, OrderItemHomeExplicit_var&);
    friend Ostream& _marshal_in(Ostream&, const OrderItemHomeExplicit_var&);
  };
  class  OrderItemHomeExplicit_out {
   private:
    OrderItemHomeExplicit_ptr& _ptr;
    static OrderItemHomeExplicit* _nil() { return (OrderItemHomeExplicit*)NULL; }

    void operator=(const OrderItemHomeExplicit_out&);
    void operator=(const OrderItemHomeExplicit_var&);
     public:
    OrderItemHomeExplicit_out(const OrderItemHomeExplicit_out& _o) : _ptr(_o._ptr) {}
    OrderItemHomeExplicit_out(OrderItemHomeExplicit_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    OrderItemHomeExplicit_out(OrderItemHomeExplicit_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    OrderItemHomeExplicit_out& operator=(OrderItemHomeExplicit* _p) {
      _ptr = _p;
      return *this;
    }
    operator OrderItemHomeExplicit_ptr&() { return _ptr; }
    OrderItemHomeExplicit_ptr& ptr() { return _ptr; }
    OrderItemHomeExplicit* operator->() { return _ptr; }
  };

  #endif // PetStore_OrderItemHomeExplicit_var_

  // idl interface: PetStore::OrderItemHomeExplicit
  class  OrderItemHomeExplicit : public virtual ::Components::CCMHome, public virtual CORBA_Object {
   private:
    static const CORBA::TypeInfo _class_info;
    void operator=(const OrderItemHomeExplicit&) {}

    protected:
    OrderItemHomeExplicit() {}
    OrderItemHomeExplicit(const OrderItemHomeExplicit&) {}

   public:
    virtual ~OrderItemHomeExplicit() {}
    static  const CORBA::TypeInfo *_desc();
    virtual const CORBA::TypeInfo *_type_info() const;
    virtual void *_safe_narrow(const CORBA::TypeInfo& ) const;
    static CORBA::Object*_factory();
    OrderItemHomeExplicit_ptr _this();
    static OrderItemHomeExplicit_ptr _duplicate(OrderItemHomeExplicit_ptr _obj) {
      if (_obj) _obj->_ref();
      return _obj;
    }
    static OrderItemHomeExplicit_ptr _nil() { return (OrderItemHomeExplicit_ptr)NULL; }
    static OrderItemHomeExplicit_ptr _narrow(CORBA::Object* _obj);
    static OrderItemHomeExplicit_ptr _clone(OrderItemHomeExplicit_ptr _obj) {
      CORBA::Object_var _obj_var(CORBA_Object::_clone(_obj));
      #if defined(_HPCC_BUG)
        return _narrow(_obj_var.operator CORBA::Object_ptr());
      #else
        return _narrow(_obj_var);
      #endif  // _HPCC_BUG
    }
    static OrderItemHomeExplicit_ptr _bind(const char *_object_name = (const char*)NULL,
                                           const char *_host_name = (const char*)NULL,
                                           const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                                           CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    static OrderItemHomeExplicit_ptr _bind(const char *_poa_name,
                                           const CORBA::OctetSequence& _id,
                                           const char *_host_name = (const char*)NULL,
                                           const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                                           CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    friend VISostream& _marshal_in(VISostream& _strm, const OrderItemHomeExplicit_ptr _obj);
    friend VISistream& _marshal_out(VISistream& _strm, OrderItemHomeExplicit_ptr& _obj);
    friend Ostream& _marshal_in(Ostream& _strm, const OrderItemHomeExplicit_ptr _obj);
    friend Istream& _marshal_out(Istream& _strm, OrderItemHomeExplicit_ptr& _obj);
  };

  class OrderItemHomeExplicit_ops;
  typedef OrderItemHomeExplicit_ops* OrderItemHomeExplicit_ops_ptr;

  class  OrderItemHomeExplicit_ops  {
   public:
    OrderItemHomeExplicit_ops() {}
    virtual ~OrderItemHomeExplicit_ops () {}

    static const VISOps_Info *_desc();
    static OrderItemHomeExplicit_ops_ptr _nil() { return (OrderItemHomeExplicit_ops_ptr)NULL; }
    static OrderItemHomeExplicit_ops_ptr _downcast(PortableServer::ServantBase* _servant);
  protected:
    static const VISOps_Info _ops_info;
  };

  #ifndef PetStore_OrderItemHomeImplicit_var_
  #define PetStore_OrderItemHomeImplicit_var_

  class  OrderItemHomeImplicit;

  typedef OrderItemHomeImplicit* OrderItemHomeImplicit_ptr;
  typedef OrderItemHomeImplicit_ptr OrderItemHomeImplicitRef;
   VISistream& _marshal_out(VISistream&, OrderItemHomeImplicit_ptr&);
   VISostream& _marshal_in(VISostream&, const OrderItemHomeImplicit_ptr);
  class  OrderItemHomeImplicit_out;

  class  OrderItemHomeImplicit_var : public CORBA::_var {
  friend class OrderItemHomeImplicit_out;
   private:
    OrderItemHomeImplicit_ptr _ptr;

   public:
    OrderItemHomeImplicit_var();
    OrderItemHomeImplicit_var(OrderItemHomeImplicit_ptr);
    OrderItemHomeImplicit_var(const OrderItemHomeImplicit_var &);
    virtual ~OrderItemHomeImplicit_var();

    static OrderItemHomeImplicit_ptr _duplicate(OrderItemHomeImplicit_ptr);
    static void _release(OrderItemHomeImplicit_ptr);

    OrderItemHomeImplicit_var& operator=(const OrderItemHomeImplicit_var& _var);
    OrderItemHomeImplicit_var& operator=(OrderItemHomeImplicit_ptr);

    operator OrderItemHomeImplicit*() const { return _ptr; }
    OrderItemHomeImplicit* operator->() const { return _ptr; }

    OrderItemHomeImplicit_ptr in() const { return _ptr; }
    OrderItemHomeImplicit_ptr& inout() { return _ptr; }
    OrderItemHomeImplicit_ptr& out();
    OrderItemHomeImplicit_ptr _retn();

    friend VISostream& _marshal_in(VISostream&, const OrderItemHomeImplicit_var&);
    friend VISistream& _marshal_out(VISistream&, OrderItemHomeImplicit_var&);

    friend Istream& _marshal_out(Istream&, OrderItemHomeImplicit_var&);
    friend Ostream& _marshal_in(Ostream&, const OrderItemHomeImplicit_var&);
  };
  class  OrderItemHomeImplicit_out {
   private:
    OrderItemHomeImplicit_ptr& _ptr;
    static OrderItemHomeImplicit* _nil() { return (OrderItemHomeImplicit*)NULL; }

    void operator=(const OrderItemHomeImplicit_out&);
    void operator=(const OrderItemHomeImplicit_var&);
     public:
    OrderItemHomeImplicit_out(const OrderItemHomeImplicit_out& _o) : _ptr(_o._ptr) {}
    OrderItemHomeImplicit_out(OrderItemHomeImplicit_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    OrderItemHomeImplicit_out(OrderItemHomeImplicit_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    OrderItemHomeImplicit_out& operator=(OrderItemHomeImplicit* _p) {
      _ptr = _p;
      return *this;
    }
    operator OrderItemHomeImplicit_ptr&() { return _ptr; }
    OrderItemHomeImplicit_ptr& ptr() { return _ptr; }
    OrderItemHomeImplicit* operator->() { return _ptr; }
  };

  #endif // PetStore_OrderItemHomeImplicit_var_

  // idl interface: PetStore::OrderItemHomeImplicit
  class  OrderItemHomeImplicit : public virtual CORBA_Object {
   private:
    static const CORBA::TypeInfo _class_info;
    void operator=(const OrderItemHomeImplicit&) {}

    protected:
    OrderItemHomeImplicit() {}
    OrderItemHomeImplicit(const OrderItemHomeImplicit&) {}

   public:
    virtual ~OrderItemHomeImplicit() {}
    static  const CORBA::TypeInfo *_desc();
    virtual const CORBA::TypeInfo *_type_info() const;
    virtual void *_safe_narrow(const CORBA::TypeInfo& ) const;
    static CORBA::Object*_factory();
    OrderItemHomeImplicit_ptr _this();
    static OrderItemHomeImplicit_ptr _duplicate(OrderItemHomeImplicit_ptr _obj) {
      if (_obj) _obj->_ref();
      return _obj;
    }
    static OrderItemHomeImplicit_ptr _nil() { return (OrderItemHomeImplicit_ptr)NULL; }
    static OrderItemHomeImplicit_ptr _narrow(CORBA::Object* _obj);
    static OrderItemHomeImplicit_ptr _clone(OrderItemHomeImplicit_ptr _obj) {
      CORBA::Object_var _obj_var(CORBA_Object::_clone(_obj));
      #if defined(_HPCC_BUG)
        return _narrow(_obj_var.operator CORBA::Object_ptr());
      #else
        return _narrow(_obj_var);
      #endif  // _HPCC_BUG
    }
    static OrderItemHomeImplicit_ptr _bind(const char *_object_name = (const char*)NULL,
                                           const char *_host_name = (const char*)NULL,
                                           const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                                           CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    static OrderItemHomeImplicit_ptr _bind(const char *_poa_name,
                                           const CORBA::OctetSequence& _id,
                                           const char *_host_name = (const char*)NULL,
                                           const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                                           CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    virtual OrderItem_ptr create(KeyID_ptr _pkey);
    virtual OrderItem_ptr find_by_primary_key(KeyID_ptr _pkey);
    virtual void remove(KeyID_ptr _pkey);
    virtual KeyID_ptr get_primary_key(OrderItem_ptr _comp);

    friend VISostream& _marshal_in(VISostream& _strm, const OrderItemHomeImplicit_ptr _obj);
    friend VISistream& _marshal_out(VISistream& _strm, OrderItemHomeImplicit_ptr& _obj);
    friend Ostream& _marshal_in(Ostream& _strm, const OrderItemHomeImplicit_ptr _obj);
    friend Istream& _marshal_out(Istream& _strm, OrderItemHomeImplicit_ptr& _obj);
  };

  class OrderItemHomeImplicit_ops;
  typedef OrderItemHomeImplicit_ops* OrderItemHomeImplicit_ops_ptr;

  class  OrderItemHomeImplicit_ops  {
   public:
    OrderItemHomeImplicit_ops() {}
    virtual ~OrderItemHomeImplicit_ops () {}

    virtual OrderItem_ptr create(KeyID_ptr _pkey) = 0;
    virtual OrderItem_ptr find_by_primary_key(KeyID_ptr _pkey) = 0;
    virtual void remove(KeyID_ptr _pkey) = 0;
    virtual KeyID_ptr get_primary_key(OrderItem_ptr _comp) = 0;

    static const VISOps_Info *_desc();
    static OrderItemHomeImplicit_ops_ptr _nil() { return (OrderItemHomeImplicit_ops_ptr)NULL; }
    static OrderItemHomeImplicit_ops_ptr _downcast(PortableServer::ServantBase* _servant);
  protected:
    static const VISOps_Info _ops_info;
  };

  #ifndef PetStore_OrderItemHome_var_
  #define PetStore_OrderItemHome_var_

  class  OrderItemHome;

  typedef OrderItemHome* OrderItemHome_ptr;
  typedef OrderItemHome_ptr OrderItemHomeRef;
   VISistream& _marshal_out(VISistream&, OrderItemHome_ptr&);
   VISostream& _marshal_in(VISostream&, const OrderItemHome_ptr);
  class  OrderItemHome_out;

  class  OrderItemHome_var : public CORBA::_var {
  friend class OrderItemHome_out;
   private:
    OrderItemHome_ptr _ptr;

   public:
    OrderItemHome_var();
    OrderItemHome_var(OrderItemHome_ptr);
    OrderItemHome_var(const OrderItemHome_var &);
    virtual ~OrderItemHome_var();

    static OrderItemHome_ptr _duplicate(OrderItemHome_ptr);
    static void _release(OrderItemHome_ptr);

    OrderItemHome_var& operator=(const OrderItemHome_var& _var);
    OrderItemHome_var& operator=(OrderItemHome_ptr);

    operator OrderItemHome*() const { return _ptr; }
    OrderItemHome* operator->() const { return _ptr; }

    OrderItemHome_ptr in() const { return _ptr; }
    OrderItemHome_ptr& inout() { return _ptr; }
    OrderItemHome_ptr& out();
    OrderItemHome_ptr _retn();

    friend VISostream& _marshal_in(VISostream&, const OrderItemHome_var&);
    friend VISistream& _marshal_out(VISistream&, OrderItemHome_var&);

    friend Istream& _marshal_out(Istream&, OrderItemHome_var&);
    friend Ostream& _marshal_in(Ostream&, const OrderItemHome_var&);
  };
  class  OrderItemHome_out {
   private:
    OrderItemHome_ptr& _ptr;
    static OrderItemHome* _nil() { return (OrderItemHome*)NULL; }

    void operator=(const OrderItemHome_out&);
    void operator=(const OrderItemHome_var&);
     public:
    OrderItemHome_out(const OrderItemHome_out& _o) : _ptr(_o._ptr) {}
    OrderItemHome_out(OrderItemHome_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    OrderItemHome_out(OrderItemHome_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    OrderItemHome_out& operator=(OrderItemHome* _p) {
      _ptr = _p;
      return *this;
    }
    operator OrderItemHome_ptr&() { return _ptr; }
    OrderItemHome_ptr& ptr() { return _ptr; }
    OrderItemHome* operator->() { return _ptr; }
  };

  #endif // PetStore_OrderItemHome_var_

  // idl interface: PetStore::OrderItemHome
  class  OrderItemHome : public virtual OrderItemHomeExplicit, public virtual OrderItemHomeImplicit, public virtual CORBA_Object {
   private:
    static const CORBA::TypeInfo _class_info;
    void operator=(const OrderItemHome&) {}

    protected:
    OrderItemHome() {}
    OrderItemHome(const OrderItemHome&) {}

   public:
    virtual ~OrderItemHome() {}
    static  const CORBA::TypeInfo *_desc();
    virtual const CORBA::TypeInfo *_type_info() const;
    virtual void *_safe_narrow(const CORBA::TypeInfo& ) const;
    static CORBA::Object*_factory();
    OrderItemHome_ptr _this();
    static OrderItemHome_ptr _duplicate(OrderItemHome_ptr _obj) {
      if (_obj) _obj->_ref();
      return _obj;
    }
    static OrderItemHome_ptr _nil() { return (OrderItemHome_ptr)NULL; }
    static OrderItemHome_ptr _narrow(CORBA::Object* _obj);
    static OrderItemHome_ptr _clone(OrderItemHome_ptr _obj) {
      CORBA::Object_var _obj_var(CORBA_Object::_clone(_obj));
      #if defined(_HPCC_BUG)
        return _narrow(_obj_var.operator CORBA::Object_ptr());
      #else
        return _narrow(_obj_var);
      #endif  // _HPCC_BUG
    }
    static OrderItemHome_ptr _bind(const char *_object_name = (const char*)NULL,
                                   const char *_host_name = (const char*)NULL,
                                   const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                                   CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    static OrderItemHome_ptr _bind(const char *_poa_name,
                                   const CORBA::OctetSequence& _id,
                                   const char *_host_name = (const char*)NULL,
                                   const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                                   CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    friend VISostream& _marshal_in(VISostream& _strm, const OrderItemHome_ptr _obj);
    friend VISistream& _marshal_out(VISistream& _strm, OrderItemHome_ptr& _obj);
    friend Ostream& _marshal_in(Ostream& _strm, const OrderItemHome_ptr _obj);
    friend Istream& _marshal_out(Istream& _strm, OrderItemHome_ptr& _obj);
  };

  class OrderItemHome_ops;
  typedef OrderItemHome_ops* OrderItemHome_ops_ptr;

  class  OrderItemHome_ops  {
   public:
    OrderItemHome_ops() {}
    virtual ~OrderItemHome_ops () {}

    static const VISOps_Info *_desc();
    static OrderItemHome_ops_ptr _nil() { return (OrderItemHome_ops_ptr)NULL; }
    static OrderItemHome_ops_ptr _downcast(PortableServer::ServantBase* _servant);
  protected:
    static const VISOps_Info _ops_info;
  };

  #ifndef PetStore_Order_var_
  #define PetStore_Order_var_

  class  Order;

  typedef Order* Order_ptr;
  typedef Order_ptr OrderRef;
   VISistream& _marshal_out(VISistream&, Order_ptr&);
   VISostream& _marshal_in(VISostream&, const Order_ptr);
  class  Order_out;

  class  Order_var : public CORBA::_var {
  friend class Order_out;
   private:
    Order_ptr _ptr;

   public:
    Order_var();
    Order_var(Order_ptr);
    Order_var(const Order_var &);
    virtual ~Order_var();

    static Order_ptr _duplicate(Order_ptr);
    static void _release(Order_ptr);

    Order_var& operator=(const Order_var& _var);
    Order_var& operator=(Order_ptr);

    operator Order*() const { return _ptr; }
    Order* operator->() const { return _ptr; }

    Order_ptr in() const { return _ptr; }
    Order_ptr& inout() { return _ptr; }
    Order_ptr& out();
    Order_ptr _retn();

    friend VISostream& _marshal_in(VISostream&, const Order_var&);
    friend VISistream& _marshal_out(VISistream&, Order_var&);

    friend Istream& _marshal_out(Istream&, Order_var&);
    friend Ostream& _marshal_in(Ostream&, const Order_var&);
  };
  class  Order_out {
   private:
    Order_ptr& _ptr;
    static Order* _nil() { return (Order*)NULL; }

    void operator=(const Order_out&);
    void operator=(const Order_var&);
     public:
    Order_out(const Order_out& _o) : _ptr(_o._ptr) {}
    Order_out(Order_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    Order_out(Order_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    Order_out& operator=(Order* _p) {
      _ptr = _p;
      return *this;
    }
    operator Order_ptr&() { return _ptr; }
    Order_ptr& ptr() { return _ptr; }
    Order* operator->() { return _ptr; }
  };

  #endif // PetStore_Order_var_

  // idl interface: PetStore::Order
  class  Order : public virtual OrderOp, public virtual ::Components::CCMObject, public virtual CORBA_Object {
   private:
    static const CORBA::TypeInfo _class_info;
    void operator=(const Order&) {}

    protected:
    Order() {}
    Order(const Order&) {}

   public:
    virtual ~Order() {}
    static  const CORBA::TypeInfo *_desc();
    virtual const CORBA::TypeInfo *_type_info() const;
    virtual void *_safe_narrow(const CORBA::TypeInfo& ) const;
    static CORBA::Object*_factory();
    Order_ptr _this();
    static Order_ptr _duplicate(Order_ptr _obj) {
      if (_obj) _obj->_ref();
      return _obj;
    }
    static Order_ptr _nil() { return (Order_ptr)NULL; }
    static Order_ptr _narrow(CORBA::Object* _obj);
    static Order_ptr _clone(Order_ptr _obj) {
      CORBA::Object_var _obj_var(CORBA_Object::_clone(_obj));
      #if defined(_HPCC_BUG)
        return _narrow(_obj_var.operator CORBA::Object_ptr());
      #else
        return _narrow(_obj_var);
      #endif  // _HPCC_BUG
    }
    static Order_ptr _bind(const char *_object_name = (const char*)NULL,
                           const char *_host_name = (const char*)NULL,
                           const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                           CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    static Order_ptr _bind(const char *_poa_name,
                           const CORBA::OctetSequence& _id,
                           const char *_host_name = (const char*)NULL,
                           const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                           CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    virtual KeyID_ptr order_id();
    virtual char* user_name();
    virtual void user_name(const char* _user_name);
    virtual char* user_address();
    virtual void user_address(const char* _user_address);
    virtual OrderItemInfoList* order_items();
    virtual void order_items(const OrderItemInfoList& _order_items);
    virtual char* order_time();
    virtual void order_time(const char* _order_time);
    virtual CORBA::ULong sub_total();
    virtual void sub_total(CORBA::ULong _sub_total);
    virtual void connect_the_OrderItemHome(OrderItemHome_ptr _conn);
    virtual OrderItemHome_ptr disconnect_the_OrderItemHome();
    virtual OrderItemHome_ptr get_connection_the_OrderItemHome();

    friend VISostream& _marshal_in(VISostream& _strm, const Order_ptr _obj);
    friend VISistream& _marshal_out(VISistream& _strm, Order_ptr& _obj);
    friend Ostream& _marshal_in(Ostream& _strm, const Order_ptr _obj);
    friend Istream& _marshal_out(Istream& _strm, Order_ptr& _obj);
  };

  class Order_ops;
  typedef Order_ops* Order_ops_ptr;

  class  Order_ops  {
   public:
    Order_ops() {}
    virtual ~Order_ops () {}

    virtual KeyID_ptr order_id() = 0;
    virtual char* user_name() = 0;
    virtual void user_name(const char* _user_name) = 0;
    virtual char* user_address() = 0;
    virtual void user_address(const char* _user_address) = 0;
    virtual OrderItemInfoList* order_items() = 0;
    virtual void order_items(const OrderItemInfoList& _order_items) = 0;
    virtual char* order_time() = 0;
    virtual void order_time(const char* _order_time) = 0;
    virtual CORBA::ULong sub_total() = 0;
    virtual void sub_total(CORBA::ULong _sub_total) = 0;
    virtual void connect_the_OrderItemHome(OrderItemHome_ptr _conn) = 0;
    virtual OrderItemHome_ptr disconnect_the_OrderItemHome() = 0;
    virtual OrderItemHome_ptr get_connection_the_OrderItemHome() = 0;

    static const VISOps_Info *_desc();
    static Order_ops_ptr _nil() { return (Order_ops_ptr)NULL; }
    static Order_ops_ptr _downcast(PortableServer::ServantBase* _servant);
  protected:
    static const VISOps_Info _ops_info;
  };

  // idl struct: PetStore::SalesRecordInfo
  struct  SalesRecordInfo {

    CORBA::String_var product_id;
    CORBA::String_var product_name;
    CORBA::ULong quantity;
    CORBA::ULong unit_price;
    CORBA::ULong sub_total;
    #if defined(MSVCNEWDLL_BUG)
      void *operator new(size_t ts);
      void *operator new(size_t ts, char*, int) {return operator new(ts);}
      void operator delete(void *p);
    #endif // MSVCNEWDLL_BUG
    friend VISostream& _marshal_in(VISostream&, const SalesRecordInfo&);
    friend VISistream& _marshal_out(VISistream&, SalesRecordInfo&);

    friend VISostream& _marshal_in(VISostream& _strm, const SalesRecordInfo* _obj);
    friend VISistream& _marshal_out(VISistream& _strm, SalesRecordInfo*& _obj);

    friend void _internal_pretty_print(VISostream&, const SalesRecordInfo&);
    friend Ostream& _marshal_in(Ostream&, const SalesRecordInfo&);
    friend Istream& _marshal_out(Istream&, SalesRecordInfo&);
    friend Istream& _marshal_out(Istream&, SalesRecordInfo*&);
  };
  typedef SalesRecordInfo * SalesRecordInfo_ptr;

  class  SalesRecordInfo_out;

  class  SalesRecordInfo_var {
    friend class SalesRecordInfo_out;
  private:
    SalesRecordInfo_ptr _ptr;
  public:
    SalesRecordInfo_var() : _ptr((SalesRecordInfo*)NULL) {}
    SalesRecordInfo_var(SalesRecordInfo *_p) : _ptr(_p) {}

    SalesRecordInfo_var(const SalesRecordInfo_var& _var) {
      _ptr = _var._ptr ? (new SalesRecordInfo(*_var._ptr)) : (SalesRecordInfo*)NULL;
    }

    virtual ~SalesRecordInfo_var() { delete _ptr; }
    SalesRecordInfo_var& operator=(SalesRecordInfo* _p);
    SalesRecordInfo_var& operator=(const SalesRecordInfo_var& _var);
    operator SalesRecordInfo*() const { return _ptr; }

    SalesRecordInfo* operator->() { return _ptr; }
    const SalesRecordInfo* operator->() const { return _ptr; }

    operator SalesRecordInfo&() { return *_ptr; }
    operator const SalesRecordInfo&() const { return *_ptr; }

    const SalesRecordInfo& in() const { return *_ptr; }
    SalesRecordInfo& inout() { return *_ptr; }
    SalesRecordInfo_ptr& out() {
      delete _ptr;
      _ptr = (SalesRecordInfo_ptr) NULL;
      return _ptr;
    }

    SalesRecordInfo_ptr _retn() {
      SalesRecordInfo_ptr temp_ptr;
      temp_ptr = _ptr;
      _ptr = (SalesRecordInfo_ptr) NULL;
      return temp_ptr;
    }
    friend VISostream& _marshal_in(VISostream&, const SalesRecordInfo_var&);
    friend VISistream& _marshal_out(VISistream&, SalesRecordInfo_var&);
  };
  class  SalesRecordInfo_out {
   private:
    SalesRecordInfo_ptr& _ptr;
    static SalesRecordInfo* _nil() { return (SalesRecordInfo*)NULL; }

    void operator=(const SalesRecordInfo_out&);
    void operator=(const SalesRecordInfo_var&);
     public:
    SalesRecordInfo_out(const SalesRecordInfo_out& _o) : _ptr(_o._ptr) {}
    SalesRecordInfo_out(SalesRecordInfo_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    SalesRecordInfo_out(SalesRecordInfo_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    SalesRecordInfo_out& operator=(SalesRecordInfo* _p) {
      _ptr = _p;
      return *this;
    }
    operator SalesRecordInfo_ptr&() { return _ptr; }
    SalesRecordInfo_ptr& ptr() { return _ptr; }
    SalesRecordInfo* operator->() { return _ptr; }
  };
  class  SalesRecordInfoList;
  typedef SalesRecordInfoList* SalesRecordInfoList_ptr;

  class  SalesRecordInfoList : private VISResource {
  private:
    SalesRecordInfo *_contents;
    CORBA::ULong _count;
    CORBA::ULong _num_allocated;
    CORBA::Boolean _release_flag;

  public:
    #if defined(MSVCNEWDLL_BUG)
      void *operator new(size_t ts);
      void *operator new(size_t ts, char*, int) {return operator new(ts);}
      void operator delete(void *p);
    #endif // MSVCNEWDLL_BUG

    static SalesRecordInfo *allocbuf(CORBA::ULong _nelems);
    static void freebuf(SalesRecordInfo* _data);

    SalesRecordInfoList(CORBA::ULong _max = 0);
    SalesRecordInfoList(CORBA::ULong max, CORBA::ULong length,
                        SalesRecordInfo *_data,
                        CORBA::Boolean release = (CORBA::Boolean)0);
    SalesRecordInfoList(const SalesRecordInfoList&);
    virtual ~SalesRecordInfoList();

    SalesRecordInfoList& operator=(const SalesRecordInfoList&);
    SalesRecordInfoList& operator=(const SalesRecordInfoList* _obj) {
      assert(_obj);
      return operator=(*_obj);
    }

    CORBA::ULong maximum() const { return _num_allocated; }

    void length(CORBA::ULong _len);
    CORBA::ULong length() const { return _count; }

    void replace(CORBA::ULong _max, CORBA::ULong _len, SalesRecordInfo *_data,
                 CORBA::Boolean _release = (CORBA::Boolean)0);

    SalesRecordInfo *get_buffer(CORBA::Boolean orphan = (CORBA::Boolean) 0);
    const SalesRecordInfo* get_buffer() const;

    SalesRecordInfo& operator[](CORBA::ULong _index);
    const SalesRecordInfo& operator[](CORBA::ULong _index) const;

    static SalesRecordInfoList *_duplicate(SalesRecordInfoList* _ptr) {
      if (_ptr) _ptr->_ref();
      return _ptr;
    }

    static void _release(SalesRecordInfoList *_ptr) {
      if (_ptr && (_ptr->_deref() == 0L)) 
        delete _ptr;
    }

    friend VISostream& _marshal_in(VISostream&, const SalesRecordInfoList&);
    friend VISistream& _marshal_out(VISistream&, SalesRecordInfoList&);

    friend VISostream& _marshal_in(VISostream& _strm, const SalesRecordInfoList* _obj);
    friend VISistream& _marshal_out(VISistream& _strm, SalesRecordInfoList_ptr& _obj);

    friend void _internal_pretty_print(VISostream&, const SalesRecordInfoList&);

    friend Ostream& _marshal_in(Ostream&, const SalesRecordInfoList&);
    friend Istream& _marshal_out(Istream&, SalesRecordInfoList&);
    friend Istream& _marshal_out(Istream&, SalesRecordInfoList_ptr&);
  };

  class  SalesRecordInfoList_out;

  class  SalesRecordInfoList_var {
    friend class SalesRecordInfoList_out;
     private:
    SalesRecordInfoList *_ptr;
     public:
    SalesRecordInfoList_var() : _ptr((SalesRecordInfoList*)NULL) {}
    SalesRecordInfoList_var(SalesRecordInfoList *_p) : _ptr(_p) {}
    SalesRecordInfoList_var(const SalesRecordInfoList_var& _var) :_ptr(SalesRecordInfoList::_duplicate(_var._ptr)){}
    virtual ~SalesRecordInfoList_var() { SalesRecordInfoList::_release(_ptr); }
    SalesRecordInfoList_var& operator=(SalesRecordInfoList* _p) {
      SalesRecordInfoList::_release(_ptr);
      _ptr = _p;
      return *this;
    }

    SalesRecordInfoList_var& operator=(const SalesRecordInfoList_var& _var) {
      SalesRecordInfoList::_release(_ptr);
      _ptr = SalesRecordInfoList::_duplicate(_var._ptr);
      return *this;
    }
    operator SalesRecordInfoList*() const { return _ptr; }

    SalesRecordInfoList* operator->() { return _ptr; }
    const SalesRecordInfoList* operator->() const { return _ptr; }

    operator SalesRecordInfoList&() { return *_ptr; }
    operator const SalesRecordInfoList&() const { return *_ptr; }

    const SalesRecordInfoList& in() const { return *_ptr; }
    SalesRecordInfoList& inout() { return *_ptr; }
    SalesRecordInfoList_ptr & out() {
      SalesRecordInfoList::_release(_ptr);
      _ptr = (SalesRecordInfoList *)NULL;
      return _ptr;
    }

    SalesRecordInfoList* _retn() {
      SalesRecordInfoList* _tmp_ptr;
      _tmp_ptr = _ptr;
      _ptr = (SalesRecordInfoList *)NULL;
      return _tmp_ptr;
    }
    friend VISostream& _marshal_in(VISostream&, const SalesRecordInfoList_var&);
    friend VISistream& _marshal_out(VISistream&, SalesRecordInfoList_var&);

    SalesRecordInfo& operator[](CORBA::ULong _index);
    const SalesRecordInfo& operator[](CORBA::ULong _index) const;

  };
  class  SalesRecordInfoList_out {
   private:
    SalesRecordInfoList_ptr& _ptr;
    static SalesRecordInfoList* _nil() { return (SalesRecordInfoList*)NULL; }

    void operator=(const SalesRecordInfoList_out&);
    void operator=(const SalesRecordInfoList_var&);
     public:
    SalesRecordInfoList_out(const SalesRecordInfoList_out& _o) : _ptr(_o._ptr) {}
    SalesRecordInfoList_out(SalesRecordInfoList_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    SalesRecordInfoList_out(SalesRecordInfoList_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    SalesRecordInfoList_out& operator=(SalesRecordInfoList* _p) {
      _ptr = _p;
      return *this;
    }
    operator SalesRecordInfoList_ptr&() { return _ptr; }
    SalesRecordInfoList_ptr& ptr() { return _ptr; }
  };
  // idl struct: PetStore::SalesRecord
  struct  SalesRecord {

    SalesRecordInfoList sales_record_list;
    CORBA::ULong sub_total;
    #if defined(MSVCNEWDLL_BUG)
      void *operator new(size_t ts);
      void *operator new(size_t ts, char*, int) {return operator new(ts);}
      void operator delete(void *p);
    #endif // MSVCNEWDLL_BUG
    friend VISostream& _marshal_in(VISostream&, const SalesRecord&);
    friend VISistream& _marshal_out(VISistream&, SalesRecord&);

    friend VISostream& _marshal_in(VISostream& _strm, const SalesRecord* _obj);
    friend VISistream& _marshal_out(VISistream& _strm, SalesRecord*& _obj);

    friend void _internal_pretty_print(VISostream&, const SalesRecord&);
    friend Ostream& _marshal_in(Ostream&, const SalesRecord&);
    friend Istream& _marshal_out(Istream&, SalesRecord&);
    friend Istream& _marshal_out(Istream&, SalesRecord*&);
  };
  typedef SalesRecord * SalesRecord_ptr;

  class  SalesRecord_out;

  class  SalesRecord_var {
    friend class SalesRecord_out;
  private:
    SalesRecord_ptr _ptr;
  public:
    SalesRecord_var() : _ptr((SalesRecord*)NULL) {}
    SalesRecord_var(SalesRecord *_p) : _ptr(_p) {}

    SalesRecord_var(const SalesRecord_var& _var) {
      _ptr = _var._ptr ? (new SalesRecord(*_var._ptr)) : (SalesRecord*)NULL;
    }

    virtual ~SalesRecord_var() { delete _ptr; }
    SalesRecord_var& operator=(SalesRecord* _p);
    SalesRecord_var& operator=(const SalesRecord_var& _var);
    operator SalesRecord*() const { return _ptr; }

    SalesRecord* operator->() { return _ptr; }
    const SalesRecord* operator->() const { return _ptr; }

    operator SalesRecord&() { return *_ptr; }
    operator const SalesRecord&() const { return *_ptr; }

    const SalesRecord& in() const { return *_ptr; }
    SalesRecord& inout() { return *_ptr; }
    SalesRecord_ptr& out() {
      delete _ptr;
      _ptr = (SalesRecord_ptr) NULL;
      return _ptr;
    }

    SalesRecord_ptr _retn() {
      SalesRecord_ptr temp_ptr;
      temp_ptr = _ptr;
      _ptr = (SalesRecord_ptr) NULL;
      return temp_ptr;
    }
    friend VISostream& _marshal_in(VISostream&, const SalesRecord_var&);
    friend VISistream& _marshal_out(VISistream&, SalesRecord_var&);
  };
  class  SalesRecord_out {
   private:
    SalesRecord_ptr& _ptr;
    static SalesRecord* _nil() { return (SalesRecord*)NULL; }

    void operator=(const SalesRecord_out&);
    void operator=(const SalesRecord_var&);
     public:
    SalesRecord_out(const SalesRecord_out& _o) : _ptr(_o._ptr) {}
    SalesRecord_out(SalesRecord_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    SalesRecord_out(SalesRecord_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    SalesRecord_out& operator=(SalesRecord* _p) {
      _ptr = _p;
      return *this;
    }
    operator SalesRecord_ptr&() { return _ptr; }
    SalesRecord_ptr& ptr() { return _ptr; }
    SalesRecord* operator->() { return _ptr; }
  };
  #ifndef PetStore_ShoppingCart_var_
  #define PetStore_ShoppingCart_var_

  class  ShoppingCart;

  typedef ShoppingCart* ShoppingCart_ptr;
  typedef ShoppingCart_ptr ShoppingCartRef;
   VISistream& _marshal_out(VISistream&, ShoppingCart_ptr&);
   VISostream& _marshal_in(VISostream&, const ShoppingCart_ptr);
  class  ShoppingCart_out;

  class  ShoppingCart_var : public CORBA::_var {
  friend class ShoppingCart_out;
   private:
    ShoppingCart_ptr _ptr;

   public:
    ShoppingCart_var();
    ShoppingCart_var(ShoppingCart_ptr);
    ShoppingCart_var(const ShoppingCart_var &);
    virtual ~ShoppingCart_var();

    static ShoppingCart_ptr _duplicate(ShoppingCart_ptr);
    static void _release(ShoppingCart_ptr);

    ShoppingCart_var& operator=(const ShoppingCart_var& _var);
    ShoppingCart_var& operator=(ShoppingCart_ptr);

    operator ShoppingCart*() const { return _ptr; }
    ShoppingCart* operator->() const { return _ptr; }

    ShoppingCart_ptr in() const { return _ptr; }
    ShoppingCart_ptr& inout() { return _ptr; }
    ShoppingCart_ptr& out();
    ShoppingCart_ptr _retn();

    friend VISostream& _marshal_in(VISostream&, const ShoppingCart_var&);
    friend VISistream& _marshal_out(VISistream&, ShoppingCart_var&);

    friend Istream& _marshal_out(Istream&, ShoppingCart_var&);
    friend Ostream& _marshal_in(Ostream&, const ShoppingCart_var&);
  };
  class  ShoppingCart_out {
   private:
    ShoppingCart_ptr& _ptr;
    static ShoppingCart* _nil() { return (ShoppingCart*)NULL; }

    void operator=(const ShoppingCart_out&);
    void operator=(const ShoppingCart_var&);
     public:
    ShoppingCart_out(const ShoppingCart_out& _o) : _ptr(_o._ptr) {}
    ShoppingCart_out(ShoppingCart_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    ShoppingCart_out(ShoppingCart_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    ShoppingCart_out& operator=(ShoppingCart* _p) {
      _ptr = _p;
      return *this;
    }
    operator ShoppingCart_ptr&() { return _ptr; }
    ShoppingCart_ptr& ptr() { return _ptr; }
    ShoppingCart* operator->() { return _ptr; }
  };

  #endif // PetStore_ShoppingCart_var_

  // idl interface: PetStore::ShoppingCart
  class  ShoppingCart : public virtual CORBA_Object {
   private:
    static const CORBA::TypeInfo _class_info;
    void operator=(const ShoppingCart&) {}

    protected:
    ShoppingCart() {}
    ShoppingCart(const ShoppingCart&) {}

   public:
    virtual ~ShoppingCart() {}
    static  const CORBA::TypeInfo *_desc();
    virtual const CORBA::TypeInfo *_type_info() const;
    virtual void *_safe_narrow(const CORBA::TypeInfo& ) const;
    static CORBA::Object*_factory();
    ShoppingCart_ptr _this();
    static ShoppingCart_ptr _duplicate(ShoppingCart_ptr _obj) {
      if (_obj) _obj->_ref();
      return _obj;
    }
    static ShoppingCart_ptr _nil() { return (ShoppingCart_ptr)NULL; }
    static ShoppingCart_ptr _narrow(CORBA::Object* _obj);
    static ShoppingCart_ptr _clone(ShoppingCart_ptr _obj) {
      CORBA::Object_var _obj_var(CORBA_Object::_clone(_obj));
      #if defined(_HPCC_BUG)
        return _narrow(_obj_var.operator CORBA::Object_ptr());
      #else
        return _narrow(_obj_var);
      #endif  // _HPCC_BUG
    }
    static ShoppingCart_ptr _bind(const char *_object_name = (const char*)NULL,
                                  const char *_host_name = (const char*)NULL,
                                  const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                                  CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    static ShoppingCart_ptr _bind(const char *_poa_name,
                                  const CORBA::OctetSequence& _id,
                                  const char *_host_name = (const char*)NULL,
                                  const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                                  CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    virtual SalesRecord* list();
    virtual void addProduct(const SalesRecordInfo& _productWanted);
    virtual void updateProductQty(const char* _productID, CORBA::ULong _newQty);
    virtual void deleteProduct(const char* _productID);
    virtual void clear();

    friend VISostream& _marshal_in(VISostream& _strm, const ShoppingCart_ptr _obj);
    friend VISistream& _marshal_out(VISistream& _strm, ShoppingCart_ptr& _obj);
    friend Ostream& _marshal_in(Ostream& _strm, const ShoppingCart_ptr _obj);
    friend Istream& _marshal_out(Istream& _strm, ShoppingCart_ptr& _obj);
  };

  class ShoppingCart_ops;
  typedef ShoppingCart_ops* ShoppingCart_ops_ptr;

  class  ShoppingCart_ops  {
   public:
    ShoppingCart_ops() {}
    virtual ~ShoppingCart_ops () {}

    virtual SalesRecord* list() = 0;
    virtual void addProduct(const SalesRecordInfo& _productWanted) = 0;
    virtual void updateProductQty(const char* _productID,
                                  CORBA::ULong _newQty) = 0;
    virtual void deleteProduct(const char* _productID) = 0;
    virtual void clear() = 0;

    static const VISOps_Info *_desc();
    static ShoppingCart_ops_ptr _nil() { return (ShoppingCart_ops_ptr)NULL; }
    static ShoppingCart_ops_ptr _downcast(PortableServer::ServantBase* _servant);
  protected:
    static const VISOps_Info _ops_info;
  };

  #ifndef PetStore_Cart_var_
  #define PetStore_Cart_var_

  class  Cart;

  typedef Cart* Cart_ptr;
  typedef Cart_ptr CartRef;
   VISistream& _marshal_out(VISistream&, Cart_ptr&);
   VISostream& _marshal_in(VISostream&, const Cart_ptr);
  class  Cart_out;

  class  Cart_var : public CORBA::_var {
  friend class Cart_out;
   private:
    Cart_ptr _ptr;

   public:
    Cart_var();
    Cart_var(Cart_ptr);
    Cart_var(const Cart_var &);
    virtual ~Cart_var();

    static Cart_ptr _duplicate(Cart_ptr);
    static void _release(Cart_ptr);

    Cart_var& operator=(const Cart_var& _var);
    Cart_var& operator=(Cart_ptr);

    operator Cart*() const { return _ptr; }
    Cart* operator->() const { return _ptr; }

    Cart_ptr in() const { return _ptr; }
    Cart_ptr& inout() { return _ptr; }
    Cart_ptr& out();
    Cart_ptr _retn();

    friend VISostream& _marshal_in(VISostream&, const Cart_var&);
    friend VISistream& _marshal_out(VISistream&, Cart_var&);

    friend Istream& _marshal_out(Istream&, Cart_var&);
    friend Ostream& _marshal_in(Ostream&, const Cart_var&);
  };
  class  Cart_out {
   private:
    Cart_ptr& _ptr;
    static Cart* _nil() { return (Cart*)NULL; }

    void operator=(const Cart_out&);
    void operator=(const Cart_var&);
     public:
    Cart_out(const Cart_out& _o) : _ptr(_o._ptr) {}
    Cart_out(Cart_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    Cart_out(Cart_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    Cart_out& operator=(Cart* _p) {
      _ptr = _p;
      return *this;
    }
    operator Cart_ptr&() { return _ptr; }
    Cart_ptr& ptr() { return _ptr; }
    Cart* operator->() { return _ptr; }
  };

  #endif // PetStore_Cart_var_

  // idl interface: PetStore::Cart
  class  Cart : public virtual ShoppingCart, public virtual ::Components::CCMObject, public virtual CORBA_Object {
   private:
    static const CORBA::TypeInfo _class_info;
    void operator=(const Cart&) {}

    protected:
    Cart() {}
    Cart(const Cart&) {}

   public:
    virtual ~Cart() {}
    static  const CORBA::TypeInfo *_desc();
    virtual const CORBA::TypeInfo *_type_info() const;
    virtual void *_safe_narrow(const CORBA::TypeInfo& ) const;
    static CORBA::Object*_factory();
    Cart_ptr _this();
    static Cart_ptr _duplicate(Cart_ptr _obj) {
      if (_obj) _obj->_ref();
      return _obj;
    }
    static Cart_ptr _nil() { return (Cart_ptr)NULL; }
    static Cart_ptr _narrow(CORBA::Object* _obj);
    static Cart_ptr _clone(Cart_ptr _obj) {
      CORBA::Object_var _obj_var(CORBA_Object::_clone(_obj));
      #if defined(_HPCC_BUG)
        return _narrow(_obj_var.operator CORBA::Object_ptr());
      #else
        return _narrow(_obj_var);
      #endif  // _HPCC_BUG
    }
    static Cart_ptr _bind(const char *_object_name = (const char*)NULL,
                          const char *_host_name = (const char*)NULL,
                          const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                          CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    static Cart_ptr _bind(const char *_poa_name,
                          const CORBA::OctetSequence& _id,
                          const char *_host_name = (const char*)NULL,
                          const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                          CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    friend VISostream& _marshal_in(VISostream& _strm, const Cart_ptr _obj);
    friend VISistream& _marshal_out(VISistream& _strm, Cart_ptr& _obj);
    friend Ostream& _marshal_in(Ostream& _strm, const Cart_ptr _obj);
    friend Istream& _marshal_out(Istream& _strm, Cart_ptr& _obj);
  };

  class Cart_ops;
  typedef Cart_ops* Cart_ops_ptr;

  class  Cart_ops  {
   public:
    Cart_ops() {}
    virtual ~Cart_ops () {}

    static const VISOps_Info *_desc();
    static Cart_ops_ptr _nil() { return (Cart_ops_ptr)NULL; }
    static Cart_ops_ptr _downcast(PortableServer::ServantBase* _servant);
  protected:
    static const VISOps_Info _ops_info;
  };

  #ifndef PetStore_SalesReport_var_
  #define PetStore_SalesReport_var_

  class  SalesReport;

  typedef SalesReport* SalesReport_ptr;

   VISistream& _marshal_out(VISistream&, SalesReport_ptr&);
   VISostream& _marshal_in(VISostream&, const SalesReport_ptr);

  class  SalesReport_out;
  class  SalesReport_var: public CORBA::_var {
    friend class SalesReport_out;
     public:
    SalesReport_var();
    SalesReport_var(SalesReport_ptr );
    SalesReport_var(const SalesReport_var &);

    virtual ~SalesReport_var();

    SalesReport_var& operator=(SalesReport_ptr);
    void operator=(const SalesReport_var & _v);

    static void _add_ref(SalesReport_ptr );
    static void _remove_ref(SalesReport_ptr );

    operator SalesReport*() const { return _ptr; }
    SalesReport* operator->() { return _ptr; }
    const SalesReport* operator->() const { return _ptr; }

    SalesReport_ptr in () const { return _ptr; }
    SalesReport_ptr& inout () { return _ptr; }
    SalesReport_ptr& out();
    SalesReport_ptr _retn();

    friend VISistream& _marshal_out(VISistream& _strm, SalesReport_var& _var);
    friend VISostream& _marshal_in(VISostream& _strm, const SalesReport_var& _var);

    friend Istream& _marshal_out(Istream&, SalesReport_var&);
    friend Ostream& _marshal_in(Ostream&, const SalesReport_var&);
     private:
    SalesReport_ptr _ptr;
  };
  class  SalesReport_out {
   private:
    SalesReport_ptr& _ptr;
    static SalesReport* _nil() { return (SalesReport*)NULL; }

    void operator=(const SalesReport_out&);
    void operator=(const SalesReport_var&);
     public:
    SalesReport_out(const SalesReport_out& _o) : _ptr(_o._ptr) {}
    SalesReport_out(SalesReport_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    SalesReport_out(SalesReport_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    SalesReport_out& operator=(SalesReport* _p) {
      _ptr = _p;
      return *this;
    }
    operator SalesReport_ptr&() { return _ptr; }
    SalesReport_ptr& ptr() { return _ptr; }
    SalesReport* operator->() { return _ptr; }
  };

  #endif	// PetStore_SalesReport_var_
  // idl valuetype: PetStore::SalesReport
  class  SalesReport  : public virtual CORBA::ValueBase,
                        public virtual ::Components::EventBase   {
    public:
      friend VISostream& _marshal_in(VISostream& _strm, const SalesReport_ptr _obj);
      friend VISistream& _marshal_out(VISistream& _strm, SalesReport_ptr& _obj);
      friend Ostream& _marshal_in(Ostream& _strm, const SalesReport_ptr _obj);
      friend Istream& _marshal_out(Istream& _strm, SalesReport_ptr& _obj);
      friend void _internal_pretty_print(VISostream& _strm, const SalesReport& _obj);
      friend void _internal_pretty_print(VISostream& _strm, const SalesReport* _obj);

      static SalesReport* _downcast(CORBA::ValueBase* vbptr);
      virtual void sub_total(const CORBA::ULong _sub_total) = 0;
      virtual const CORBA::ULong sub_total() const = 0;

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
       SalesReport() {}
        virtual ~SalesReport() {}

      private:
        void operator=(const SalesReport&);
  };
  class  OBV_SalesReport: public virtual SalesReport  {
    public:
      virtual void sub_total(const CORBA::ULong _sub_total) {
        _obv_sub_total = _sub_total;
      }
      virtual const CORBA::ULong sub_total() const {
        return _obv_sub_total;
      }

    protected:
      OBV_SalesReport();
      virtual ~OBV_SalesReport();

      OBV_SalesReport(const CORBA::ULong _sub_total) {
                _obv_sub_total = _sub_total;
  
      }
      CORBA::ULong _obv_sub_total;
  };
  class SalesReport_init : public CORBA::ValueFactoryBase {
    virtual SalesReport_ptr create (CORBA::ULong sub_total) = 0;
  };
  // idl struct: PetStore::ProductIndexInfo
  struct  ProductIndexInfo {

    CORBA::String_var product_id;
    CORBA::String_var product_name;
    CORBA::String_var product_category;
    #if defined(MSVCNEWDLL_BUG)
      void *operator new(size_t ts);
      void *operator new(size_t ts, char*, int) {return operator new(ts);}
      void operator delete(void *p);
    #endif // MSVCNEWDLL_BUG
    friend VISostream& _marshal_in(VISostream&, const ProductIndexInfo&);
    friend VISistream& _marshal_out(VISistream&, ProductIndexInfo&);

    friend VISostream& _marshal_in(VISostream& _strm, const ProductIndexInfo* _obj);
    friend VISistream& _marshal_out(VISistream& _strm, ProductIndexInfo*& _obj);

    friend void _internal_pretty_print(VISostream&, const ProductIndexInfo&);
    friend Ostream& _marshal_in(Ostream&, const ProductIndexInfo&);
    friend Istream& _marshal_out(Istream&, ProductIndexInfo&);
    friend Istream& _marshal_out(Istream&, ProductIndexInfo*&);
  };
  typedef ProductIndexInfo * ProductIndexInfo_ptr;

  class  ProductIndexInfo_out;

  class  ProductIndexInfo_var {
    friend class ProductIndexInfo_out;
  private:
    ProductIndexInfo_ptr _ptr;
  public:
    ProductIndexInfo_var() : _ptr((ProductIndexInfo*)NULL) {}
    ProductIndexInfo_var(ProductIndexInfo *_p) : _ptr(_p) {}

    ProductIndexInfo_var(const ProductIndexInfo_var& _var) {
      _ptr = _var._ptr ? (new ProductIndexInfo(*_var._ptr)) : (ProductIndexInfo*)NULL;
    }

    virtual ~ProductIndexInfo_var() { delete _ptr; }
    ProductIndexInfo_var& operator=(ProductIndexInfo* _p);
    ProductIndexInfo_var& operator=(const ProductIndexInfo_var& _var);
    operator ProductIndexInfo*() const { return _ptr; }

    ProductIndexInfo* operator->() { return _ptr; }
    const ProductIndexInfo* operator->() const { return _ptr; }

    operator ProductIndexInfo&() { return *_ptr; }
    operator const ProductIndexInfo&() const { return *_ptr; }

    const ProductIndexInfo& in() const { return *_ptr; }
    ProductIndexInfo& inout() { return *_ptr; }
    ProductIndexInfo_ptr& out() {
      delete _ptr;
      _ptr = (ProductIndexInfo_ptr) NULL;
      return _ptr;
    }

    ProductIndexInfo_ptr _retn() {
      ProductIndexInfo_ptr temp_ptr;
      temp_ptr = _ptr;
      _ptr = (ProductIndexInfo_ptr) NULL;
      return temp_ptr;
    }
    friend VISostream& _marshal_in(VISostream&, const ProductIndexInfo_var&);
    friend VISistream& _marshal_out(VISistream&, ProductIndexInfo_var&);
  };
  class  ProductIndexInfo_out {
   private:
    ProductIndexInfo_ptr& _ptr;
    static ProductIndexInfo* _nil() { return (ProductIndexInfo*)NULL; }

    void operator=(const ProductIndexInfo_out&);
    void operator=(const ProductIndexInfo_var&);
     public:
    ProductIndexInfo_out(const ProductIndexInfo_out& _o) : _ptr(_o._ptr) {}
    ProductIndexInfo_out(ProductIndexInfo_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    ProductIndexInfo_out(ProductIndexInfo_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    ProductIndexInfo_out& operator=(ProductIndexInfo* _p) {
      _ptr = _p;
      return *this;
    }
    operator ProductIndexInfo_ptr&() { return _ptr; }
    ProductIndexInfo_ptr& ptr() { return _ptr; }
    ProductIndexInfo* operator->() { return _ptr; }
  };
  class  ProductIndexInfoList;
  typedef ProductIndexInfoList* ProductIndexInfoList_ptr;

  class  ProductIndexInfoList : private VISResource {
  private:
    ProductIndexInfo *_contents;
    CORBA::ULong _count;
    CORBA::ULong _num_allocated;
    CORBA::Boolean _release_flag;

  public:
    #if defined(MSVCNEWDLL_BUG)
      void *operator new(size_t ts);
      void *operator new(size_t ts, char*, int) {return operator new(ts);}
      void operator delete(void *p);
    #endif // MSVCNEWDLL_BUG

    static ProductIndexInfo *allocbuf(CORBA::ULong _nelems);
    static void freebuf(ProductIndexInfo* _data);

    ProductIndexInfoList(CORBA::ULong _max = 0);
    ProductIndexInfoList(CORBA::ULong max, CORBA::ULong length,
                         ProductIndexInfo *_data,
                         CORBA::Boolean release = (CORBA::Boolean)0);
    ProductIndexInfoList(const ProductIndexInfoList&);
    virtual ~ProductIndexInfoList();

    ProductIndexInfoList& operator=(const ProductIndexInfoList&);
    ProductIndexInfoList& operator=(const ProductIndexInfoList* _obj) {
      assert(_obj);
      return operator=(*_obj);
    }

    CORBA::ULong maximum() const { return _num_allocated; }

    void length(CORBA::ULong _len);
    CORBA::ULong length() const { return _count; }

    void replace(CORBA::ULong _max, CORBA::ULong _len,
                 ProductIndexInfo *_data,
                 CORBA::Boolean _release = (CORBA::Boolean)0);

    ProductIndexInfo *get_buffer(CORBA::Boolean orphan = (CORBA::Boolean) 0);
    const ProductIndexInfo* get_buffer() const;

    ProductIndexInfo& operator[](CORBA::ULong _index);
    const ProductIndexInfo& operator[](CORBA::ULong _index) const;

    static ProductIndexInfoList *_duplicate(ProductIndexInfoList* _ptr) {
      if (_ptr) _ptr->_ref();
      return _ptr;
    }

    static void _release(ProductIndexInfoList *_ptr) {
      if (_ptr && (_ptr->_deref() == 0L)) 
        delete _ptr;
    }

    friend VISostream& _marshal_in(VISostream&, const ProductIndexInfoList&);
    friend VISistream& _marshal_out(VISistream&, ProductIndexInfoList&);

    friend VISostream& _marshal_in(VISostream& _strm, const ProductIndexInfoList* _obj);
    friend VISistream& _marshal_out(VISistream& _strm, ProductIndexInfoList_ptr& _obj);

    friend void _internal_pretty_print(VISostream&, const ProductIndexInfoList&);

    friend Ostream& _marshal_in(Ostream&, const ProductIndexInfoList&);
    friend Istream& _marshal_out(Istream&, ProductIndexInfoList&);
    friend Istream& _marshal_out(Istream&, ProductIndexInfoList_ptr&);
  };

  class  ProductIndexInfoList_out;

  class  ProductIndexInfoList_var {
    friend class ProductIndexInfoList_out;
     private:
    ProductIndexInfoList *_ptr;
     public:
    ProductIndexInfoList_var() : _ptr((ProductIndexInfoList*)NULL) {}
    ProductIndexInfoList_var(ProductIndexInfoList *_p) : _ptr(_p) {}
    ProductIndexInfoList_var(const ProductIndexInfoList_var& _var) :_ptr(ProductIndexInfoList::_duplicate(_var._ptr)){}
    virtual ~ProductIndexInfoList_var() { ProductIndexInfoList::_release(_ptr); }
    ProductIndexInfoList_var& operator=(ProductIndexInfoList* _p) {
      ProductIndexInfoList::_release(_ptr);
      _ptr = _p;
      return *this;
    }

    ProductIndexInfoList_var& operator=(const ProductIndexInfoList_var& _var) {
      ProductIndexInfoList::_release(_ptr);
      _ptr = ProductIndexInfoList::_duplicate(_var._ptr);
      return *this;
    }
    operator ProductIndexInfoList*() const { return _ptr; }

    ProductIndexInfoList* operator->() { return _ptr; }
    const ProductIndexInfoList* operator->() const { return _ptr; }

    operator ProductIndexInfoList&() { return *_ptr; }
    operator const ProductIndexInfoList&() const { return *_ptr; }

    const ProductIndexInfoList& in() const { return *_ptr; }
    ProductIndexInfoList& inout() { return *_ptr; }
    ProductIndexInfoList_ptr & out() {
      ProductIndexInfoList::_release(_ptr);
      _ptr = (ProductIndexInfoList *)NULL;
      return _ptr;
    }

    ProductIndexInfoList* _retn() {
      ProductIndexInfoList* _tmp_ptr;
      _tmp_ptr = _ptr;
      _ptr = (ProductIndexInfoList *)NULL;
      return _tmp_ptr;
    }
    friend VISostream& _marshal_in(VISostream&, const ProductIndexInfoList_var&);
    friend VISistream& _marshal_out(VISistream&, ProductIndexInfoList_var&);

    ProductIndexInfo& operator[](CORBA::ULong _index);
    const ProductIndexInfo& operator[](CORBA::ULong _index) const;

  };
  class  ProductIndexInfoList_out {
   private:
    ProductIndexInfoList_ptr& _ptr;
    static ProductIndexInfoList* _nil() { return (ProductIndexInfoList*)NULL; }

    void operator=(const ProductIndexInfoList_out&);
    void operator=(const ProductIndexInfoList_var&);
     public:
    ProductIndexInfoList_out(const ProductIndexInfoList_out& _o) : _ptr(_o._ptr) {}
    ProductIndexInfoList_out(ProductIndexInfoList_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    ProductIndexInfoList_out(ProductIndexInfoList_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    ProductIndexInfoList_out& operator=(ProductIndexInfoList* _p) {
      _ptr = _p;
      return *this;
    }
    operator ProductIndexInfoList_ptr&() { return _ptr; }
    ProductIndexInfoList_ptr& ptr() { return _ptr; }
  };
  enum ProductCategory {
     DOG, 
     CAT, 
     FISH, 
     BIRD
  };

  typedef ProductCategory& ProductCategory_out;

   VISostream& _marshal_in(VISostream& _strm, const ProductCategory _enum_type);
   VISistream& _marshal_out(VISistream& _strm, ProductCategory& _enum_type);

   Ostream& _marshal_in(Ostream&, const ProductCategory&);
   Istream& _marshal_out(Istream&, ProductCategory&);

   void _internal_pretty_print(VISostream& _strm, ProductCategory _val);

  // idl struct: PetStore::ProductInfo
  struct  ProductInfo {

    CORBA::String_var product_id;
    CORBA::String_var product_name;
    CORBA::String_var product_picture;
    CORBA::String_var product_category;
    CORBA::ULong product_price;
    CORBA::ULong product_qty;
    CORBA::String_var product_description;
    #if defined(MSVCNEWDLL_BUG)
      void *operator new(size_t ts);
      void *operator new(size_t ts, char*, int) {return operator new(ts);}
      void operator delete(void *p);
    #endif // MSVCNEWDLL_BUG
    friend VISostream& _marshal_in(VISostream&, const ProductInfo&);
    friend VISistream& _marshal_out(VISistream&, ProductInfo&);

    friend VISostream& _marshal_in(VISostream& _strm, const ProductInfo* _obj);
    friend VISistream& _marshal_out(VISistream& _strm, ProductInfo*& _obj);

    friend void _internal_pretty_print(VISostream&, const ProductInfo&);
    friend Ostream& _marshal_in(Ostream&, const ProductInfo&);
    friend Istream& _marshal_out(Istream&, ProductInfo&);
    friend Istream& _marshal_out(Istream&, ProductInfo*&);
  };
  typedef ProductInfo * ProductInfo_ptr;

  class  ProductInfo_out;

  class  ProductInfo_var {
    friend class ProductInfo_out;
  private:
    ProductInfo_ptr _ptr;
  public:
    ProductInfo_var() : _ptr((ProductInfo*)NULL) {}
    ProductInfo_var(ProductInfo *_p) : _ptr(_p) {}

    ProductInfo_var(const ProductInfo_var& _var) {
      _ptr = _var._ptr ? (new ProductInfo(*_var._ptr)) : (ProductInfo*)NULL;
    }

    virtual ~ProductInfo_var() { delete _ptr; }
    ProductInfo_var& operator=(ProductInfo* _p);
    ProductInfo_var& operator=(const ProductInfo_var& _var);
    operator ProductInfo*() const { return _ptr; }

    ProductInfo* operator->() { return _ptr; }
    const ProductInfo* operator->() const { return _ptr; }

    operator ProductInfo&() { return *_ptr; }
    operator const ProductInfo&() const { return *_ptr; }

    const ProductInfo& in() const { return *_ptr; }
    ProductInfo& inout() { return *_ptr; }
    ProductInfo_ptr& out() {
      delete _ptr;
      _ptr = (ProductInfo_ptr) NULL;
      return _ptr;
    }

    ProductInfo_ptr _retn() {
      ProductInfo_ptr temp_ptr;
      temp_ptr = _ptr;
      _ptr = (ProductInfo_ptr) NULL;
      return temp_ptr;
    }
    friend VISostream& _marshal_in(VISostream&, const ProductInfo_var&);
    friend VISistream& _marshal_out(VISistream&, ProductInfo_var&);
  };
  class  ProductInfo_out {
   private:
    ProductInfo_ptr& _ptr;
    static ProductInfo* _nil() { return (ProductInfo*)NULL; }

    void operator=(const ProductInfo_out&);
    void operator=(const ProductInfo_var&);
     public:
    ProductInfo_out(const ProductInfo_out& _o) : _ptr(_o._ptr) {}
    ProductInfo_out(ProductInfo_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    ProductInfo_out(ProductInfo_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    ProductInfo_out& operator=(ProductInfo* _p) {
      _ptr = _p;
      return *this;
    }
    operator ProductInfo_ptr&() { return _ptr; }
    ProductInfo_ptr& ptr() { return _ptr; }
    ProductInfo* operator->() { return _ptr; }
  };
  // idl struct: PetStore::OrderInfo
  struct  OrderInfo {

    CORBA::String_var order_id;
    CORBA::String_var user_name;
    CORBA::String_var user_address;
    CORBA::String_var order_time;
    OrderItemInfoList order_items;
    CORBA::ULong sub_total;
    #if defined(MSVCNEWDLL_BUG)
      void *operator new(size_t ts);
      void *operator new(size_t ts, char*, int) {return operator new(ts);}
      void operator delete(void *p);
    #endif // MSVCNEWDLL_BUG
    friend VISostream& _marshal_in(VISostream&, const OrderInfo&);
    friend VISistream& _marshal_out(VISistream&, OrderInfo&);

    friend VISostream& _marshal_in(VISostream& _strm, const OrderInfo* _obj);
    friend VISistream& _marshal_out(VISistream& _strm, OrderInfo*& _obj);

    friend void _internal_pretty_print(VISostream&, const OrderInfo&);
    friend Ostream& _marshal_in(Ostream&, const OrderInfo&);
    friend Istream& _marshal_out(Istream&, OrderInfo&);
    friend Istream& _marshal_out(Istream&, OrderInfo*&);
  };
  typedef OrderInfo * OrderInfo_ptr;

  class  OrderInfo_out;

  class  OrderInfo_var {
    friend class OrderInfo_out;
  private:
    OrderInfo_ptr _ptr;
  public:
    OrderInfo_var() : _ptr((OrderInfo*)NULL) {}
    OrderInfo_var(OrderInfo *_p) : _ptr(_p) {}

    OrderInfo_var(const OrderInfo_var& _var) {
      _ptr = _var._ptr ? (new OrderInfo(*_var._ptr)) : (OrderInfo*)NULL;
    }

    virtual ~OrderInfo_var() { delete _ptr; }
    OrderInfo_var& operator=(OrderInfo* _p);
    OrderInfo_var& operator=(const OrderInfo_var& _var);
    operator OrderInfo*() const { return _ptr; }

    OrderInfo* operator->() { return _ptr; }
    const OrderInfo* operator->() const { return _ptr; }

    operator OrderInfo&() { return *_ptr; }
    operator const OrderInfo&() const { return *_ptr; }

    const OrderInfo& in() const { return *_ptr; }
    OrderInfo& inout() { return *_ptr; }
    OrderInfo_ptr& out() {
      delete _ptr;
      _ptr = (OrderInfo_ptr) NULL;
      return _ptr;
    }

    OrderInfo_ptr _retn() {
      OrderInfo_ptr temp_ptr;
      temp_ptr = _ptr;
      _ptr = (OrderInfo_ptr) NULL;
      return temp_ptr;
    }
    friend VISostream& _marshal_in(VISostream&, const OrderInfo_var&);
    friend VISistream& _marshal_out(VISistream&, OrderInfo_var&);
  };
  class  OrderInfo_out {
   private:
    OrderInfo_ptr& _ptr;
    static OrderInfo* _nil() { return (OrderInfo*)NULL; }

    void operator=(const OrderInfo_out&);
    void operator=(const OrderInfo_var&);
     public:
    OrderInfo_out(const OrderInfo_out& _o) : _ptr(_o._ptr) {}
    OrderInfo_out(OrderInfo_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    OrderInfo_out(OrderInfo_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    OrderInfo_out& operator=(OrderInfo* _p) {
      _ptr = _p;
      return *this;
    }
    operator OrderInfo_ptr&() { return _ptr; }
    OrderInfo_ptr& ptr() { return _ptr; }
    OrderInfo* operator->() { return _ptr; }
  };
  #ifndef PetStore_BusinessOp_var_
  #define PetStore_BusinessOp_var_

  class  BusinessOp;

  typedef BusinessOp* BusinessOp_ptr;
  typedef BusinessOp_ptr BusinessOpRef;
   VISistream& _marshal_out(VISistream&, BusinessOp_ptr&);
   VISostream& _marshal_in(VISostream&, const BusinessOp_ptr);
  class  BusinessOp_out;

  class  BusinessOp_var : public CORBA::_var {
  friend class BusinessOp_out;
   private:
    BusinessOp_ptr _ptr;

   public:
    BusinessOp_var();
    BusinessOp_var(BusinessOp_ptr);
    BusinessOp_var(const BusinessOp_var &);
    virtual ~BusinessOp_var();

    static BusinessOp_ptr _duplicate(BusinessOp_ptr);
    static void _release(BusinessOp_ptr);

    BusinessOp_var& operator=(const BusinessOp_var& _var);
    BusinessOp_var& operator=(BusinessOp_ptr);

    operator BusinessOp*() const { return _ptr; }
    BusinessOp* operator->() const { return _ptr; }

    BusinessOp_ptr in() const { return _ptr; }
    BusinessOp_ptr& inout() { return _ptr; }
    BusinessOp_ptr& out();
    BusinessOp_ptr _retn();

    friend VISostream& _marshal_in(VISostream&, const BusinessOp_var&);
    friend VISistream& _marshal_out(VISistream&, BusinessOp_var&);

    friend Istream& _marshal_out(Istream&, BusinessOp_var&);
    friend Ostream& _marshal_in(Ostream&, const BusinessOp_var&);
  };
  class  BusinessOp_out {
   private:
    BusinessOp_ptr& _ptr;
    static BusinessOp* _nil() { return (BusinessOp*)NULL; }

    void operator=(const BusinessOp_out&);
    void operator=(const BusinessOp_var&);
     public:
    BusinessOp_out(const BusinessOp_out& _o) : _ptr(_o._ptr) {}
    BusinessOp_out(BusinessOp_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    BusinessOp_out(BusinessOp_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    BusinessOp_out& operator=(BusinessOp* _p) {
      _ptr = _p;
      return *this;
    }
    operator BusinessOp_ptr&() { return _ptr; }
    BusinessOp_ptr& ptr() { return _ptr; }
    BusinessOp* operator->() { return _ptr; }
  };

  #endif // PetStore_BusinessOp_var_

  // idl interface: PetStore::BusinessOp
  class  BusinessOp : public virtual CORBA_Object {
   private:
    static const CORBA::TypeInfo _class_info;
    void operator=(const BusinessOp&) {}

    protected:
    BusinessOp() {}
    BusinessOp(const BusinessOp&) {}

   public:
    virtual ~BusinessOp() {}
    static  const CORBA::TypeInfo *_desc();
    virtual const CORBA::TypeInfo *_type_info() const;
    virtual void *_safe_narrow(const CORBA::TypeInfo& ) const;
    static CORBA::Object*_factory();
    BusinessOp_ptr _this();
    static BusinessOp_ptr _duplicate(BusinessOp_ptr _obj) {
      if (_obj) _obj->_ref();
      return _obj;
    }
    static BusinessOp_ptr _nil() { return (BusinessOp_ptr)NULL; }
    static BusinessOp_ptr _narrow(CORBA::Object* _obj);
    static BusinessOp_ptr _clone(BusinessOp_ptr _obj) {
      CORBA::Object_var _obj_var(CORBA_Object::_clone(_obj));
      #if defined(_HPCC_BUG)
        return _narrow(_obj_var.operator CORBA::Object_ptr());
      #else
        return _narrow(_obj_var);
      #endif  // _HPCC_BUG
    }
    static BusinessOp_ptr _bind(const char *_object_name = (const char*)NULL,
                                const char *_host_name = (const char*)NULL,
                                const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                                CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    static BusinessOp_ptr _bind(const char *_poa_name,
                                const CORBA::OctetSequence& _id,
                                const char *_host_name = (const char*)NULL,
                                const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                                CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    virtual ProductIndexInfoList* listAllProducts();
    virtual ProductIndexInfoList* listProductByCategory(ProductCategory _category);
    virtual ProductInfo* listProduct(const char* _productID);
    virtual char* placeOrder(const char* _userName, const char* _userAddress,
                             Cart_ptr _myCart);
    virtual void cancelOrder(const char* _orderID);
    virtual OrderInfo* listOrder(const char* _orderID);

    friend VISostream& _marshal_in(VISostream& _strm, const BusinessOp_ptr _obj);
    friend VISistream& _marshal_out(VISistream& _strm, BusinessOp_ptr& _obj);
    friend Ostream& _marshal_in(Ostream& _strm, const BusinessOp_ptr _obj);
    friend Istream& _marshal_out(Istream& _strm, BusinessOp_ptr& _obj);
  };

  class BusinessOp_ops;
  typedef BusinessOp_ops* BusinessOp_ops_ptr;

  class  BusinessOp_ops  {
   public:
    BusinessOp_ops() {}
    virtual ~BusinessOp_ops () {}

    virtual ProductIndexInfoList* listAllProducts() = 0;
    virtual ProductIndexInfoList* listProductByCategory(ProductCategory _category) = 0;
    virtual ProductInfo* listProduct(const char* _productID) = 0;
    virtual char* placeOrder(const char* _userName, const char* _userAddress,
                             Cart_ptr _myCart) = 0;
    virtual void cancelOrder(const char* _orderID) = 0;
    virtual OrderInfo* listOrder(const char* _orderID) = 0;

    static const VISOps_Info *_desc();
    static BusinessOp_ops_ptr _nil() { return (BusinessOp_ops_ptr)NULL; }
    static BusinessOp_ops_ptr _downcast(PortableServer::ServantBase* _servant);
  protected:
    static const VISOps_Info _ops_info;
  };

  class  OrderInfoList;
  typedef OrderInfoList* OrderInfoList_ptr;

  class  OrderInfoList : private VISResource {
  private:
    OrderInfo *_contents;
    CORBA::ULong _count;
    CORBA::ULong _num_allocated;
    CORBA::Boolean _release_flag;

  public:
    #if defined(MSVCNEWDLL_BUG)
      void *operator new(size_t ts);
      void *operator new(size_t ts, char*, int) {return operator new(ts);}
      void operator delete(void *p);
    #endif // MSVCNEWDLL_BUG

    static OrderInfo *allocbuf(CORBA::ULong _nelems);
    static void freebuf(OrderInfo* _data);

    OrderInfoList(CORBA::ULong _max = 0);
    OrderInfoList(CORBA::ULong max, CORBA::ULong length, OrderInfo *_data,
                  CORBA::Boolean release = (CORBA::Boolean)0);
    OrderInfoList(const OrderInfoList&);
    virtual ~OrderInfoList();

    OrderInfoList& operator=(const OrderInfoList&);
    OrderInfoList& operator=(const OrderInfoList* _obj) {
      assert(_obj);
      return operator=(*_obj);
    }

    CORBA::ULong maximum() const { return _num_allocated; }

    void length(CORBA::ULong _len);
    CORBA::ULong length() const { return _count; }

    void replace(CORBA::ULong _max, CORBA::ULong _len, OrderInfo *_data,
                 CORBA::Boolean _release = (CORBA::Boolean)0);

    OrderInfo *get_buffer(CORBA::Boolean orphan = (CORBA::Boolean) 0);
    const OrderInfo* get_buffer() const;

    OrderInfo& operator[](CORBA::ULong _index);
    const OrderInfo& operator[](CORBA::ULong _index) const;

    static OrderInfoList *_duplicate(OrderInfoList* _ptr) {
      if (_ptr) _ptr->_ref();
      return _ptr;
    }

    static void _release(OrderInfoList *_ptr) {
      if (_ptr && (_ptr->_deref() == 0L)) 
        delete _ptr;
    }

    friend VISostream& _marshal_in(VISostream&, const OrderInfoList&);
    friend VISistream& _marshal_out(VISistream&, OrderInfoList&);

    friend VISostream& _marshal_in(VISostream& _strm, const OrderInfoList* _obj);
    friend VISistream& _marshal_out(VISistream& _strm, OrderInfoList_ptr& _obj);

    friend void _internal_pretty_print(VISostream&, const OrderInfoList&);

    friend Ostream& _marshal_in(Ostream&, const OrderInfoList&);
    friend Istream& _marshal_out(Istream&, OrderInfoList&);
    friend Istream& _marshal_out(Istream&, OrderInfoList_ptr&);
  };

  class  OrderInfoList_out;

  class  OrderInfoList_var {
    friend class OrderInfoList_out;
     private:
    OrderInfoList *_ptr;
     public:
    OrderInfoList_var() : _ptr((OrderInfoList*)NULL) {}
    OrderInfoList_var(OrderInfoList *_p) : _ptr(_p) {}
    OrderInfoList_var(const OrderInfoList_var& _var) :_ptr(OrderInfoList::_duplicate(_var._ptr)){}
    virtual ~OrderInfoList_var() { OrderInfoList::_release(_ptr); }
    OrderInfoList_var& operator=(OrderInfoList* _p) {
      OrderInfoList::_release(_ptr);
      _ptr = _p;
      return *this;
    }

    OrderInfoList_var& operator=(const OrderInfoList_var& _var) {
      OrderInfoList::_release(_ptr);
      _ptr = OrderInfoList::_duplicate(_var._ptr);
      return *this;
    }
    operator OrderInfoList*() const { return _ptr; }

    OrderInfoList* operator->() { return _ptr; }
    const OrderInfoList* operator->() const { return _ptr; }

    operator OrderInfoList&() { return *_ptr; }
    operator const OrderInfoList&() const { return *_ptr; }

    const OrderInfoList& in() const { return *_ptr; }
    OrderInfoList& inout() { return *_ptr; }
    OrderInfoList_ptr & out() {
      OrderInfoList::_release(_ptr);
      _ptr = (OrderInfoList *)NULL;
      return _ptr;
    }

    OrderInfoList* _retn() {
      OrderInfoList* _tmp_ptr;
      _tmp_ptr = _ptr;
      _ptr = (OrderInfoList *)NULL;
      return _tmp_ptr;
    }
    friend VISostream& _marshal_in(VISostream&, const OrderInfoList_var&);
    friend VISistream& _marshal_out(VISistream&, OrderInfoList_var&);

    OrderInfo& operator[](CORBA::ULong _index);
    const OrderInfo& operator[](CORBA::ULong _index) const;

  };
  class  OrderInfoList_out {
   private:
    OrderInfoList_ptr& _ptr;
    static OrderInfoList* _nil() { return (OrderInfoList*)NULL; }

    void operator=(const OrderInfoList_out&);
    void operator=(const OrderInfoList_var&);
     public:
    OrderInfoList_out(const OrderInfoList_out& _o) : _ptr(_o._ptr) {}
    OrderInfoList_out(OrderInfoList_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    OrderInfoList_out(OrderInfoList_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    OrderInfoList_out& operator=(OrderInfoList* _p) {
      _ptr = _p;
      return *this;
    }
    operator OrderInfoList_ptr&() { return _ptr; }
    OrderInfoList_ptr& ptr() { return _ptr; }
  };
  #ifndef PetStore_AdvancedOp_var_
  #define PetStore_AdvancedOp_var_

  class  AdvancedOp;

  typedef AdvancedOp* AdvancedOp_ptr;
  typedef AdvancedOp_ptr AdvancedOpRef;
   VISistream& _marshal_out(VISistream&, AdvancedOp_ptr&);
   VISostream& _marshal_in(VISostream&, const AdvancedOp_ptr);
  class  AdvancedOp_out;

  class  AdvancedOp_var : public CORBA::_var {
  friend class AdvancedOp_out;
   private:
    AdvancedOp_ptr _ptr;

   public:
    AdvancedOp_var();
    AdvancedOp_var(AdvancedOp_ptr);
    AdvancedOp_var(const AdvancedOp_var &);
    virtual ~AdvancedOp_var();

    static AdvancedOp_ptr _duplicate(AdvancedOp_ptr);
    static void _release(AdvancedOp_ptr);

    AdvancedOp_var& operator=(const AdvancedOp_var& _var);
    AdvancedOp_var& operator=(AdvancedOp_ptr);

    operator AdvancedOp*() const { return _ptr; }
    AdvancedOp* operator->() const { return _ptr; }

    AdvancedOp_ptr in() const { return _ptr; }
    AdvancedOp_ptr& inout() { return _ptr; }
    AdvancedOp_ptr& out();
    AdvancedOp_ptr _retn();

    friend VISostream& _marshal_in(VISostream&, const AdvancedOp_var&);
    friend VISistream& _marshal_out(VISistream&, AdvancedOp_var&);

    friend Istream& _marshal_out(Istream&, AdvancedOp_var&);
    friend Ostream& _marshal_in(Ostream&, const AdvancedOp_var&);
  };
  class  AdvancedOp_out {
   private:
    AdvancedOp_ptr& _ptr;
    static AdvancedOp* _nil() { return (AdvancedOp*)NULL; }

    void operator=(const AdvancedOp_out&);
    void operator=(const AdvancedOp_var&);
     public:
    AdvancedOp_out(const AdvancedOp_out& _o) : _ptr(_o._ptr) {}
    AdvancedOp_out(AdvancedOp_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    AdvancedOp_out(AdvancedOp_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    AdvancedOp_out& operator=(AdvancedOp* _p) {
      _ptr = _p;
      return *this;
    }
    operator AdvancedOp_ptr&() { return _ptr; }
    AdvancedOp_ptr& ptr() { return _ptr; }
    AdvancedOp* operator->() { return _ptr; }
  };

  #endif // PetStore_AdvancedOp_var_

  // idl interface: PetStore::AdvancedOp
  class  AdvancedOp : public virtual CORBA_Object {
   private:
    static const CORBA::TypeInfo _class_info;
    void operator=(const AdvancedOp&) {}

    protected:
    AdvancedOp() {}
    AdvancedOp(const AdvancedOp&) {}

   public:
    virtual ~AdvancedOp() {}
    static  const CORBA::TypeInfo *_desc();
    virtual const CORBA::TypeInfo *_type_info() const;
    virtual void *_safe_narrow(const CORBA::TypeInfo& ) const;
    static CORBA::Object*_factory();
    AdvancedOp_ptr _this();
    static AdvancedOp_ptr _duplicate(AdvancedOp_ptr _obj) {
      if (_obj) _obj->_ref();
      return _obj;
    }
    static AdvancedOp_ptr _nil() { return (AdvancedOp_ptr)NULL; }
    static AdvancedOp_ptr _narrow(CORBA::Object* _obj);
    static AdvancedOp_ptr _clone(AdvancedOp_ptr _obj) {
      CORBA::Object_var _obj_var(CORBA_Object::_clone(_obj));
      #if defined(_HPCC_BUG)
        return _narrow(_obj_var.operator CORBA::Object_ptr());
      #else
        return _narrow(_obj_var);
      #endif  // _HPCC_BUG
    }
    static AdvancedOp_ptr _bind(const char *_object_name = (const char*)NULL,
                                const char *_host_name = (const char*)NULL,
                                const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                                CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    static AdvancedOp_ptr _bind(const char *_poa_name,
                                const CORBA::OctetSequence& _id,
                                const char *_host_name = (const char*)NULL,
                                const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                                CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    virtual OrderInfoList* listAllOrders();
    virtual void addProduct(const ProductInfo& _newProduct);

    friend VISostream& _marshal_in(VISostream& _strm, const AdvancedOp_ptr _obj);
    friend VISistream& _marshal_out(VISistream& _strm, AdvancedOp_ptr& _obj);
    friend Ostream& _marshal_in(Ostream& _strm, const AdvancedOp_ptr _obj);
    friend Istream& _marshal_out(Istream& _strm, AdvancedOp_ptr& _obj);
  };

  class AdvancedOp_ops;
  typedef AdvancedOp_ops* AdvancedOp_ops_ptr;

  class  AdvancedOp_ops  {
   public:
    AdvancedOp_ops() {}
    virtual ~AdvancedOp_ops () {}

    virtual OrderInfoList* listAllOrders() = 0;
    virtual void addProduct(const ProductInfo& _newProduct) = 0;

    static const VISOps_Info *_desc();
    static AdvancedOp_ops_ptr _nil() { return (AdvancedOp_ops_ptr)NULL; }
    static AdvancedOp_ops_ptr _downcast(PortableServer::ServantBase* _servant);
  protected:
    static const VISOps_Info _ops_info;
  };

  #ifndef PetStore_ProductHomeExplicit_var_
  #define PetStore_ProductHomeExplicit_var_

  class  ProductHomeExplicit;

  typedef ProductHomeExplicit* ProductHomeExplicit_ptr;
  typedef ProductHomeExplicit_ptr ProductHomeExplicitRef;
   VISistream& _marshal_out(VISistream&, ProductHomeExplicit_ptr&);
   VISostream& _marshal_in(VISostream&, const ProductHomeExplicit_ptr);
  class  ProductHomeExplicit_out;

  class  ProductHomeExplicit_var : public CORBA::_var {
  friend class ProductHomeExplicit_out;
   private:
    ProductHomeExplicit_ptr _ptr;

   public:
    ProductHomeExplicit_var();
    ProductHomeExplicit_var(ProductHomeExplicit_ptr);
    ProductHomeExplicit_var(const ProductHomeExplicit_var &);
    virtual ~ProductHomeExplicit_var();

    static ProductHomeExplicit_ptr _duplicate(ProductHomeExplicit_ptr);
    static void _release(ProductHomeExplicit_ptr);

    ProductHomeExplicit_var& operator=(const ProductHomeExplicit_var& _var);
    ProductHomeExplicit_var& operator=(ProductHomeExplicit_ptr);

    operator ProductHomeExplicit*() const { return _ptr; }
    ProductHomeExplicit* operator->() const { return _ptr; }

    ProductHomeExplicit_ptr in() const { return _ptr; }
    ProductHomeExplicit_ptr& inout() { return _ptr; }
    ProductHomeExplicit_ptr& out();
    ProductHomeExplicit_ptr _retn();

    friend VISostream& _marshal_in(VISostream&, const ProductHomeExplicit_var&);
    friend VISistream& _marshal_out(VISistream&, ProductHomeExplicit_var&);

    friend Istream& _marshal_out(Istream&, ProductHomeExplicit_var&);
    friend Ostream& _marshal_in(Ostream&, const ProductHomeExplicit_var&);
  };
  class  ProductHomeExplicit_out {
   private:
    ProductHomeExplicit_ptr& _ptr;
    static ProductHomeExplicit* _nil() { return (ProductHomeExplicit*)NULL; }

    void operator=(const ProductHomeExplicit_out&);
    void operator=(const ProductHomeExplicit_var&);
     public:
    ProductHomeExplicit_out(const ProductHomeExplicit_out& _o) : _ptr(_o._ptr) {}
    ProductHomeExplicit_out(ProductHomeExplicit_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    ProductHomeExplicit_out(ProductHomeExplicit_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    ProductHomeExplicit_out& operator=(ProductHomeExplicit* _p) {
      _ptr = _p;
      return *this;
    }
    operator ProductHomeExplicit_ptr&() { return _ptr; }
    ProductHomeExplicit_ptr& ptr() { return _ptr; }
    ProductHomeExplicit* operator->() { return _ptr; }
  };

  #endif // PetStore_ProductHomeExplicit_var_

  // idl interface: PetStore::ProductHomeExplicit
  class  ProductHomeExplicit : public virtual ::Components::CCMHome, public virtual CORBA_Object {
   private:
    static const CORBA::TypeInfo _class_info;
    void operator=(const ProductHomeExplicit&) {}

    protected:
    ProductHomeExplicit() {}
    ProductHomeExplicit(const ProductHomeExplicit&) {}

   public:
    virtual ~ProductHomeExplicit() {}
    static  const CORBA::TypeInfo *_desc();
    virtual const CORBA::TypeInfo *_type_info() const;
    virtual void *_safe_narrow(const CORBA::TypeInfo& ) const;
    static CORBA::Object*_factory();
    ProductHomeExplicit_ptr _this();
    static ProductHomeExplicit_ptr _duplicate(ProductHomeExplicit_ptr _obj) {
      if (_obj) _obj->_ref();
      return _obj;
    }
    static ProductHomeExplicit_ptr _nil() { return (ProductHomeExplicit_ptr)NULL; }
    static ProductHomeExplicit_ptr _narrow(CORBA::Object* _obj);
    static ProductHomeExplicit_ptr _clone(ProductHomeExplicit_ptr _obj) {
      CORBA::Object_var _obj_var(CORBA_Object::_clone(_obj));
      #if defined(_HPCC_BUG)
        return _narrow(_obj_var.operator CORBA::Object_ptr());
      #else
        return _narrow(_obj_var);
      #endif  // _HPCC_BUG
    }
    static ProductHomeExplicit_ptr _bind(const char *_object_name = (const char*)NULL,
                                         const char *_host_name = (const char*)NULL,
                                         const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                                         CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    static ProductHomeExplicit_ptr _bind(const char *_poa_name,
                                         const CORBA::OctetSequence& _id,
                                         const char *_host_name = (const char*)NULL,
                                         const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                                         CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    virtual ProductIndexInfoList* findByProductCategory(ProductCategory _category);
    virtual ProductIndexInfoList* findAllProducts();

    friend VISostream& _marshal_in(VISostream& _strm, const ProductHomeExplicit_ptr _obj);
    friend VISistream& _marshal_out(VISistream& _strm, ProductHomeExplicit_ptr& _obj);
    friend Ostream& _marshal_in(Ostream& _strm, const ProductHomeExplicit_ptr _obj);
    friend Istream& _marshal_out(Istream& _strm, ProductHomeExplicit_ptr& _obj);
  };

  class ProductHomeExplicit_ops;
  typedef ProductHomeExplicit_ops* ProductHomeExplicit_ops_ptr;

  class  ProductHomeExplicit_ops  {
   public:
    ProductHomeExplicit_ops() {}
    virtual ~ProductHomeExplicit_ops () {}

    virtual ProductIndexInfoList* findByProductCategory(ProductCategory _category) = 0;
    virtual ProductIndexInfoList* findAllProducts() = 0;

    static const VISOps_Info *_desc();
    static ProductHomeExplicit_ops_ptr _nil() { return (ProductHomeExplicit_ops_ptr)NULL; }
    static ProductHomeExplicit_ops_ptr _downcast(PortableServer::ServantBase* _servant);
  protected:
    static const VISOps_Info _ops_info;
  };

  #ifndef PetStore_ProductHomeImplicit_var_
  #define PetStore_ProductHomeImplicit_var_

  class  ProductHomeImplicit;

  typedef ProductHomeImplicit* ProductHomeImplicit_ptr;
  typedef ProductHomeImplicit_ptr ProductHomeImplicitRef;
   VISistream& _marshal_out(VISistream&, ProductHomeImplicit_ptr&);
   VISostream& _marshal_in(VISostream&, const ProductHomeImplicit_ptr);
  class  ProductHomeImplicit_out;

  class  ProductHomeImplicit_var : public CORBA::_var {
  friend class ProductHomeImplicit_out;
   private:
    ProductHomeImplicit_ptr _ptr;

   public:
    ProductHomeImplicit_var();
    ProductHomeImplicit_var(ProductHomeImplicit_ptr);
    ProductHomeImplicit_var(const ProductHomeImplicit_var &);
    virtual ~ProductHomeImplicit_var();

    static ProductHomeImplicit_ptr _duplicate(ProductHomeImplicit_ptr);
    static void _release(ProductHomeImplicit_ptr);

    ProductHomeImplicit_var& operator=(const ProductHomeImplicit_var& _var);
    ProductHomeImplicit_var& operator=(ProductHomeImplicit_ptr);

    operator ProductHomeImplicit*() const { return _ptr; }
    ProductHomeImplicit* operator->() const { return _ptr; }

    ProductHomeImplicit_ptr in() const { return _ptr; }
    ProductHomeImplicit_ptr& inout() { return _ptr; }
    ProductHomeImplicit_ptr& out();
    ProductHomeImplicit_ptr _retn();

    friend VISostream& _marshal_in(VISostream&, const ProductHomeImplicit_var&);
    friend VISistream& _marshal_out(VISistream&, ProductHomeImplicit_var&);

    friend Istream& _marshal_out(Istream&, ProductHomeImplicit_var&);
    friend Ostream& _marshal_in(Ostream&, const ProductHomeImplicit_var&);
  };
  class  ProductHomeImplicit_out {
   private:
    ProductHomeImplicit_ptr& _ptr;
    static ProductHomeImplicit* _nil() { return (ProductHomeImplicit*)NULL; }

    void operator=(const ProductHomeImplicit_out&);
    void operator=(const ProductHomeImplicit_var&);
     public:
    ProductHomeImplicit_out(const ProductHomeImplicit_out& _o) : _ptr(_o._ptr) {}
    ProductHomeImplicit_out(ProductHomeImplicit_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    ProductHomeImplicit_out(ProductHomeImplicit_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    ProductHomeImplicit_out& operator=(ProductHomeImplicit* _p) {
      _ptr = _p;
      return *this;
    }
    operator ProductHomeImplicit_ptr&() { return _ptr; }
    ProductHomeImplicit_ptr& ptr() { return _ptr; }
    ProductHomeImplicit* operator->() { return _ptr; }
  };

  #endif // PetStore_ProductHomeImplicit_var_

  // idl interface: PetStore::ProductHomeImplicit
  class  ProductHomeImplicit : public virtual CORBA_Object {
   private:
    static const CORBA::TypeInfo _class_info;
    void operator=(const ProductHomeImplicit&) {}

    protected:
    ProductHomeImplicit() {}
    ProductHomeImplicit(const ProductHomeImplicit&) {}

   public:
    virtual ~ProductHomeImplicit() {}
    static  const CORBA::TypeInfo *_desc();
    virtual const CORBA::TypeInfo *_type_info() const;
    virtual void *_safe_narrow(const CORBA::TypeInfo& ) const;
    static CORBA::Object*_factory();
    ProductHomeImplicit_ptr _this();
    static ProductHomeImplicit_ptr _duplicate(ProductHomeImplicit_ptr _obj) {
      if (_obj) _obj->_ref();
      return _obj;
    }
    static ProductHomeImplicit_ptr _nil() { return (ProductHomeImplicit_ptr)NULL; }
    static ProductHomeImplicit_ptr _narrow(CORBA::Object* _obj);
    static ProductHomeImplicit_ptr _clone(ProductHomeImplicit_ptr _obj) {
      CORBA::Object_var _obj_var(CORBA_Object::_clone(_obj));
      #if defined(_HPCC_BUG)
        return _narrow(_obj_var.operator CORBA::Object_ptr());
      #else
        return _narrow(_obj_var);
      #endif  // _HPCC_BUG
    }
    static ProductHomeImplicit_ptr _bind(const char *_object_name = (const char*)NULL,
                                         const char *_host_name = (const char*)NULL,
                                         const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                                         CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    static ProductHomeImplicit_ptr _bind(const char *_poa_name,
                                         const CORBA::OctetSequence& _id,
                                         const char *_host_name = (const char*)NULL,
                                         const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                                         CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    virtual Product_ptr create(KeyID_ptr _pkey);
    virtual Product_ptr find_by_primary_key(KeyID_ptr _pkey);
    virtual void remove(KeyID_ptr _pkey);
    virtual KeyID_ptr get_primary_key(Product_ptr _comp);

    friend VISostream& _marshal_in(VISostream& _strm, const ProductHomeImplicit_ptr _obj);
    friend VISistream& _marshal_out(VISistream& _strm, ProductHomeImplicit_ptr& _obj);
    friend Ostream& _marshal_in(Ostream& _strm, const ProductHomeImplicit_ptr _obj);
    friend Istream& _marshal_out(Istream& _strm, ProductHomeImplicit_ptr& _obj);
  };

  class ProductHomeImplicit_ops;
  typedef ProductHomeImplicit_ops* ProductHomeImplicit_ops_ptr;

  class  ProductHomeImplicit_ops  {
   public:
    ProductHomeImplicit_ops() {}
    virtual ~ProductHomeImplicit_ops () {}

    virtual Product_ptr create(KeyID_ptr _pkey) = 0;
    virtual Product_ptr find_by_primary_key(KeyID_ptr _pkey) = 0;
    virtual void remove(KeyID_ptr _pkey) = 0;
    virtual KeyID_ptr get_primary_key(Product_ptr _comp) = 0;

    static const VISOps_Info *_desc();
    static ProductHomeImplicit_ops_ptr _nil() { return (ProductHomeImplicit_ops_ptr)NULL; }
    static ProductHomeImplicit_ops_ptr _downcast(PortableServer::ServantBase* _servant);
  protected:
    static const VISOps_Info _ops_info;
  };

  #ifndef PetStore_ProductHome_var_
  #define PetStore_ProductHome_var_

  class  ProductHome;

  typedef ProductHome* ProductHome_ptr;
  typedef ProductHome_ptr ProductHomeRef;
   VISistream& _marshal_out(VISistream&, ProductHome_ptr&);
   VISostream& _marshal_in(VISostream&, const ProductHome_ptr);
  class  ProductHome_out;

  class  ProductHome_var : public CORBA::_var {
  friend class ProductHome_out;
   private:
    ProductHome_ptr _ptr;

   public:
    ProductHome_var();
    ProductHome_var(ProductHome_ptr);
    ProductHome_var(const ProductHome_var &);
    virtual ~ProductHome_var();

    static ProductHome_ptr _duplicate(ProductHome_ptr);
    static void _release(ProductHome_ptr);

    ProductHome_var& operator=(const ProductHome_var& _var);
    ProductHome_var& operator=(ProductHome_ptr);

    operator ProductHome*() const { return _ptr; }
    ProductHome* operator->() const { return _ptr; }

    ProductHome_ptr in() const { return _ptr; }
    ProductHome_ptr& inout() { return _ptr; }
    ProductHome_ptr& out();
    ProductHome_ptr _retn();

    friend VISostream& _marshal_in(VISostream&, const ProductHome_var&);
    friend VISistream& _marshal_out(VISistream&, ProductHome_var&);

    friend Istream& _marshal_out(Istream&, ProductHome_var&);
    friend Ostream& _marshal_in(Ostream&, const ProductHome_var&);
  };
  class  ProductHome_out {
   private:
    ProductHome_ptr& _ptr;
    static ProductHome* _nil() { return (ProductHome*)NULL; }

    void operator=(const ProductHome_out&);
    void operator=(const ProductHome_var&);
     public:
    ProductHome_out(const ProductHome_out& _o) : _ptr(_o._ptr) {}
    ProductHome_out(ProductHome_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    ProductHome_out(ProductHome_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    ProductHome_out& operator=(ProductHome* _p) {
      _ptr = _p;
      return *this;
    }
    operator ProductHome_ptr&() { return _ptr; }
    ProductHome_ptr& ptr() { return _ptr; }
    ProductHome* operator->() { return _ptr; }
  };

  #endif // PetStore_ProductHome_var_

  // idl interface: PetStore::ProductHome
  class  ProductHome : public virtual ProductHomeExplicit, public virtual ProductHomeImplicit, public virtual CORBA_Object {
   private:
    static const CORBA::TypeInfo _class_info;
    void operator=(const ProductHome&) {}

    protected:
    ProductHome() {}
    ProductHome(const ProductHome&) {}

   public:
    virtual ~ProductHome() {}
    static  const CORBA::TypeInfo *_desc();
    virtual const CORBA::TypeInfo *_type_info() const;
    virtual void *_safe_narrow(const CORBA::TypeInfo& ) const;
    static CORBA::Object*_factory();
    ProductHome_ptr _this();
    static ProductHome_ptr _duplicate(ProductHome_ptr _obj) {
      if (_obj) _obj->_ref();
      return _obj;
    }
    static ProductHome_ptr _nil() { return (ProductHome_ptr)NULL; }
    static ProductHome_ptr _narrow(CORBA::Object* _obj);
    static ProductHome_ptr _clone(ProductHome_ptr _obj) {
      CORBA::Object_var _obj_var(CORBA_Object::_clone(_obj));
      #if defined(_HPCC_BUG)
        return _narrow(_obj_var.operator CORBA::Object_ptr());
      #else
        return _narrow(_obj_var);
      #endif  // _HPCC_BUG
    }
    static ProductHome_ptr _bind(const char *_object_name = (const char*)NULL,
                                 const char *_host_name = (const char*)NULL,
                                 const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                                 CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    static ProductHome_ptr _bind(const char *_poa_name,
                                 const CORBA::OctetSequence& _id,
                                 const char *_host_name = (const char*)NULL,
                                 const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                                 CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    friend VISostream& _marshal_in(VISostream& _strm, const ProductHome_ptr _obj);
    friend VISistream& _marshal_out(VISistream& _strm, ProductHome_ptr& _obj);
    friend Ostream& _marshal_in(Ostream& _strm, const ProductHome_ptr _obj);
    friend Istream& _marshal_out(Istream& _strm, ProductHome_ptr& _obj);
  };

  class ProductHome_ops;
  typedef ProductHome_ops* ProductHome_ops_ptr;

  class  ProductHome_ops  {
   public:
    ProductHome_ops() {}
    virtual ~ProductHome_ops () {}

    static const VISOps_Info *_desc();
    static ProductHome_ops_ptr _nil() { return (ProductHome_ops_ptr)NULL; }
    static ProductHome_ops_ptr _downcast(PortableServer::ServantBase* _servant);
  protected:
    static const VISOps_Info _ops_info;
  };

  // idl struct: PetStore::OrderIndexInfo
  struct  OrderIndexInfo {

    CORBA::String_var order_id;
    CORBA::ULong sub_total;
    #if defined(MSVCNEWDLL_BUG)
      void *operator new(size_t ts);
      void *operator new(size_t ts, char*, int) {return operator new(ts);}
      void operator delete(void *p);
    #endif // MSVCNEWDLL_BUG
    friend VISostream& _marshal_in(VISostream&, const OrderIndexInfo&);
    friend VISistream& _marshal_out(VISistream&, OrderIndexInfo&);

    friend VISostream& _marshal_in(VISostream& _strm, const OrderIndexInfo* _obj);
    friend VISistream& _marshal_out(VISistream& _strm, OrderIndexInfo*& _obj);

    friend void _internal_pretty_print(VISostream&, const OrderIndexInfo&);
    friend Ostream& _marshal_in(Ostream&, const OrderIndexInfo&);
    friend Istream& _marshal_out(Istream&, OrderIndexInfo&);
    friend Istream& _marshal_out(Istream&, OrderIndexInfo*&);
  };
  typedef OrderIndexInfo * OrderIndexInfo_ptr;

  class  OrderIndexInfo_out;

  class  OrderIndexInfo_var {
    friend class OrderIndexInfo_out;
  private:
    OrderIndexInfo_ptr _ptr;
  public:
    OrderIndexInfo_var() : _ptr((OrderIndexInfo*)NULL) {}
    OrderIndexInfo_var(OrderIndexInfo *_p) : _ptr(_p) {}

    OrderIndexInfo_var(const OrderIndexInfo_var& _var) {
      _ptr = _var._ptr ? (new OrderIndexInfo(*_var._ptr)) : (OrderIndexInfo*)NULL;
    }

    virtual ~OrderIndexInfo_var() { delete _ptr; }
    OrderIndexInfo_var& operator=(OrderIndexInfo* _p);
    OrderIndexInfo_var& operator=(const OrderIndexInfo_var& _var);
    operator OrderIndexInfo*() const { return _ptr; }

    OrderIndexInfo* operator->() { return _ptr; }
    const OrderIndexInfo* operator->() const { return _ptr; }

    operator OrderIndexInfo&() { return *_ptr; }
    operator const OrderIndexInfo&() const { return *_ptr; }

    const OrderIndexInfo& in() const { return *_ptr; }
    OrderIndexInfo& inout() { return *_ptr; }
    OrderIndexInfo_ptr& out() {
      delete _ptr;
      _ptr = (OrderIndexInfo_ptr) NULL;
      return _ptr;
    }

    OrderIndexInfo_ptr _retn() {
      OrderIndexInfo_ptr temp_ptr;
      temp_ptr = _ptr;
      _ptr = (OrderIndexInfo_ptr) NULL;
      return temp_ptr;
    }
    friend VISostream& _marshal_in(VISostream&, const OrderIndexInfo_var&);
    friend VISistream& _marshal_out(VISistream&, OrderIndexInfo_var&);
  };
  class  OrderIndexInfo_out {
   private:
    OrderIndexInfo_ptr& _ptr;
    static OrderIndexInfo* _nil() { return (OrderIndexInfo*)NULL; }

    void operator=(const OrderIndexInfo_out&);
    void operator=(const OrderIndexInfo_var&);
     public:
    OrderIndexInfo_out(const OrderIndexInfo_out& _o) : _ptr(_o._ptr) {}
    OrderIndexInfo_out(OrderIndexInfo_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    OrderIndexInfo_out(OrderIndexInfo_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    OrderIndexInfo_out& operator=(OrderIndexInfo* _p) {
      _ptr = _p;
      return *this;
    }
    operator OrderIndexInfo_ptr&() { return _ptr; }
    OrderIndexInfo_ptr& ptr() { return _ptr; }
    OrderIndexInfo* operator->() { return _ptr; }
  };
  class  OrderIndexInfoList;
  typedef OrderIndexInfoList* OrderIndexInfoList_ptr;

  class  OrderIndexInfoList : private VISResource {
  private:
    OrderIndexInfo *_contents;
    CORBA::ULong _count;
    CORBA::ULong _num_allocated;
    CORBA::Boolean _release_flag;

  public:
    #if defined(MSVCNEWDLL_BUG)
      void *operator new(size_t ts);
      void *operator new(size_t ts, char*, int) {return operator new(ts);}
      void operator delete(void *p);
    #endif // MSVCNEWDLL_BUG

    static OrderIndexInfo *allocbuf(CORBA::ULong _nelems);
    static void freebuf(OrderIndexInfo* _data);

    OrderIndexInfoList(CORBA::ULong _max = 0);
    OrderIndexInfoList(CORBA::ULong max, CORBA::ULong length,
                       OrderIndexInfo *_data,
                       CORBA::Boolean release = (CORBA::Boolean)0);
    OrderIndexInfoList(const OrderIndexInfoList&);
    virtual ~OrderIndexInfoList();

    OrderIndexInfoList& operator=(const OrderIndexInfoList&);
    OrderIndexInfoList& operator=(const OrderIndexInfoList* _obj) {
      assert(_obj);
      return operator=(*_obj);
    }

    CORBA::ULong maximum() const { return _num_allocated; }

    void length(CORBA::ULong _len);
    CORBA::ULong length() const { return _count; }

    void replace(CORBA::ULong _max, CORBA::ULong _len, OrderIndexInfo *_data,
                 CORBA::Boolean _release = (CORBA::Boolean)0);

    OrderIndexInfo *get_buffer(CORBA::Boolean orphan = (CORBA::Boolean) 0);
    const OrderIndexInfo* get_buffer() const;

    OrderIndexInfo& operator[](CORBA::ULong _index);
    const OrderIndexInfo& operator[](CORBA::ULong _index) const;

    static OrderIndexInfoList *_duplicate(OrderIndexInfoList* _ptr) {
      if (_ptr) _ptr->_ref();
      return _ptr;
    }

    static void _release(OrderIndexInfoList *_ptr) {
      if (_ptr && (_ptr->_deref() == 0L)) 
        delete _ptr;
    }

    friend VISostream& _marshal_in(VISostream&, const OrderIndexInfoList&);
    friend VISistream& _marshal_out(VISistream&, OrderIndexInfoList&);

    friend VISostream& _marshal_in(VISostream& _strm, const OrderIndexInfoList* _obj);
    friend VISistream& _marshal_out(VISistream& _strm, OrderIndexInfoList_ptr& _obj);

    friend void _internal_pretty_print(VISostream&, const OrderIndexInfoList&);

    friend Ostream& _marshal_in(Ostream&, const OrderIndexInfoList&);
    friend Istream& _marshal_out(Istream&, OrderIndexInfoList&);
    friend Istream& _marshal_out(Istream&, OrderIndexInfoList_ptr&);
  };

  class  OrderIndexInfoList_out;

  class  OrderIndexInfoList_var {
    friend class OrderIndexInfoList_out;
     private:
    OrderIndexInfoList *_ptr;
     public:
    OrderIndexInfoList_var() : _ptr((OrderIndexInfoList*)NULL) {}
    OrderIndexInfoList_var(OrderIndexInfoList *_p) : _ptr(_p) {}
    OrderIndexInfoList_var(const OrderIndexInfoList_var& _var) :_ptr(OrderIndexInfoList::_duplicate(_var._ptr)){}
    virtual ~OrderIndexInfoList_var() { OrderIndexInfoList::_release(_ptr); }
    OrderIndexInfoList_var& operator=(OrderIndexInfoList* _p) {
      OrderIndexInfoList::_release(_ptr);
      _ptr = _p;
      return *this;
    }

    OrderIndexInfoList_var& operator=(const OrderIndexInfoList_var& _var) {
      OrderIndexInfoList::_release(_ptr);
      _ptr = OrderIndexInfoList::_duplicate(_var._ptr);
      return *this;
    }
    operator OrderIndexInfoList*() const { return _ptr; }

    OrderIndexInfoList* operator->() { return _ptr; }
    const OrderIndexInfoList* operator->() const { return _ptr; }

    operator OrderIndexInfoList&() { return *_ptr; }
    operator const OrderIndexInfoList&() const { return *_ptr; }

    const OrderIndexInfoList& in() const { return *_ptr; }
    OrderIndexInfoList& inout() { return *_ptr; }
    OrderIndexInfoList_ptr & out() {
      OrderIndexInfoList::_release(_ptr);
      _ptr = (OrderIndexInfoList *)NULL;
      return _ptr;
    }

    OrderIndexInfoList* _retn() {
      OrderIndexInfoList* _tmp_ptr;
      _tmp_ptr = _ptr;
      _ptr = (OrderIndexInfoList *)NULL;
      return _tmp_ptr;
    }
    friend VISostream& _marshal_in(VISostream&, const OrderIndexInfoList_var&);
    friend VISistream& _marshal_out(VISistream&, OrderIndexInfoList_var&);

    OrderIndexInfo& operator[](CORBA::ULong _index);
    const OrderIndexInfo& operator[](CORBA::ULong _index) const;

  };
  class  OrderIndexInfoList_out {
   private:
    OrderIndexInfoList_ptr& _ptr;
    static OrderIndexInfoList* _nil() { return (OrderIndexInfoList*)NULL; }

    void operator=(const OrderIndexInfoList_out&);
    void operator=(const OrderIndexInfoList_var&);
     public:
    OrderIndexInfoList_out(const OrderIndexInfoList_out& _o) : _ptr(_o._ptr) {}
    OrderIndexInfoList_out(OrderIndexInfoList_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    OrderIndexInfoList_out(OrderIndexInfoList_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    OrderIndexInfoList_out& operator=(OrderIndexInfoList* _p) {
      _ptr = _p;
      return *this;
    }
    operator OrderIndexInfoList_ptr&() { return _ptr; }
    OrderIndexInfoList_ptr& ptr() { return _ptr; }
  };
  #ifndef PetStore_OrderHomeExplicit_var_
  #define PetStore_OrderHomeExplicit_var_

  class  OrderHomeExplicit;

  typedef OrderHomeExplicit* OrderHomeExplicit_ptr;
  typedef OrderHomeExplicit_ptr OrderHomeExplicitRef;
   VISistream& _marshal_out(VISistream&, OrderHomeExplicit_ptr&);
   VISostream& _marshal_in(VISostream&, const OrderHomeExplicit_ptr);
  class  OrderHomeExplicit_out;

  class  OrderHomeExplicit_var : public CORBA::_var {
  friend class OrderHomeExplicit_out;
   private:
    OrderHomeExplicit_ptr _ptr;

   public:
    OrderHomeExplicit_var();
    OrderHomeExplicit_var(OrderHomeExplicit_ptr);
    OrderHomeExplicit_var(const OrderHomeExplicit_var &);
    virtual ~OrderHomeExplicit_var();

    static OrderHomeExplicit_ptr _duplicate(OrderHomeExplicit_ptr);
    static void _release(OrderHomeExplicit_ptr);

    OrderHomeExplicit_var& operator=(const OrderHomeExplicit_var& _var);
    OrderHomeExplicit_var& operator=(OrderHomeExplicit_ptr);

    operator OrderHomeExplicit*() const { return _ptr; }
    OrderHomeExplicit* operator->() const { return _ptr; }

    OrderHomeExplicit_ptr in() const { return _ptr; }
    OrderHomeExplicit_ptr& inout() { return _ptr; }
    OrderHomeExplicit_ptr& out();
    OrderHomeExplicit_ptr _retn();

    friend VISostream& _marshal_in(VISostream&, const OrderHomeExplicit_var&);
    friend VISistream& _marshal_out(VISistream&, OrderHomeExplicit_var&);

    friend Istream& _marshal_out(Istream&, OrderHomeExplicit_var&);
    friend Ostream& _marshal_in(Ostream&, const OrderHomeExplicit_var&);
  };
  class  OrderHomeExplicit_out {
   private:
    OrderHomeExplicit_ptr& _ptr;
    static OrderHomeExplicit* _nil() { return (OrderHomeExplicit*)NULL; }

    void operator=(const OrderHomeExplicit_out&);
    void operator=(const OrderHomeExplicit_var&);
     public:
    OrderHomeExplicit_out(const OrderHomeExplicit_out& _o) : _ptr(_o._ptr) {}
    OrderHomeExplicit_out(OrderHomeExplicit_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    OrderHomeExplicit_out(OrderHomeExplicit_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    OrderHomeExplicit_out& operator=(OrderHomeExplicit* _p) {
      _ptr = _p;
      return *this;
    }
    operator OrderHomeExplicit_ptr&() { return _ptr; }
    OrderHomeExplicit_ptr& ptr() { return _ptr; }
    OrderHomeExplicit* operator->() { return _ptr; }
  };

  #endif // PetStore_OrderHomeExplicit_var_

  // idl interface: PetStore::OrderHomeExplicit
  class  OrderHomeExplicit : public virtual ::Components::CCMHome, public virtual CORBA_Object {
   private:
    static const CORBA::TypeInfo _class_info;
    void operator=(const OrderHomeExplicit&) {}

    protected:
    OrderHomeExplicit() {}
    OrderHomeExplicit(const OrderHomeExplicit&) {}

   public:
    virtual ~OrderHomeExplicit() {}
    static  const CORBA::TypeInfo *_desc();
    virtual const CORBA::TypeInfo *_type_info() const;
    virtual void *_safe_narrow(const CORBA::TypeInfo& ) const;
    static CORBA::Object*_factory();
    OrderHomeExplicit_ptr _this();
    static OrderHomeExplicit_ptr _duplicate(OrderHomeExplicit_ptr _obj) {
      if (_obj) _obj->_ref();
      return _obj;
    }
    static OrderHomeExplicit_ptr _nil() { return (OrderHomeExplicit_ptr)NULL; }
    static OrderHomeExplicit_ptr _narrow(CORBA::Object* _obj);
    static OrderHomeExplicit_ptr _clone(OrderHomeExplicit_ptr _obj) {
      CORBA::Object_var _obj_var(CORBA_Object::_clone(_obj));
      #if defined(_HPCC_BUG)
        return _narrow(_obj_var.operator CORBA::Object_ptr());
      #else
        return _narrow(_obj_var);
      #endif  // _HPCC_BUG
    }
    static OrderHomeExplicit_ptr _bind(const char *_object_name = (const char*)NULL,
                                       const char *_host_name = (const char*)NULL,
                                       const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                                       CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    static OrderHomeExplicit_ptr _bind(const char *_poa_name,
                                       const CORBA::OctetSequence& _id,
                                       const char *_host_name = (const char*)NULL,
                                       const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                                       CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    virtual OrderIndexInfoList* findAllOrders();

    friend VISostream& _marshal_in(VISostream& _strm, const OrderHomeExplicit_ptr _obj);
    friend VISistream& _marshal_out(VISistream& _strm, OrderHomeExplicit_ptr& _obj);
    friend Ostream& _marshal_in(Ostream& _strm, const OrderHomeExplicit_ptr _obj);
    friend Istream& _marshal_out(Istream& _strm, OrderHomeExplicit_ptr& _obj);
  };

  class OrderHomeExplicit_ops;
  typedef OrderHomeExplicit_ops* OrderHomeExplicit_ops_ptr;

  class  OrderHomeExplicit_ops  {
   public:
    OrderHomeExplicit_ops() {}
    virtual ~OrderHomeExplicit_ops () {}

    virtual OrderIndexInfoList* findAllOrders() = 0;

    static const VISOps_Info *_desc();
    static OrderHomeExplicit_ops_ptr _nil() { return (OrderHomeExplicit_ops_ptr)NULL; }
    static OrderHomeExplicit_ops_ptr _downcast(PortableServer::ServantBase* _servant);
  protected:
    static const VISOps_Info _ops_info;
  };

  #ifndef PetStore_OrderHomeImplicit_var_
  #define PetStore_OrderHomeImplicit_var_

  class  OrderHomeImplicit;

  typedef OrderHomeImplicit* OrderHomeImplicit_ptr;
  typedef OrderHomeImplicit_ptr OrderHomeImplicitRef;
   VISistream& _marshal_out(VISistream&, OrderHomeImplicit_ptr&);
   VISostream& _marshal_in(VISostream&, const OrderHomeImplicit_ptr);
  class  OrderHomeImplicit_out;

  class  OrderHomeImplicit_var : public CORBA::_var {
  friend class OrderHomeImplicit_out;
   private:
    OrderHomeImplicit_ptr _ptr;

   public:
    OrderHomeImplicit_var();
    OrderHomeImplicit_var(OrderHomeImplicit_ptr);
    OrderHomeImplicit_var(const OrderHomeImplicit_var &);
    virtual ~OrderHomeImplicit_var();

    static OrderHomeImplicit_ptr _duplicate(OrderHomeImplicit_ptr);
    static void _release(OrderHomeImplicit_ptr);

    OrderHomeImplicit_var& operator=(const OrderHomeImplicit_var& _var);
    OrderHomeImplicit_var& operator=(OrderHomeImplicit_ptr);

    operator OrderHomeImplicit*() const { return _ptr; }
    OrderHomeImplicit* operator->() const { return _ptr; }

    OrderHomeImplicit_ptr in() const { return _ptr; }
    OrderHomeImplicit_ptr& inout() { return _ptr; }
    OrderHomeImplicit_ptr& out();
    OrderHomeImplicit_ptr _retn();

    friend VISostream& _marshal_in(VISostream&, const OrderHomeImplicit_var&);
    friend VISistream& _marshal_out(VISistream&, OrderHomeImplicit_var&);

    friend Istream& _marshal_out(Istream&, OrderHomeImplicit_var&);
    friend Ostream& _marshal_in(Ostream&, const OrderHomeImplicit_var&);
  };
  class  OrderHomeImplicit_out {
   private:
    OrderHomeImplicit_ptr& _ptr;
    static OrderHomeImplicit* _nil() { return (OrderHomeImplicit*)NULL; }

    void operator=(const OrderHomeImplicit_out&);
    void operator=(const OrderHomeImplicit_var&);
     public:
    OrderHomeImplicit_out(const OrderHomeImplicit_out& _o) : _ptr(_o._ptr) {}
    OrderHomeImplicit_out(OrderHomeImplicit_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    OrderHomeImplicit_out(OrderHomeImplicit_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    OrderHomeImplicit_out& operator=(OrderHomeImplicit* _p) {
      _ptr = _p;
      return *this;
    }
    operator OrderHomeImplicit_ptr&() { return _ptr; }
    OrderHomeImplicit_ptr& ptr() { return _ptr; }
    OrderHomeImplicit* operator->() { return _ptr; }
  };

  #endif // PetStore_OrderHomeImplicit_var_

  // idl interface: PetStore::OrderHomeImplicit
  class  OrderHomeImplicit : public virtual CORBA_Object {
   private:
    static const CORBA::TypeInfo _class_info;
    void operator=(const OrderHomeImplicit&) {}

    protected:
    OrderHomeImplicit() {}
    OrderHomeImplicit(const OrderHomeImplicit&) {}

   public:
    virtual ~OrderHomeImplicit() {}
    static  const CORBA::TypeInfo *_desc();
    virtual const CORBA::TypeInfo *_type_info() const;
    virtual void *_safe_narrow(const CORBA::TypeInfo& ) const;
    static CORBA::Object*_factory();
    OrderHomeImplicit_ptr _this();
    static OrderHomeImplicit_ptr _duplicate(OrderHomeImplicit_ptr _obj) {
      if (_obj) _obj->_ref();
      return _obj;
    }
    static OrderHomeImplicit_ptr _nil() { return (OrderHomeImplicit_ptr)NULL; }
    static OrderHomeImplicit_ptr _narrow(CORBA::Object* _obj);
    static OrderHomeImplicit_ptr _clone(OrderHomeImplicit_ptr _obj) {
      CORBA::Object_var _obj_var(CORBA_Object::_clone(_obj));
      #if defined(_HPCC_BUG)
        return _narrow(_obj_var.operator CORBA::Object_ptr());
      #else
        return _narrow(_obj_var);
      #endif  // _HPCC_BUG
    }
    static OrderHomeImplicit_ptr _bind(const char *_object_name = (const char*)NULL,
                                       const char *_host_name = (const char*)NULL,
                                       const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                                       CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    static OrderHomeImplicit_ptr _bind(const char *_poa_name,
                                       const CORBA::OctetSequence& _id,
                                       const char *_host_name = (const char*)NULL,
                                       const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                                       CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    virtual Order_ptr create(KeyID_ptr _pkey);
    virtual Order_ptr find_by_primary_key(KeyID_ptr _pkey);
    virtual void remove(KeyID_ptr _pkey);
    virtual KeyID_ptr get_primary_key(Order_ptr _comp);

    friend VISostream& _marshal_in(VISostream& _strm, const OrderHomeImplicit_ptr _obj);
    friend VISistream& _marshal_out(VISistream& _strm, OrderHomeImplicit_ptr& _obj);
    friend Ostream& _marshal_in(Ostream& _strm, const OrderHomeImplicit_ptr _obj);
    friend Istream& _marshal_out(Istream& _strm, OrderHomeImplicit_ptr& _obj);
  };

  class OrderHomeImplicit_ops;
  typedef OrderHomeImplicit_ops* OrderHomeImplicit_ops_ptr;

  class  OrderHomeImplicit_ops  {
   public:
    OrderHomeImplicit_ops() {}
    virtual ~OrderHomeImplicit_ops () {}

    virtual Order_ptr create(KeyID_ptr _pkey) = 0;
    virtual Order_ptr find_by_primary_key(KeyID_ptr _pkey) = 0;
    virtual void remove(KeyID_ptr _pkey) = 0;
    virtual KeyID_ptr get_primary_key(Order_ptr _comp) = 0;

    static const VISOps_Info *_desc();
    static OrderHomeImplicit_ops_ptr _nil() { return (OrderHomeImplicit_ops_ptr)NULL; }
    static OrderHomeImplicit_ops_ptr _downcast(PortableServer::ServantBase* _servant);
  protected:
    static const VISOps_Info _ops_info;
  };

  #ifndef PetStore_OrderHome_var_
  #define PetStore_OrderHome_var_

  class  OrderHome;

  typedef OrderHome* OrderHome_ptr;
  typedef OrderHome_ptr OrderHomeRef;
   VISistream& _marshal_out(VISistream&, OrderHome_ptr&);
   VISostream& _marshal_in(VISostream&, const OrderHome_ptr);
  class  OrderHome_out;

  class  OrderHome_var : public CORBA::_var {
  friend class OrderHome_out;
   private:
    OrderHome_ptr _ptr;

   public:
    OrderHome_var();
    OrderHome_var(OrderHome_ptr);
    OrderHome_var(const OrderHome_var &);
    virtual ~OrderHome_var();

    static OrderHome_ptr _duplicate(OrderHome_ptr);
    static void _release(OrderHome_ptr);

    OrderHome_var& operator=(const OrderHome_var& _var);
    OrderHome_var& operator=(OrderHome_ptr);

    operator OrderHome*() const { return _ptr; }
    OrderHome* operator->() const { return _ptr; }

    OrderHome_ptr in() const { return _ptr; }
    OrderHome_ptr& inout() { return _ptr; }
    OrderHome_ptr& out();
    OrderHome_ptr _retn();

    friend VISostream& _marshal_in(VISostream&, const OrderHome_var&);
    friend VISistream& _marshal_out(VISistream&, OrderHome_var&);

    friend Istream& _marshal_out(Istream&, OrderHome_var&);
    friend Ostream& _marshal_in(Ostream&, const OrderHome_var&);
  };
  class  OrderHome_out {
   private:
    OrderHome_ptr& _ptr;
    static OrderHome* _nil() { return (OrderHome*)NULL; }

    void operator=(const OrderHome_out&);
    void operator=(const OrderHome_var&);
     public:
    OrderHome_out(const OrderHome_out& _o) : _ptr(_o._ptr) {}
    OrderHome_out(OrderHome_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    OrderHome_out(OrderHome_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    OrderHome_out& operator=(OrderHome* _p) {
      _ptr = _p;
      return *this;
    }
    operator OrderHome_ptr&() { return _ptr; }
    OrderHome_ptr& ptr() { return _ptr; }
    OrderHome* operator->() { return _ptr; }
  };

  #endif // PetStore_OrderHome_var_

  // idl interface: PetStore::OrderHome
  class  OrderHome : public virtual OrderHomeExplicit, public virtual OrderHomeImplicit, public virtual CORBA_Object {
   private:
    static const CORBA::TypeInfo _class_info;
    void operator=(const OrderHome&) {}

    protected:
    OrderHome() {}
    OrderHome(const OrderHome&) {}

   public:
    virtual ~OrderHome() {}
    static  const CORBA::TypeInfo *_desc();
    virtual const CORBA::TypeInfo *_type_info() const;
    virtual void *_safe_narrow(const CORBA::TypeInfo& ) const;
    static CORBA::Object*_factory();
    OrderHome_ptr _this();
    static OrderHome_ptr _duplicate(OrderHome_ptr _obj) {
      if (_obj) _obj->_ref();
      return _obj;
    }
    static OrderHome_ptr _nil() { return (OrderHome_ptr)NULL; }
    static OrderHome_ptr _narrow(CORBA::Object* _obj);
    static OrderHome_ptr _clone(OrderHome_ptr _obj) {
      CORBA::Object_var _obj_var(CORBA_Object::_clone(_obj));
      #if defined(_HPCC_BUG)
        return _narrow(_obj_var.operator CORBA::Object_ptr());
      #else
        return _narrow(_obj_var);
      #endif  // _HPCC_BUG
    }
    static OrderHome_ptr _bind(const char *_object_name = (const char*)NULL,
                               const char *_host_name = (const char*)NULL,
                               const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                               CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    static OrderHome_ptr _bind(const char *_poa_name,
                               const CORBA::OctetSequence& _id,
                               const char *_host_name = (const char*)NULL,
                               const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                               CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    friend VISostream& _marshal_in(VISostream& _strm, const OrderHome_ptr _obj);
    friend VISistream& _marshal_out(VISistream& _strm, OrderHome_ptr& _obj);
    friend Ostream& _marshal_in(Ostream& _strm, const OrderHome_ptr _obj);
    friend Istream& _marshal_out(Istream& _strm, OrderHome_ptr& _obj);
  };

  class OrderHome_ops;
  typedef OrderHome_ops* OrderHome_ops_ptr;

  class  OrderHome_ops  {
   public:
    OrderHome_ops() {}
    virtual ~OrderHome_ops () {}

    static const VISOps_Info *_desc();
    static OrderHome_ops_ptr _nil() { return (OrderHome_ops_ptr)NULL; }
    static OrderHome_ops_ptr _downcast(PortableServer::ServantBase* _servant);
  protected:
    static const VISOps_Info _ops_info;
  };

  #ifndef PetStore_SalesReportConsumer_var_
  #define PetStore_SalesReportConsumer_var_

  class  SalesReportConsumer;

  typedef SalesReportConsumer* SalesReportConsumer_ptr;
  typedef SalesReportConsumer_ptr SalesReportConsumerRef;
   VISistream& _marshal_out(VISistream&, SalesReportConsumer_ptr&);
   VISostream& _marshal_in(VISostream&, const SalesReportConsumer_ptr);
  class  SalesReportConsumer_out;

  class  SalesReportConsumer_var : public CORBA::_var {
  friend class SalesReportConsumer_out;
   private:
    SalesReportConsumer_ptr _ptr;

   public:
    SalesReportConsumer_var();
    SalesReportConsumer_var(SalesReportConsumer_ptr);
    SalesReportConsumer_var(const SalesReportConsumer_var &);
    virtual ~SalesReportConsumer_var();

    static SalesReportConsumer_ptr _duplicate(SalesReportConsumer_ptr);
    static void _release(SalesReportConsumer_ptr);

    SalesReportConsumer_var& operator=(const SalesReportConsumer_var& _var);
    SalesReportConsumer_var& operator=(SalesReportConsumer_ptr);

    operator SalesReportConsumer*() const { return _ptr; }
    SalesReportConsumer* operator->() const { return _ptr; }

    SalesReportConsumer_ptr in() const { return _ptr; }
    SalesReportConsumer_ptr& inout() { return _ptr; }
    SalesReportConsumer_ptr& out();
    SalesReportConsumer_ptr _retn();

    friend VISostream& _marshal_in(VISostream&, const SalesReportConsumer_var&);
    friend VISistream& _marshal_out(VISistream&, SalesReportConsumer_var&);

    friend Istream& _marshal_out(Istream&, SalesReportConsumer_var&);
    friend Ostream& _marshal_in(Ostream&, const SalesReportConsumer_var&);
  };
  class  SalesReportConsumer_out {
   private:
    SalesReportConsumer_ptr& _ptr;
    static SalesReportConsumer* _nil() { return (SalesReportConsumer*)NULL; }

    void operator=(const SalesReportConsumer_out&);
    void operator=(const SalesReportConsumer_var&);
     public:
    SalesReportConsumer_out(const SalesReportConsumer_out& _o) : _ptr(_o._ptr) {}
    SalesReportConsumer_out(SalesReportConsumer_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    SalesReportConsumer_out(SalesReportConsumer_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    SalesReportConsumer_out& operator=(SalesReportConsumer* _p) {
      _ptr = _p;
      return *this;
    }
    operator SalesReportConsumer_ptr&() { return _ptr; }
    SalesReportConsumer_ptr& ptr() { return _ptr; }
    SalesReportConsumer* operator->() { return _ptr; }
  };

  #endif // PetStore_SalesReportConsumer_var_

  // idl interface: PetStore::SalesReportConsumer
  class  SalesReportConsumer : public virtual ::Components::EventConsumerBase, public virtual CORBA_Object {
   private:
    static const CORBA::TypeInfo _class_info;
    void operator=(const SalesReportConsumer&) {}

    protected:
    SalesReportConsumer() {}
    SalesReportConsumer(const SalesReportConsumer&) {}

   public:
    virtual ~SalesReportConsumer() {}
    static  const CORBA::TypeInfo *_desc();
    virtual const CORBA::TypeInfo *_type_info() const;
    virtual void *_safe_narrow(const CORBA::TypeInfo& ) const;
    static CORBA::Object*_factory();
    SalesReportConsumer_ptr _this();
    static SalesReportConsumer_ptr _duplicate(SalesReportConsumer_ptr _obj) {
      if (_obj) _obj->_ref();
      return _obj;
    }
    static SalesReportConsumer_ptr _nil() { return (SalesReportConsumer_ptr)NULL; }
    static SalesReportConsumer_ptr _narrow(CORBA::Object* _obj);
    static SalesReportConsumer_ptr _clone(SalesReportConsumer_ptr _obj) {
      CORBA::Object_var _obj_var(CORBA_Object::_clone(_obj));
      #if defined(_HPCC_BUG)
        return _narrow(_obj_var.operator CORBA::Object_ptr());
      #else
        return _narrow(_obj_var);
      #endif  // _HPCC_BUG
    }
    static SalesReportConsumer_ptr _bind(const char *_object_name = (const char*)NULL,
                                         const char *_host_name = (const char*)NULL,
                                         const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                                         CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    static SalesReportConsumer_ptr _bind(const char *_poa_name,
                                         const CORBA::OctetSequence& _id,
                                         const char *_host_name = (const char*)NULL,
                                         const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                                         CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    virtual void push_SalesReportConsumer(SalesReport_ptr _event);

    friend VISostream& _marshal_in(VISostream& _strm, const SalesReportConsumer_ptr _obj);
    friend VISistream& _marshal_out(VISistream& _strm, SalesReportConsumer_ptr& _obj);
    friend Ostream& _marshal_in(Ostream& _strm, const SalesReportConsumer_ptr _obj);
    friend Istream& _marshal_out(Istream& _strm, SalesReportConsumer_ptr& _obj);
  };

  class SalesReportConsumer_ops;
  typedef SalesReportConsumer_ops* SalesReportConsumer_ops_ptr;

  class  SalesReportConsumer_ops  {
   public:
    SalesReportConsumer_ops() {}
    virtual ~SalesReportConsumer_ops () {}

    virtual void push_SalesReportConsumer(SalesReport_ptr _event) = 0;

    static const VISOps_Info *_desc();
    static SalesReportConsumer_ops_ptr _nil() { return (SalesReportConsumer_ops_ptr)NULL; }
    static SalesReportConsumer_ops_ptr _downcast(PortableServer::ServantBase* _servant);
  protected:
    static const VISOps_Info _ops_info;
  };

  #ifndef PetStore_Sales_var_
  #define PetStore_Sales_var_

  class  Sales;

  typedef Sales* Sales_ptr;
  typedef Sales_ptr SalesRef;
   VISistream& _marshal_out(VISistream&, Sales_ptr&);
   VISostream& _marshal_in(VISostream&, const Sales_ptr);
  class  Sales_out;

  class  Sales_var : public CORBA::_var {
  friend class Sales_out;
   private:
    Sales_ptr _ptr;

   public:
    Sales_var();
    Sales_var(Sales_ptr);
    Sales_var(const Sales_var &);
    virtual ~Sales_var();

    static Sales_ptr _duplicate(Sales_ptr);
    static void _release(Sales_ptr);

    Sales_var& operator=(const Sales_var& _var);
    Sales_var& operator=(Sales_ptr);

    operator Sales*() const { return _ptr; }
    Sales* operator->() const { return _ptr; }

    Sales_ptr in() const { return _ptr; }
    Sales_ptr& inout() { return _ptr; }
    Sales_ptr& out();
    Sales_ptr _retn();

    friend VISostream& _marshal_in(VISostream&, const Sales_var&);
    friend VISistream& _marshal_out(VISistream&, Sales_var&);

    friend Istream& _marshal_out(Istream&, Sales_var&);
    friend Ostream& _marshal_in(Ostream&, const Sales_var&);
  };
  class  Sales_out {
   private:
    Sales_ptr& _ptr;
    static Sales* _nil() { return (Sales*)NULL; }

    void operator=(const Sales_out&);
    void operator=(const Sales_var&);
     public:
    Sales_out(const Sales_out& _o) : _ptr(_o._ptr) {}
    Sales_out(Sales_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    Sales_out(Sales_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    Sales_out& operator=(Sales* _p) {
      _ptr = _p;
      return *this;
    }
    operator Sales_ptr&() { return _ptr; }
    Sales_ptr& ptr() { return _ptr; }
    Sales* operator->() { return _ptr; }
  };

  #endif // PetStore_Sales_var_

  // idl interface: PetStore::Sales
  class  Sales : public virtual ::Components::CCMObject, public virtual CORBA_Object {
   private:
    static const CORBA::TypeInfo _class_info;
    void operator=(const Sales&) {}

    protected:
    Sales() {}
    Sales(const Sales&) {}

   public:
    virtual ~Sales() {}
    static  const CORBA::TypeInfo *_desc();
    virtual const CORBA::TypeInfo *_type_info() const;
    virtual void *_safe_narrow(const CORBA::TypeInfo& ) const;
    static CORBA::Object*_factory();
    Sales_ptr _this();
    static Sales_ptr _duplicate(Sales_ptr _obj) {
      if (_obj) _obj->_ref();
      return _obj;
    }
    static Sales_ptr _nil() { return (Sales_ptr)NULL; }
    static Sales_ptr _narrow(CORBA::Object* _obj);
    static Sales_ptr _clone(Sales_ptr _obj) {
      CORBA::Object_var _obj_var(CORBA_Object::_clone(_obj));
      #if defined(_HPCC_BUG)
        return _narrow(_obj_var.operator CORBA::Object_ptr());
      #else
        return _narrow(_obj_var);
      #endif  // _HPCC_BUG
    }
    static Sales_ptr _bind(const char *_object_name = (const char*)NULL,
                           const char *_host_name = (const char*)NULL,
                           const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                           CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    static Sales_ptr _bind(const char *_poa_name,
                           const CORBA::OctetSequence& _id,
                           const char *_host_name = (const char*)NULL,
                           const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                           CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    virtual BusinessOp_ptr provide_for_client();
    virtual AdvancedOp_ptr provide_for_mgr();
    virtual void connect_the_calculate(Calculate_ptr _conn);
    virtual Calculate_ptr disconnect_the_calculate();
    virtual Calculate_ptr get_connection_the_calculate();
    virtual void connect_product_home(ProductHome_ptr _conn);
    virtual ProductHome_ptr disconnect_product_home();
    virtual ProductHome_ptr get_connection_product_home();
    virtual void connect_order_home(OrderHome_ptr _conn);
    virtual OrderHome_ptr disconnect_order_home();
    virtual OrderHome_ptr get_connection_order_home();
    virtual void connect_report(SalesReportConsumer_ptr _consumer);
    virtual SalesReportConsumer_ptr disconnect_report();

    friend VISostream& _marshal_in(VISostream& _strm, const Sales_ptr _obj);
    friend VISistream& _marshal_out(VISistream& _strm, Sales_ptr& _obj);
    friend Ostream& _marshal_in(Ostream& _strm, const Sales_ptr _obj);
    friend Istream& _marshal_out(Istream& _strm, Sales_ptr& _obj);
  };

  class Sales_ops;
  typedef Sales_ops* Sales_ops_ptr;

  class  Sales_ops  {
   public:
    Sales_ops() {}
    virtual ~Sales_ops () {}

    virtual BusinessOp_ptr provide_for_client() = 0;
    virtual AdvancedOp_ptr provide_for_mgr() = 0;
    virtual void connect_the_calculate(Calculate_ptr _conn) = 0;
    virtual Calculate_ptr disconnect_the_calculate() = 0;
    virtual Calculate_ptr get_connection_the_calculate() = 0;
    virtual void connect_product_home(ProductHome_ptr _conn) = 0;
    virtual ProductHome_ptr disconnect_product_home() = 0;
    virtual ProductHome_ptr get_connection_product_home() = 0;
    virtual void connect_order_home(OrderHome_ptr _conn) = 0;
    virtual OrderHome_ptr disconnect_order_home() = 0;
    virtual OrderHome_ptr get_connection_order_home() = 0;
    virtual void connect_report(SalesReportConsumer_ptr _consumer) = 0;
    virtual SalesReportConsumer_ptr disconnect_report() = 0;

    static const VISOps_Info *_desc();
    static Sales_ops_ptr _nil() { return (Sales_ops_ptr)NULL; }
    static Sales_ops_ptr _downcast(PortableServer::ServantBase* _servant);
  protected:
    static const VISOps_Info _ops_info;
  };

  // idl struct: PetStore::OrderRecord
  struct  OrderRecord {

    OrderIndexInfoList order_index_list;
    CORBA::ULong sub_total;
    #if defined(MSVCNEWDLL_BUG)
      void *operator new(size_t ts);
      void *operator new(size_t ts, char*, int) {return operator new(ts);}
      void operator delete(void *p);
    #endif // MSVCNEWDLL_BUG
    friend VISostream& _marshal_in(VISostream&, const OrderRecord&);
    friend VISistream& _marshal_out(VISistream&, OrderRecord&);

    friend VISostream& _marshal_in(VISostream& _strm, const OrderRecord* _obj);
    friend VISistream& _marshal_out(VISistream& _strm, OrderRecord*& _obj);

    friend void _internal_pretty_print(VISostream&, const OrderRecord&);
    friend Ostream& _marshal_in(Ostream&, const OrderRecord&);
    friend Istream& _marshal_out(Istream&, OrderRecord&);
    friend Istream& _marshal_out(Istream&, OrderRecord*&);
  };
  typedef OrderRecord * OrderRecord_ptr;

  class  OrderRecord_out;

  class  OrderRecord_var {
    friend class OrderRecord_out;
  private:
    OrderRecord_ptr _ptr;
  public:
    OrderRecord_var() : _ptr((OrderRecord*)NULL) {}
    OrderRecord_var(OrderRecord *_p) : _ptr(_p) {}

    OrderRecord_var(const OrderRecord_var& _var) {
      _ptr = _var._ptr ? (new OrderRecord(*_var._ptr)) : (OrderRecord*)NULL;
    }

    virtual ~OrderRecord_var() { delete _ptr; }
    OrderRecord_var& operator=(OrderRecord* _p);
    OrderRecord_var& operator=(const OrderRecord_var& _var);
    operator OrderRecord*() const { return _ptr; }

    OrderRecord* operator->() { return _ptr; }
    const OrderRecord* operator->() const { return _ptr; }

    operator OrderRecord&() { return *_ptr; }
    operator const OrderRecord&() const { return *_ptr; }

    const OrderRecord& in() const { return *_ptr; }
    OrderRecord& inout() { return *_ptr; }
    OrderRecord_ptr& out() {
      delete _ptr;
      _ptr = (OrderRecord_ptr) NULL;
      return _ptr;
    }

    OrderRecord_ptr _retn() {
      OrderRecord_ptr temp_ptr;
      temp_ptr = _ptr;
      _ptr = (OrderRecord_ptr) NULL;
      return temp_ptr;
    }
    friend VISostream& _marshal_in(VISostream&, const OrderRecord_var&);
    friend VISistream& _marshal_out(VISistream&, OrderRecord_var&);
  };
  class  OrderRecord_out {
   private:
    OrderRecord_ptr& _ptr;
    static OrderRecord* _nil() { return (OrderRecord*)NULL; }

    void operator=(const OrderRecord_out&);
    void operator=(const OrderRecord_var&);
     public:
    OrderRecord_out(const OrderRecord_out& _o) : _ptr(_o._ptr) {}
    OrderRecord_out(OrderRecord_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    OrderRecord_out(OrderRecord_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    OrderRecord_out& operator=(OrderRecord* _p) {
      _ptr = _p;
      return *this;
    }
    operator OrderRecord_ptr&() { return _ptr; }
    OrderRecord_ptr& ptr() { return _ptr; }
    OrderRecord* operator->() { return _ptr; }
  };
  #ifndef PetStore_Manage_var_
  #define PetStore_Manage_var_

  class  Manage;

  typedef Manage* Manage_ptr;
  typedef Manage_ptr ManageRef;
   VISistream& _marshal_out(VISistream&, Manage_ptr&);
   VISostream& _marshal_in(VISostream&, const Manage_ptr);
  class  Manage_out;

  class  Manage_var : public CORBA::_var {
  friend class Manage_out;
   private:
    Manage_ptr _ptr;

   public:
    Manage_var();
    Manage_var(Manage_ptr);
    Manage_var(const Manage_var &);
    virtual ~Manage_var();

    static Manage_ptr _duplicate(Manage_ptr);
    static void _release(Manage_ptr);

    Manage_var& operator=(const Manage_var& _var);
    Manage_var& operator=(Manage_ptr);

    operator Manage*() const { return _ptr; }
    Manage* operator->() const { return _ptr; }

    Manage_ptr in() const { return _ptr; }
    Manage_ptr& inout() { return _ptr; }
    Manage_ptr& out();
    Manage_ptr _retn();

    friend VISostream& _marshal_in(VISostream&, const Manage_var&);
    friend VISistream& _marshal_out(VISistream&, Manage_var&);

    friend Istream& _marshal_out(Istream&, Manage_var&);
    friend Ostream& _marshal_in(Ostream&, const Manage_var&);
  };
  class  Manage_out {
   private:
    Manage_ptr& _ptr;
    static Manage* _nil() { return (Manage*)NULL; }

    void operator=(const Manage_out&);
    void operator=(const Manage_var&);
     public:
    Manage_out(const Manage_out& _o) : _ptr(_o._ptr) {}
    Manage_out(Manage_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    Manage_out(Manage_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    Manage_out& operator=(Manage* _p) {
      _ptr = _p;
      return *this;
    }
    operator Manage_ptr&() { return _ptr; }
    Manage_ptr& ptr() { return _ptr; }
    Manage* operator->() { return _ptr; }
  };

  #endif // PetStore_Manage_var_

  // idl interface: PetStore::Manage
  class  Manage : public virtual CORBA_Object {
   private:
    static const CORBA::TypeInfo _class_info;
    void operator=(const Manage&) {}

    protected:
    Manage() {}
    Manage(const Manage&) {}

   public:
    virtual ~Manage() {}
    static  const CORBA::TypeInfo *_desc();
    virtual const CORBA::TypeInfo *_type_info() const;
    virtual void *_safe_narrow(const CORBA::TypeInfo& ) const;
    static CORBA::Object*_factory();
    Manage_ptr _this();
    static Manage_ptr _duplicate(Manage_ptr _obj) {
      if (_obj) _obj->_ref();
      return _obj;
    }
    static Manage_ptr _nil() { return (Manage_ptr)NULL; }
    static Manage_ptr _narrow(CORBA::Object* _obj);
    static Manage_ptr _clone(Manage_ptr _obj) {
      CORBA::Object_var _obj_var(CORBA_Object::_clone(_obj));
      #if defined(_HPCC_BUG)
        return _narrow(_obj_var.operator CORBA::Object_ptr());
      #else
        return _narrow(_obj_var);
      #endif  // _HPCC_BUG
    }
    static Manage_ptr _bind(const char *_object_name = (const char*)NULL,
                            const char *_host_name = (const char*)NULL,
                            const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                            CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    static Manage_ptr _bind(const char *_poa_name,
                            const CORBA::OctetSequence& _id,
                            const char *_host_name = (const char*)NULL,
                            const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                            CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    virtual void addProduct(const ProductInfo& _newProduct);
    virtual OrderInfoList* listAllOrders();
    virtual OrderRecord* account();

    friend VISostream& _marshal_in(VISostream& _strm, const Manage_ptr _obj);
    friend VISistream& _marshal_out(VISistream& _strm, Manage_ptr& _obj);
    friend Ostream& _marshal_in(Ostream& _strm, const Manage_ptr _obj);
    friend Istream& _marshal_out(Istream& _strm, Manage_ptr& _obj);
  };

  class Manage_ops;
  typedef Manage_ops* Manage_ops_ptr;

  class  Manage_ops  {
   public:
    Manage_ops() {}
    virtual ~Manage_ops () {}

    virtual void addProduct(const ProductInfo& _newProduct) = 0;
    virtual OrderInfoList* listAllOrders() = 0;
    virtual OrderRecord* account() = 0;

    static const VISOps_Info *_desc();
    static Manage_ops_ptr _nil() { return (Manage_ops_ptr)NULL; }
    static Manage_ops_ptr _downcast(PortableServer::ServantBase* _servant);
  protected:
    static const VISOps_Info _ops_info;
  };

  #ifndef PetStore_Manager_var_
  #define PetStore_Manager_var_

  class  Manager;

  typedef Manager* Manager_ptr;
  typedef Manager_ptr ManagerRef;
   VISistream& _marshal_out(VISistream&, Manager_ptr&);
   VISostream& _marshal_in(VISostream&, const Manager_ptr);
  class  Manager_out;

  class  Manager_var : public CORBA::_var {
  friend class Manager_out;
   private:
    Manager_ptr _ptr;

   public:
    Manager_var();
    Manager_var(Manager_ptr);
    Manager_var(const Manager_var &);
    virtual ~Manager_var();

    static Manager_ptr _duplicate(Manager_ptr);
    static void _release(Manager_ptr);

    Manager_var& operator=(const Manager_var& _var);
    Manager_var& operator=(Manager_ptr);

    operator Manager*() const { return _ptr; }
    Manager* operator->() const { return _ptr; }

    Manager_ptr in() const { return _ptr; }
    Manager_ptr& inout() { return _ptr; }
    Manager_ptr& out();
    Manager_ptr _retn();

    friend VISostream& _marshal_in(VISostream&, const Manager_var&);
    friend VISistream& _marshal_out(VISistream&, Manager_var&);

    friend Istream& _marshal_out(Istream&, Manager_var&);
    friend Ostream& _marshal_in(Ostream&, const Manager_var&);
  };
  class  Manager_out {
   private:
    Manager_ptr& _ptr;
    static Manager* _nil() { return (Manager*)NULL; }

    void operator=(const Manager_out&);
    void operator=(const Manager_var&);
     public:
    Manager_out(const Manager_out& _o) : _ptr(_o._ptr) {}
    Manager_out(Manager_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    Manager_out(Manager_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    Manager_out& operator=(Manager* _p) {
      _ptr = _p;
      return *this;
    }
    operator Manager_ptr&() { return _ptr; }
    Manager_ptr& ptr() { return _ptr; }
    Manager* operator->() { return _ptr; }
  };

  #endif // PetStore_Manager_var_

  // idl interface: PetStore::Manager
  class  Manager : public virtual Manage, public virtual ::Components::CCMObject, public virtual CORBA_Object {
   private:
    static const CORBA::TypeInfo _class_info;
    void operator=(const Manager&) {}

    protected:
    Manager() {}
    Manager(const Manager&) {}

   public:
    virtual ~Manager() {}
    static  const CORBA::TypeInfo *_desc();
    virtual const CORBA::TypeInfo *_type_info() const;
    virtual void *_safe_narrow(const CORBA::TypeInfo& ) const;
    static CORBA::Object*_factory();
    Manager_ptr _this();
    static Manager_ptr _duplicate(Manager_ptr _obj) {
      if (_obj) _obj->_ref();
      return _obj;
    }
    static Manager_ptr _nil() { return (Manager_ptr)NULL; }
    static Manager_ptr _narrow(CORBA::Object* _obj);
    static Manager_ptr _clone(Manager_ptr _obj) {
      CORBA::Object_var _obj_var(CORBA_Object::_clone(_obj));
      #if defined(_HPCC_BUG)
        return _narrow(_obj_var.operator CORBA::Object_ptr());
      #else
        return _narrow(_obj_var);
      #endif  // _HPCC_BUG
    }
    static Manager_ptr _bind(const char *_object_name = (const char*)NULL,
                             const char *_host_name = (const char*)NULL,
                             const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                             CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    static Manager_ptr _bind(const char *_poa_name,
                             const CORBA::OctetSequence& _id,
                             const char *_host_name = (const char*)NULL,
                             const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                             CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    virtual void connect_manage_op(AdvancedOp_ptr _conn);
    virtual AdvancedOp_ptr disconnect_manage_op();
    virtual AdvancedOp_ptr get_connection_manage_op();
    virtual void connect_price_calculater(Calculate_ptr _conn);
    virtual Calculate_ptr disconnect_price_calculater();
    virtual Calculate_ptr get_connection_price_calculater();
    virtual SalesReportConsumer_ptr get_consumer_analyse();

    friend VISostream& _marshal_in(VISostream& _strm, const Manager_ptr _obj);
    friend VISistream& _marshal_out(VISistream& _strm, Manager_ptr& _obj);
    friend Ostream& _marshal_in(Ostream& _strm, const Manager_ptr _obj);
    friend Istream& _marshal_out(Istream& _strm, Manager_ptr& _obj);
  };

  class Manager_ops;
  typedef Manager_ops* Manager_ops_ptr;

  class  Manager_ops  {
   public:
    Manager_ops() {}
    virtual ~Manager_ops () {}

    virtual void connect_manage_op(AdvancedOp_ptr _conn) = 0;
    virtual AdvancedOp_ptr disconnect_manage_op() = 0;
    virtual AdvancedOp_ptr get_connection_manage_op() = 0;
    virtual void connect_price_calculater(Calculate_ptr _conn) = 0;
    virtual Calculate_ptr disconnect_price_calculater() = 0;
    virtual Calculate_ptr get_connection_price_calculater() = 0;
    virtual SalesReportConsumer_ptr get_consumer_analyse() = 0;

    static const VISOps_Info *_desc();
    static Manager_ops_ptr _nil() { return (Manager_ops_ptr)NULL; }
    static Manager_ops_ptr _downcast(PortableServer::ServantBase* _servant);
  protected:
    static const VISOps_Info _ops_info;
  };

  #ifndef PetStore_PricerHomeExplicit_var_
  #define PetStore_PricerHomeExplicit_var_

  class  PricerHomeExplicit;

  typedef PricerHomeExplicit* PricerHomeExplicit_ptr;
  typedef PricerHomeExplicit_ptr PricerHomeExplicitRef;
   VISistream& _marshal_out(VISistream&, PricerHomeExplicit_ptr&);
   VISostream& _marshal_in(VISostream&, const PricerHomeExplicit_ptr);
  class  PricerHomeExplicit_out;

  class  PricerHomeExplicit_var : public CORBA::_var {
  friend class PricerHomeExplicit_out;
   private:
    PricerHomeExplicit_ptr _ptr;

   public:
    PricerHomeExplicit_var();
    PricerHomeExplicit_var(PricerHomeExplicit_ptr);
    PricerHomeExplicit_var(const PricerHomeExplicit_var &);
    virtual ~PricerHomeExplicit_var();

    static PricerHomeExplicit_ptr _duplicate(PricerHomeExplicit_ptr);
    static void _release(PricerHomeExplicit_ptr);

    PricerHomeExplicit_var& operator=(const PricerHomeExplicit_var& _var);
    PricerHomeExplicit_var& operator=(PricerHomeExplicit_ptr);

    operator PricerHomeExplicit*() const { return _ptr; }
    PricerHomeExplicit* operator->() const { return _ptr; }

    PricerHomeExplicit_ptr in() const { return _ptr; }
    PricerHomeExplicit_ptr& inout() { return _ptr; }
    PricerHomeExplicit_ptr& out();
    PricerHomeExplicit_ptr _retn();

    friend VISostream& _marshal_in(VISostream&, const PricerHomeExplicit_var&);
    friend VISistream& _marshal_out(VISistream&, PricerHomeExplicit_var&);

    friend Istream& _marshal_out(Istream&, PricerHomeExplicit_var&);
    friend Ostream& _marshal_in(Ostream&, const PricerHomeExplicit_var&);
  };
  class  PricerHomeExplicit_out {
   private:
    PricerHomeExplicit_ptr& _ptr;
    static PricerHomeExplicit* _nil() { return (PricerHomeExplicit*)NULL; }

    void operator=(const PricerHomeExplicit_out&);
    void operator=(const PricerHomeExplicit_var&);
     public:
    PricerHomeExplicit_out(const PricerHomeExplicit_out& _o) : _ptr(_o._ptr) {}
    PricerHomeExplicit_out(PricerHomeExplicit_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    PricerHomeExplicit_out(PricerHomeExplicit_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    PricerHomeExplicit_out& operator=(PricerHomeExplicit* _p) {
      _ptr = _p;
      return *this;
    }
    operator PricerHomeExplicit_ptr&() { return _ptr; }
    PricerHomeExplicit_ptr& ptr() { return _ptr; }
    PricerHomeExplicit* operator->() { return _ptr; }
  };

  #endif // PetStore_PricerHomeExplicit_var_

  // idl interface: PetStore::PricerHomeExplicit
  class  PricerHomeExplicit : public virtual ::Components::CCMHome, public virtual CORBA_Object {
   private:
    static const CORBA::TypeInfo _class_info;
    void operator=(const PricerHomeExplicit&) {}

    protected:
    PricerHomeExplicit() {}
    PricerHomeExplicit(const PricerHomeExplicit&) {}

   public:
    virtual ~PricerHomeExplicit() {}
    static  const CORBA::TypeInfo *_desc();
    virtual const CORBA::TypeInfo *_type_info() const;
    virtual void *_safe_narrow(const CORBA::TypeInfo& ) const;
    static CORBA::Object*_factory();
    PricerHomeExplicit_ptr _this();
    static PricerHomeExplicit_ptr _duplicate(PricerHomeExplicit_ptr _obj) {
      if (_obj) _obj->_ref();
      return _obj;
    }
    static PricerHomeExplicit_ptr _nil() { return (PricerHomeExplicit_ptr)NULL; }
    static PricerHomeExplicit_ptr _narrow(CORBA::Object* _obj);
    static PricerHomeExplicit_ptr _clone(PricerHomeExplicit_ptr _obj) {
      CORBA::Object_var _obj_var(CORBA_Object::_clone(_obj));
      #if defined(_HPCC_BUG)
        return _narrow(_obj_var.operator CORBA::Object_ptr());
      #else
        return _narrow(_obj_var);
      #endif  // _HPCC_BUG
    }
    static PricerHomeExplicit_ptr _bind(const char *_object_name = (const char*)NULL,
                                        const char *_host_name = (const char*)NULL,
                                        const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                                        CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    static PricerHomeExplicit_ptr _bind(const char *_poa_name,
                                        const CORBA::OctetSequence& _id,
                                        const char *_host_name = (const char*)NULL,
                                        const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                                        CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    friend VISostream& _marshal_in(VISostream& _strm, const PricerHomeExplicit_ptr _obj);
    friend VISistream& _marshal_out(VISistream& _strm, PricerHomeExplicit_ptr& _obj);
    friend Ostream& _marshal_in(Ostream& _strm, const PricerHomeExplicit_ptr _obj);
    friend Istream& _marshal_out(Istream& _strm, PricerHomeExplicit_ptr& _obj);
  };

  class PricerHomeExplicit_ops;
  typedef PricerHomeExplicit_ops* PricerHomeExplicit_ops_ptr;

  class  PricerHomeExplicit_ops  {
   public:
    PricerHomeExplicit_ops() {}
    virtual ~PricerHomeExplicit_ops () {}

    static const VISOps_Info *_desc();
    static PricerHomeExplicit_ops_ptr _nil() { return (PricerHomeExplicit_ops_ptr)NULL; }
    static PricerHomeExplicit_ops_ptr _downcast(PortableServer::ServantBase* _servant);
  protected:
    static const VISOps_Info _ops_info;
  };

  #ifndef PetStore_PricerHomeImplicit_var_
  #define PetStore_PricerHomeImplicit_var_

  class  PricerHomeImplicit;

  typedef PricerHomeImplicit* PricerHomeImplicit_ptr;
  typedef PricerHomeImplicit_ptr PricerHomeImplicitRef;
   VISistream& _marshal_out(VISistream&, PricerHomeImplicit_ptr&);
   VISostream& _marshal_in(VISostream&, const PricerHomeImplicit_ptr);
  class  PricerHomeImplicit_out;

  class  PricerHomeImplicit_var : public CORBA::_var {
  friend class PricerHomeImplicit_out;
   private:
    PricerHomeImplicit_ptr _ptr;

   public:
    PricerHomeImplicit_var();
    PricerHomeImplicit_var(PricerHomeImplicit_ptr);
    PricerHomeImplicit_var(const PricerHomeImplicit_var &);
    virtual ~PricerHomeImplicit_var();

    static PricerHomeImplicit_ptr _duplicate(PricerHomeImplicit_ptr);
    static void _release(PricerHomeImplicit_ptr);

    PricerHomeImplicit_var& operator=(const PricerHomeImplicit_var& _var);
    PricerHomeImplicit_var& operator=(PricerHomeImplicit_ptr);

    operator PricerHomeImplicit*() const { return _ptr; }
    PricerHomeImplicit* operator->() const { return _ptr; }

    PricerHomeImplicit_ptr in() const { return _ptr; }
    PricerHomeImplicit_ptr& inout() { return _ptr; }
    PricerHomeImplicit_ptr& out();
    PricerHomeImplicit_ptr _retn();

    friend VISostream& _marshal_in(VISostream&, const PricerHomeImplicit_var&);
    friend VISistream& _marshal_out(VISistream&, PricerHomeImplicit_var&);

    friend Istream& _marshal_out(Istream&, PricerHomeImplicit_var&);
    friend Ostream& _marshal_in(Ostream&, const PricerHomeImplicit_var&);
  };
  class  PricerHomeImplicit_out {
   private:
    PricerHomeImplicit_ptr& _ptr;
    static PricerHomeImplicit* _nil() { return (PricerHomeImplicit*)NULL; }

    void operator=(const PricerHomeImplicit_out&);
    void operator=(const PricerHomeImplicit_var&);
     public:
    PricerHomeImplicit_out(const PricerHomeImplicit_out& _o) : _ptr(_o._ptr) {}
    PricerHomeImplicit_out(PricerHomeImplicit_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    PricerHomeImplicit_out(PricerHomeImplicit_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    PricerHomeImplicit_out& operator=(PricerHomeImplicit* _p) {
      _ptr = _p;
      return *this;
    }
    operator PricerHomeImplicit_ptr&() { return _ptr; }
    PricerHomeImplicit_ptr& ptr() { return _ptr; }
    PricerHomeImplicit* operator->() { return _ptr; }
  };

  #endif // PetStore_PricerHomeImplicit_var_

  // idl interface: PetStore::PricerHomeImplicit
  class  PricerHomeImplicit : public virtual ::Components::KeylessCCMHome, public virtual CORBA_Object {
   private:
    static const CORBA::TypeInfo _class_info;
    void operator=(const PricerHomeImplicit&) {}

    protected:
    PricerHomeImplicit() {}
    PricerHomeImplicit(const PricerHomeImplicit&) {}

   public:
    virtual ~PricerHomeImplicit() {}
    static  const CORBA::TypeInfo *_desc();
    virtual const CORBA::TypeInfo *_type_info() const;
    virtual void *_safe_narrow(const CORBA::TypeInfo& ) const;
    static CORBA::Object*_factory();
    PricerHomeImplicit_ptr _this();
    static PricerHomeImplicit_ptr _duplicate(PricerHomeImplicit_ptr _obj) {
      if (_obj) _obj->_ref();
      return _obj;
    }
    static PricerHomeImplicit_ptr _nil() { return (PricerHomeImplicit_ptr)NULL; }
    static PricerHomeImplicit_ptr _narrow(CORBA::Object* _obj);
    static PricerHomeImplicit_ptr _clone(PricerHomeImplicit_ptr _obj) {
      CORBA::Object_var _obj_var(CORBA_Object::_clone(_obj));
      #if defined(_HPCC_BUG)
        return _narrow(_obj_var.operator CORBA::Object_ptr());
      #else
        return _narrow(_obj_var);
      #endif  // _HPCC_BUG
    }
    static PricerHomeImplicit_ptr _bind(const char *_object_name = (const char*)NULL,
                                        const char *_host_name = (const char*)NULL,
                                        const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                                        CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    static PricerHomeImplicit_ptr _bind(const char *_poa_name,
                                        const CORBA::OctetSequence& _id,
                                        const char *_host_name = (const char*)NULL,
                                        const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                                        CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    virtual Pricer_ptr create();

    friend VISostream& _marshal_in(VISostream& _strm, const PricerHomeImplicit_ptr _obj);
    friend VISistream& _marshal_out(VISistream& _strm, PricerHomeImplicit_ptr& _obj);
    friend Ostream& _marshal_in(Ostream& _strm, const PricerHomeImplicit_ptr _obj);
    friend Istream& _marshal_out(Istream& _strm, PricerHomeImplicit_ptr& _obj);
  };

  class PricerHomeImplicit_ops;
  typedef PricerHomeImplicit_ops* PricerHomeImplicit_ops_ptr;

  class  PricerHomeImplicit_ops  {
   public:
    PricerHomeImplicit_ops() {}
    virtual ~PricerHomeImplicit_ops () {}

    virtual Pricer_ptr create() = 0;

    static const VISOps_Info *_desc();
    static PricerHomeImplicit_ops_ptr _nil() { return (PricerHomeImplicit_ops_ptr)NULL; }
    static PricerHomeImplicit_ops_ptr _downcast(PortableServer::ServantBase* _servant);
  protected:
    static const VISOps_Info _ops_info;
  };

  #ifndef PetStore_PricerHome_var_
  #define PetStore_PricerHome_var_

  class  PricerHome;

  typedef PricerHome* PricerHome_ptr;
  typedef PricerHome_ptr PricerHomeRef;
   VISistream& _marshal_out(VISistream&, PricerHome_ptr&);
   VISostream& _marshal_in(VISostream&, const PricerHome_ptr);
  class  PricerHome_out;

  class  PricerHome_var : public CORBA::_var {
  friend class PricerHome_out;
   private:
    PricerHome_ptr _ptr;

   public:
    PricerHome_var();
    PricerHome_var(PricerHome_ptr);
    PricerHome_var(const PricerHome_var &);
    virtual ~PricerHome_var();

    static PricerHome_ptr _duplicate(PricerHome_ptr);
    static void _release(PricerHome_ptr);

    PricerHome_var& operator=(const PricerHome_var& _var);
    PricerHome_var& operator=(PricerHome_ptr);

    operator PricerHome*() const { return _ptr; }
    PricerHome* operator->() const { return _ptr; }

    PricerHome_ptr in() const { return _ptr; }
    PricerHome_ptr& inout() { return _ptr; }
    PricerHome_ptr& out();
    PricerHome_ptr _retn();

    friend VISostream& _marshal_in(VISostream&, const PricerHome_var&);
    friend VISistream& _marshal_out(VISistream&, PricerHome_var&);

    friend Istream& _marshal_out(Istream&, PricerHome_var&);
    friend Ostream& _marshal_in(Ostream&, const PricerHome_var&);
  };
  class  PricerHome_out {
   private:
    PricerHome_ptr& _ptr;
    static PricerHome* _nil() { return (PricerHome*)NULL; }

    void operator=(const PricerHome_out&);
    void operator=(const PricerHome_var&);
     public:
    PricerHome_out(const PricerHome_out& _o) : _ptr(_o._ptr) {}
    PricerHome_out(PricerHome_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    PricerHome_out(PricerHome_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    PricerHome_out& operator=(PricerHome* _p) {
      _ptr = _p;
      return *this;
    }
    operator PricerHome_ptr&() { return _ptr; }
    PricerHome_ptr& ptr() { return _ptr; }
    PricerHome* operator->() { return _ptr; }
  };

  #endif // PetStore_PricerHome_var_

  // idl interface: PetStore::PricerHome
  class  PricerHome : public virtual PricerHomeExplicit, public virtual PricerHomeImplicit, public virtual CORBA_Object {
   private:
    static const CORBA::TypeInfo _class_info;
    void operator=(const PricerHome&) {}

    protected:
    PricerHome() {}
    PricerHome(const PricerHome&) {}

   public:
    virtual ~PricerHome() {}
    static  const CORBA::TypeInfo *_desc();
    virtual const CORBA::TypeInfo *_type_info() const;
    virtual void *_safe_narrow(const CORBA::TypeInfo& ) const;
    static CORBA::Object*_factory();
    PricerHome_ptr _this();
    static PricerHome_ptr _duplicate(PricerHome_ptr _obj) {
      if (_obj) _obj->_ref();
      return _obj;
    }
    static PricerHome_ptr _nil() { return (PricerHome_ptr)NULL; }
    static PricerHome_ptr _narrow(CORBA::Object* _obj);
    static PricerHome_ptr _clone(PricerHome_ptr _obj) {
      CORBA::Object_var _obj_var(CORBA_Object::_clone(_obj));
      #if defined(_HPCC_BUG)
        return _narrow(_obj_var.operator CORBA::Object_ptr());
      #else
        return _narrow(_obj_var);
      #endif  // _HPCC_BUG
    }
    static PricerHome_ptr _bind(const char *_object_name = (const char*)NULL,
                                const char *_host_name = (const char*)NULL,
                                const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                                CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    static PricerHome_ptr _bind(const char *_poa_name,
                                const CORBA::OctetSequence& _id,
                                const char *_host_name = (const char*)NULL,
                                const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                                CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    friend VISostream& _marshal_in(VISostream& _strm, const PricerHome_ptr _obj);
    friend VISistream& _marshal_out(VISistream& _strm, PricerHome_ptr& _obj);
    friend Ostream& _marshal_in(Ostream& _strm, const PricerHome_ptr _obj);
    friend Istream& _marshal_out(Istream& _strm, PricerHome_ptr& _obj);
  };

  class PricerHome_ops;
  typedef PricerHome_ops* PricerHome_ops_ptr;

  class  PricerHome_ops  {
   public:
    PricerHome_ops() {}
    virtual ~PricerHome_ops () {}

    static const VISOps_Info *_desc();
    static PricerHome_ops_ptr _nil() { return (PricerHome_ops_ptr)NULL; }
    static PricerHome_ops_ptr _downcast(PortableServer::ServantBase* _servant);
  protected:
    static const VISOps_Info _ops_info;
  };

  class  ProductList;
  typedef ProductList* ProductList_ptr;

  class  ProductList : private VISResource {
  private:
    Product_ptr *_contents;
    CORBA::ULong _count;
    CORBA::ULong _num_allocated;
    CORBA::Boolean _release_flag;

  public:
    #if defined(MSVCNEWDLL_BUG)
      void *operator new(size_t ts);
      void *operator new(size_t ts, char*, int) {return operator new(ts);}
      void operator delete(void *p);
    #endif // MSVCNEWDLL_BUG

    static Product_ptr *allocbuf(CORBA::ULong _nelems);
    static void freebuf(Product_ptr* _data);
    static void freebuf_elems(Product_ptr *_data, CORBA::ULong _nelems);

    class  Sequence_ProductList {
      friend class ProductList;
       private:
      CORBA::Boolean _release_flag;
      Product_ptr &_ptr;

      void release() {
        if (_release_flag) {
          PetStore::Product_var::_release(_ptr);
          _ptr = (PetStore::Product *)NULL;
        }
      }

   public:
      Sequence_ProductList(Product_ptr &_p, CORBA::Boolean _rel)
       : _ptr(_p), _release_flag(_rel) {}

      Sequence_ProductList(const Sequence_ProductList& _seq)
       : _ptr(_seq._ptr), _release_flag(_seq._release_flag) {}

      Sequence_ProductList& operator=(Product_ptr _p) {
        if (_ptr != _p) {
          release();
          _ptr = _p;
        }
        return *this;
      }

      Sequence_ProductList& operator=(const Sequence_ProductList& _p) {
        if (_ptr != _p._ptr) {
          if (_release_flag) {
            release();
            _ptr = ::PetStore::Product_var::_duplicate(_p._ptr);
          } else
            _ptr = _p._ptr;
        }
        return *this;
      }

      operator Product_ptr () const { return _ptr; }
      Product_ptr operator ->() const { return _ptr; }
    };

    ProductList(CORBA::ULong _max = 0);
    ProductList(CORBA::ULong max, CORBA::ULong length, Product_ptr *_data,
                CORBA::Boolean release = (CORBA::Boolean)0);
    ProductList(const ProductList&);
    virtual ~ProductList();

    ProductList& operator=(const ProductList&);
    ProductList& operator=(const ProductList* _obj) {
      assert(_obj);
      return operator=(*_obj);
    }

    CORBA::ULong maximum() const { return _num_allocated; }

    void length(CORBA::ULong _len);
    CORBA::ULong length() const { return _count; }

    void replace(CORBA::ULong _max, CORBA::ULong _len, Product_ptr *_data,
                 CORBA::Boolean _release = (CORBA::Boolean)0);

    Product_ptr *get_buffer(CORBA::Boolean orphan = (CORBA::Boolean) 0);
    const Product_ptr* get_buffer() const;

    Sequence_ProductList operator[](CORBA::ULong _index);
    const Sequence_ProductList operator[](CORBA::ULong _index) const;

    static ProductList *_duplicate(ProductList* _ptr) {
      if (_ptr) _ptr->_ref();
      return _ptr;
    }

    static void _release(ProductList *_ptr) {
      if (_ptr && (_ptr->_deref() == 0L)) 
        delete _ptr;
    }

    friend VISostream& _marshal_in(VISostream&, const ProductList&);
    friend VISistream& _marshal_out(VISistream&, ProductList&);

    friend VISostream& _marshal_in(VISostream& _strm, const ProductList* _obj);
    friend VISistream& _marshal_out(VISistream& _strm, ProductList_ptr& _obj);

    friend void _internal_pretty_print(VISostream&, const ProductList&);

    friend Ostream& _marshal_in(Ostream&, const ProductList&);
    friend Istream& _marshal_out(Istream&, ProductList&);
    friend Istream& _marshal_out(Istream&, ProductList_ptr&);
  };

  class  ProductList_out;

  class  ProductList_var {
    friend class ProductList_out;
     private:
    ProductList *_ptr;
     public:
    ProductList_var() : _ptr((ProductList*)NULL) {}
    ProductList_var(ProductList *_p) : _ptr(_p) {}
    ProductList_var(const ProductList_var& _var) :_ptr(ProductList::_duplicate(_var._ptr)){}
    virtual ~ProductList_var() { ProductList::_release(_ptr); }
    ProductList_var& operator=(ProductList* _p) {
      ProductList::_release(_ptr);
      _ptr = _p;
      return *this;
    }

    ProductList_var& operator=(const ProductList_var& _var) {
      ProductList::_release(_ptr);
      _ptr = ProductList::_duplicate(_var._ptr);
      return *this;
    }
    operator ProductList*() const { return _ptr; }

    ProductList* operator->() { return _ptr; }
    const ProductList* operator->() const { return _ptr; }

    operator ProductList&() { return *_ptr; }
    operator const ProductList&() const { return *_ptr; }

    const ProductList& in() const { return *_ptr; }
    ProductList& inout() { return *_ptr; }
    ProductList_ptr & out() {
      ProductList::_release(_ptr);
      _ptr = (ProductList *)NULL;
      return _ptr;
    }

    ProductList* _retn() {
      ProductList* _tmp_ptr;
      _tmp_ptr = _ptr;
      _ptr = (ProductList *)NULL;
      return _tmp_ptr;
    }
    friend VISostream& _marshal_in(VISostream&, const ProductList_var&);
    friend VISistream& _marshal_out(VISistream&, ProductList_var&);

    ProductList::Sequence_ProductList operator[](CORBA::ULong _index);
    const ProductList::Sequence_ProductList operator[](CORBA::ULong _index) const;

  };
  class  ProductList_out {
   private:
    ProductList_ptr& _ptr;
    static ProductList* _nil() { return (ProductList*)NULL; }

    void operator=(const ProductList_out&);
    void operator=(const ProductList_var&);
     public:
    ProductList_out(const ProductList_out& _o) : _ptr(_o._ptr) {}
    ProductList_out(ProductList_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    ProductList_out(ProductList_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    ProductList_out& operator=(ProductList* _p) {
      _ptr = _p;
      return *this;
    }
    operator ProductList_ptr&() { return _ptr; }
    ProductList_ptr& ptr() { return _ptr; }
  };
  class  IDSeq;
  typedef IDSeq* IDSeq_ptr;

  class  IDSeq : private VISResource {
  private:
    KeyID_ptr *_contents;
    CORBA::ULong _count;
    CORBA::ULong _num_allocated;
    CORBA::Boolean _release_flag;

  public:
    #if defined(MSVCNEWDLL_BUG)
      void *operator new(size_t ts);
      void *operator new(size_t ts, char*, int) {return operator new(ts);}
      void operator delete(void *p);
    #endif // MSVCNEWDLL_BUG

    static KeyID_ptr *allocbuf(CORBA::ULong _nelems);
    static void freebuf(KeyID_ptr* _data);
    static void freebuf_elems(KeyID_ptr *_data, CORBA::ULong _nelems);

    class  Sequence_IDSeq {
      friend class IDSeq;
       private:
      CORBA::Boolean _release_flag;
      KeyID_ptr &_ptr;

      void release() {
        if (_release_flag) {
          CORBA::remove_ref(_ptr);
          _ptr = (PetStore::KeyID *)NULL;
        }
      }

   public:
      Sequence_IDSeq(KeyID_ptr &_p, CORBA::Boolean _rel)
       : _ptr(_p), _release_flag(_rel) {}

      Sequence_IDSeq(const Sequence_IDSeq& _seq)
       : _ptr(_seq._ptr), _release_flag(_seq._release_flag) {}

      Sequence_IDSeq& operator=(KeyID_ptr _p) {
        if (_ptr != _p) {
          release();
          _ptr = _p;
        }
        return *this;
      }

      Sequence_IDSeq& operator=(const Sequence_IDSeq& _p) {
        if (_ptr != _p._ptr) {
          if (_release_flag) {
            release();
            ::PetStore::KeyID_var::_add_ref(_p._ptr);
            _ptr = _p._ptr;
          } else
            _ptr = _p._ptr;
        }
        return *this;
      }

      operator KeyID_ptr () const { return _ptr; }
      KeyID_ptr operator ->() const { return _ptr; }
    };

    IDSeq(CORBA::ULong _max = 0);
    IDSeq(CORBA::ULong max, CORBA::ULong length, KeyID_ptr *_data,
          CORBA::Boolean release = (CORBA::Boolean)0);
    IDSeq(const IDSeq&);
    virtual ~IDSeq();

    IDSeq& operator=(const IDSeq&);
    IDSeq& operator=(const IDSeq* _obj) {
      assert(_obj);
      return operator=(*_obj);
    }

    CORBA::ULong maximum() const { return _num_allocated; }

    void length(CORBA::ULong _len);
    CORBA::ULong length() const { return _count; }

    void replace(CORBA::ULong _max, CORBA::ULong _len, KeyID_ptr *_data,
                 CORBA::Boolean _release = (CORBA::Boolean)0);

    KeyID_ptr *get_buffer(CORBA::Boolean orphan = (CORBA::Boolean) 0);
    const KeyID_ptr* get_buffer() const;

    Sequence_IDSeq operator[](CORBA::ULong _index);
    const Sequence_IDSeq operator[](CORBA::ULong _index) const;

    static IDSeq *_duplicate(IDSeq* _ptr) {
      if (_ptr) _ptr->_ref();
      return _ptr;
    }

    static void _release(IDSeq *_ptr) {
      if (_ptr && (_ptr->_deref() == 0L)) 
        delete _ptr;
    }

    friend VISostream& _marshal_in(VISostream&, const IDSeq&);
    friend VISistream& _marshal_out(VISistream&, IDSeq&);

    friend VISostream& _marshal_in(VISostream& _strm, const IDSeq* _obj);
    friend VISistream& _marshal_out(VISistream& _strm, IDSeq_ptr& _obj);

    friend void _internal_pretty_print(VISostream&, const IDSeq&);

    friend Ostream& _marshal_in(Ostream&, const IDSeq&);
    friend Istream& _marshal_out(Istream&, IDSeq&);
    friend Istream& _marshal_out(Istream&, IDSeq_ptr&);
  };

  class  IDSeq_out;

  class  IDSeq_var {
    friend class IDSeq_out;
     private:
    IDSeq *_ptr;
     public:
    IDSeq_var() : _ptr((IDSeq*)NULL) {}
    IDSeq_var(IDSeq *_p) : _ptr(_p) {}
    IDSeq_var(const IDSeq_var& _var) :_ptr(IDSeq::_duplicate(_var._ptr)){}
    virtual ~IDSeq_var() { IDSeq::_release(_ptr); }
    IDSeq_var& operator=(IDSeq* _p) {
      IDSeq::_release(_ptr);
      _ptr = _p;
      return *this;
    }

    IDSeq_var& operator=(const IDSeq_var& _var) {
      IDSeq::_release(_ptr);
      _ptr = IDSeq::_duplicate(_var._ptr);
      return *this;
    }
    operator IDSeq*() const { return _ptr; }

    IDSeq* operator->() { return _ptr; }
    const IDSeq* operator->() const { return _ptr; }

    operator IDSeq&() { return *_ptr; }
    operator const IDSeq&() const { return *_ptr; }

    const IDSeq& in() const { return *_ptr; }
    IDSeq& inout() { return *_ptr; }
    IDSeq_ptr & out() {
      IDSeq::_release(_ptr);
      _ptr = (IDSeq *)NULL;
      return _ptr;
    }

    IDSeq* _retn() {
      IDSeq* _tmp_ptr;
      _tmp_ptr = _ptr;
      _ptr = (IDSeq *)NULL;
      return _tmp_ptr;
    }
    friend VISostream& _marshal_in(VISostream&, const IDSeq_var&);
    friend VISistream& _marshal_out(VISistream&, IDSeq_var&);

    IDSeq::Sequence_IDSeq operator[](CORBA::ULong _index);
    const IDSeq::Sequence_IDSeq operator[](CORBA::ULong _index) const;

  };
  class  IDSeq_out {
   private:
    IDSeq_ptr& _ptr;
    static IDSeq* _nil() { return (IDSeq*)NULL; }

    void operator=(const IDSeq_out&);
    void operator=(const IDSeq_var&);
     public:
    IDSeq_out(const IDSeq_out& _o) : _ptr(_o._ptr) {}
    IDSeq_out(IDSeq_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    IDSeq_out(IDSeq_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    IDSeq_out& operator=(IDSeq* _p) {
      _ptr = _p;
      return *this;
    }
    operator IDSeq_ptr&() { return _ptr; }
    IDSeq_ptr& ptr() { return _ptr; }
  };
  class  OrderList;
  typedef OrderList* OrderList_ptr;

  class  OrderList : private VISResource {
  private:
    Order_ptr *_contents;
    CORBA::ULong _count;
    CORBA::ULong _num_allocated;
    CORBA::Boolean _release_flag;

  public:
    #if defined(MSVCNEWDLL_BUG)
      void *operator new(size_t ts);
      void *operator new(size_t ts, char*, int) {return operator new(ts);}
      void operator delete(void *p);
    #endif // MSVCNEWDLL_BUG

    static Order_ptr *allocbuf(CORBA::ULong _nelems);
    static void freebuf(Order_ptr* _data);
    static void freebuf_elems(Order_ptr *_data, CORBA::ULong _nelems);

    class  Sequence_OrderList {
      friend class OrderList;
       private:
      CORBA::Boolean _release_flag;
      Order_ptr &_ptr;

      void release() {
        if (_release_flag) {
          PetStore::Order_var::_release(_ptr);
          _ptr = (PetStore::Order *)NULL;
        }
      }

   public:
      Sequence_OrderList(Order_ptr &_p, CORBA::Boolean _rel)
       : _ptr(_p), _release_flag(_rel) {}

      Sequence_OrderList(const Sequence_OrderList& _seq)
       : _ptr(_seq._ptr), _release_flag(_seq._release_flag) {}

      Sequence_OrderList& operator=(Order_ptr _p) {
        if (_ptr != _p) {
          release();
          _ptr = _p;
        }
        return *this;
      }

      Sequence_OrderList& operator=(const Sequence_OrderList& _p) {
        if (_ptr != _p._ptr) {
          if (_release_flag) {
            release();
            _ptr = ::PetStore::Order_var::_duplicate(_p._ptr);
          } else
            _ptr = _p._ptr;
        }
        return *this;
      }

      operator Order_ptr () const { return _ptr; }
      Order_ptr operator ->() const { return _ptr; }
    };

    OrderList(CORBA::ULong _max = 0);
    OrderList(CORBA::ULong max, CORBA::ULong length, Order_ptr *_data,
              CORBA::Boolean release = (CORBA::Boolean)0);
    OrderList(const OrderList&);
    virtual ~OrderList();

    OrderList& operator=(const OrderList&);
    OrderList& operator=(const OrderList* _obj) {
      assert(_obj);
      return operator=(*_obj);
    }

    CORBA::ULong maximum() const { return _num_allocated; }

    void length(CORBA::ULong _len);
    CORBA::ULong length() const { return _count; }

    void replace(CORBA::ULong _max, CORBA::ULong _len, Order_ptr *_data,
                 CORBA::Boolean _release = (CORBA::Boolean)0);

    Order_ptr *get_buffer(CORBA::Boolean orphan = (CORBA::Boolean) 0);
    const Order_ptr* get_buffer() const;

    Sequence_OrderList operator[](CORBA::ULong _index);
    const Sequence_OrderList operator[](CORBA::ULong _index) const;

    static OrderList *_duplicate(OrderList* _ptr) {
      if (_ptr) _ptr->_ref();
      return _ptr;
    }

    static void _release(OrderList *_ptr) {
      if (_ptr && (_ptr->_deref() == 0L)) 
        delete _ptr;
    }

    friend VISostream& _marshal_in(VISostream&, const OrderList&);
    friend VISistream& _marshal_out(VISistream&, OrderList&);

    friend VISostream& _marshal_in(VISostream& _strm, const OrderList* _obj);
    friend VISistream& _marshal_out(VISistream& _strm, OrderList_ptr& _obj);

    friend void _internal_pretty_print(VISostream&, const OrderList&);

    friend Ostream& _marshal_in(Ostream&, const OrderList&);
    friend Istream& _marshal_out(Istream&, OrderList&);
    friend Istream& _marshal_out(Istream&, OrderList_ptr&);
  };

  class  OrderList_out;

  class  OrderList_var {
    friend class OrderList_out;
     private:
    OrderList *_ptr;
     public:
    OrderList_var() : _ptr((OrderList*)NULL) {}
    OrderList_var(OrderList *_p) : _ptr(_p) {}
    OrderList_var(const OrderList_var& _var) :_ptr(OrderList::_duplicate(_var._ptr)){}
    virtual ~OrderList_var() { OrderList::_release(_ptr); }
    OrderList_var& operator=(OrderList* _p) {
      OrderList::_release(_ptr);
      _ptr = _p;
      return *this;
    }

    OrderList_var& operator=(const OrderList_var& _var) {
      OrderList::_release(_ptr);
      _ptr = OrderList::_duplicate(_var._ptr);
      return *this;
    }
    operator OrderList*() const { return _ptr; }

    OrderList* operator->() { return _ptr; }
    const OrderList* operator->() const { return _ptr; }

    operator OrderList&() { return *_ptr; }
    operator const OrderList&() const { return *_ptr; }

    const OrderList& in() const { return *_ptr; }
    OrderList& inout() { return *_ptr; }
    OrderList_ptr & out() {
      OrderList::_release(_ptr);
      _ptr = (OrderList *)NULL;
      return _ptr;
    }

    OrderList* _retn() {
      OrderList* _tmp_ptr;
      _tmp_ptr = _ptr;
      _ptr = (OrderList *)NULL;
      return _tmp_ptr;
    }
    friend VISostream& _marshal_in(VISostream&, const OrderList_var&);
    friend VISistream& _marshal_out(VISistream&, OrderList_var&);

    OrderList::Sequence_OrderList operator[](CORBA::ULong _index);
    const OrderList::Sequence_OrderList operator[](CORBA::ULong _index) const;

  };
  class  OrderList_out {
   private:
    OrderList_ptr& _ptr;
    static OrderList* _nil() { return (OrderList*)NULL; }

    void operator=(const OrderList_out&);
    void operator=(const OrderList_var&);
     public:
    OrderList_out(const OrderList_out& _o) : _ptr(_o._ptr) {}
    OrderList_out(OrderList_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    OrderList_out(OrderList_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    OrderList_out& operator=(OrderList* _p) {
      _ptr = _p;
      return *this;
    }
    operator OrderList_ptr&() { return _ptr; }
    OrderList_ptr& ptr() { return _ptr; }
  };
  #ifndef PetStore_CartHomeExplicit_var_
  #define PetStore_CartHomeExplicit_var_

  class  CartHomeExplicit;

  typedef CartHomeExplicit* CartHomeExplicit_ptr;
  typedef CartHomeExplicit_ptr CartHomeExplicitRef;
   VISistream& _marshal_out(VISistream&, CartHomeExplicit_ptr&);
   VISostream& _marshal_in(VISostream&, const CartHomeExplicit_ptr);
  class  CartHomeExplicit_out;

  class  CartHomeExplicit_var : public CORBA::_var {
  friend class CartHomeExplicit_out;
   private:
    CartHomeExplicit_ptr _ptr;

   public:
    CartHomeExplicit_var();
    CartHomeExplicit_var(CartHomeExplicit_ptr);
    CartHomeExplicit_var(const CartHomeExplicit_var &);
    virtual ~CartHomeExplicit_var();

    static CartHomeExplicit_ptr _duplicate(CartHomeExplicit_ptr);
    static void _release(CartHomeExplicit_ptr);

    CartHomeExplicit_var& operator=(const CartHomeExplicit_var& _var);
    CartHomeExplicit_var& operator=(CartHomeExplicit_ptr);

    operator CartHomeExplicit*() const { return _ptr; }
    CartHomeExplicit* operator->() const { return _ptr; }

    CartHomeExplicit_ptr in() const { return _ptr; }
    CartHomeExplicit_ptr& inout() { return _ptr; }
    CartHomeExplicit_ptr& out();
    CartHomeExplicit_ptr _retn();

    friend VISostream& _marshal_in(VISostream&, const CartHomeExplicit_var&);
    friend VISistream& _marshal_out(VISistream&, CartHomeExplicit_var&);

    friend Istream& _marshal_out(Istream&, CartHomeExplicit_var&);
    friend Ostream& _marshal_in(Ostream&, const CartHomeExplicit_var&);
  };
  class  CartHomeExplicit_out {
   private:
    CartHomeExplicit_ptr& _ptr;
    static CartHomeExplicit* _nil() { return (CartHomeExplicit*)NULL; }

    void operator=(const CartHomeExplicit_out&);
    void operator=(const CartHomeExplicit_var&);
     public:
    CartHomeExplicit_out(const CartHomeExplicit_out& _o) : _ptr(_o._ptr) {}
    CartHomeExplicit_out(CartHomeExplicit_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    CartHomeExplicit_out(CartHomeExplicit_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    CartHomeExplicit_out& operator=(CartHomeExplicit* _p) {
      _ptr = _p;
      return *this;
    }
    operator CartHomeExplicit_ptr&() { return _ptr; }
    CartHomeExplicit_ptr& ptr() { return _ptr; }
    CartHomeExplicit* operator->() { return _ptr; }
  };

  #endif // PetStore_CartHomeExplicit_var_

  // idl interface: PetStore::CartHomeExplicit
  class  CartHomeExplicit : public virtual ::Components::CCMHome, public virtual CORBA_Object {
   private:
    static const CORBA::TypeInfo _class_info;
    void operator=(const CartHomeExplicit&) {}

    protected:
    CartHomeExplicit() {}
    CartHomeExplicit(const CartHomeExplicit&) {}

   public:
    virtual ~CartHomeExplicit() {}
    static  const CORBA::TypeInfo *_desc();
    virtual const CORBA::TypeInfo *_type_info() const;
    virtual void *_safe_narrow(const CORBA::TypeInfo& ) const;
    static CORBA::Object*_factory();
    CartHomeExplicit_ptr _this();
    static CartHomeExplicit_ptr _duplicate(CartHomeExplicit_ptr _obj) {
      if (_obj) _obj->_ref();
      return _obj;
    }
    static CartHomeExplicit_ptr _nil() { return (CartHomeExplicit_ptr)NULL; }
    static CartHomeExplicit_ptr _narrow(CORBA::Object* _obj);
    static CartHomeExplicit_ptr _clone(CartHomeExplicit_ptr _obj) {
      CORBA::Object_var _obj_var(CORBA_Object::_clone(_obj));
      #if defined(_HPCC_BUG)
        return _narrow(_obj_var.operator CORBA::Object_ptr());
      #else
        return _narrow(_obj_var);
      #endif  // _HPCC_BUG
    }
    static CartHomeExplicit_ptr _bind(const char *_object_name = (const char*)NULL,
                                      const char *_host_name = (const char*)NULL,
                                      const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                                      CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    static CartHomeExplicit_ptr _bind(const char *_poa_name,
                                      const CORBA::OctetSequence& _id,
                                      const char *_host_name = (const char*)NULL,
                                      const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                                      CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    friend VISostream& _marshal_in(VISostream& _strm, const CartHomeExplicit_ptr _obj);
    friend VISistream& _marshal_out(VISistream& _strm, CartHomeExplicit_ptr& _obj);
    friend Ostream& _marshal_in(Ostream& _strm, const CartHomeExplicit_ptr _obj);
    friend Istream& _marshal_out(Istream& _strm, CartHomeExplicit_ptr& _obj);
  };

  class CartHomeExplicit_ops;
  typedef CartHomeExplicit_ops* CartHomeExplicit_ops_ptr;

  class  CartHomeExplicit_ops  {
   public:
    CartHomeExplicit_ops() {}
    virtual ~CartHomeExplicit_ops () {}

    static const VISOps_Info *_desc();
    static CartHomeExplicit_ops_ptr _nil() { return (CartHomeExplicit_ops_ptr)NULL; }
    static CartHomeExplicit_ops_ptr _downcast(PortableServer::ServantBase* _servant);
  protected:
    static const VISOps_Info _ops_info;
  };

  #ifndef PetStore_CartHomeImplicit_var_
  #define PetStore_CartHomeImplicit_var_

  class  CartHomeImplicit;

  typedef CartHomeImplicit* CartHomeImplicit_ptr;
  typedef CartHomeImplicit_ptr CartHomeImplicitRef;
   VISistream& _marshal_out(VISistream&, CartHomeImplicit_ptr&);
   VISostream& _marshal_in(VISostream&, const CartHomeImplicit_ptr);
  class  CartHomeImplicit_out;

  class  CartHomeImplicit_var : public CORBA::_var {
  friend class CartHomeImplicit_out;
   private:
    CartHomeImplicit_ptr _ptr;

   public:
    CartHomeImplicit_var();
    CartHomeImplicit_var(CartHomeImplicit_ptr);
    CartHomeImplicit_var(const CartHomeImplicit_var &);
    virtual ~CartHomeImplicit_var();

    static CartHomeImplicit_ptr _duplicate(CartHomeImplicit_ptr);
    static void _release(CartHomeImplicit_ptr);

    CartHomeImplicit_var& operator=(const CartHomeImplicit_var& _var);
    CartHomeImplicit_var& operator=(CartHomeImplicit_ptr);

    operator CartHomeImplicit*() const { return _ptr; }
    CartHomeImplicit* operator->() const { return _ptr; }

    CartHomeImplicit_ptr in() const { return _ptr; }
    CartHomeImplicit_ptr& inout() { return _ptr; }
    CartHomeImplicit_ptr& out();
    CartHomeImplicit_ptr _retn();

    friend VISostream& _marshal_in(VISostream&, const CartHomeImplicit_var&);
    friend VISistream& _marshal_out(VISistream&, CartHomeImplicit_var&);

    friend Istream& _marshal_out(Istream&, CartHomeImplicit_var&);
    friend Ostream& _marshal_in(Ostream&, const CartHomeImplicit_var&);
  };
  class  CartHomeImplicit_out {
   private:
    CartHomeImplicit_ptr& _ptr;
    static CartHomeImplicit* _nil() { return (CartHomeImplicit*)NULL; }

    void operator=(const CartHomeImplicit_out&);
    void operator=(const CartHomeImplicit_var&);
     public:
    CartHomeImplicit_out(const CartHomeImplicit_out& _o) : _ptr(_o._ptr) {}
    CartHomeImplicit_out(CartHomeImplicit_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    CartHomeImplicit_out(CartHomeImplicit_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    CartHomeImplicit_out& operator=(CartHomeImplicit* _p) {
      _ptr = _p;
      return *this;
    }
    operator CartHomeImplicit_ptr&() { return _ptr; }
    CartHomeImplicit_ptr& ptr() { return _ptr; }
    CartHomeImplicit* operator->() { return _ptr; }
  };

  #endif // PetStore_CartHomeImplicit_var_

  // idl interface: PetStore::CartHomeImplicit
  class  CartHomeImplicit : public virtual ::Components::KeylessCCMHome, public virtual CORBA_Object {
   private:
    static const CORBA::TypeInfo _class_info;
    void operator=(const CartHomeImplicit&) {}

    protected:
    CartHomeImplicit() {}
    CartHomeImplicit(const CartHomeImplicit&) {}

   public:
    virtual ~CartHomeImplicit() {}
    static  const CORBA::TypeInfo *_desc();
    virtual const CORBA::TypeInfo *_type_info() const;
    virtual void *_safe_narrow(const CORBA::TypeInfo& ) const;
    static CORBA::Object*_factory();
    CartHomeImplicit_ptr _this();
    static CartHomeImplicit_ptr _duplicate(CartHomeImplicit_ptr _obj) {
      if (_obj) _obj->_ref();
      return _obj;
    }
    static CartHomeImplicit_ptr _nil() { return (CartHomeImplicit_ptr)NULL; }
    static CartHomeImplicit_ptr _narrow(CORBA::Object* _obj);
    static CartHomeImplicit_ptr _clone(CartHomeImplicit_ptr _obj) {
      CORBA::Object_var _obj_var(CORBA_Object::_clone(_obj));
      #if defined(_HPCC_BUG)
        return _narrow(_obj_var.operator CORBA::Object_ptr());
      #else
        return _narrow(_obj_var);
      #endif  // _HPCC_BUG
    }
    static CartHomeImplicit_ptr _bind(const char *_object_name = (const char*)NULL,
                                      const char *_host_name = (const char*)NULL,
                                      const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                                      CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    static CartHomeImplicit_ptr _bind(const char *_poa_name,
                                      const CORBA::OctetSequence& _id,
                                      const char *_host_name = (const char*)NULL,
                                      const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                                      CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    virtual Cart_ptr create();

    friend VISostream& _marshal_in(VISostream& _strm, const CartHomeImplicit_ptr _obj);
    friend VISistream& _marshal_out(VISistream& _strm, CartHomeImplicit_ptr& _obj);
    friend Ostream& _marshal_in(Ostream& _strm, const CartHomeImplicit_ptr _obj);
    friend Istream& _marshal_out(Istream& _strm, CartHomeImplicit_ptr& _obj);
  };

  class CartHomeImplicit_ops;
  typedef CartHomeImplicit_ops* CartHomeImplicit_ops_ptr;

  class  CartHomeImplicit_ops  {
   public:
    CartHomeImplicit_ops() {}
    virtual ~CartHomeImplicit_ops () {}

    virtual Cart_ptr create() = 0;

    static const VISOps_Info *_desc();
    static CartHomeImplicit_ops_ptr _nil() { return (CartHomeImplicit_ops_ptr)NULL; }
    static CartHomeImplicit_ops_ptr _downcast(PortableServer::ServantBase* _servant);
  protected:
    static const VISOps_Info _ops_info;
  };

  #ifndef PetStore_CartHome_var_
  #define PetStore_CartHome_var_

  class  CartHome;

  typedef CartHome* CartHome_ptr;
  typedef CartHome_ptr CartHomeRef;
   VISistream& _marshal_out(VISistream&, CartHome_ptr&);
   VISostream& _marshal_in(VISostream&, const CartHome_ptr);
  class  CartHome_out;

  class  CartHome_var : public CORBA::_var {
  friend class CartHome_out;
   private:
    CartHome_ptr _ptr;

   public:
    CartHome_var();
    CartHome_var(CartHome_ptr);
    CartHome_var(const CartHome_var &);
    virtual ~CartHome_var();

    static CartHome_ptr _duplicate(CartHome_ptr);
    static void _release(CartHome_ptr);

    CartHome_var& operator=(const CartHome_var& _var);
    CartHome_var& operator=(CartHome_ptr);

    operator CartHome*() const { return _ptr; }
    CartHome* operator->() const { return _ptr; }

    CartHome_ptr in() const { return _ptr; }
    CartHome_ptr& inout() { return _ptr; }
    CartHome_ptr& out();
    CartHome_ptr _retn();

    friend VISostream& _marshal_in(VISostream&, const CartHome_var&);
    friend VISistream& _marshal_out(VISistream&, CartHome_var&);

    friend Istream& _marshal_out(Istream&, CartHome_var&);
    friend Ostream& _marshal_in(Ostream&, const CartHome_var&);
  };
  class  CartHome_out {
   private:
    CartHome_ptr& _ptr;
    static CartHome* _nil() { return (CartHome*)NULL; }

    void operator=(const CartHome_out&);
    void operator=(const CartHome_var&);
     public:
    CartHome_out(const CartHome_out& _o) : _ptr(_o._ptr) {}
    CartHome_out(CartHome_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    CartHome_out(CartHome_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    CartHome_out& operator=(CartHome* _p) {
      _ptr = _p;
      return *this;
    }
    operator CartHome_ptr&() { return _ptr; }
    CartHome_ptr& ptr() { return _ptr; }
    CartHome* operator->() { return _ptr; }
  };

  #endif // PetStore_CartHome_var_

  // idl interface: PetStore::CartHome
  class  CartHome : public virtual CartHomeExplicit, public virtual CartHomeImplicit, public virtual CORBA_Object {
   private:
    static const CORBA::TypeInfo _class_info;
    void operator=(const CartHome&) {}

    protected:
    CartHome() {}
    CartHome(const CartHome&) {}

   public:
    virtual ~CartHome() {}
    static  const CORBA::TypeInfo *_desc();
    virtual const CORBA::TypeInfo *_type_info() const;
    virtual void *_safe_narrow(const CORBA::TypeInfo& ) const;
    static CORBA::Object*_factory();
    CartHome_ptr _this();
    static CartHome_ptr _duplicate(CartHome_ptr _obj) {
      if (_obj) _obj->_ref();
      return _obj;
    }
    static CartHome_ptr _nil() { return (CartHome_ptr)NULL; }
    static CartHome_ptr _narrow(CORBA::Object* _obj);
    static CartHome_ptr _clone(CartHome_ptr _obj) {
      CORBA::Object_var _obj_var(CORBA_Object::_clone(_obj));
      #if defined(_HPCC_BUG)
        return _narrow(_obj_var.operator CORBA::Object_ptr());
      #else
        return _narrow(_obj_var);
      #endif  // _HPCC_BUG
    }
    static CartHome_ptr _bind(const char *_object_name = (const char*)NULL,
                              const char *_host_name = (const char*)NULL,
                              const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                              CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    static CartHome_ptr _bind(const char *_poa_name,
                              const CORBA::OctetSequence& _id,
                              const char *_host_name = (const char*)NULL,
                              const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                              CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    friend VISostream& _marshal_in(VISostream& _strm, const CartHome_ptr _obj);
    friend VISistream& _marshal_out(VISistream& _strm, CartHome_ptr& _obj);
    friend Ostream& _marshal_in(Ostream& _strm, const CartHome_ptr _obj);
    friend Istream& _marshal_out(Istream& _strm, CartHome_ptr& _obj);
  };

  class CartHome_ops;
  typedef CartHome_ops* CartHome_ops_ptr;

  class  CartHome_ops  {
   public:
    CartHome_ops() {}
    virtual ~CartHome_ops () {}

    static const VISOps_Info *_desc();
    static CartHome_ops_ptr _nil() { return (CartHome_ops_ptr)NULL; }
    static CartHome_ops_ptr _downcast(PortableServer::ServantBase* _servant);
  protected:
    static const VISOps_Info _ops_info;
  };

  class  ProductInfoList;
  typedef ProductInfoList* ProductInfoList_ptr;

  class  ProductInfoList : private VISResource {
  private:
    ProductInfo *_contents;
    CORBA::ULong _count;
    CORBA::ULong _num_allocated;
    CORBA::Boolean _release_flag;

  public:
    #if defined(MSVCNEWDLL_BUG)
      void *operator new(size_t ts);
      void *operator new(size_t ts, char*, int) {return operator new(ts);}
      void operator delete(void *p);
    #endif // MSVCNEWDLL_BUG

    static ProductInfo *allocbuf(CORBA::ULong _nelems);
    static void freebuf(ProductInfo* _data);

    ProductInfoList(CORBA::ULong _max = 0);
    ProductInfoList(CORBA::ULong max, CORBA::ULong length, ProductInfo *_data,
                    CORBA::Boolean release = (CORBA::Boolean)0);
    ProductInfoList(const ProductInfoList&);
    virtual ~ProductInfoList();

    ProductInfoList& operator=(const ProductInfoList&);
    ProductInfoList& operator=(const ProductInfoList* _obj) {
      assert(_obj);
      return operator=(*_obj);
    }

    CORBA::ULong maximum() const { return _num_allocated; }

    void length(CORBA::ULong _len);
    CORBA::ULong length() const { return _count; }

    void replace(CORBA::ULong _max, CORBA::ULong _len, ProductInfo *_data,
                 CORBA::Boolean _release = (CORBA::Boolean)0);

    ProductInfo *get_buffer(CORBA::Boolean orphan = (CORBA::Boolean) 0);
    const ProductInfo* get_buffer() const;

    ProductInfo& operator[](CORBA::ULong _index);
    const ProductInfo& operator[](CORBA::ULong _index) const;

    static ProductInfoList *_duplicate(ProductInfoList* _ptr) {
      if (_ptr) _ptr->_ref();
      return _ptr;
    }

    static void _release(ProductInfoList *_ptr) {
      if (_ptr && (_ptr->_deref() == 0L)) 
        delete _ptr;
    }

    friend VISostream& _marshal_in(VISostream&, const ProductInfoList&);
    friend VISistream& _marshal_out(VISistream&, ProductInfoList&);

    friend VISostream& _marshal_in(VISostream& _strm, const ProductInfoList* _obj);
    friend VISistream& _marshal_out(VISistream& _strm, ProductInfoList_ptr& _obj);

    friend void _internal_pretty_print(VISostream&, const ProductInfoList&);

    friend Ostream& _marshal_in(Ostream&, const ProductInfoList&);
    friend Istream& _marshal_out(Istream&, ProductInfoList&);
    friend Istream& _marshal_out(Istream&, ProductInfoList_ptr&);
  };

  class  ProductInfoList_out;

  class  ProductInfoList_var {
    friend class ProductInfoList_out;
     private:
    ProductInfoList *_ptr;
     public:
    ProductInfoList_var() : _ptr((ProductInfoList*)NULL) {}
    ProductInfoList_var(ProductInfoList *_p) : _ptr(_p) {}
    ProductInfoList_var(const ProductInfoList_var& _var) :_ptr(ProductInfoList::_duplicate(_var._ptr)){}
    virtual ~ProductInfoList_var() { ProductInfoList::_release(_ptr); }
    ProductInfoList_var& operator=(ProductInfoList* _p) {
      ProductInfoList::_release(_ptr);
      _ptr = _p;
      return *this;
    }

    ProductInfoList_var& operator=(const ProductInfoList_var& _var) {
      ProductInfoList::_release(_ptr);
      _ptr = ProductInfoList::_duplicate(_var._ptr);
      return *this;
    }
    operator ProductInfoList*() const { return _ptr; }

    ProductInfoList* operator->() { return _ptr; }
    const ProductInfoList* operator->() const { return _ptr; }

    operator ProductInfoList&() { return *_ptr; }
    operator const ProductInfoList&() const { return *_ptr; }

    const ProductInfoList& in() const { return *_ptr; }
    ProductInfoList& inout() { return *_ptr; }
    ProductInfoList_ptr & out() {
      ProductInfoList::_release(_ptr);
      _ptr = (ProductInfoList *)NULL;
      return _ptr;
    }

    ProductInfoList* _retn() {
      ProductInfoList* _tmp_ptr;
      _tmp_ptr = _ptr;
      _ptr = (ProductInfoList *)NULL;
      return _tmp_ptr;
    }
    friend VISostream& _marshal_in(VISostream&, const ProductInfoList_var&);
    friend VISistream& _marshal_out(VISistream&, ProductInfoList_var&);

    ProductInfo& operator[](CORBA::ULong _index);
    const ProductInfo& operator[](CORBA::ULong _index) const;

  };
  class  ProductInfoList_out {
   private:
    ProductInfoList_ptr& _ptr;
    static ProductInfoList* _nil() { return (ProductInfoList*)NULL; }

    void operator=(const ProductInfoList_out&);
    void operator=(const ProductInfoList_var&);
     public:
    ProductInfoList_out(const ProductInfoList_out& _o) : _ptr(_o._ptr) {}
    ProductInfoList_out(ProductInfoList_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    ProductInfoList_out(ProductInfoList_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    ProductInfoList_out& operator=(ProductInfoList* _p) {
      _ptr = _p;
      return *this;
    }
    operator ProductInfoList_ptr&() { return _ptr; }
    ProductInfoList_ptr& ptr() { return _ptr; }
  };
  #ifndef PetStore_SalesHomeExplicit_var_
  #define PetStore_SalesHomeExplicit_var_

  class  SalesHomeExplicit;

  typedef SalesHomeExplicit* SalesHomeExplicit_ptr;
  typedef SalesHomeExplicit_ptr SalesHomeExplicitRef;
   VISistream& _marshal_out(VISistream&, SalesHomeExplicit_ptr&);
   VISostream& _marshal_in(VISostream&, const SalesHomeExplicit_ptr);
  class  SalesHomeExplicit_out;

  class  SalesHomeExplicit_var : public CORBA::_var {
  friend class SalesHomeExplicit_out;
   private:
    SalesHomeExplicit_ptr _ptr;

   public:
    SalesHomeExplicit_var();
    SalesHomeExplicit_var(SalesHomeExplicit_ptr);
    SalesHomeExplicit_var(const SalesHomeExplicit_var &);
    virtual ~SalesHomeExplicit_var();

    static SalesHomeExplicit_ptr _duplicate(SalesHomeExplicit_ptr);
    static void _release(SalesHomeExplicit_ptr);

    SalesHomeExplicit_var& operator=(const SalesHomeExplicit_var& _var);
    SalesHomeExplicit_var& operator=(SalesHomeExplicit_ptr);

    operator SalesHomeExplicit*() const { return _ptr; }
    SalesHomeExplicit* operator->() const { return _ptr; }

    SalesHomeExplicit_ptr in() const { return _ptr; }
    SalesHomeExplicit_ptr& inout() { return _ptr; }
    SalesHomeExplicit_ptr& out();
    SalesHomeExplicit_ptr _retn();

    friend VISostream& _marshal_in(VISostream&, const SalesHomeExplicit_var&);
    friend VISistream& _marshal_out(VISistream&, SalesHomeExplicit_var&);

    friend Istream& _marshal_out(Istream&, SalesHomeExplicit_var&);
    friend Ostream& _marshal_in(Ostream&, const SalesHomeExplicit_var&);
  };
  class  SalesHomeExplicit_out {
   private:
    SalesHomeExplicit_ptr& _ptr;
    static SalesHomeExplicit* _nil() { return (SalesHomeExplicit*)NULL; }

    void operator=(const SalesHomeExplicit_out&);
    void operator=(const SalesHomeExplicit_var&);
     public:
    SalesHomeExplicit_out(const SalesHomeExplicit_out& _o) : _ptr(_o._ptr) {}
    SalesHomeExplicit_out(SalesHomeExplicit_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    SalesHomeExplicit_out(SalesHomeExplicit_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    SalesHomeExplicit_out& operator=(SalesHomeExplicit* _p) {
      _ptr = _p;
      return *this;
    }
    operator SalesHomeExplicit_ptr&() { return _ptr; }
    SalesHomeExplicit_ptr& ptr() { return _ptr; }
    SalesHomeExplicit* operator->() { return _ptr; }
  };

  #endif // PetStore_SalesHomeExplicit_var_

  // idl interface: PetStore::SalesHomeExplicit
  class  SalesHomeExplicit : public virtual ::Components::CCMHome, public virtual CORBA_Object {
   private:
    static const CORBA::TypeInfo _class_info;
    void operator=(const SalesHomeExplicit&) {}

    protected:
    SalesHomeExplicit() {}
    SalesHomeExplicit(const SalesHomeExplicit&) {}

   public:
    virtual ~SalesHomeExplicit() {}
    static  const CORBA::TypeInfo *_desc();
    virtual const CORBA::TypeInfo *_type_info() const;
    virtual void *_safe_narrow(const CORBA::TypeInfo& ) const;
    static CORBA::Object*_factory();
    SalesHomeExplicit_ptr _this();
    static SalesHomeExplicit_ptr _duplicate(SalesHomeExplicit_ptr _obj) {
      if (_obj) _obj->_ref();
      return _obj;
    }
    static SalesHomeExplicit_ptr _nil() { return (SalesHomeExplicit_ptr)NULL; }
    static SalesHomeExplicit_ptr _narrow(CORBA::Object* _obj);
    static SalesHomeExplicit_ptr _clone(SalesHomeExplicit_ptr _obj) {
      CORBA::Object_var _obj_var(CORBA_Object::_clone(_obj));
      #if defined(_HPCC_BUG)
        return _narrow(_obj_var.operator CORBA::Object_ptr());
      #else
        return _narrow(_obj_var);
      #endif  // _HPCC_BUG
    }
    static SalesHomeExplicit_ptr _bind(const char *_object_name = (const char*)NULL,
                                       const char *_host_name = (const char*)NULL,
                                       const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                                       CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    static SalesHomeExplicit_ptr _bind(const char *_poa_name,
                                       const CORBA::OctetSequence& _id,
                                       const char *_host_name = (const char*)NULL,
                                       const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                                       CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    friend VISostream& _marshal_in(VISostream& _strm, const SalesHomeExplicit_ptr _obj);
    friend VISistream& _marshal_out(VISistream& _strm, SalesHomeExplicit_ptr& _obj);
    friend Ostream& _marshal_in(Ostream& _strm, const SalesHomeExplicit_ptr _obj);
    friend Istream& _marshal_out(Istream& _strm, SalesHomeExplicit_ptr& _obj);
  };

  class SalesHomeExplicit_ops;
  typedef SalesHomeExplicit_ops* SalesHomeExplicit_ops_ptr;

  class  SalesHomeExplicit_ops  {
   public:
    SalesHomeExplicit_ops() {}
    virtual ~SalesHomeExplicit_ops () {}

    static const VISOps_Info *_desc();
    static SalesHomeExplicit_ops_ptr _nil() { return (SalesHomeExplicit_ops_ptr)NULL; }
    static SalesHomeExplicit_ops_ptr _downcast(PortableServer::ServantBase* _servant);
  protected:
    static const VISOps_Info _ops_info;
  };

  #ifndef PetStore_SalesHomeImplicit_var_
  #define PetStore_SalesHomeImplicit_var_

  class  SalesHomeImplicit;

  typedef SalesHomeImplicit* SalesHomeImplicit_ptr;
  typedef SalesHomeImplicit_ptr SalesHomeImplicitRef;
   VISistream& _marshal_out(VISistream&, SalesHomeImplicit_ptr&);
   VISostream& _marshal_in(VISostream&, const SalesHomeImplicit_ptr);
  class  SalesHomeImplicit_out;

  class  SalesHomeImplicit_var : public CORBA::_var {
  friend class SalesHomeImplicit_out;
   private:
    SalesHomeImplicit_ptr _ptr;

   public:
    SalesHomeImplicit_var();
    SalesHomeImplicit_var(SalesHomeImplicit_ptr);
    SalesHomeImplicit_var(const SalesHomeImplicit_var &);
    virtual ~SalesHomeImplicit_var();

    static SalesHomeImplicit_ptr _duplicate(SalesHomeImplicit_ptr);
    static void _release(SalesHomeImplicit_ptr);

    SalesHomeImplicit_var& operator=(const SalesHomeImplicit_var& _var);
    SalesHomeImplicit_var& operator=(SalesHomeImplicit_ptr);

    operator SalesHomeImplicit*() const { return _ptr; }
    SalesHomeImplicit* operator->() const { return _ptr; }

    SalesHomeImplicit_ptr in() const { return _ptr; }
    SalesHomeImplicit_ptr& inout() { return _ptr; }
    SalesHomeImplicit_ptr& out();
    SalesHomeImplicit_ptr _retn();

    friend VISostream& _marshal_in(VISostream&, const SalesHomeImplicit_var&);
    friend VISistream& _marshal_out(VISistream&, SalesHomeImplicit_var&);

    friend Istream& _marshal_out(Istream&, SalesHomeImplicit_var&);
    friend Ostream& _marshal_in(Ostream&, const SalesHomeImplicit_var&);
  };
  class  SalesHomeImplicit_out {
   private:
    SalesHomeImplicit_ptr& _ptr;
    static SalesHomeImplicit* _nil() { return (SalesHomeImplicit*)NULL; }

    void operator=(const SalesHomeImplicit_out&);
    void operator=(const SalesHomeImplicit_var&);
     public:
    SalesHomeImplicit_out(const SalesHomeImplicit_out& _o) : _ptr(_o._ptr) {}
    SalesHomeImplicit_out(SalesHomeImplicit_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    SalesHomeImplicit_out(SalesHomeImplicit_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    SalesHomeImplicit_out& operator=(SalesHomeImplicit* _p) {
      _ptr = _p;
      return *this;
    }
    operator SalesHomeImplicit_ptr&() { return _ptr; }
    SalesHomeImplicit_ptr& ptr() { return _ptr; }
    SalesHomeImplicit* operator->() { return _ptr; }
  };

  #endif // PetStore_SalesHomeImplicit_var_

  // idl interface: PetStore::SalesHomeImplicit
  class  SalesHomeImplicit : public virtual ::Components::KeylessCCMHome, public virtual CORBA_Object {
   private:
    static const CORBA::TypeInfo _class_info;
    void operator=(const SalesHomeImplicit&) {}

    protected:
    SalesHomeImplicit() {}
    SalesHomeImplicit(const SalesHomeImplicit&) {}

   public:
    virtual ~SalesHomeImplicit() {}
    static  const CORBA::TypeInfo *_desc();
    virtual const CORBA::TypeInfo *_type_info() const;
    virtual void *_safe_narrow(const CORBA::TypeInfo& ) const;
    static CORBA::Object*_factory();
    SalesHomeImplicit_ptr _this();
    static SalesHomeImplicit_ptr _duplicate(SalesHomeImplicit_ptr _obj) {
      if (_obj) _obj->_ref();
      return _obj;
    }
    static SalesHomeImplicit_ptr _nil() { return (SalesHomeImplicit_ptr)NULL; }
    static SalesHomeImplicit_ptr _narrow(CORBA::Object* _obj);
    static SalesHomeImplicit_ptr _clone(SalesHomeImplicit_ptr _obj) {
      CORBA::Object_var _obj_var(CORBA_Object::_clone(_obj));
      #if defined(_HPCC_BUG)
        return _narrow(_obj_var.operator CORBA::Object_ptr());
      #else
        return _narrow(_obj_var);
      #endif  // _HPCC_BUG
    }
    static SalesHomeImplicit_ptr _bind(const char *_object_name = (const char*)NULL,
                                       const char *_host_name = (const char*)NULL,
                                       const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                                       CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    static SalesHomeImplicit_ptr _bind(const char *_poa_name,
                                       const CORBA::OctetSequence& _id,
                                       const char *_host_name = (const char*)NULL,
                                       const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                                       CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    virtual Sales_ptr create();

    friend VISostream& _marshal_in(VISostream& _strm, const SalesHomeImplicit_ptr _obj);
    friend VISistream& _marshal_out(VISistream& _strm, SalesHomeImplicit_ptr& _obj);
    friend Ostream& _marshal_in(Ostream& _strm, const SalesHomeImplicit_ptr _obj);
    friend Istream& _marshal_out(Istream& _strm, SalesHomeImplicit_ptr& _obj);
  };

  class SalesHomeImplicit_ops;
  typedef SalesHomeImplicit_ops* SalesHomeImplicit_ops_ptr;

  class  SalesHomeImplicit_ops  {
   public:
    SalesHomeImplicit_ops() {}
    virtual ~SalesHomeImplicit_ops () {}

    virtual Sales_ptr create() = 0;

    static const VISOps_Info *_desc();
    static SalesHomeImplicit_ops_ptr _nil() { return (SalesHomeImplicit_ops_ptr)NULL; }
    static SalesHomeImplicit_ops_ptr _downcast(PortableServer::ServantBase* _servant);
  protected:
    static const VISOps_Info _ops_info;
  };

  #ifndef PetStore_SalesHome_var_
  #define PetStore_SalesHome_var_

  class  SalesHome;

  typedef SalesHome* SalesHome_ptr;
  typedef SalesHome_ptr SalesHomeRef;
   VISistream& _marshal_out(VISistream&, SalesHome_ptr&);
   VISostream& _marshal_in(VISostream&, const SalesHome_ptr);
  class  SalesHome_out;

  class  SalesHome_var : public CORBA::_var {
  friend class SalesHome_out;
   private:
    SalesHome_ptr _ptr;

   public:
    SalesHome_var();
    SalesHome_var(SalesHome_ptr);
    SalesHome_var(const SalesHome_var &);
    virtual ~SalesHome_var();

    static SalesHome_ptr _duplicate(SalesHome_ptr);
    static void _release(SalesHome_ptr);

    SalesHome_var& operator=(const SalesHome_var& _var);
    SalesHome_var& operator=(SalesHome_ptr);

    operator SalesHome*() const { return _ptr; }
    SalesHome* operator->() const { return _ptr; }

    SalesHome_ptr in() const { return _ptr; }
    SalesHome_ptr& inout() { return _ptr; }
    SalesHome_ptr& out();
    SalesHome_ptr _retn();

    friend VISostream& _marshal_in(VISostream&, const SalesHome_var&);
    friend VISistream& _marshal_out(VISistream&, SalesHome_var&);

    friend Istream& _marshal_out(Istream&, SalesHome_var&);
    friend Ostream& _marshal_in(Ostream&, const SalesHome_var&);
  };
  class  SalesHome_out {
   private:
    SalesHome_ptr& _ptr;
    static SalesHome* _nil() { return (SalesHome*)NULL; }

    void operator=(const SalesHome_out&);
    void operator=(const SalesHome_var&);
     public:
    SalesHome_out(const SalesHome_out& _o) : _ptr(_o._ptr) {}
    SalesHome_out(SalesHome_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    SalesHome_out(SalesHome_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    SalesHome_out& operator=(SalesHome* _p) {
      _ptr = _p;
      return *this;
    }
    operator SalesHome_ptr&() { return _ptr; }
    SalesHome_ptr& ptr() { return _ptr; }
    SalesHome* operator->() { return _ptr; }
  };

  #endif // PetStore_SalesHome_var_

  // idl interface: PetStore::SalesHome
  class  SalesHome : public virtual SalesHomeExplicit, public virtual SalesHomeImplicit, public virtual CORBA_Object {
   private:
    static const CORBA::TypeInfo _class_info;
    void operator=(const SalesHome&) {}

    protected:
    SalesHome() {}
    SalesHome(const SalesHome&) {}

   public:
    virtual ~SalesHome() {}
    static  const CORBA::TypeInfo *_desc();
    virtual const CORBA::TypeInfo *_type_info() const;
    virtual void *_safe_narrow(const CORBA::TypeInfo& ) const;
    static CORBA::Object*_factory();
    SalesHome_ptr _this();
    static SalesHome_ptr _duplicate(SalesHome_ptr _obj) {
      if (_obj) _obj->_ref();
      return _obj;
    }
    static SalesHome_ptr _nil() { return (SalesHome_ptr)NULL; }
    static SalesHome_ptr _narrow(CORBA::Object* _obj);
    static SalesHome_ptr _clone(SalesHome_ptr _obj) {
      CORBA::Object_var _obj_var(CORBA_Object::_clone(_obj));
      #if defined(_HPCC_BUG)
        return _narrow(_obj_var.operator CORBA::Object_ptr());
      #else
        return _narrow(_obj_var);
      #endif  // _HPCC_BUG
    }
    static SalesHome_ptr _bind(const char *_object_name = (const char*)NULL,
                               const char *_host_name = (const char*)NULL,
                               const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                               CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    static SalesHome_ptr _bind(const char *_poa_name,
                               const CORBA::OctetSequence& _id,
                               const char *_host_name = (const char*)NULL,
                               const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                               CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    friend VISostream& _marshal_in(VISostream& _strm, const SalesHome_ptr _obj);
    friend VISistream& _marshal_out(VISistream& _strm, SalesHome_ptr& _obj);
    friend Ostream& _marshal_in(Ostream& _strm, const SalesHome_ptr _obj);
    friend Istream& _marshal_out(Istream& _strm, SalesHome_ptr& _obj);
  };

  class SalesHome_ops;
  typedef SalesHome_ops* SalesHome_ops_ptr;

  class  SalesHome_ops  {
   public:
    SalesHome_ops() {}
    virtual ~SalesHome_ops () {}

    static const VISOps_Info *_desc();
    static SalesHome_ops_ptr _nil() { return (SalesHome_ops_ptr)NULL; }
    static SalesHome_ops_ptr _downcast(PortableServer::ServantBase* _servant);
  protected:
    static const VISOps_Info _ops_info;
  };

  #ifndef PetStore_ManagerHomeExplicit_var_
  #define PetStore_ManagerHomeExplicit_var_

  class  ManagerHomeExplicit;

  typedef ManagerHomeExplicit* ManagerHomeExplicit_ptr;
  typedef ManagerHomeExplicit_ptr ManagerHomeExplicitRef;
   VISistream& _marshal_out(VISistream&, ManagerHomeExplicit_ptr&);
   VISostream& _marshal_in(VISostream&, const ManagerHomeExplicit_ptr);
  class  ManagerHomeExplicit_out;

  class  ManagerHomeExplicit_var : public CORBA::_var {
  friend class ManagerHomeExplicit_out;
   private:
    ManagerHomeExplicit_ptr _ptr;

   public:
    ManagerHomeExplicit_var();
    ManagerHomeExplicit_var(ManagerHomeExplicit_ptr);
    ManagerHomeExplicit_var(const ManagerHomeExplicit_var &);
    virtual ~ManagerHomeExplicit_var();

    static ManagerHomeExplicit_ptr _duplicate(ManagerHomeExplicit_ptr);
    static void _release(ManagerHomeExplicit_ptr);

    ManagerHomeExplicit_var& operator=(const ManagerHomeExplicit_var& _var);
    ManagerHomeExplicit_var& operator=(ManagerHomeExplicit_ptr);

    operator ManagerHomeExplicit*() const { return _ptr; }
    ManagerHomeExplicit* operator->() const { return _ptr; }

    ManagerHomeExplicit_ptr in() const { return _ptr; }
    ManagerHomeExplicit_ptr& inout() { return _ptr; }
    ManagerHomeExplicit_ptr& out();
    ManagerHomeExplicit_ptr _retn();

    friend VISostream& _marshal_in(VISostream&, const ManagerHomeExplicit_var&);
    friend VISistream& _marshal_out(VISistream&, ManagerHomeExplicit_var&);

    friend Istream& _marshal_out(Istream&, ManagerHomeExplicit_var&);
    friend Ostream& _marshal_in(Ostream&, const ManagerHomeExplicit_var&);
  };
  class  ManagerHomeExplicit_out {
   private:
    ManagerHomeExplicit_ptr& _ptr;
    static ManagerHomeExplicit* _nil() { return (ManagerHomeExplicit*)NULL; }

    void operator=(const ManagerHomeExplicit_out&);
    void operator=(const ManagerHomeExplicit_var&);
     public:
    ManagerHomeExplicit_out(const ManagerHomeExplicit_out& _o) : _ptr(_o._ptr) {}
    ManagerHomeExplicit_out(ManagerHomeExplicit_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    ManagerHomeExplicit_out(ManagerHomeExplicit_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    ManagerHomeExplicit_out& operator=(ManagerHomeExplicit* _p) {
      _ptr = _p;
      return *this;
    }
    operator ManagerHomeExplicit_ptr&() { return _ptr; }
    ManagerHomeExplicit_ptr& ptr() { return _ptr; }
    ManagerHomeExplicit* operator->() { return _ptr; }
  };

  #endif // PetStore_ManagerHomeExplicit_var_

  // idl interface: PetStore::ManagerHomeExplicit
  class  ManagerHomeExplicit : public virtual ::Components::CCMHome, public virtual CORBA_Object {
   private:
    static const CORBA::TypeInfo _class_info;
    void operator=(const ManagerHomeExplicit&) {}

    protected:
    ManagerHomeExplicit() {}
    ManagerHomeExplicit(const ManagerHomeExplicit&) {}

   public:
    virtual ~ManagerHomeExplicit() {}
    static  const CORBA::TypeInfo *_desc();
    virtual const CORBA::TypeInfo *_type_info() const;
    virtual void *_safe_narrow(const CORBA::TypeInfo& ) const;
    static CORBA::Object*_factory();
    ManagerHomeExplicit_ptr _this();
    static ManagerHomeExplicit_ptr _duplicate(ManagerHomeExplicit_ptr _obj) {
      if (_obj) _obj->_ref();
      return _obj;
    }
    static ManagerHomeExplicit_ptr _nil() { return (ManagerHomeExplicit_ptr)NULL; }
    static ManagerHomeExplicit_ptr _narrow(CORBA::Object* _obj);
    static ManagerHomeExplicit_ptr _clone(ManagerHomeExplicit_ptr _obj) {
      CORBA::Object_var _obj_var(CORBA_Object::_clone(_obj));
      #if defined(_HPCC_BUG)
        return _narrow(_obj_var.operator CORBA::Object_ptr());
      #else
        return _narrow(_obj_var);
      #endif  // _HPCC_BUG
    }
    static ManagerHomeExplicit_ptr _bind(const char *_object_name = (const char*)NULL,
                                         const char *_host_name = (const char*)NULL,
                                         const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                                         CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    static ManagerHomeExplicit_ptr _bind(const char *_poa_name,
                                         const CORBA::OctetSequence& _id,
                                         const char *_host_name = (const char*)NULL,
                                         const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                                         CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    friend VISostream& _marshal_in(VISostream& _strm, const ManagerHomeExplicit_ptr _obj);
    friend VISistream& _marshal_out(VISistream& _strm, ManagerHomeExplicit_ptr& _obj);
    friend Ostream& _marshal_in(Ostream& _strm, const ManagerHomeExplicit_ptr _obj);
    friend Istream& _marshal_out(Istream& _strm, ManagerHomeExplicit_ptr& _obj);
  };

  class ManagerHomeExplicit_ops;
  typedef ManagerHomeExplicit_ops* ManagerHomeExplicit_ops_ptr;

  class  ManagerHomeExplicit_ops  {
   public:
    ManagerHomeExplicit_ops() {}
    virtual ~ManagerHomeExplicit_ops () {}

    static const VISOps_Info *_desc();
    static ManagerHomeExplicit_ops_ptr _nil() { return (ManagerHomeExplicit_ops_ptr)NULL; }
    static ManagerHomeExplicit_ops_ptr _downcast(PortableServer::ServantBase* _servant);
  protected:
    static const VISOps_Info _ops_info;
  };

  #ifndef PetStore_ManagerHomeImplicit_var_
  #define PetStore_ManagerHomeImplicit_var_

  class  ManagerHomeImplicit;

  typedef ManagerHomeImplicit* ManagerHomeImplicit_ptr;
  typedef ManagerHomeImplicit_ptr ManagerHomeImplicitRef;
   VISistream& _marshal_out(VISistream&, ManagerHomeImplicit_ptr&);
   VISostream& _marshal_in(VISostream&, const ManagerHomeImplicit_ptr);
  class  ManagerHomeImplicit_out;

  class  ManagerHomeImplicit_var : public CORBA::_var {
  friend class ManagerHomeImplicit_out;
   private:
    ManagerHomeImplicit_ptr _ptr;

   public:
    ManagerHomeImplicit_var();
    ManagerHomeImplicit_var(ManagerHomeImplicit_ptr);
    ManagerHomeImplicit_var(const ManagerHomeImplicit_var &);
    virtual ~ManagerHomeImplicit_var();

    static ManagerHomeImplicit_ptr _duplicate(ManagerHomeImplicit_ptr);
    static void _release(ManagerHomeImplicit_ptr);

    ManagerHomeImplicit_var& operator=(const ManagerHomeImplicit_var& _var);
    ManagerHomeImplicit_var& operator=(ManagerHomeImplicit_ptr);

    operator ManagerHomeImplicit*() const { return _ptr; }
    ManagerHomeImplicit* operator->() const { return _ptr; }

    ManagerHomeImplicit_ptr in() const { return _ptr; }
    ManagerHomeImplicit_ptr& inout() { return _ptr; }
    ManagerHomeImplicit_ptr& out();
    ManagerHomeImplicit_ptr _retn();

    friend VISostream& _marshal_in(VISostream&, const ManagerHomeImplicit_var&);
    friend VISistream& _marshal_out(VISistream&, ManagerHomeImplicit_var&);

    friend Istream& _marshal_out(Istream&, ManagerHomeImplicit_var&);
    friend Ostream& _marshal_in(Ostream&, const ManagerHomeImplicit_var&);
  };
  class  ManagerHomeImplicit_out {
   private:
    ManagerHomeImplicit_ptr& _ptr;
    static ManagerHomeImplicit* _nil() { return (ManagerHomeImplicit*)NULL; }

    void operator=(const ManagerHomeImplicit_out&);
    void operator=(const ManagerHomeImplicit_var&);
     public:
    ManagerHomeImplicit_out(const ManagerHomeImplicit_out& _o) : _ptr(_o._ptr) {}
    ManagerHomeImplicit_out(ManagerHomeImplicit_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    ManagerHomeImplicit_out(ManagerHomeImplicit_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    ManagerHomeImplicit_out& operator=(ManagerHomeImplicit* _p) {
      _ptr = _p;
      return *this;
    }
    operator ManagerHomeImplicit_ptr&() { return _ptr; }
    ManagerHomeImplicit_ptr& ptr() { return _ptr; }
    ManagerHomeImplicit* operator->() { return _ptr; }
  };

  #endif // PetStore_ManagerHomeImplicit_var_

  // idl interface: PetStore::ManagerHomeImplicit
  class  ManagerHomeImplicit : public virtual ::Components::KeylessCCMHome, public virtual CORBA_Object {
   private:
    static const CORBA::TypeInfo _class_info;
    void operator=(const ManagerHomeImplicit&) {}

    protected:
    ManagerHomeImplicit() {}
    ManagerHomeImplicit(const ManagerHomeImplicit&) {}

   public:
    virtual ~ManagerHomeImplicit() {}
    static  const CORBA::TypeInfo *_desc();
    virtual const CORBA::TypeInfo *_type_info() const;
    virtual void *_safe_narrow(const CORBA::TypeInfo& ) const;
    static CORBA::Object*_factory();
    ManagerHomeImplicit_ptr _this();
    static ManagerHomeImplicit_ptr _duplicate(ManagerHomeImplicit_ptr _obj) {
      if (_obj) _obj->_ref();
      return _obj;
    }
    static ManagerHomeImplicit_ptr _nil() { return (ManagerHomeImplicit_ptr)NULL; }
    static ManagerHomeImplicit_ptr _narrow(CORBA::Object* _obj);
    static ManagerHomeImplicit_ptr _clone(ManagerHomeImplicit_ptr _obj) {
      CORBA::Object_var _obj_var(CORBA_Object::_clone(_obj));
      #if defined(_HPCC_BUG)
        return _narrow(_obj_var.operator CORBA::Object_ptr());
      #else
        return _narrow(_obj_var);
      #endif  // _HPCC_BUG
    }
    static ManagerHomeImplicit_ptr _bind(const char *_object_name = (const char*)NULL,
                                         const char *_host_name = (const char*)NULL,
                                         const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                                         CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    static ManagerHomeImplicit_ptr _bind(const char *_poa_name,
                                         const CORBA::OctetSequence& _id,
                                         const char *_host_name = (const char*)NULL,
                                         const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                                         CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    virtual Manager_ptr create();

    friend VISostream& _marshal_in(VISostream& _strm, const ManagerHomeImplicit_ptr _obj);
    friend VISistream& _marshal_out(VISistream& _strm, ManagerHomeImplicit_ptr& _obj);
    friend Ostream& _marshal_in(Ostream& _strm, const ManagerHomeImplicit_ptr _obj);
    friend Istream& _marshal_out(Istream& _strm, ManagerHomeImplicit_ptr& _obj);
  };

  class ManagerHomeImplicit_ops;
  typedef ManagerHomeImplicit_ops* ManagerHomeImplicit_ops_ptr;

  class  ManagerHomeImplicit_ops  {
   public:
    ManagerHomeImplicit_ops() {}
    virtual ~ManagerHomeImplicit_ops () {}

    virtual Manager_ptr create() = 0;

    static const VISOps_Info *_desc();
    static ManagerHomeImplicit_ops_ptr _nil() { return (ManagerHomeImplicit_ops_ptr)NULL; }
    static ManagerHomeImplicit_ops_ptr _downcast(PortableServer::ServantBase* _servant);
  protected:
    static const VISOps_Info _ops_info;
  };

  #ifndef PetStore_ManagerHome_var_
  #define PetStore_ManagerHome_var_

  class  ManagerHome;

  typedef ManagerHome* ManagerHome_ptr;
  typedef ManagerHome_ptr ManagerHomeRef;
   VISistream& _marshal_out(VISistream&, ManagerHome_ptr&);
   VISostream& _marshal_in(VISostream&, const ManagerHome_ptr);
  class  ManagerHome_out;

  class  ManagerHome_var : public CORBA::_var {
  friend class ManagerHome_out;
   private:
    ManagerHome_ptr _ptr;

   public:
    ManagerHome_var();
    ManagerHome_var(ManagerHome_ptr);
    ManagerHome_var(const ManagerHome_var &);
    virtual ~ManagerHome_var();

    static ManagerHome_ptr _duplicate(ManagerHome_ptr);
    static void _release(ManagerHome_ptr);

    ManagerHome_var& operator=(const ManagerHome_var& _var);
    ManagerHome_var& operator=(ManagerHome_ptr);

    operator ManagerHome*() const { return _ptr; }
    ManagerHome* operator->() const { return _ptr; }

    ManagerHome_ptr in() const { return _ptr; }
    ManagerHome_ptr& inout() { return _ptr; }
    ManagerHome_ptr& out();
    ManagerHome_ptr _retn();

    friend VISostream& _marshal_in(VISostream&, const ManagerHome_var&);
    friend VISistream& _marshal_out(VISistream&, ManagerHome_var&);

    friend Istream& _marshal_out(Istream&, ManagerHome_var&);
    friend Ostream& _marshal_in(Ostream&, const ManagerHome_var&);
  };
  class  ManagerHome_out {
   private:
    ManagerHome_ptr& _ptr;
    static ManagerHome* _nil() { return (ManagerHome*)NULL; }

    void operator=(const ManagerHome_out&);
    void operator=(const ManagerHome_var&);
     public:
    ManagerHome_out(const ManagerHome_out& _o) : _ptr(_o._ptr) {}
    ManagerHome_out(ManagerHome_ptr& _p) : _ptr(_p) {
      _ptr = _nil();
    }
    ManagerHome_out(ManagerHome_var& _v) : _ptr(_v._ptr) {
      _v = _nil();
    }
    ManagerHome_out& operator=(ManagerHome* _p) {
      _ptr = _p;
      return *this;
    }
    operator ManagerHome_ptr&() { return _ptr; }
    ManagerHome_ptr& ptr() { return _ptr; }
    ManagerHome* operator->() { return _ptr; }
  };

  #endif // PetStore_ManagerHome_var_

  // idl interface: PetStore::ManagerHome
  class  ManagerHome : public virtual ManagerHomeExplicit, public virtual ManagerHomeImplicit, public virtual CORBA_Object {
   private:
    static const CORBA::TypeInfo _class_info;
    void operator=(const ManagerHome&) {}

    protected:
    ManagerHome() {}
    ManagerHome(const ManagerHome&) {}

   public:
    virtual ~ManagerHome() {}
    static  const CORBA::TypeInfo *_desc();
    virtual const CORBA::TypeInfo *_type_info() const;
    virtual void *_safe_narrow(const CORBA::TypeInfo& ) const;
    static CORBA::Object*_factory();
    ManagerHome_ptr _this();
    static ManagerHome_ptr _duplicate(ManagerHome_ptr _obj) {
      if (_obj) _obj->_ref();
      return _obj;
    }
    static ManagerHome_ptr _nil() { return (ManagerHome_ptr)NULL; }
    static ManagerHome_ptr _narrow(CORBA::Object* _obj);
    static ManagerHome_ptr _clone(ManagerHome_ptr _obj) {
      CORBA::Object_var _obj_var(CORBA_Object::_clone(_obj));
      #if defined(_HPCC_BUG)
        return _narrow(_obj_var.operator CORBA::Object_ptr());
      #else
        return _narrow(_obj_var);
      #endif  // _HPCC_BUG
    }
    static ManagerHome_ptr _bind(const char *_object_name = (const char*)NULL,
                                 const char *_host_name = (const char*)NULL,
                                 const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                                 CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    static ManagerHome_ptr _bind(const char *_poa_name,
                                 const CORBA::OctetSequence& _id,
                                 const char *_host_name = (const char*)NULL,
                                 const CORBA::BindOptions* _opt = (CORBA::BindOptions*)NULL,
                                 CORBA::ORB_ptr _orb = (CORBA::ORB_ptr)NULL);

    friend VISostream& _marshal_in(VISostream& _strm, const ManagerHome_ptr _obj);
    friend VISistream& _marshal_out(VISistream& _strm, ManagerHome_ptr& _obj);
    friend Ostream& _marshal_in(Ostream& _strm, const ManagerHome_ptr _obj);
    friend Istream& _marshal_out(Istream& _strm, ManagerHome_ptr& _obj);
  };

  class ManagerHome_ops;
  typedef ManagerHome_ops* ManagerHome_ops_ptr;

  class  ManagerHome_ops  {
   public:
    ManagerHome_ops() {}
    virtual ~ManagerHome_ops () {}

    static const VISOps_Info *_desc();
    static ManagerHome_ops_ptr _nil() { return (ManagerHome_ops_ptr)NULL; }
    static ManagerHome_ops_ptr _downcast(PortableServer::ServantBase* _servant);
  protected:
    static const VISOps_Info _ops_info;
  };

}
inline Istream& operator>>(Istream& s, PetStore::PricerHomeExplicit_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline void _pretty_print(VISostream& s, const PetStore::ProductIndexInfoList& v) {
  PetStore::_internal_pretty_print(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::Calculate_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::OrderInfoList_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::ProductHomeExplicit_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::ManagerHomeExplicit_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::ItemPriceList_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::OrderInfo_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::KeyID_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::OrderItem_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::ProductInfoList* v) {
  return PetStore::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::OrderHomeExplicit_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::SalesRecordInfo_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::ProductList& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::IDSeq_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline void _pretty_print(VISostream& s, const PetStore::OrderRecord& v) {
  PetStore::_internal_pretty_print(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::SalesReportConsumer_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::ProductIndexInfoList& v) {
  return PetStore::_marshal_out(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::SalesHomeExplicit_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline void _pretty_print(VISostream& s, const PetStore::KeyID& v) {
  PetStore::_internal_pretty_print(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::ProductInfo_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::OrderItem_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::ProductCategory& v) {
  return PetStore::_marshal_out(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::ItemPriceList& v) {
  return PetStore::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::OrderIndexInfoList* v) {
  return PetStore::_marshal_in(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::Manage_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::SalesRecordInfo*& v) {
  return PetStore::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::ProductInfo& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::OrderIndexInfo_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::OrderItemHomeExplicit_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::OrderItemInfo_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::PricerHome_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::KeyID_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::OrderIndexInfoList_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::ItemPriceList& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::OrderItemInfo& v) {
  return PetStore::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::Order_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::ProductCategory v) {
  return PetStore::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::SalesRecord_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::OrderHome_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::ShoppingCart_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::ProductHome_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::PricerHome_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::ProductInfoList& v) {
  return PetStore::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::AdvancedOp_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::ProductHomeExplicit_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::ManagerHomeExplicit_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::OrderRecord& v) {
  return PetStore::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::OrderIndexInfoList_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::Sales_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::CartHomeImplicit_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::SalesReportConsumer_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::SalesHomeExplicit_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::SalesRecordInfoList& v) {
  return PetStore::_marshal_in(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::OrderInfoList& v) {
  return PetStore::_marshal_in(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::OrderOp_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::Pricer_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::SalesReportConsumer_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::OrderItemHomeImplicit_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::ShoppingCart_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::ProductHome_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::OrderIndexInfo& v) {
  return PetStore::_marshal_out(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::ProductHomeExplicit_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::SalesReport_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::ManagerHome_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::OrderList& v) {
  return PetStore::_marshal_in(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::SalesRecordInfo& v) {
  return PetStore::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::ProductIndexInfoList& v) {
  return PetStore::_marshal_in(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::OrderItemHome_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline void _pretty_print(VISostream& s, PetStore::ProductCategory v) {
  PetStore::_internal_pretty_print(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::ManagerHomeImplicit_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::OrderInfoList& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::ProductHomeImplicit_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::OrderItemHomeExplicit_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::BusinessOp_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::OrderInfo& v) {
  return PetStore::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::OrderItemHome_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::CartHome_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::Product_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::CartHomeExplicit_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::OrderItemHomeExplicit_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::OrderHome_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::SalesHome_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::PricerHomeImplicit_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::CartHomeExplicit_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::Calculate_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::ProductInfo* v) {
  return PetStore::_marshal_in(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::Order_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::ProductInfoList_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::Cart_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::OrderRecord& v) {
  return PetStore::_marshal_out(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::PricerHomeImplicit_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::OrderItemHome_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::OrderHomeExplicit_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::CartHomeImplicit_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::SalesRecordInfoList& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::OrderItemInfoList& v) {
  return PetStore::_marshal_in(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::CartHomeExplicit_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::KeyID_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::OrderItem_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::ProductIndexInfo*& v) {
  return PetStore::_marshal_out(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::BusinessOp_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::ProductIndexInfo& v) {
  return PetStore::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::ShoppingCart_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::OrderOp_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::OrderHomeExplicit_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::OrderHomeImplicit_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::ProductCategory& v) {
  return PetStore::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::OrderOp_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::OrderItemHomeExplicit_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::PricerHome_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::ItemPriceList& v) {
  return PetStore::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::Pricer_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline void _pretty_print(VISostream& s, const PetStore::ProductInfo& v) {
  PetStore::_internal_pretty_print(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::SalesHomeExplicit_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::OrderHome_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::SalesHome_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::AdvancedOp_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::ProductHomeImplicit_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::ProductInfo*& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::Calculate_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::ProductInfoList_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::ManagerHome_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::OrderItem_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::SalesRecordInfo& v) {
  return PetStore::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::OrderIndexInfo_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::ProductHomeExplicit_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::ManagerHomeExplicit_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::OrderItemInfoList_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::CartHome_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::CartHomeImplicit_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::OrderItemHome_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::OrderIndexInfo* v) {
  return PetStore::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::SalesReportConsumer_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::SalesHomeImplicit_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::ShoppingCart_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline void _pretty_print(VISostream& s, const PetStore::ProductInfoList& v) {
  PetStore::_internal_pretty_print(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::OrderItemHome_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::OrderItemHomeImplicit_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::OrderInfo* v) {
  return PetStore::_marshal_in(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::ProductHome_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::ProductInfo& v) {
  return PetStore::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::Cart_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::SalesReport_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::OrderItemInfo* v) {
  return PetStore::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::SalesRecordInfoList_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::BusinessOp_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::SalesRecordInfo*& v) {
  return PetStore::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::Manager_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::OrderItemHome_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::CartHomeImplicit_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::SalesHome_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::OrderRecord*& v) {
  return PetStore::_marshal_out(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::SalesReportConsumer_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::SalesRecord& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::ProductInfoList& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::PricerHomeImplicit_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::OrderIndexInfo& v) {
  return PetStore::_marshal_out(s, v);
}
inline void _pretty_print(VISostream& s, const PetStore::OrderList& v) {
  PetStore::_internal_pretty_print(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::Pricer_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::CartHome_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::SalesRecord& v) {
  return PetStore::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::Manage_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::Cart_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::Product_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::OrderItemHomeImplicit_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::ProductIndexInfo& v) {
  return PetStore::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::ManagerHomeExplicit_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::SalesHomeExplicit_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline void _pretty_print(VISostream& s, const PetStore::OrderInfo& v) {
  PetStore::_internal_pretty_print(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::OrderItem_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::KeyID_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::OrderInfoList& v) {
  return PetStore::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::OrderOp_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::CartHomeExplicit_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::ManagerHome_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::OrderHomeImplicit_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::PricerHomeImplicit_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::SalesRecord_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::Manager_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::ProductHomeImplicit_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::Pricer_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::ManagerHomeImplicit_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::OrderItemInfoList& v) {
  return PetStore::_marshal_in(s, v);
}
inline void _pretty_print(VISostream& s, const PetStore::ProductIndexInfo& v) {
  PetStore::_internal_pretty_print(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::OrderHome_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::OrderOp_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::KeyID_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::ProductIndexInfoList_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::Product_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::CartHome_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::Product_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::OrderItemInfoList_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::ProductHome_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::SalesReport_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::ProductIndexInfoList_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::Manage_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::OrderHome_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::SalesHomeImplicit_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::ProductList& v) {
  return PetStore::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::PricerHome_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::SalesRecord*& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::PricerHomeExplicit_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::OrderItemHome_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::ProductInfoList_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::PricerHomeImplicit_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::KeyID_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::Pricer_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::Manager_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::ItemPriceList* v) {
  return PetStore::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::SalesRecordInfo& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::OrderItemHomeExplicit_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::BusinessOp_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::OrderInfoList_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::CartHomeExplicit_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::ItemPriceList_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::IDSeq& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::OrderList& v) {
  return PetStore::_marshal_out(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::OrderHomeImplicit_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::AdvancedOp_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::Manager_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::BusinessOp_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::SalesHome_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::SalesReportConsumer_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::IDSeq& v) {
  return PetStore::_marshal_in(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::ManagerHomeExplicit_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::OrderHomeExplicit_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::Cart_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::Pricer_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::OrderInfo& v) {
  return PetStore::_marshal_out(s, v);
}
inline void _pretty_print(VISostream& s, const PetStore::KeyID* v) {
  PetStore::_internal_pretty_print(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::ProductInfoList& v) {
  return PetStore::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::AdvancedOp_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::Product_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::OrderItemHomeImplicit_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::SalesReportConsumer_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::OrderItem_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::OrderItemInfoList& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::ShoppingCart_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::Pricer_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::OrderIndexInfoList_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline void _pretty_print(VISostream& s, const PetStore::SalesReport& v) {
  PetStore::_internal_pretty_print(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::CartHomeExplicit_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::SalesReport_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::IDSeq& v) {
  return PetStore::_marshal_out(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::SalesHome_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::PricerHomeExplicit_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::ProductList_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::KeyID_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::SalesHomeExplicit_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::Cart_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::ManagerHome_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::OrderList& v) {
  return PetStore::_marshal_in(s, v);
}
inline void _pretty_print(VISostream& s, const PetStore::OrderItemInfoList& v) {
  PetStore::_internal_pretty_print(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::ProductIndexInfoList& v) {
  return PetStore::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::KeyID_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::SalesRecordInfoList* v) {
  return PetStore::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::ProductIndexInfo& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::OrderRecord& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::OrderInfo& v) {
  return PetStore::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::SalesRecordInfo_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::OrderRecord& v) {
  return PetStore::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::ProductIndexInfoList* v) {
  return PetStore::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::OrderList_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::OrderRecord*& v) {
  return PetStore::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::PricerHome_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::Order_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::ProductIndexInfo_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::ProductHomeExplicit_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::PricerHomeExplicit_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::ProductInfoList_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::PricerHomeImplicit_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::ManagerHome_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::SalesHomeImplicit_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::OrderInfoList& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::ManagerHomeImplicit_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::CartHome_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::OrderItemHomeExplicit_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::OrderItemHomeImplicit_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::SalesReport_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::SalesRecordInfoList& v) {
  return PetStore::_marshal_out(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::OrderHomeImplicit_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::Sales_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::AdvancedOp_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::BusinessOp_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::OrderHomeExplicit_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::ProductList_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::OrderInfoList_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::ProductInfoList& v) {
  return PetStore::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::ItemPriceList_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::OrderItem_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline void _pretty_print(VISostream& s, const PetStore::SalesRecordInfo& v) {
  PetStore::_internal_pretty_print(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::OrderItemHomeExplicit_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::AdvancedOp_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::SalesHomeExplicit_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::ProductHomeImplicit_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::ManagerHomeImplicit_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline void _pretty_print(VISostream& s, const PetStore::OrderIndexInfo& v) {
  PetStore::_internal_pretty_print(s, v);
}
inline void _pretty_print(VISostream& s, const PetStore::OrderItemInfo& v) {
  PetStore::_internal_pretty_print(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::OrderIndexInfoList& v) {
  return PetStore::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::OrderHomeExplicit_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::OrderItemInfoList& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::ProductIndexInfo*& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::ProductIndexInfoList_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::OrderList& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::PricerHome_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::ProductHomeExplicit_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::OrderList_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::OrderHomeImplicit_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::BusinessOp_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::OrderItemInfo_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::CartHomeExplicit_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline void _pretty_print(VISostream& s, const PetStore::SalesRecordInfoList& v) {
  PetStore::_internal_pretty_print(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::OrderInfo& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::ShoppingCart_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::OrderIndexInfoList_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::SalesRecordInfoList& v) {
  return PetStore::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::IDSeq_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::Sales_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::SalesHomeExplicit_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::SalesRecord*& v) {
  return PetStore::_marshal_out(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::SalesReport_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::Cart_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::SalesRecordInfo* v) {
  return PetStore::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::SalesRecordInfoList_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::PricerHomeExplicit_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::OrderItemInfoList_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::OrderOp_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::IDSeq& v) {
  return PetStore::_marshal_in(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::PricerHome_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::SalesHomeImplicit_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::Calculate_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::OrderIndexInfo*& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::PricerHomeExplicit_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::Pricer_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::OrderRecord_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::ProductList& v) {
  return PetStore::_marshal_out(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::OrderHomeExplicit_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::Calculate_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::Order_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::ProductInfo& v) {
  return PetStore::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::ManagerHome_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::Cart_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::ProductHome_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::SalesReport_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::ProductInfo& v) {
  return PetStore::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::Order_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::CartHomeImplicit_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::ProductList_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::AdvancedOp_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::SalesRecordInfoList_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::SalesRecord* v) {
  return PetStore::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::PricerHomeImplicit_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::OrderOp_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::ProductInfo_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::ProductIndexInfo* v) {
  return PetStore::_marshal_in(s, v);
}
inline void _pretty_print(VISostream& s, const PetStore::ItemPriceList& v) {
  PetStore::_internal_pretty_print(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::ProductHomeImplicit_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::ManagerHomeImplicit_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::Sales_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::OrderHome_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::OrderHomeImplicit_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::OrderItemInfo*& v) {
  return PetStore::_marshal_out(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::ShoppingCart_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::ProductHomeImplicit_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::ProductIndexInfoList_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::ProductIndexInfo& v) {
  return PetStore::_marshal_out(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::SalesHomeImplicit_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::CartHome_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::OrderInfoList* v) {
  return PetStore::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::BusinessOp_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::Manage_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::OrderInfo_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::OrderIndexInfoList& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::ManagerHome_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::Manage_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::OrderItemInfoList_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline void _pretty_print(VISostream& s, const PetStore::SalesRecord& v) {
  PetStore::_internal_pretty_print(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::Sales_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::IDSeq_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::OrderItemHomeImplicit_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline void _pretty_print(VISostream& s, const PetStore::OrderInfoList& v) {
  PetStore::_internal_pretty_print(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::Manager_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::ProductCategory& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::OrderIndexInfo& v) {
  return PetStore::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::PricerHomeExplicit_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::Manager_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::ManagerHomeImplicit_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::OrderInfo*& v) {
  return PetStore::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::OrderOp_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::SalesHomeImplicit_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::ProductIndexInfoList& v) {
  return PetStore::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::OrderItemHomeExplicit_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::ProductList* v) {
  return PetStore::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::Manage_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::Manager_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::OrderItemInfo& v) {
  return PetStore::_marshal_in(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::CartHomeImplicit_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::OrderList_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::Product_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::PricerHomeExplicit_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::SalesRecord& v) {
  return PetStore::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::OrderHomeImplicit_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::OrderInfoList_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::Calculate_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::ProductHomeExplicit_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::ManagerHomeExplicit_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::ItemPriceList_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::SalesHome_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::ProductList_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::Calculate_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::SalesReportConsumer_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::OrderItemHomeImplicit_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::ProductHome_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::OrderInfo*& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::IDSeq* v) {
  return PetStore::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::Order_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::CartHomeImplicit_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline void _pretty_print(VISostream& s, const PetStore::ProductList& v) {
  PetStore::_internal_pretty_print(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::OrderIndexInfoList& v) {
  return PetStore::_marshal_in(s, v);
}
inline void _pretty_print(VISostream& s, const PetStore::IDSeq& v) {
  PetStore::_internal_pretty_print(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::SalesRecordInfoList_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::ShoppingCart_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::OrderItemInfo& v) {
  return PetStore::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::PricerHomeImplicit_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::CartHomeImplicit_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::OrderHome_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::Sales_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::ManagerHomeExplicit_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::ProductHome_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::Order_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::PricerHome_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::OrderIndexInfo*& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::OrderItemInfoList* v) {
  return PetStore::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::OrderHomeImplicit_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::OrderHomeExplicit_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::OrderItemInfo& v) {
  return PetStore::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::OrderHome_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::ProductHomeExplicit_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::ManagerHomeExplicit_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline void _pretty_print(VISostream& s, const PetStore::SalesReport* v) {
  PetStore::_internal_pretty_print(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::AdvancedOp_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::CartHome_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::SalesRecord& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::Calculate_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::Cart_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::Manage_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::Sales_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::SalesHomeExplicit_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::OrderList* v) {
  return PetStore::_marshal_in(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::ProductList& v) {
  return PetStore::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::ManagerHome_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::OrderItemHomeImplicit_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::SalesRecordInfo& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::ProductHome_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::SalesReport_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::Order_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::SalesHomeImplicit_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::Manager_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::OrderItem_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::OrderItemInfo*& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::ProductIndexInfo_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::Product_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::OrderIndexInfoList& v) {
  return PetStore::_marshal_out(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::ProductHomeImplicit_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::ManagerHomeImplicit_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::OrderIndexInfo& v) {
  return PetStore::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::ProductInfo*& v) {
  return PetStore::_marshal_out(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::SalesHome_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::ProductHomeImplicit_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::ManagerHomeImplicit_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::OrderRecord* v) {
  return PetStore::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::OrderRecord_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::Sales_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::IDSeq_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::OrderItemHome_var& v) {
  return PetStore::_marshal_in(s, v);
}
inline Ostream& operator<<(Ostream& s, const PetStore::SalesHomeImplicit_ptr v) {
  return PetStore::_marshal_in(s, v);
}
inline VISostream& operator<<(VISostream& s, const PetStore::ItemPriceList& v) {
  return PetStore::_marshal_in(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::Product_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::CartHome_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::CartHomeExplicit_ptr& v) {
  return PetStore::_marshal_out(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::Manage_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline void _pretty_print(VISostream& s, const PetStore::OrderIndexInfoList& v) {
  PetStore::_internal_pretty_print(s, v);
}
inline Istream& operator>>(Istream& s, PetStore::SalesHome_var& v) {
  return PetStore::_marshal_out(s, v);
}
inline VISistream& operator>>(VISistream& s, PetStore::OrderList_ptr& v) {
  return PetStore::_marshal_out(s, v);
}

#include "vpost.h"
#endif // __PetStore_idl___client
