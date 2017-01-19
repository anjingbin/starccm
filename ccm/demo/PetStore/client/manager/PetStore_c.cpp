/*
 * Copyright 2001 Borland Software Corporation, Inc.
 *
 * GENERATED CODE --- DO NOT EDIT
 * 
 */

#include "PetStore_c.h"

#if defined(MSVCNEWDLL_BUG)
void *PetStore::ItemPriceList::operator new(size_t ts) {
  return ::operator new(ts);
}
void PetStore::ItemPriceList::operator delete(void *p) {
  ::operator delete(p);
}
#endif // MSVCNEWDLL_BUG

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::ItemPriceList& _seq) {
  VISostream _ostrm(_strm);
  if (&_strm == &Cout || &_strm == &Cerr || &_strm == &Clog) {
    _pretty_print(_ostrm, _seq);
    _ostrm << endl;
  } else {
    _ostrm << _seq;
  }
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::ItemPriceList& _obj) {
  VISistream _istrm(_strm);
  _istrm >> _obj;
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::ItemPriceList_ptr& _ptr) {
  VISistream _istrm(_strm);
  _istrm >> _ptr;
  return _strm;
}

CORBA::ULong* PetStore::ItemPriceList::allocbuf(CORBA::ULong _nelems) {
  if (_nelems) {
    return new CORBA::ULong[_nelems];
  }
  return (CORBA::ULong *)NULL;
}

void PetStore::ItemPriceList::freebuf(CORBA::ULong *_data) {
  if (_data)
    delete[] _data;
}

PetStore::ItemPriceList::ItemPriceList(CORBA::ULong _max) {
  _num_allocated = _max;
  _count = 0;
  _contents = allocbuf(_num_allocated);
  _release_flag = 1;
}

PetStore::ItemPriceList::ItemPriceList(CORBA::ULong _max, CORBA::ULong _len,
                                       CORBA::ULong *_data,
                                       CORBA::Boolean _release) {
  _num_allocated = _max;
  _count = _len;
  _contents = _data;
  _release_flag = _release;
}

PetStore::ItemPriceList::ItemPriceList(const PetStore::ItemPriceList& _seq) {
  _num_allocated = _seq._num_allocated;
  _count = _seq._count;
  _contents = allocbuf(_num_allocated);
  if (_num_allocated > 0) {
    memcpy(_contents, _seq._contents, (size_t(_count) * sizeof(CORBA::ULong)));
  }
  _release_flag = 1;
}

PetStore::ItemPriceList::~ItemPriceList() {
  if (_release_flag) 
    freebuf(_contents);
}

PetStore::ItemPriceList& PetStore::ItemPriceList::operator=(const PetStore::ItemPriceList& _seq) {
  if (_num_allocated < _seq._count) {
    if (_release_flag) 
      freebuf(_contents);
    else
      _release_flag = (CORBA::Boolean) 1;

    _contents = allocbuf(_seq._num_allocated);
    _num_allocated = _seq._num_allocated;
  }
  _count = _seq._count;
  if (_num_allocated > 0) {
    memcpy(_contents, _seq._contents, (size_t(_count) * sizeof(CORBA::ULong)));
  }
  return *this;
}

void PetStore::ItemPriceList::length(CORBA::ULong _newlen) {
  if (_newlen > _num_allocated) {
    CORBA::ULong *temp = allocbuf(_newlen);
    memcpy(temp, _contents, (size_t(_count) * sizeof(CORBA::ULong)));
    if (_release_flag) 
      freebuf(_contents);
    else
      _release_flag = (CORBA::Boolean) 1;

    _num_allocated = _newlen;
    _contents = temp;
  }
  _count = _newlen;
}

void PetStore::ItemPriceList::replace(CORBA::ULong _max, CORBA::ULong _len,
                                      CORBA::ULong *_data,
                                      CORBA::Boolean _release) {
  if (_release_flag) 
    freebuf(_contents);
  _num_allocated = _max;
  _count = _len;
  _contents = _data;
  _release_flag = _release;
}

CORBA::ULong *PetStore::ItemPriceList::get_buffer(CORBA::Boolean orphan) {
  if (orphan == (CORBA::Boolean) 1) {
    if (_release_flag == (CORBA::Boolean) 0)
      return (CORBA::ULong*)NULL;
    else {
      _release_flag = (CORBA::Boolean) 0;
      return _contents;
    }
  } else
    return _contents;
}

const CORBA::ULong* PetStore::ItemPriceList::get_buffer() const {
  // Do not use const_cast here because it might not be supported by the compiler
  return (const CORBA::ULong*)(void*)_contents;
}

CORBA::ULong& PetStore::ItemPriceList::operator[](CORBA::ULong _index) {
  if (_index >= _count) {
    VISTHROW(CORBA::BAD_PARAM())
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(static CORBA::ULong *_null = 0; return *_null;)
#endif
  }
  return _contents[_index];
}

const CORBA::ULong& PetStore::ItemPriceList::operator[](CORBA::ULong _index) const {
  if (_index >= _count) {
    VISTHROW(CORBA::BAD_PARAM())
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(static CORBA::ULong *_null = 0; return (const CORBA::ULong&)*_null;)
#endif
  }
  // Do not use const_cast here because it might not be supported by the compiler
  return *(const CORBA::ULong*)(void*)&_contents[_index];
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::ItemPriceList& _seq) {
  _strm << _seq._count;
  if (_seq._count > 0)
    _strm.put(_seq._contents, (unsigned int)_seq._count);
  return _strm;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::ItemPriceList& _seq) {
  CORBA::ULong t_count;
  _strm >> t_count;

  if (t_count > _seq._count) {
    if (_seq._num_allocated < t_count) {
      if (_seq._release_flag)
        PetStore::ItemPriceList::freebuf(_seq._contents);
      else
        _seq._release_flag = (CORBA::Boolean) 1;
      _seq._contents = PetStore::ItemPriceList::allocbuf(t_count);
      _seq._num_allocated = t_count;
    }
  }
  _seq._count = t_count;

  if (_seq._count > 0) {
    _strm.get(_seq._contents, (unsigned int)_seq._count);
  }
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::ItemPriceList* _obj) {
  if (!_obj)
    VISTHROW(CORBA::BAD_PARAM())
  else
    _strm << *_obj;
  return _strm;
}
VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::ItemPriceList_ptr& _obj) {
  _obj = new PetStore::ItemPriceList();
  _strm >> *_obj;
  return _strm;
}

void PetStore::_internal_pretty_print(VISostream& _strm, const PetStore::ItemPriceList& _seq) {
  _strm << "SEQUENCE PetStore::ItemPriceList" << endl;
  _strm << "Number of Elements: " << _seq._count << endl;
  for (CORBA::ULong i=0; i < _seq._count; i++) {
    _strm << "[" << i << "]" << endl;
    _strm << "\t_contents[i]:" << endl;
    _strm << "\t" << _seq._contents[i] << endl;
    _strm << endl;
  }
}
CORBA::ULong& PetStore::ItemPriceList_var::operator[](CORBA::ULong _index) {
  if (_ptr)
    return (*_ptr)[_index];
  static PetStore::ItemPriceList _tmp;
  return _tmp[_index];
}

const CORBA::ULong& PetStore::ItemPriceList_var::operator[](CORBA::ULong _index) const {
  if (_ptr)
    return (*(const PetStore::ItemPriceList*)_ptr)[_index];
  static const PetStore::ItemPriceList _tmp;
  return _tmp[_index];
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::ItemPriceList_var& _var) {
  if (!_var._ptr)
    VISTHROW(CORBA::BAD_PARAM())
  else
    _strm << *_var._ptr;
  return _strm;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::ItemPriceList_var& _var) {
  if (!_var._ptr)
    _var._ptr = new PetStore::ItemPriceList;
  _strm >> *_var._ptr;
  return _strm;
}

PetStore::Calculate_ptr PetStore::Calculate_var::_duplicate(PetStore::Calculate_ptr _p) {
  return PetStore::Calculate::_duplicate(_p);
}

void
PetStore::Calculate_var::_release(PetStore::Calculate_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_p);
}

PetStore::Calculate_var::Calculate_var()
  : _ptr(PetStore::Calculate::_nil()) {}

PetStore::Calculate_var::Calculate_var(PetStore::Calculate_ptr _p)
  : _ptr(_p) {}

PetStore::Calculate_var::Calculate_var(const PetStore::Calculate_var& _var)
  : _ptr(PetStore::Calculate::_duplicate((PetStore::Calculate_ptr) _var)) {}

PetStore::Calculate_var::~Calculate_var() {
  CORBA::release((CORBA::Object_ptr)_ptr);
}

PetStore::Calculate_var&
PetStore::Calculate_var::operator=(const PetStore::Calculate_var& _var) {
    _release(_ptr);
    _ptr = _duplicate(_var._ptr);
    return *this;
}

PetStore::Calculate_var&
PetStore::Calculate_var::operator=(PetStore::Calculate_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_ptr);
  _ptr = _p;
  return *this;
}

PetStore::Calculate_ptr& PetStore::Calculate_var::out() {
  _release(_ptr);
  _ptr = (PetStore::Calculate_ptr)NULL;
  return _ptr;
}

PetStore::Calculate_ptr PetStore::Calculate_var::_retn() {
  PetStore::Calculate_ptr _tmp_ptr = _ptr;
  _ptr = (PetStore::Calculate_ptr)NULL;
  return _tmp_ptr;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::Calculate_var& _var) {
  _strm >> _var._ptr;
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::Calculate_var& _var) {
  _strm << _var._ptr;
  return _strm;
}

Istream& PetStore::_marshal_out(Istream& _strm, PetStore::Calculate_var& _var) {
  VISistream _istrm(_strm);
  _istrm >> _var._ptr;
  return _strm;
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::Calculate_var& _var) {
  _strm << (CORBA::Object_ptr)_var._ptr;
  return _strm;
}

// If dllimport is specified, you might probably want to
// disable these definitions
// disable the const definitions
const VISOps_Info PetStore::Calculate_ops::_ops_info("PetStore::Calculate_ops");
const VISOps_Info *PetStore::Calculate_ops::_desc() { return &_ops_info; }
PetStore::Calculate_ops_ptr PetStore::Calculate_ops::_downcast(PortableServer::ServantBase* _servant) {
  if (_servant == (PortableServer::ServantBase*)NULL)
    return PetStore::Calculate_ops::_nil();
  return (PetStore::Calculate_ops_ptr)_servant->_safe_downcast_ops(_ops_info);
}
const CORBA::TypeInfo
#ifdef AIXV3
PetStore::Calculate::_class_info("PetStore::Calculate",
                                 "IDL:PetStore/Calculate:1.0", NULL,
                                 PetStore::Calculate::_factory, NULL, 0, NULL,
                                 0, CORBA::Object::_desc(), 0L);
#else
PetStore::Calculate::_class_info("PetStore::Calculate",
                                 "IDL:PetStore/Calculate:1.0", NULL,
                                 PetStore::Calculate::_factory, NULL, 0, NULL,
                                 0, CORBA::Object::_desc(), 0L);
#endif

const CORBA::TypeInfo *PetStore::Calculate::_desc() { return &_class_info; }
const CORBA::TypeInfo *PetStore::Calculate::_type_info() const {
  return &_class_info;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::Calculate_ptr& _obj) {
  CORBA::Object_var _var_obj(_obj);
  _var_obj = CORBA::Object::_read(_strm, PetStore::Calculate::_desc());
  _obj = PetStore::Calculate::_narrow(_var_obj);
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::Calculate_ptr _obj) {
  _strm << (CORBA::Object_ptr)_obj;
  return _strm;
}
void* PetStore::Calculate::_safe_narrow(const CORBA::TypeInfo& _info) const {
  if (_info == _class_info)
    return (void *)this;
  return CORBA_Object::_safe_narrow(_info);
}

CORBA::Object *PetStore::Calculate::_factory() {
  return new PetStore::Calculate;
}

PetStore::Calculate_ptr PetStore::Calculate::_this() {
  return PetStore::Calculate::_duplicate(this);
}

PetStore::Calculate_ptr PetStore::Calculate::_narrow(CORBA::Object * _obj) {
  if (_obj == PetStore::Calculate::_nil())
    return PetStore::Calculate::_nil();
  else
    return PetStore::Calculate::_duplicate((PetStore::Calculate_ptr)_obj->_safe_narrow(_class_info));
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::Calculate_ptr _obj) {
  _strm << (CORBA::Object_ptr) _obj;
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::Calculate_ptr& _obj) {
  VISistream _istrm(_strm);
  _istrm >> _obj;
  return _strm;
}

PetStore::Calculate *PetStore::Calculate::_bind(const char *_object_name,
                                                const char *_host_name,
                                                const CORBA::BindOptions *_opt,
                                                CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
          "IDL:PetStore/Calculate:1.0", _object_name, _host_name, _opt, _orb);
  return Calculate::_narrow(_obj);
}

PetStore::Calculate *PetStore::Calculate::_bind(const char *_poa_name,
                                                const CORBA::OctetSequence& _id,
                                                const char *_host_name,
                                                const CORBA::BindOptions *_opt,
                                                CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
   "IDL:PetStore/Calculate:1.0", _poa_name, _id, _host_name, _opt, _orb);
  return Calculate::_narrow(_obj);
}
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
CORBA::ULong PetStore::Calculate::multiply(CORBA::ULong _arg_unitPrice ,
                                           CORBA::Long _arg_quantity ) {
  CORBA::ULong _ret = (CORBA::ULong) 0;
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("multiply");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::Calculate_ops*  _calculate = PetStore::Calculate_ops::_downcast(_servant);
      if (!_calculate) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "multiply");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }

      VISTRY {
         _ret = _calculate->multiply(_arg_unitPrice , _arg_quantity );
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "multiply"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "multiply");
      return _ret;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("multiply", 1);
    VISIF_EXCEP(return _ret;)
    VISostream& _ostrm = *VISostream::_downcast(_obuf);
    _ostrm << _arg_unitPrice;
    _ostrm << _arg_quantity;

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return _ret;)
#endif
  }

  VISistream& _istrm = *VISistream::_downcast(_ibuf);
  _istrm >> _ret;
  return _ret;
}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
CORBA::ULong PetStore::Calculate::add(const ::PetStore::ItemPriceList& _arg_itemList ) {
  CORBA::ULong _ret = (CORBA::ULong) 0;
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("add");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::Calculate_ops*  _calculate = PetStore::Calculate_ops::_downcast(_servant);
      if (!_calculate) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "add");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }

      VISTRY {
         _ret = _calculate->add(_arg_itemList );
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "add"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "add");
      return _ret;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("add", 1);
    VISIF_EXCEP(return _ret;)
    VISostream& _ostrm = *VISostream::_downcast(_obuf);
    _ostrm << _arg_itemList;

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return _ret;)
#endif
  }

  VISistream& _istrm = *VISistream::_downcast(_ibuf);
  _istrm >> _ret;

  return _ret;
}

#if defined(__BORLANDC__)
 #pragma option pop
#endif

PetStore::Pricer_ptr PetStore::Pricer_var::_duplicate(PetStore::Pricer_ptr _p) {
  return PetStore::Pricer::_duplicate(_p);
}

void
PetStore::Pricer_var::_release(PetStore::Pricer_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_p);
}

PetStore::Pricer_var::Pricer_var()
  : _ptr(PetStore::Pricer::_nil()) {}

PetStore::Pricer_var::Pricer_var(PetStore::Pricer_ptr _p)
  : _ptr(_p) {}

PetStore::Pricer_var::Pricer_var(const PetStore::Pricer_var& _var)
  : _ptr(PetStore::Pricer::_duplicate((PetStore::Pricer_ptr) _var)) {}

PetStore::Pricer_var::~Pricer_var() {
  CORBA::release((CORBA::Object_ptr)_ptr);
}

PetStore::Pricer_var&
PetStore::Pricer_var::operator=(const PetStore::Pricer_var& _var) {
    _release(_ptr);
    _ptr = _duplicate(_var._ptr);
    return *this;
}

PetStore::Pricer_var&
PetStore::Pricer_var::operator=(PetStore::Pricer_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_ptr);
  _ptr = _p;
  return *this;
}

PetStore::Pricer_ptr& PetStore::Pricer_var::out() {
  _release(_ptr);
  _ptr = (PetStore::Pricer_ptr)NULL;
  return _ptr;
}

PetStore::Pricer_ptr PetStore::Pricer_var::_retn() {
  PetStore::Pricer_ptr _tmp_ptr = _ptr;
  _ptr = (PetStore::Pricer_ptr)NULL;
  return _tmp_ptr;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::Pricer_var& _var) {
  _strm >> _var._ptr;
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::Pricer_var& _var) {
  _strm << _var._ptr;
  return _strm;
}

Istream& PetStore::_marshal_out(Istream& _strm, PetStore::Pricer_var& _var) {
  VISistream _istrm(_strm);
  _istrm >> _var._ptr;
  return _strm;
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::Pricer_var& _var) {
  _strm << (CORBA::Object_ptr)_var._ptr;
  return _strm;
}

// If dllimport is specified, you might probably want to
// disable these definitions
// disable the const definitions
const VISOps_Info PetStore::Pricer_ops::_ops_info("PetStore::Pricer_ops");
const VISOps_Info *PetStore::Pricer_ops::_desc() { return &_ops_info; }
PetStore::Pricer_ops_ptr PetStore::Pricer_ops::_downcast(PortableServer::ServantBase* _servant) {
  if (_servant == (PortableServer::ServantBase*)NULL)
    return PetStore::Pricer_ops::_nil();
  return (PetStore::Pricer_ops_ptr)_servant->_safe_downcast_ops(_ops_info);
}
const CORBA::TypeInfo
#ifdef AIXV3
PetStore::Pricer::_class_info("PetStore::Pricer", "IDL:PetStore/Pricer:1.0",
                              NULL, PetStore::Pricer::_factory, NULL, 0, NULL,
                              0, PetStore::Calculate::_desc(),
                              Components::CCMObject::_desc(), 0L);
#else
PetStore::Pricer::_class_info("PetStore::Pricer", "IDL:PetStore/Pricer:1.0",
                              NULL, PetStore::Pricer::_factory, NULL, 0, NULL,
                              0, PetStore::Calculate::_desc(),
                              Components::CCMObject::_desc(), 0L);
#endif

const CORBA::TypeInfo *PetStore::Pricer::_desc() { return &_class_info; }
const CORBA::TypeInfo *PetStore::Pricer::_type_info() const {
  return &_class_info;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::Pricer_ptr& _obj) {
  CORBA::Object_var _var_obj(_obj);
  _var_obj = CORBA::Object::_read(_strm, PetStore::Pricer::_desc());
  _obj = PetStore::Pricer::_narrow(_var_obj);
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::Pricer_ptr _obj) {
  _strm << (CORBA::Object_ptr)_obj;
  return _strm;
}
void* PetStore::Pricer::_safe_narrow(const CORBA::TypeInfo& _info) const {
  if (_info == _class_info)
    return (void *)this;
  if (_info == *PetStore::Calculate::_desc())
    return (void *) (PetStore::Calculate *) this;
  if (_info == *Components::CCMObject::_desc())
    return (void *) (Components::CCMObject *) this;
  if (_info == *Components::Navigation::_desc())
    return (void *) (Components::Navigation *) this;
  if (_info == *Components::Receptacles::_desc())
    return (void *) (Components::Receptacles *) this;
  if (_info == *Components::Events::_desc())
    return (void *) (Components::Events *) this;
  return CORBA_Object::_safe_narrow(_info);
}

CORBA::Object *PetStore::Pricer::_factory() {
  return new PetStore::Pricer;
}

PetStore::Pricer_ptr PetStore::Pricer::_this() {
  return PetStore::Pricer::_duplicate(this);
}

PetStore::Pricer_ptr PetStore::Pricer::_narrow(CORBA::Object * _obj) {
  if (_obj == PetStore::Pricer::_nil())
    return PetStore::Pricer::_nil();
  else
    return PetStore::Pricer::_duplicate((PetStore::Pricer_ptr)_obj->_safe_narrow(_class_info));
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::Pricer_ptr _obj) {
  _strm << (CORBA::Object_ptr) _obj;
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::Pricer_ptr& _obj) {
  VISistream _istrm(_strm);
  _istrm >> _obj;
  return _strm;
}

PetStore::Pricer *PetStore::Pricer::_bind(const char *_object_name,
                                          const char *_host_name,
                                          const CORBA::BindOptions *_opt,
                                          CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
          "IDL:PetStore/Pricer:1.0", _object_name, _host_name, _opt, _orb);
  return Pricer::_narrow(_obj);
}

PetStore::Pricer *PetStore::Pricer::_bind(const char *_poa_name,
                                          const CORBA::OctetSequence& _id,
                                          const char *_host_name,
                                          const CORBA::BindOptions *_opt,
                                          CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
   "IDL:PetStore/Pricer:1.0", _poa_name, _id, _host_name, _opt, _orb);
  return Pricer::_narrow(_obj);
}

void PetStore::keyID_var::_add_ref(PetStore::keyID_ptr _p) {
  CORBA::add_ref(_p);
}

void PetStore::keyID_var::_remove_ref(PetStore::keyID_ptr _p) {
  CORBA::remove_ref(_p);
}

PetStore::keyID_var::keyID_var() : _ptr((PetStore::keyID_ptr)NULL) {}

PetStore::keyID_var::keyID_var(PetStore::keyID_ptr _p) : _ptr(_p) {}

PetStore::keyID_var::keyID_var(const PetStore::keyID_var& _var) {
  CORBA::add_ref(_var._ptr);
  _ptr = _var._ptr;
}

PetStore::keyID_var::~keyID_var() { CORBA::remove_ref(_ptr); }

PetStore::keyID_var& PetStore::keyID_var::operator=(PetStore::keyID_ptr _p) {
  CORBA::remove_ref(_ptr);
  _ptr = _p;
  return *this;
}

void PetStore::keyID_var::operator=(const PetStore::keyID_var & _v) {
  if (_ptr)
    _remove_ref(_ptr);
  if (_v._ptr) {
    _add_ref(_v._ptr);
    _ptr = _v._ptr;
  } else
    _ptr = (PetStore::keyID_ptr)NULL;
}

PetStore::keyID_ptr& PetStore::keyID_var::out() {
  CORBA::remove_ref(_ptr);
  _ptr = (PetStore::keyID_ptr)NULL;
  return _ptr;
}

PetStore::keyID_ptr PetStore::keyID_var::_retn() {
  PetStore::keyID_ptr _tmp_ptr = _ptr;
  _ptr = (PetStore::keyID_ptr)NULL;
  return _tmp_ptr;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::keyID_var& _var) {
  _strm >> _var._ptr;
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::keyID_var& _var) {
  _strm << _var._ptr;
  return _strm;
}

Istream& PetStore::_marshal_out(Istream& _strm, PetStore::keyID_var& _var) {
  VISistream _istrm(_strm);
  _istrm >> _var._ptr;
  return _strm;
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::keyID_var& _var) {
  _strm << _var._ptr;
  return _strm;
}

PetStore::keyID_ptr PetStore::keyID::_downcast(CORBA::ValueBase* _val) {
  if (!_val)
    return (PetStore::keyID_ptr)NULL;

  PetStore::keyID_ptr retval = (PetStore::keyID_ptr)_val->_safe_downcast(keyID::_info());
  return retval;
}

const VISValueInfo& PetStore::keyID::_stat_info = PetStore::keyID::_info();
const VISValueInfo& PetStore::keyID::_info() {
  static VISValueInfo _val_info("IDL:PetStore/keyID:1.0", 0, 1,
                                "IDL:PetStore/keyID:1.0");
  return _val_info;
}

const VISValueInfo& PetStore::keyID ::_type_info() const {
  return PetStore::keyID::_info();
}

void *PetStore::keyID::_safe_downcast(const VISValueInfo &info) const {
  if (PetStore::keyID::_info() == info)
    return (void *) this;
  else if (Components::PrimaryKeyBase::_info() == info) {
    return (void*) (Components::PrimaryKeyBase*) this;
  }
  return (void*)NULL;
}

#if defined(MSVCNEWDLL_BUG)
void *PetStore::keyID::operator new(size_t ts) {
  return ::operator new(ts);
}
void PetStore::keyID::operator delete(void *p) {
  ::operator delete(p);
}
#endif // MSVCNEWDLL_BUG

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::keyID_ptr _obj) {
  VISostream _ostrm(_strm);
  if (&_strm == &Cout || &_strm == &Cerr || &_strm == &Clog) {
    _pretty_print(_ostrm, *_obj);
}
  else {
    _ostrm << _obj;
}
  return _strm;
}

Istream& PetStore::_marshal_out(Istream& _strm, PetStore::keyID_ptr& _obj) {
  VISistream _istrm(_strm);
  _istrm >> _obj;
  return _strm;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::keyID_ptr& _obj) {
  _obj = PetStore::keyID::_downcast(PetStore::keyID::_info().read(_strm));
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::keyID_ptr _obj) {
  PetStore::keyID::_info().write(_strm, _obj);
  return _strm;
}

void PetStore::_internal_pretty_print(VISostream& _strm, const PetStore::keyID& _obj) {
  _strm << "VALUETYPE PetStore::keyID {" << endl;
  if(&_obj) {
        _strm << "\tid():" << endl;
    _strm << "\t" << _obj.id() << endl;

  } else
    _strm << "NULL" << endl;
  _strm << "}" << endl;
}
void PetStore::_internal_pretty_print(VISostream& _strm, const PetStore::keyID* _obj) {
  _pretty_print(_strm, *_obj);
}

#if defined(__BORLANDC__)
 #pragma option push
 #pragma warn -8057
#endif

void PetStore::keyID::_write_state(VISostream& _ostrm) {
  CORBA::String_var _obv_id;
  _obv_id = CORBA::string_dup(id());
  _ostrm << _obv_id;
}
void PetStore::keyID::_read_state(VISistream& _istrm) {
  CORBA::String_var _obv_id;
  _istrm >> _obv_id;
  id(_obv_id);
}
#if defined(__BORLANDC__)
 #pragma option pop
#endif

PetStore::OBV_keyID::OBV_keyID(){}
PetStore::OBV_keyID::~OBV_keyID(){}

PetStore::Product_ptr PetStore::Product_var::_duplicate(PetStore::Product_ptr _p) {
  return PetStore::Product::_duplicate(_p);
}

void
PetStore::Product_var::_release(PetStore::Product_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_p);
}

PetStore::Product_var::Product_var()
  : _ptr(PetStore::Product::_nil()) {}

PetStore::Product_var::Product_var(PetStore::Product_ptr _p)
  : _ptr(_p) {}

PetStore::Product_var::Product_var(const PetStore::Product_var& _var)
  : _ptr(PetStore::Product::_duplicate((PetStore::Product_ptr) _var)) {}

PetStore::Product_var::~Product_var() {
  CORBA::release((CORBA::Object_ptr)_ptr);
}

PetStore::Product_var&
PetStore::Product_var::operator=(const PetStore::Product_var& _var) {
    _release(_ptr);
    _ptr = _duplicate(_var._ptr);
    return *this;
}

PetStore::Product_var&
PetStore::Product_var::operator=(PetStore::Product_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_ptr);
  _ptr = _p;
  return *this;
}

PetStore::Product_ptr& PetStore::Product_var::out() {
  _release(_ptr);
  _ptr = (PetStore::Product_ptr)NULL;
  return _ptr;
}

PetStore::Product_ptr PetStore::Product_var::_retn() {
  PetStore::Product_ptr _tmp_ptr = _ptr;
  _ptr = (PetStore::Product_ptr)NULL;
  return _tmp_ptr;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::Product_var& _var) {
  _strm >> _var._ptr;
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::Product_var& _var) {
  _strm << _var._ptr;
  return _strm;
}

Istream& PetStore::_marshal_out(Istream& _strm, PetStore::Product_var& _var) {
  VISistream _istrm(_strm);
  _istrm >> _var._ptr;
  return _strm;
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::Product_var& _var) {
  _strm << (CORBA::Object_ptr)_var._ptr;
  return _strm;
}

// If dllimport is specified, you might probably want to
// disable these definitions
// disable the const definitions
const VISOps_Info PetStore::Product_ops::_ops_info("PetStore::Product_ops");
const VISOps_Info *PetStore::Product_ops::_desc() { return &_ops_info; }
PetStore::Product_ops_ptr PetStore::Product_ops::_downcast(PortableServer::ServantBase* _servant) {
  if (_servant == (PortableServer::ServantBase*)NULL)
    return PetStore::Product_ops::_nil();
  return (PetStore::Product_ops_ptr)_servant->_safe_downcast_ops(_ops_info);
}
const CORBA::TypeInfo
#ifdef AIXV3
PetStore::Product::_class_info("PetStore::Product",
                               "IDL:PetStore/Product:1.0", NULL,
                               PetStore::Product::_factory, NULL, 0, NULL, 0,
                               Components::CCMObject::_desc(), 0L);
#else
PetStore::Product::_class_info("PetStore::Product",
                               "IDL:PetStore/Product:1.0", NULL,
                               PetStore::Product::_factory, NULL, 0, NULL, 0,
                               Components::CCMObject::_desc(), 0L);
#endif

const CORBA::TypeInfo *PetStore::Product::_desc() { return &_class_info; }
const CORBA::TypeInfo *PetStore::Product::_type_info() const {
  return &_class_info;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::Product_ptr& _obj) {
  CORBA::Object_var _var_obj(_obj);
  _var_obj = CORBA::Object::_read(_strm, PetStore::Product::_desc());
  _obj = PetStore::Product::_narrow(_var_obj);
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::Product_ptr _obj) {
  _strm << (CORBA::Object_ptr)_obj;
  return _strm;
}
void* PetStore::Product::_safe_narrow(const CORBA::TypeInfo& _info) const {
  if (_info == _class_info)
    return (void *)this;
  if (_info == *Components::CCMObject::_desc())
    return (void *) (Components::CCMObject *) this;
  if (_info == *Components::Navigation::_desc())
    return (void *) (Components::Navigation *) this;
  if (_info == *Components::Receptacles::_desc())
    return (void *) (Components::Receptacles *) this;
  if (_info == *Components::Events::_desc())
    return (void *) (Components::Events *) this;
  return CORBA_Object::_safe_narrow(_info);
}

CORBA::Object *PetStore::Product::_factory() {
  return new PetStore::Product;
}

PetStore::Product_ptr PetStore::Product::_this() {
  return PetStore::Product::_duplicate(this);
}

PetStore::Product_ptr PetStore::Product::_narrow(CORBA::Object * _obj) {
  if (_obj == PetStore::Product::_nil())
    return PetStore::Product::_nil();
  else
    return PetStore::Product::_duplicate((PetStore::Product_ptr)_obj->_safe_narrow(_class_info));
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::Product_ptr _obj) {
  _strm << (CORBA::Object_ptr) _obj;
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::Product_ptr& _obj) {
  VISistream _istrm(_strm);
  _istrm >> _obj;
  return _strm;
}

PetStore::Product *PetStore::Product::_bind(const char *_object_name,
                                            const char *_host_name,
                                            const CORBA::BindOptions *_opt,
                                            CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
          "IDL:PetStore/Product:1.0", _object_name, _host_name, _opt, _orb);
  return Product::_narrow(_obj);
}

PetStore::Product *PetStore::Product::_bind(const char *_poa_name,
                                            const CORBA::OctetSequence& _id,
                                            const char *_host_name,
                                            const CORBA::BindOptions *_opt,
                                            CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
   "IDL:PetStore/Product:1.0", _poa_name, _id, _host_name, _opt, _orb);
  return Product::_narrow(_obj);
}
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
char* PetStore::Product::product_id() {
  char* _ret = (char*) 0;
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("_get_product_id");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::Product_ops*  _product = PetStore::Product_ops::_downcast(_servant);
      if (!_product) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "_get_product_id");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }

      VISTRY {
         _ret = _product->product_id();
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "_get_product_id"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "_get_product_id");
      return _ret;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("_get_product_id", 1);
    VISIF_EXCEP(return _ret;)

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return _ret;)
#endif
  }

  VISistream& _istrm = *VISistream::_downcast(_ibuf);
  _istrm >> _ret;
  return _ret;
}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
char* PetStore::Product::product_name() {
  char* _ret = (char*) 0;
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("_get_product_name");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::Product_ops*  _product = PetStore::Product_ops::_downcast(_servant);
      if (!_product) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "_get_product_name");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }

      VISTRY {
         _ret = _product->product_name();
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "_get_product_name"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "_get_product_name");
      return _ret;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("_get_product_name", 1);
    VISIF_EXCEP(return _ret;)

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return _ret;)
#endif
  }

  VISistream& _istrm = *VISistream::_downcast(_ibuf);
  _istrm >> _ret;
  return _ret;
}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
void PetStore::Product::product_name(const char* _arg_product_name ) {
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("_set_product_name");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::Product_ops*  _product = PetStore::Product_ops::_downcast(_servant);
      if (!_product) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "_set_product_name");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }

      VISTRY {
        _product->product_name(_arg_product_name );
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "_set_product_name"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "_set_product_name");
      return;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("_set_product_name", 1);
    VISIF_EXCEP(return;)
    VISostream& _ostrm = *VISostream::_downcast(_obuf);
    _ostrm << _arg_product_name;

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return;)
#endif
  }

}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
char* PetStore::Product::product_picture() {
  char* _ret = (char*) 0;
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("_get_product_picture");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::Product_ops*  _product = PetStore::Product_ops::_downcast(_servant);
      if (!_product) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "_get_product_picture");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }

      VISTRY {
         _ret = _product->product_picture();
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "_get_product_picture"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "_get_product_picture");
      return _ret;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("_get_product_picture", 1);
    VISIF_EXCEP(return _ret;)

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return _ret;)
#endif
  }

  VISistream& _istrm = *VISistream::_downcast(_ibuf);
  _istrm >> _ret;
  return _ret;
}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
void PetStore::Product::product_picture(const char* _arg_product_picture ) {
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("_set_product_picture");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::Product_ops*  _product = PetStore::Product_ops::_downcast(_servant);
      if (!_product) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "_set_product_picture");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }

      VISTRY {
        _product->product_picture(_arg_product_picture );
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "_set_product_picture"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "_set_product_picture");
      return;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("_set_product_picture", 1);
    VISIF_EXCEP(return;)
    VISostream& _ostrm = *VISostream::_downcast(_obuf);
    _ostrm << _arg_product_picture;

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return;)
#endif
  }

}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
char* PetStore::Product::product_category() {
  char* _ret = (char*) 0;
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("_get_product_category");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::Product_ops*  _product = PetStore::Product_ops::_downcast(_servant);
      if (!_product) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "_get_product_category");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }

      VISTRY {
         _ret = _product->product_category();
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "_get_product_category"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "_get_product_category");
      return _ret;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("_get_product_category", 1);
    VISIF_EXCEP(return _ret;)

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return _ret;)
#endif
  }

  VISistream& _istrm = *VISistream::_downcast(_ibuf);
  _istrm >> _ret;
  return _ret;
}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
void PetStore::Product::product_category(const char* _arg_product_category ) {
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("_set_product_category");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::Product_ops*  _product = PetStore::Product_ops::_downcast(_servant);
      if (!_product) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "_set_product_category");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }

      VISTRY {
        _product->product_category(_arg_product_category );
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "_set_product_category"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "_set_product_category");
      return;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("_set_product_category", 1);
    VISIF_EXCEP(return;)
    VISostream& _ostrm = *VISostream::_downcast(_obuf);
    _ostrm << _arg_product_category;

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return;)
#endif
  }

}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
CORBA::ULong PetStore::Product::product_price() {
  CORBA::ULong _ret = (CORBA::ULong) 0;
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("_get_product_price");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::Product_ops*  _product = PetStore::Product_ops::_downcast(_servant);
      if (!_product) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "_get_product_price");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }

      VISTRY {
         _ret = _product->product_price();
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "_get_product_price"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "_get_product_price");
      return _ret;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("_get_product_price", 1);
    VISIF_EXCEP(return _ret;)

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return _ret;)
#endif
  }

  VISistream& _istrm = *VISistream::_downcast(_ibuf);
  _istrm >> _ret;
  return _ret;
}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
void PetStore::Product::product_price(CORBA::ULong _arg_product_price ) {
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("_set_product_price");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::Product_ops*  _product = PetStore::Product_ops::_downcast(_servant);
      if (!_product) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "_set_product_price");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }

      VISTRY {
        _product->product_price(_arg_product_price );
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "_set_product_price"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "_set_product_price");
      return;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("_set_product_price", 1);
    VISIF_EXCEP(return;)
    VISostream& _ostrm = *VISostream::_downcast(_obuf);
    _ostrm << _arg_product_price;

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return;)
#endif
  }

}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
CORBA::Long PetStore::Product::product_qty() {
  CORBA::Long _ret = (CORBA::Long) 0;
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("_get_product_qty");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::Product_ops*  _product = PetStore::Product_ops::_downcast(_servant);
      if (!_product) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "_get_product_qty");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }

      VISTRY {
         _ret = _product->product_qty();
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "_get_product_qty"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "_get_product_qty");
      return _ret;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("_get_product_qty", 1);
    VISIF_EXCEP(return _ret;)

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return _ret;)
#endif
  }

  VISistream& _istrm = *VISistream::_downcast(_ibuf);
  _istrm >> _ret;
  return _ret;
}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
void PetStore::Product::product_qty(CORBA::Long _arg_product_qty ) {
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("_set_product_qty");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::Product_ops*  _product = PetStore::Product_ops::_downcast(_servant);
      if (!_product) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "_set_product_qty");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }

      VISTRY {
        _product->product_qty(_arg_product_qty );
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "_set_product_qty"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "_set_product_qty");
      return;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("_set_product_qty", 1);
    VISIF_EXCEP(return;)
    VISostream& _ostrm = *VISostream::_downcast(_obuf);
    _ostrm << _arg_product_qty;

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return;)
#endif
  }

}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
char* PetStore::Product::product_description() {
  char* _ret = (char*) 0;
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("_get_product_description");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::Product_ops*  _product = PetStore::Product_ops::_downcast(_servant);
      if (!_product) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "_get_product_description");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }

      VISTRY {
         _ret = _product->product_description();
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "_get_product_description"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "_get_product_description");
      return _ret;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("_get_product_description", 1);
    VISIF_EXCEP(return _ret;)

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return _ret;)
#endif
  }

  VISistream& _istrm = *VISistream::_downcast(_ibuf);
  _istrm >> _ret;
  return _ret;
}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
void PetStore::Product::product_description(const char* _arg_product_description ) {
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("_set_product_description");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::Product_ops*  _product = PetStore::Product_ops::_downcast(_servant);
      if (!_product) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "_set_product_description");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }

      VISTRY {
        _product->product_description(_arg_product_description );
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "_set_product_description"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "_set_product_description");
      return;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("_set_product_description", 1);
    VISIF_EXCEP(return;)
    VISostream& _ostrm = *VISostream::_downcast(_obuf);
    _ostrm << _arg_product_description;

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return;)
#endif
  }

}

#if defined(__BORLANDC__)
 #pragma option pop
#endif

PetStore::OrderItem_ptr PetStore::OrderItem_var::_duplicate(PetStore::OrderItem_ptr _p) {
  return PetStore::OrderItem::_duplicate(_p);
}

void
PetStore::OrderItem_var::_release(PetStore::OrderItem_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_p);
}

PetStore::OrderItem_var::OrderItem_var()
  : _ptr(PetStore::OrderItem::_nil()) {}

PetStore::OrderItem_var::OrderItem_var(PetStore::OrderItem_ptr _p)
  : _ptr(_p) {}

PetStore::OrderItem_var::OrderItem_var(const PetStore::OrderItem_var& _var)
  : _ptr(PetStore::OrderItem::_duplicate((PetStore::OrderItem_ptr) _var)) {}

PetStore::OrderItem_var::~OrderItem_var() {
  CORBA::release((CORBA::Object_ptr)_ptr);
}

PetStore::OrderItem_var&
PetStore::OrderItem_var::operator=(const PetStore::OrderItem_var& _var) {
    _release(_ptr);
    _ptr = _duplicate(_var._ptr);
    return *this;
}

PetStore::OrderItem_var&
PetStore::OrderItem_var::operator=(PetStore::OrderItem_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_ptr);
  _ptr = _p;
  return *this;
}

PetStore::OrderItem_ptr& PetStore::OrderItem_var::out() {
  _release(_ptr);
  _ptr = (PetStore::OrderItem_ptr)NULL;
  return _ptr;
}

PetStore::OrderItem_ptr PetStore::OrderItem_var::_retn() {
  PetStore::OrderItem_ptr _tmp_ptr = _ptr;
  _ptr = (PetStore::OrderItem_ptr)NULL;
  return _tmp_ptr;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::OrderItem_var& _var) {
  _strm >> _var._ptr;
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::OrderItem_var& _var) {
  _strm << _var._ptr;
  return _strm;
}

Istream& PetStore::_marshal_out(Istream& _strm, PetStore::OrderItem_var& _var) {
  VISistream _istrm(_strm);
  _istrm >> _var._ptr;
  return _strm;
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::OrderItem_var& _var) {
  _strm << (CORBA::Object_ptr)_var._ptr;
  return _strm;
}

// If dllimport is specified, you might probably want to
// disable these definitions
// disable the const definitions
const VISOps_Info PetStore::OrderItem_ops::_ops_info("PetStore::OrderItem_ops");
const VISOps_Info *PetStore::OrderItem_ops::_desc() { return &_ops_info; }
PetStore::OrderItem_ops_ptr PetStore::OrderItem_ops::_downcast(PortableServer::ServantBase* _servant) {
  if (_servant == (PortableServer::ServantBase*)NULL)
    return PetStore::OrderItem_ops::_nil();
  return (PetStore::OrderItem_ops_ptr)_servant->_safe_downcast_ops(_ops_info);
}
const CORBA::TypeInfo
#ifdef AIXV3
PetStore::OrderItem::_class_info("PetStore::OrderItem",
                                 "IDL:PetStore/OrderItem:1.0", NULL,
                                 PetStore::OrderItem::_factory, NULL, 0, NULL,
                                 0, Components::CCMObject::_desc(), 0L);
#else
PetStore::OrderItem::_class_info("PetStore::OrderItem",
                                 "IDL:PetStore/OrderItem:1.0", NULL,
                                 PetStore::OrderItem::_factory, NULL, 0, NULL,
                                 0, Components::CCMObject::_desc(), 0L);
#endif

const CORBA::TypeInfo *PetStore::OrderItem::_desc() { return &_class_info; }
const CORBA::TypeInfo *PetStore::OrderItem::_type_info() const {
  return &_class_info;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::OrderItem_ptr& _obj) {
  CORBA::Object_var _var_obj(_obj);
  _var_obj = CORBA::Object::_read(_strm, PetStore::OrderItem::_desc());
  _obj = PetStore::OrderItem::_narrow(_var_obj);
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::OrderItem_ptr _obj) {
  _strm << (CORBA::Object_ptr)_obj;
  return _strm;
}
void* PetStore::OrderItem::_safe_narrow(const CORBA::TypeInfo& _info) const {
  if (_info == _class_info)
    return (void *)this;
  if (_info == *Components::CCMObject::_desc())
    return (void *) (Components::CCMObject *) this;
  if (_info == *Components::Navigation::_desc())
    return (void *) (Components::Navigation *) this;
  if (_info == *Components::Receptacles::_desc())
    return (void *) (Components::Receptacles *) this;
  if (_info == *Components::Events::_desc())
    return (void *) (Components::Events *) this;
  return CORBA_Object::_safe_narrow(_info);
}

CORBA::Object *PetStore::OrderItem::_factory() {
  return new PetStore::OrderItem;
}

PetStore::OrderItem_ptr PetStore::OrderItem::_this() {
  return PetStore::OrderItem::_duplicate(this);
}

PetStore::OrderItem_ptr PetStore::OrderItem::_narrow(CORBA::Object * _obj) {
  if (_obj == PetStore::OrderItem::_nil())
    return PetStore::OrderItem::_nil();
  else
    return PetStore::OrderItem::_duplicate((PetStore::OrderItem_ptr)_obj->_safe_narrow(_class_info));
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::OrderItem_ptr _obj) {
  _strm << (CORBA::Object_ptr) _obj;
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::OrderItem_ptr& _obj) {
  VISistream _istrm(_strm);
  _istrm >> _obj;
  return _strm;
}

PetStore::OrderItem *PetStore::OrderItem::_bind(const char *_object_name,
                                                const char *_host_name,
                                                const CORBA::BindOptions *_opt,
                                                CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
          "IDL:PetStore/OrderItem:1.0", _object_name, _host_name, _opt, _orb);
  return OrderItem::_narrow(_obj);
}

PetStore::OrderItem *PetStore::OrderItem::_bind(const char *_poa_name,
                                                const CORBA::OctetSequence& _id,
                                                const char *_host_name,
                                                const CORBA::BindOptions *_opt,
                                                CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
   "IDL:PetStore/OrderItem:1.0", _poa_name, _id, _host_name, _opt, _orb);
  return OrderItem::_narrow(_obj);
}
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
char* PetStore::OrderItem::orderItem_id() {
  char* _ret = (char*) 0;
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("_get_orderItem_id");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::OrderItem_ops*  _orderitem = PetStore::OrderItem_ops::_downcast(_servant);
      if (!_orderitem) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "_get_orderItem_id");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }

      VISTRY {
         _ret = _orderitem->orderItem_id();
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "_get_orderItem_id"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "_get_orderItem_id");
      return _ret;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("_get_orderItem_id", 1);
    VISIF_EXCEP(return _ret;)

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return _ret;)
#endif
  }

  VISistream& _istrm = *VISistream::_downcast(_ibuf);
  _istrm >> _ret;
  return _ret;
}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
char* PetStore::OrderItem::product_id() {
  char* _ret = (char*) 0;
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("_get_product_id");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::OrderItem_ops*  _orderitem = PetStore::OrderItem_ops::_downcast(_servant);
      if (!_orderitem) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "_get_product_id");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }

      VISTRY {
         _ret = _orderitem->product_id();
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "_get_product_id"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "_get_product_id");
      return _ret;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("_get_product_id", 1);
    VISIF_EXCEP(return _ret;)

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return _ret;)
#endif
  }

  VISistream& _istrm = *VISistream::_downcast(_ibuf);
  _istrm >> _ret;
  return _ret;
}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
void PetStore::OrderItem::product_id(const char* _arg_product_id ) {
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("_set_product_id");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::OrderItem_ops*  _orderitem = PetStore::OrderItem_ops::_downcast(_servant);
      if (!_orderitem) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "_set_product_id");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }

      VISTRY {
        _orderitem->product_id(_arg_product_id );
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "_set_product_id"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "_set_product_id");
      return;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("_set_product_id", 1);
    VISIF_EXCEP(return;)
    VISostream& _ostrm = *VISostream::_downcast(_obuf);
    _ostrm << _arg_product_id;

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return;)
#endif
  }

}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
char* PetStore::OrderItem::product_name() {
  char* _ret = (char*) 0;
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("_get_product_name");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::OrderItem_ops*  _orderitem = PetStore::OrderItem_ops::_downcast(_servant);
      if (!_orderitem) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "_get_product_name");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }

      VISTRY {
         _ret = _orderitem->product_name();
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "_get_product_name"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "_get_product_name");
      return _ret;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("_get_product_name", 1);
    VISIF_EXCEP(return _ret;)

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return _ret;)
#endif
  }

  VISistream& _istrm = *VISistream::_downcast(_ibuf);
  _istrm >> _ret;
  return _ret;
}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
void PetStore::OrderItem::product_name(const char* _arg_product_name ) {
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("_set_product_name");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::OrderItem_ops*  _orderitem = PetStore::OrderItem_ops::_downcast(_servant);
      if (!_orderitem) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "_set_product_name");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }

      VISTRY {
        _orderitem->product_name(_arg_product_name );
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "_set_product_name"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "_set_product_name");
      return;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("_set_product_name", 1);
    VISIF_EXCEP(return;)
    VISostream& _ostrm = *VISostream::_downcast(_obuf);
    _ostrm << _arg_product_name;

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return;)
#endif
  }

}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
CORBA::Long PetStore::OrderItem::quantity() {
  CORBA::Long _ret = (CORBA::Long) 0;
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("_get_quantity");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::OrderItem_ops*  _orderitem = PetStore::OrderItem_ops::_downcast(_servant);
      if (!_orderitem) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "_get_quantity");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }

      VISTRY {
         _ret = _orderitem->quantity();
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "_get_quantity"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "_get_quantity");
      return _ret;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("_get_quantity", 1);
    VISIF_EXCEP(return _ret;)

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return _ret;)
#endif
  }

  VISistream& _istrm = *VISistream::_downcast(_ibuf);
  _istrm >> _ret;
  return _ret;
}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
void PetStore::OrderItem::quantity(CORBA::Long _arg_quantity ) {
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("_set_quantity");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::OrderItem_ops*  _orderitem = PetStore::OrderItem_ops::_downcast(_servant);
      if (!_orderitem) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "_set_quantity");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }

      VISTRY {
        _orderitem->quantity(_arg_quantity );
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "_set_quantity"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "_set_quantity");
      return;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("_set_quantity", 1);
    VISIF_EXCEP(return;)
    VISostream& _ostrm = *VISostream::_downcast(_obuf);
    _ostrm << _arg_quantity;

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return;)
#endif
  }

}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
CORBA::ULong PetStore::OrderItem::unit_price() {
  CORBA::ULong _ret = (CORBA::ULong) 0;
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("_get_unit_price");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::OrderItem_ops*  _orderitem = PetStore::OrderItem_ops::_downcast(_servant);
      if (!_orderitem) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "_get_unit_price");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }

      VISTRY {
         _ret = _orderitem->unit_price();
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "_get_unit_price"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "_get_unit_price");
      return _ret;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("_get_unit_price", 1);
    VISIF_EXCEP(return _ret;)

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return _ret;)
#endif
  }

  VISistream& _istrm = *VISistream::_downcast(_ibuf);
  _istrm >> _ret;
  return _ret;
}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
void PetStore::OrderItem::unit_price(CORBA::ULong _arg_unit_price ) {
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("_set_unit_price");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::OrderItem_ops*  _orderitem = PetStore::OrderItem_ops::_downcast(_servant);
      if (!_orderitem) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "_set_unit_price");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }

      VISTRY {
        _orderitem->unit_price(_arg_unit_price );
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "_set_unit_price"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "_set_unit_price");
      return;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("_set_unit_price", 1);
    VISIF_EXCEP(return;)
    VISostream& _ostrm = *VISostream::_downcast(_obuf);
    _ostrm << _arg_unit_price;

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return;)
#endif
  }

}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
CORBA::ULong PetStore::OrderItem::subTotal() {
  CORBA::ULong _ret = (CORBA::ULong) 0;
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("_get_subTotal");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::OrderItem_ops*  _orderitem = PetStore::OrderItem_ops::_downcast(_servant);
      if (!_orderitem) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "_get_subTotal");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }

      VISTRY {
         _ret = _orderitem->subTotal();
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "_get_subTotal"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "_get_subTotal");
      return _ret;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("_get_subTotal", 1);
    VISIF_EXCEP(return _ret;)

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return _ret;)
#endif
  }

  VISistream& _istrm = *VISistream::_downcast(_ibuf);
  _istrm >> _ret;
  return _ret;
}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
void PetStore::OrderItem::subTotal(CORBA::ULong _arg_subTotal ) {
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("_set_subTotal");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::OrderItem_ops*  _orderitem = PetStore::OrderItem_ops::_downcast(_servant);
      if (!_orderitem) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "_set_subTotal");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }

      VISTRY {
        _orderitem->subTotal(_arg_subTotal );
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "_set_subTotal"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "_set_subTotal");
      return;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("_set_subTotal", 1);
    VISIF_EXCEP(return;)
    VISostream& _ostrm = *VISostream::_downcast(_obuf);
    _ostrm << _arg_subTotal;

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return;)
#endif
  }

}

#if defined(__BORLANDC__)
 #pragma option pop
#endif

#if defined(MSVCNEWDLL_BUG)
void *PetStore::StrSeq::operator new(size_t ts) {
  return ::operator new(ts);
}
void PetStore::StrSeq::operator delete(void *p) {
  ::operator delete(p);
}
#endif // MSVCNEWDLL_BUG

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::StrSeq& _seq) {
  VISostream _ostrm(_strm);
  if (&_strm == &Cout || &_strm == &Cerr || &_strm == &Clog) {
    _pretty_print(_ostrm, _seq);
    _ostrm << endl;
  } else {
    _ostrm << _seq;
  }
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::StrSeq& _obj) {
  VISistream _istrm(_strm);
  _istrm >> _obj;
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::StrSeq_ptr& _ptr) {
  VISistream _istrm(_strm);
  _istrm >> _ptr;
  return _strm;
}

char** PetStore::StrSeq::allocbuf(CORBA::ULong _nelems) {
  if (_nelems) {
    char* *ret = new char*[_nelems];
    memset(ret, 0, size_t(_nelems) * sizeof(char*));
    return ret;
  }
  return (char* *)NULL;
}

void PetStore::StrSeq::freebuf(char* *_data) {
  if (_data)
    delete[] _data;
}

void PetStore::StrSeq::freebuf_elems(char* *_data, CORBA::ULong _nelems) {
  if (_data) {
    for (CORBA::ULong i = 0; i < _nelems; i++) {
      CORBA::string_free(_data[i]);
      _data[i] = (char *) NULL;
    }
    delete[] _data;
  }
}

PetStore::StrSeq::StrSeq(CORBA::ULong _max) {
  _num_allocated = _max;
  _count = 0;
  _contents = allocbuf(_num_allocated);
  _release_flag = 1;
}

PetStore::StrSeq::StrSeq(CORBA::ULong _max, CORBA::ULong _len, char* *_data,
                         CORBA::Boolean _release) {
  _num_allocated = _max;
  _count = _len;
  _contents = _data;
  _release_flag = _release;
}

PetStore::StrSeq::StrSeq(const PetStore::StrSeq& _seq) {
  _num_allocated = _seq._num_allocated;
  _count = _seq._count;
  _contents = allocbuf(_num_allocated);
  if (_num_allocated > 0) {
    for (CORBA::ULong i=0; i < _count; i++) {
      _contents[i] = CORBA::string_dup(_seq._contents[i]);
    }
  }
  _release_flag = 1;
}

PetStore::StrSeq::~StrSeq() {
  if (_release_flag) 
    freebuf_elems(_contents, _count);
}

PetStore::StrSeq& PetStore::StrSeq::operator=(const PetStore::StrSeq& _seq) {
  if (_num_allocated < _seq._count) {
    if (_release_flag) 
      freebuf_elems(_contents, _count);
    else
      _release_flag = (CORBA::Boolean) 1;

    _contents = allocbuf(_seq._num_allocated);
    _num_allocated = _seq._num_allocated;
  } else {
    for (CORBA::ULong i = _seq._count; i < _count; i++) {
      if (_contents[i]) {
        CORBA::string_free(_contents[i]);
        _contents[i] = (char *) NULL;
      }
    }
  }
  _count = _seq._count;
  if (_num_allocated > 0) {
    for (CORBA::ULong i = 0; i < _count; i++) {
      if (_contents[i]) {
        CORBA::string_free(_contents[i]);
        _contents[i] = (char *) NULL;
      }
      _contents[i] = CORBA::string_dup(_seq._contents[i]);
    }
  }
  return *this;
}

void PetStore::StrSeq::length(CORBA::ULong _newlen) {
  if (_newlen > _num_allocated) {
    char* *temp = allocbuf(_newlen);
    for (CORBA::ULong i=0; i < _count; i++) {
      temp[i] = CORBA::string_dup(_contents[i]);
    }
    if (_release_flag) 
      freebuf_elems(_contents, _count);
    else
      _release_flag = (CORBA::Boolean) 1;

    _num_allocated = _newlen;
    _contents = temp;
  }
  else if (_newlen > _count)
    memset(&_contents[_count], 0, size_t(_newlen - _count) * sizeof(char*));
  _count = _newlen;
}

void PetStore::StrSeq::replace(CORBA::ULong _max, CORBA::ULong _len,
                               char* *_data, CORBA::Boolean _release) {
  if (_release_flag) 
    freebuf_elems(_contents, _count);
  _num_allocated = _max;
  _count = _len;
  _contents = _data;
  _release_flag = _release;
}

char* *PetStore::StrSeq::get_buffer(CORBA::Boolean orphan) {
  if (orphan == (CORBA::Boolean) 1) {
    if (_release_flag == (CORBA::Boolean) 0)
      return (char**)NULL;
    else {
      _release_flag = (CORBA::Boolean) 0;
      return _contents;
    }
  } else
    return _contents;
}

const char* const * PetStore::StrSeq::get_buffer() const {
  // Do not use const_cast here because it might not be supported by the compiler
  return (const char* const *)(void*)_contents;
}

PetStore::StrSeq::Sequence_StrSeq PetStore::StrSeq::operator[](CORBA::ULong _index) {
  if (_index >= _count) {
    VISTHROW(CORBA::BAD_PARAM())
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(char* _tmp = (char*)NULL; return PetStore::StrSeq::Sequence_StrSeq(_tmp, (CORBA::Boolean) 0);)
#endif
  }
  return PetStore::StrSeq::Sequence_StrSeq(_contents[_index], _release_flag);
}

const PetStore::StrSeq::Sequence_StrSeq PetStore::StrSeq::operator[](CORBA::ULong _index) const {
  if (_index >= _count) {
    VISTHROW(CORBA::BAD_PARAM())
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(char* _tmp = (char*)NULL; return PetStore::StrSeq::Sequence_StrSeq(_tmp, (CORBA::Boolean) 0);)
#endif
  }
  // Do not use const_cast here because it might not be supported by the compiler
  return PetStore::StrSeq::Sequence_StrSeq(((StrSeq*)this)->_contents[_index], _release_flag);
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::StrSeq& _seq) {
  _strm << _seq._count;
  for (CORBA::ULong i=0; i < _seq._count; i++) {
    _strm << _seq._contents[i];
  }
  return _strm;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::StrSeq& _seq) {
  CORBA::ULong t_count;
  _strm >> t_count;

  if (t_count > _seq._count) {
    if (_seq._num_allocated < t_count) {
      if (_seq._release_flag)
        PetStore::StrSeq::freebuf_elems(_seq._contents, _seq._count);
      else
        _seq._release_flag = (CORBA::Boolean) 1;
      _seq._contents = PetStore::StrSeq::allocbuf(t_count);
      _seq._num_allocated = t_count;
    }
  } else {
    for (CORBA::ULong i = t_count; i < _seq._count; i++) {
      if (_seq._contents[i]) {
        CORBA::string_free(_seq._contents[i]);
        _seq._contents[i] = (char *) NULL;
      }
    }
  }
  _seq._count = t_count;

  for (CORBA::ULong i=0; i < _seq._count; i++) {
    _strm >> _seq._contents[i];
  }
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::StrSeq* _obj) {
  if (!_obj)
    VISTHROW(CORBA::BAD_PARAM())
  else
    _strm << *_obj;
  return _strm;
}
VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::StrSeq_ptr& _obj) {
  _obj = new PetStore::StrSeq();
  _strm >> *_obj;
  return _strm;
}

void PetStore::_internal_pretty_print(VISostream& _strm, const PetStore::StrSeq& _seq) {
  _strm << "SEQUENCE PetStore::StrSeq" << endl;
  _strm << "Number of Elements: " << _seq._count << endl;
  for (CORBA::ULong i=0; i < _seq._count; i++) {
    _strm << "[" << i << "]" << endl;
    _strm << "\t_contents[i]:" << endl;
    _strm << "\t" << _seq._contents[i] << endl;
    _strm << endl;
  }
}
PetStore::StrSeq::Sequence_StrSeq PetStore::StrSeq_var::operator[](CORBA::ULong _index) {
  if (_ptr)
    return (*_ptr)[_index];
  VISTHROW(CORBA::BAD_PARAM())
#if defined(_VIS_NOEXCEPTIONS)
  VISRETURN(static char* _tmp = (char*)NULL;
            return PetStore::StrSeq::Sequence_StrSeq(_tmp, (CORBA::Boolean)0);)
#endif
}

const PetStore::StrSeq::Sequence_StrSeq PetStore::StrSeq_var::operator[](CORBA::ULong _index) const {
  if (_ptr)
    return (*(const PetStore::StrSeq*)_ptr)[_index];
  VISTHROW(CORBA::BAD_PARAM())
#if defined(_VIS_NOEXCEPTIONS)
  VISRETURN(static char* _tmp = (char*)NULL;
            return PetStore::StrSeq::Sequence_StrSeq(_tmp, (CORBA::Boolean) 0);)
#endif
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::StrSeq_var& _var) {
  if (!_var._ptr)
    VISTHROW(CORBA::BAD_PARAM())
  else
    _strm << *_var._ptr;
  return _strm;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::StrSeq_var& _var) {
  if (!_var._ptr)
    _var._ptr = new PetStore::StrSeq;
  _strm >> *_var._ptr;
  return _strm;
}

PetStore::Order_ptr PetStore::Order_var::_duplicate(PetStore::Order_ptr _p) {
  return PetStore::Order::_duplicate(_p);
}

void
PetStore::Order_var::_release(PetStore::Order_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_p);
}

PetStore::Order_var::Order_var()
  : _ptr(PetStore::Order::_nil()) {}

PetStore::Order_var::Order_var(PetStore::Order_ptr _p)
  : _ptr(_p) {}

PetStore::Order_var::Order_var(const PetStore::Order_var& _var)
  : _ptr(PetStore::Order::_duplicate((PetStore::Order_ptr) _var)) {}

PetStore::Order_var::~Order_var() {
  CORBA::release((CORBA::Object_ptr)_ptr);
}

PetStore::Order_var&
PetStore::Order_var::operator=(const PetStore::Order_var& _var) {
    _release(_ptr);
    _ptr = _duplicate(_var._ptr);
    return *this;
}

PetStore::Order_var&
PetStore::Order_var::operator=(PetStore::Order_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_ptr);
  _ptr = _p;
  return *this;
}

PetStore::Order_ptr& PetStore::Order_var::out() {
  _release(_ptr);
  _ptr = (PetStore::Order_ptr)NULL;
  return _ptr;
}

PetStore::Order_ptr PetStore::Order_var::_retn() {
  PetStore::Order_ptr _tmp_ptr = _ptr;
  _ptr = (PetStore::Order_ptr)NULL;
  return _tmp_ptr;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::Order_var& _var) {
  _strm >> _var._ptr;
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::Order_var& _var) {
  _strm << _var._ptr;
  return _strm;
}

Istream& PetStore::_marshal_out(Istream& _strm, PetStore::Order_var& _var) {
  VISistream _istrm(_strm);
  _istrm >> _var._ptr;
  return _strm;
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::Order_var& _var) {
  _strm << (CORBA::Object_ptr)_var._ptr;
  return _strm;
}

// If dllimport is specified, you might probably want to
// disable these definitions
// disable the const definitions
const VISOps_Info PetStore::Order_ops::_ops_info("PetStore::Order_ops");
const VISOps_Info *PetStore::Order_ops::_desc() { return &_ops_info; }
PetStore::Order_ops_ptr PetStore::Order_ops::_downcast(PortableServer::ServantBase* _servant) {
  if (_servant == (PortableServer::ServantBase*)NULL)
    return PetStore::Order_ops::_nil();
  return (PetStore::Order_ops_ptr)_servant->_safe_downcast_ops(_ops_info);
}
const CORBA::TypeInfo
#ifdef AIXV3
PetStore::Order::_class_info("PetStore::Order", "IDL:PetStore/Order:1.0",
                             NULL, PetStore::Order::_factory, NULL, 0, NULL,
                             0, Components::CCMObject::_desc(), 0L);
#else
PetStore::Order::_class_info("PetStore::Order", "IDL:PetStore/Order:1.0",
                             NULL, PetStore::Order::_factory, NULL, 0, NULL,
                             0, Components::CCMObject::_desc(), 0L);
#endif

const CORBA::TypeInfo *PetStore::Order::_desc() { return &_class_info; }
const CORBA::TypeInfo *PetStore::Order::_type_info() const {
  return &_class_info;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::Order_ptr& _obj) {
  CORBA::Object_var _var_obj(_obj);
  _var_obj = CORBA::Object::_read(_strm, PetStore::Order::_desc());
  _obj = PetStore::Order::_narrow(_var_obj);
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::Order_ptr _obj) {
  _strm << (CORBA::Object_ptr)_obj;
  return _strm;
}
void* PetStore::Order::_safe_narrow(const CORBA::TypeInfo& _info) const {
  if (_info == _class_info)
    return (void *)this;
  if (_info == *Components::CCMObject::_desc())
    return (void *) (Components::CCMObject *) this;
  if (_info == *Components::Navigation::_desc())
    return (void *) (Components::Navigation *) this;
  if (_info == *Components::Receptacles::_desc())
    return (void *) (Components::Receptacles *) this;
  if (_info == *Components::Events::_desc())
    return (void *) (Components::Events *) this;
  return CORBA_Object::_safe_narrow(_info);
}

CORBA::Object *PetStore::Order::_factory() {
  return new PetStore::Order;
}

PetStore::Order_ptr PetStore::Order::_this() {
  return PetStore::Order::_duplicate(this);
}

PetStore::Order_ptr PetStore::Order::_narrow(CORBA::Object * _obj) {
  if (_obj == PetStore::Order::_nil())
    return PetStore::Order::_nil();
  else
    return PetStore::Order::_duplicate((PetStore::Order_ptr)_obj->_safe_narrow(_class_info));
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::Order_ptr _obj) {
  _strm << (CORBA::Object_ptr) _obj;
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::Order_ptr& _obj) {
  VISistream _istrm(_strm);
  _istrm >> _obj;
  return _strm;
}

PetStore::Order *PetStore::Order::_bind(const char *_object_name,
                                        const char *_host_name,
                                        const CORBA::BindOptions *_opt,
                                        CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
          "IDL:PetStore/Order:1.0", _object_name, _host_name, _opt, _orb);
  return Order::_narrow(_obj);
}

PetStore::Order *PetStore::Order::_bind(const char *_poa_name,
                                        const CORBA::OctetSequence& _id,
                                        const char *_host_name,
                                        const CORBA::BindOptions *_opt,
                                        CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
   "IDL:PetStore/Order:1.0", _poa_name, _id, _host_name, _opt, _orb);
  return Order::_narrow(_obj);
}
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
char* PetStore::Order::order_id() {
  char* _ret = (char*) 0;
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("_get_order_id");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::Order_ops*  _order = PetStore::Order_ops::_downcast(_servant);
      if (!_order) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "_get_order_id");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }

      VISTRY {
         _ret = _order->order_id();
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "_get_order_id"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "_get_order_id");
      return _ret;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("_get_order_id", 1);
    VISIF_EXCEP(return _ret;)

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return _ret;)
#endif
  }

  VISistream& _istrm = *VISistream::_downcast(_ibuf);
  _istrm >> _ret;
  return _ret;
}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
char* PetStore::Order::user_name() {
  char* _ret = (char*) 0;
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("_get_user_name");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::Order_ops*  _order = PetStore::Order_ops::_downcast(_servant);
      if (!_order) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "_get_user_name");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }

      VISTRY {
         _ret = _order->user_name();
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "_get_user_name"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "_get_user_name");
      return _ret;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("_get_user_name", 1);
    VISIF_EXCEP(return _ret;)

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return _ret;)
#endif
  }

  VISistream& _istrm = *VISistream::_downcast(_ibuf);
  _istrm >> _ret;
  return _ret;
}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
void PetStore::Order::user_name(const char* _arg_user_name ) {
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("_set_user_name");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::Order_ops*  _order = PetStore::Order_ops::_downcast(_servant);
      if (!_order) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "_set_user_name");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }

      VISTRY {
        _order->user_name(_arg_user_name );
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "_set_user_name"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "_set_user_name");
      return;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("_set_user_name", 1);
    VISIF_EXCEP(return;)
    VISostream& _ostrm = *VISostream::_downcast(_obuf);
    _ostrm << _arg_user_name;

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return;)
#endif
  }

}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
char* PetStore::Order::user_address() {
  char* _ret = (char*) 0;
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("_get_user_address");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::Order_ops*  _order = PetStore::Order_ops::_downcast(_servant);
      if (!_order) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "_get_user_address");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }

      VISTRY {
         _ret = _order->user_address();
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "_get_user_address"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "_get_user_address");
      return _ret;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("_get_user_address", 1);
    VISIF_EXCEP(return _ret;)

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return _ret;)
#endif
  }

  VISistream& _istrm = *VISistream::_downcast(_ibuf);
  _istrm >> _ret;
  return _ret;
}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
void PetStore::Order::user_address(const char* _arg_user_address ) {
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("_set_user_address");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::Order_ops*  _order = PetStore::Order_ops::_downcast(_servant);
      if (!_order) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "_set_user_address");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }

      VISTRY {
        _order->user_address(_arg_user_address );
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "_set_user_address"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "_set_user_address");
      return;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("_set_user_address", 1);
    VISIF_EXCEP(return;)
    VISostream& _ostrm = *VISostream::_downcast(_obuf);
    _ostrm << _arg_user_address;

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return;)
#endif
  }

}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
::PetStore::StrSeq* PetStore::Order::order_items() {
  ::PetStore::StrSeq* _ret = (::PetStore::StrSeq_ptr) NULL;
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("_get_order_items");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::Order_ops*  _order = PetStore::Order_ops::_downcast(_servant);
      if (!_order) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "_get_order_items");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }

      VISTRY {
         _ret = _order->order_items();
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "_get_order_items"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "_get_order_items");
      return _ret;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("_get_order_items", 1);
    VISIF_EXCEP(return _ret;)

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return _ret;)
#endif
  }

  VISistream& _istrm = *VISistream::_downcast(_ibuf);
  _istrm >> _ret;
  return _ret;
}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
void PetStore::Order::order_items(const ::PetStore::StrSeq& _arg_order_items ) {
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("_set_order_items");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::Order_ops*  _order = PetStore::Order_ops::_downcast(_servant);
      if (!_order) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "_set_order_items");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }

      VISTRY {
        _order->order_items(_arg_order_items );
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "_set_order_items"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "_set_order_items");
      return;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("_set_order_items", 1);
    VISIF_EXCEP(return;)
    VISostream& _ostrm = *VISostream::_downcast(_obuf);
    _ostrm << _arg_order_items;

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return;)
#endif
  }

}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
char* PetStore::Order::order_time() {
  char* _ret = (char*) 0;
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("_get_order_time");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::Order_ops*  _order = PetStore::Order_ops::_downcast(_servant);
      if (!_order) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "_get_order_time");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }

      VISTRY {
         _ret = _order->order_time();
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "_get_order_time"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "_get_order_time");
      return _ret;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("_get_order_time", 1);
    VISIF_EXCEP(return _ret;)

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return _ret;)
#endif
  }

  VISistream& _istrm = *VISistream::_downcast(_ibuf);
  _istrm >> _ret;
  return _ret;
}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
void PetStore::Order::order_time(const char* _arg_order_time ) {
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("_set_order_time");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::Order_ops*  _order = PetStore::Order_ops::_downcast(_servant);
      if (!_order) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "_set_order_time");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }

      VISTRY {
        _order->order_time(_arg_order_time );
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "_set_order_time"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "_set_order_time");
      return;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("_set_order_time", 1);
    VISIF_EXCEP(return;)
    VISostream& _ostrm = *VISostream::_downcast(_obuf);
    _ostrm << _arg_order_time;

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return;)
#endif
  }

}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
CORBA::ULong PetStore::Order::subTotal() {
  CORBA::ULong _ret = (CORBA::ULong) 0;
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("_get_subTotal");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::Order_ops*  _order = PetStore::Order_ops::_downcast(_servant);
      if (!_order) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "_get_subTotal");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }

      VISTRY {
         _ret = _order->subTotal();
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "_get_subTotal"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "_get_subTotal");
      return _ret;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("_get_subTotal", 1);
    VISIF_EXCEP(return _ret;)

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return _ret;)
#endif
  }

  VISistream& _istrm = *VISistream::_downcast(_ibuf);
  _istrm >> _ret;
  return _ret;
}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
void PetStore::Order::subTotal(CORBA::ULong _arg_subTotal ) {
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("_set_subTotal");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::Order_ops*  _order = PetStore::Order_ops::_downcast(_servant);
      if (!_order) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "_set_subTotal");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }

      VISTRY {
        _order->subTotal(_arg_subTotal );
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "_set_subTotal"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "_set_subTotal");
      return;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("_set_subTotal", 1);
    VISIF_EXCEP(return;)
    VISostream& _ostrm = *VISostream::_downcast(_obuf);
    _ostrm << _arg_subTotal;

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return;)
#endif
  }

}

#if defined(__BORLANDC__)
 #pragma option pop
#endif

#if defined(MSVCNEWDLL_BUG)
void *PetStore::SalesRecordInfo::operator new(size_t ts) {
  return ::operator new(ts);
}
void PetStore::SalesRecordInfo::operator delete(void *p) {
  ::operator delete(p);
}
#endif // MSVCNEWDLL_BUG

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::SalesRecordInfo& _s) {
    _strm << _s.product_id;

    _strm << _s.product_name;

    _strm << _s.quantity;

    _strm << _s.unit_price;

    _strm << _s.subTotal;

  return _strm;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::SalesRecordInfo& _s) {
    _strm >> _s.product_id;

    _strm >> _s.product_name;

    _strm >> _s.quantity;

    _strm >> _s.unit_price;

    _strm >> _s.subTotal;

  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::SalesRecordInfo* _obj) {
  if (!_obj)
    VISTHROW(CORBA::BAD_PARAM())
  else
    _strm << *_obj;
  return _strm;
}
VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::SalesRecordInfo*& _obj) {
  _obj = new PetStore::SalesRecordInfo();
  _strm >> *_obj;
  return _strm;
}

void PetStore::_internal_pretty_print(VISostream& _strm, const PetStore::SalesRecordInfo& _s) {
  _strm << "STRUCT PetStore::SalesRecordInfo {" << endl;
    _strm << "\tproduct_id:" << endl;
  _strm << "\t" << _s.product_id << endl;

    _strm << "\tproduct_name:" << endl;
  _strm << "\t" << _s.product_name << endl;

    _strm << "\tquantity:" << endl;
  _strm << "\t" << _s.quantity << endl;

    _strm << "\tunit_price:" << endl;
  _strm << "\t" << _s.unit_price << endl;

    _strm << "\tsubTotal:" << endl;
  _strm << "\t" << _s.subTotal << endl;

  _strm << "}" << endl;
}
Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::SalesRecordInfo& _s) {
  VISostream _ostrm(_strm);
  if (&_strm == &Cout || &_strm == &Cerr || &_strm == &Clog) {
    _pretty_print(_ostrm, _s);
    _ostrm << endl;
  } else {
      _ostrm << _s;
  }
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::SalesRecordInfo& _obj) {
  VISistream _istrm(_strm);
    _istrm >> _obj;
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::SalesRecordInfo*& _obj) {
  VISistream _istrm(_strm);
    _istrm >> _obj;
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::SalesRecordInfo_var& _var) {
  if (!_var._ptr)
    VISTHROW(CORBA::BAD_PARAM())
  else
    _strm << *_var._ptr;
  return _strm;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::SalesRecordInfo_var& _var) {
  if (!_var._ptr)
    _var._ptr = new PetStore::SalesRecordInfo;
  _strm >> *_var._ptr;
  return _strm;
}

PetStore::SalesRecordInfo_var& PetStore::SalesRecordInfo_var::operator=(PetStore::SalesRecordInfo* _p) {
  delete _ptr;
  _ptr = _p;
  return *this;
}

PetStore::SalesRecordInfo_var& PetStore::SalesRecordInfo_var::operator=(const PetStore::SalesRecordInfo_var& _var) {
  delete _ptr;
  _ptr = _var._ptr ? new PetStore::SalesRecordInfo(*_var._ptr) : 
  (PetStore::SalesRecordInfo*)NULL;
  return *this;
}

#if defined(MSVCNEWDLL_BUG)
void *PetStore::SalesRecordInfoList::operator new(size_t ts) {
  return ::operator new(ts);
}
void PetStore::SalesRecordInfoList::operator delete(void *p) {
  ::operator delete(p);
}
#endif // MSVCNEWDLL_BUG

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::SalesRecordInfoList& _seq) {
  VISostream _ostrm(_strm);
  if (&_strm == &Cout || &_strm == &Cerr || &_strm == &Clog) {
    _pretty_print(_ostrm, _seq);
    _ostrm << endl;
  } else {
    _ostrm << _seq;
  }
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::SalesRecordInfoList& _obj) {
  VISistream _istrm(_strm);
  _istrm >> _obj;
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::SalesRecordInfoList_ptr& _ptr) {
  VISistream _istrm(_strm);
  _istrm >> _ptr;
  return _strm;
}

::PetStore::SalesRecordInfo* PetStore::SalesRecordInfoList::allocbuf(CORBA::ULong _nelems) {
  if (_nelems) {
    return new ::PetStore::SalesRecordInfo[_nelems];
  }
  return (::PetStore::SalesRecordInfo *)NULL;
}

void PetStore::SalesRecordInfoList::freebuf(::PetStore::SalesRecordInfo *_data) {
  if (_data)
    delete[] _data;
}

PetStore::SalesRecordInfoList::SalesRecordInfoList(CORBA::ULong _max) {
  _num_allocated = _max;
  _count = 0;
  _contents = allocbuf(_num_allocated);
  _release_flag = 1;
}

PetStore::SalesRecordInfoList::SalesRecordInfoList(CORBA::ULong _max,
                                                   CORBA::ULong _len,
                                                   ::PetStore::SalesRecordInfo *_data,
                                                   CORBA::Boolean _release) {
  _num_allocated = _max;
  _count = _len;
  _contents = _data;
  _release_flag = _release;
}

PetStore::SalesRecordInfoList::SalesRecordInfoList(const PetStore::SalesRecordInfoList& _seq) {
  _num_allocated = _seq._num_allocated;
  _count = _seq._count;
  _contents = allocbuf(_num_allocated);
  if (_num_allocated > 0) {
    for (CORBA::ULong i=0; i < _count; i++) {
      _contents[i] = _seq._contents[i];
    }
  }
  _release_flag = 1;
}

PetStore::SalesRecordInfoList::~SalesRecordInfoList() {
  if (_release_flag) 
    freebuf(_contents);
}

PetStore::SalesRecordInfoList& PetStore::SalesRecordInfoList::operator=(const PetStore::SalesRecordInfoList& _seq) {
  if (_num_allocated < _seq._count) {
    if (_release_flag) 
      freebuf(_contents);
    else
      _release_flag = (CORBA::Boolean) 1;

    _contents = allocbuf(_seq._num_allocated);
    _num_allocated = _seq._num_allocated;
  }
  _count = _seq._count;
  if (_num_allocated > 0) {
    for (CORBA::ULong i = 0; i < _count; i++) {
      _contents[i] = _seq._contents[i];
    }
  }
  return *this;
}

void PetStore::SalesRecordInfoList::length(CORBA::ULong _newlen) {
  if (_newlen > _num_allocated) {
    ::PetStore::SalesRecordInfo *temp = allocbuf(_newlen);
    for (CORBA::ULong i=0; i < _count; i++) {
      temp[i] = _contents[i];
    }
    if (_release_flag) 
      freebuf(_contents);
    else
      _release_flag = (CORBA::Boolean) 1;

    _num_allocated = _newlen;
    _contents = temp;
  }
  _count = _newlen;
}

void PetStore::SalesRecordInfoList::replace(CORBA::ULong _max,
                                            CORBA::ULong _len,
                                            SalesRecordInfo *_data,
                                            CORBA::Boolean _release) {
  if (_release_flag) 
    freebuf(_contents);
  _num_allocated = _max;
  _count = _len;
  _contents = _data;
  _release_flag = _release;
}

::PetStore::SalesRecordInfo *PetStore::SalesRecordInfoList::get_buffer(CORBA::Boolean orphan) {
  if (orphan == (CORBA::Boolean) 1) {
    if (_release_flag == (CORBA::Boolean) 0)
      return (::PetStore::SalesRecordInfo*)NULL;
    else {
      _release_flag = (CORBA::Boolean) 0;
      return _contents;
    }
  } else
    return _contents;
}

const ::PetStore::SalesRecordInfo* PetStore::SalesRecordInfoList::get_buffer() const {
  // Do not use const_cast here because it might not be supported by the compiler
  return (const ::PetStore::SalesRecordInfo*)(void*)_contents;
}

::PetStore::SalesRecordInfo& PetStore::SalesRecordInfoList::operator[](CORBA::ULong _index) {
  if (_index >= _count) {
    VISTHROW(CORBA::BAD_PARAM())
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(static ::PetStore::SalesRecordInfo *_null = 0; return *_null;)
#endif
  }
  return _contents[_index];
}

const ::PetStore::SalesRecordInfo& PetStore::SalesRecordInfoList::operator[](CORBA::ULong _index) const {
  if (_index >= _count) {
    VISTHROW(CORBA::BAD_PARAM())
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(static ::PetStore::SalesRecordInfo *_null = 0; return (const ::PetStore::SalesRecordInfo&)*_null;)
#endif
  }
  // Do not use const_cast here because it might not be supported by the compiler
  return *(const ::PetStore::SalesRecordInfo*)(void*)&_contents[_index];
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::SalesRecordInfoList& _seq) {
  _strm << _seq._count;
  for (CORBA::ULong i=0; i < _seq._count; i++) {
    _strm << _seq._contents[i];
  }
  return _strm;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::SalesRecordInfoList& _seq) {
  CORBA::ULong t_count;
  _strm >> t_count;

  if (t_count > _seq._count) {
    if (_seq._num_allocated < t_count) {
      if (_seq._release_flag)
        PetStore::SalesRecordInfoList::freebuf(_seq._contents);
      else
        _seq._release_flag = (CORBA::Boolean) 1;
      _seq._contents = PetStore::SalesRecordInfoList::allocbuf(t_count);
      _seq._num_allocated = t_count;
    }
  }
  _seq._count = t_count;

  for (CORBA::ULong i=0; i < _seq._count; i++) {
    _strm >> _seq._contents[i];
  }
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::SalesRecordInfoList* _obj) {
  if (!_obj)
    VISTHROW(CORBA::BAD_PARAM())
  else
    _strm << *_obj;
  return _strm;
}
VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::SalesRecordInfoList_ptr& _obj) {
  _obj = new PetStore::SalesRecordInfoList();
  _strm >> *_obj;
  return _strm;
}

void PetStore::_internal_pretty_print(VISostream& _strm, const PetStore::SalesRecordInfoList& _seq) {
  _strm << "SEQUENCE PetStore::SalesRecordInfoList" << endl;
  _strm << "Number of Elements: " << _seq._count << endl;
  for (CORBA::ULong i=0; i < _seq._count; i++) {
    _strm << "[" << i << "]" << endl;
    _strm << "\t_contents[i]:" << endl;
    _pretty_print(_strm, _seq._contents[i]);
    _strm << endl;
    _strm << endl;
  }
}
::PetStore::SalesRecordInfo& PetStore::SalesRecordInfoList_var::operator[](CORBA::ULong _index) {
  if (_ptr)
    return (*_ptr)[_index];
  static PetStore::SalesRecordInfoList _tmp;
  return _tmp[_index];
}

const ::PetStore::SalesRecordInfo& PetStore::SalesRecordInfoList_var::operator[](CORBA::ULong _index) const {
  if (_ptr)
    return (*(const PetStore::SalesRecordInfoList*)_ptr)[_index];
  static const PetStore::SalesRecordInfoList _tmp;
  return _tmp[_index];
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::SalesRecordInfoList_var& _var) {
  if (!_var._ptr)
    VISTHROW(CORBA::BAD_PARAM())
  else
    _strm << *_var._ptr;
  return _strm;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::SalesRecordInfoList_var& _var) {
  if (!_var._ptr)
    _var._ptr = new PetStore::SalesRecordInfoList;
  _strm >> *_var._ptr;
  return _strm;
}
#if defined(MSVCNEWDLL_BUG)
void *PetStore::SalesRecord::operator new(size_t ts) {
  return ::operator new(ts);
}
void PetStore::SalesRecord::operator delete(void *p) {
  ::operator delete(p);
}
#endif // MSVCNEWDLL_BUG

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::SalesRecord& _s) {
    _strm << _s.sales_record_list;

    _strm << _s.subTotal;

  return _strm;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::SalesRecord& _s) {
    _strm >> _s.sales_record_list;

    _strm >> _s.subTotal;

  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::SalesRecord* _obj) {
  if (!_obj)
    VISTHROW(CORBA::BAD_PARAM())
  else
    _strm << *_obj;
  return _strm;
}
VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::SalesRecord*& _obj) {
  _obj = new PetStore::SalesRecord();
  _strm >> *_obj;
  return _strm;
}

void PetStore::_internal_pretty_print(VISostream& _strm, const PetStore::SalesRecord& _s) {
  _strm << "STRUCT PetStore::SalesRecord {" << endl;
    _strm << "\tsales_record_list:" << endl;
  _strm << "\tsales_record_list:" << endl;
  _pretty_print(_strm, _s.sales_record_list);
  _strm << endl;

    _strm << "\tsubTotal:" << endl;
  _strm << "\t" << _s.subTotal << endl;

  _strm << "}" << endl;
}
Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::SalesRecord& _s) {
  VISostream _ostrm(_strm);
  if (&_strm == &Cout || &_strm == &Cerr || &_strm == &Clog) {
    _pretty_print(_ostrm, _s);
    _ostrm << endl;
  } else {
      _ostrm << _s;
  }
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::SalesRecord& _obj) {
  VISistream _istrm(_strm);
    _istrm >> _obj;
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::SalesRecord*& _obj) {
  VISistream _istrm(_strm);
    _istrm >> _obj;
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::SalesRecord_var& _var) {
  if (!_var._ptr)
    VISTHROW(CORBA::BAD_PARAM())
  else
    _strm << *_var._ptr;
  return _strm;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::SalesRecord_var& _var) {
  if (!_var._ptr)
    _var._ptr = new PetStore::SalesRecord;
  _strm >> *_var._ptr;
  return _strm;
}

PetStore::SalesRecord_var& PetStore::SalesRecord_var::operator=(PetStore::SalesRecord* _p) {
  delete _ptr;
  _ptr = _p;
  return *this;
}

PetStore::SalesRecord_var& PetStore::SalesRecord_var::operator=(const PetStore::SalesRecord_var& _var) {
  delete _ptr;
  _ptr = _var._ptr ? new PetStore::SalesRecord(*_var._ptr) : 
  (PetStore::SalesRecord*)NULL;
  return *this;
}

PetStore::ShoppingCart_ptr PetStore::ShoppingCart_var::_duplicate(PetStore::ShoppingCart_ptr _p) {
  return PetStore::ShoppingCart::_duplicate(_p);
}

void
PetStore::ShoppingCart_var::_release(PetStore::ShoppingCart_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_p);
}

PetStore::ShoppingCart_var::ShoppingCart_var()
  : _ptr(PetStore::ShoppingCart::_nil()) {}

PetStore::ShoppingCart_var::ShoppingCart_var(PetStore::ShoppingCart_ptr _p)
  : _ptr(_p) {}

PetStore::ShoppingCart_var::ShoppingCart_var(const PetStore::ShoppingCart_var& _var)
  : _ptr(PetStore::ShoppingCart::_duplicate((PetStore::ShoppingCart_ptr) _var)) {}

PetStore::ShoppingCart_var::~ShoppingCart_var() {
  CORBA::release((CORBA::Object_ptr)_ptr);
}

PetStore::ShoppingCart_var&
PetStore::ShoppingCart_var::operator=(const PetStore::ShoppingCart_var& _var) {
    _release(_ptr);
    _ptr = _duplicate(_var._ptr);
    return *this;
}

PetStore::ShoppingCart_var&
PetStore::ShoppingCart_var::operator=(PetStore::ShoppingCart_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_ptr);
  _ptr = _p;
  return *this;
}

PetStore::ShoppingCart_ptr& PetStore::ShoppingCart_var::out() {
  _release(_ptr);
  _ptr = (PetStore::ShoppingCart_ptr)NULL;
  return _ptr;
}

PetStore::ShoppingCart_ptr PetStore::ShoppingCart_var::_retn() {
  PetStore::ShoppingCart_ptr _tmp_ptr = _ptr;
  _ptr = (PetStore::ShoppingCart_ptr)NULL;
  return _tmp_ptr;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::ShoppingCart_var& _var) {
  _strm >> _var._ptr;
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::ShoppingCart_var& _var) {
  _strm << _var._ptr;
  return _strm;
}

Istream& PetStore::_marshal_out(Istream& _strm, PetStore::ShoppingCart_var& _var) {
  VISistream _istrm(_strm);
  _istrm >> _var._ptr;
  return _strm;
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::ShoppingCart_var& _var) {
  _strm << (CORBA::Object_ptr)_var._ptr;
  return _strm;
}

// If dllimport is specified, you might probably want to
// disable these definitions
// disable the const definitions
const VISOps_Info PetStore::ShoppingCart_ops::_ops_info("PetStore::ShoppingCart_ops");
const VISOps_Info *PetStore::ShoppingCart_ops::_desc() { return &_ops_info; }
PetStore::ShoppingCart_ops_ptr PetStore::ShoppingCart_ops::_downcast(PortableServer::ServantBase* _servant) {
  if (_servant == (PortableServer::ServantBase*)NULL)
    return PetStore::ShoppingCart_ops::_nil();
  return (PetStore::ShoppingCart_ops_ptr)_servant->_safe_downcast_ops(_ops_info);
}
const CORBA::TypeInfo
#ifdef AIXV3
PetStore::ShoppingCart::_class_info("PetStore::ShoppingCart",
                                    "IDL:PetStore/ShoppingCart:1.0", NULL,
                                    PetStore::ShoppingCart::_factory, NULL, 0,
                                    NULL, 0, CORBA::Object::_desc(), 0L);
#else
PetStore::ShoppingCart::_class_info("PetStore::ShoppingCart",
                                    "IDL:PetStore/ShoppingCart:1.0", NULL,
                                    PetStore::ShoppingCart::_factory, NULL, 0,
                                    NULL, 0, CORBA::Object::_desc(), 0L);
#endif

const CORBA::TypeInfo *PetStore::ShoppingCart::_desc() { return &_class_info; }
const CORBA::TypeInfo *PetStore::ShoppingCart::_type_info() const {
  return &_class_info;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::ShoppingCart_ptr& _obj) {
  CORBA::Object_var _var_obj(_obj);
  _var_obj = CORBA::Object::_read(_strm, PetStore::ShoppingCart::_desc());
  _obj = PetStore::ShoppingCart::_narrow(_var_obj);
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::ShoppingCart_ptr _obj) {
  _strm << (CORBA::Object_ptr)_obj;
  return _strm;
}
void* PetStore::ShoppingCart::_safe_narrow(const CORBA::TypeInfo& _info) const {
  if (_info == _class_info)
    return (void *)this;
  return CORBA_Object::_safe_narrow(_info);
}

CORBA::Object *PetStore::ShoppingCart::_factory() {
  return new PetStore::ShoppingCart;
}

PetStore::ShoppingCart_ptr PetStore::ShoppingCart::_this() {
  return PetStore::ShoppingCart::_duplicate(this);
}

PetStore::ShoppingCart_ptr PetStore::ShoppingCart::_narrow(CORBA::Object * _obj) {
  if (_obj == PetStore::ShoppingCart::_nil())
    return PetStore::ShoppingCart::_nil();
  else
    return PetStore::ShoppingCart::_duplicate((PetStore::ShoppingCart_ptr)_obj->_safe_narrow(_class_info));
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::ShoppingCart_ptr _obj) {
  _strm << (CORBA::Object_ptr) _obj;
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::ShoppingCart_ptr& _obj) {
  VISistream _istrm(_strm);
  _istrm >> _obj;
  return _strm;
}

PetStore::ShoppingCart *PetStore::ShoppingCart::_bind(const char *_object_name,
                                                      const char *_host_name,
                                                      const CORBA::BindOptions *_opt,
                                                      CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
          "IDL:PetStore/ShoppingCart:1.0", _object_name, _host_name, _opt,
_orb);
  return ShoppingCart::_narrow(_obj);
}

PetStore::ShoppingCart *PetStore::ShoppingCart::_bind(const char *_poa_name,
                                                      const CORBA::OctetSequence& _id,
                                                      const char *_host_name,
                                                      const CORBA::BindOptions *_opt,
                                                      CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
   "IDL:PetStore/ShoppingCart:1.0", _poa_name, _id, _host_name, _opt, _orb);
  return ShoppingCart::_narrow(_obj);
}
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
::PetStore::SalesRecord* PetStore::ShoppingCart::list() {
  ::PetStore::SalesRecord* _ret = (::PetStore::SalesRecord_ptr) NULL;
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("list");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::ShoppingCart_ops*  _shoppingcart = PetStore::ShoppingCart_ops::_downcast(_servant);
      if (!_shoppingcart) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "list");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }

      VISTRY {
         _ret = _shoppingcart->list();
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "list"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "list");
      return _ret;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("list", 1);
    VISIF_EXCEP(return _ret;)

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return _ret;)
#endif
  }

  VISistream& _istrm = *VISistream::_downcast(_ibuf);
  _istrm >> _ret;
  return _ret;
}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
void PetStore::ShoppingCart::addProduct(const ::PetStore::SalesRecordInfo& _arg_productWanted ) {
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("addProduct");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::ShoppingCart_ops*  _shoppingcart = PetStore::ShoppingCart_ops::_downcast(_servant);
      if (!_shoppingcart) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "addProduct");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }

      VISTRY {
        _shoppingcart->addProduct(_arg_productWanted );
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "addProduct"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "addProduct");
      return;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("addProduct", 1);
    VISIF_EXCEP(return;)
    VISostream& _ostrm = *VISostream::_downcast(_obuf);
    _ostrm << _arg_productWanted;

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return;)
#endif
  }

}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
void PetStore::ShoppingCart::updateProductQty(const char* _arg_productID ,
                                              CORBA::Long _arg_newQty ) {
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("updateProductQty");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::ShoppingCart_ops*  _shoppingcart = PetStore::ShoppingCart_ops::_downcast(_servant);
      if (!_shoppingcart) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "updateProductQty");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }

      VISTRY {
        _shoppingcart->updateProductQty(_arg_productID , _arg_newQty );
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "updateProductQty"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "updateProductQty");
      return;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("updateProductQty", 1);
    VISIF_EXCEP(return;)
    VISostream& _ostrm = *VISostream::_downcast(_obuf);
    _ostrm << _arg_productID;
    _ostrm << _arg_newQty;

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return;)
#endif
  }

}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
void PetStore::ShoppingCart::deleteProduct(const char* _arg_productID ) {
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("deleteProduct");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::ShoppingCart_ops*  _shoppingcart = PetStore::ShoppingCart_ops::_downcast(_servant);
      if (!_shoppingcart) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "deleteProduct");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }

      VISTRY {
        _shoppingcart->deleteProduct(_arg_productID );
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "deleteProduct"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "deleteProduct");
      return;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("deleteProduct", 1);
    VISIF_EXCEP(return;)
    VISostream& _ostrm = *VISostream::_downcast(_obuf);
    _ostrm << _arg_productID;

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return;)
#endif
  }

}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
void PetStore::ShoppingCart::clear() {
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("clear");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::ShoppingCart_ops*  _shoppingcart = PetStore::ShoppingCart_ops::_downcast(_servant);
      if (!_shoppingcart) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "clear");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }

      VISTRY {
        _shoppingcart->clear();
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "clear"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "clear");
      return;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("clear", 1);
    VISIF_EXCEP(return;)

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return;)
#endif
  }

}

#if defined(__BORLANDC__)
 #pragma option pop
#endif

PetStore::Cart_ptr PetStore::Cart_var::_duplicate(PetStore::Cart_ptr _p) {
  return PetStore::Cart::_duplicate(_p);
}

void
PetStore::Cart_var::_release(PetStore::Cart_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_p);
}

PetStore::Cart_var::Cart_var()
  : _ptr(PetStore::Cart::_nil()) {}

PetStore::Cart_var::Cart_var(PetStore::Cart_ptr _p)
  : _ptr(_p) {}

PetStore::Cart_var::Cart_var(const PetStore::Cart_var& _var)
  : _ptr(PetStore::Cart::_duplicate((PetStore::Cart_ptr) _var)) {}

PetStore::Cart_var::~Cart_var() {
  CORBA::release((CORBA::Object_ptr)_ptr);
}

PetStore::Cart_var&
PetStore::Cart_var::operator=(const PetStore::Cart_var& _var) {
    _release(_ptr);
    _ptr = _duplicate(_var._ptr);
    return *this;
}

PetStore::Cart_var&
PetStore::Cart_var::operator=(PetStore::Cart_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_ptr);
  _ptr = _p;
  return *this;
}

PetStore::Cart_ptr& PetStore::Cart_var::out() {
  _release(_ptr);
  _ptr = (PetStore::Cart_ptr)NULL;
  return _ptr;
}

PetStore::Cart_ptr PetStore::Cart_var::_retn() {
  PetStore::Cart_ptr _tmp_ptr = _ptr;
  _ptr = (PetStore::Cart_ptr)NULL;
  return _tmp_ptr;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::Cart_var& _var) {
  _strm >> _var._ptr;
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::Cart_var& _var) {
  _strm << _var._ptr;
  return _strm;
}

Istream& PetStore::_marshal_out(Istream& _strm, PetStore::Cart_var& _var) {
  VISistream _istrm(_strm);
  _istrm >> _var._ptr;
  return _strm;
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::Cart_var& _var) {
  _strm << (CORBA::Object_ptr)_var._ptr;
  return _strm;
}

// If dllimport is specified, you might probably want to
// disable these definitions
// disable the const definitions
const VISOps_Info PetStore::Cart_ops::_ops_info("PetStore::Cart_ops");
const VISOps_Info *PetStore::Cart_ops::_desc() { return &_ops_info; }
PetStore::Cart_ops_ptr PetStore::Cart_ops::_downcast(PortableServer::ServantBase* _servant) {
  if (_servant == (PortableServer::ServantBase*)NULL)
    return PetStore::Cart_ops::_nil();
  return (PetStore::Cart_ops_ptr)_servant->_safe_downcast_ops(_ops_info);
}
const CORBA::TypeInfo
#ifdef AIXV3
PetStore::Cart::_class_info("PetStore::Cart", "IDL:PetStore/Cart:1.0", NULL,
                            PetStore::Cart::_factory, NULL, 0, NULL, 0,
                            PetStore::ShoppingCart::_desc(),
                            Components::CCMObject::_desc(), 0L);
#else
PetStore::Cart::_class_info("PetStore::Cart", "IDL:PetStore/Cart:1.0", NULL,
                            PetStore::Cart::_factory, NULL, 0, NULL, 0,
                            PetStore::ShoppingCart::_desc(),
                            Components::CCMObject::_desc(), 0L);
#endif

const CORBA::TypeInfo *PetStore::Cart::_desc() { return &_class_info; }
const CORBA::TypeInfo *PetStore::Cart::_type_info() const {
  return &_class_info;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::Cart_ptr& _obj) {
  CORBA::Object_var _var_obj(_obj);
  _var_obj = CORBA::Object::_read(_strm, PetStore::Cart::_desc());
  _obj = PetStore::Cart::_narrow(_var_obj);
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::Cart_ptr _obj) {
  _strm << (CORBA::Object_ptr)_obj;
  return _strm;
}
void* PetStore::Cart::_safe_narrow(const CORBA::TypeInfo& _info) const {
  if (_info == _class_info)
    return (void *)this;
  if (_info == *PetStore::ShoppingCart::_desc())
    return (void *) (PetStore::ShoppingCart *) this;
  if (_info == *Components::CCMObject::_desc())
    return (void *) (Components::CCMObject *) this;
  if (_info == *Components::Navigation::_desc())
    return (void *) (Components::Navigation *) this;
  if (_info == *Components::Receptacles::_desc())
    return (void *) (Components::Receptacles *) this;
  if (_info == *Components::Events::_desc())
    return (void *) (Components::Events *) this;
  return CORBA_Object::_safe_narrow(_info);
}

CORBA::Object *PetStore::Cart::_factory() {
  return new PetStore::Cart;
}

PetStore::Cart_ptr PetStore::Cart::_this() {
  return PetStore::Cart::_duplicate(this);
}

PetStore::Cart_ptr PetStore::Cart::_narrow(CORBA::Object * _obj) {
  if (_obj == PetStore::Cart::_nil())
    return PetStore::Cart::_nil();
  else
    return PetStore::Cart::_duplicate((PetStore::Cart_ptr)_obj->_safe_narrow(_class_info));
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::Cart_ptr _obj) {
  _strm << (CORBA::Object_ptr) _obj;
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::Cart_ptr& _obj) {
  VISistream _istrm(_strm);
  _istrm >> _obj;
  return _strm;
}

PetStore::Cart *PetStore::Cart::_bind(const char *_object_name,
                                      const char *_host_name,
                                      const CORBA::BindOptions *_opt,
                                      CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
          "IDL:PetStore/Cart:1.0", _object_name, _host_name, _opt, _orb);
  return Cart::_narrow(_obj);
}

PetStore::Cart *PetStore::Cart::_bind(const char *_poa_name,
                                      const CORBA::OctetSequence& _id,
                                      const char *_host_name,
                                      const CORBA::BindOptions *_opt,
                                      CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
   "IDL:PetStore/Cart:1.0", _poa_name, _id, _host_name, _opt, _orb);
  return Cart::_narrow(_obj);
}
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
void PetStore::Cart::connect_price_calculater(::PetStore::Calculate_ptr _arg_conn ) {
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("connect_price_calculater");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::Cart_ops*  _cart = PetStore::Cart_ops::_downcast(_servant);
      if (!_cart) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "connect_price_calculater");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }

      VISTRY {
        _cart->connect_price_calculater(_arg_conn );
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "connect_price_calculater"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "connect_price_calculater");
      return;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("connect_price_calculater", 1);
    VISIF_EXCEP(return;)
    VISostream& _ostrm = *VISostream::_downcast(_obuf);
    _ostrm << _arg_conn;

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return;)
#endif
  }

}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
::PetStore::Calculate_ptr PetStore::Cart::disconnect_price_calculater() {
  ::PetStore::Calculate_ptr _ret = (::PetStore::Calculate_ptr) NULL;
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("disconnect_price_calculater");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::Cart_ops*  _cart = PetStore::Cart_ops::_downcast(_servant);
      if (!_cart) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "disconnect_price_calculater");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }

      VISTRY {
         _ret = _cart->disconnect_price_calculater();
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "disconnect_price_calculater"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "disconnect_price_calculater");
      return _ret;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("disconnect_price_calculater", 1);
    VISIF_EXCEP(return _ret;)

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return _ret;)
#endif
  }

  VISistream& _istrm = *VISistream::_downcast(_ibuf);
  _istrm >> _ret;
  return _ret;
}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
::PetStore::Calculate_ptr PetStore::Cart::get_connection_price_calculater() {
  ::PetStore::Calculate_ptr _ret = (::PetStore::Calculate_ptr) NULL;
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("get_connection_price_calculater");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::Cart_ops*  _cart = PetStore::Cart_ops::_downcast(_servant);
      if (!_cart) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "get_connection_price_calculater");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }

      VISTRY {
         _ret = _cart->get_connection_price_calculater();
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "get_connection_price_calculater"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "get_connection_price_calculater");
      return _ret;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("get_connection_price_calculater", 1);
    VISIF_EXCEP(return _ret;)

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return _ret;)
#endif
  }

  VISistream& _istrm = *VISistream::_downcast(_ibuf);
  _istrm >> _ret;
  return _ret;
}

#if defined(__BORLANDC__)
 #pragma option pop
#endif

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::OrderState& _enum_type) {
  VISostream _ostrm(_strm);
  if (&_strm == &Cout || &_strm == &Cerr || &_strm == &Clog) {
    _pretty_print(_ostrm, _enum_type);
    _ostrm << endl;
  } else {
      _ostrm << (CORBA::ULong)_enum_type;
  }
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::OrderState& _enum_type) {
  VISistream _istrm(_strm);
    {CORBA::ULong _temp; _istrm >> _temp; _enum_type = (::PetStore::OrderState)_temp;};
  return _strm;
}
VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::OrderState _enum_type) {
  _strm << (CORBA::ULong)_enum_type;
  return _strm;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::OrderState& _enum_type) {
  CORBA::ULong _temp;
  _strm >> _temp;
  if (_temp >= 2) {
    VISTHROW(CORBA::BAD_PARAM())
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return _strm;)
#endif
  }
  _enum_type = (PetStore::OrderState) _temp;
  return _strm;
}
void PetStore::_internal_pretty_print(VISostream& _strm, PetStore::OrderState _val) {
  _strm << "ENUM PetStore::OrderState::";
  switch (_val) {
    case PetStore::ADD: _strm << "ADD" << endl; break;
    case PetStore::CANCEL: _strm << "CANCEL" << endl; break;
  }
}
void PetStore::SalesReport_var::_add_ref(PetStore::SalesReport_ptr _p) {
  CORBA::add_ref(_p);
}

void PetStore::SalesReport_var::_remove_ref(PetStore::SalesReport_ptr _p) {
  CORBA::remove_ref(_p);
}

PetStore::SalesReport_var::SalesReport_var() : _ptr((PetStore::SalesReport_ptr)NULL) {}

PetStore::SalesReport_var::SalesReport_var(PetStore::SalesReport_ptr _p) : _ptr(_p) {}

PetStore::SalesReport_var::SalesReport_var(const PetStore::SalesReport_var& _var) {
  CORBA::add_ref(_var._ptr);
  _ptr = _var._ptr;
}

PetStore::SalesReport_var::~SalesReport_var() { CORBA::remove_ref(_ptr); }

PetStore::SalesReport_var& PetStore::SalesReport_var::operator=(PetStore::SalesReport_ptr _p) {
  CORBA::remove_ref(_ptr);
  _ptr = _p;
  return *this;
}

void PetStore::SalesReport_var::operator=(const PetStore::SalesReport_var & _v) {
  if (_ptr)
    _remove_ref(_ptr);
  if (_v._ptr) {
    _add_ref(_v._ptr);
    _ptr = _v._ptr;
  } else
    _ptr = (PetStore::SalesReport_ptr)NULL;
}

PetStore::SalesReport_ptr& PetStore::SalesReport_var::out() {
  CORBA::remove_ref(_ptr);
  _ptr = (PetStore::SalesReport_ptr)NULL;
  return _ptr;
}

PetStore::SalesReport_ptr PetStore::SalesReport_var::_retn() {
  PetStore::SalesReport_ptr _tmp_ptr = _ptr;
  _ptr = (PetStore::SalesReport_ptr)NULL;
  return _tmp_ptr;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::SalesReport_var& _var) {
  _strm >> _var._ptr;
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::SalesReport_var& _var) {
  _strm << _var._ptr;
  return _strm;
}

Istream& PetStore::_marshal_out(Istream& _strm, PetStore::SalesReport_var& _var) {
  VISistream _istrm(_strm);
  _istrm >> _var._ptr;
  return _strm;
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::SalesReport_var& _var) {
  _strm << _var._ptr;
  return _strm;
}

PetStore::SalesReport_ptr PetStore::SalesReport::_downcast(CORBA::ValueBase* _val) {
  if (!_val)
    return (PetStore::SalesReport_ptr)NULL;

  PetStore::SalesReport_ptr retval = (PetStore::SalesReport_ptr)_val->_safe_downcast(SalesReport::_info());
  return retval;
}

const VISValueInfo& PetStore::SalesReport::_stat_info = PetStore::SalesReport::_info();
const VISValueInfo& PetStore::SalesReport::_info() {
  static VISValueInfo _val_info("IDL:PetStore/SalesReport:1.0", 0, 1,
                                "IDL:PetStore/SalesReport:1.0");
  return _val_info;
}

const VISValueInfo& PetStore::SalesReport ::_type_info() const {
  return PetStore::SalesReport::_info();
}

void *PetStore::SalesReport::_safe_downcast(const VISValueInfo &info) const {
  if (PetStore::SalesReport::_info() == info)
    return (void *) this;
  else if (Components::EventBase::_info() == info) {
    return (void*) (Components::EventBase*) this;
  }
  return (void*)NULL;
}

#if defined(MSVCNEWDLL_BUG)
void *PetStore::SalesReport::operator new(size_t ts) {
  return ::operator new(ts);
}
void PetStore::SalesReport::operator delete(void *p) {
  ::operator delete(p);
}
#endif // MSVCNEWDLL_BUG

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::SalesReport_ptr _obj) {
  VISostream _ostrm(_strm);
  if (&_strm == &Cout || &_strm == &Cerr || &_strm == &Clog) {
    _pretty_print(_ostrm, *_obj);
}
  else {
    _ostrm << _obj;
}
  return _strm;
}

Istream& PetStore::_marshal_out(Istream& _strm, PetStore::SalesReport_ptr& _obj) {
  VISistream _istrm(_strm);
  _istrm >> _obj;
  return _strm;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::SalesReport_ptr& _obj) {
  _obj = PetStore::SalesReport::_downcast(PetStore::SalesReport::_info().read(_strm));
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::SalesReport_ptr _obj) {
  PetStore::SalesReport::_info().write(_strm, _obj);
  return _strm;
}

void PetStore::_internal_pretty_print(VISostream& _strm, const PetStore::SalesReport& _obj) {
  _strm << "VALUETYPE PetStore::SalesReport {" << endl;
  if(&_obj) {
        _strm << "\torder_id():" << endl;
    _strm << "\t" << _obj.order_id() << endl;

        _strm << "\tsubTotal():" << endl;
    _strm << "\t" << _obj.subTotal() << endl;

        _strm << "\torder_state():" << endl;
    _pretty_print(_strm, _obj.order_state());
    _strm << endl;

  } else
    _strm << "NULL" << endl;
  _strm << "}" << endl;
}
void PetStore::_internal_pretty_print(VISostream& _strm, const PetStore::SalesReport* _obj) {
  _pretty_print(_strm, *_obj);
}

#if defined(__BORLANDC__)
 #pragma option push
 #pragma warn -8057
#endif

void PetStore::SalesReport::_write_state(VISostream& _ostrm) {
  CORBA::String_var _obv_order_id;
  _obv_order_id = CORBA::string_dup(order_id());
  _ostrm << _obv_order_id;
  CORBA::ULong _obv_subTotal;
  _obv_subTotal = subTotal();
  _ostrm << _obv_subTotal;
  OrderState _obv_order_state;
  _obv_order_state = order_state();
  _ostrm << (CORBA::ULong)_obv_order_state;
}
void PetStore::SalesReport::_read_state(VISistream& _istrm) {
  CORBA::String_var _obv_order_id;
  _istrm >> _obv_order_id;
  order_id(_obv_order_id);
  CORBA::ULong _obv_subTotal;
  _istrm >> _obv_subTotal;
  subTotal(_obv_subTotal);
  OrderState _obv_order_state;
  {CORBA::ULong _temp; _istrm >> _temp; _obv_order_state = (::PetStore::OrderState)_temp;};
  order_state(_obv_order_state);
}
#if defined(__BORLANDC__)
 #pragma option pop
#endif

PetStore::OBV_SalesReport::OBV_SalesReport(){}
PetStore::OBV_SalesReport::~OBV_SalesReport(){}
#if defined(MSVCNEWDLL_BUG)
void *PetStore::ProductIndexInfo::operator new(size_t ts) {
  return ::operator new(ts);
}
void PetStore::ProductIndexInfo::operator delete(void *p) {
  ::operator delete(p);
}
#endif // MSVCNEWDLL_BUG

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::ProductIndexInfo& _s) {
    _strm << _s.product_id;

    _strm << _s.product_name;

    _strm << _s.product_category;

  return _strm;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::ProductIndexInfo& _s) {
    _strm >> _s.product_id;

    _strm >> _s.product_name;

    _strm >> _s.product_category;

  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::ProductIndexInfo* _obj) {
  if (!_obj)
    VISTHROW(CORBA::BAD_PARAM())
  else
    _strm << *_obj;
  return _strm;
}
VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::ProductIndexInfo*& _obj) {
  _obj = new PetStore::ProductIndexInfo();
  _strm >> *_obj;
  return _strm;
}

void PetStore::_internal_pretty_print(VISostream& _strm, const PetStore::ProductIndexInfo& _s) {
  _strm << "STRUCT PetStore::ProductIndexInfo {" << endl;
    _strm << "\tproduct_id:" << endl;
  _strm << "\t" << _s.product_id << endl;

    _strm << "\tproduct_name:" << endl;
  _strm << "\t" << _s.product_name << endl;

    _strm << "\tproduct_category:" << endl;
  _strm << "\t" << _s.product_category << endl;

  _strm << "}" << endl;
}
Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::ProductIndexInfo& _s) {
  VISostream _ostrm(_strm);
  if (&_strm == &Cout || &_strm == &Cerr || &_strm == &Clog) {
    _pretty_print(_ostrm, _s);
    _ostrm << endl;
  } else {
      _ostrm << _s;
  }
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::ProductIndexInfo& _obj) {
  VISistream _istrm(_strm);
    _istrm >> _obj;
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::ProductIndexInfo*& _obj) {
  VISistream _istrm(_strm);
    _istrm >> _obj;
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::ProductIndexInfo_var& _var) {
  if (!_var._ptr)
    VISTHROW(CORBA::BAD_PARAM())
  else
    _strm << *_var._ptr;
  return _strm;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::ProductIndexInfo_var& _var) {
  if (!_var._ptr)
    _var._ptr = new PetStore::ProductIndexInfo;
  _strm >> *_var._ptr;
  return _strm;
}

PetStore::ProductIndexInfo_var& PetStore::ProductIndexInfo_var::operator=(PetStore::ProductIndexInfo* _p) {
  delete _ptr;
  _ptr = _p;
  return *this;
}

PetStore::ProductIndexInfo_var& PetStore::ProductIndexInfo_var::operator=(const PetStore::ProductIndexInfo_var& _var) {
  delete _ptr;
  _ptr = _var._ptr ? new PetStore::ProductIndexInfo(*_var._ptr) : 
  (PetStore::ProductIndexInfo*)NULL;
  return *this;
}

#if defined(MSVCNEWDLL_BUG)
void *PetStore::ProductIndexInfoList::operator new(size_t ts) {
  return ::operator new(ts);
}
void PetStore::ProductIndexInfoList::operator delete(void *p) {
  ::operator delete(p);
}
#endif // MSVCNEWDLL_BUG

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::ProductIndexInfoList& _seq) {
  VISostream _ostrm(_strm);
  if (&_strm == &Cout || &_strm == &Cerr || &_strm == &Clog) {
    _pretty_print(_ostrm, _seq);
    _ostrm << endl;
  } else {
    _ostrm << _seq;
  }
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::ProductIndexInfoList& _obj) {
  VISistream _istrm(_strm);
  _istrm >> _obj;
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::ProductIndexInfoList_ptr& _ptr) {
  VISistream _istrm(_strm);
  _istrm >> _ptr;
  return _strm;
}

::PetStore::ProductIndexInfo* PetStore::ProductIndexInfoList::allocbuf(CORBA::ULong _nelems) {
  if (_nelems) {
    return new ::PetStore::ProductIndexInfo[_nelems];
  }
  return (::PetStore::ProductIndexInfo *)NULL;
}

void PetStore::ProductIndexInfoList::freebuf(::PetStore::ProductIndexInfo *_data) {
  if (_data)
    delete[] _data;
}

PetStore::ProductIndexInfoList::ProductIndexInfoList(CORBA::ULong _max) {
  _num_allocated = _max;
  _count = 0;
  _contents = allocbuf(_num_allocated);
  _release_flag = 1;
}

PetStore::ProductIndexInfoList::ProductIndexInfoList(CORBA::ULong _max,
                                                     CORBA::ULong _len,
                                                     ::PetStore::ProductIndexInfo *_data,
                                                     CORBA::Boolean _release) {
  _num_allocated = _max;
  _count = _len;
  _contents = _data;
  _release_flag = _release;
}

PetStore::ProductIndexInfoList::ProductIndexInfoList(const PetStore::ProductIndexInfoList& _seq) {
  _num_allocated = _seq._num_allocated;
  _count = _seq._count;
  _contents = allocbuf(_num_allocated);
  if (_num_allocated > 0) {
    for (CORBA::ULong i=0; i < _count; i++) {
      _contents[i] = _seq._contents[i];
    }
  }
  _release_flag = 1;
}

PetStore::ProductIndexInfoList::~ProductIndexInfoList() {
  if (_release_flag) 
    freebuf(_contents);
}

PetStore::ProductIndexInfoList& PetStore::ProductIndexInfoList::operator=(const PetStore::ProductIndexInfoList& _seq) {
  if (_num_allocated < _seq._count) {
    if (_release_flag) 
      freebuf(_contents);
    else
      _release_flag = (CORBA::Boolean) 1;

    _contents = allocbuf(_seq._num_allocated);
    _num_allocated = _seq._num_allocated;
  }
  _count = _seq._count;
  if (_num_allocated > 0) {
    for (CORBA::ULong i = 0; i < _count; i++) {
      _contents[i] = _seq._contents[i];
    }
  }
  return *this;
}

void PetStore::ProductIndexInfoList::length(CORBA::ULong _newlen) {
  if (_newlen > _num_allocated) {
    ::PetStore::ProductIndexInfo *temp = allocbuf(_newlen);
    for (CORBA::ULong i=0; i < _count; i++) {
      temp[i] = _contents[i];
    }
    if (_release_flag) 
      freebuf(_contents);
    else
      _release_flag = (CORBA::Boolean) 1;

    _num_allocated = _newlen;
    _contents = temp;
  }
  _count = _newlen;
}

void PetStore::ProductIndexInfoList::replace(CORBA::ULong _max,
                                             CORBA::ULong _len,
                                             ProductIndexInfo *_data,
                                             CORBA::Boolean _release) {
  if (_release_flag) 
    freebuf(_contents);
  _num_allocated = _max;
  _count = _len;
  _contents = _data;
  _release_flag = _release;
}

::PetStore::ProductIndexInfo *PetStore::ProductIndexInfoList::get_buffer(CORBA::Boolean orphan) {
  if (orphan == (CORBA::Boolean) 1) {
    if (_release_flag == (CORBA::Boolean) 0)
      return (::PetStore::ProductIndexInfo*)NULL;
    else {
      _release_flag = (CORBA::Boolean) 0;
      return _contents;
    }
  } else
    return _contents;
}

const ::PetStore::ProductIndexInfo* PetStore::ProductIndexInfoList::get_buffer() const {
  // Do not use const_cast here because it might not be supported by the compiler
  return (const ::PetStore::ProductIndexInfo*)(void*)_contents;
}

::PetStore::ProductIndexInfo& PetStore::ProductIndexInfoList::operator[](CORBA::ULong _index) {
  if (_index >= _count) {
    VISTHROW(CORBA::BAD_PARAM())
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(static ::PetStore::ProductIndexInfo *_null = 0; return *_null;)
#endif
  }
  return _contents[_index];
}

const ::PetStore::ProductIndexInfo& PetStore::ProductIndexInfoList::operator[](CORBA::ULong _index) const {
  if (_index >= _count) {
    VISTHROW(CORBA::BAD_PARAM())
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(static ::PetStore::ProductIndexInfo *_null = 0; return (const ::PetStore::ProductIndexInfo&)*_null;)
#endif
  }
  // Do not use const_cast here because it might not be supported by the compiler
  return *(const ::PetStore::ProductIndexInfo*)(void*)&_contents[_index];
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::ProductIndexInfoList& _seq) {
  _strm << _seq._count;
  for (CORBA::ULong i=0; i < _seq._count; i++) {
    _strm << _seq._contents[i];
  }
  return _strm;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::ProductIndexInfoList& _seq) {
  CORBA::ULong t_count;
  _strm >> t_count;

  if (t_count > _seq._count) {
    if (_seq._num_allocated < t_count) {
      if (_seq._release_flag)
        PetStore::ProductIndexInfoList::freebuf(_seq._contents);
      else
        _seq._release_flag = (CORBA::Boolean) 1;
      _seq._contents = PetStore::ProductIndexInfoList::allocbuf(t_count);
      _seq._num_allocated = t_count;
    }
  }
  _seq._count = t_count;

  for (CORBA::ULong i=0; i < _seq._count; i++) {
    _strm >> _seq._contents[i];
  }
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::ProductIndexInfoList* _obj) {
  if (!_obj)
    VISTHROW(CORBA::BAD_PARAM())
  else
    _strm << *_obj;
  return _strm;
}
VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::ProductIndexInfoList_ptr& _obj) {
  _obj = new PetStore::ProductIndexInfoList();
  _strm >> *_obj;
  return _strm;
}

void PetStore::_internal_pretty_print(VISostream& _strm, const PetStore::ProductIndexInfoList& _seq) {
  _strm << "SEQUENCE PetStore::ProductIndexInfoList" << endl;
  _strm << "Number of Elements: " << _seq._count << endl;
  for (CORBA::ULong i=0; i < _seq._count; i++) {
    _strm << "[" << i << "]" << endl;
    _strm << "\t_contents[i]:" << endl;
    _pretty_print(_strm, _seq._contents[i]);
    _strm << endl;
    _strm << endl;
  }
}
::PetStore::ProductIndexInfo& PetStore::ProductIndexInfoList_var::operator[](CORBA::ULong _index) {
  if (_ptr)
    return (*_ptr)[_index];
  static PetStore::ProductIndexInfoList _tmp;
  return _tmp[_index];
}

const ::PetStore::ProductIndexInfo& PetStore::ProductIndexInfoList_var::operator[](CORBA::ULong _index) const {
  if (_ptr)
    return (*(const PetStore::ProductIndexInfoList*)_ptr)[_index];
  static const PetStore::ProductIndexInfoList _tmp;
  return _tmp[_index];
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::ProductIndexInfoList_var& _var) {
  if (!_var._ptr)
    VISTHROW(CORBA::BAD_PARAM())
  else
    _strm << *_var._ptr;
  return _strm;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::ProductIndexInfoList_var& _var) {
  if (!_var._ptr)
    _var._ptr = new PetStore::ProductIndexInfoList;
  _strm >> *_var._ptr;
  return _strm;
}
Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::ProductCategory& _enum_type) {
  VISostream _ostrm(_strm);
  if (&_strm == &Cout || &_strm == &Cerr || &_strm == &Clog) {
    _pretty_print(_ostrm, _enum_type);
    _ostrm << endl;
  } else {
      _ostrm << (CORBA::ULong)_enum_type;
  }
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::ProductCategory& _enum_type) {
  VISistream _istrm(_strm);
    {CORBA::ULong _temp; _istrm >> _temp; _enum_type = (::PetStore::ProductCategory)_temp;};
  return _strm;
}
VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::ProductCategory _enum_type) {
  _strm << (CORBA::ULong)_enum_type;
  return _strm;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::ProductCategory& _enum_type) {
  CORBA::ULong _temp;
  _strm >> _temp;
  if (_temp >= 4) {
    VISTHROW(CORBA::BAD_PARAM())
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return _strm;)
#endif
  }
  _enum_type = (PetStore::ProductCategory) _temp;
  return _strm;
}
void PetStore::_internal_pretty_print(VISostream& _strm, PetStore::ProductCategory _val) {
  _strm << "ENUM PetStore::ProductCategory::";
  switch (_val) {
    case PetStore::DOG: _strm << "DOG" << endl; break;
    case PetStore::CAT: _strm << "CAT" << endl; break;
    case PetStore::FISH: _strm << "FISH" << endl; break;
    case PetStore::BIRD: _strm << "BIRD" << endl; break;
  }
}
#if defined(MSVCNEWDLL_BUG)
void *PetStore::ProductInfo::operator new(size_t ts) {
  return ::operator new(ts);
}
void PetStore::ProductInfo::operator delete(void *p) {
  ::operator delete(p);
}
#endif // MSVCNEWDLL_BUG

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::ProductInfo& _s) {
    _strm << _s.product_id;

    _strm << _s.product_name;

    _strm << _s.product_picture;

    _strm << _s.product_category;

    _strm << _s.product_price;

    _strm << _s.product_qty;

    _strm << _s.product_description;

  return _strm;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::ProductInfo& _s) {
    _strm >> _s.product_id;

    _strm >> _s.product_name;

    _strm >> _s.product_picture;

    _strm >> _s.product_category;

    _strm >> _s.product_price;

    _strm >> _s.product_qty;

    _strm >> _s.product_description;

  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::ProductInfo* _obj) {
  if (!_obj)
    VISTHROW(CORBA::BAD_PARAM())
  else
    _strm << *_obj;
  return _strm;
}
VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::ProductInfo*& _obj) {
  _obj = new PetStore::ProductInfo();
  _strm >> *_obj;
  return _strm;
}

void PetStore::_internal_pretty_print(VISostream& _strm, const PetStore::ProductInfo& _s) {
  _strm << "STRUCT PetStore::ProductInfo {" << endl;
    _strm << "\tproduct_id:" << endl;
  _strm << "\t" << _s.product_id << endl;

    _strm << "\tproduct_name:" << endl;
  _strm << "\t" << _s.product_name << endl;

    _strm << "\tproduct_picture:" << endl;
  _strm << "\t" << _s.product_picture << endl;

    _strm << "\tproduct_category:" << endl;
  _strm << "\t" << _s.product_category << endl;

    _strm << "\tproduct_price:" << endl;
  _strm << "\t" << _s.product_price << endl;

    _strm << "\tproduct_qty:" << endl;
  _strm << "\t" << _s.product_qty << endl;

    _strm << "\tproduct_description:" << endl;
  _strm << "\t" << _s.product_description << endl;

  _strm << "}" << endl;
}
Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::ProductInfo& _s) {
  VISostream _ostrm(_strm);
  if (&_strm == &Cout || &_strm == &Cerr || &_strm == &Clog) {
    _pretty_print(_ostrm, _s);
    _ostrm << endl;
  } else {
      _ostrm << _s;
  }
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::ProductInfo& _obj) {
  VISistream _istrm(_strm);
    _istrm >> _obj;
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::ProductInfo*& _obj) {
  VISistream _istrm(_strm);
    _istrm >> _obj;
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::ProductInfo_var& _var) {
  if (!_var._ptr)
    VISTHROW(CORBA::BAD_PARAM())
  else
    _strm << *_var._ptr;
  return _strm;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::ProductInfo_var& _var) {
  if (!_var._ptr)
    _var._ptr = new PetStore::ProductInfo;
  _strm >> *_var._ptr;
  return _strm;
}

PetStore::ProductInfo_var& PetStore::ProductInfo_var::operator=(PetStore::ProductInfo* _p) {
  delete _ptr;
  _ptr = _p;
  return *this;
}

PetStore::ProductInfo_var& PetStore::ProductInfo_var::operator=(const PetStore::ProductInfo_var& _var) {
  delete _ptr;
  _ptr = _var._ptr ? new PetStore::ProductInfo(*_var._ptr) : 
  (PetStore::ProductInfo*)NULL;
  return *this;
}
#if defined(MSVCNEWDLL_BUG)
void *PetStore::OrderItemInfo::operator new(size_t ts) {
  return ::operator new(ts);
}
void PetStore::OrderItemInfo::operator delete(void *p) {
  ::operator delete(p);
}
#endif // MSVCNEWDLL_BUG

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::OrderItemInfo& _s) {
    _strm << _s.orderItem_id;

    _strm << _s.product_id;

    _strm << _s.product_name;

    _strm << _s.quantity;

    _strm << _s.unit_price;

    _strm << _s.subTotal;

  return _strm;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::OrderItemInfo& _s) {
    _strm >> _s.orderItem_id;

    _strm >> _s.product_id;

    _strm >> _s.product_name;

    _strm >> _s.quantity;

    _strm >> _s.unit_price;

    _strm >> _s.subTotal;

  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::OrderItemInfo* _obj) {
  if (!_obj)
    VISTHROW(CORBA::BAD_PARAM())
  else
    _strm << *_obj;
  return _strm;
}
VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::OrderItemInfo*& _obj) {
  _obj = new PetStore::OrderItemInfo();
  _strm >> *_obj;
  return _strm;
}

void PetStore::_internal_pretty_print(VISostream& _strm, const PetStore::OrderItemInfo& _s) {
  _strm << "STRUCT PetStore::OrderItemInfo {" << endl;
    _strm << "\torderItem_id:" << endl;
  _strm << "\t" << _s.orderItem_id << endl;

    _strm << "\tproduct_id:" << endl;
  _strm << "\t" << _s.product_id << endl;

    _strm << "\tproduct_name:" << endl;
  _strm << "\t" << _s.product_name << endl;

    _strm << "\tquantity:" << endl;
  _strm << "\t" << _s.quantity << endl;

    _strm << "\tunit_price:" << endl;
  _strm << "\t" << _s.unit_price << endl;

    _strm << "\tsubTotal:" << endl;
  _strm << "\t" << _s.subTotal << endl;

  _strm << "}" << endl;
}
Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::OrderItemInfo& _s) {
  VISostream _ostrm(_strm);
  if (&_strm == &Cout || &_strm == &Cerr || &_strm == &Clog) {
    _pretty_print(_ostrm, _s);
    _ostrm << endl;
  } else {
      _ostrm << _s;
  }
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::OrderItemInfo& _obj) {
  VISistream _istrm(_strm);
    _istrm >> _obj;
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::OrderItemInfo*& _obj) {
  VISistream _istrm(_strm);
    _istrm >> _obj;
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::OrderItemInfo_var& _var) {
  if (!_var._ptr)
    VISTHROW(CORBA::BAD_PARAM())
  else
    _strm << *_var._ptr;
  return _strm;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::OrderItemInfo_var& _var) {
  if (!_var._ptr)
    _var._ptr = new PetStore::OrderItemInfo;
  _strm >> *_var._ptr;
  return _strm;
}

PetStore::OrderItemInfo_var& PetStore::OrderItemInfo_var::operator=(PetStore::OrderItemInfo* _p) {
  delete _ptr;
  _ptr = _p;
  return *this;
}

PetStore::OrderItemInfo_var& PetStore::OrderItemInfo_var::operator=(const PetStore::OrderItemInfo_var& _var) {
  delete _ptr;
  _ptr = _var._ptr ? new PetStore::OrderItemInfo(*_var._ptr) : 
  (PetStore::OrderItemInfo*)NULL;
  return *this;
}

#if defined(MSVCNEWDLL_BUG)
void *PetStore::OrderItemInfoList::operator new(size_t ts) {
  return ::operator new(ts);
}
void PetStore::OrderItemInfoList::operator delete(void *p) {
  ::operator delete(p);
}
#endif // MSVCNEWDLL_BUG

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::OrderItemInfoList& _seq) {
  VISostream _ostrm(_strm);
  if (&_strm == &Cout || &_strm == &Cerr || &_strm == &Clog) {
    _pretty_print(_ostrm, _seq);
    _ostrm << endl;
  } else {
    _ostrm << _seq;
  }
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::OrderItemInfoList& _obj) {
  VISistream _istrm(_strm);
  _istrm >> _obj;
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::OrderItemInfoList_ptr& _ptr) {
  VISistream _istrm(_strm);
  _istrm >> _ptr;
  return _strm;
}

::PetStore::OrderItemInfo* PetStore::OrderItemInfoList::allocbuf(CORBA::ULong _nelems) {
  if (_nelems) {
    return new ::PetStore::OrderItemInfo[_nelems];
  }
  return (::PetStore::OrderItemInfo *)NULL;
}

void PetStore::OrderItemInfoList::freebuf(::PetStore::OrderItemInfo *_data) {
  if (_data)
    delete[] _data;
}

PetStore::OrderItemInfoList::OrderItemInfoList(CORBA::ULong _max) {
  _num_allocated = _max;
  _count = 0;
  _contents = allocbuf(_num_allocated);
  _release_flag = 1;
}

PetStore::OrderItemInfoList::OrderItemInfoList(CORBA::ULong _max,
                                               CORBA::ULong _len,
                                               ::PetStore::OrderItemInfo *_data,
                                               CORBA::Boolean _release) {
  _num_allocated = _max;
  _count = _len;
  _contents = _data;
  _release_flag = _release;
}

PetStore::OrderItemInfoList::OrderItemInfoList(const PetStore::OrderItemInfoList& _seq) {
  _num_allocated = _seq._num_allocated;
  _count = _seq._count;
  _contents = allocbuf(_num_allocated);
  if (_num_allocated > 0) {
    for (CORBA::ULong i=0; i < _count; i++) {
      _contents[i] = _seq._contents[i];
    }
  }
  _release_flag = 1;
}

PetStore::OrderItemInfoList::~OrderItemInfoList() {
  if (_release_flag) 
    freebuf(_contents);
}

PetStore::OrderItemInfoList& PetStore::OrderItemInfoList::operator=(const PetStore::OrderItemInfoList& _seq) {
  if (_num_allocated < _seq._count) {
    if (_release_flag) 
      freebuf(_contents);
    else
      _release_flag = (CORBA::Boolean) 1;

    _contents = allocbuf(_seq._num_allocated);
    _num_allocated = _seq._num_allocated;
  }
  _count = _seq._count;
  if (_num_allocated > 0) {
    for (CORBA::ULong i = 0; i < _count; i++) {
      _contents[i] = _seq._contents[i];
    }
  }
  return *this;
}

void PetStore::OrderItemInfoList::length(CORBA::ULong _newlen) {
  if (_newlen > _num_allocated) {
    ::PetStore::OrderItemInfo *temp = allocbuf(_newlen);
    for (CORBA::ULong i=0; i < _count; i++) {
      temp[i] = _contents[i];
    }
    if (_release_flag) 
      freebuf(_contents);
    else
      _release_flag = (CORBA::Boolean) 1;

    _num_allocated = _newlen;
    _contents = temp;
  }
  _count = _newlen;
}

void PetStore::OrderItemInfoList::replace(CORBA::ULong _max,
                                          CORBA::ULong _len,
                                          OrderItemInfo *_data,
                                          CORBA::Boolean _release) {
  if (_release_flag) 
    freebuf(_contents);
  _num_allocated = _max;
  _count = _len;
  _contents = _data;
  _release_flag = _release;
}

::PetStore::OrderItemInfo *PetStore::OrderItemInfoList::get_buffer(CORBA::Boolean orphan) {
  if (orphan == (CORBA::Boolean) 1) {
    if (_release_flag == (CORBA::Boolean) 0)
      return (::PetStore::OrderItemInfo*)NULL;
    else {
      _release_flag = (CORBA::Boolean) 0;
      return _contents;
    }
  } else
    return _contents;
}

const ::PetStore::OrderItemInfo* PetStore::OrderItemInfoList::get_buffer() const {
  // Do not use const_cast here because it might not be supported by the compiler
  return (const ::PetStore::OrderItemInfo*)(void*)_contents;
}

::PetStore::OrderItemInfo& PetStore::OrderItemInfoList::operator[](CORBA::ULong _index) {
  if (_index >= _count) {
    VISTHROW(CORBA::BAD_PARAM())
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(static ::PetStore::OrderItemInfo *_null = 0; return *_null;)
#endif
  }
  return _contents[_index];
}

const ::PetStore::OrderItemInfo& PetStore::OrderItemInfoList::operator[](CORBA::ULong _index) const {
  if (_index >= _count) {
    VISTHROW(CORBA::BAD_PARAM())
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(static ::PetStore::OrderItemInfo *_null = 0; return (const ::PetStore::OrderItemInfo&)*_null;)
#endif
  }
  // Do not use const_cast here because it might not be supported by the compiler
  return *(const ::PetStore::OrderItemInfo*)(void*)&_contents[_index];
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::OrderItemInfoList& _seq) {
  _strm << _seq._count;
  for (CORBA::ULong i=0; i < _seq._count; i++) {
    _strm << _seq._contents[i];
  }
  return _strm;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::OrderItemInfoList& _seq) {
  CORBA::ULong t_count;
  _strm >> t_count;

  if (t_count > _seq._count) {
    if (_seq._num_allocated < t_count) {
      if (_seq._release_flag)
        PetStore::OrderItemInfoList::freebuf(_seq._contents);
      else
        _seq._release_flag = (CORBA::Boolean) 1;
      _seq._contents = PetStore::OrderItemInfoList::allocbuf(t_count);
      _seq._num_allocated = t_count;
    }
  }
  _seq._count = t_count;

  for (CORBA::ULong i=0; i < _seq._count; i++) {
    _strm >> _seq._contents[i];
  }
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::OrderItemInfoList* _obj) {
  if (!_obj)
    VISTHROW(CORBA::BAD_PARAM())
  else
    _strm << *_obj;
  return _strm;
}
VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::OrderItemInfoList_ptr& _obj) {
  _obj = new PetStore::OrderItemInfoList();
  _strm >> *_obj;
  return _strm;
}

void PetStore::_internal_pretty_print(VISostream& _strm, const PetStore::OrderItemInfoList& _seq) {
  _strm << "SEQUENCE PetStore::OrderItemInfoList" << endl;
  _strm << "Number of Elements: " << _seq._count << endl;
  for (CORBA::ULong i=0; i < _seq._count; i++) {
    _strm << "[" << i << "]" << endl;
    _strm << "\t_contents[i]:" << endl;
    _pretty_print(_strm, _seq._contents[i]);
    _strm << endl;
    _strm << endl;
  }
}
::PetStore::OrderItemInfo& PetStore::OrderItemInfoList_var::operator[](CORBA::ULong _index) {
  if (_ptr)
    return (*_ptr)[_index];
  static PetStore::OrderItemInfoList _tmp;
  return _tmp[_index];
}

const ::PetStore::OrderItemInfo& PetStore::OrderItemInfoList_var::operator[](CORBA::ULong _index) const {
  if (_ptr)
    return (*(const PetStore::OrderItemInfoList*)_ptr)[_index];
  static const PetStore::OrderItemInfoList _tmp;
  return _tmp[_index];
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::OrderItemInfoList_var& _var) {
  if (!_var._ptr)
    VISTHROW(CORBA::BAD_PARAM())
  else
    _strm << *_var._ptr;
  return _strm;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::OrderItemInfoList_var& _var) {
  if (!_var._ptr)
    _var._ptr = new PetStore::OrderItemInfoList;
  _strm >> *_var._ptr;
  return _strm;
}
#if defined(MSVCNEWDLL_BUG)
void *PetStore::OrderInfo::operator new(size_t ts) {
  return ::operator new(ts);
}
void PetStore::OrderInfo::operator delete(void *p) {
  ::operator delete(p);
}
#endif // MSVCNEWDLL_BUG

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::OrderInfo& _s) {
    _strm << _s.order_id;

    _strm << _s.user_name;

    _strm << _s.user_address;

    _strm << _s.order_time;

    _strm << _s.order_items;

    _strm << _s.subTotal;

  return _strm;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::OrderInfo& _s) {
    _strm >> _s.order_id;

    _strm >> _s.user_name;

    _strm >> _s.user_address;

    _strm >> _s.order_time;

    _strm >> _s.order_items;

    _strm >> _s.subTotal;

  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::OrderInfo* _obj) {
  if (!_obj)
    VISTHROW(CORBA::BAD_PARAM())
  else
    _strm << *_obj;
  return _strm;
}
VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::OrderInfo*& _obj) {
  _obj = new PetStore::OrderInfo();
  _strm >> *_obj;
  return _strm;
}

void PetStore::_internal_pretty_print(VISostream& _strm, const PetStore::OrderInfo& _s) {
  _strm << "STRUCT PetStore::OrderInfo {" << endl;
    _strm << "\torder_id:" << endl;
  _strm << "\t" << _s.order_id << endl;

    _strm << "\tuser_name:" << endl;
  _strm << "\t" << _s.user_name << endl;

    _strm << "\tuser_address:" << endl;
  _strm << "\t" << _s.user_address << endl;

    _strm << "\torder_time:" << endl;
  _strm << "\t" << _s.order_time << endl;

    _strm << "\torder_items:" << endl;
  _strm << "\torder_items:" << endl;
  _pretty_print(_strm, _s.order_items);
  _strm << endl;

    _strm << "\tsubTotal:" << endl;
  _strm << "\t" << _s.subTotal << endl;

  _strm << "}" << endl;
}
Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::OrderInfo& _s) {
  VISostream _ostrm(_strm);
  if (&_strm == &Cout || &_strm == &Cerr || &_strm == &Clog) {
    _pretty_print(_ostrm, _s);
    _ostrm << endl;
  } else {
      _ostrm << _s;
  }
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::OrderInfo& _obj) {
  VISistream _istrm(_strm);
    _istrm >> _obj;
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::OrderInfo*& _obj) {
  VISistream _istrm(_strm);
    _istrm >> _obj;
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::OrderInfo_var& _var) {
  if (!_var._ptr)
    VISTHROW(CORBA::BAD_PARAM())
  else
    _strm << *_var._ptr;
  return _strm;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::OrderInfo_var& _var) {
  if (!_var._ptr)
    _var._ptr = new PetStore::OrderInfo;
  _strm >> *_var._ptr;
  return _strm;
}

PetStore::OrderInfo_var& PetStore::OrderInfo_var::operator=(PetStore::OrderInfo* _p) {
  delete _ptr;
  _ptr = _p;
  return *this;
}

PetStore::OrderInfo_var& PetStore::OrderInfo_var::operator=(const PetStore::OrderInfo_var& _var) {
  delete _ptr;
  _ptr = _var._ptr ? new PetStore::OrderInfo(*_var._ptr) : 
  (PetStore::OrderInfo*)NULL;
  return *this;
}

PetStore::BusinessOp_ptr PetStore::BusinessOp_var::_duplicate(PetStore::BusinessOp_ptr _p) {
  return PetStore::BusinessOp::_duplicate(_p);
}

void
PetStore::BusinessOp_var::_release(PetStore::BusinessOp_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_p);
}

PetStore::BusinessOp_var::BusinessOp_var()
  : _ptr(PetStore::BusinessOp::_nil()) {}

PetStore::BusinessOp_var::BusinessOp_var(PetStore::BusinessOp_ptr _p)
  : _ptr(_p) {}

PetStore::BusinessOp_var::BusinessOp_var(const PetStore::BusinessOp_var& _var)
  : _ptr(PetStore::BusinessOp::_duplicate((PetStore::BusinessOp_ptr) _var)) {}

PetStore::BusinessOp_var::~BusinessOp_var() {
  CORBA::release((CORBA::Object_ptr)_ptr);
}

PetStore::BusinessOp_var&
PetStore::BusinessOp_var::operator=(const PetStore::BusinessOp_var& _var) {
    _release(_ptr);
    _ptr = _duplicate(_var._ptr);
    return *this;
}

PetStore::BusinessOp_var&
PetStore::BusinessOp_var::operator=(PetStore::BusinessOp_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_ptr);
  _ptr = _p;
  return *this;
}

PetStore::BusinessOp_ptr& PetStore::BusinessOp_var::out() {
  _release(_ptr);
  _ptr = (PetStore::BusinessOp_ptr)NULL;
  return _ptr;
}

PetStore::BusinessOp_ptr PetStore::BusinessOp_var::_retn() {
  PetStore::BusinessOp_ptr _tmp_ptr = _ptr;
  _ptr = (PetStore::BusinessOp_ptr)NULL;
  return _tmp_ptr;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::BusinessOp_var& _var) {
  _strm >> _var._ptr;
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::BusinessOp_var& _var) {
  _strm << _var._ptr;
  return _strm;
}

Istream& PetStore::_marshal_out(Istream& _strm, PetStore::BusinessOp_var& _var) {
  VISistream _istrm(_strm);
  _istrm >> _var._ptr;
  return _strm;
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::BusinessOp_var& _var) {
  _strm << (CORBA::Object_ptr)_var._ptr;
  return _strm;
}

// If dllimport is specified, you might probably want to
// disable these definitions
// disable the const definitions
const VISOps_Info PetStore::BusinessOp_ops::_ops_info("PetStore::BusinessOp_ops");
const VISOps_Info *PetStore::BusinessOp_ops::_desc() { return &_ops_info; }
PetStore::BusinessOp_ops_ptr PetStore::BusinessOp_ops::_downcast(PortableServer::ServantBase* _servant) {
  if (_servant == (PortableServer::ServantBase*)NULL)
    return PetStore::BusinessOp_ops::_nil();
  return (PetStore::BusinessOp_ops_ptr)_servant->_safe_downcast_ops(_ops_info);
}
const CORBA::TypeInfo
#ifdef AIXV3
PetStore::BusinessOp::_class_info("PetStore::BusinessOp",
                                  "IDL:PetStore/BusinessOp:1.0", NULL,
                                  PetStore::BusinessOp::_factory, NULL, 0,
                                  NULL, 0, CORBA::Object::_desc(), 0L);
#else
PetStore::BusinessOp::_class_info("PetStore::BusinessOp",
                                  "IDL:PetStore/BusinessOp:1.0", NULL,
                                  PetStore::BusinessOp::_factory, NULL, 0,
                                  NULL, 0, CORBA::Object::_desc(), 0L);
#endif

const CORBA::TypeInfo *PetStore::BusinessOp::_desc() { return &_class_info; }
const CORBA::TypeInfo *PetStore::BusinessOp::_type_info() const {
  return &_class_info;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::BusinessOp_ptr& _obj) {
  CORBA::Object_var _var_obj(_obj);
  _var_obj = CORBA::Object::_read(_strm, PetStore::BusinessOp::_desc());
  _obj = PetStore::BusinessOp::_narrow(_var_obj);
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::BusinessOp_ptr _obj) {
  _strm << (CORBA::Object_ptr)_obj;
  return _strm;
}
void* PetStore::BusinessOp::_safe_narrow(const CORBA::TypeInfo& _info) const {
  if (_info == _class_info)
    return (void *)this;
  return CORBA_Object::_safe_narrow(_info);
}

CORBA::Object *PetStore::BusinessOp::_factory() {
  return new PetStore::BusinessOp;
}

PetStore::BusinessOp_ptr PetStore::BusinessOp::_this() {
  return PetStore::BusinessOp::_duplicate(this);
}

PetStore::BusinessOp_ptr PetStore::BusinessOp::_narrow(CORBA::Object * _obj) {
  if (_obj == PetStore::BusinessOp::_nil())
    return PetStore::BusinessOp::_nil();
  else
    return PetStore::BusinessOp::_duplicate((PetStore::BusinessOp_ptr)_obj->_safe_narrow(_class_info));
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::BusinessOp_ptr _obj) {
  _strm << (CORBA::Object_ptr) _obj;
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::BusinessOp_ptr& _obj) {
  VISistream _istrm(_strm);
  _istrm >> _obj;
  return _strm;
}

PetStore::BusinessOp *PetStore::BusinessOp::_bind(const char *_object_name,
                                                  const char *_host_name,
                                                  const CORBA::BindOptions *_opt,
                                                  CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
          "IDL:PetStore/BusinessOp:1.0", _object_name, _host_name, _opt, _orb);
  return BusinessOp::_narrow(_obj);
}

PetStore::BusinessOp *PetStore::BusinessOp::_bind(const char *_poa_name,
                                                  const CORBA::OctetSequence& _id,
                                                  const char *_host_name,
                                                  const CORBA::BindOptions *_opt,
                                                  CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
   "IDL:PetStore/BusinessOp:1.0", _poa_name, _id, _host_name, _opt, _orb);
  return BusinessOp::_narrow(_obj);
}
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
::PetStore::ProductIndexInfoList* PetStore::BusinessOp::listAllProducts() {
  ::PetStore::ProductIndexInfoList* _ret = (::PetStore::ProductIndexInfoList_ptr) NULL;
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("listAllProducts");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::BusinessOp_ops*  _businessop = PetStore::BusinessOp_ops::_downcast(_servant);
      if (!_businessop) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "listAllProducts");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }

      VISTRY {
         _ret = _businessop->listAllProducts();
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "listAllProducts"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "listAllProducts");
      return _ret;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("listAllProducts", 1);
    VISIF_EXCEP(return _ret;)

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return _ret;)
#endif
  }

  VISistream& _istrm = *VISistream::_downcast(_ibuf);
  _istrm >> _ret;
  return _ret;
}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
::PetStore::ProductIndexInfoList* PetStore::BusinessOp::listProductByCategory(::PetStore::ProductCategory _arg_category ) {
  ::PetStore::ProductIndexInfoList* _ret = (::PetStore::ProductIndexInfoList_ptr) NULL;
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("listProductByCategory");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::BusinessOp_ops*  _businessop = PetStore::BusinessOp_ops::_downcast(_servant);
      if (!_businessop) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "listProductByCategory");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }

      VISTRY {
         _ret = _businessop->listProductByCategory(_arg_category );
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "listProductByCategory"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "listProductByCategory");
      return _ret;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("listProductByCategory", 1);
    VISIF_EXCEP(return _ret;)
    VISostream& _ostrm = *VISostream::_downcast(_obuf);
    _ostrm << (CORBA::ULong)_arg_category;

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return _ret;)
#endif
  }

  VISistream& _istrm = *VISistream::_downcast(_ibuf);
  _istrm >> _ret;
  return _ret;
}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
::PetStore::ProductInfo* PetStore::BusinessOp::listProduct(const char* _arg_productID ) {
  ::PetStore::ProductInfo* _ret = (::PetStore::ProductInfo_ptr) NULL;
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("listProduct");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::BusinessOp_ops*  _businessop = PetStore::BusinessOp_ops::_downcast(_servant);
      if (!_businessop) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "listProduct");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }

      VISTRY {
         _ret = _businessop->listProduct(_arg_productID );
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "listProduct"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "listProduct");
      return _ret;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("listProduct", 1);
    VISIF_EXCEP(return _ret;)
    VISostream& _ostrm = *VISostream::_downcast(_obuf);
    _ostrm << _arg_productID;

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return _ret;)
#endif
  }

  VISistream& _istrm = *VISistream::_downcast(_ibuf);
  _istrm >> _ret;
  return _ret;
}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
void PetStore::BusinessOp::placeInCart(const ::PetStore::SalesRecordInfo& _arg_productWanted ) {
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("placeInCart");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::BusinessOp_ops*  _businessop = PetStore::BusinessOp_ops::_downcast(_servant);
      if (!_businessop) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "placeInCart");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }

      VISTRY {
        _businessop->placeInCart(_arg_productWanted );
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "placeInCart"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "placeInCart");
      return;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("placeInCart", 1);
    VISIF_EXCEP(return;)
    VISostream& _ostrm = *VISostream::_downcast(_obuf);
    _ostrm << _arg_productWanted;

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return;)
#endif
  }

}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
void PetStore::BusinessOp::updateQtyInCart(const char* _arg_productID ,
                                           CORBA::Long _arg_newQty ) {
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("updateQtyInCart");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::BusinessOp_ops*  _businessop = PetStore::BusinessOp_ops::_downcast(_servant);
      if (!_businessop) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "updateQtyInCart");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }

      VISTRY {
        _businessop->updateQtyInCart(_arg_productID , _arg_newQty );
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "updateQtyInCart"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "updateQtyInCart");
      return;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("updateQtyInCart", 1);
    VISIF_EXCEP(return;)
    VISostream& _ostrm = *VISostream::_downcast(_obuf);
    _ostrm << _arg_productID;
    _ostrm << _arg_newQty;

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return;)
#endif
  }

}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
void PetStore::BusinessOp::removeFromCart(const char* _arg_productID ) {
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("removeFromCart");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::BusinessOp_ops*  _businessop = PetStore::BusinessOp_ops::_downcast(_servant);
      if (!_businessop) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "removeFromCart");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }

      VISTRY {
        _businessop->removeFromCart(_arg_productID );
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "removeFromCart"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "removeFromCart");
      return;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("removeFromCart", 1);
    VISIF_EXCEP(return;)
    VISostream& _ostrm = *VISostream::_downcast(_obuf);
    _ostrm << _arg_productID;

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return;)
#endif
  }

}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
::PetStore::SalesRecord* PetStore::BusinessOp::displayCart() {
  ::PetStore::SalesRecord* _ret = (::PetStore::SalesRecord_ptr) NULL;
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("displayCart");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::BusinessOp_ops*  _businessop = PetStore::BusinessOp_ops::_downcast(_servant);
      if (!_businessop) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "displayCart");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }

      VISTRY {
         _ret = _businessop->displayCart();
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "displayCart"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "displayCart");
      return _ret;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("displayCart", 1);
    VISIF_EXCEP(return _ret;)

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return _ret;)
#endif
  }

  VISistream& _istrm = *VISistream::_downcast(_ibuf);
  _istrm >> _ret;
  return _ret;
}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
char* PetStore::BusinessOp::placeOrder(const char* _arg_userName ,
                                       const char* _arg_userAddress ) {
  char* _ret = (char*) 0;
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("placeOrder");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::BusinessOp_ops*  _businessop = PetStore::BusinessOp_ops::_downcast(_servant);
      if (!_businessop) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "placeOrder");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }

      VISTRY {
         _ret = _businessop->placeOrder(_arg_userName , _arg_userAddress );
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "placeOrder"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "placeOrder");
      return _ret;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("placeOrder", 1);
    VISIF_EXCEP(return _ret;)
    VISostream& _ostrm = *VISostream::_downcast(_obuf);
    _ostrm << _arg_userName;
    _ostrm << _arg_userAddress;

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return _ret;)
#endif
  }

  VISistream& _istrm = *VISistream::_downcast(_ibuf);
  _istrm >> _ret;
  return _ret;
}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
void PetStore::BusinessOp::cancelOrder(const char* _arg_orderID ) {
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("cancelOrder");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::BusinessOp_ops*  _businessop = PetStore::BusinessOp_ops::_downcast(_servant);
      if (!_businessop) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "cancelOrder");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }

      VISTRY {
        _businessop->cancelOrder(_arg_orderID );
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "cancelOrder"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "cancelOrder");
      return;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("cancelOrder", 1);
    VISIF_EXCEP(return;)
    VISostream& _ostrm = *VISostream::_downcast(_obuf);
    _ostrm << _arg_orderID;

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return;)
#endif
  }

}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
::PetStore::OrderInfo* PetStore::BusinessOp::listOrder(const char* _arg_orderID ) {
  ::PetStore::OrderInfo* _ret = (::PetStore::OrderInfo_ptr) NULL;
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("listOrder");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::BusinessOp_ops*  _businessop = PetStore::BusinessOp_ops::_downcast(_servant);
      if (!_businessop) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "listOrder");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }

      VISTRY {
         _ret = _businessop->listOrder(_arg_orderID );
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "listOrder"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "listOrder");
      return _ret;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("listOrder", 1);
    VISIF_EXCEP(return _ret;)
    VISostream& _ostrm = *VISostream::_downcast(_obuf);
    _ostrm << _arg_orderID;

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return _ret;)
#endif
  }

  VISistream& _istrm = *VISistream::_downcast(_ibuf);
  _istrm >> _ret;
  return _ret;
}

#if defined(__BORLANDC__)
 #pragma option pop
#endif

#if defined(MSVCNEWDLL_BUG)
void *PetStore::OrderInfoList::operator new(size_t ts) {
  return ::operator new(ts);
}
void PetStore::OrderInfoList::operator delete(void *p) {
  ::operator delete(p);
}
#endif // MSVCNEWDLL_BUG

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::OrderInfoList& _seq) {
  VISostream _ostrm(_strm);
  if (&_strm == &Cout || &_strm == &Cerr || &_strm == &Clog) {
    _pretty_print(_ostrm, _seq);
    _ostrm << endl;
  } else {
    _ostrm << _seq;
  }
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::OrderInfoList& _obj) {
  VISistream _istrm(_strm);
  _istrm >> _obj;
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::OrderInfoList_ptr& _ptr) {
  VISistream _istrm(_strm);
  _istrm >> _ptr;
  return _strm;
}

::PetStore::OrderInfo* PetStore::OrderInfoList::allocbuf(CORBA::ULong _nelems) {
  if (_nelems) {
    return new ::PetStore::OrderInfo[_nelems];
  }
  return (::PetStore::OrderInfo *)NULL;
}

void PetStore::OrderInfoList::freebuf(::PetStore::OrderInfo *_data) {
  if (_data)
    delete[] _data;
}

PetStore::OrderInfoList::OrderInfoList(CORBA::ULong _max) {
  _num_allocated = _max;
  _count = 0;
  _contents = allocbuf(_num_allocated);
  _release_flag = 1;
}

PetStore::OrderInfoList::OrderInfoList(CORBA::ULong _max, CORBA::ULong _len,
                                       ::PetStore::OrderInfo *_data,
                                       CORBA::Boolean _release) {
  _num_allocated = _max;
  _count = _len;
  _contents = _data;
  _release_flag = _release;
}

PetStore::OrderInfoList::OrderInfoList(const PetStore::OrderInfoList& _seq) {
  _num_allocated = _seq._num_allocated;
  _count = _seq._count;
  _contents = allocbuf(_num_allocated);
  if (_num_allocated > 0) {
    for (CORBA::ULong i=0; i < _count; i++) {
      _contents[i] = _seq._contents[i];
    }
  }
  _release_flag = 1;
}

PetStore::OrderInfoList::~OrderInfoList() {
  if (_release_flag) 
    freebuf(_contents);
}

PetStore::OrderInfoList& PetStore::OrderInfoList::operator=(const PetStore::OrderInfoList& _seq) {
  if (_num_allocated < _seq._count) {
    if (_release_flag) 
      freebuf(_contents);
    else
      _release_flag = (CORBA::Boolean) 1;

    _contents = allocbuf(_seq._num_allocated);
    _num_allocated = _seq._num_allocated;
  }
  _count = _seq._count;
  if (_num_allocated > 0) {
    for (CORBA::ULong i = 0; i < _count; i++) {
      _contents[i] = _seq._contents[i];
    }
  }
  return *this;
}

void PetStore::OrderInfoList::length(CORBA::ULong _newlen) {
  if (_newlen > _num_allocated) {
    ::PetStore::OrderInfo *temp = allocbuf(_newlen);
    for (CORBA::ULong i=0; i < _count; i++) {
      temp[i] = _contents[i];
    }
    if (_release_flag) 
      freebuf(_contents);
    else
      _release_flag = (CORBA::Boolean) 1;

    _num_allocated = _newlen;
    _contents = temp;
  }
  _count = _newlen;
}

void PetStore::OrderInfoList::replace(CORBA::ULong _max, CORBA::ULong _len,
                                      OrderInfo *_data,
                                      CORBA::Boolean _release) {
  if (_release_flag) 
    freebuf(_contents);
  _num_allocated = _max;
  _count = _len;
  _contents = _data;
  _release_flag = _release;
}

::PetStore::OrderInfo *PetStore::OrderInfoList::get_buffer(CORBA::Boolean orphan) {
  if (orphan == (CORBA::Boolean) 1) {
    if (_release_flag == (CORBA::Boolean) 0)
      return (::PetStore::OrderInfo*)NULL;
    else {
      _release_flag = (CORBA::Boolean) 0;
      return _contents;
    }
  } else
    return _contents;
}

const ::PetStore::OrderInfo* PetStore::OrderInfoList::get_buffer() const {
  // Do not use const_cast here because it might not be supported by the compiler
  return (const ::PetStore::OrderInfo*)(void*)_contents;
}

::PetStore::OrderInfo& PetStore::OrderInfoList::operator[](CORBA::ULong _index) {
  if (_index >= _count) {
    VISTHROW(CORBA::BAD_PARAM())
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(static ::PetStore::OrderInfo *_null = 0; return *_null;)
#endif
  }
  return _contents[_index];
}

const ::PetStore::OrderInfo& PetStore::OrderInfoList::operator[](CORBA::ULong _index) const {
  if (_index >= _count) {
    VISTHROW(CORBA::BAD_PARAM())
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(static ::PetStore::OrderInfo *_null = 0; return (const ::PetStore::OrderInfo&)*_null;)
#endif
  }
  // Do not use const_cast here because it might not be supported by the compiler
  return *(const ::PetStore::OrderInfo*)(void*)&_contents[_index];
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::OrderInfoList& _seq) {
  _strm << _seq._count;
  for (CORBA::ULong i=0; i < _seq._count; i++) {
    _strm << _seq._contents[i];
  }
  return _strm;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::OrderInfoList& _seq) {
  CORBA::ULong t_count;
  _strm >> t_count;

  if (t_count > _seq._count) {
    if (_seq._num_allocated < t_count) {
      if (_seq._release_flag)
        PetStore::OrderInfoList::freebuf(_seq._contents);
      else
        _seq._release_flag = (CORBA::Boolean) 1;
      _seq._contents = PetStore::OrderInfoList::allocbuf(t_count);
      _seq._num_allocated = t_count;
    }
  }
  _seq._count = t_count;

  for (CORBA::ULong i=0; i < _seq._count; i++) {
    _strm >> _seq._contents[i];
  }
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::OrderInfoList* _obj) {
  if (!_obj)
    VISTHROW(CORBA::BAD_PARAM())
  else
    _strm << *_obj;
  return _strm;
}
VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::OrderInfoList_ptr& _obj) {
  _obj = new PetStore::OrderInfoList();
  _strm >> *_obj;
  return _strm;
}

void PetStore::_internal_pretty_print(VISostream& _strm, const PetStore::OrderInfoList& _seq) {
  _strm << "SEQUENCE PetStore::OrderInfoList" << endl;
  _strm << "Number of Elements: " << _seq._count << endl;
  for (CORBA::ULong i=0; i < _seq._count; i++) {
    _strm << "[" << i << "]" << endl;
    _strm << "\t_contents[i]:" << endl;
    _pretty_print(_strm, _seq._contents[i]);
    _strm << endl;
    _strm << endl;
  }
}
::PetStore::OrderInfo& PetStore::OrderInfoList_var::operator[](CORBA::ULong _index) {
  if (_ptr)
    return (*_ptr)[_index];
  static PetStore::OrderInfoList _tmp;
  return _tmp[_index];
}

const ::PetStore::OrderInfo& PetStore::OrderInfoList_var::operator[](CORBA::ULong _index) const {
  if (_ptr)
    return (*(const PetStore::OrderInfoList*)_ptr)[_index];
  static const PetStore::OrderInfoList _tmp;
  return _tmp[_index];
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::OrderInfoList_var& _var) {
  if (!_var._ptr)
    VISTHROW(CORBA::BAD_PARAM())
  else
    _strm << *_var._ptr;
  return _strm;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::OrderInfoList_var& _var) {
  if (!_var._ptr)
    _var._ptr = new PetStore::OrderInfoList;
  _strm >> *_var._ptr;
  return _strm;
}

PetStore::AdvancedOp_ptr PetStore::AdvancedOp_var::_duplicate(PetStore::AdvancedOp_ptr _p) {
  return PetStore::AdvancedOp::_duplicate(_p);
}

void
PetStore::AdvancedOp_var::_release(PetStore::AdvancedOp_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_p);
}

PetStore::AdvancedOp_var::AdvancedOp_var()
  : _ptr(PetStore::AdvancedOp::_nil()) {}

PetStore::AdvancedOp_var::AdvancedOp_var(PetStore::AdvancedOp_ptr _p)
  : _ptr(_p) {}

PetStore::AdvancedOp_var::AdvancedOp_var(const PetStore::AdvancedOp_var& _var)
  : _ptr(PetStore::AdvancedOp::_duplicate((PetStore::AdvancedOp_ptr) _var)) {}

PetStore::AdvancedOp_var::~AdvancedOp_var() {
  CORBA::release((CORBA::Object_ptr)_ptr);
}

PetStore::AdvancedOp_var&
PetStore::AdvancedOp_var::operator=(const PetStore::AdvancedOp_var& _var) {
    _release(_ptr);
    _ptr = _duplicate(_var._ptr);
    return *this;
}

PetStore::AdvancedOp_var&
PetStore::AdvancedOp_var::operator=(PetStore::AdvancedOp_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_ptr);
  _ptr = _p;
  return *this;
}

PetStore::AdvancedOp_ptr& PetStore::AdvancedOp_var::out() {
  _release(_ptr);
  _ptr = (PetStore::AdvancedOp_ptr)NULL;
  return _ptr;
}

PetStore::AdvancedOp_ptr PetStore::AdvancedOp_var::_retn() {
  PetStore::AdvancedOp_ptr _tmp_ptr = _ptr;
  _ptr = (PetStore::AdvancedOp_ptr)NULL;
  return _tmp_ptr;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::AdvancedOp_var& _var) {
  _strm >> _var._ptr;
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::AdvancedOp_var& _var) {
  _strm << _var._ptr;
  return _strm;
}

Istream& PetStore::_marshal_out(Istream& _strm, PetStore::AdvancedOp_var& _var) {
  VISistream _istrm(_strm);
  _istrm >> _var._ptr;
  return _strm;
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::AdvancedOp_var& _var) {
  _strm << (CORBA::Object_ptr)_var._ptr;
  return _strm;
}

// If dllimport is specified, you might probably want to
// disable these definitions
// disable the const definitions
const VISOps_Info PetStore::AdvancedOp_ops::_ops_info("PetStore::AdvancedOp_ops");
const VISOps_Info *PetStore::AdvancedOp_ops::_desc() { return &_ops_info; }
PetStore::AdvancedOp_ops_ptr PetStore::AdvancedOp_ops::_downcast(PortableServer::ServantBase* _servant) {
  if (_servant == (PortableServer::ServantBase*)NULL)
    return PetStore::AdvancedOp_ops::_nil();
  return (PetStore::AdvancedOp_ops_ptr)_servant->_safe_downcast_ops(_ops_info);
}
const CORBA::TypeInfo
#ifdef AIXV3
PetStore::AdvancedOp::_class_info("PetStore::AdvancedOp",
                                  "IDL:PetStore/AdvancedOp:1.0", NULL,
                                  PetStore::AdvancedOp::_factory, NULL, 0,
                                  NULL, 0, CORBA::Object::_desc(), 0L);
#else
PetStore::AdvancedOp::_class_info("PetStore::AdvancedOp",
                                  "IDL:PetStore/AdvancedOp:1.0", NULL,
                                  PetStore::AdvancedOp::_factory, NULL, 0,
                                  NULL, 0, CORBA::Object::_desc(), 0L);
#endif

const CORBA::TypeInfo *PetStore::AdvancedOp::_desc() { return &_class_info; }
const CORBA::TypeInfo *PetStore::AdvancedOp::_type_info() const {
  return &_class_info;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::AdvancedOp_ptr& _obj) {
  CORBA::Object_var _var_obj(_obj);
  _var_obj = CORBA::Object::_read(_strm, PetStore::AdvancedOp::_desc());
  _obj = PetStore::AdvancedOp::_narrow(_var_obj);
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::AdvancedOp_ptr _obj) {
  _strm << (CORBA::Object_ptr)_obj;
  return _strm;
}
void* PetStore::AdvancedOp::_safe_narrow(const CORBA::TypeInfo& _info) const {
  if (_info == _class_info)
    return (void *)this;
  return CORBA_Object::_safe_narrow(_info);
}

CORBA::Object *PetStore::AdvancedOp::_factory() {
  return new PetStore::AdvancedOp;
}

PetStore::AdvancedOp_ptr PetStore::AdvancedOp::_this() {
  return PetStore::AdvancedOp::_duplicate(this);
}

PetStore::AdvancedOp_ptr PetStore::AdvancedOp::_narrow(CORBA::Object * _obj) {
  if (_obj == PetStore::AdvancedOp::_nil())
    return PetStore::AdvancedOp::_nil();
  else
    return PetStore::AdvancedOp::_duplicate((PetStore::AdvancedOp_ptr)_obj->_safe_narrow(_class_info));
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::AdvancedOp_ptr _obj) {
  _strm << (CORBA::Object_ptr) _obj;
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::AdvancedOp_ptr& _obj) {
  VISistream _istrm(_strm);
  _istrm >> _obj;
  return _strm;
}

PetStore::AdvancedOp *PetStore::AdvancedOp::_bind(const char *_object_name,
                                                  const char *_host_name,
                                                  const CORBA::BindOptions *_opt,
                                                  CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
          "IDL:PetStore/AdvancedOp:1.0", _object_name, _host_name, _opt, _orb);
  return AdvancedOp::_narrow(_obj);
}

PetStore::AdvancedOp *PetStore::AdvancedOp::_bind(const char *_poa_name,
                                                  const CORBA::OctetSequence& _id,
                                                  const char *_host_name,
                                                  const CORBA::BindOptions *_opt,
                                                  CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
   "IDL:PetStore/AdvancedOp:1.0", _poa_name, _id, _host_name, _opt, _orb);
  return AdvancedOp::_narrow(_obj);
}
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
::PetStore::OrderInfoList* PetStore::AdvancedOp::listAllOrders() {
  ::PetStore::OrderInfoList* _ret = (::PetStore::OrderInfoList_ptr) NULL;
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("listAllOrders");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::AdvancedOp_ops*  _advancedop = PetStore::AdvancedOp_ops::_downcast(_servant);
      if (!_advancedop) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "listAllOrders");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }

      VISTRY {
         _ret = _advancedop->listAllOrders();
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "listAllOrders"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "listAllOrders");
      return _ret;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("listAllOrders", 1);
    VISIF_EXCEP(return _ret;)

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return _ret;)
#endif
  }

  VISistream& _istrm = *VISistream::_downcast(_ibuf);
  _istrm >> _ret;
  return _ret;
}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
void PetStore::AdvancedOp::addProduct(const ::PetStore::ProductInfo& _arg_newProduct ) {
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("addProduct");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::AdvancedOp_ops*  _advancedop = PetStore::AdvancedOp_ops::_downcast(_servant);
      if (!_advancedop) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "addProduct");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }

      VISTRY {
        _advancedop->addProduct(_arg_newProduct );
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "addProduct"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "addProduct");
      return;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("addProduct", 1);
    VISIF_EXCEP(return;)
    VISostream& _ostrm = *VISostream::_downcast(_obuf);
    _ostrm << _arg_newProduct;

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return;)
#endif
  }

}

#if defined(__BORLANDC__)
 #pragma option pop
#endif

#if defined(MSVCNEWDLL_BUG)
void *PetStore::ProductList::operator new(size_t ts) {
  return ::operator new(ts);
}
void PetStore::ProductList::operator delete(void *p) {
  ::operator delete(p);
}
#endif // MSVCNEWDLL_BUG

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::ProductList& _seq) {
  VISostream _ostrm(_strm);
  if (&_strm == &Cout || &_strm == &Cerr || &_strm == &Clog) {
    _pretty_print(_ostrm, _seq);
    _ostrm << endl;
  } else {
    _ostrm << _seq;
  }
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::ProductList& _obj) {
  VISistream _istrm(_strm);
  _istrm >> _obj;
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::ProductList_ptr& _ptr) {
  VISistream _istrm(_strm);
  _istrm >> _ptr;
  return _strm;
}

::PetStore::Product_ptr* PetStore::ProductList::allocbuf(CORBA::ULong _nelems) {
  if (_nelems) {
    ::PetStore::Product_ptr *ret = new ::PetStore::Product_ptr[_nelems];
    memset(ret, 0, size_t(_nelems) * sizeof(::PetStore::Product_ptr));
    return ret;
  }
  return (::PetStore::Product_ptr *)NULL;
}

void PetStore::ProductList::freebuf(::PetStore::Product_ptr *_data) {
  if (_data)
    delete[] _data;
}

void PetStore::ProductList::freebuf_elems(::PetStore::Product_ptr *_data,
                                          CORBA::ULong _nelems) {
  if (_data) {
    for (CORBA::ULong i = 0; i < _nelems; i++) {
      PetStore::Product_var::_release(_data[i]);
      _data[i] = (PetStore::Product *)NULL;
    }
    delete[] _data;
  }
}

PetStore::ProductList::ProductList(CORBA::ULong _max) {
  _num_allocated = _max;
  _count = 0;
  _contents = allocbuf(_num_allocated);
  _release_flag = 1;
}

PetStore::ProductList::ProductList(CORBA::ULong _max, CORBA::ULong _len,
                                   ::PetStore::Product_ptr *_data,
                                   CORBA::Boolean _release) {
  _num_allocated = _max;
  _count = _len;
  _contents = _data;
  _release_flag = _release;
}

PetStore::ProductList::ProductList(const PetStore::ProductList& _seq) {
  _num_allocated = _seq._num_allocated;
  _count = _seq._count;
  _contents = allocbuf(_num_allocated);
  if (_num_allocated > 0) {
    for (CORBA::ULong i=0; i < _count; i++) {
      _contents[i] = ::PetStore::Product_var::_duplicate(_seq._contents[i]);
    }
  }
  _release_flag = 1;
}

PetStore::ProductList::~ProductList() {
  if (_release_flag) 
    freebuf_elems(_contents, _count);
}

PetStore::ProductList& PetStore::ProductList::operator=(const PetStore::ProductList& _seq) {
  if (_num_allocated < _seq._count) {
    if (_release_flag) 
      freebuf_elems(_contents, _count);
    else
      _release_flag = (CORBA::Boolean) 1;

    _contents = allocbuf(_seq._num_allocated);
    _num_allocated = _seq._num_allocated;
  } else {
    for (CORBA::ULong i = _seq._count; i < _count; i++) {
      if (_contents[i]) {
        PetStore::Product_var::_release(_contents[i]);
        _contents[i] = (PetStore::Product *)NULL;
      }
    }
  }
  _count = _seq._count;
  if (_num_allocated > 0) {
    for (CORBA::ULong i = 0; i < _count; i++) {
      if (_contents[i]) {
        PetStore::Product_var::_release(_contents[i]);
        _contents[i] = (PetStore::Product *)NULL;
      }
      _contents[i] = ::PetStore::Product_var::_duplicate(_seq._contents[i]);
    }
  }
  return *this;
}

void PetStore::ProductList::length(CORBA::ULong _newlen) {
  if (_newlen > _num_allocated) {
    ::PetStore::Product_ptr *temp = allocbuf(_newlen);
    for (CORBA::ULong i=0; i < _count; i++) {
      temp[i] = ::PetStore::Product_var::_duplicate(_contents[i]);
    }
    if (_release_flag) 
      freebuf_elems(_contents, _count);
    else
      _release_flag = (CORBA::Boolean) 1;

    _num_allocated = _newlen;
    _contents = temp;
  }
  else if (_newlen > _count)
    memset(&_contents[_count], 0,
           size_t(_newlen - _count) * sizeof(::PetStore::Product_ptr));
  _count = _newlen;
}

void PetStore::ProductList::replace(CORBA::ULong _max, CORBA::ULong _len,
                                    Product_ptr *_data,
                                    CORBA::Boolean _release) {
  if (_release_flag) 
    freebuf_elems(_contents, _count);
  _num_allocated = _max;
  _count = _len;
  _contents = _data;
  _release_flag = _release;
}

::PetStore::Product_ptr *PetStore::ProductList::get_buffer(CORBA::Boolean orphan) {
  if (orphan == (CORBA::Boolean) 1) {
    if (_release_flag == (CORBA::Boolean) 0)
      return (::PetStore::Product_ptr*)NULL;
    else {
      _release_flag = (CORBA::Boolean) 0;
      return _contents;
    }
  } else
    return _contents;
}

const ::PetStore::Product_ptr* PetStore::ProductList::get_buffer() const {
  // Do not use const_cast here because it might not be supported by the compiler
  return (const ::PetStore::Product_ptr*)(void*)_contents;
}

PetStore::ProductList::Sequence_ProductList PetStore::ProductList::operator[](CORBA::ULong _index) {
  if (_index >= _count) {
    VISTHROW(CORBA::BAD_PARAM())
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(Product_ptr _tmp = (Product_ptr)NULL; return PetStore::ProductList::Sequence_ProductList(_tmp, (CORBA::Boolean) 0);)
#endif
  }
  return PetStore::ProductList::Sequence_ProductList(_contents[_index], _release_flag);
}

const PetStore::ProductList::Sequence_ProductList PetStore::ProductList::operator[](CORBA::ULong _index) const {
  if (_index >= _count) {
    VISTHROW(CORBA::BAD_PARAM())
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(Product_ptr _tmp = (Product_ptr)NULL; return PetStore::ProductList::Sequence_ProductList(_tmp, (CORBA::Boolean) 0);)
#endif
  }
  // Do not use const_cast here because it might not be supported by the compiler
  return PetStore::ProductList::Sequence_ProductList(((ProductList*)this)->_contents[_index], _release_flag);
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::ProductList& _seq) {
  _strm << _seq._count;
  for (CORBA::ULong i=0; i < _seq._count; i++) {
    _strm << _seq._contents[i];
  }
  return _strm;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::ProductList& _seq) {
  CORBA::ULong t_count;
  _strm >> t_count;

  if (t_count > _seq._count) {
    if (_seq._num_allocated < t_count) {
      if (_seq._release_flag)
        PetStore::ProductList::freebuf_elems(_seq._contents, _seq._count);
      else
        _seq._release_flag = (CORBA::Boolean) 1;
      _seq._contents = PetStore::ProductList::allocbuf(t_count);
      _seq._num_allocated = t_count;
    }
  } else {
    for (CORBA::ULong i = t_count; i < _seq._count; i++) {
      if (_seq._contents[i]) {
        PetStore::Product_var::_release(_seq._contents[i]);
        _seq._contents[i] = (PetStore::Product *)NULL;
      }
    }
  }
  _seq._count = t_count;

  for (CORBA::ULong i=0; i < _seq._count; i++) {
    _strm >> _seq._contents[i];
  }
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::ProductList* _obj) {
  if (!_obj)
    VISTHROW(CORBA::BAD_PARAM())
  else
    _strm << *_obj;
  return _strm;
}
VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::ProductList_ptr& _obj) {
  _obj = new PetStore::ProductList();
  _strm >> *_obj;
  return _strm;
}

void PetStore::_internal_pretty_print(VISostream& _strm, const PetStore::ProductList& _seq) {
  _strm << "SEQUENCE PetStore::ProductList" << endl;
  _strm << "Number of Elements: " << _seq._count << endl;
  for (CORBA::ULong i=0; i < _seq._count; i++) {
    _strm << "[" << i << "]" << endl;
    _strm << "\t_contents[i]:" << endl;
    _strm << "\t" << _seq._contents[i] << endl;
    _strm << endl;
  }
}
PetStore::ProductList::Sequence_ProductList PetStore::ProductList_var::operator[](CORBA::ULong _index) {
  if (_ptr)
    return (*_ptr)[_index];
  VISTHROW(CORBA::BAD_PARAM())
#if defined(_VIS_NOEXCEPTIONS)
  VISRETURN(static ::PetStore::Product_ptr _tmp = (::PetStore::Product_ptr)NULL;
            return PetStore::ProductList::Sequence_ProductList(_tmp, (CORBA::Boolean)0);)
#endif
}

const PetStore::ProductList::Sequence_ProductList PetStore::ProductList_var::operator[](CORBA::ULong _index) const {
  if (_ptr)
    return (*(const PetStore::ProductList*)_ptr)[_index];
  VISTHROW(CORBA::BAD_PARAM())
#if defined(_VIS_NOEXCEPTIONS)
  VISRETURN(static ::PetStore::Product_ptr _tmp = (::PetStore::Product_ptr)NULL;
            return PetStore::ProductList::Sequence_ProductList(_tmp, (CORBA::Boolean) 0);)
#endif
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::ProductList_var& _var) {
  if (!_var._ptr)
    VISTHROW(CORBA::BAD_PARAM())
  else
    _strm << *_var._ptr;
  return _strm;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::ProductList_var& _var) {
  if (!_var._ptr)
    _var._ptr = new PetStore::ProductList;
  _strm >> *_var._ptr;
  return _strm;
}

PetStore::ProductHomeExplicit_ptr PetStore::ProductHomeExplicit_var::_duplicate(PetStore::ProductHomeExplicit_ptr _p) {
  return PetStore::ProductHomeExplicit::_duplicate(_p);
}

void
PetStore::ProductHomeExplicit_var::_release(PetStore::ProductHomeExplicit_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_p);
}

PetStore::ProductHomeExplicit_var::ProductHomeExplicit_var()
  : _ptr(PetStore::ProductHomeExplicit::_nil()) {}

PetStore::ProductHomeExplicit_var::ProductHomeExplicit_var(PetStore::ProductHomeExplicit_ptr _p)
  : _ptr(_p) {}

PetStore::ProductHomeExplicit_var::ProductHomeExplicit_var(const PetStore::ProductHomeExplicit_var& _var)
  : _ptr(PetStore::ProductHomeExplicit::_duplicate((PetStore::ProductHomeExplicit_ptr) _var)) {}

PetStore::ProductHomeExplicit_var::~ProductHomeExplicit_var() {
  CORBA::release((CORBA::Object_ptr)_ptr);
}

PetStore::ProductHomeExplicit_var&
PetStore::ProductHomeExplicit_var::operator=(const PetStore::ProductHomeExplicit_var& _var) {
    _release(_ptr);
    _ptr = _duplicate(_var._ptr);
    return *this;
}

PetStore::ProductHomeExplicit_var&
PetStore::ProductHomeExplicit_var::operator=(PetStore::ProductHomeExplicit_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_ptr);
  _ptr = _p;
  return *this;
}

PetStore::ProductHomeExplicit_ptr& PetStore::ProductHomeExplicit_var::out() {
  _release(_ptr);
  _ptr = (PetStore::ProductHomeExplicit_ptr)NULL;
  return _ptr;
}

PetStore::ProductHomeExplicit_ptr PetStore::ProductHomeExplicit_var::_retn() {
  PetStore::ProductHomeExplicit_ptr _tmp_ptr = _ptr;
  _ptr = (PetStore::ProductHomeExplicit_ptr)NULL;
  return _tmp_ptr;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::ProductHomeExplicit_var& _var) {
  _strm >> _var._ptr;
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::ProductHomeExplicit_var& _var) {
  _strm << _var._ptr;
  return _strm;
}

Istream& PetStore::_marshal_out(Istream& _strm, PetStore::ProductHomeExplicit_var& _var) {
  VISistream _istrm(_strm);
  _istrm >> _var._ptr;
  return _strm;
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::ProductHomeExplicit_var& _var) {
  _strm << (CORBA::Object_ptr)_var._ptr;
  return _strm;
}

// If dllimport is specified, you might probably want to
// disable these definitions
// disable the const definitions
const VISOps_Info PetStore::ProductHomeExplicit_ops::_ops_info("PetStore::ProductHomeExplicit_ops");
const VISOps_Info *PetStore::ProductHomeExplicit_ops::_desc() { return &_ops_info; }
PetStore::ProductHomeExplicit_ops_ptr PetStore::ProductHomeExplicit_ops::_downcast(PortableServer::ServantBase* _servant) {
  if (_servant == (PortableServer::ServantBase*)NULL)
    return PetStore::ProductHomeExplicit_ops::_nil();
  return (PetStore::ProductHomeExplicit_ops_ptr)_servant->_safe_downcast_ops(_ops_info);
}
const CORBA::TypeInfo
#ifdef AIXV3
PetStore::ProductHomeExplicit::_class_info("PetStore::ProductHomeExplicit",
                                           "IDL:PetStore/ProductHomeExplicit:1.0",
                                           NULL,
                                           PetStore::ProductHomeExplicit::_factory,
                                           NULL, 0, NULL, 0,
                                           Components::CCMHome::_desc(), 0L);
#else
PetStore::ProductHomeExplicit::_class_info("PetStore::ProductHomeExplicit",
                                           "IDL:PetStore/ProductHomeExplicit:1.0",
                                           NULL,
                                           PetStore::ProductHomeExplicit::_factory,
                                           NULL, 0, NULL, 0,
                                           Components::CCMHome::_desc(), 0L);
#endif

const CORBA::TypeInfo *PetStore::ProductHomeExplicit::_desc() { return &_class_info; }
const CORBA::TypeInfo *PetStore::ProductHomeExplicit::_type_info() const {
  return &_class_info;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::ProductHomeExplicit_ptr& _obj) {
  CORBA::Object_var _var_obj(_obj);
  _var_obj = CORBA::Object::_read(_strm, PetStore::ProductHomeExplicit::_desc());
  _obj = PetStore::ProductHomeExplicit::_narrow(_var_obj);
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::ProductHomeExplicit_ptr _obj) {
  _strm << (CORBA::Object_ptr)_obj;
  return _strm;
}
void* PetStore::ProductHomeExplicit::_safe_narrow(const CORBA::TypeInfo& _info) const {
  if (_info == _class_info)
    return (void *)this;
  if (_info == *Components::CCMHome::_desc())
    return (void *) (Components::CCMHome *) this;
  return CORBA_Object::_safe_narrow(_info);
}

CORBA::Object *PetStore::ProductHomeExplicit::_factory() {
  return new PetStore::ProductHomeExplicit;
}

PetStore::ProductHomeExplicit_ptr PetStore::ProductHomeExplicit::_this() {
  return PetStore::ProductHomeExplicit::_duplicate(this);
}

PetStore::ProductHomeExplicit_ptr PetStore::ProductHomeExplicit::_narrow(CORBA::Object * _obj) {
  if (_obj == PetStore::ProductHomeExplicit::_nil())
    return PetStore::ProductHomeExplicit::_nil();
  else
    return PetStore::ProductHomeExplicit::_duplicate((PetStore::ProductHomeExplicit_ptr)_obj->_safe_narrow(_class_info));
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::ProductHomeExplicit_ptr _obj) {
  _strm << (CORBA::Object_ptr) _obj;
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::ProductHomeExplicit_ptr& _obj) {
  VISistream _istrm(_strm);
  _istrm >> _obj;
  return _strm;
}

PetStore::ProductHomeExplicit *PetStore::ProductHomeExplicit::_bind(const char *_object_name,
                                                                    const char *_host_name,
                                                                    const CORBA::BindOptions *_opt,
                                                                    CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
          "IDL:PetStore/ProductHomeExplicit:1.0", _object_name, _host_name,
_opt, _orb);
  return ProductHomeExplicit::_narrow(_obj);
}

PetStore::ProductHomeExplicit *PetStore::ProductHomeExplicit::_bind(const char *_poa_name,
                                                                    const CORBA::OctetSequence& _id,
                                                                    const char *_host_name,
                                                                    const CORBA::BindOptions *_opt,
                                                                    CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
   "IDL:PetStore/ProductHomeExplicit:1.0", _poa_name, _id, _host_name, _opt,
   _orb);
  return ProductHomeExplicit::_narrow(_obj);
}
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
::PetStore::ProductList* PetStore::ProductHomeExplicit::findByProductCategory(::PetStore::ProductCategory _arg_category ) {
  ::PetStore::ProductList* _ret = (::PetStore::ProductList_ptr) NULL;
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("findByProductCategory");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::ProductHomeExplicit_ops*  _producthomeexplicit = PetStore::ProductHomeExplicit_ops::_downcast(_servant);
      if (!_producthomeexplicit) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "findByProductCategory");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }

      VISTRY {
         _ret = _producthomeexplicit->findByProductCategory(_arg_category );
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "findByProductCategory"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "findByProductCategory");
      return _ret;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("findByProductCategory", 1);
    VISIF_EXCEP(return _ret;)
    VISostream& _ostrm = *VISostream::_downcast(_obuf);
    _ostrm << (CORBA::ULong)_arg_category;

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return _ret;)
#endif
  }

  VISistream& _istrm = *VISistream::_downcast(_ibuf);
  _istrm >> _ret;
  return _ret;
}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
::PetStore::ProductList* PetStore::ProductHomeExplicit::findAllProducts() {
  ::PetStore::ProductList* _ret = (::PetStore::ProductList_ptr) NULL;
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("findAllProducts");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::ProductHomeExplicit_ops*  _producthomeexplicit = PetStore::ProductHomeExplicit_ops::_downcast(_servant);
      if (!_producthomeexplicit) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "findAllProducts");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }

      VISTRY {
         _ret = _producthomeexplicit->findAllProducts();
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "findAllProducts"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "findAllProducts");
      return _ret;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("findAllProducts", 1);
    VISIF_EXCEP(return _ret;)

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return _ret;)
#endif
  }

  VISistream& _istrm = *VISistream::_downcast(_ibuf);
  _istrm >> _ret;
  return _ret;
}

#if defined(__BORLANDC__)
 #pragma option pop
#endif

PetStore::ProductHomeImplicit_ptr PetStore::ProductHomeImplicit_var::_duplicate(PetStore::ProductHomeImplicit_ptr _p) {
  return PetStore::ProductHomeImplicit::_duplicate(_p);
}

void
PetStore::ProductHomeImplicit_var::_release(PetStore::ProductHomeImplicit_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_p);
}

PetStore::ProductHomeImplicit_var::ProductHomeImplicit_var()
  : _ptr(PetStore::ProductHomeImplicit::_nil()) {}

PetStore::ProductHomeImplicit_var::ProductHomeImplicit_var(PetStore::ProductHomeImplicit_ptr _p)
  : _ptr(_p) {}

PetStore::ProductHomeImplicit_var::ProductHomeImplicit_var(const PetStore::ProductHomeImplicit_var& _var)
  : _ptr(PetStore::ProductHomeImplicit::_duplicate((PetStore::ProductHomeImplicit_ptr) _var)) {}

PetStore::ProductHomeImplicit_var::~ProductHomeImplicit_var() {
  CORBA::release((CORBA::Object_ptr)_ptr);
}

PetStore::ProductHomeImplicit_var&
PetStore::ProductHomeImplicit_var::operator=(const PetStore::ProductHomeImplicit_var& _var) {
    _release(_ptr);
    _ptr = _duplicate(_var._ptr);
    return *this;
}

PetStore::ProductHomeImplicit_var&
PetStore::ProductHomeImplicit_var::operator=(PetStore::ProductHomeImplicit_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_ptr);
  _ptr = _p;
  return *this;
}

PetStore::ProductHomeImplicit_ptr& PetStore::ProductHomeImplicit_var::out() {
  _release(_ptr);
  _ptr = (PetStore::ProductHomeImplicit_ptr)NULL;
  return _ptr;
}

PetStore::ProductHomeImplicit_ptr PetStore::ProductHomeImplicit_var::_retn() {
  PetStore::ProductHomeImplicit_ptr _tmp_ptr = _ptr;
  _ptr = (PetStore::ProductHomeImplicit_ptr)NULL;
  return _tmp_ptr;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::ProductHomeImplicit_var& _var) {
  _strm >> _var._ptr;
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::ProductHomeImplicit_var& _var) {
  _strm << _var._ptr;
  return _strm;
}

Istream& PetStore::_marshal_out(Istream& _strm, PetStore::ProductHomeImplicit_var& _var) {
  VISistream _istrm(_strm);
  _istrm >> _var._ptr;
  return _strm;
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::ProductHomeImplicit_var& _var) {
  _strm << (CORBA::Object_ptr)_var._ptr;
  return _strm;
}

// If dllimport is specified, you might probably want to
// disable these definitions
// disable the const definitions
const VISOps_Info PetStore::ProductHomeImplicit_ops::_ops_info("PetStore::ProductHomeImplicit_ops");
const VISOps_Info *PetStore::ProductHomeImplicit_ops::_desc() { return &_ops_info; }
PetStore::ProductHomeImplicit_ops_ptr PetStore::ProductHomeImplicit_ops::_downcast(PortableServer::ServantBase* _servant) {
  if (_servant == (PortableServer::ServantBase*)NULL)
    return PetStore::ProductHomeImplicit_ops::_nil();
  return (PetStore::ProductHomeImplicit_ops_ptr)_servant->_safe_downcast_ops(_ops_info);
}
const CORBA::TypeInfo
#ifdef AIXV3
PetStore::ProductHomeImplicit::_class_info("PetStore::ProductHomeImplicit",
                                           "IDL:PetStore/ProductHomeImplicit:1.0",
                                           NULL,
                                           PetStore::ProductHomeImplicit::_factory,
                                           NULL, 0, NULL, 0,
                                           CORBA::Object::_desc(), 0L);
#else
PetStore::ProductHomeImplicit::_class_info("PetStore::ProductHomeImplicit",
                                           "IDL:PetStore/ProductHomeImplicit:1.0",
                                           NULL,
                                           PetStore::ProductHomeImplicit::_factory,
                                           NULL, 0, NULL, 0,
                                           CORBA::Object::_desc(), 0L);
#endif

const CORBA::TypeInfo *PetStore::ProductHomeImplicit::_desc() { return &_class_info; }
const CORBA::TypeInfo *PetStore::ProductHomeImplicit::_type_info() const {
  return &_class_info;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::ProductHomeImplicit_ptr& _obj) {
  CORBA::Object_var _var_obj(_obj);
  _var_obj = CORBA::Object::_read(_strm, PetStore::ProductHomeImplicit::_desc());
  _obj = PetStore::ProductHomeImplicit::_narrow(_var_obj);
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::ProductHomeImplicit_ptr _obj) {
  _strm << (CORBA::Object_ptr)_obj;
  return _strm;
}
void* PetStore::ProductHomeImplicit::_safe_narrow(const CORBA::TypeInfo& _info) const {
  if (_info == _class_info)
    return (void *)this;
  return CORBA_Object::_safe_narrow(_info);
}

CORBA::Object *PetStore::ProductHomeImplicit::_factory() {
  return new PetStore::ProductHomeImplicit;
}

PetStore::ProductHomeImplicit_ptr PetStore::ProductHomeImplicit::_this() {
  return PetStore::ProductHomeImplicit::_duplicate(this);
}

PetStore::ProductHomeImplicit_ptr PetStore::ProductHomeImplicit::_narrow(CORBA::Object * _obj) {
  if (_obj == PetStore::ProductHomeImplicit::_nil())
    return PetStore::ProductHomeImplicit::_nil();
  else
    return PetStore::ProductHomeImplicit::_duplicate((PetStore::ProductHomeImplicit_ptr)_obj->_safe_narrow(_class_info));
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::ProductHomeImplicit_ptr _obj) {
  _strm << (CORBA::Object_ptr) _obj;
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::ProductHomeImplicit_ptr& _obj) {
  VISistream _istrm(_strm);
  _istrm >> _obj;
  return _strm;
}

PetStore::ProductHomeImplicit *PetStore::ProductHomeImplicit::_bind(const char *_object_name,
                                                                    const char *_host_name,
                                                                    const CORBA::BindOptions *_opt,
                                                                    CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
          "IDL:PetStore/ProductHomeImplicit:1.0", _object_name, _host_name,
_opt, _orb);
  return ProductHomeImplicit::_narrow(_obj);
}

PetStore::ProductHomeImplicit *PetStore::ProductHomeImplicit::_bind(const char *_poa_name,
                                                                    const CORBA::OctetSequence& _id,
                                                                    const char *_host_name,
                                                                    const CORBA::BindOptions *_opt,
                                                                    CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
   "IDL:PetStore/ProductHomeImplicit:1.0", _poa_name, _id, _host_name, _opt,
   _orb);
  return ProductHomeImplicit::_narrow(_obj);
}
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
::PetStore::Product_ptr PetStore::ProductHomeImplicit::create(::PetStore::keyID_ptr _arg_pkey ) {
  ::PetStore::Product_ptr _ret = (::PetStore::Product_ptr) NULL;
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("create");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::ProductHomeImplicit_ops*  _producthomeimplicit = PetStore::ProductHomeImplicit_ops::_downcast(_servant);
      if (!_producthomeimplicit) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "create");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }

      VISTRY {
         _ret = _producthomeimplicit->create(_arg_pkey );
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "create"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "create");
      return _ret;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("create", 1);
    VISIF_EXCEP(return _ret;)
    VISostream& _ostrm = *VISostream::_downcast(_obuf);
    _ostrm << _arg_pkey;

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return _ret;)
#endif
  }

  VISistream& _istrm = *VISistream::_downcast(_ibuf);
  _istrm >> _ret;
  return _ret;
}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
::PetStore::Product_ptr PetStore::ProductHomeImplicit::find_by_primary_key(::PetStore::keyID_ptr _arg_pkey ) {
  ::PetStore::Product_ptr _ret = (::PetStore::Product_ptr) NULL;
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("find_by_primary_key");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::ProductHomeImplicit_ops*  _producthomeimplicit = PetStore::ProductHomeImplicit_ops::_downcast(_servant);
      if (!_producthomeimplicit) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "find_by_primary_key");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }

      VISTRY {
         _ret = _producthomeimplicit->find_by_primary_key(_arg_pkey );
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "find_by_primary_key"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "find_by_primary_key");
      return _ret;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("find_by_primary_key", 1);
    VISIF_EXCEP(return _ret;)
    VISostream& _ostrm = *VISostream::_downcast(_obuf);
    _ostrm << _arg_pkey;

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return _ret;)
#endif
  }

  VISistream& _istrm = *VISistream::_downcast(_ibuf);
  _istrm >> _ret;
  return _ret;
}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
void PetStore::ProductHomeImplicit::remove(::PetStore::keyID_ptr _arg_pkey ) {
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("remove");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::ProductHomeImplicit_ops*  _producthomeimplicit = PetStore::ProductHomeImplicit_ops::_downcast(_servant);
      if (!_producthomeimplicit) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "remove");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }

      VISTRY {
        _producthomeimplicit->remove(_arg_pkey );
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "remove"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "remove");
      return;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("remove", 1);
    VISIF_EXCEP(return;)
    VISostream& _ostrm = *VISostream::_downcast(_obuf);
    _ostrm << _arg_pkey;

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return;)
#endif
  }

}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
::PetStore::keyID_ptr PetStore::ProductHomeImplicit::get_primary_key(::PetStore::Product_ptr _arg_comp ) {
  ::PetStore::keyID_ptr _ret = (::PetStore::keyID_ptr) NULL;
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("get_primary_key");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::ProductHomeImplicit_ops*  _producthomeimplicit = PetStore::ProductHomeImplicit_ops::_downcast(_servant);
      if (!_producthomeimplicit) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "get_primary_key");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }

      VISTRY {
         _ret = _producthomeimplicit->get_primary_key(_arg_comp );
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "get_primary_key"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "get_primary_key");
      return _ret;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("get_primary_key", 1);
    VISIF_EXCEP(return _ret;)
    VISostream& _ostrm = *VISostream::_downcast(_obuf);
    _ostrm << _arg_comp;

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return _ret;)
#endif
  }

  VISistream& _istrm = *VISistream::_downcast(_ibuf);
  _istrm >> _ret;
  return _ret;
}

#if defined(__BORLANDC__)
 #pragma option pop
#endif

PetStore::ProductHome_ptr PetStore::ProductHome_var::_duplicate(PetStore::ProductHome_ptr _p) {
  return PetStore::ProductHome::_duplicate(_p);
}

void
PetStore::ProductHome_var::_release(PetStore::ProductHome_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_p);
}

PetStore::ProductHome_var::ProductHome_var()
  : _ptr(PetStore::ProductHome::_nil()) {}

PetStore::ProductHome_var::ProductHome_var(PetStore::ProductHome_ptr _p)
  : _ptr(_p) {}

PetStore::ProductHome_var::ProductHome_var(const PetStore::ProductHome_var& _var)
  : _ptr(PetStore::ProductHome::_duplicate((PetStore::ProductHome_ptr) _var)) {}

PetStore::ProductHome_var::~ProductHome_var() {
  CORBA::release((CORBA::Object_ptr)_ptr);
}

PetStore::ProductHome_var&
PetStore::ProductHome_var::operator=(const PetStore::ProductHome_var& _var) {
    _release(_ptr);
    _ptr = _duplicate(_var._ptr);
    return *this;
}

PetStore::ProductHome_var&
PetStore::ProductHome_var::operator=(PetStore::ProductHome_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_ptr);
  _ptr = _p;
  return *this;
}

PetStore::ProductHome_ptr& PetStore::ProductHome_var::out() {
  _release(_ptr);
  _ptr = (PetStore::ProductHome_ptr)NULL;
  return _ptr;
}

PetStore::ProductHome_ptr PetStore::ProductHome_var::_retn() {
  PetStore::ProductHome_ptr _tmp_ptr = _ptr;
  _ptr = (PetStore::ProductHome_ptr)NULL;
  return _tmp_ptr;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::ProductHome_var& _var) {
  _strm >> _var._ptr;
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::ProductHome_var& _var) {
  _strm << _var._ptr;
  return _strm;
}

Istream& PetStore::_marshal_out(Istream& _strm, PetStore::ProductHome_var& _var) {
  VISistream _istrm(_strm);
  _istrm >> _var._ptr;
  return _strm;
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::ProductHome_var& _var) {
  _strm << (CORBA::Object_ptr)_var._ptr;
  return _strm;
}

// If dllimport is specified, you might probably want to
// disable these definitions
// disable the const definitions
const VISOps_Info PetStore::ProductHome_ops::_ops_info("PetStore::ProductHome_ops");
const VISOps_Info *PetStore::ProductHome_ops::_desc() { return &_ops_info; }
PetStore::ProductHome_ops_ptr PetStore::ProductHome_ops::_downcast(PortableServer::ServantBase* _servant) {
  if (_servant == (PortableServer::ServantBase*)NULL)
    return PetStore::ProductHome_ops::_nil();
  return (PetStore::ProductHome_ops_ptr)_servant->_safe_downcast_ops(_ops_info);
}
const CORBA::TypeInfo
#ifdef AIXV3
PetStore::ProductHome::_class_info("PetStore::ProductHome",
                                   "IDL:PetStore/ProductHome:1.0", NULL,
                                   PetStore::ProductHome::_factory, NULL, 0,
                                   NULL, 0,
                                   PetStore::ProductHomeExplicit::_desc(),
                                   PetStore::ProductHomeImplicit::_desc(), 0L);
#else
PetStore::ProductHome::_class_info("PetStore::ProductHome",
                                   "IDL:PetStore/ProductHome:1.0", NULL,
                                   PetStore::ProductHome::_factory, NULL, 0,
                                   NULL, 0,
                                   PetStore::ProductHomeExplicit::_desc(),
                                   PetStore::ProductHomeImplicit::_desc(), 0L);
#endif

const CORBA::TypeInfo *PetStore::ProductHome::_desc() { return &_class_info; }
const CORBA::TypeInfo *PetStore::ProductHome::_type_info() const {
  return &_class_info;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::ProductHome_ptr& _obj) {
  CORBA::Object_var _var_obj(_obj);
  _var_obj = CORBA::Object::_read(_strm, PetStore::ProductHome::_desc());
  _obj = PetStore::ProductHome::_narrow(_var_obj);
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::ProductHome_ptr _obj) {
  _strm << (CORBA::Object_ptr)_obj;
  return _strm;
}
void* PetStore::ProductHome::_safe_narrow(const CORBA::TypeInfo& _info) const {
  if (_info == _class_info)
    return (void *)this;
  if (_info == *PetStore::ProductHomeExplicit::_desc())
    return (void *) (PetStore::ProductHomeExplicit *) this;
  if (_info == *Components::CCMHome::_desc())
    return (void *) (Components::CCMHome *) this;
  if (_info == *PetStore::ProductHomeImplicit::_desc())
    return (void *) (PetStore::ProductHomeImplicit *) this;
  return CORBA_Object::_safe_narrow(_info);
}

CORBA::Object *PetStore::ProductHome::_factory() {
  return new PetStore::ProductHome;
}

PetStore::ProductHome_ptr PetStore::ProductHome::_this() {
  return PetStore::ProductHome::_duplicate(this);
}

PetStore::ProductHome_ptr PetStore::ProductHome::_narrow(CORBA::Object * _obj) {
  if (_obj == PetStore::ProductHome::_nil())
    return PetStore::ProductHome::_nil();
  else
    return PetStore::ProductHome::_duplicate((PetStore::ProductHome_ptr)_obj->_safe_narrow(_class_info));
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::ProductHome_ptr _obj) {
  _strm << (CORBA::Object_ptr) _obj;
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::ProductHome_ptr& _obj) {
  VISistream _istrm(_strm);
  _istrm >> _obj;
  return _strm;
}

PetStore::ProductHome *PetStore::ProductHome::_bind(const char *_object_name,
                                                    const char *_host_name,
                                                    const CORBA::BindOptions *_opt,
                                                    CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
          "IDL:PetStore/ProductHome:1.0", _object_name, _host_name, _opt, _orb);
  return ProductHome::_narrow(_obj);
}

PetStore::ProductHome *PetStore::ProductHome::_bind(const char *_poa_name,
                                                    const CORBA::OctetSequence& _id,
                                                    const char *_host_name,
                                                    const CORBA::BindOptions *_opt,
                                                    CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
   "IDL:PetStore/ProductHome:1.0", _poa_name, _id, _host_name, _opt, _orb);
  return ProductHome::_narrow(_obj);
}

#if defined(MSVCNEWDLL_BUG)
void *PetStore::OrderList::operator new(size_t ts) {
  return ::operator new(ts);
}
void PetStore::OrderList::operator delete(void *p) {
  ::operator delete(p);
}
#endif // MSVCNEWDLL_BUG

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::OrderList& _seq) {
  VISostream _ostrm(_strm);
  if (&_strm == &Cout || &_strm == &Cerr || &_strm == &Clog) {
    _pretty_print(_ostrm, _seq);
    _ostrm << endl;
  } else {
    _ostrm << _seq;
  }
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::OrderList& _obj) {
  VISistream _istrm(_strm);
  _istrm >> _obj;
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::OrderList_ptr& _ptr) {
  VISistream _istrm(_strm);
  _istrm >> _ptr;
  return _strm;
}

::PetStore::Order_ptr* PetStore::OrderList::allocbuf(CORBA::ULong _nelems) {
  if (_nelems) {
    ::PetStore::Order_ptr *ret = new ::PetStore::Order_ptr[_nelems];
    memset(ret, 0, size_t(_nelems) * sizeof(::PetStore::Order_ptr));
    return ret;
  }
  return (::PetStore::Order_ptr *)NULL;
}

void PetStore::OrderList::freebuf(::PetStore::Order_ptr *_data) {
  if (_data)
    delete[] _data;
}

void PetStore::OrderList::freebuf_elems(::PetStore::Order_ptr *_data,
                                        CORBA::ULong _nelems) {
  if (_data) {
    for (CORBA::ULong i = 0; i < _nelems; i++) {
      PetStore::Order_var::_release(_data[i]);
      _data[i] = (PetStore::Order *)NULL;
    }
    delete[] _data;
  }
}

PetStore::OrderList::OrderList(CORBA::ULong _max) {
  _num_allocated = _max;
  _count = 0;
  _contents = allocbuf(_num_allocated);
  _release_flag = 1;
}

PetStore::OrderList::OrderList(CORBA::ULong _max, CORBA::ULong _len,
                               ::PetStore::Order_ptr *_data,
                               CORBA::Boolean _release) {
  _num_allocated = _max;
  _count = _len;
  _contents = _data;
  _release_flag = _release;
}

PetStore::OrderList::OrderList(const PetStore::OrderList& _seq) {
  _num_allocated = _seq._num_allocated;
  _count = _seq._count;
  _contents = allocbuf(_num_allocated);
  if (_num_allocated > 0) {
    for (CORBA::ULong i=0; i < _count; i++) {
      _contents[i] = ::PetStore::Order_var::_duplicate(_seq._contents[i]);
    }
  }
  _release_flag = 1;
}

PetStore::OrderList::~OrderList() {
  if (_release_flag) 
    freebuf_elems(_contents, _count);
}

PetStore::OrderList& PetStore::OrderList::operator=(const PetStore::OrderList& _seq) {
  if (_num_allocated < _seq._count) {
    if (_release_flag) 
      freebuf_elems(_contents, _count);
    else
      _release_flag = (CORBA::Boolean) 1;

    _contents = allocbuf(_seq._num_allocated);
    _num_allocated = _seq._num_allocated;
  } else {
    for (CORBA::ULong i = _seq._count; i < _count; i++) {
      if (_contents[i]) {
        PetStore::Order_var::_release(_contents[i]);
        _contents[i] = (PetStore::Order *)NULL;
      }
    }
  }
  _count = _seq._count;
  if (_num_allocated > 0) {
    for (CORBA::ULong i = 0; i < _count; i++) {
      if (_contents[i]) {
        PetStore::Order_var::_release(_contents[i]);
        _contents[i] = (PetStore::Order *)NULL;
      }
      _contents[i] = ::PetStore::Order_var::_duplicate(_seq._contents[i]);
    }
  }
  return *this;
}

void PetStore::OrderList::length(CORBA::ULong _newlen) {
  if (_newlen > _num_allocated) {
    ::PetStore::Order_ptr *temp = allocbuf(_newlen);
    for (CORBA::ULong i=0; i < _count; i++) {
      temp[i] = ::PetStore::Order_var::_duplicate(_contents[i]);
    }
    if (_release_flag) 
      freebuf_elems(_contents, _count);
    else
      _release_flag = (CORBA::Boolean) 1;

    _num_allocated = _newlen;
    _contents = temp;
  }
  else if (_newlen > _count)
    memset(&_contents[_count], 0,
           size_t(_newlen - _count) * sizeof(::PetStore::Order_ptr));
  _count = _newlen;
}

void PetStore::OrderList::replace(CORBA::ULong _max, CORBA::ULong _len,
                                  Order_ptr *_data, CORBA::Boolean _release) {
  if (_release_flag) 
    freebuf_elems(_contents, _count);
  _num_allocated = _max;
  _count = _len;
  _contents = _data;
  _release_flag = _release;
}

::PetStore::Order_ptr *PetStore::OrderList::get_buffer(CORBA::Boolean orphan) {
  if (orphan == (CORBA::Boolean) 1) {
    if (_release_flag == (CORBA::Boolean) 0)
      return (::PetStore::Order_ptr*)NULL;
    else {
      _release_flag = (CORBA::Boolean) 0;
      return _contents;
    }
  } else
    return _contents;
}

const ::PetStore::Order_ptr* PetStore::OrderList::get_buffer() const {
  // Do not use const_cast here because it might not be supported by the compiler
  return (const ::PetStore::Order_ptr*)(void*)_contents;
}

PetStore::OrderList::Sequence_OrderList PetStore::OrderList::operator[](CORBA::ULong _index) {
  if (_index >= _count) {
    VISTHROW(CORBA::BAD_PARAM())
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(Order_ptr _tmp = (Order_ptr)NULL; return PetStore::OrderList::Sequence_OrderList(_tmp, (CORBA::Boolean) 0);)
#endif
  }
  return PetStore::OrderList::Sequence_OrderList(_contents[_index], _release_flag);
}

const PetStore::OrderList::Sequence_OrderList PetStore::OrderList::operator[](CORBA::ULong _index) const {
  if (_index >= _count) {
    VISTHROW(CORBA::BAD_PARAM())
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(Order_ptr _tmp = (Order_ptr)NULL; return PetStore::OrderList::Sequence_OrderList(_tmp, (CORBA::Boolean) 0);)
#endif
  }
  // Do not use const_cast here because it might not be supported by the compiler
  return PetStore::OrderList::Sequence_OrderList(((OrderList*)this)->_contents[_index], _release_flag);
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::OrderList& _seq) {
  _strm << _seq._count;
  for (CORBA::ULong i=0; i < _seq._count; i++) {
    _strm << _seq._contents[i];
  }
  return _strm;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::OrderList& _seq) {
  CORBA::ULong t_count;
  _strm >> t_count;

  if (t_count > _seq._count) {
    if (_seq._num_allocated < t_count) {
      if (_seq._release_flag)
        PetStore::OrderList::freebuf_elems(_seq._contents, _seq._count);
      else
        _seq._release_flag = (CORBA::Boolean) 1;
      _seq._contents = PetStore::OrderList::allocbuf(t_count);
      _seq._num_allocated = t_count;
    }
  } else {
    for (CORBA::ULong i = t_count; i < _seq._count; i++) {
      if (_seq._contents[i]) {
        PetStore::Order_var::_release(_seq._contents[i]);
        _seq._contents[i] = (PetStore::Order *)NULL;
      }
    }
  }
  _seq._count = t_count;

  for (CORBA::ULong i=0; i < _seq._count; i++) {
    _strm >> _seq._contents[i];
  }
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::OrderList* _obj) {
  if (!_obj)
    VISTHROW(CORBA::BAD_PARAM())
  else
    _strm << *_obj;
  return _strm;
}
VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::OrderList_ptr& _obj) {
  _obj = new PetStore::OrderList();
  _strm >> *_obj;
  return _strm;
}

void PetStore::_internal_pretty_print(VISostream& _strm, const PetStore::OrderList& _seq) {
  _strm << "SEQUENCE PetStore::OrderList" << endl;
  _strm << "Number of Elements: " << _seq._count << endl;
  for (CORBA::ULong i=0; i < _seq._count; i++) {
    _strm << "[" << i << "]" << endl;
    _strm << "\t_contents[i]:" << endl;
    _strm << "\t" << _seq._contents[i] << endl;
    _strm << endl;
  }
}
PetStore::OrderList::Sequence_OrderList PetStore::OrderList_var::operator[](CORBA::ULong _index) {
  if (_ptr)
    return (*_ptr)[_index];
  VISTHROW(CORBA::BAD_PARAM())
#if defined(_VIS_NOEXCEPTIONS)
  VISRETURN(static ::PetStore::Order_ptr _tmp = (::PetStore::Order_ptr)NULL;
            return PetStore::OrderList::Sequence_OrderList(_tmp, (CORBA::Boolean)0);)
#endif
}

const PetStore::OrderList::Sequence_OrderList PetStore::OrderList_var::operator[](CORBA::ULong _index) const {
  if (_ptr)
    return (*(const PetStore::OrderList*)_ptr)[_index];
  VISTHROW(CORBA::BAD_PARAM())
#if defined(_VIS_NOEXCEPTIONS)
  VISRETURN(static ::PetStore::Order_ptr _tmp = (::PetStore::Order_ptr)NULL;
            return PetStore::OrderList::Sequence_OrderList(_tmp, (CORBA::Boolean) 0);)
#endif
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::OrderList_var& _var) {
  if (!_var._ptr)
    VISTHROW(CORBA::BAD_PARAM())
  else
    _strm << *_var._ptr;
  return _strm;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::OrderList_var& _var) {
  if (!_var._ptr)
    _var._ptr = new PetStore::OrderList;
  _strm >> *_var._ptr;
  return _strm;
}

PetStore::OrderHomeExplicit_ptr PetStore::OrderHomeExplicit_var::_duplicate(PetStore::OrderHomeExplicit_ptr _p) {
  return PetStore::OrderHomeExplicit::_duplicate(_p);
}

void
PetStore::OrderHomeExplicit_var::_release(PetStore::OrderHomeExplicit_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_p);
}

PetStore::OrderHomeExplicit_var::OrderHomeExplicit_var()
  : _ptr(PetStore::OrderHomeExplicit::_nil()) {}

PetStore::OrderHomeExplicit_var::OrderHomeExplicit_var(PetStore::OrderHomeExplicit_ptr _p)
  : _ptr(_p) {}

PetStore::OrderHomeExplicit_var::OrderHomeExplicit_var(const PetStore::OrderHomeExplicit_var& _var)
  : _ptr(PetStore::OrderHomeExplicit::_duplicate((PetStore::OrderHomeExplicit_ptr) _var)) {}

PetStore::OrderHomeExplicit_var::~OrderHomeExplicit_var() {
  CORBA::release((CORBA::Object_ptr)_ptr);
}

PetStore::OrderHomeExplicit_var&
PetStore::OrderHomeExplicit_var::operator=(const PetStore::OrderHomeExplicit_var& _var) {
    _release(_ptr);
    _ptr = _duplicate(_var._ptr);
    return *this;
}

PetStore::OrderHomeExplicit_var&
PetStore::OrderHomeExplicit_var::operator=(PetStore::OrderHomeExplicit_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_ptr);
  _ptr = _p;
  return *this;
}

PetStore::OrderHomeExplicit_ptr& PetStore::OrderHomeExplicit_var::out() {
  _release(_ptr);
  _ptr = (PetStore::OrderHomeExplicit_ptr)NULL;
  return _ptr;
}

PetStore::OrderHomeExplicit_ptr PetStore::OrderHomeExplicit_var::_retn() {
  PetStore::OrderHomeExplicit_ptr _tmp_ptr = _ptr;
  _ptr = (PetStore::OrderHomeExplicit_ptr)NULL;
  return _tmp_ptr;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::OrderHomeExplicit_var& _var) {
  _strm >> _var._ptr;
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::OrderHomeExplicit_var& _var) {
  _strm << _var._ptr;
  return _strm;
}

Istream& PetStore::_marshal_out(Istream& _strm, PetStore::OrderHomeExplicit_var& _var) {
  VISistream _istrm(_strm);
  _istrm >> _var._ptr;
  return _strm;
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::OrderHomeExplicit_var& _var) {
  _strm << (CORBA::Object_ptr)_var._ptr;
  return _strm;
}

// If dllimport is specified, you might probably want to
// disable these definitions
// disable the const definitions
const VISOps_Info PetStore::OrderHomeExplicit_ops::_ops_info("PetStore::OrderHomeExplicit_ops");
const VISOps_Info *PetStore::OrderHomeExplicit_ops::_desc() { return &_ops_info; }
PetStore::OrderHomeExplicit_ops_ptr PetStore::OrderHomeExplicit_ops::_downcast(PortableServer::ServantBase* _servant) {
  if (_servant == (PortableServer::ServantBase*)NULL)
    return PetStore::OrderHomeExplicit_ops::_nil();
  return (PetStore::OrderHomeExplicit_ops_ptr)_servant->_safe_downcast_ops(_ops_info);
}
const CORBA::TypeInfo
#ifdef AIXV3
PetStore::OrderHomeExplicit::_class_info("PetStore::OrderHomeExplicit",
                                         "IDL:PetStore/OrderHomeExplicit:1.0",
                                         NULL,
                                         PetStore::OrderHomeExplicit::_factory,
                                         NULL, 0, NULL, 0,
                                         Components::CCMHome::_desc(), 0L);
#else
PetStore::OrderHomeExplicit::_class_info("PetStore::OrderHomeExplicit",
                                         "IDL:PetStore/OrderHomeExplicit:1.0",
                                         NULL,
                                         PetStore::OrderHomeExplicit::_factory,
                                         NULL, 0, NULL, 0,
                                         Components::CCMHome::_desc(), 0L);
#endif

const CORBA::TypeInfo *PetStore::OrderHomeExplicit::_desc() { return &_class_info; }
const CORBA::TypeInfo *PetStore::OrderHomeExplicit::_type_info() const {
  return &_class_info;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::OrderHomeExplicit_ptr& _obj) {
  CORBA::Object_var _var_obj(_obj);
  _var_obj = CORBA::Object::_read(_strm, PetStore::OrderHomeExplicit::_desc());
  _obj = PetStore::OrderHomeExplicit::_narrow(_var_obj);
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::OrderHomeExplicit_ptr _obj) {
  _strm << (CORBA::Object_ptr)_obj;
  return _strm;
}
void* PetStore::OrderHomeExplicit::_safe_narrow(const CORBA::TypeInfo& _info) const {
  if (_info == _class_info)
    return (void *)this;
  if (_info == *Components::CCMHome::_desc())
    return (void *) (Components::CCMHome *) this;
  return CORBA_Object::_safe_narrow(_info);
}

CORBA::Object *PetStore::OrderHomeExplicit::_factory() {
  return new PetStore::OrderHomeExplicit;
}

PetStore::OrderHomeExplicit_ptr PetStore::OrderHomeExplicit::_this() {
  return PetStore::OrderHomeExplicit::_duplicate(this);
}

PetStore::OrderHomeExplicit_ptr PetStore::OrderHomeExplicit::_narrow(CORBA::Object * _obj) {
  if (_obj == PetStore::OrderHomeExplicit::_nil())
    return PetStore::OrderHomeExplicit::_nil();
  else
    return PetStore::OrderHomeExplicit::_duplicate((PetStore::OrderHomeExplicit_ptr)_obj->_safe_narrow(_class_info));
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::OrderHomeExplicit_ptr _obj) {
  _strm << (CORBA::Object_ptr) _obj;
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::OrderHomeExplicit_ptr& _obj) {
  VISistream _istrm(_strm);
  _istrm >> _obj;
  return _strm;
}

PetStore::OrderHomeExplicit *PetStore::OrderHomeExplicit::_bind(const char *_object_name,
                                                                const char *_host_name,
                                                                const CORBA::BindOptions *_opt,
                                                                CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
          "IDL:PetStore/OrderHomeExplicit:1.0", _object_name, _host_name,
_opt, _orb);
  return OrderHomeExplicit::_narrow(_obj);
}

PetStore::OrderHomeExplicit *PetStore::OrderHomeExplicit::_bind(const char *_poa_name,
                                                                const CORBA::OctetSequence& _id,
                                                                const char *_host_name,
                                                                const CORBA::BindOptions *_opt,
                                                                CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
   "IDL:PetStore/OrderHomeExplicit:1.0", _poa_name, _id, _host_name, _opt,
   _orb);
  return OrderHomeExplicit::_narrow(_obj);
}
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
::PetStore::OrderList* PetStore::OrderHomeExplicit::findAllOrders() {
  ::PetStore::OrderList* _ret = (::PetStore::OrderList_ptr) NULL;
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("findAllOrders");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::OrderHomeExplicit_ops*  _orderhomeexplicit = PetStore::OrderHomeExplicit_ops::_downcast(_servant);
      if (!_orderhomeexplicit) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "findAllOrders");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }

      VISTRY {
         _ret = _orderhomeexplicit->findAllOrders();
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "findAllOrders"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "findAllOrders");
      return _ret;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("findAllOrders", 1);
    VISIF_EXCEP(return _ret;)

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return _ret;)
#endif
  }

  VISistream& _istrm = *VISistream::_downcast(_ibuf);
  _istrm >> _ret;
  return _ret;
}

#if defined(__BORLANDC__)
 #pragma option pop
#endif

PetStore::OrderHomeImplicit_ptr PetStore::OrderHomeImplicit_var::_duplicate(PetStore::OrderHomeImplicit_ptr _p) {
  return PetStore::OrderHomeImplicit::_duplicate(_p);
}

void
PetStore::OrderHomeImplicit_var::_release(PetStore::OrderHomeImplicit_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_p);
}

PetStore::OrderHomeImplicit_var::OrderHomeImplicit_var()
  : _ptr(PetStore::OrderHomeImplicit::_nil()) {}

PetStore::OrderHomeImplicit_var::OrderHomeImplicit_var(PetStore::OrderHomeImplicit_ptr _p)
  : _ptr(_p) {}

PetStore::OrderHomeImplicit_var::OrderHomeImplicit_var(const PetStore::OrderHomeImplicit_var& _var)
  : _ptr(PetStore::OrderHomeImplicit::_duplicate((PetStore::OrderHomeImplicit_ptr) _var)) {}

PetStore::OrderHomeImplicit_var::~OrderHomeImplicit_var() {
  CORBA::release((CORBA::Object_ptr)_ptr);
}

PetStore::OrderHomeImplicit_var&
PetStore::OrderHomeImplicit_var::operator=(const PetStore::OrderHomeImplicit_var& _var) {
    _release(_ptr);
    _ptr = _duplicate(_var._ptr);
    return *this;
}

PetStore::OrderHomeImplicit_var&
PetStore::OrderHomeImplicit_var::operator=(PetStore::OrderHomeImplicit_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_ptr);
  _ptr = _p;
  return *this;
}

PetStore::OrderHomeImplicit_ptr& PetStore::OrderHomeImplicit_var::out() {
  _release(_ptr);
  _ptr = (PetStore::OrderHomeImplicit_ptr)NULL;
  return _ptr;
}

PetStore::OrderHomeImplicit_ptr PetStore::OrderHomeImplicit_var::_retn() {
  PetStore::OrderHomeImplicit_ptr _tmp_ptr = _ptr;
  _ptr = (PetStore::OrderHomeImplicit_ptr)NULL;
  return _tmp_ptr;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::OrderHomeImplicit_var& _var) {
  _strm >> _var._ptr;
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::OrderHomeImplicit_var& _var) {
  _strm << _var._ptr;
  return _strm;
}

Istream& PetStore::_marshal_out(Istream& _strm, PetStore::OrderHomeImplicit_var& _var) {
  VISistream _istrm(_strm);
  _istrm >> _var._ptr;
  return _strm;
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::OrderHomeImplicit_var& _var) {
  _strm << (CORBA::Object_ptr)_var._ptr;
  return _strm;
}

// If dllimport is specified, you might probably want to
// disable these definitions
// disable the const definitions
const VISOps_Info PetStore::OrderHomeImplicit_ops::_ops_info("PetStore::OrderHomeImplicit_ops");
const VISOps_Info *PetStore::OrderHomeImplicit_ops::_desc() { return &_ops_info; }
PetStore::OrderHomeImplicit_ops_ptr PetStore::OrderHomeImplicit_ops::_downcast(PortableServer::ServantBase* _servant) {
  if (_servant == (PortableServer::ServantBase*)NULL)
    return PetStore::OrderHomeImplicit_ops::_nil();
  return (PetStore::OrderHomeImplicit_ops_ptr)_servant->_safe_downcast_ops(_ops_info);
}
const CORBA::TypeInfo
#ifdef AIXV3
PetStore::OrderHomeImplicit::_class_info("PetStore::OrderHomeImplicit",
                                         "IDL:PetStore/OrderHomeImplicit:1.0",
                                         NULL,
                                         PetStore::OrderHomeImplicit::_factory,
                                         NULL, 0, NULL, 0,
                                         CORBA::Object::_desc(), 0L);
#else
PetStore::OrderHomeImplicit::_class_info("PetStore::OrderHomeImplicit",
                                         "IDL:PetStore/OrderHomeImplicit:1.0",
                                         NULL,
                                         PetStore::OrderHomeImplicit::_factory,
                                         NULL, 0, NULL, 0,
                                         CORBA::Object::_desc(), 0L);
#endif

const CORBA::TypeInfo *PetStore::OrderHomeImplicit::_desc() { return &_class_info; }
const CORBA::TypeInfo *PetStore::OrderHomeImplicit::_type_info() const {
  return &_class_info;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::OrderHomeImplicit_ptr& _obj) {
  CORBA::Object_var _var_obj(_obj);
  _var_obj = CORBA::Object::_read(_strm, PetStore::OrderHomeImplicit::_desc());
  _obj = PetStore::OrderHomeImplicit::_narrow(_var_obj);
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::OrderHomeImplicit_ptr _obj) {
  _strm << (CORBA::Object_ptr)_obj;
  return _strm;
}
void* PetStore::OrderHomeImplicit::_safe_narrow(const CORBA::TypeInfo& _info) const {
  if (_info == _class_info)
    return (void *)this;
  return CORBA_Object::_safe_narrow(_info);
}

CORBA::Object *PetStore::OrderHomeImplicit::_factory() {
  return new PetStore::OrderHomeImplicit;
}

PetStore::OrderHomeImplicit_ptr PetStore::OrderHomeImplicit::_this() {
  return PetStore::OrderHomeImplicit::_duplicate(this);
}

PetStore::OrderHomeImplicit_ptr PetStore::OrderHomeImplicit::_narrow(CORBA::Object * _obj) {
  if (_obj == PetStore::OrderHomeImplicit::_nil())
    return PetStore::OrderHomeImplicit::_nil();
  else
    return PetStore::OrderHomeImplicit::_duplicate((PetStore::OrderHomeImplicit_ptr)_obj->_safe_narrow(_class_info));
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::OrderHomeImplicit_ptr _obj) {
  _strm << (CORBA::Object_ptr) _obj;
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::OrderHomeImplicit_ptr& _obj) {
  VISistream _istrm(_strm);
  _istrm >> _obj;
  return _strm;
}

PetStore::OrderHomeImplicit *PetStore::OrderHomeImplicit::_bind(const char *_object_name,
                                                                const char *_host_name,
                                                                const CORBA::BindOptions *_opt,
                                                                CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
          "IDL:PetStore/OrderHomeImplicit:1.0", _object_name, _host_name,
_opt, _orb);
  return OrderHomeImplicit::_narrow(_obj);
}

PetStore::OrderHomeImplicit *PetStore::OrderHomeImplicit::_bind(const char *_poa_name,
                                                                const CORBA::OctetSequence& _id,
                                                                const char *_host_name,
                                                                const CORBA::BindOptions *_opt,
                                                                CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
   "IDL:PetStore/OrderHomeImplicit:1.0", _poa_name, _id, _host_name, _opt,
   _orb);
  return OrderHomeImplicit::_narrow(_obj);
}
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
::PetStore::Order_ptr PetStore::OrderHomeImplicit::create(::PetStore::keyID_ptr _arg_pkey ) {
  ::PetStore::Order_ptr _ret = (::PetStore::Order_ptr) NULL;
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("create");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::OrderHomeImplicit_ops*  _orderhomeimplicit = PetStore::OrderHomeImplicit_ops::_downcast(_servant);
      if (!_orderhomeimplicit) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "create");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }

      VISTRY {
         _ret = _orderhomeimplicit->create(_arg_pkey );
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "create"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "create");
      return _ret;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("create", 1);
    VISIF_EXCEP(return _ret;)
    VISostream& _ostrm = *VISostream::_downcast(_obuf);
    _ostrm << _arg_pkey;

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return _ret;)
#endif
  }

  VISistream& _istrm = *VISistream::_downcast(_ibuf);
  _istrm >> _ret;
  return _ret;
}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
::PetStore::Order_ptr PetStore::OrderHomeImplicit::find_by_primary_key(::PetStore::keyID_ptr _arg_pkey ) {
  ::PetStore::Order_ptr _ret = (::PetStore::Order_ptr) NULL;
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("find_by_primary_key");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::OrderHomeImplicit_ops*  _orderhomeimplicit = PetStore::OrderHomeImplicit_ops::_downcast(_servant);
      if (!_orderhomeimplicit) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "find_by_primary_key");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }

      VISTRY {
         _ret = _orderhomeimplicit->find_by_primary_key(_arg_pkey );
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "find_by_primary_key"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "find_by_primary_key");
      return _ret;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("find_by_primary_key", 1);
    VISIF_EXCEP(return _ret;)
    VISostream& _ostrm = *VISostream::_downcast(_obuf);
    _ostrm << _arg_pkey;

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return _ret;)
#endif
  }

  VISistream& _istrm = *VISistream::_downcast(_ibuf);
  _istrm >> _ret;
  return _ret;
}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
void PetStore::OrderHomeImplicit::remove(::PetStore::keyID_ptr _arg_pkey ) {
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("remove");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::OrderHomeImplicit_ops*  _orderhomeimplicit = PetStore::OrderHomeImplicit_ops::_downcast(_servant);
      if (!_orderhomeimplicit) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "remove");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }

      VISTRY {
        _orderhomeimplicit->remove(_arg_pkey );
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "remove"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "remove");
      return;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("remove", 1);
    VISIF_EXCEP(return;)
    VISostream& _ostrm = *VISostream::_downcast(_obuf);
    _ostrm << _arg_pkey;

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return;)
#endif
  }

}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
::PetStore::keyID_ptr PetStore::OrderHomeImplicit::get_primary_key(::PetStore::Order_ptr _arg_comp ) {
  ::PetStore::keyID_ptr _ret = (::PetStore::keyID_ptr) NULL;
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("get_primary_key");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::OrderHomeImplicit_ops*  _orderhomeimplicit = PetStore::OrderHomeImplicit_ops::_downcast(_servant);
      if (!_orderhomeimplicit) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "get_primary_key");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }

      VISTRY {
         _ret = _orderhomeimplicit->get_primary_key(_arg_comp );
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "get_primary_key"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "get_primary_key");
      return _ret;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("get_primary_key", 1);
    VISIF_EXCEP(return _ret;)
    VISostream& _ostrm = *VISostream::_downcast(_obuf);
    _ostrm << _arg_comp;

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return _ret;)
#endif
  }

  VISistream& _istrm = *VISistream::_downcast(_ibuf);
  _istrm >> _ret;
  return _ret;
}

#if defined(__BORLANDC__)
 #pragma option pop
#endif

PetStore::OrderHome_ptr PetStore::OrderHome_var::_duplicate(PetStore::OrderHome_ptr _p) {
  return PetStore::OrderHome::_duplicate(_p);
}

void
PetStore::OrderHome_var::_release(PetStore::OrderHome_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_p);
}

PetStore::OrderHome_var::OrderHome_var()
  : _ptr(PetStore::OrderHome::_nil()) {}

PetStore::OrderHome_var::OrderHome_var(PetStore::OrderHome_ptr _p)
  : _ptr(_p) {}

PetStore::OrderHome_var::OrderHome_var(const PetStore::OrderHome_var& _var)
  : _ptr(PetStore::OrderHome::_duplicate((PetStore::OrderHome_ptr) _var)) {}

PetStore::OrderHome_var::~OrderHome_var() {
  CORBA::release((CORBA::Object_ptr)_ptr);
}

PetStore::OrderHome_var&
PetStore::OrderHome_var::operator=(const PetStore::OrderHome_var& _var) {
    _release(_ptr);
    _ptr = _duplicate(_var._ptr);
    return *this;
}

PetStore::OrderHome_var&
PetStore::OrderHome_var::operator=(PetStore::OrderHome_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_ptr);
  _ptr = _p;
  return *this;
}

PetStore::OrderHome_ptr& PetStore::OrderHome_var::out() {
  _release(_ptr);
  _ptr = (PetStore::OrderHome_ptr)NULL;
  return _ptr;
}

PetStore::OrderHome_ptr PetStore::OrderHome_var::_retn() {
  PetStore::OrderHome_ptr _tmp_ptr = _ptr;
  _ptr = (PetStore::OrderHome_ptr)NULL;
  return _tmp_ptr;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::OrderHome_var& _var) {
  _strm >> _var._ptr;
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::OrderHome_var& _var) {
  _strm << _var._ptr;
  return _strm;
}

Istream& PetStore::_marshal_out(Istream& _strm, PetStore::OrderHome_var& _var) {
  VISistream _istrm(_strm);
  _istrm >> _var._ptr;
  return _strm;
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::OrderHome_var& _var) {
  _strm << (CORBA::Object_ptr)_var._ptr;
  return _strm;
}

// If dllimport is specified, you might probably want to
// disable these definitions
// disable the const definitions
const VISOps_Info PetStore::OrderHome_ops::_ops_info("PetStore::OrderHome_ops");
const VISOps_Info *PetStore::OrderHome_ops::_desc() { return &_ops_info; }
PetStore::OrderHome_ops_ptr PetStore::OrderHome_ops::_downcast(PortableServer::ServantBase* _servant) {
  if (_servant == (PortableServer::ServantBase*)NULL)
    return PetStore::OrderHome_ops::_nil();
  return (PetStore::OrderHome_ops_ptr)_servant->_safe_downcast_ops(_ops_info);
}
const CORBA::TypeInfo
#ifdef AIXV3
PetStore::OrderHome::_class_info("PetStore::OrderHome",
                                 "IDL:PetStore/OrderHome:1.0", NULL,
                                 PetStore::OrderHome::_factory, NULL, 0, NULL,
                                 0, PetStore::OrderHomeExplicit::_desc(),
                                 PetStore::OrderHomeImplicit::_desc(), 0L);
#else
PetStore::OrderHome::_class_info("PetStore::OrderHome",
                                 "IDL:PetStore/OrderHome:1.0", NULL,
                                 PetStore::OrderHome::_factory, NULL, 0, NULL,
                                 0, PetStore::OrderHomeExplicit::_desc(),
                                 PetStore::OrderHomeImplicit::_desc(), 0L);
#endif

const CORBA::TypeInfo *PetStore::OrderHome::_desc() { return &_class_info; }
const CORBA::TypeInfo *PetStore::OrderHome::_type_info() const {
  return &_class_info;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::OrderHome_ptr& _obj) {
  CORBA::Object_var _var_obj(_obj);
  _var_obj = CORBA::Object::_read(_strm, PetStore::OrderHome::_desc());
  _obj = PetStore::OrderHome::_narrow(_var_obj);
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::OrderHome_ptr _obj) {
  _strm << (CORBA::Object_ptr)_obj;
  return _strm;
}
void* PetStore::OrderHome::_safe_narrow(const CORBA::TypeInfo& _info) const {
  if (_info == _class_info)
    return (void *)this;
  if (_info == *PetStore::OrderHomeExplicit::_desc())
    return (void *) (PetStore::OrderHomeExplicit *) this;
  if (_info == *Components::CCMHome::_desc())
    return (void *) (Components::CCMHome *) this;
  if (_info == *PetStore::OrderHomeImplicit::_desc())
    return (void *) (PetStore::OrderHomeImplicit *) this;
  return CORBA_Object::_safe_narrow(_info);
}

CORBA::Object *PetStore::OrderHome::_factory() {
  return new PetStore::OrderHome;
}

PetStore::OrderHome_ptr PetStore::OrderHome::_this() {
  return PetStore::OrderHome::_duplicate(this);
}

PetStore::OrderHome_ptr PetStore::OrderHome::_narrow(CORBA::Object * _obj) {
  if (_obj == PetStore::OrderHome::_nil())
    return PetStore::OrderHome::_nil();
  else
    return PetStore::OrderHome::_duplicate((PetStore::OrderHome_ptr)_obj->_safe_narrow(_class_info));
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::OrderHome_ptr _obj) {
  _strm << (CORBA::Object_ptr) _obj;
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::OrderHome_ptr& _obj) {
  VISistream _istrm(_strm);
  _istrm >> _obj;
  return _strm;
}

PetStore::OrderHome *PetStore::OrderHome::_bind(const char *_object_name,
                                                const char *_host_name,
                                                const CORBA::BindOptions *_opt,
                                                CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
          "IDL:PetStore/OrderHome:1.0", _object_name, _host_name, _opt, _orb);
  return OrderHome::_narrow(_obj);
}

PetStore::OrderHome *PetStore::OrderHome::_bind(const char *_poa_name,
                                                const CORBA::OctetSequence& _id,
                                                const char *_host_name,
                                                const CORBA::BindOptions *_opt,
                                                CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
   "IDL:PetStore/OrderHome:1.0", _poa_name, _id, _host_name, _opt, _orb);
  return OrderHome::_narrow(_obj);
}

PetStore::OrderItemHomeExplicit_ptr PetStore::OrderItemHomeExplicit_var::_duplicate(PetStore::OrderItemHomeExplicit_ptr _p) {
  return PetStore::OrderItemHomeExplicit::_duplicate(_p);
}

void
PetStore::OrderItemHomeExplicit_var::_release(PetStore::OrderItemHomeExplicit_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_p);
}

PetStore::OrderItemHomeExplicit_var::OrderItemHomeExplicit_var()
  : _ptr(PetStore::OrderItemHomeExplicit::_nil()) {}

PetStore::OrderItemHomeExplicit_var::OrderItemHomeExplicit_var(PetStore::OrderItemHomeExplicit_ptr _p)
  : _ptr(_p) {}

PetStore::OrderItemHomeExplicit_var::OrderItemHomeExplicit_var(const PetStore::OrderItemHomeExplicit_var& _var)
  : _ptr(PetStore::OrderItemHomeExplicit::_duplicate((PetStore::OrderItemHomeExplicit_ptr) _var)) {}

PetStore::OrderItemHomeExplicit_var::~OrderItemHomeExplicit_var() {
  CORBA::release((CORBA::Object_ptr)_ptr);
}

PetStore::OrderItemHomeExplicit_var&
PetStore::OrderItemHomeExplicit_var::operator=(const PetStore::OrderItemHomeExplicit_var& _var) {
    _release(_ptr);
    _ptr = _duplicate(_var._ptr);
    return *this;
}

PetStore::OrderItemHomeExplicit_var&
PetStore::OrderItemHomeExplicit_var::operator=(PetStore::OrderItemHomeExplicit_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_ptr);
  _ptr = _p;
  return *this;
}

PetStore::OrderItemHomeExplicit_ptr& PetStore::OrderItemHomeExplicit_var::out() {
  _release(_ptr);
  _ptr = (PetStore::OrderItemHomeExplicit_ptr)NULL;
  return _ptr;
}

PetStore::OrderItemHomeExplicit_ptr PetStore::OrderItemHomeExplicit_var::_retn() {
  PetStore::OrderItemHomeExplicit_ptr _tmp_ptr = _ptr;
  _ptr = (PetStore::OrderItemHomeExplicit_ptr)NULL;
  return _tmp_ptr;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::OrderItemHomeExplicit_var& _var) {
  _strm >> _var._ptr;
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::OrderItemHomeExplicit_var& _var) {
  _strm << _var._ptr;
  return _strm;
}

Istream& PetStore::_marshal_out(Istream& _strm, PetStore::OrderItemHomeExplicit_var& _var) {
  VISistream _istrm(_strm);
  _istrm >> _var._ptr;
  return _strm;
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::OrderItemHomeExplicit_var& _var) {
  _strm << (CORBA::Object_ptr)_var._ptr;
  return _strm;
}

// If dllimport is specified, you might probably want to
// disable these definitions
// disable the const definitions
const VISOps_Info PetStore::OrderItemHomeExplicit_ops::_ops_info("PetStore::OrderItemHomeExplicit_ops");
const VISOps_Info *PetStore::OrderItemHomeExplicit_ops::_desc() { return &_ops_info; }
PetStore::OrderItemHomeExplicit_ops_ptr PetStore::OrderItemHomeExplicit_ops::_downcast(PortableServer::ServantBase* _servant) {
  if (_servant == (PortableServer::ServantBase*)NULL)
    return PetStore::OrderItemHomeExplicit_ops::_nil();
  return (PetStore::OrderItemHomeExplicit_ops_ptr)_servant->_safe_downcast_ops(_ops_info);
}
const CORBA::TypeInfo
#ifdef AIXV3
PetStore::OrderItemHomeExplicit::_class_info("PetStore::OrderItemHomeExplicit",
                                             "IDL:PetStore/OrderItemHomeExplicit:1.0",
                                             NULL,
                                             PetStore::OrderItemHomeExplicit::_factory,
                                             NULL, 0, NULL, 0,
                                             Components::CCMHome::_desc(), 0L);
#else
PetStore::OrderItemHomeExplicit::_class_info("PetStore::OrderItemHomeExplicit",
                                             "IDL:PetStore/OrderItemHomeExplicit:1.0",
                                             NULL,
                                             PetStore::OrderItemHomeExplicit::_factory,
                                             NULL, 0, NULL, 0,
                                             Components::CCMHome::_desc(), 0L);
#endif

const CORBA::TypeInfo *PetStore::OrderItemHomeExplicit::_desc() { return &_class_info; }
const CORBA::TypeInfo *PetStore::OrderItemHomeExplicit::_type_info() const {
  return &_class_info;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::OrderItemHomeExplicit_ptr& _obj) {
  CORBA::Object_var _var_obj(_obj);
  _var_obj = CORBA::Object::_read(_strm, PetStore::OrderItemHomeExplicit::_desc());
  _obj = PetStore::OrderItemHomeExplicit::_narrow(_var_obj);
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::OrderItemHomeExplicit_ptr _obj) {
  _strm << (CORBA::Object_ptr)_obj;
  return _strm;
}
void* PetStore::OrderItemHomeExplicit::_safe_narrow(const CORBA::TypeInfo& _info) const {
  if (_info == _class_info)
    return (void *)this;
  if (_info == *Components::CCMHome::_desc())
    return (void *) (Components::CCMHome *) this;
  return CORBA_Object::_safe_narrow(_info);
}

CORBA::Object *PetStore::OrderItemHomeExplicit::_factory() {
  return new PetStore::OrderItemHomeExplicit;
}

PetStore::OrderItemHomeExplicit_ptr PetStore::OrderItemHomeExplicit::_this() {
  return PetStore::OrderItemHomeExplicit::_duplicate(this);
}

PetStore::OrderItemHomeExplicit_ptr PetStore::OrderItemHomeExplicit::_narrow(CORBA::Object * _obj) {
  if (_obj == PetStore::OrderItemHomeExplicit::_nil())
    return PetStore::OrderItemHomeExplicit::_nil();
  else
    return PetStore::OrderItemHomeExplicit::_duplicate((PetStore::OrderItemHomeExplicit_ptr)_obj->_safe_narrow(_class_info));
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::OrderItemHomeExplicit_ptr _obj) {
  _strm << (CORBA::Object_ptr) _obj;
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::OrderItemHomeExplicit_ptr& _obj) {
  VISistream _istrm(_strm);
  _istrm >> _obj;
  return _strm;
}

PetStore::OrderItemHomeExplicit *PetStore::OrderItemHomeExplicit::_bind(const char *_object_name,
                                                                        const char *_host_name,
                                                                        const CORBA::BindOptions *_opt,
                                                                        CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
          "IDL:PetStore/OrderItemHomeExplicit:1.0", _object_name, _host_name,
_opt, _orb);
  return OrderItemHomeExplicit::_narrow(_obj);
}

PetStore::OrderItemHomeExplicit *PetStore::OrderItemHomeExplicit::_bind(const char *_poa_name,
                                                                        const CORBA::OctetSequence& _id,
                                                                        const char *_host_name,
                                                                        const CORBA::BindOptions *_opt,
                                                                        CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
   "IDL:PetStore/OrderItemHomeExplicit:1.0", _poa_name, _id, _host_name, _opt,
   _orb);
  return OrderItemHomeExplicit::_narrow(_obj);
}

PetStore::OrderItemHomeImplicit_ptr PetStore::OrderItemHomeImplicit_var::_duplicate(PetStore::OrderItemHomeImplicit_ptr _p) {
  return PetStore::OrderItemHomeImplicit::_duplicate(_p);
}

void
PetStore::OrderItemHomeImplicit_var::_release(PetStore::OrderItemHomeImplicit_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_p);
}

PetStore::OrderItemHomeImplicit_var::OrderItemHomeImplicit_var()
  : _ptr(PetStore::OrderItemHomeImplicit::_nil()) {}

PetStore::OrderItemHomeImplicit_var::OrderItemHomeImplicit_var(PetStore::OrderItemHomeImplicit_ptr _p)
  : _ptr(_p) {}

PetStore::OrderItemHomeImplicit_var::OrderItemHomeImplicit_var(const PetStore::OrderItemHomeImplicit_var& _var)
  : _ptr(PetStore::OrderItemHomeImplicit::_duplicate((PetStore::OrderItemHomeImplicit_ptr) _var)) {}

PetStore::OrderItemHomeImplicit_var::~OrderItemHomeImplicit_var() {
  CORBA::release((CORBA::Object_ptr)_ptr);
}

PetStore::OrderItemHomeImplicit_var&
PetStore::OrderItemHomeImplicit_var::operator=(const PetStore::OrderItemHomeImplicit_var& _var) {
    _release(_ptr);
    _ptr = _duplicate(_var._ptr);
    return *this;
}

PetStore::OrderItemHomeImplicit_var&
PetStore::OrderItemHomeImplicit_var::operator=(PetStore::OrderItemHomeImplicit_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_ptr);
  _ptr = _p;
  return *this;
}

PetStore::OrderItemHomeImplicit_ptr& PetStore::OrderItemHomeImplicit_var::out() {
  _release(_ptr);
  _ptr = (PetStore::OrderItemHomeImplicit_ptr)NULL;
  return _ptr;
}

PetStore::OrderItemHomeImplicit_ptr PetStore::OrderItemHomeImplicit_var::_retn() {
  PetStore::OrderItemHomeImplicit_ptr _tmp_ptr = _ptr;
  _ptr = (PetStore::OrderItemHomeImplicit_ptr)NULL;
  return _tmp_ptr;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::OrderItemHomeImplicit_var& _var) {
  _strm >> _var._ptr;
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::OrderItemHomeImplicit_var& _var) {
  _strm << _var._ptr;
  return _strm;
}

Istream& PetStore::_marshal_out(Istream& _strm, PetStore::OrderItemHomeImplicit_var& _var) {
  VISistream _istrm(_strm);
  _istrm >> _var._ptr;
  return _strm;
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::OrderItemHomeImplicit_var& _var) {
  _strm << (CORBA::Object_ptr)_var._ptr;
  return _strm;
}

// If dllimport is specified, you might probably want to
// disable these definitions
// disable the const definitions
const VISOps_Info PetStore::OrderItemHomeImplicit_ops::_ops_info("PetStore::OrderItemHomeImplicit_ops");
const VISOps_Info *PetStore::OrderItemHomeImplicit_ops::_desc() { return &_ops_info; }
PetStore::OrderItemHomeImplicit_ops_ptr PetStore::OrderItemHomeImplicit_ops::_downcast(PortableServer::ServantBase* _servant) {
  if (_servant == (PortableServer::ServantBase*)NULL)
    return PetStore::OrderItemHomeImplicit_ops::_nil();
  return (PetStore::OrderItemHomeImplicit_ops_ptr)_servant->_safe_downcast_ops(_ops_info);
}
const CORBA::TypeInfo
#ifdef AIXV3
PetStore::OrderItemHomeImplicit::_class_info("PetStore::OrderItemHomeImplicit",
                                             "IDL:PetStore/OrderItemHomeImplicit:1.0",
                                             NULL,
                                             PetStore::OrderItemHomeImplicit::_factory,
                                             NULL, 0, NULL, 0,
                                             CORBA::Object::_desc(), 0L);
#else
PetStore::OrderItemHomeImplicit::_class_info("PetStore::OrderItemHomeImplicit",
                                             "IDL:PetStore/OrderItemHomeImplicit:1.0",
                                             NULL,
                                             PetStore::OrderItemHomeImplicit::_factory,
                                             NULL, 0, NULL, 0,
                                             CORBA::Object::_desc(), 0L);
#endif

const CORBA::TypeInfo *PetStore::OrderItemHomeImplicit::_desc() { return &_class_info; }
const CORBA::TypeInfo *PetStore::OrderItemHomeImplicit::_type_info() const {
  return &_class_info;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::OrderItemHomeImplicit_ptr& _obj) {
  CORBA::Object_var _var_obj(_obj);
  _var_obj = CORBA::Object::_read(_strm, PetStore::OrderItemHomeImplicit::_desc());
  _obj = PetStore::OrderItemHomeImplicit::_narrow(_var_obj);
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::OrderItemHomeImplicit_ptr _obj) {
  _strm << (CORBA::Object_ptr)_obj;
  return _strm;
}
void* PetStore::OrderItemHomeImplicit::_safe_narrow(const CORBA::TypeInfo& _info) const {
  if (_info == _class_info)
    return (void *)this;
  return CORBA_Object::_safe_narrow(_info);
}

CORBA::Object *PetStore::OrderItemHomeImplicit::_factory() {
  return new PetStore::OrderItemHomeImplicit;
}

PetStore::OrderItemHomeImplicit_ptr PetStore::OrderItemHomeImplicit::_this() {
  return PetStore::OrderItemHomeImplicit::_duplicate(this);
}

PetStore::OrderItemHomeImplicit_ptr PetStore::OrderItemHomeImplicit::_narrow(CORBA::Object * _obj) {
  if (_obj == PetStore::OrderItemHomeImplicit::_nil())
    return PetStore::OrderItemHomeImplicit::_nil();
  else
    return PetStore::OrderItemHomeImplicit::_duplicate((PetStore::OrderItemHomeImplicit_ptr)_obj->_safe_narrow(_class_info));
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::OrderItemHomeImplicit_ptr _obj) {
  _strm << (CORBA::Object_ptr) _obj;
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::OrderItemHomeImplicit_ptr& _obj) {
  VISistream _istrm(_strm);
  _istrm >> _obj;
  return _strm;
}

PetStore::OrderItemHomeImplicit *PetStore::OrderItemHomeImplicit::_bind(const char *_object_name,
                                                                        const char *_host_name,
                                                                        const CORBA::BindOptions *_opt,
                                                                        CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
          "IDL:PetStore/OrderItemHomeImplicit:1.0", _object_name, _host_name,
_opt, _orb);
  return OrderItemHomeImplicit::_narrow(_obj);
}

PetStore::OrderItemHomeImplicit *PetStore::OrderItemHomeImplicit::_bind(const char *_poa_name,
                                                                        const CORBA::OctetSequence& _id,
                                                                        const char *_host_name,
                                                                        const CORBA::BindOptions *_opt,
                                                                        CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
   "IDL:PetStore/OrderItemHomeImplicit:1.0", _poa_name, _id, _host_name, _opt,
   _orb);
  return OrderItemHomeImplicit::_narrow(_obj);
}
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
::PetStore::OrderItem_ptr PetStore::OrderItemHomeImplicit::create(::PetStore::keyID_ptr _arg_pkey ) {
  ::PetStore::OrderItem_ptr _ret = (::PetStore::OrderItem_ptr) NULL;
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("create");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::OrderItemHomeImplicit_ops*  _orderitemhomeimplicit = PetStore::OrderItemHomeImplicit_ops::_downcast(_servant);
      if (!_orderitemhomeimplicit) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "create");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }

      VISTRY {
         _ret = _orderitemhomeimplicit->create(_arg_pkey );
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "create"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "create");
      return _ret;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("create", 1);
    VISIF_EXCEP(return _ret;)
    VISostream& _ostrm = *VISostream::_downcast(_obuf);
    _ostrm << _arg_pkey;

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return _ret;)
#endif
  }

  VISistream& _istrm = *VISistream::_downcast(_ibuf);
  _istrm >> _ret;
  return _ret;
}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
::PetStore::OrderItem_ptr PetStore::OrderItemHomeImplicit::find_by_primary_key(::PetStore::keyID_ptr _arg_pkey ) {
  ::PetStore::OrderItem_ptr _ret = (::PetStore::OrderItem_ptr) NULL;
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("find_by_primary_key");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::OrderItemHomeImplicit_ops*  _orderitemhomeimplicit = PetStore::OrderItemHomeImplicit_ops::_downcast(_servant);
      if (!_orderitemhomeimplicit) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "find_by_primary_key");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }

      VISTRY {
         _ret = _orderitemhomeimplicit->find_by_primary_key(_arg_pkey );
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "find_by_primary_key"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "find_by_primary_key");
      return _ret;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("find_by_primary_key", 1);
    VISIF_EXCEP(return _ret;)
    VISostream& _ostrm = *VISostream::_downcast(_obuf);
    _ostrm << _arg_pkey;

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return _ret;)
#endif
  }

  VISistream& _istrm = *VISistream::_downcast(_ibuf);
  _istrm >> _ret;
  return _ret;
}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
void PetStore::OrderItemHomeImplicit::remove(::PetStore::keyID_ptr _arg_pkey ) {
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("remove");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::OrderItemHomeImplicit_ops*  _orderitemhomeimplicit = PetStore::OrderItemHomeImplicit_ops::_downcast(_servant);
      if (!_orderitemhomeimplicit) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "remove");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }

      VISTRY {
        _orderitemhomeimplicit->remove(_arg_pkey );
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "remove"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "remove");
      return;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("remove", 1);
    VISIF_EXCEP(return;)
    VISostream& _ostrm = *VISostream::_downcast(_obuf);
    _ostrm << _arg_pkey;

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return;)
#endif
  }

}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
::PetStore::keyID_ptr PetStore::OrderItemHomeImplicit::get_primary_key(::PetStore::OrderItem_ptr _arg_comp ) {
  ::PetStore::keyID_ptr _ret = (::PetStore::keyID_ptr) NULL;
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("get_primary_key");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::OrderItemHomeImplicit_ops*  _orderitemhomeimplicit = PetStore::OrderItemHomeImplicit_ops::_downcast(_servant);
      if (!_orderitemhomeimplicit) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "get_primary_key");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }

      VISTRY {
         _ret = _orderitemhomeimplicit->get_primary_key(_arg_comp );
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "get_primary_key"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "get_primary_key");
      return _ret;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("get_primary_key", 1);
    VISIF_EXCEP(return _ret;)
    VISostream& _ostrm = *VISostream::_downcast(_obuf);
    _ostrm << _arg_comp;

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return _ret;)
#endif
  }

  VISistream& _istrm = *VISistream::_downcast(_ibuf);
  _istrm >> _ret;
  return _ret;
}

#if defined(__BORLANDC__)
 #pragma option pop
#endif

PetStore::OrderItemHome_ptr PetStore::OrderItemHome_var::_duplicate(PetStore::OrderItemHome_ptr _p) {
  return PetStore::OrderItemHome::_duplicate(_p);
}

void
PetStore::OrderItemHome_var::_release(PetStore::OrderItemHome_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_p);
}

PetStore::OrderItemHome_var::OrderItemHome_var()
  : _ptr(PetStore::OrderItemHome::_nil()) {}

PetStore::OrderItemHome_var::OrderItemHome_var(PetStore::OrderItemHome_ptr _p)
  : _ptr(_p) {}

PetStore::OrderItemHome_var::OrderItemHome_var(const PetStore::OrderItemHome_var& _var)
  : _ptr(PetStore::OrderItemHome::_duplicate((PetStore::OrderItemHome_ptr) _var)) {}

PetStore::OrderItemHome_var::~OrderItemHome_var() {
  CORBA::release((CORBA::Object_ptr)_ptr);
}

PetStore::OrderItemHome_var&
PetStore::OrderItemHome_var::operator=(const PetStore::OrderItemHome_var& _var) {
    _release(_ptr);
    _ptr = _duplicate(_var._ptr);
    return *this;
}

PetStore::OrderItemHome_var&
PetStore::OrderItemHome_var::operator=(PetStore::OrderItemHome_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_ptr);
  _ptr = _p;
  return *this;
}

PetStore::OrderItemHome_ptr& PetStore::OrderItemHome_var::out() {
  _release(_ptr);
  _ptr = (PetStore::OrderItemHome_ptr)NULL;
  return _ptr;
}

PetStore::OrderItemHome_ptr PetStore::OrderItemHome_var::_retn() {
  PetStore::OrderItemHome_ptr _tmp_ptr = _ptr;
  _ptr = (PetStore::OrderItemHome_ptr)NULL;
  return _tmp_ptr;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::OrderItemHome_var& _var) {
  _strm >> _var._ptr;
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::OrderItemHome_var& _var) {
  _strm << _var._ptr;
  return _strm;
}

Istream& PetStore::_marshal_out(Istream& _strm, PetStore::OrderItemHome_var& _var) {
  VISistream _istrm(_strm);
  _istrm >> _var._ptr;
  return _strm;
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::OrderItemHome_var& _var) {
  _strm << (CORBA::Object_ptr)_var._ptr;
  return _strm;
}

// If dllimport is specified, you might probably want to
// disable these definitions
// disable the const definitions
const VISOps_Info PetStore::OrderItemHome_ops::_ops_info("PetStore::OrderItemHome_ops");
const VISOps_Info *PetStore::OrderItemHome_ops::_desc() { return &_ops_info; }
PetStore::OrderItemHome_ops_ptr PetStore::OrderItemHome_ops::_downcast(PortableServer::ServantBase* _servant) {
  if (_servant == (PortableServer::ServantBase*)NULL)
    return PetStore::OrderItemHome_ops::_nil();
  return (PetStore::OrderItemHome_ops_ptr)_servant->_safe_downcast_ops(_ops_info);
}
const CORBA::TypeInfo
#ifdef AIXV3
PetStore::OrderItemHome::_class_info("PetStore::OrderItemHome",
                                     "IDL:PetStore/OrderItemHome:1.0", NULL,
                                     PetStore::OrderItemHome::_factory, NULL,
                                     0, NULL, 0,
                                     PetStore::OrderItemHomeExplicit::_desc(),
                                     PetStore::OrderItemHomeImplicit::_desc(),
                                     0L);
#else
PetStore::OrderItemHome::_class_info("PetStore::OrderItemHome",
                                     "IDL:PetStore/OrderItemHome:1.0", NULL,
                                     PetStore::OrderItemHome::_factory, NULL,
                                     0, NULL, 0,
                                     PetStore::OrderItemHomeExplicit::_desc(),
                                     PetStore::OrderItemHomeImplicit::_desc(),
                                     0L);
#endif

const CORBA::TypeInfo *PetStore::OrderItemHome::_desc() { return &_class_info; }
const CORBA::TypeInfo *PetStore::OrderItemHome::_type_info() const {
  return &_class_info;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::OrderItemHome_ptr& _obj) {
  CORBA::Object_var _var_obj(_obj);
  _var_obj = CORBA::Object::_read(_strm, PetStore::OrderItemHome::_desc());
  _obj = PetStore::OrderItemHome::_narrow(_var_obj);
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::OrderItemHome_ptr _obj) {
  _strm << (CORBA::Object_ptr)_obj;
  return _strm;
}
void* PetStore::OrderItemHome::_safe_narrow(const CORBA::TypeInfo& _info) const {
  if (_info == _class_info)
    return (void *)this;
  if (_info == *PetStore::OrderItemHomeExplicit::_desc())
    return (void *) (PetStore::OrderItemHomeExplicit *) this;
  if (_info == *Components::CCMHome::_desc())
    return (void *) (Components::CCMHome *) this;
  if (_info == *PetStore::OrderItemHomeImplicit::_desc())
    return (void *) (PetStore::OrderItemHomeImplicit *) this;
  return CORBA_Object::_safe_narrow(_info);
}

CORBA::Object *PetStore::OrderItemHome::_factory() {
  return new PetStore::OrderItemHome;
}

PetStore::OrderItemHome_ptr PetStore::OrderItemHome::_this() {
  return PetStore::OrderItemHome::_duplicate(this);
}

PetStore::OrderItemHome_ptr PetStore::OrderItemHome::_narrow(CORBA::Object * _obj) {
  if (_obj == PetStore::OrderItemHome::_nil())
    return PetStore::OrderItemHome::_nil();
  else
    return PetStore::OrderItemHome::_duplicate((PetStore::OrderItemHome_ptr)_obj->_safe_narrow(_class_info));
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::OrderItemHome_ptr _obj) {
  _strm << (CORBA::Object_ptr) _obj;
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::OrderItemHome_ptr& _obj) {
  VISistream _istrm(_strm);
  _istrm >> _obj;
  return _strm;
}

PetStore::OrderItemHome *PetStore::OrderItemHome::_bind(const char *_object_name,
                                                        const char *_host_name,
                                                        const CORBA::BindOptions *_opt,
                                                        CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
          "IDL:PetStore/OrderItemHome:1.0", _object_name, _host_name, _opt,
_orb);
  return OrderItemHome::_narrow(_obj);
}

PetStore::OrderItemHome *PetStore::OrderItemHome::_bind(const char *_poa_name,
                                                        const CORBA::OctetSequence& _id,
                                                        const char *_host_name,
                                                        const CORBA::BindOptions *_opt,
                                                        CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
   "IDL:PetStore/OrderItemHome:1.0", _poa_name, _id, _host_name, _opt, _orb);
  return OrderItemHome::_narrow(_obj);
}

PetStore::SalesReportConsumer_ptr PetStore::SalesReportConsumer_var::_duplicate(PetStore::SalesReportConsumer_ptr _p) {
  return PetStore::SalesReportConsumer::_duplicate(_p);
}

void
PetStore::SalesReportConsumer_var::_release(PetStore::SalesReportConsumer_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_p);
}

PetStore::SalesReportConsumer_var::SalesReportConsumer_var()
  : _ptr(PetStore::SalesReportConsumer::_nil()) {}

PetStore::SalesReportConsumer_var::SalesReportConsumer_var(PetStore::SalesReportConsumer_ptr _p)
  : _ptr(_p) {}

PetStore::SalesReportConsumer_var::SalesReportConsumer_var(const PetStore::SalesReportConsumer_var& _var)
  : _ptr(PetStore::SalesReportConsumer::_duplicate((PetStore::SalesReportConsumer_ptr) _var)) {}

PetStore::SalesReportConsumer_var::~SalesReportConsumer_var() {
  CORBA::release((CORBA::Object_ptr)_ptr);
}

PetStore::SalesReportConsumer_var&
PetStore::SalesReportConsumer_var::operator=(const PetStore::SalesReportConsumer_var& _var) {
    _release(_ptr);
    _ptr = _duplicate(_var._ptr);
    return *this;
}

PetStore::SalesReportConsumer_var&
PetStore::SalesReportConsumer_var::operator=(PetStore::SalesReportConsumer_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_ptr);
  _ptr = _p;
  return *this;
}

PetStore::SalesReportConsumer_ptr& PetStore::SalesReportConsumer_var::out() {
  _release(_ptr);
  _ptr = (PetStore::SalesReportConsumer_ptr)NULL;
  return _ptr;
}

PetStore::SalesReportConsumer_ptr PetStore::SalesReportConsumer_var::_retn() {
  PetStore::SalesReportConsumer_ptr _tmp_ptr = _ptr;
  _ptr = (PetStore::SalesReportConsumer_ptr)NULL;
  return _tmp_ptr;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::SalesReportConsumer_var& _var) {
  _strm >> _var._ptr;
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::SalesReportConsumer_var& _var) {
  _strm << _var._ptr;
  return _strm;
}

Istream& PetStore::_marshal_out(Istream& _strm, PetStore::SalesReportConsumer_var& _var) {
  VISistream _istrm(_strm);
  _istrm >> _var._ptr;
  return _strm;
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::SalesReportConsumer_var& _var) {
  _strm << (CORBA::Object_ptr)_var._ptr;
  return _strm;
}

// If dllimport is specified, you might probably want to
// disable these definitions
// disable the const definitions
const VISOps_Info PetStore::SalesReportConsumer_ops::_ops_info("PetStore::SalesReportConsumer_ops");
const VISOps_Info *PetStore::SalesReportConsumer_ops::_desc() { return &_ops_info; }
PetStore::SalesReportConsumer_ops_ptr PetStore::SalesReportConsumer_ops::_downcast(PortableServer::ServantBase* _servant) {
  if (_servant == (PortableServer::ServantBase*)NULL)
    return PetStore::SalesReportConsumer_ops::_nil();
  return (PetStore::SalesReportConsumer_ops_ptr)_servant->_safe_downcast_ops(_ops_info);
}
const CORBA::TypeInfo
#ifdef AIXV3
PetStore::SalesReportConsumer::_class_info("PetStore::SalesReportConsumer",
                                           "IDL:PetStore/SalesReportConsumer:1.0",
                                           NULL,
                                           PetStore::SalesReportConsumer::_factory,
                                           NULL, 0, NULL, 0,
                                           Components::EventConsumerBase::_desc(),
                                           0L);
#else
PetStore::SalesReportConsumer::_class_info("PetStore::SalesReportConsumer",
                                           "IDL:PetStore/SalesReportConsumer:1.0",
                                           NULL,
                                           PetStore::SalesReportConsumer::_factory,
                                           NULL, 0, NULL, 0,
                                           Components::EventConsumerBase::_desc(),
                                           0L);
#endif

const CORBA::TypeInfo *PetStore::SalesReportConsumer::_desc() { return &_class_info; }
const CORBA::TypeInfo *PetStore::SalesReportConsumer::_type_info() const {
  return &_class_info;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::SalesReportConsumer_ptr& _obj) {
  CORBA::Object_var _var_obj(_obj);
  _var_obj = CORBA::Object::_read(_strm, PetStore::SalesReportConsumer::_desc());
  _obj = PetStore::SalesReportConsumer::_narrow(_var_obj);
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::SalesReportConsumer_ptr _obj) {
  _strm << (CORBA::Object_ptr)_obj;
  return _strm;
}
void* PetStore::SalesReportConsumer::_safe_narrow(const CORBA::TypeInfo& _info) const {
  if (_info == _class_info)
    return (void *)this;
  if (_info == *Components::EventConsumerBase::_desc())
    return (void *) (Components::EventConsumerBase *) this;
  return CORBA_Object::_safe_narrow(_info);
}

CORBA::Object *PetStore::SalesReportConsumer::_factory() {
  return new PetStore::SalesReportConsumer;
}

PetStore::SalesReportConsumer_ptr PetStore::SalesReportConsumer::_this() {
  return PetStore::SalesReportConsumer::_duplicate(this);
}

PetStore::SalesReportConsumer_ptr PetStore::SalesReportConsumer::_narrow(CORBA::Object * _obj) {
  if (_obj == PetStore::SalesReportConsumer::_nil())
    return PetStore::SalesReportConsumer::_nil();
  else
    return PetStore::SalesReportConsumer::_duplicate((PetStore::SalesReportConsumer_ptr)_obj->_safe_narrow(_class_info));
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::SalesReportConsumer_ptr _obj) {
  _strm << (CORBA::Object_ptr) _obj;
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::SalesReportConsumer_ptr& _obj) {
  VISistream _istrm(_strm);
  _istrm >> _obj;
  return _strm;
}

PetStore::SalesReportConsumer *PetStore::SalesReportConsumer::_bind(const char *_object_name,
                                                                    const char *_host_name,
                                                                    const CORBA::BindOptions *_opt,
                                                                    CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
          "IDL:PetStore/SalesReportConsumer:1.0", _object_name, _host_name,
_opt, _orb);
  return SalesReportConsumer::_narrow(_obj);
}

PetStore::SalesReportConsumer *PetStore::SalesReportConsumer::_bind(const char *_poa_name,
                                                                    const CORBA::OctetSequence& _id,
                                                                    const char *_host_name,
                                                                    const CORBA::BindOptions *_opt,
                                                                    CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
   "IDL:PetStore/SalesReportConsumer:1.0", _poa_name, _id, _host_name, _opt,
   _orb);
  return SalesReportConsumer::_narrow(_obj);
}
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
void PetStore::SalesReportConsumer::push_SalesReportConsumer(::PetStore::SalesReport_ptr _arg_event ) {
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("push_SalesReportConsumer");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::SalesReportConsumer_ops*  _salesreportconsumer = PetStore::SalesReportConsumer_ops::_downcast(_servant);
      if (!_salesreportconsumer) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "push_SalesReportConsumer");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }

      VISTRY {
        _salesreportconsumer->push_SalesReportConsumer(_arg_event );
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "push_SalesReportConsumer"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "push_SalesReportConsumer");
      return;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("push_SalesReportConsumer", 1);
    VISIF_EXCEP(return;)
    VISostream& _ostrm = *VISostream::_downcast(_obuf);
    _ostrm << _arg_event;

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return;)
#endif
  }

}

#if defined(__BORLANDC__)
 #pragma option pop
#endif

PetStore::Sales_ptr PetStore::Sales_var::_duplicate(PetStore::Sales_ptr _p) {
  return PetStore::Sales::_duplicate(_p);
}

void
PetStore::Sales_var::_release(PetStore::Sales_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_p);
}

PetStore::Sales_var::Sales_var()
  : _ptr(PetStore::Sales::_nil()) {}

PetStore::Sales_var::Sales_var(PetStore::Sales_ptr _p)
  : _ptr(_p) {}

PetStore::Sales_var::Sales_var(const PetStore::Sales_var& _var)
  : _ptr(PetStore::Sales::_duplicate((PetStore::Sales_ptr) _var)) {}

PetStore::Sales_var::~Sales_var() {
  CORBA::release((CORBA::Object_ptr)_ptr);
}

PetStore::Sales_var&
PetStore::Sales_var::operator=(const PetStore::Sales_var& _var) {
    _release(_ptr);
    _ptr = _duplicate(_var._ptr);
    return *this;
}

PetStore::Sales_var&
PetStore::Sales_var::operator=(PetStore::Sales_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_ptr);
  _ptr = _p;
  return *this;
}

PetStore::Sales_ptr& PetStore::Sales_var::out() {
  _release(_ptr);
  _ptr = (PetStore::Sales_ptr)NULL;
  return _ptr;
}

PetStore::Sales_ptr PetStore::Sales_var::_retn() {
  PetStore::Sales_ptr _tmp_ptr = _ptr;
  _ptr = (PetStore::Sales_ptr)NULL;
  return _tmp_ptr;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::Sales_var& _var) {
  _strm >> _var._ptr;
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::Sales_var& _var) {
  _strm << _var._ptr;
  return _strm;
}

Istream& PetStore::_marshal_out(Istream& _strm, PetStore::Sales_var& _var) {
  VISistream _istrm(_strm);
  _istrm >> _var._ptr;
  return _strm;
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::Sales_var& _var) {
  _strm << (CORBA::Object_ptr)_var._ptr;
  return _strm;
}

// If dllimport is specified, you might probably want to
// disable these definitions
// disable the const definitions
const VISOps_Info PetStore::Sales_ops::_ops_info("PetStore::Sales_ops");
const VISOps_Info *PetStore::Sales_ops::_desc() { return &_ops_info; }
PetStore::Sales_ops_ptr PetStore::Sales_ops::_downcast(PortableServer::ServantBase* _servant) {
  if (_servant == (PortableServer::ServantBase*)NULL)
    return PetStore::Sales_ops::_nil();
  return (PetStore::Sales_ops_ptr)_servant->_safe_downcast_ops(_ops_info);
}
const CORBA::TypeInfo
#ifdef AIXV3
PetStore::Sales::_class_info("PetStore::Sales", "IDL:PetStore/Sales:1.0",
                             NULL, PetStore::Sales::_factory, NULL, 0, NULL,
                             0, Components::CCMObject::_desc(), 0L);
#else
PetStore::Sales::_class_info("PetStore::Sales", "IDL:PetStore/Sales:1.0",
                             NULL, PetStore::Sales::_factory, NULL, 0, NULL,
                             0, Components::CCMObject::_desc(), 0L);
#endif

const CORBA::TypeInfo *PetStore::Sales::_desc() { return &_class_info; }
const CORBA::TypeInfo *PetStore::Sales::_type_info() const {
  return &_class_info;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::Sales_ptr& _obj) {
  CORBA::Object_var _var_obj(_obj);
  _var_obj = CORBA::Object::_read(_strm, PetStore::Sales::_desc());
  _obj = PetStore::Sales::_narrow(_var_obj);
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::Sales_ptr _obj) {
  _strm << (CORBA::Object_ptr)_obj;
  return _strm;
}
void* PetStore::Sales::_safe_narrow(const CORBA::TypeInfo& _info) const {
  if (_info == _class_info)
    return (void *)this;
  if (_info == *Components::CCMObject::_desc())
    return (void *) (Components::CCMObject *) this;
  if (_info == *Components::Navigation::_desc())
    return (void *) (Components::Navigation *) this;
  if (_info == *Components::Receptacles::_desc())
    return (void *) (Components::Receptacles *) this;
  if (_info == *Components::Events::_desc())
    return (void *) (Components::Events *) this;
  return CORBA_Object::_safe_narrow(_info);
}

CORBA::Object *PetStore::Sales::_factory() {
  return new PetStore::Sales;
}

PetStore::Sales_ptr PetStore::Sales::_this() {
  return PetStore::Sales::_duplicate(this);
}

PetStore::Sales_ptr PetStore::Sales::_narrow(CORBA::Object * _obj) {
  if (_obj == PetStore::Sales::_nil())
    return PetStore::Sales::_nil();
  else
    return PetStore::Sales::_duplicate((PetStore::Sales_ptr)_obj->_safe_narrow(_class_info));
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::Sales_ptr _obj) {
  _strm << (CORBA::Object_ptr) _obj;
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::Sales_ptr& _obj) {
  VISistream _istrm(_strm);
  _istrm >> _obj;
  return _strm;
}

PetStore::Sales *PetStore::Sales::_bind(const char *_object_name,
                                        const char *_host_name,
                                        const CORBA::BindOptions *_opt,
                                        CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
          "IDL:PetStore/Sales:1.0", _object_name, _host_name, _opt, _orb);
  return Sales::_narrow(_obj);
}

PetStore::Sales *PetStore::Sales::_bind(const char *_poa_name,
                                        const CORBA::OctetSequence& _id,
                                        const char *_host_name,
                                        const CORBA::BindOptions *_opt,
                                        CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
   "IDL:PetStore/Sales:1.0", _poa_name, _id, _host_name, _opt, _orb);
  return Sales::_narrow(_obj);
}
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
::PetStore::BusinessOp_ptr PetStore::Sales::provide_for_client() {
  ::PetStore::BusinessOp_ptr _ret = (::PetStore::BusinessOp_ptr) NULL;
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("provide_for_client");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::Sales_ops*  _sales = PetStore::Sales_ops::_downcast(_servant);
      if (!_sales) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "provide_for_client");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }

      VISTRY {
         _ret = _sales->provide_for_client();
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "provide_for_client"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "provide_for_client");
      return _ret;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("provide_for_client", 1);
    VISIF_EXCEP(return _ret;)

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return _ret;)
#endif
  }

  VISistream& _istrm = *VISistream::_downcast(_ibuf);
  _istrm >> _ret;
  return _ret;
}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
::PetStore::AdvancedOp_ptr PetStore::Sales::provide_for_mgr() {
  ::PetStore::AdvancedOp_ptr _ret = (::PetStore::AdvancedOp_ptr) NULL;
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("provide_for_mgr");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::Sales_ops*  _sales = PetStore::Sales_ops::_downcast(_servant);
      if (!_sales) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "provide_for_mgr");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }

      VISTRY {
         _ret = _sales->provide_for_mgr();
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "provide_for_mgr"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "provide_for_mgr");
      return _ret;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("provide_for_mgr", 1);
    VISIF_EXCEP(return _ret;)

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return _ret;)
#endif
  }

  VISistream& _istrm = *VISistream::_downcast(_ibuf);
  _istrm >> _ret;
  return _ret;
}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
void PetStore::Sales::connect_shopping_cart(::PetStore::ShoppingCart_ptr _arg_conn ) {
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("connect_shopping_cart");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::Sales_ops*  _sales = PetStore::Sales_ops::_downcast(_servant);
      if (!_sales) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "connect_shopping_cart");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }

      VISTRY {
        _sales->connect_shopping_cart(_arg_conn );
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "connect_shopping_cart"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "connect_shopping_cart");
      return;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("connect_shopping_cart", 1);
    VISIF_EXCEP(return;)
    VISostream& _ostrm = *VISostream::_downcast(_obuf);
    _ostrm << _arg_conn;

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return;)
#endif
  }

}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
::PetStore::ShoppingCart_ptr PetStore::Sales::disconnect_shopping_cart() {
  ::PetStore::ShoppingCart_ptr _ret = (::PetStore::ShoppingCart_ptr) NULL;
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("disconnect_shopping_cart");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::Sales_ops*  _sales = PetStore::Sales_ops::_downcast(_servant);
      if (!_sales) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "disconnect_shopping_cart");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }

      VISTRY {
         _ret = _sales->disconnect_shopping_cart();
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "disconnect_shopping_cart"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "disconnect_shopping_cart");
      return _ret;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("disconnect_shopping_cart", 1);
    VISIF_EXCEP(return _ret;)

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return _ret;)
#endif
  }

  VISistream& _istrm = *VISistream::_downcast(_ibuf);
  _istrm >> _ret;
  return _ret;
}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
::PetStore::ShoppingCart_ptr PetStore::Sales::get_connection_shopping_cart() {
  ::PetStore::ShoppingCart_ptr _ret = (::PetStore::ShoppingCart_ptr) NULL;
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("get_connection_shopping_cart");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::Sales_ops*  _sales = PetStore::Sales_ops::_downcast(_servant);
      if (!_sales) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "get_connection_shopping_cart");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }

      VISTRY {
         _ret = _sales->get_connection_shopping_cart();
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "get_connection_shopping_cart"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "get_connection_shopping_cart");
      return _ret;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("get_connection_shopping_cart", 1);
    VISIF_EXCEP(return _ret;)

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return _ret;)
#endif
  }

  VISistream& _istrm = *VISistream::_downcast(_ibuf);
  _istrm >> _ret;
  return _ret;
}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
void PetStore::Sales::connect_product_home(::PetStore::ProductHome_ptr _arg_conn ) {
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("connect_product_home");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::Sales_ops*  _sales = PetStore::Sales_ops::_downcast(_servant);
      if (!_sales) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "connect_product_home");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }

      VISTRY {
        _sales->connect_product_home(_arg_conn );
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "connect_product_home"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "connect_product_home");
      return;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("connect_product_home", 1);
    VISIF_EXCEP(return;)
    VISostream& _ostrm = *VISostream::_downcast(_obuf);
    _ostrm << _arg_conn;

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return;)
#endif
  }

}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
::PetStore::ProductHome_ptr PetStore::Sales::disconnect_product_home() {
  ::PetStore::ProductHome_ptr _ret = (::PetStore::ProductHome_ptr) NULL;
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("disconnect_product_home");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::Sales_ops*  _sales = PetStore::Sales_ops::_downcast(_servant);
      if (!_sales) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "disconnect_product_home");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }

      VISTRY {
         _ret = _sales->disconnect_product_home();
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "disconnect_product_home"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "disconnect_product_home");
      return _ret;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("disconnect_product_home", 1);
    VISIF_EXCEP(return _ret;)

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return _ret;)
#endif
  }

  VISistream& _istrm = *VISistream::_downcast(_ibuf);
  _istrm >> _ret;
  return _ret;
}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
::PetStore::ProductHome_ptr PetStore::Sales::get_connection_product_home() {
  ::PetStore::ProductHome_ptr _ret = (::PetStore::ProductHome_ptr) NULL;
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("get_connection_product_home");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::Sales_ops*  _sales = PetStore::Sales_ops::_downcast(_servant);
      if (!_sales) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "get_connection_product_home");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }

      VISTRY {
         _ret = _sales->get_connection_product_home();
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "get_connection_product_home"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "get_connection_product_home");
      return _ret;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("get_connection_product_home", 1);
    VISIF_EXCEP(return _ret;)

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return _ret;)
#endif
  }

  VISistream& _istrm = *VISistream::_downcast(_ibuf);
  _istrm >> _ret;
  return _ret;
}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
void PetStore::Sales::connect_order_home(::PetStore::OrderHome_ptr _arg_conn ) {
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("connect_order_home");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::Sales_ops*  _sales = PetStore::Sales_ops::_downcast(_servant);
      if (!_sales) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "connect_order_home");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }

      VISTRY {
        _sales->connect_order_home(_arg_conn );
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "connect_order_home"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "connect_order_home");
      return;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("connect_order_home", 1);
    VISIF_EXCEP(return;)
    VISostream& _ostrm = *VISostream::_downcast(_obuf);
    _ostrm << _arg_conn;

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return;)
#endif
  }

}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
::PetStore::OrderHome_ptr PetStore::Sales::disconnect_order_home() {
  ::PetStore::OrderHome_ptr _ret = (::PetStore::OrderHome_ptr) NULL;
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("disconnect_order_home");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::Sales_ops*  _sales = PetStore::Sales_ops::_downcast(_servant);
      if (!_sales) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "disconnect_order_home");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }

      VISTRY {
         _ret = _sales->disconnect_order_home();
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "disconnect_order_home"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "disconnect_order_home");
      return _ret;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("disconnect_order_home", 1);
    VISIF_EXCEP(return _ret;)

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return _ret;)
#endif
  }

  VISistream& _istrm = *VISistream::_downcast(_ibuf);
  _istrm >> _ret;
  return _ret;
}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
::PetStore::OrderHome_ptr PetStore::Sales::get_connection_order_home() {
  ::PetStore::OrderHome_ptr _ret = (::PetStore::OrderHome_ptr) NULL;
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("get_connection_order_home");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::Sales_ops*  _sales = PetStore::Sales_ops::_downcast(_servant);
      if (!_sales) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "get_connection_order_home");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }

      VISTRY {
         _ret = _sales->get_connection_order_home();
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "get_connection_order_home"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "get_connection_order_home");
      return _ret;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("get_connection_order_home", 1);
    VISIF_EXCEP(return _ret;)

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return _ret;)
#endif
  }

  VISistream& _istrm = *VISistream::_downcast(_ibuf);
  _istrm >> _ret;
  return _ret;
}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
void PetStore::Sales::connect_order_Item_home(::PetStore::OrderItemHome_ptr _arg_conn ) {
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("connect_order_Item_home");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::Sales_ops*  _sales = PetStore::Sales_ops::_downcast(_servant);
      if (!_sales) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "connect_order_Item_home");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }

      VISTRY {
        _sales->connect_order_Item_home(_arg_conn );
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "connect_order_Item_home"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "connect_order_Item_home");
      return;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("connect_order_Item_home", 1);
    VISIF_EXCEP(return;)
    VISostream& _ostrm = *VISostream::_downcast(_obuf);
    _ostrm << _arg_conn;

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return;)
#endif
  }

}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
::PetStore::OrderItemHome_ptr PetStore::Sales::disconnect_order_Item_home() {
  ::PetStore::OrderItemHome_ptr _ret = (::PetStore::OrderItemHome_ptr) NULL;
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("disconnect_order_Item_home");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::Sales_ops*  _sales = PetStore::Sales_ops::_downcast(_servant);
      if (!_sales) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "disconnect_order_Item_home");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }

      VISTRY {
         _ret = _sales->disconnect_order_Item_home();
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "disconnect_order_Item_home"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "disconnect_order_Item_home");
      return _ret;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("disconnect_order_Item_home", 1);
    VISIF_EXCEP(return _ret;)

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return _ret;)
#endif
  }

  VISistream& _istrm = *VISistream::_downcast(_ibuf);
  _istrm >> _ret;
  return _ret;
}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
::PetStore::OrderItemHome_ptr PetStore::Sales::get_connection_order_Item_home() {
  ::PetStore::OrderItemHome_ptr _ret = (::PetStore::OrderItemHome_ptr) NULL;
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("get_connection_order_Item_home");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::Sales_ops*  _sales = PetStore::Sales_ops::_downcast(_servant);
      if (!_sales) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "get_connection_order_Item_home");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }

      VISTRY {
         _ret = _sales->get_connection_order_Item_home();
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "get_connection_order_Item_home"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "get_connection_order_Item_home");
      return _ret;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("get_connection_order_Item_home", 1);
    VISIF_EXCEP(return _ret;)

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return _ret;)
#endif
  }

  VISistream& _istrm = *VISistream::_downcast(_ibuf);
  _istrm >> _ret;
  return _ret;
}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
void PetStore::Sales::connect_report(::PetStore::SalesReportConsumer_ptr _arg_consumer ) {
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("connect_report");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::Sales_ops*  _sales = PetStore::Sales_ops::_downcast(_servant);
      if (!_sales) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "connect_report");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }

      VISTRY {
        _sales->connect_report(_arg_consumer );
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "connect_report"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "connect_report");
      return;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("connect_report", 1);
    VISIF_EXCEP(return;)
    VISostream& _ostrm = *VISostream::_downcast(_obuf);
    _ostrm << _arg_consumer;

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return;)
#endif
  }

}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
::PetStore::SalesReportConsumer_ptr PetStore::Sales::disconnect_report() {
  ::PetStore::SalesReportConsumer_ptr _ret = (::PetStore::SalesReportConsumer_ptr) NULL;
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("disconnect_report");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::Sales_ops*  _sales = PetStore::Sales_ops::_downcast(_servant);
      if (!_sales) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "disconnect_report");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }

      VISTRY {
         _ret = _sales->disconnect_report();
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "disconnect_report"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "disconnect_report");
      return _ret;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("disconnect_report", 1);
    VISIF_EXCEP(return _ret;)

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return _ret;)
#endif
  }

  VISistream& _istrm = *VISistream::_downcast(_ibuf);
  _istrm >> _ret;
  return _ret;
}

#if defined(__BORLANDC__)
 #pragma option pop
#endif

#if defined(MSVCNEWDLL_BUG)
void *PetStore::OrderIndexInfo::operator new(size_t ts) {
  return ::operator new(ts);
}
void PetStore::OrderIndexInfo::operator delete(void *p) {
  ::operator delete(p);
}
#endif // MSVCNEWDLL_BUG

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::OrderIndexInfo& _s) {
    _strm << _s.order_id;

    _strm << _s.subTotal;

  return _strm;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::OrderIndexInfo& _s) {
    _strm >> _s.order_id;

    _strm >> _s.subTotal;

  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::OrderIndexInfo* _obj) {
  if (!_obj)
    VISTHROW(CORBA::BAD_PARAM())
  else
    _strm << *_obj;
  return _strm;
}
VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::OrderIndexInfo*& _obj) {
  _obj = new PetStore::OrderIndexInfo();
  _strm >> *_obj;
  return _strm;
}

void PetStore::_internal_pretty_print(VISostream& _strm, const PetStore::OrderIndexInfo& _s) {
  _strm << "STRUCT PetStore::OrderIndexInfo {" << endl;
    _strm << "\torder_id:" << endl;
  _strm << "\t" << _s.order_id << endl;

    _strm << "\tsubTotal:" << endl;
  _strm << "\t" << _s.subTotal << endl;

  _strm << "}" << endl;
}
Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::OrderIndexInfo& _s) {
  VISostream _ostrm(_strm);
  if (&_strm == &Cout || &_strm == &Cerr || &_strm == &Clog) {
    _pretty_print(_ostrm, _s);
    _ostrm << endl;
  } else {
      _ostrm << _s;
  }
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::OrderIndexInfo& _obj) {
  VISistream _istrm(_strm);
    _istrm >> _obj;
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::OrderIndexInfo*& _obj) {
  VISistream _istrm(_strm);
    _istrm >> _obj;
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::OrderIndexInfo_var& _var) {
  if (!_var._ptr)
    VISTHROW(CORBA::BAD_PARAM())
  else
    _strm << *_var._ptr;
  return _strm;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::OrderIndexInfo_var& _var) {
  if (!_var._ptr)
    _var._ptr = new PetStore::OrderIndexInfo;
  _strm >> *_var._ptr;
  return _strm;
}

PetStore::OrderIndexInfo_var& PetStore::OrderIndexInfo_var::operator=(PetStore::OrderIndexInfo* _p) {
  delete _ptr;
  _ptr = _p;
  return *this;
}

PetStore::OrderIndexInfo_var& PetStore::OrderIndexInfo_var::operator=(const PetStore::OrderIndexInfo_var& _var) {
  delete _ptr;
  _ptr = _var._ptr ? new PetStore::OrderIndexInfo(*_var._ptr) : 
  (PetStore::OrderIndexInfo*)NULL;
  return *this;
}

#if defined(MSVCNEWDLL_BUG)
void *PetStore::OrderIndexInfoList::operator new(size_t ts) {
  return ::operator new(ts);
}
void PetStore::OrderIndexInfoList::operator delete(void *p) {
  ::operator delete(p);
}
#endif // MSVCNEWDLL_BUG

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::OrderIndexInfoList& _seq) {
  VISostream _ostrm(_strm);
  if (&_strm == &Cout || &_strm == &Cerr || &_strm == &Clog) {
    _pretty_print(_ostrm, _seq);
    _ostrm << endl;
  } else {
    _ostrm << _seq;
  }
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::OrderIndexInfoList& _obj) {
  VISistream _istrm(_strm);
  _istrm >> _obj;
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::OrderIndexInfoList_ptr& _ptr) {
  VISistream _istrm(_strm);
  _istrm >> _ptr;
  return _strm;
}

::PetStore::OrderIndexInfo* PetStore::OrderIndexInfoList::allocbuf(CORBA::ULong _nelems) {
  if (_nelems) {
    return new ::PetStore::OrderIndexInfo[_nelems];
  }
  return (::PetStore::OrderIndexInfo *)NULL;
}

void PetStore::OrderIndexInfoList::freebuf(::PetStore::OrderIndexInfo *_data) {
  if (_data)
    delete[] _data;
}

PetStore::OrderIndexInfoList::OrderIndexInfoList(CORBA::ULong _max) {
  _num_allocated = _max;
  _count = 0;
  _contents = allocbuf(_num_allocated);
  _release_flag = 1;
}

PetStore::OrderIndexInfoList::OrderIndexInfoList(CORBA::ULong _max,
                                                 CORBA::ULong _len,
                                                 ::PetStore::OrderIndexInfo *_data,
                                                 CORBA::Boolean _release) {
  _num_allocated = _max;
  _count = _len;
  _contents = _data;
  _release_flag = _release;
}

PetStore::OrderIndexInfoList::OrderIndexInfoList(const PetStore::OrderIndexInfoList& _seq) {
  _num_allocated = _seq._num_allocated;
  _count = _seq._count;
  _contents = allocbuf(_num_allocated);
  if (_num_allocated > 0) {
    for (CORBA::ULong i=0; i < _count; i++) {
      _contents[i] = _seq._contents[i];
    }
  }
  _release_flag = 1;
}

PetStore::OrderIndexInfoList::~OrderIndexInfoList() {
  if (_release_flag) 
    freebuf(_contents);
}

PetStore::OrderIndexInfoList& PetStore::OrderIndexInfoList::operator=(const PetStore::OrderIndexInfoList& _seq) {
  if (_num_allocated < _seq._count) {
    if (_release_flag) 
      freebuf(_contents);
    else
      _release_flag = (CORBA::Boolean) 1;

    _contents = allocbuf(_seq._num_allocated);
    _num_allocated = _seq._num_allocated;
  }
  _count = _seq._count;
  if (_num_allocated > 0) {
    for (CORBA::ULong i = 0; i < _count; i++) {
      _contents[i] = _seq._contents[i];
    }
  }
  return *this;
}

void PetStore::OrderIndexInfoList::length(CORBA::ULong _newlen) {
  if (_newlen > _num_allocated) {
    ::PetStore::OrderIndexInfo *temp = allocbuf(_newlen);
    for (CORBA::ULong i=0; i < _count; i++) {
      temp[i] = _contents[i];
    }
    if (_release_flag) 
      freebuf(_contents);
    else
      _release_flag = (CORBA::Boolean) 1;

    _num_allocated = _newlen;
    _contents = temp;
  }
  _count = _newlen;
}

void PetStore::OrderIndexInfoList::replace(CORBA::ULong _max,
                                           CORBA::ULong _len,
                                           OrderIndexInfo *_data,
                                           CORBA::Boolean _release) {
  if (_release_flag) 
    freebuf(_contents);
  _num_allocated = _max;
  _count = _len;
  _contents = _data;
  _release_flag = _release;
}

::PetStore::OrderIndexInfo *PetStore::OrderIndexInfoList::get_buffer(CORBA::Boolean orphan) {
  if (orphan == (CORBA::Boolean) 1) {
    if (_release_flag == (CORBA::Boolean) 0)
      return (::PetStore::OrderIndexInfo*)NULL;
    else {
      _release_flag = (CORBA::Boolean) 0;
      return _contents;
    }
  } else
    return _contents;
}

const ::PetStore::OrderIndexInfo* PetStore::OrderIndexInfoList::get_buffer() const {
  // Do not use const_cast here because it might not be supported by the compiler
  return (const ::PetStore::OrderIndexInfo*)(void*)_contents;
}

::PetStore::OrderIndexInfo& PetStore::OrderIndexInfoList::operator[](CORBA::ULong _index) {
  if (_index >= _count) {
    VISTHROW(CORBA::BAD_PARAM())
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(static ::PetStore::OrderIndexInfo *_null = 0; return *_null;)
#endif
  }
  return _contents[_index];
}

const ::PetStore::OrderIndexInfo& PetStore::OrderIndexInfoList::operator[](CORBA::ULong _index) const {
  if (_index >= _count) {
    VISTHROW(CORBA::BAD_PARAM())
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(static ::PetStore::OrderIndexInfo *_null = 0; return (const ::PetStore::OrderIndexInfo&)*_null;)
#endif
  }
  // Do not use const_cast here because it might not be supported by the compiler
  return *(const ::PetStore::OrderIndexInfo*)(void*)&_contents[_index];
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::OrderIndexInfoList& _seq) {
  _strm << _seq._count;
  for (CORBA::ULong i=0; i < _seq._count; i++) {
    _strm << _seq._contents[i];
  }
  return _strm;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::OrderIndexInfoList& _seq) {
  CORBA::ULong t_count;
  _strm >> t_count;

  if (t_count > _seq._count) {
    if (_seq._num_allocated < t_count) {
      if (_seq._release_flag)
        PetStore::OrderIndexInfoList::freebuf(_seq._contents);
      else
        _seq._release_flag = (CORBA::Boolean) 1;
      _seq._contents = PetStore::OrderIndexInfoList::allocbuf(t_count);
      _seq._num_allocated = t_count;
    }
  }
  _seq._count = t_count;

  for (CORBA::ULong i=0; i < _seq._count; i++) {
    _strm >> _seq._contents[i];
  }
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::OrderIndexInfoList* _obj) {
  if (!_obj)
    VISTHROW(CORBA::BAD_PARAM())
  else
    _strm << *_obj;
  return _strm;
}
VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::OrderIndexInfoList_ptr& _obj) {
  _obj = new PetStore::OrderIndexInfoList();
  _strm >> *_obj;
  return _strm;
}

void PetStore::_internal_pretty_print(VISostream& _strm, const PetStore::OrderIndexInfoList& _seq) {
  _strm << "SEQUENCE PetStore::OrderIndexInfoList" << endl;
  _strm << "Number of Elements: " << _seq._count << endl;
  for (CORBA::ULong i=0; i < _seq._count; i++) {
    _strm << "[" << i << "]" << endl;
    _strm << "\t_contents[i]:" << endl;
    _pretty_print(_strm, _seq._contents[i]);
    _strm << endl;
    _strm << endl;
  }
}
::PetStore::OrderIndexInfo& PetStore::OrderIndexInfoList_var::operator[](CORBA::ULong _index) {
  if (_ptr)
    return (*_ptr)[_index];
  static PetStore::OrderIndexInfoList _tmp;
  return _tmp[_index];
}

const ::PetStore::OrderIndexInfo& PetStore::OrderIndexInfoList_var::operator[](CORBA::ULong _index) const {
  if (_ptr)
    return (*(const PetStore::OrderIndexInfoList*)_ptr)[_index];
  static const PetStore::OrderIndexInfoList _tmp;
  return _tmp[_index];
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::OrderIndexInfoList_var& _var) {
  if (!_var._ptr)
    VISTHROW(CORBA::BAD_PARAM())
  else
    _strm << *_var._ptr;
  return _strm;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::OrderIndexInfoList_var& _var) {
  if (!_var._ptr)
    _var._ptr = new PetStore::OrderIndexInfoList;
  _strm >> *_var._ptr;
  return _strm;
}
#if defined(MSVCNEWDLL_BUG)
void *PetStore::OrderRecord::operator new(size_t ts) {
  return ::operator new(ts);
}
void PetStore::OrderRecord::operator delete(void *p) {
  ::operator delete(p);
}
#endif // MSVCNEWDLL_BUG

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::OrderRecord& _s) {
    _strm << _s.order_index_list;

    _strm << _s.subTotal;

  return _strm;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::OrderRecord& _s) {
    _strm >> _s.order_index_list;

    _strm >> _s.subTotal;

  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::OrderRecord* _obj) {
  if (!_obj)
    VISTHROW(CORBA::BAD_PARAM())
  else
    _strm << *_obj;
  return _strm;
}
VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::OrderRecord*& _obj) {
  _obj = new PetStore::OrderRecord();
  _strm >> *_obj;
  return _strm;
}

void PetStore::_internal_pretty_print(VISostream& _strm, const PetStore::OrderRecord& _s) {
  _strm << "STRUCT PetStore::OrderRecord {" << endl;
    _strm << "\torder_index_list:" << endl;
  _strm << "\torder_index_list:" << endl;
  _pretty_print(_strm, _s.order_index_list);
  _strm << endl;

    _strm << "\tsubTotal:" << endl;
  _strm << "\t" << _s.subTotal << endl;

  _strm << "}" << endl;
}
Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::OrderRecord& _s) {
  VISostream _ostrm(_strm);
  if (&_strm == &Cout || &_strm == &Cerr || &_strm == &Clog) {
    _pretty_print(_ostrm, _s);
    _ostrm << endl;
  } else {
      _ostrm << _s;
  }
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::OrderRecord& _obj) {
  VISistream _istrm(_strm);
    _istrm >> _obj;
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::OrderRecord*& _obj) {
  VISistream _istrm(_strm);
    _istrm >> _obj;
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::OrderRecord_var& _var) {
  if (!_var._ptr)
    VISTHROW(CORBA::BAD_PARAM())
  else
    _strm << *_var._ptr;
  return _strm;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::OrderRecord_var& _var) {
  if (!_var._ptr)
    _var._ptr = new PetStore::OrderRecord;
  _strm >> *_var._ptr;
  return _strm;
}

PetStore::OrderRecord_var& PetStore::OrderRecord_var::operator=(PetStore::OrderRecord* _p) {
  delete _ptr;
  _ptr = _p;
  return *this;
}

PetStore::OrderRecord_var& PetStore::OrderRecord_var::operator=(const PetStore::OrderRecord_var& _var) {
  delete _ptr;
  _ptr = _var._ptr ? new PetStore::OrderRecord(*_var._ptr) : 
  (PetStore::OrderRecord*)NULL;
  return *this;
}

PetStore::Manage_ptr PetStore::Manage_var::_duplicate(PetStore::Manage_ptr _p) {
  return PetStore::Manage::_duplicate(_p);
}

void
PetStore::Manage_var::_release(PetStore::Manage_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_p);
}

PetStore::Manage_var::Manage_var()
  : _ptr(PetStore::Manage::_nil()) {}

PetStore::Manage_var::Manage_var(PetStore::Manage_ptr _p)
  : _ptr(_p) {}

PetStore::Manage_var::Manage_var(const PetStore::Manage_var& _var)
  : _ptr(PetStore::Manage::_duplicate((PetStore::Manage_ptr) _var)) {}

PetStore::Manage_var::~Manage_var() {
  CORBA::release((CORBA::Object_ptr)_ptr);
}

PetStore::Manage_var&
PetStore::Manage_var::operator=(const PetStore::Manage_var& _var) {
    _release(_ptr);
    _ptr = _duplicate(_var._ptr);
    return *this;
}

PetStore::Manage_var&
PetStore::Manage_var::operator=(PetStore::Manage_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_ptr);
  _ptr = _p;
  return *this;
}

PetStore::Manage_ptr& PetStore::Manage_var::out() {
  _release(_ptr);
  _ptr = (PetStore::Manage_ptr)NULL;
  return _ptr;
}

PetStore::Manage_ptr PetStore::Manage_var::_retn() {
  PetStore::Manage_ptr _tmp_ptr = _ptr;
  _ptr = (PetStore::Manage_ptr)NULL;
  return _tmp_ptr;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::Manage_var& _var) {
  _strm >> _var._ptr;
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::Manage_var& _var) {
  _strm << _var._ptr;
  return _strm;
}

Istream& PetStore::_marshal_out(Istream& _strm, PetStore::Manage_var& _var) {
  VISistream _istrm(_strm);
  _istrm >> _var._ptr;
  return _strm;
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::Manage_var& _var) {
  _strm << (CORBA::Object_ptr)_var._ptr;
  return _strm;
}

// If dllimport is specified, you might probably want to
// disable these definitions
// disable the const definitions
const VISOps_Info PetStore::Manage_ops::_ops_info("PetStore::Manage_ops");
const VISOps_Info *PetStore::Manage_ops::_desc() { return &_ops_info; }
PetStore::Manage_ops_ptr PetStore::Manage_ops::_downcast(PortableServer::ServantBase* _servant) {
  if (_servant == (PortableServer::ServantBase*)NULL)
    return PetStore::Manage_ops::_nil();
  return (PetStore::Manage_ops_ptr)_servant->_safe_downcast_ops(_ops_info);
}
const CORBA::TypeInfo
#ifdef AIXV3
PetStore::Manage::_class_info("PetStore::Manage", "IDL:PetStore/Manage:1.0",
                              NULL, PetStore::Manage::_factory, NULL, 0, NULL,
                              0, CORBA::Object::_desc(), 0L);
#else
PetStore::Manage::_class_info("PetStore::Manage", "IDL:PetStore/Manage:1.0",
                              NULL, PetStore::Manage::_factory, NULL, 0, NULL,
                              0, CORBA::Object::_desc(), 0L);
#endif

const CORBA::TypeInfo *PetStore::Manage::_desc() { return &_class_info; }
const CORBA::TypeInfo *PetStore::Manage::_type_info() const {
  return &_class_info;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::Manage_ptr& _obj) {
  CORBA::Object_var _var_obj(_obj);
  _var_obj = CORBA::Object::_read(_strm, PetStore::Manage::_desc());
  _obj = PetStore::Manage::_narrow(_var_obj);
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::Manage_ptr _obj) {
  _strm << (CORBA::Object_ptr)_obj;
  return _strm;
}
void* PetStore::Manage::_safe_narrow(const CORBA::TypeInfo& _info) const {
  if (_info == _class_info)
    return (void *)this;
  return CORBA_Object::_safe_narrow(_info);
}

CORBA::Object *PetStore::Manage::_factory() {
  return new PetStore::Manage;
}

PetStore::Manage_ptr PetStore::Manage::_this() {
  return PetStore::Manage::_duplicate(this);
}

PetStore::Manage_ptr PetStore::Manage::_narrow(CORBA::Object * _obj) {
  if (_obj == PetStore::Manage::_nil())
    return PetStore::Manage::_nil();
  else
    return PetStore::Manage::_duplicate((PetStore::Manage_ptr)_obj->_safe_narrow(_class_info));
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::Manage_ptr _obj) {
  _strm << (CORBA::Object_ptr) _obj;
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::Manage_ptr& _obj) {
  VISistream _istrm(_strm);
  _istrm >> _obj;
  return _strm;
}

PetStore::Manage *PetStore::Manage::_bind(const char *_object_name,
                                          const char *_host_name,
                                          const CORBA::BindOptions *_opt,
                                          CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
          "IDL:PetStore/Manage:1.0", _object_name, _host_name, _opt, _orb);
  return Manage::_narrow(_obj);
}

PetStore::Manage *PetStore::Manage::_bind(const char *_poa_name,
                                          const CORBA::OctetSequence& _id,
                                          const char *_host_name,
                                          const CORBA::BindOptions *_opt,
                                          CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
   "IDL:PetStore/Manage:1.0", _poa_name, _id, _host_name, _opt, _orb);
  return Manage::_narrow(_obj);
}
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
void PetStore::Manage::addProduct(const ::PetStore::ProductInfo& _arg_newProduct ) {
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("addProduct");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::Manage_ops*  _manage = PetStore::Manage_ops::_downcast(_servant);
      if (!_manage) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "addProduct");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }

      VISTRY {
        _manage->addProduct(_arg_newProduct );
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "addProduct"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "addProduct");
      return;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("addProduct", 1);
    VISIF_EXCEP(return;)
    VISostream& _ostrm = *VISostream::_downcast(_obuf);
    _ostrm << _arg_newProduct;

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return;)
#endif
  }

}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
::PetStore::OrderInfoList* PetStore::Manage::listAllOrders() {
  ::PetStore::OrderInfoList* _ret = (::PetStore::OrderInfoList_ptr) NULL;
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("listAllOrders");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::Manage_ops*  _manage = PetStore::Manage_ops::_downcast(_servant);
      if (!_manage) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "listAllOrders");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }

      VISTRY {
         _ret = _manage->listAllOrders();
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "listAllOrders"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "listAllOrders");
      return _ret;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("listAllOrders", 1);
    VISIF_EXCEP(return _ret;)

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return _ret;)
#endif
  }

  VISistream& _istrm = *VISistream::_downcast(_ibuf);
  _istrm >> _ret;
  return _ret;
}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
::PetStore::OrderRecord* PetStore::Manage::account() {
  ::PetStore::OrderRecord* _ret = (::PetStore::OrderRecord_ptr) NULL;
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("account");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::Manage_ops*  _manage = PetStore::Manage_ops::_downcast(_servant);
      if (!_manage) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "account");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }

      VISTRY {
         _ret = _manage->account();
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "account"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "account");
      return _ret;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("account", 1);
    VISIF_EXCEP(return _ret;)

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return _ret;)
#endif
  }

  VISistream& _istrm = *VISistream::_downcast(_ibuf);
  _istrm >> _ret;
  return _ret;
}

#if defined(__BORLANDC__)
 #pragma option pop
#endif

PetStore::Manager_ptr PetStore::Manager_var::_duplicate(PetStore::Manager_ptr _p) {
  return PetStore::Manager::_duplicate(_p);
}

void
PetStore::Manager_var::_release(PetStore::Manager_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_p);
}

PetStore::Manager_var::Manager_var()
  : _ptr(PetStore::Manager::_nil()) {}

PetStore::Manager_var::Manager_var(PetStore::Manager_ptr _p)
  : _ptr(_p) {}

PetStore::Manager_var::Manager_var(const PetStore::Manager_var& _var)
  : _ptr(PetStore::Manager::_duplicate((PetStore::Manager_ptr) _var)) {}

PetStore::Manager_var::~Manager_var() {
  CORBA::release((CORBA::Object_ptr)_ptr);
}

PetStore::Manager_var&
PetStore::Manager_var::operator=(const PetStore::Manager_var& _var) {
    _release(_ptr);
    _ptr = _duplicate(_var._ptr);
    return *this;
}

PetStore::Manager_var&
PetStore::Manager_var::operator=(PetStore::Manager_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_ptr);
  _ptr = _p;
  return *this;
}

PetStore::Manager_ptr& PetStore::Manager_var::out() {
  _release(_ptr);
  _ptr = (PetStore::Manager_ptr)NULL;
  return _ptr;
}

PetStore::Manager_ptr PetStore::Manager_var::_retn() {
  PetStore::Manager_ptr _tmp_ptr = _ptr;
  _ptr = (PetStore::Manager_ptr)NULL;
  return _tmp_ptr;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::Manager_var& _var) {
  _strm >> _var._ptr;
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::Manager_var& _var) {
  _strm << _var._ptr;
  return _strm;
}

Istream& PetStore::_marshal_out(Istream& _strm, PetStore::Manager_var& _var) {
  VISistream _istrm(_strm);
  _istrm >> _var._ptr;
  return _strm;
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::Manager_var& _var) {
  _strm << (CORBA::Object_ptr)_var._ptr;
  return _strm;
}

// If dllimport is specified, you might probably want to
// disable these definitions
// disable the const definitions
const VISOps_Info PetStore::Manager_ops::_ops_info("PetStore::Manager_ops");
const VISOps_Info *PetStore::Manager_ops::_desc() { return &_ops_info; }
PetStore::Manager_ops_ptr PetStore::Manager_ops::_downcast(PortableServer::ServantBase* _servant) {
  if (_servant == (PortableServer::ServantBase*)NULL)
    return PetStore::Manager_ops::_nil();
  return (PetStore::Manager_ops_ptr)_servant->_safe_downcast_ops(_ops_info);
}
const CORBA::TypeInfo
#ifdef AIXV3
PetStore::Manager::_class_info("PetStore::Manager",
                               "IDL:PetStore/Manager:1.0", NULL,
                               PetStore::Manager::_factory, NULL, 0, NULL, 0,
                               PetStore::Manage::_desc(),
                               Components::CCMObject::_desc(), 0L);
#else
PetStore::Manager::_class_info("PetStore::Manager",
                               "IDL:PetStore/Manager:1.0", NULL,
                               PetStore::Manager::_factory, NULL, 0, NULL, 0,
                               PetStore::Manage::_desc(),
                               Components::CCMObject::_desc(), 0L);
#endif

const CORBA::TypeInfo *PetStore::Manager::_desc() { return &_class_info; }
const CORBA::TypeInfo *PetStore::Manager::_type_info() const {
  return &_class_info;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::Manager_ptr& _obj) {
  CORBA::Object_var _var_obj(_obj);
  _var_obj = CORBA::Object::_read(_strm, PetStore::Manager::_desc());
  _obj = PetStore::Manager::_narrow(_var_obj);
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::Manager_ptr _obj) {
  _strm << (CORBA::Object_ptr)_obj;
  return _strm;
}
void* PetStore::Manager::_safe_narrow(const CORBA::TypeInfo& _info) const {
  if (_info == _class_info)
    return (void *)this;
  if (_info == *PetStore::Manage::_desc())
    return (void *) (PetStore::Manage *) this;
  if (_info == *Components::CCMObject::_desc())
    return (void *) (Components::CCMObject *) this;
  if (_info == *Components::Navigation::_desc())
    return (void *) (Components::Navigation *) this;
  if (_info == *Components::Receptacles::_desc())
    return (void *) (Components::Receptacles *) this;
  if (_info == *Components::Events::_desc())
    return (void *) (Components::Events *) this;
  return CORBA_Object::_safe_narrow(_info);
}

CORBA::Object *PetStore::Manager::_factory() {
  return new PetStore::Manager;
}

PetStore::Manager_ptr PetStore::Manager::_this() {
  return PetStore::Manager::_duplicate(this);
}

PetStore::Manager_ptr PetStore::Manager::_narrow(CORBA::Object * _obj) {
  if (_obj == PetStore::Manager::_nil())
    return PetStore::Manager::_nil();
  else
    return PetStore::Manager::_duplicate((PetStore::Manager_ptr)_obj->_safe_narrow(_class_info));
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::Manager_ptr _obj) {
  _strm << (CORBA::Object_ptr) _obj;
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::Manager_ptr& _obj) {
  VISistream _istrm(_strm);
  _istrm >> _obj;
  return _strm;
}

PetStore::Manager *PetStore::Manager::_bind(const char *_object_name,
                                            const char *_host_name,
                                            const CORBA::BindOptions *_opt,
                                            CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
          "IDL:PetStore/Manager:1.0", _object_name, _host_name, _opt, _orb);
  return Manager::_narrow(_obj);
}

PetStore::Manager *PetStore::Manager::_bind(const char *_poa_name,
                                            const CORBA::OctetSequence& _id,
                                            const char *_host_name,
                                            const CORBA::BindOptions *_opt,
                                            CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
   "IDL:PetStore/Manager:1.0", _poa_name, _id, _host_name, _opt, _orb);
  return Manager::_narrow(_obj);
}
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
void PetStore::Manager::connect_manage_op(::PetStore::AdvancedOp_ptr _arg_conn ) {
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("connect_manage_op");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::Manager_ops*  _manager = PetStore::Manager_ops::_downcast(_servant);
      if (!_manager) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "connect_manage_op");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }

      VISTRY {
        _manager->connect_manage_op(_arg_conn );
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "connect_manage_op"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "connect_manage_op");
      return;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("connect_manage_op", 1);
    VISIF_EXCEP(return;)
    VISostream& _ostrm = *VISostream::_downcast(_obuf);
    _ostrm << _arg_conn;

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return;)
#endif
  }

}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
::PetStore::AdvancedOp_ptr PetStore::Manager::disconnect_manage_op() {
  ::PetStore::AdvancedOp_ptr _ret = (::PetStore::AdvancedOp_ptr) NULL;
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("disconnect_manage_op");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::Manager_ops*  _manager = PetStore::Manager_ops::_downcast(_servant);
      if (!_manager) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "disconnect_manage_op");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }

      VISTRY {
         _ret = _manager->disconnect_manage_op();
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "disconnect_manage_op"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "disconnect_manage_op");
      return _ret;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("disconnect_manage_op", 1);
    VISIF_EXCEP(return _ret;)

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return _ret;)
#endif
  }

  VISistream& _istrm = *VISistream::_downcast(_ibuf);
  _istrm >> _ret;
  return _ret;
}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
::PetStore::AdvancedOp_ptr PetStore::Manager::get_connection_manage_op() {
  ::PetStore::AdvancedOp_ptr _ret = (::PetStore::AdvancedOp_ptr) NULL;
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("get_connection_manage_op");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::Manager_ops*  _manager = PetStore::Manager_ops::_downcast(_servant);
      if (!_manager) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "get_connection_manage_op");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }

      VISTRY {
         _ret = _manager->get_connection_manage_op();
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "get_connection_manage_op"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "get_connection_manage_op");
      return _ret;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("get_connection_manage_op", 1);
    VISIF_EXCEP(return _ret;)

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return _ret;)
#endif
  }

  VISistream& _istrm = *VISistream::_downcast(_ibuf);
  _istrm >> _ret;
  return _ret;
}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
void PetStore::Manager::connect_price_calculater(::PetStore::Calculate_ptr _arg_conn ) {
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("connect_price_calculater");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::Manager_ops*  _manager = PetStore::Manager_ops::_downcast(_servant);
      if (!_manager) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "connect_price_calculater");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }

      VISTRY {
        _manager->connect_price_calculater(_arg_conn );
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "connect_price_calculater"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "connect_price_calculater");
      return;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("connect_price_calculater", 1);
    VISIF_EXCEP(return;)
    VISostream& _ostrm = *VISostream::_downcast(_obuf);
    _ostrm << _arg_conn;

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return;)
#endif
  }

}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
::PetStore::Calculate_ptr PetStore::Manager::disconnect_price_calculater() {
  ::PetStore::Calculate_ptr _ret = (::PetStore::Calculate_ptr) NULL;
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("disconnect_price_calculater");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::Manager_ops*  _manager = PetStore::Manager_ops::_downcast(_servant);
      if (!_manager) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "disconnect_price_calculater");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }

      VISTRY {
         _ret = _manager->disconnect_price_calculater();
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "disconnect_price_calculater"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "disconnect_price_calculater");
      return _ret;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("disconnect_price_calculater", 1);
    VISIF_EXCEP(return _ret;)

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return _ret;)
#endif
  }

  VISistream& _istrm = *VISistream::_downcast(_ibuf);
  _istrm >> _ret;
  return _ret;
}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
::PetStore::Calculate_ptr PetStore::Manager::get_connection_price_calculater() {
  ::PetStore::Calculate_ptr _ret = (::PetStore::Calculate_ptr) NULL;
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("get_connection_price_calculater");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::Manager_ops*  _manager = PetStore::Manager_ops::_downcast(_servant);
      if (!_manager) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "get_connection_price_calculater");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }

      VISTRY {
         _ret = _manager->get_connection_price_calculater();
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "get_connection_price_calculater"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "get_connection_price_calculater");
      return _ret;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("get_connection_price_calculater", 1);
    VISIF_EXCEP(return _ret;)

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return _ret;)
#endif
  }

  VISistream& _istrm = *VISistream::_downcast(_ibuf);
  _istrm >> _ret;
  return _ret;
}

#if defined(__BORLANDC__)
 #pragma option pop
#endif
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
::PetStore::SalesReportConsumer_ptr PetStore::Manager::get_consumer_analyse() {
  ::PetStore::SalesReportConsumer_ptr _ret = (::PetStore::SalesReportConsumer_ptr) NULL;
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("get_consumer_analyse");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::Manager_ops*  _manager = PetStore::Manager_ops::_downcast(_servant);
      if (!_manager) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "get_consumer_analyse");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }

      VISTRY {
         _ret = _manager->get_consumer_analyse();
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "get_consumer_analyse"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "get_consumer_analyse");
      return _ret;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("get_consumer_analyse", 1);
    VISIF_EXCEP(return _ret;)

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return _ret;)
#endif
  }

  VISistream& _istrm = *VISistream::_downcast(_ibuf);
  _istrm >> _ret;
  return _ret;
}

#if defined(__BORLANDC__)
 #pragma option pop
#endif

PetStore::PricerHomeExplicit_ptr PetStore::PricerHomeExplicit_var::_duplicate(PetStore::PricerHomeExplicit_ptr _p) {
  return PetStore::PricerHomeExplicit::_duplicate(_p);
}

void
PetStore::PricerHomeExplicit_var::_release(PetStore::PricerHomeExplicit_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_p);
}

PetStore::PricerHomeExplicit_var::PricerHomeExplicit_var()
  : _ptr(PetStore::PricerHomeExplicit::_nil()) {}

PetStore::PricerHomeExplicit_var::PricerHomeExplicit_var(PetStore::PricerHomeExplicit_ptr _p)
  : _ptr(_p) {}

PetStore::PricerHomeExplicit_var::PricerHomeExplicit_var(const PetStore::PricerHomeExplicit_var& _var)
  : _ptr(PetStore::PricerHomeExplicit::_duplicate((PetStore::PricerHomeExplicit_ptr) _var)) {}

PetStore::PricerHomeExplicit_var::~PricerHomeExplicit_var() {
  CORBA::release((CORBA::Object_ptr)_ptr);
}

PetStore::PricerHomeExplicit_var&
PetStore::PricerHomeExplicit_var::operator=(const PetStore::PricerHomeExplicit_var& _var) {
    _release(_ptr);
    _ptr = _duplicate(_var._ptr);
    return *this;
}

PetStore::PricerHomeExplicit_var&
PetStore::PricerHomeExplicit_var::operator=(PetStore::PricerHomeExplicit_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_ptr);
  _ptr = _p;
  return *this;
}

PetStore::PricerHomeExplicit_ptr& PetStore::PricerHomeExplicit_var::out() {
  _release(_ptr);
  _ptr = (PetStore::PricerHomeExplicit_ptr)NULL;
  return _ptr;
}

PetStore::PricerHomeExplicit_ptr PetStore::PricerHomeExplicit_var::_retn() {
  PetStore::PricerHomeExplicit_ptr _tmp_ptr = _ptr;
  _ptr = (PetStore::PricerHomeExplicit_ptr)NULL;
  return _tmp_ptr;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::PricerHomeExplicit_var& _var) {
  _strm >> _var._ptr;
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::PricerHomeExplicit_var& _var) {
  _strm << _var._ptr;
  return _strm;
}

Istream& PetStore::_marshal_out(Istream& _strm, PetStore::PricerHomeExplicit_var& _var) {
  VISistream _istrm(_strm);
  _istrm >> _var._ptr;
  return _strm;
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::PricerHomeExplicit_var& _var) {
  _strm << (CORBA::Object_ptr)_var._ptr;
  return _strm;
}

// If dllimport is specified, you might probably want to
// disable these definitions
// disable the const definitions
const VISOps_Info PetStore::PricerHomeExplicit_ops::_ops_info("PetStore::PricerHomeExplicit_ops");
const VISOps_Info *PetStore::PricerHomeExplicit_ops::_desc() { return &_ops_info; }
PetStore::PricerHomeExplicit_ops_ptr PetStore::PricerHomeExplicit_ops::_downcast(PortableServer::ServantBase* _servant) {
  if (_servant == (PortableServer::ServantBase*)NULL)
    return PetStore::PricerHomeExplicit_ops::_nil();
  return (PetStore::PricerHomeExplicit_ops_ptr)_servant->_safe_downcast_ops(_ops_info);
}
const CORBA::TypeInfo
#ifdef AIXV3
PetStore::PricerHomeExplicit::_class_info("PetStore::PricerHomeExplicit",
                                          "IDL:PetStore/PricerHomeExplicit:1.0",
                                          NULL,
                                          PetStore::PricerHomeExplicit::_factory,
                                          NULL, 0, NULL, 0,
                                          Components::CCMHome::_desc(), 0L);
#else
PetStore::PricerHomeExplicit::_class_info("PetStore::PricerHomeExplicit",
                                          "IDL:PetStore/PricerHomeExplicit:1.0",
                                          NULL,
                                          PetStore::PricerHomeExplicit::_factory,
                                          NULL, 0, NULL, 0,
                                          Components::CCMHome::_desc(), 0L);
#endif

const CORBA::TypeInfo *PetStore::PricerHomeExplicit::_desc() { return &_class_info; }
const CORBA::TypeInfo *PetStore::PricerHomeExplicit::_type_info() const {
  return &_class_info;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::PricerHomeExplicit_ptr& _obj) {
  CORBA::Object_var _var_obj(_obj);
  _var_obj = CORBA::Object::_read(_strm, PetStore::PricerHomeExplicit::_desc());
  _obj = PetStore::PricerHomeExplicit::_narrow(_var_obj);
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::PricerHomeExplicit_ptr _obj) {
  _strm << (CORBA::Object_ptr)_obj;
  return _strm;
}
void* PetStore::PricerHomeExplicit::_safe_narrow(const CORBA::TypeInfo& _info) const {
  if (_info == _class_info)
    return (void *)this;
  if (_info == *Components::CCMHome::_desc())
    return (void *) (Components::CCMHome *) this;
  return CORBA_Object::_safe_narrow(_info);
}

CORBA::Object *PetStore::PricerHomeExplicit::_factory() {
  return new PetStore::PricerHomeExplicit;
}

PetStore::PricerHomeExplicit_ptr PetStore::PricerHomeExplicit::_this() {
  return PetStore::PricerHomeExplicit::_duplicate(this);
}

PetStore::PricerHomeExplicit_ptr PetStore::PricerHomeExplicit::_narrow(CORBA::Object * _obj) {
  if (_obj == PetStore::PricerHomeExplicit::_nil())
    return PetStore::PricerHomeExplicit::_nil();
  else
    return PetStore::PricerHomeExplicit::_duplicate((PetStore::PricerHomeExplicit_ptr)_obj->_safe_narrow(_class_info));
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::PricerHomeExplicit_ptr _obj) {
  _strm << (CORBA::Object_ptr) _obj;
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::PricerHomeExplicit_ptr& _obj) {
  VISistream _istrm(_strm);
  _istrm >> _obj;
  return _strm;
}

PetStore::PricerHomeExplicit *PetStore::PricerHomeExplicit::_bind(const char *_object_name,
                                                                  const char *_host_name,
                                                                  const CORBA::BindOptions *_opt,
                                                                  CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
          "IDL:PetStore/PricerHomeExplicit:1.0", _object_name, _host_name,
_opt, _orb);
  return PricerHomeExplicit::_narrow(_obj);
}

PetStore::PricerHomeExplicit *PetStore::PricerHomeExplicit::_bind(const char *_poa_name,
                                                                  const CORBA::OctetSequence& _id,
                                                                  const char *_host_name,
                                                                  const CORBA::BindOptions *_opt,
                                                                  CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
   "IDL:PetStore/PricerHomeExplicit:1.0", _poa_name, _id, _host_name, _opt,
   _orb);
  return PricerHomeExplicit::_narrow(_obj);
}

PetStore::PricerHomeImplicit_ptr PetStore::PricerHomeImplicit_var::_duplicate(PetStore::PricerHomeImplicit_ptr _p) {
  return PetStore::PricerHomeImplicit::_duplicate(_p);
}

void
PetStore::PricerHomeImplicit_var::_release(PetStore::PricerHomeImplicit_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_p);
}

PetStore::PricerHomeImplicit_var::PricerHomeImplicit_var()
  : _ptr(PetStore::PricerHomeImplicit::_nil()) {}

PetStore::PricerHomeImplicit_var::PricerHomeImplicit_var(PetStore::PricerHomeImplicit_ptr _p)
  : _ptr(_p) {}

PetStore::PricerHomeImplicit_var::PricerHomeImplicit_var(const PetStore::PricerHomeImplicit_var& _var)
  : _ptr(PetStore::PricerHomeImplicit::_duplicate((PetStore::PricerHomeImplicit_ptr) _var)) {}

PetStore::PricerHomeImplicit_var::~PricerHomeImplicit_var() {
  CORBA::release((CORBA::Object_ptr)_ptr);
}

PetStore::PricerHomeImplicit_var&
PetStore::PricerHomeImplicit_var::operator=(const PetStore::PricerHomeImplicit_var& _var) {
    _release(_ptr);
    _ptr = _duplicate(_var._ptr);
    return *this;
}

PetStore::PricerHomeImplicit_var&
PetStore::PricerHomeImplicit_var::operator=(PetStore::PricerHomeImplicit_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_ptr);
  _ptr = _p;
  return *this;
}

PetStore::PricerHomeImplicit_ptr& PetStore::PricerHomeImplicit_var::out() {
  _release(_ptr);
  _ptr = (PetStore::PricerHomeImplicit_ptr)NULL;
  return _ptr;
}

PetStore::PricerHomeImplicit_ptr PetStore::PricerHomeImplicit_var::_retn() {
  PetStore::PricerHomeImplicit_ptr _tmp_ptr = _ptr;
  _ptr = (PetStore::PricerHomeImplicit_ptr)NULL;
  return _tmp_ptr;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::PricerHomeImplicit_var& _var) {
  _strm >> _var._ptr;
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::PricerHomeImplicit_var& _var) {
  _strm << _var._ptr;
  return _strm;
}

Istream& PetStore::_marshal_out(Istream& _strm, PetStore::PricerHomeImplicit_var& _var) {
  VISistream _istrm(_strm);
  _istrm >> _var._ptr;
  return _strm;
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::PricerHomeImplicit_var& _var) {
  _strm << (CORBA::Object_ptr)_var._ptr;
  return _strm;
}

// If dllimport is specified, you might probably want to
// disable these definitions
// disable the const definitions
const VISOps_Info PetStore::PricerHomeImplicit_ops::_ops_info("PetStore::PricerHomeImplicit_ops");
const VISOps_Info *PetStore::PricerHomeImplicit_ops::_desc() { return &_ops_info; }
PetStore::PricerHomeImplicit_ops_ptr PetStore::PricerHomeImplicit_ops::_downcast(PortableServer::ServantBase* _servant) {
  if (_servant == (PortableServer::ServantBase*)NULL)
    return PetStore::PricerHomeImplicit_ops::_nil();
  return (PetStore::PricerHomeImplicit_ops_ptr)_servant->_safe_downcast_ops(_ops_info);
}
const CORBA::TypeInfo
#ifdef AIXV3
PetStore::PricerHomeImplicit::_class_info("PetStore::PricerHomeImplicit",
                                          "IDL:PetStore/PricerHomeImplicit:1.0",
                                          NULL,
                                          PetStore::PricerHomeImplicit::_factory,
                                          NULL, 0, NULL, 0,
                                          Components::KeylessCCMHome::_desc(),
                                          0L);
#else
PetStore::PricerHomeImplicit::_class_info("PetStore::PricerHomeImplicit",
                                          "IDL:PetStore/PricerHomeImplicit:1.0",
                                          NULL,
                                          PetStore::PricerHomeImplicit::_factory,
                                          NULL, 0, NULL, 0,
                                          Components::KeylessCCMHome::_desc(),
                                          0L);
#endif

const CORBA::TypeInfo *PetStore::PricerHomeImplicit::_desc() { return &_class_info; }
const CORBA::TypeInfo *PetStore::PricerHomeImplicit::_type_info() const {
  return &_class_info;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::PricerHomeImplicit_ptr& _obj) {
  CORBA::Object_var _var_obj(_obj);
  _var_obj = CORBA::Object::_read(_strm, PetStore::PricerHomeImplicit::_desc());
  _obj = PetStore::PricerHomeImplicit::_narrow(_var_obj);
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::PricerHomeImplicit_ptr _obj) {
  _strm << (CORBA::Object_ptr)_obj;
  return _strm;
}
void* PetStore::PricerHomeImplicit::_safe_narrow(const CORBA::TypeInfo& _info) const {
  if (_info == _class_info)
    return (void *)this;
  if (_info == *Components::KeylessCCMHome::_desc())
    return (void *) (Components::KeylessCCMHome *) this;
  return CORBA_Object::_safe_narrow(_info);
}

CORBA::Object *PetStore::PricerHomeImplicit::_factory() {
  return new PetStore::PricerHomeImplicit;
}

PetStore::PricerHomeImplicit_ptr PetStore::PricerHomeImplicit::_this() {
  return PetStore::PricerHomeImplicit::_duplicate(this);
}

PetStore::PricerHomeImplicit_ptr PetStore::PricerHomeImplicit::_narrow(CORBA::Object * _obj) {
  if (_obj == PetStore::PricerHomeImplicit::_nil())
    return PetStore::PricerHomeImplicit::_nil();
  else
    return PetStore::PricerHomeImplicit::_duplicate((PetStore::PricerHomeImplicit_ptr)_obj->_safe_narrow(_class_info));
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::PricerHomeImplicit_ptr _obj) {
  _strm << (CORBA::Object_ptr) _obj;
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::PricerHomeImplicit_ptr& _obj) {
  VISistream _istrm(_strm);
  _istrm >> _obj;
  return _strm;
}

PetStore::PricerHomeImplicit *PetStore::PricerHomeImplicit::_bind(const char *_object_name,
                                                                  const char *_host_name,
                                                                  const CORBA::BindOptions *_opt,
                                                                  CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
          "IDL:PetStore/PricerHomeImplicit:1.0", _object_name, _host_name,
_opt, _orb);
  return PricerHomeImplicit::_narrow(_obj);
}

PetStore::PricerHomeImplicit *PetStore::PricerHomeImplicit::_bind(const char *_poa_name,
                                                                  const CORBA::OctetSequence& _id,
                                                                  const char *_host_name,
                                                                  const CORBA::BindOptions *_opt,
                                                                  CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
   "IDL:PetStore/PricerHomeImplicit:1.0", _poa_name, _id, _host_name, _opt,
   _orb);
  return PricerHomeImplicit::_narrow(_obj);
}
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
::PetStore::Pricer_ptr PetStore::PricerHomeImplicit::create() {
  ::PetStore::Pricer_ptr _ret = (::PetStore::Pricer_ptr) NULL;
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("create");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::PricerHomeImplicit_ops*  _pricerhomeimplicit = PetStore::PricerHomeImplicit_ops::_downcast(_servant);
      if (!_pricerhomeimplicit) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "create");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }

      VISTRY {
         _ret = _pricerhomeimplicit->create();
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "create"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "create");
      return _ret;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("create", 1);
    VISIF_EXCEP(return _ret;)

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return _ret;)
#endif
  }

  VISistream& _istrm = *VISistream::_downcast(_ibuf);
  _istrm >> _ret;
  return _ret;
}

#if defined(__BORLANDC__)
 #pragma option pop
#endif

PetStore::PricerHome_ptr PetStore::PricerHome_var::_duplicate(PetStore::PricerHome_ptr _p) {
  return PetStore::PricerHome::_duplicate(_p);
}

void
PetStore::PricerHome_var::_release(PetStore::PricerHome_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_p);
}

PetStore::PricerHome_var::PricerHome_var()
  : _ptr(PetStore::PricerHome::_nil()) {}

PetStore::PricerHome_var::PricerHome_var(PetStore::PricerHome_ptr _p)
  : _ptr(_p) {}

PetStore::PricerHome_var::PricerHome_var(const PetStore::PricerHome_var& _var)
  : _ptr(PetStore::PricerHome::_duplicate((PetStore::PricerHome_ptr) _var)) {}

PetStore::PricerHome_var::~PricerHome_var() {
  CORBA::release((CORBA::Object_ptr)_ptr);
}

PetStore::PricerHome_var&
PetStore::PricerHome_var::operator=(const PetStore::PricerHome_var& _var) {
    _release(_ptr);
    _ptr = _duplicate(_var._ptr);
    return *this;
}

PetStore::PricerHome_var&
PetStore::PricerHome_var::operator=(PetStore::PricerHome_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_ptr);
  _ptr = _p;
  return *this;
}

PetStore::PricerHome_ptr& PetStore::PricerHome_var::out() {
  _release(_ptr);
  _ptr = (PetStore::PricerHome_ptr)NULL;
  return _ptr;
}

PetStore::PricerHome_ptr PetStore::PricerHome_var::_retn() {
  PetStore::PricerHome_ptr _tmp_ptr = _ptr;
  _ptr = (PetStore::PricerHome_ptr)NULL;
  return _tmp_ptr;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::PricerHome_var& _var) {
  _strm >> _var._ptr;
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::PricerHome_var& _var) {
  _strm << _var._ptr;
  return _strm;
}

Istream& PetStore::_marshal_out(Istream& _strm, PetStore::PricerHome_var& _var) {
  VISistream _istrm(_strm);
  _istrm >> _var._ptr;
  return _strm;
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::PricerHome_var& _var) {
  _strm << (CORBA::Object_ptr)_var._ptr;
  return _strm;
}

// If dllimport is specified, you might probably want to
// disable these definitions
// disable the const definitions
const VISOps_Info PetStore::PricerHome_ops::_ops_info("PetStore::PricerHome_ops");
const VISOps_Info *PetStore::PricerHome_ops::_desc() { return &_ops_info; }
PetStore::PricerHome_ops_ptr PetStore::PricerHome_ops::_downcast(PortableServer::ServantBase* _servant) {
  if (_servant == (PortableServer::ServantBase*)NULL)
    return PetStore::PricerHome_ops::_nil();
  return (PetStore::PricerHome_ops_ptr)_servant->_safe_downcast_ops(_ops_info);
}
const CORBA::TypeInfo
#ifdef AIXV3
PetStore::PricerHome::_class_info("PetStore::PricerHome",
                                  "IDL:PetStore/PricerHome:1.0", NULL,
                                  PetStore::PricerHome::_factory, NULL, 0,
                                  NULL, 0,
                                  PetStore::PricerHomeExplicit::_desc(),
                                  PetStore::PricerHomeImplicit::_desc(), 0L);
#else
PetStore::PricerHome::_class_info("PetStore::PricerHome",
                                  "IDL:PetStore/PricerHome:1.0", NULL,
                                  PetStore::PricerHome::_factory, NULL, 0,
                                  NULL, 0,
                                  PetStore::PricerHomeExplicit::_desc(),
                                  PetStore::PricerHomeImplicit::_desc(), 0L);
#endif

const CORBA::TypeInfo *PetStore::PricerHome::_desc() { return &_class_info; }
const CORBA::TypeInfo *PetStore::PricerHome::_type_info() const {
  return &_class_info;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::PricerHome_ptr& _obj) {
  CORBA::Object_var _var_obj(_obj);
  _var_obj = CORBA::Object::_read(_strm, PetStore::PricerHome::_desc());
  _obj = PetStore::PricerHome::_narrow(_var_obj);
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::PricerHome_ptr _obj) {
  _strm << (CORBA::Object_ptr)_obj;
  return _strm;
}
void* PetStore::PricerHome::_safe_narrow(const CORBA::TypeInfo& _info) const {
  if (_info == _class_info)
    return (void *)this;
  if (_info == *PetStore::PricerHomeExplicit::_desc())
    return (void *) (PetStore::PricerHomeExplicit *) this;
  if (_info == *Components::CCMHome::_desc())
    return (void *) (Components::CCMHome *) this;
  if (_info == *PetStore::PricerHomeImplicit::_desc())
    return (void *) (PetStore::PricerHomeImplicit *) this;
  if (_info == *Components::KeylessCCMHome::_desc())
    return (void *) (Components::KeylessCCMHome *) this;
  return CORBA_Object::_safe_narrow(_info);
}

CORBA::Object *PetStore::PricerHome::_factory() {
  return new PetStore::PricerHome;
}

PetStore::PricerHome_ptr PetStore::PricerHome::_this() {
  return PetStore::PricerHome::_duplicate(this);
}

PetStore::PricerHome_ptr PetStore::PricerHome::_narrow(CORBA::Object * _obj) {
  if (_obj == PetStore::PricerHome::_nil())
    return PetStore::PricerHome::_nil();
  else
    return PetStore::PricerHome::_duplicate((PetStore::PricerHome_ptr)_obj->_safe_narrow(_class_info));
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::PricerHome_ptr _obj) {
  _strm << (CORBA::Object_ptr) _obj;
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::PricerHome_ptr& _obj) {
  VISistream _istrm(_strm);
  _istrm >> _obj;
  return _strm;
}

PetStore::PricerHome *PetStore::PricerHome::_bind(const char *_object_name,
                                                  const char *_host_name,
                                                  const CORBA::BindOptions *_opt,
                                                  CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
          "IDL:PetStore/PricerHome:1.0", _object_name, _host_name, _opt, _orb);
  return PricerHome::_narrow(_obj);
}

PetStore::PricerHome *PetStore::PricerHome::_bind(const char *_poa_name,
                                                  const CORBA::OctetSequence& _id,
                                                  const char *_host_name,
                                                  const CORBA::BindOptions *_opt,
                                                  CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
   "IDL:PetStore/PricerHome:1.0", _poa_name, _id, _host_name, _opt, _orb);
  return PricerHome::_narrow(_obj);
}

PetStore::CartHomeExplicit_ptr PetStore::CartHomeExplicit_var::_duplicate(PetStore::CartHomeExplicit_ptr _p) {
  return PetStore::CartHomeExplicit::_duplicate(_p);
}

void
PetStore::CartHomeExplicit_var::_release(PetStore::CartHomeExplicit_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_p);
}

PetStore::CartHomeExplicit_var::CartHomeExplicit_var()
  : _ptr(PetStore::CartHomeExplicit::_nil()) {}

PetStore::CartHomeExplicit_var::CartHomeExplicit_var(PetStore::CartHomeExplicit_ptr _p)
  : _ptr(_p) {}

PetStore::CartHomeExplicit_var::CartHomeExplicit_var(const PetStore::CartHomeExplicit_var& _var)
  : _ptr(PetStore::CartHomeExplicit::_duplicate((PetStore::CartHomeExplicit_ptr) _var)) {}

PetStore::CartHomeExplicit_var::~CartHomeExplicit_var() {
  CORBA::release((CORBA::Object_ptr)_ptr);
}

PetStore::CartHomeExplicit_var&
PetStore::CartHomeExplicit_var::operator=(const PetStore::CartHomeExplicit_var& _var) {
    _release(_ptr);
    _ptr = _duplicate(_var._ptr);
    return *this;
}

PetStore::CartHomeExplicit_var&
PetStore::CartHomeExplicit_var::operator=(PetStore::CartHomeExplicit_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_ptr);
  _ptr = _p;
  return *this;
}

PetStore::CartHomeExplicit_ptr& PetStore::CartHomeExplicit_var::out() {
  _release(_ptr);
  _ptr = (PetStore::CartHomeExplicit_ptr)NULL;
  return _ptr;
}

PetStore::CartHomeExplicit_ptr PetStore::CartHomeExplicit_var::_retn() {
  PetStore::CartHomeExplicit_ptr _tmp_ptr = _ptr;
  _ptr = (PetStore::CartHomeExplicit_ptr)NULL;
  return _tmp_ptr;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::CartHomeExplicit_var& _var) {
  _strm >> _var._ptr;
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::CartHomeExplicit_var& _var) {
  _strm << _var._ptr;
  return _strm;
}

Istream& PetStore::_marshal_out(Istream& _strm, PetStore::CartHomeExplicit_var& _var) {
  VISistream _istrm(_strm);
  _istrm >> _var._ptr;
  return _strm;
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::CartHomeExplicit_var& _var) {
  _strm << (CORBA::Object_ptr)_var._ptr;
  return _strm;
}

// If dllimport is specified, you might probably want to
// disable these definitions
// disable the const definitions
const VISOps_Info PetStore::CartHomeExplicit_ops::_ops_info("PetStore::CartHomeExplicit_ops");
const VISOps_Info *PetStore::CartHomeExplicit_ops::_desc() { return &_ops_info; }
PetStore::CartHomeExplicit_ops_ptr PetStore::CartHomeExplicit_ops::_downcast(PortableServer::ServantBase* _servant) {
  if (_servant == (PortableServer::ServantBase*)NULL)
    return PetStore::CartHomeExplicit_ops::_nil();
  return (PetStore::CartHomeExplicit_ops_ptr)_servant->_safe_downcast_ops(_ops_info);
}
const CORBA::TypeInfo
#ifdef AIXV3
PetStore::CartHomeExplicit::_class_info("PetStore::CartHomeExplicit",
                                        "IDL:PetStore/CartHomeExplicit:1.0",
                                        NULL,
                                        PetStore::CartHomeExplicit::_factory,
                                        NULL, 0, NULL, 0,
                                        Components::CCMHome::_desc(), 0L);
#else
PetStore::CartHomeExplicit::_class_info("PetStore::CartHomeExplicit",
                                        "IDL:PetStore/CartHomeExplicit:1.0",
                                        NULL,
                                        PetStore::CartHomeExplicit::_factory,
                                        NULL, 0, NULL, 0,
                                        Components::CCMHome::_desc(), 0L);
#endif

const CORBA::TypeInfo *PetStore::CartHomeExplicit::_desc() { return &_class_info; }
const CORBA::TypeInfo *PetStore::CartHomeExplicit::_type_info() const {
  return &_class_info;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::CartHomeExplicit_ptr& _obj) {
  CORBA::Object_var _var_obj(_obj);
  _var_obj = CORBA::Object::_read(_strm, PetStore::CartHomeExplicit::_desc());
  _obj = PetStore::CartHomeExplicit::_narrow(_var_obj);
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::CartHomeExplicit_ptr _obj) {
  _strm << (CORBA::Object_ptr)_obj;
  return _strm;
}
void* PetStore::CartHomeExplicit::_safe_narrow(const CORBA::TypeInfo& _info) const {
  if (_info == _class_info)
    return (void *)this;
  if (_info == *Components::CCMHome::_desc())
    return (void *) (Components::CCMHome *) this;
  return CORBA_Object::_safe_narrow(_info);
}

CORBA::Object *PetStore::CartHomeExplicit::_factory() {
  return new PetStore::CartHomeExplicit;
}

PetStore::CartHomeExplicit_ptr PetStore::CartHomeExplicit::_this() {
  return PetStore::CartHomeExplicit::_duplicate(this);
}

PetStore::CartHomeExplicit_ptr PetStore::CartHomeExplicit::_narrow(CORBA::Object * _obj) {
  if (_obj == PetStore::CartHomeExplicit::_nil())
    return PetStore::CartHomeExplicit::_nil();
  else
    return PetStore::CartHomeExplicit::_duplicate((PetStore::CartHomeExplicit_ptr)_obj->_safe_narrow(_class_info));
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::CartHomeExplicit_ptr _obj) {
  _strm << (CORBA::Object_ptr) _obj;
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::CartHomeExplicit_ptr& _obj) {
  VISistream _istrm(_strm);
  _istrm >> _obj;
  return _strm;
}

PetStore::CartHomeExplicit *PetStore::CartHomeExplicit::_bind(const char *_object_name,
                                                              const char *_host_name,
                                                              const CORBA::BindOptions *_opt,
                                                              CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
          "IDL:PetStore/CartHomeExplicit:1.0", _object_name, _host_name, _opt,
_orb);
  return CartHomeExplicit::_narrow(_obj);
}

PetStore::CartHomeExplicit *PetStore::CartHomeExplicit::_bind(const char *_poa_name,
                                                              const CORBA::OctetSequence& _id,
                                                              const char *_host_name,
                                                              const CORBA::BindOptions *_opt,
                                                              CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
   "IDL:PetStore/CartHomeExplicit:1.0", _poa_name, _id, _host_name, _opt, _orb);
  return CartHomeExplicit::_narrow(_obj);
}

PetStore::CartHomeImplicit_ptr PetStore::CartHomeImplicit_var::_duplicate(PetStore::CartHomeImplicit_ptr _p) {
  return PetStore::CartHomeImplicit::_duplicate(_p);
}

void
PetStore::CartHomeImplicit_var::_release(PetStore::CartHomeImplicit_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_p);
}

PetStore::CartHomeImplicit_var::CartHomeImplicit_var()
  : _ptr(PetStore::CartHomeImplicit::_nil()) {}

PetStore::CartHomeImplicit_var::CartHomeImplicit_var(PetStore::CartHomeImplicit_ptr _p)
  : _ptr(_p) {}

PetStore::CartHomeImplicit_var::CartHomeImplicit_var(const PetStore::CartHomeImplicit_var& _var)
  : _ptr(PetStore::CartHomeImplicit::_duplicate((PetStore::CartHomeImplicit_ptr) _var)) {}

PetStore::CartHomeImplicit_var::~CartHomeImplicit_var() {
  CORBA::release((CORBA::Object_ptr)_ptr);
}

PetStore::CartHomeImplicit_var&
PetStore::CartHomeImplicit_var::operator=(const PetStore::CartHomeImplicit_var& _var) {
    _release(_ptr);
    _ptr = _duplicate(_var._ptr);
    return *this;
}

PetStore::CartHomeImplicit_var&
PetStore::CartHomeImplicit_var::operator=(PetStore::CartHomeImplicit_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_ptr);
  _ptr = _p;
  return *this;
}

PetStore::CartHomeImplicit_ptr& PetStore::CartHomeImplicit_var::out() {
  _release(_ptr);
  _ptr = (PetStore::CartHomeImplicit_ptr)NULL;
  return _ptr;
}

PetStore::CartHomeImplicit_ptr PetStore::CartHomeImplicit_var::_retn() {
  PetStore::CartHomeImplicit_ptr _tmp_ptr = _ptr;
  _ptr = (PetStore::CartHomeImplicit_ptr)NULL;
  return _tmp_ptr;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::CartHomeImplicit_var& _var) {
  _strm >> _var._ptr;
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::CartHomeImplicit_var& _var) {
  _strm << _var._ptr;
  return _strm;
}

Istream& PetStore::_marshal_out(Istream& _strm, PetStore::CartHomeImplicit_var& _var) {
  VISistream _istrm(_strm);
  _istrm >> _var._ptr;
  return _strm;
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::CartHomeImplicit_var& _var) {
  _strm << (CORBA::Object_ptr)_var._ptr;
  return _strm;
}

// If dllimport is specified, you might probably want to
// disable these definitions
// disable the const definitions
const VISOps_Info PetStore::CartHomeImplicit_ops::_ops_info("PetStore::CartHomeImplicit_ops");
const VISOps_Info *PetStore::CartHomeImplicit_ops::_desc() { return &_ops_info; }
PetStore::CartHomeImplicit_ops_ptr PetStore::CartHomeImplicit_ops::_downcast(PortableServer::ServantBase* _servant) {
  if (_servant == (PortableServer::ServantBase*)NULL)
    return PetStore::CartHomeImplicit_ops::_nil();
  return (PetStore::CartHomeImplicit_ops_ptr)_servant->_safe_downcast_ops(_ops_info);
}
const CORBA::TypeInfo
#ifdef AIXV3
PetStore::CartHomeImplicit::_class_info("PetStore::CartHomeImplicit",
                                        "IDL:PetStore/CartHomeImplicit:1.0",
                                        NULL,
                                        PetStore::CartHomeImplicit::_factory,
                                        NULL, 0, NULL, 0,
                                        Components::KeylessCCMHome::_desc(),
                                        0L);
#else
PetStore::CartHomeImplicit::_class_info("PetStore::CartHomeImplicit",
                                        "IDL:PetStore/CartHomeImplicit:1.0",
                                        NULL,
                                        PetStore::CartHomeImplicit::_factory,
                                        NULL, 0, NULL, 0,
                                        Components::KeylessCCMHome::_desc(),
                                        0L);
#endif

const CORBA::TypeInfo *PetStore::CartHomeImplicit::_desc() { return &_class_info; }
const CORBA::TypeInfo *PetStore::CartHomeImplicit::_type_info() const {
  return &_class_info;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::CartHomeImplicit_ptr& _obj) {
  CORBA::Object_var _var_obj(_obj);
  _var_obj = CORBA::Object::_read(_strm, PetStore::CartHomeImplicit::_desc());
  _obj = PetStore::CartHomeImplicit::_narrow(_var_obj);
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::CartHomeImplicit_ptr _obj) {
  _strm << (CORBA::Object_ptr)_obj;
  return _strm;
}
void* PetStore::CartHomeImplicit::_safe_narrow(const CORBA::TypeInfo& _info) const {
  if (_info == _class_info)
    return (void *)this;
  if (_info == *Components::KeylessCCMHome::_desc())
    return (void *) (Components::KeylessCCMHome *) this;
  return CORBA_Object::_safe_narrow(_info);
}

CORBA::Object *PetStore::CartHomeImplicit::_factory() {
  return new PetStore::CartHomeImplicit;
}

PetStore::CartHomeImplicit_ptr PetStore::CartHomeImplicit::_this() {
  return PetStore::CartHomeImplicit::_duplicate(this);
}

PetStore::CartHomeImplicit_ptr PetStore::CartHomeImplicit::_narrow(CORBA::Object * _obj) {
  if (_obj == PetStore::CartHomeImplicit::_nil())
    return PetStore::CartHomeImplicit::_nil();
  else
    return PetStore::CartHomeImplicit::_duplicate((PetStore::CartHomeImplicit_ptr)_obj->_safe_narrow(_class_info));
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::CartHomeImplicit_ptr _obj) {
  _strm << (CORBA::Object_ptr) _obj;
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::CartHomeImplicit_ptr& _obj) {
  VISistream _istrm(_strm);
  _istrm >> _obj;
  return _strm;
}

PetStore::CartHomeImplicit *PetStore::CartHomeImplicit::_bind(const char *_object_name,
                                                              const char *_host_name,
                                                              const CORBA::BindOptions *_opt,
                                                              CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
          "IDL:PetStore/CartHomeImplicit:1.0", _object_name, _host_name, _opt,
_orb);
  return CartHomeImplicit::_narrow(_obj);
}

PetStore::CartHomeImplicit *PetStore::CartHomeImplicit::_bind(const char *_poa_name,
                                                              const CORBA::OctetSequence& _id,
                                                              const char *_host_name,
                                                              const CORBA::BindOptions *_opt,
                                                              CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
   "IDL:PetStore/CartHomeImplicit:1.0", _poa_name, _id, _host_name, _opt, _orb);
  return CartHomeImplicit::_narrow(_obj);
}
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
::PetStore::Cart_ptr PetStore::CartHomeImplicit::create() {
  ::PetStore::Cart_ptr _ret = (::PetStore::Cart_ptr) NULL;
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("create");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::CartHomeImplicit_ops*  _carthomeimplicit = PetStore::CartHomeImplicit_ops::_downcast(_servant);
      if (!_carthomeimplicit) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "create");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }

      VISTRY {
         _ret = _carthomeimplicit->create();
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "create"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "create");
      return _ret;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("create", 1);
    VISIF_EXCEP(return _ret;)

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return _ret;)
#endif
  }

  VISistream& _istrm = *VISistream::_downcast(_ibuf);
  _istrm >> _ret;
  return _ret;
}

#if defined(__BORLANDC__)
 #pragma option pop
#endif

PetStore::CartHome_ptr PetStore::CartHome_var::_duplicate(PetStore::CartHome_ptr _p) {
  return PetStore::CartHome::_duplicate(_p);
}

void
PetStore::CartHome_var::_release(PetStore::CartHome_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_p);
}

PetStore::CartHome_var::CartHome_var()
  : _ptr(PetStore::CartHome::_nil()) {}

PetStore::CartHome_var::CartHome_var(PetStore::CartHome_ptr _p)
  : _ptr(_p) {}

PetStore::CartHome_var::CartHome_var(const PetStore::CartHome_var& _var)
  : _ptr(PetStore::CartHome::_duplicate((PetStore::CartHome_ptr) _var)) {}

PetStore::CartHome_var::~CartHome_var() {
  CORBA::release((CORBA::Object_ptr)_ptr);
}

PetStore::CartHome_var&
PetStore::CartHome_var::operator=(const PetStore::CartHome_var& _var) {
    _release(_ptr);
    _ptr = _duplicate(_var._ptr);
    return *this;
}

PetStore::CartHome_var&
PetStore::CartHome_var::operator=(PetStore::CartHome_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_ptr);
  _ptr = _p;
  return *this;
}

PetStore::CartHome_ptr& PetStore::CartHome_var::out() {
  _release(_ptr);
  _ptr = (PetStore::CartHome_ptr)NULL;
  return _ptr;
}

PetStore::CartHome_ptr PetStore::CartHome_var::_retn() {
  PetStore::CartHome_ptr _tmp_ptr = _ptr;
  _ptr = (PetStore::CartHome_ptr)NULL;
  return _tmp_ptr;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::CartHome_var& _var) {
  _strm >> _var._ptr;
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::CartHome_var& _var) {
  _strm << _var._ptr;
  return _strm;
}

Istream& PetStore::_marshal_out(Istream& _strm, PetStore::CartHome_var& _var) {
  VISistream _istrm(_strm);
  _istrm >> _var._ptr;
  return _strm;
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::CartHome_var& _var) {
  _strm << (CORBA::Object_ptr)_var._ptr;
  return _strm;
}

// If dllimport is specified, you might probably want to
// disable these definitions
// disable the const definitions
const VISOps_Info PetStore::CartHome_ops::_ops_info("PetStore::CartHome_ops");
const VISOps_Info *PetStore::CartHome_ops::_desc() { return &_ops_info; }
PetStore::CartHome_ops_ptr PetStore::CartHome_ops::_downcast(PortableServer::ServantBase* _servant) {
  if (_servant == (PortableServer::ServantBase*)NULL)
    return PetStore::CartHome_ops::_nil();
  return (PetStore::CartHome_ops_ptr)_servant->_safe_downcast_ops(_ops_info);
}
const CORBA::TypeInfo
#ifdef AIXV3
PetStore::CartHome::_class_info("PetStore::CartHome",
                                "IDL:PetStore/CartHome:1.0", NULL,
                                PetStore::CartHome::_factory, NULL, 0, NULL,
                                0, PetStore::CartHomeExplicit::_desc(),
                                PetStore::CartHomeImplicit::_desc(), 0L);
#else
PetStore::CartHome::_class_info("PetStore::CartHome",
                                "IDL:PetStore/CartHome:1.0", NULL,
                                PetStore::CartHome::_factory, NULL, 0, NULL,
                                0, PetStore::CartHomeExplicit::_desc(),
                                PetStore::CartHomeImplicit::_desc(), 0L);
#endif

const CORBA::TypeInfo *PetStore::CartHome::_desc() { return &_class_info; }
const CORBA::TypeInfo *PetStore::CartHome::_type_info() const {
  return &_class_info;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::CartHome_ptr& _obj) {
  CORBA::Object_var _var_obj(_obj);
  _var_obj = CORBA::Object::_read(_strm, PetStore::CartHome::_desc());
  _obj = PetStore::CartHome::_narrow(_var_obj);
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::CartHome_ptr _obj) {
  _strm << (CORBA::Object_ptr)_obj;
  return _strm;
}
void* PetStore::CartHome::_safe_narrow(const CORBA::TypeInfo& _info) const {
  if (_info == _class_info)
    return (void *)this;
  if (_info == *PetStore::CartHomeExplicit::_desc())
    return (void *) (PetStore::CartHomeExplicit *) this;
  if (_info == *Components::CCMHome::_desc())
    return (void *) (Components::CCMHome *) this;
  if (_info == *PetStore::CartHomeImplicit::_desc())
    return (void *) (PetStore::CartHomeImplicit *) this;
  if (_info == *Components::KeylessCCMHome::_desc())
    return (void *) (Components::KeylessCCMHome *) this;
  return CORBA_Object::_safe_narrow(_info);
}

CORBA::Object *PetStore::CartHome::_factory() {
  return new PetStore::CartHome;
}

PetStore::CartHome_ptr PetStore::CartHome::_this() {
  return PetStore::CartHome::_duplicate(this);
}

PetStore::CartHome_ptr PetStore::CartHome::_narrow(CORBA::Object * _obj) {
  if (_obj == PetStore::CartHome::_nil())
    return PetStore::CartHome::_nil();
  else
    return PetStore::CartHome::_duplicate((PetStore::CartHome_ptr)_obj->_safe_narrow(_class_info));
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::CartHome_ptr _obj) {
  _strm << (CORBA::Object_ptr) _obj;
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::CartHome_ptr& _obj) {
  VISistream _istrm(_strm);
  _istrm >> _obj;
  return _strm;
}

PetStore::CartHome *PetStore::CartHome::_bind(const char *_object_name,
                                              const char *_host_name,
                                              const CORBA::BindOptions *_opt,
                                              CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
          "IDL:PetStore/CartHome:1.0", _object_name, _host_name, _opt, _orb);
  return CartHome::_narrow(_obj);
}

PetStore::CartHome *PetStore::CartHome::_bind(const char *_poa_name,
                                              const CORBA::OctetSequence& _id,
                                              const char *_host_name,
                                              const CORBA::BindOptions *_opt,
                                              CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
   "IDL:PetStore/CartHome:1.0", _poa_name, _id, _host_name, _opt, _orb);
  return CartHome::_narrow(_obj);
}

#if defined(MSVCNEWDLL_BUG)
void *PetStore::ProductInfoList::operator new(size_t ts) {
  return ::operator new(ts);
}
void PetStore::ProductInfoList::operator delete(void *p) {
  ::operator delete(p);
}
#endif // MSVCNEWDLL_BUG

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::ProductInfoList& _seq) {
  VISostream _ostrm(_strm);
  if (&_strm == &Cout || &_strm == &Cerr || &_strm == &Clog) {
    _pretty_print(_ostrm, _seq);
    _ostrm << endl;
  } else {
    _ostrm << _seq;
  }
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::ProductInfoList& _obj) {
  VISistream _istrm(_strm);
  _istrm >> _obj;
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::ProductInfoList_ptr& _ptr) {
  VISistream _istrm(_strm);
  _istrm >> _ptr;
  return _strm;
}

::PetStore::ProductInfo* PetStore::ProductInfoList::allocbuf(CORBA::ULong _nelems) {
  if (_nelems) {
    return new ::PetStore::ProductInfo[_nelems];
  }
  return (::PetStore::ProductInfo *)NULL;
}

void PetStore::ProductInfoList::freebuf(::PetStore::ProductInfo *_data) {
  if (_data)
    delete[] _data;
}

PetStore::ProductInfoList::ProductInfoList(CORBA::ULong _max) {
  _num_allocated = _max;
  _count = 0;
  _contents = allocbuf(_num_allocated);
  _release_flag = 1;
}

PetStore::ProductInfoList::ProductInfoList(CORBA::ULong _max,
                                           CORBA::ULong _len,
                                           ::PetStore::ProductInfo *_data,
                                           CORBA::Boolean _release) {
  _num_allocated = _max;
  _count = _len;
  _contents = _data;
  _release_flag = _release;
}

PetStore::ProductInfoList::ProductInfoList(const PetStore::ProductInfoList& _seq) {
  _num_allocated = _seq._num_allocated;
  _count = _seq._count;
  _contents = allocbuf(_num_allocated);
  if (_num_allocated > 0) {
    for (CORBA::ULong i=0; i < _count; i++) {
      _contents[i] = _seq._contents[i];
    }
  }
  _release_flag = 1;
}

PetStore::ProductInfoList::~ProductInfoList() {
  if (_release_flag) 
    freebuf(_contents);
}

PetStore::ProductInfoList& PetStore::ProductInfoList::operator=(const PetStore::ProductInfoList& _seq) {
  if (_num_allocated < _seq._count) {
    if (_release_flag) 
      freebuf(_contents);
    else
      _release_flag = (CORBA::Boolean) 1;

    _contents = allocbuf(_seq._num_allocated);
    _num_allocated = _seq._num_allocated;
  }
  _count = _seq._count;
  if (_num_allocated > 0) {
    for (CORBA::ULong i = 0; i < _count; i++) {
      _contents[i] = _seq._contents[i];
    }
  }
  return *this;
}

void PetStore::ProductInfoList::length(CORBA::ULong _newlen) {
  if (_newlen > _num_allocated) {
    ::PetStore::ProductInfo *temp = allocbuf(_newlen);
    for (CORBA::ULong i=0; i < _count; i++) {
      temp[i] = _contents[i];
    }
    if (_release_flag) 
      freebuf(_contents);
    else
      _release_flag = (CORBA::Boolean) 1;

    _num_allocated = _newlen;
    _contents = temp;
  }
  _count = _newlen;
}

void PetStore::ProductInfoList::replace(CORBA::ULong _max, CORBA::ULong _len,
                                        ProductInfo *_data,
                                        CORBA::Boolean _release) {
  if (_release_flag) 
    freebuf(_contents);
  _num_allocated = _max;
  _count = _len;
  _contents = _data;
  _release_flag = _release;
}

::PetStore::ProductInfo *PetStore::ProductInfoList::get_buffer(CORBA::Boolean orphan) {
  if (orphan == (CORBA::Boolean) 1) {
    if (_release_flag == (CORBA::Boolean) 0)
      return (::PetStore::ProductInfo*)NULL;
    else {
      _release_flag = (CORBA::Boolean) 0;
      return _contents;
    }
  } else
    return _contents;
}

const ::PetStore::ProductInfo* PetStore::ProductInfoList::get_buffer() const {
  // Do not use const_cast here because it might not be supported by the compiler
  return (const ::PetStore::ProductInfo*)(void*)_contents;
}

::PetStore::ProductInfo& PetStore::ProductInfoList::operator[](CORBA::ULong _index) {
  if (_index >= _count) {
    VISTHROW(CORBA::BAD_PARAM())
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(static ::PetStore::ProductInfo *_null = 0; return *_null;)
#endif
  }
  return _contents[_index];
}

const ::PetStore::ProductInfo& PetStore::ProductInfoList::operator[](CORBA::ULong _index) const {
  if (_index >= _count) {
    VISTHROW(CORBA::BAD_PARAM())
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(static ::PetStore::ProductInfo *_null = 0; return (const ::PetStore::ProductInfo&)*_null;)
#endif
  }
  // Do not use const_cast here because it might not be supported by the compiler
  return *(const ::PetStore::ProductInfo*)(void*)&_contents[_index];
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::ProductInfoList& _seq) {
  _strm << _seq._count;
  for (CORBA::ULong i=0; i < _seq._count; i++) {
    _strm << _seq._contents[i];
  }
  return _strm;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::ProductInfoList& _seq) {
  CORBA::ULong t_count;
  _strm >> t_count;

  if (t_count > _seq._count) {
    if (_seq._num_allocated < t_count) {
      if (_seq._release_flag)
        PetStore::ProductInfoList::freebuf(_seq._contents);
      else
        _seq._release_flag = (CORBA::Boolean) 1;
      _seq._contents = PetStore::ProductInfoList::allocbuf(t_count);
      _seq._num_allocated = t_count;
    }
  }
  _seq._count = t_count;

  for (CORBA::ULong i=0; i < _seq._count; i++) {
    _strm >> _seq._contents[i];
  }
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::ProductInfoList* _obj) {
  if (!_obj)
    VISTHROW(CORBA::BAD_PARAM())
  else
    _strm << *_obj;
  return _strm;
}
VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::ProductInfoList_ptr& _obj) {
  _obj = new PetStore::ProductInfoList();
  _strm >> *_obj;
  return _strm;
}

void PetStore::_internal_pretty_print(VISostream& _strm, const PetStore::ProductInfoList& _seq) {
  _strm << "SEQUENCE PetStore::ProductInfoList" << endl;
  _strm << "Number of Elements: " << _seq._count << endl;
  for (CORBA::ULong i=0; i < _seq._count; i++) {
    _strm << "[" << i << "]" << endl;
    _strm << "\t_contents[i]:" << endl;
    _pretty_print(_strm, _seq._contents[i]);
    _strm << endl;
    _strm << endl;
  }
}
::PetStore::ProductInfo& PetStore::ProductInfoList_var::operator[](CORBA::ULong _index) {
  if (_ptr)
    return (*_ptr)[_index];
  static PetStore::ProductInfoList _tmp;
  return _tmp[_index];
}

const ::PetStore::ProductInfo& PetStore::ProductInfoList_var::operator[](CORBA::ULong _index) const {
  if (_ptr)
    return (*(const PetStore::ProductInfoList*)_ptr)[_index];
  static const PetStore::ProductInfoList _tmp;
  return _tmp[_index];
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::ProductInfoList_var& _var) {
  if (!_var._ptr)
    VISTHROW(CORBA::BAD_PARAM())
  else
    _strm << *_var._ptr;
  return _strm;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::ProductInfoList_var& _var) {
  if (!_var._ptr)
    _var._ptr = new PetStore::ProductInfoList;
  _strm >> *_var._ptr;
  return _strm;
}

PetStore::SalesHomeExplicit_ptr PetStore::SalesHomeExplicit_var::_duplicate(PetStore::SalesHomeExplicit_ptr _p) {
  return PetStore::SalesHomeExplicit::_duplicate(_p);
}

void
PetStore::SalesHomeExplicit_var::_release(PetStore::SalesHomeExplicit_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_p);
}

PetStore::SalesHomeExplicit_var::SalesHomeExplicit_var()
  : _ptr(PetStore::SalesHomeExplicit::_nil()) {}

PetStore::SalesHomeExplicit_var::SalesHomeExplicit_var(PetStore::SalesHomeExplicit_ptr _p)
  : _ptr(_p) {}

PetStore::SalesHomeExplicit_var::SalesHomeExplicit_var(const PetStore::SalesHomeExplicit_var& _var)
  : _ptr(PetStore::SalesHomeExplicit::_duplicate((PetStore::SalesHomeExplicit_ptr) _var)) {}

PetStore::SalesHomeExplicit_var::~SalesHomeExplicit_var() {
  CORBA::release((CORBA::Object_ptr)_ptr);
}

PetStore::SalesHomeExplicit_var&
PetStore::SalesHomeExplicit_var::operator=(const PetStore::SalesHomeExplicit_var& _var) {
    _release(_ptr);
    _ptr = _duplicate(_var._ptr);
    return *this;
}

PetStore::SalesHomeExplicit_var&
PetStore::SalesHomeExplicit_var::operator=(PetStore::SalesHomeExplicit_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_ptr);
  _ptr = _p;
  return *this;
}

PetStore::SalesHomeExplicit_ptr& PetStore::SalesHomeExplicit_var::out() {
  _release(_ptr);
  _ptr = (PetStore::SalesHomeExplicit_ptr)NULL;
  return _ptr;
}

PetStore::SalesHomeExplicit_ptr PetStore::SalesHomeExplicit_var::_retn() {
  PetStore::SalesHomeExplicit_ptr _tmp_ptr = _ptr;
  _ptr = (PetStore::SalesHomeExplicit_ptr)NULL;
  return _tmp_ptr;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::SalesHomeExplicit_var& _var) {
  _strm >> _var._ptr;
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::SalesHomeExplicit_var& _var) {
  _strm << _var._ptr;
  return _strm;
}

Istream& PetStore::_marshal_out(Istream& _strm, PetStore::SalesHomeExplicit_var& _var) {
  VISistream _istrm(_strm);
  _istrm >> _var._ptr;
  return _strm;
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::SalesHomeExplicit_var& _var) {
  _strm << (CORBA::Object_ptr)_var._ptr;
  return _strm;
}

// If dllimport is specified, you might probably want to
// disable these definitions
// disable the const definitions
const VISOps_Info PetStore::SalesHomeExplicit_ops::_ops_info("PetStore::SalesHomeExplicit_ops");
const VISOps_Info *PetStore::SalesHomeExplicit_ops::_desc() { return &_ops_info; }
PetStore::SalesHomeExplicit_ops_ptr PetStore::SalesHomeExplicit_ops::_downcast(PortableServer::ServantBase* _servant) {
  if (_servant == (PortableServer::ServantBase*)NULL)
    return PetStore::SalesHomeExplicit_ops::_nil();
  return (PetStore::SalesHomeExplicit_ops_ptr)_servant->_safe_downcast_ops(_ops_info);
}
const CORBA::TypeInfo
#ifdef AIXV3
PetStore::SalesHomeExplicit::_class_info("PetStore::SalesHomeExplicit",
                                         "IDL:PetStore/SalesHomeExplicit:1.0",
                                         NULL,
                                         PetStore::SalesHomeExplicit::_factory,
                                         NULL, 0, NULL, 0,
                                         Components::CCMHome::_desc(), 0L);
#else
PetStore::SalesHomeExplicit::_class_info("PetStore::SalesHomeExplicit",
                                         "IDL:PetStore/SalesHomeExplicit:1.0",
                                         NULL,
                                         PetStore::SalesHomeExplicit::_factory,
                                         NULL, 0, NULL, 0,
                                         Components::CCMHome::_desc(), 0L);
#endif

const CORBA::TypeInfo *PetStore::SalesHomeExplicit::_desc() { return &_class_info; }
const CORBA::TypeInfo *PetStore::SalesHomeExplicit::_type_info() const {
  return &_class_info;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::SalesHomeExplicit_ptr& _obj) {
  CORBA::Object_var _var_obj(_obj);
  _var_obj = CORBA::Object::_read(_strm, PetStore::SalesHomeExplicit::_desc());
  _obj = PetStore::SalesHomeExplicit::_narrow(_var_obj);
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::SalesHomeExplicit_ptr _obj) {
  _strm << (CORBA::Object_ptr)_obj;
  return _strm;
}
void* PetStore::SalesHomeExplicit::_safe_narrow(const CORBA::TypeInfo& _info) const {
  if (_info == _class_info)
    return (void *)this;
  if (_info == *Components::CCMHome::_desc())
    return (void *) (Components::CCMHome *) this;
  return CORBA_Object::_safe_narrow(_info);
}

CORBA::Object *PetStore::SalesHomeExplicit::_factory() {
  return new PetStore::SalesHomeExplicit;
}

PetStore::SalesHomeExplicit_ptr PetStore::SalesHomeExplicit::_this() {
  return PetStore::SalesHomeExplicit::_duplicate(this);
}

PetStore::SalesHomeExplicit_ptr PetStore::SalesHomeExplicit::_narrow(CORBA::Object * _obj) {
  if (_obj == PetStore::SalesHomeExplicit::_nil())
    return PetStore::SalesHomeExplicit::_nil();
  else
    return PetStore::SalesHomeExplicit::_duplicate((PetStore::SalesHomeExplicit_ptr)_obj->_safe_narrow(_class_info));
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::SalesHomeExplicit_ptr _obj) {
  _strm << (CORBA::Object_ptr) _obj;
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::SalesHomeExplicit_ptr& _obj) {
  VISistream _istrm(_strm);
  _istrm >> _obj;
  return _strm;
}

PetStore::SalesHomeExplicit *PetStore::SalesHomeExplicit::_bind(const char *_object_name,
                                                                const char *_host_name,
                                                                const CORBA::BindOptions *_opt,
                                                                CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
          "IDL:PetStore/SalesHomeExplicit:1.0", _object_name, _host_name,
_opt, _orb);
  return SalesHomeExplicit::_narrow(_obj);
}

PetStore::SalesHomeExplicit *PetStore::SalesHomeExplicit::_bind(const char *_poa_name,
                                                                const CORBA::OctetSequence& _id,
                                                                const char *_host_name,
                                                                const CORBA::BindOptions *_opt,
                                                                CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
   "IDL:PetStore/SalesHomeExplicit:1.0", _poa_name, _id, _host_name, _opt,
   _orb);
  return SalesHomeExplicit::_narrow(_obj);
}

PetStore::SalesHomeImplicit_ptr PetStore::SalesHomeImplicit_var::_duplicate(PetStore::SalesHomeImplicit_ptr _p) {
  return PetStore::SalesHomeImplicit::_duplicate(_p);
}

void
PetStore::SalesHomeImplicit_var::_release(PetStore::SalesHomeImplicit_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_p);
}

PetStore::SalesHomeImplicit_var::SalesHomeImplicit_var()
  : _ptr(PetStore::SalesHomeImplicit::_nil()) {}

PetStore::SalesHomeImplicit_var::SalesHomeImplicit_var(PetStore::SalesHomeImplicit_ptr _p)
  : _ptr(_p) {}

PetStore::SalesHomeImplicit_var::SalesHomeImplicit_var(const PetStore::SalesHomeImplicit_var& _var)
  : _ptr(PetStore::SalesHomeImplicit::_duplicate((PetStore::SalesHomeImplicit_ptr) _var)) {}

PetStore::SalesHomeImplicit_var::~SalesHomeImplicit_var() {
  CORBA::release((CORBA::Object_ptr)_ptr);
}

PetStore::SalesHomeImplicit_var&
PetStore::SalesHomeImplicit_var::operator=(const PetStore::SalesHomeImplicit_var& _var) {
    _release(_ptr);
    _ptr = _duplicate(_var._ptr);
    return *this;
}

PetStore::SalesHomeImplicit_var&
PetStore::SalesHomeImplicit_var::operator=(PetStore::SalesHomeImplicit_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_ptr);
  _ptr = _p;
  return *this;
}

PetStore::SalesHomeImplicit_ptr& PetStore::SalesHomeImplicit_var::out() {
  _release(_ptr);
  _ptr = (PetStore::SalesHomeImplicit_ptr)NULL;
  return _ptr;
}

PetStore::SalesHomeImplicit_ptr PetStore::SalesHomeImplicit_var::_retn() {
  PetStore::SalesHomeImplicit_ptr _tmp_ptr = _ptr;
  _ptr = (PetStore::SalesHomeImplicit_ptr)NULL;
  return _tmp_ptr;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::SalesHomeImplicit_var& _var) {
  _strm >> _var._ptr;
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::SalesHomeImplicit_var& _var) {
  _strm << _var._ptr;
  return _strm;
}

Istream& PetStore::_marshal_out(Istream& _strm, PetStore::SalesHomeImplicit_var& _var) {
  VISistream _istrm(_strm);
  _istrm >> _var._ptr;
  return _strm;
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::SalesHomeImplicit_var& _var) {
  _strm << (CORBA::Object_ptr)_var._ptr;
  return _strm;
}

// If dllimport is specified, you might probably want to
// disable these definitions
// disable the const definitions
const VISOps_Info PetStore::SalesHomeImplicit_ops::_ops_info("PetStore::SalesHomeImplicit_ops");
const VISOps_Info *PetStore::SalesHomeImplicit_ops::_desc() { return &_ops_info; }
PetStore::SalesHomeImplicit_ops_ptr PetStore::SalesHomeImplicit_ops::_downcast(PortableServer::ServantBase* _servant) {
  if (_servant == (PortableServer::ServantBase*)NULL)
    return PetStore::SalesHomeImplicit_ops::_nil();
  return (PetStore::SalesHomeImplicit_ops_ptr)_servant->_safe_downcast_ops(_ops_info);
}
const CORBA::TypeInfo
#ifdef AIXV3
PetStore::SalesHomeImplicit::_class_info("PetStore::SalesHomeImplicit",
                                         "IDL:PetStore/SalesHomeImplicit:1.0",
                                         NULL,
                                         PetStore::SalesHomeImplicit::_factory,
                                         NULL, 0, NULL, 0,
                                         Components::KeylessCCMHome::_desc(),
                                         0L);
#else
PetStore::SalesHomeImplicit::_class_info("PetStore::SalesHomeImplicit",
                                         "IDL:PetStore/SalesHomeImplicit:1.0",
                                         NULL,
                                         PetStore::SalesHomeImplicit::_factory,
                                         NULL, 0, NULL, 0,
                                         Components::KeylessCCMHome::_desc(),
                                         0L);
#endif

const CORBA::TypeInfo *PetStore::SalesHomeImplicit::_desc() { return &_class_info; }
const CORBA::TypeInfo *PetStore::SalesHomeImplicit::_type_info() const {
  return &_class_info;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::SalesHomeImplicit_ptr& _obj) {
  CORBA::Object_var _var_obj(_obj);
  _var_obj = CORBA::Object::_read(_strm, PetStore::SalesHomeImplicit::_desc());
  _obj = PetStore::SalesHomeImplicit::_narrow(_var_obj);
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::SalesHomeImplicit_ptr _obj) {
  _strm << (CORBA::Object_ptr)_obj;
  return _strm;
}
void* PetStore::SalesHomeImplicit::_safe_narrow(const CORBA::TypeInfo& _info) const {
  if (_info == _class_info)
    return (void *)this;
  if (_info == *Components::KeylessCCMHome::_desc())
    return (void *) (Components::KeylessCCMHome *) this;
  return CORBA_Object::_safe_narrow(_info);
}

CORBA::Object *PetStore::SalesHomeImplicit::_factory() {
  return new PetStore::SalesHomeImplicit;
}

PetStore::SalesHomeImplicit_ptr PetStore::SalesHomeImplicit::_this() {
  return PetStore::SalesHomeImplicit::_duplicate(this);
}

PetStore::SalesHomeImplicit_ptr PetStore::SalesHomeImplicit::_narrow(CORBA::Object * _obj) {
  if (_obj == PetStore::SalesHomeImplicit::_nil())
    return PetStore::SalesHomeImplicit::_nil();
  else
    return PetStore::SalesHomeImplicit::_duplicate((PetStore::SalesHomeImplicit_ptr)_obj->_safe_narrow(_class_info));
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::SalesHomeImplicit_ptr _obj) {
  _strm << (CORBA::Object_ptr) _obj;
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::SalesHomeImplicit_ptr& _obj) {
  VISistream _istrm(_strm);
  _istrm >> _obj;
  return _strm;
}

PetStore::SalesHomeImplicit *PetStore::SalesHomeImplicit::_bind(const char *_object_name,
                                                                const char *_host_name,
                                                                const CORBA::BindOptions *_opt,
                                                                CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
          "IDL:PetStore/SalesHomeImplicit:1.0", _object_name, _host_name,
_opt, _orb);
  return SalesHomeImplicit::_narrow(_obj);
}

PetStore::SalesHomeImplicit *PetStore::SalesHomeImplicit::_bind(const char *_poa_name,
                                                                const CORBA::OctetSequence& _id,
                                                                const char *_host_name,
                                                                const CORBA::BindOptions *_opt,
                                                                CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
   "IDL:PetStore/SalesHomeImplicit:1.0", _poa_name, _id, _host_name, _opt,
   _orb);
  return SalesHomeImplicit::_narrow(_obj);
}
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
::PetStore::Sales_ptr PetStore::SalesHomeImplicit::create() {
  ::PetStore::Sales_ptr _ret = (::PetStore::Sales_ptr) NULL;
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("create");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::SalesHomeImplicit_ops*  _saleshomeimplicit = PetStore::SalesHomeImplicit_ops::_downcast(_servant);
      if (!_saleshomeimplicit) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "create");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }

      VISTRY {
         _ret = _saleshomeimplicit->create();
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "create"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "create");
      return _ret;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("create", 1);
    VISIF_EXCEP(return _ret;)

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return _ret;)
#endif
  }

  VISistream& _istrm = *VISistream::_downcast(_ibuf);
  _istrm >> _ret;
  return _ret;
}

#if defined(__BORLANDC__)
 #pragma option pop
#endif

PetStore::SalesHome_ptr PetStore::SalesHome_var::_duplicate(PetStore::SalesHome_ptr _p) {
  return PetStore::SalesHome::_duplicate(_p);
}

void
PetStore::SalesHome_var::_release(PetStore::SalesHome_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_p);
}

PetStore::SalesHome_var::SalesHome_var()
  : _ptr(PetStore::SalesHome::_nil()) {}

PetStore::SalesHome_var::SalesHome_var(PetStore::SalesHome_ptr _p)
  : _ptr(_p) {}

PetStore::SalesHome_var::SalesHome_var(const PetStore::SalesHome_var& _var)
  : _ptr(PetStore::SalesHome::_duplicate((PetStore::SalesHome_ptr) _var)) {}

PetStore::SalesHome_var::~SalesHome_var() {
  CORBA::release((CORBA::Object_ptr)_ptr);
}

PetStore::SalesHome_var&
PetStore::SalesHome_var::operator=(const PetStore::SalesHome_var& _var) {
    _release(_ptr);
    _ptr = _duplicate(_var._ptr);
    return *this;
}

PetStore::SalesHome_var&
PetStore::SalesHome_var::operator=(PetStore::SalesHome_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_ptr);
  _ptr = _p;
  return *this;
}

PetStore::SalesHome_ptr& PetStore::SalesHome_var::out() {
  _release(_ptr);
  _ptr = (PetStore::SalesHome_ptr)NULL;
  return _ptr;
}

PetStore::SalesHome_ptr PetStore::SalesHome_var::_retn() {
  PetStore::SalesHome_ptr _tmp_ptr = _ptr;
  _ptr = (PetStore::SalesHome_ptr)NULL;
  return _tmp_ptr;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::SalesHome_var& _var) {
  _strm >> _var._ptr;
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::SalesHome_var& _var) {
  _strm << _var._ptr;
  return _strm;
}

Istream& PetStore::_marshal_out(Istream& _strm, PetStore::SalesHome_var& _var) {
  VISistream _istrm(_strm);
  _istrm >> _var._ptr;
  return _strm;
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::SalesHome_var& _var) {
  _strm << (CORBA::Object_ptr)_var._ptr;
  return _strm;
}

// If dllimport is specified, you might probably want to
// disable these definitions
// disable the const definitions
const VISOps_Info PetStore::SalesHome_ops::_ops_info("PetStore::SalesHome_ops");
const VISOps_Info *PetStore::SalesHome_ops::_desc() { return &_ops_info; }
PetStore::SalesHome_ops_ptr PetStore::SalesHome_ops::_downcast(PortableServer::ServantBase* _servant) {
  if (_servant == (PortableServer::ServantBase*)NULL)
    return PetStore::SalesHome_ops::_nil();
  return (PetStore::SalesHome_ops_ptr)_servant->_safe_downcast_ops(_ops_info);
}
const CORBA::TypeInfo
#ifdef AIXV3
PetStore::SalesHome::_class_info("PetStore::SalesHome",
                                 "IDL:PetStore/SalesHome:1.0", NULL,
                                 PetStore::SalesHome::_factory, NULL, 0, NULL,
                                 0, PetStore::SalesHomeExplicit::_desc(),
                                 PetStore::SalesHomeImplicit::_desc(), 0L);
#else
PetStore::SalesHome::_class_info("PetStore::SalesHome",
                                 "IDL:PetStore/SalesHome:1.0", NULL,
                                 PetStore::SalesHome::_factory, NULL, 0, NULL,
                                 0, PetStore::SalesHomeExplicit::_desc(),
                                 PetStore::SalesHomeImplicit::_desc(), 0L);
#endif

const CORBA::TypeInfo *PetStore::SalesHome::_desc() { return &_class_info; }
const CORBA::TypeInfo *PetStore::SalesHome::_type_info() const {
  return &_class_info;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::SalesHome_ptr& _obj) {
  CORBA::Object_var _var_obj(_obj);
  _var_obj = CORBA::Object::_read(_strm, PetStore::SalesHome::_desc());
  _obj = PetStore::SalesHome::_narrow(_var_obj);
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::SalesHome_ptr _obj) {
  _strm << (CORBA::Object_ptr)_obj;
  return _strm;
}
void* PetStore::SalesHome::_safe_narrow(const CORBA::TypeInfo& _info) const {
  if (_info == _class_info)
    return (void *)this;
  if (_info == *PetStore::SalesHomeExplicit::_desc())
    return (void *) (PetStore::SalesHomeExplicit *) this;
  if (_info == *Components::CCMHome::_desc())
    return (void *) (Components::CCMHome *) this;
  if (_info == *PetStore::SalesHomeImplicit::_desc())
    return (void *) (PetStore::SalesHomeImplicit *) this;
  if (_info == *Components::KeylessCCMHome::_desc())
    return (void *) (Components::KeylessCCMHome *) this;
  return CORBA_Object::_safe_narrow(_info);
}

CORBA::Object *PetStore::SalesHome::_factory() {
  return new PetStore::SalesHome;
}

PetStore::SalesHome_ptr PetStore::SalesHome::_this() {
  return PetStore::SalesHome::_duplicate(this);
}

PetStore::SalesHome_ptr PetStore::SalesHome::_narrow(CORBA::Object * _obj) {
  if (_obj == PetStore::SalesHome::_nil())
    return PetStore::SalesHome::_nil();
  else
    return PetStore::SalesHome::_duplicate((PetStore::SalesHome_ptr)_obj->_safe_narrow(_class_info));
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::SalesHome_ptr _obj) {
  _strm << (CORBA::Object_ptr) _obj;
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::SalesHome_ptr& _obj) {
  VISistream _istrm(_strm);
  _istrm >> _obj;
  return _strm;
}

PetStore::SalesHome *PetStore::SalesHome::_bind(const char *_object_name,
                                                const char *_host_name,
                                                const CORBA::BindOptions *_opt,
                                                CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
          "IDL:PetStore/SalesHome:1.0", _object_name, _host_name, _opt, _orb);
  return SalesHome::_narrow(_obj);
}

PetStore::SalesHome *PetStore::SalesHome::_bind(const char *_poa_name,
                                                const CORBA::OctetSequence& _id,
                                                const char *_host_name,
                                                const CORBA::BindOptions *_opt,
                                                CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
   "IDL:PetStore/SalesHome:1.0", _poa_name, _id, _host_name, _opt, _orb);
  return SalesHome::_narrow(_obj);
}

PetStore::ManagerHomeExplicit_ptr PetStore::ManagerHomeExplicit_var::_duplicate(PetStore::ManagerHomeExplicit_ptr _p) {
  return PetStore::ManagerHomeExplicit::_duplicate(_p);
}

void
PetStore::ManagerHomeExplicit_var::_release(PetStore::ManagerHomeExplicit_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_p);
}

PetStore::ManagerHomeExplicit_var::ManagerHomeExplicit_var()
  : _ptr(PetStore::ManagerHomeExplicit::_nil()) {}

PetStore::ManagerHomeExplicit_var::ManagerHomeExplicit_var(PetStore::ManagerHomeExplicit_ptr _p)
  : _ptr(_p) {}

PetStore::ManagerHomeExplicit_var::ManagerHomeExplicit_var(const PetStore::ManagerHomeExplicit_var& _var)
  : _ptr(PetStore::ManagerHomeExplicit::_duplicate((PetStore::ManagerHomeExplicit_ptr) _var)) {}

PetStore::ManagerHomeExplicit_var::~ManagerHomeExplicit_var() {
  CORBA::release((CORBA::Object_ptr)_ptr);
}

PetStore::ManagerHomeExplicit_var&
PetStore::ManagerHomeExplicit_var::operator=(const PetStore::ManagerHomeExplicit_var& _var) {
    _release(_ptr);
    _ptr = _duplicate(_var._ptr);
    return *this;
}

PetStore::ManagerHomeExplicit_var&
PetStore::ManagerHomeExplicit_var::operator=(PetStore::ManagerHomeExplicit_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_ptr);
  _ptr = _p;
  return *this;
}

PetStore::ManagerHomeExplicit_ptr& PetStore::ManagerHomeExplicit_var::out() {
  _release(_ptr);
  _ptr = (PetStore::ManagerHomeExplicit_ptr)NULL;
  return _ptr;
}

PetStore::ManagerHomeExplicit_ptr PetStore::ManagerHomeExplicit_var::_retn() {
  PetStore::ManagerHomeExplicit_ptr _tmp_ptr = _ptr;
  _ptr = (PetStore::ManagerHomeExplicit_ptr)NULL;
  return _tmp_ptr;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::ManagerHomeExplicit_var& _var) {
  _strm >> _var._ptr;
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::ManagerHomeExplicit_var& _var) {
  _strm << _var._ptr;
  return _strm;
}

Istream& PetStore::_marshal_out(Istream& _strm, PetStore::ManagerHomeExplicit_var& _var) {
  VISistream _istrm(_strm);
  _istrm >> _var._ptr;
  return _strm;
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::ManagerHomeExplicit_var& _var) {
  _strm << (CORBA::Object_ptr)_var._ptr;
  return _strm;
}

// If dllimport is specified, you might probably want to
// disable these definitions
// disable the const definitions
const VISOps_Info PetStore::ManagerHomeExplicit_ops::_ops_info("PetStore::ManagerHomeExplicit_ops");
const VISOps_Info *PetStore::ManagerHomeExplicit_ops::_desc() { return &_ops_info; }
PetStore::ManagerHomeExplicit_ops_ptr PetStore::ManagerHomeExplicit_ops::_downcast(PortableServer::ServantBase* _servant) {
  if (_servant == (PortableServer::ServantBase*)NULL)
    return PetStore::ManagerHomeExplicit_ops::_nil();
  return (PetStore::ManagerHomeExplicit_ops_ptr)_servant->_safe_downcast_ops(_ops_info);
}
const CORBA::TypeInfo
#ifdef AIXV3
PetStore::ManagerHomeExplicit::_class_info("PetStore::ManagerHomeExplicit",
                                           "IDL:PetStore/ManagerHomeExplicit:1.0",
                                           NULL,
                                           PetStore::ManagerHomeExplicit::_factory,
                                           NULL, 0, NULL, 0,
                                           Components::CCMHome::_desc(), 0L);
#else
PetStore::ManagerHomeExplicit::_class_info("PetStore::ManagerHomeExplicit",
                                           "IDL:PetStore/ManagerHomeExplicit:1.0",
                                           NULL,
                                           PetStore::ManagerHomeExplicit::_factory,
                                           NULL, 0, NULL, 0,
                                           Components::CCMHome::_desc(), 0L);
#endif

const CORBA::TypeInfo *PetStore::ManagerHomeExplicit::_desc() { return &_class_info; }
const CORBA::TypeInfo *PetStore::ManagerHomeExplicit::_type_info() const {
  return &_class_info;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::ManagerHomeExplicit_ptr& _obj) {
  CORBA::Object_var _var_obj(_obj);
  _var_obj = CORBA::Object::_read(_strm, PetStore::ManagerHomeExplicit::_desc());
  _obj = PetStore::ManagerHomeExplicit::_narrow(_var_obj);
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::ManagerHomeExplicit_ptr _obj) {
  _strm << (CORBA::Object_ptr)_obj;
  return _strm;
}
void* PetStore::ManagerHomeExplicit::_safe_narrow(const CORBA::TypeInfo& _info) const {
  if (_info == _class_info)
    return (void *)this;
  if (_info == *Components::CCMHome::_desc())
    return (void *) (Components::CCMHome *) this;
  return CORBA_Object::_safe_narrow(_info);
}

CORBA::Object *PetStore::ManagerHomeExplicit::_factory() {
  return new PetStore::ManagerHomeExplicit;
}

PetStore::ManagerHomeExplicit_ptr PetStore::ManagerHomeExplicit::_this() {
  return PetStore::ManagerHomeExplicit::_duplicate(this);
}

PetStore::ManagerHomeExplicit_ptr PetStore::ManagerHomeExplicit::_narrow(CORBA::Object * _obj) {
  if (_obj == PetStore::ManagerHomeExplicit::_nil())
    return PetStore::ManagerHomeExplicit::_nil();
  else
    return PetStore::ManagerHomeExplicit::_duplicate((PetStore::ManagerHomeExplicit_ptr)_obj->_safe_narrow(_class_info));
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::ManagerHomeExplicit_ptr _obj) {
  _strm << (CORBA::Object_ptr) _obj;
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::ManagerHomeExplicit_ptr& _obj) {
  VISistream _istrm(_strm);
  _istrm >> _obj;
  return _strm;
}

PetStore::ManagerHomeExplicit *PetStore::ManagerHomeExplicit::_bind(const char *_object_name,
                                                                    const char *_host_name,
                                                                    const CORBA::BindOptions *_opt,
                                                                    CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
          "IDL:PetStore/ManagerHomeExplicit:1.0", _object_name, _host_name,
_opt, _orb);
  return ManagerHomeExplicit::_narrow(_obj);
}

PetStore::ManagerHomeExplicit *PetStore::ManagerHomeExplicit::_bind(const char *_poa_name,
                                                                    const CORBA::OctetSequence& _id,
                                                                    const char *_host_name,
                                                                    const CORBA::BindOptions *_opt,
                                                                    CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
   "IDL:PetStore/ManagerHomeExplicit:1.0", _poa_name, _id, _host_name, _opt,
   _orb);
  return ManagerHomeExplicit::_narrow(_obj);
}

PetStore::ManagerHomeImplicit_ptr PetStore::ManagerHomeImplicit_var::_duplicate(PetStore::ManagerHomeImplicit_ptr _p) {
  return PetStore::ManagerHomeImplicit::_duplicate(_p);
}

void
PetStore::ManagerHomeImplicit_var::_release(PetStore::ManagerHomeImplicit_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_p);
}

PetStore::ManagerHomeImplicit_var::ManagerHomeImplicit_var()
  : _ptr(PetStore::ManagerHomeImplicit::_nil()) {}

PetStore::ManagerHomeImplicit_var::ManagerHomeImplicit_var(PetStore::ManagerHomeImplicit_ptr _p)
  : _ptr(_p) {}

PetStore::ManagerHomeImplicit_var::ManagerHomeImplicit_var(const PetStore::ManagerHomeImplicit_var& _var)
  : _ptr(PetStore::ManagerHomeImplicit::_duplicate((PetStore::ManagerHomeImplicit_ptr) _var)) {}

PetStore::ManagerHomeImplicit_var::~ManagerHomeImplicit_var() {
  CORBA::release((CORBA::Object_ptr)_ptr);
}

PetStore::ManagerHomeImplicit_var&
PetStore::ManagerHomeImplicit_var::operator=(const PetStore::ManagerHomeImplicit_var& _var) {
    _release(_ptr);
    _ptr = _duplicate(_var._ptr);
    return *this;
}

PetStore::ManagerHomeImplicit_var&
PetStore::ManagerHomeImplicit_var::operator=(PetStore::ManagerHomeImplicit_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_ptr);
  _ptr = _p;
  return *this;
}

PetStore::ManagerHomeImplicit_ptr& PetStore::ManagerHomeImplicit_var::out() {
  _release(_ptr);
  _ptr = (PetStore::ManagerHomeImplicit_ptr)NULL;
  return _ptr;
}

PetStore::ManagerHomeImplicit_ptr PetStore::ManagerHomeImplicit_var::_retn() {
  PetStore::ManagerHomeImplicit_ptr _tmp_ptr = _ptr;
  _ptr = (PetStore::ManagerHomeImplicit_ptr)NULL;
  return _tmp_ptr;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::ManagerHomeImplicit_var& _var) {
  _strm >> _var._ptr;
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::ManagerHomeImplicit_var& _var) {
  _strm << _var._ptr;
  return _strm;
}

Istream& PetStore::_marshal_out(Istream& _strm, PetStore::ManagerHomeImplicit_var& _var) {
  VISistream _istrm(_strm);
  _istrm >> _var._ptr;
  return _strm;
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::ManagerHomeImplicit_var& _var) {
  _strm << (CORBA::Object_ptr)_var._ptr;
  return _strm;
}

// If dllimport is specified, you might probably want to
// disable these definitions
// disable the const definitions
const VISOps_Info PetStore::ManagerHomeImplicit_ops::_ops_info("PetStore::ManagerHomeImplicit_ops");
const VISOps_Info *PetStore::ManagerHomeImplicit_ops::_desc() { return &_ops_info; }
PetStore::ManagerHomeImplicit_ops_ptr PetStore::ManagerHomeImplicit_ops::_downcast(PortableServer::ServantBase* _servant) {
  if (_servant == (PortableServer::ServantBase*)NULL)
    return PetStore::ManagerHomeImplicit_ops::_nil();
  return (PetStore::ManagerHomeImplicit_ops_ptr)_servant->_safe_downcast_ops(_ops_info);
}
const CORBA::TypeInfo
#ifdef AIXV3
PetStore::ManagerHomeImplicit::_class_info("PetStore::ManagerHomeImplicit",
                                           "IDL:PetStore/ManagerHomeImplicit:1.0",
                                           NULL,
                                           PetStore::ManagerHomeImplicit::_factory,
                                           NULL, 0, NULL, 0,
                                           Components::KeylessCCMHome::_desc(),
                                           0L);
#else
PetStore::ManagerHomeImplicit::_class_info("PetStore::ManagerHomeImplicit",
                                           "IDL:PetStore/ManagerHomeImplicit:1.0",
                                           NULL,
                                           PetStore::ManagerHomeImplicit::_factory,
                                           NULL, 0, NULL, 0,
                                           Components::KeylessCCMHome::_desc(),
                                           0L);
#endif

const CORBA::TypeInfo *PetStore::ManagerHomeImplicit::_desc() { return &_class_info; }
const CORBA::TypeInfo *PetStore::ManagerHomeImplicit::_type_info() const {
  return &_class_info;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::ManagerHomeImplicit_ptr& _obj) {
  CORBA::Object_var _var_obj(_obj);
  _var_obj = CORBA::Object::_read(_strm, PetStore::ManagerHomeImplicit::_desc());
  _obj = PetStore::ManagerHomeImplicit::_narrow(_var_obj);
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::ManagerHomeImplicit_ptr _obj) {
  _strm << (CORBA::Object_ptr)_obj;
  return _strm;
}
void* PetStore::ManagerHomeImplicit::_safe_narrow(const CORBA::TypeInfo& _info) const {
  if (_info == _class_info)
    return (void *)this;
  if (_info == *Components::KeylessCCMHome::_desc())
    return (void *) (Components::KeylessCCMHome *) this;
  return CORBA_Object::_safe_narrow(_info);
}

CORBA::Object *PetStore::ManagerHomeImplicit::_factory() {
  return new PetStore::ManagerHomeImplicit;
}

PetStore::ManagerHomeImplicit_ptr PetStore::ManagerHomeImplicit::_this() {
  return PetStore::ManagerHomeImplicit::_duplicate(this);
}

PetStore::ManagerHomeImplicit_ptr PetStore::ManagerHomeImplicit::_narrow(CORBA::Object * _obj) {
  if (_obj == PetStore::ManagerHomeImplicit::_nil())
    return PetStore::ManagerHomeImplicit::_nil();
  else
    return PetStore::ManagerHomeImplicit::_duplicate((PetStore::ManagerHomeImplicit_ptr)_obj->_safe_narrow(_class_info));
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::ManagerHomeImplicit_ptr _obj) {
  _strm << (CORBA::Object_ptr) _obj;
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::ManagerHomeImplicit_ptr& _obj) {
  VISistream _istrm(_strm);
  _istrm >> _obj;
  return _strm;
}

PetStore::ManagerHomeImplicit *PetStore::ManagerHomeImplicit::_bind(const char *_object_name,
                                                                    const char *_host_name,
                                                                    const CORBA::BindOptions *_opt,
                                                                    CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
          "IDL:PetStore/ManagerHomeImplicit:1.0", _object_name, _host_name,
_opt, _orb);
  return ManagerHomeImplicit::_narrow(_obj);
}

PetStore::ManagerHomeImplicit *PetStore::ManagerHomeImplicit::_bind(const char *_poa_name,
                                                                    const CORBA::OctetSequence& _id,
                                                                    const char *_host_name,
                                                                    const CORBA::BindOptions *_opt,
                                                                    CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
   "IDL:PetStore/ManagerHomeImplicit:1.0", _poa_name, _id, _host_name, _opt,
   _orb);
  return ManagerHomeImplicit::_narrow(_obj);
}
#if defined(__BORLANDC__)
#pragma option push
#pragma warn -8057
#pragma warn -8004
#endif
::PetStore::Manager_ptr PetStore::ManagerHomeImplicit::create() {
  ::PetStore::Manager_ptr _ret = (::PetStore::Manager_ptr) NULL;
  CORBA_MarshalInBuffer_var _ibuf;

  while (1) {
    VISCLEAR_EXCEP 

    if (_is_local()) {
      PortableServer::ServantBase_ptr _servant;
      VISTRY {
        _servant = _preinvoke("create");
      }
      VISCATCH(VISRemarshal, _vis_except) { continue; } 
      VISEND_CATCH

      PetStore::ManagerHomeImplicit_ops*  _managerhomeimplicit = PetStore::ManagerHomeImplicit_ops::_downcast(_servant);
      if (!_managerhomeimplicit) {
        if ((PortableServer::ServantBase*)_servant)
          _postinvoke(_servant, "create");
        VISTHROW(CORBA::BAD_OPERATION());
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }

      VISTRY {
         _ret = _managerhomeimplicit->create();
      }
      VISCATCH_ALL{
        _postinvoke(_servant, "create"); 
        VISTHROW_LAST;
#if defined(_VIS_NOEXCEPTIONS)
        VISRETURN(return _ret;)
#endif
      }
      VISEND_CATCH
      _postinvoke(_servant, "create");
      return _ret;
    }

    CORBA::MarshalOutBuffer_var _obuf;
    _obuf = _request("create", 1);
    VISIF_EXCEP(return _ret;)

    VISTRY {
      _ibuf = _invoke(_obuf);
      VISIFNOT_EXCEP
      break;
      VISEND_IFNOT_EXCEP
    } VISCATCH(VISRemarshal, _vis_except) { continue; } 
    VISEND_CATCH
#if defined(_VIS_NOEXCEPTIONS)
    VISRETURN(return _ret;)
#endif
  }

  VISistream& _istrm = *VISistream::_downcast(_ibuf);
  _istrm >> _ret;
  return _ret;
}

#if defined(__BORLANDC__)
 #pragma option pop
#endif

PetStore::ManagerHome_ptr PetStore::ManagerHome_var::_duplicate(PetStore::ManagerHome_ptr _p) {
  return PetStore::ManagerHome::_duplicate(_p);
}

void
PetStore::ManagerHome_var::_release(PetStore::ManagerHome_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_p);
}

PetStore::ManagerHome_var::ManagerHome_var()
  : _ptr(PetStore::ManagerHome::_nil()) {}

PetStore::ManagerHome_var::ManagerHome_var(PetStore::ManagerHome_ptr _p)
  : _ptr(_p) {}

PetStore::ManagerHome_var::ManagerHome_var(const PetStore::ManagerHome_var& _var)
  : _ptr(PetStore::ManagerHome::_duplicate((PetStore::ManagerHome_ptr) _var)) {}

PetStore::ManagerHome_var::~ManagerHome_var() {
  CORBA::release((CORBA::Object_ptr)_ptr);
}

PetStore::ManagerHome_var&
PetStore::ManagerHome_var::operator=(const PetStore::ManagerHome_var& _var) {
    _release(_ptr);
    _ptr = _duplicate(_var._ptr);
    return *this;
}

PetStore::ManagerHome_var&
PetStore::ManagerHome_var::operator=(PetStore::ManagerHome_ptr _p) {
  CORBA::release((CORBA::Object_ptr)_ptr);
  _ptr = _p;
  return *this;
}

PetStore::ManagerHome_ptr& PetStore::ManagerHome_var::out() {
  _release(_ptr);
  _ptr = (PetStore::ManagerHome_ptr)NULL;
  return _ptr;
}

PetStore::ManagerHome_ptr PetStore::ManagerHome_var::_retn() {
  PetStore::ManagerHome_ptr _tmp_ptr = _ptr;
  _ptr = (PetStore::ManagerHome_ptr)NULL;
  return _tmp_ptr;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::ManagerHome_var& _var) {
  _strm >> _var._ptr;
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::ManagerHome_var& _var) {
  _strm << _var._ptr;
  return _strm;
}

Istream& PetStore::_marshal_out(Istream& _strm, PetStore::ManagerHome_var& _var) {
  VISistream _istrm(_strm);
  _istrm >> _var._ptr;
  return _strm;
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::ManagerHome_var& _var) {
  _strm << (CORBA::Object_ptr)_var._ptr;
  return _strm;
}

// If dllimport is specified, you might probably want to
// disable these definitions
// disable the const definitions
const VISOps_Info PetStore::ManagerHome_ops::_ops_info("PetStore::ManagerHome_ops");
const VISOps_Info *PetStore::ManagerHome_ops::_desc() { return &_ops_info; }
PetStore::ManagerHome_ops_ptr PetStore::ManagerHome_ops::_downcast(PortableServer::ServantBase* _servant) {
  if (_servant == (PortableServer::ServantBase*)NULL)
    return PetStore::ManagerHome_ops::_nil();
  return (PetStore::ManagerHome_ops_ptr)_servant->_safe_downcast_ops(_ops_info);
}
const CORBA::TypeInfo
#ifdef AIXV3
PetStore::ManagerHome::_class_info("PetStore::ManagerHome",
                                   "IDL:PetStore/ManagerHome:1.0", NULL,
                                   PetStore::ManagerHome::_factory, NULL, 0,
                                   NULL, 0,
                                   PetStore::ManagerHomeExplicit::_desc(),
                                   PetStore::ManagerHomeImplicit::_desc(), 0L);
#else
PetStore::ManagerHome::_class_info("PetStore::ManagerHome",
                                   "IDL:PetStore/ManagerHome:1.0", NULL,
                                   PetStore::ManagerHome::_factory, NULL, 0,
                                   NULL, 0,
                                   PetStore::ManagerHomeExplicit::_desc(),
                                   PetStore::ManagerHomeImplicit::_desc(), 0L);
#endif

const CORBA::TypeInfo *PetStore::ManagerHome::_desc() { return &_class_info; }
const CORBA::TypeInfo *PetStore::ManagerHome::_type_info() const {
  return &_class_info;
}

VISistream& PetStore::_marshal_out(VISistream& _strm, PetStore::ManagerHome_ptr& _obj) {
  CORBA::Object_var _var_obj(_obj);
  _var_obj = CORBA::Object::_read(_strm, PetStore::ManagerHome::_desc());
  _obj = PetStore::ManagerHome::_narrow(_var_obj);
  return _strm;
}

VISostream& PetStore::_marshal_in(VISostream& _strm, const PetStore::ManagerHome_ptr _obj) {
  _strm << (CORBA::Object_ptr)_obj;
  return _strm;
}
void* PetStore::ManagerHome::_safe_narrow(const CORBA::TypeInfo& _info) const {
  if (_info == _class_info)
    return (void *)this;
  if (_info == *PetStore::ManagerHomeExplicit::_desc())
    return (void *) (PetStore::ManagerHomeExplicit *) this;
  if (_info == *Components::CCMHome::_desc())
    return (void *) (Components::CCMHome *) this;
  if (_info == *PetStore::ManagerHomeImplicit::_desc())
    return (void *) (PetStore::ManagerHomeImplicit *) this;
  if (_info == *Components::KeylessCCMHome::_desc())
    return (void *) (Components::KeylessCCMHome *) this;
  return CORBA_Object::_safe_narrow(_info);
}

CORBA::Object *PetStore::ManagerHome::_factory() {
  return new PetStore::ManagerHome;
}

PetStore::ManagerHome_ptr PetStore::ManagerHome::_this() {
  return PetStore::ManagerHome::_duplicate(this);
}

PetStore::ManagerHome_ptr PetStore::ManagerHome::_narrow(CORBA::Object * _obj) {
  if (_obj == PetStore::ManagerHome::_nil())
    return PetStore::ManagerHome::_nil();
  else
    return PetStore::ManagerHome::_duplicate((PetStore::ManagerHome_ptr)_obj->_safe_narrow(_class_info));
}

Ostream& PetStore::_marshal_in(Ostream& _strm, const PetStore::ManagerHome_ptr _obj) {
  _strm << (CORBA::Object_ptr) _obj;
  return _strm;
}
Istream& PetStore::_marshal_out(Istream& _strm, PetStore::ManagerHome_ptr& _obj) {
  VISistream _istrm(_strm);
  _istrm >> _obj;
  return _strm;
}

PetStore::ManagerHome *PetStore::ManagerHome::_bind(const char *_object_name,
                                                    const char *_host_name,
                                                    const CORBA::BindOptions *_opt,
                                                    CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
          "IDL:PetStore/ManagerHome:1.0", _object_name, _host_name, _opt, _orb);
  return ManagerHome::_narrow(_obj);
}

PetStore::ManagerHome *PetStore::ManagerHome::_bind(const char *_poa_name,
                                                    const CORBA::OctetSequence& _id,
                                                    const char *_host_name,
                                                    const CORBA::BindOptions *_opt,
                                                    CORBA::ORB_ptr _orb)
{
  VISCLEAR_EXCEP
  CORBA::Object_var _obj = CORBA::Object::_bind_to_object(
   "IDL:PetStore/ManagerHome:1.0", _poa_name, _id, _host_name, _opt, _orb);
  return ManagerHome::_narrow(_obj);
}

