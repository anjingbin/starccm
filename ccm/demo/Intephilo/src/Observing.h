// *************************************************************************
//
// This File Is Automatically Generated by the StarBusIDL-to-C++ Compiler !
//
// Copyright (c) 2003
// Network Information Security Institute,Computer College,NUDT.
// ChangSha,Hunan,PRC
//
// All Rights Reserved
//
// *************************************************************************

// Version: 5.0.0

#ifndef ___Observing_h__
#define ___Observing_h__

#include <STAR/StringValue.h>

namespace javax
{

namespace ejb
{

class EJBHome;
typedef EJBHome* EJBHome_ptr;
typedef EJBHome* EJBHomeRef;

extern STAR::TypeCodeConst _tc_EJBHome;

class EJBObject;
typedef EJBObject* EJBObject_ptr;
typedef EJBObject* EJBObjectRef;

extern STAR::TypeCodeConst _tc_EJBObject;

} // End of namespace ejb

} // End of namespace javax

namespace Dinner
{

class Observing;
typedef Observing* Observing_ptr;
typedef Observing* ObservingRef;

extern STAR::TypeCodeConst _tc_Observing;

class ObservingHome;
typedef ObservingHome* ObservingHome_ptr;
typedef ObservingHome* ObservingHomeRef;

extern STAR::TypeCodeConst _tc_ObservingHome;

} // End of namespace Dinner

void STARDuplicate(javax::ejb::EJBHome_ptr);
void STARRelease(javax::ejb::EJBHome_ptr);

void STARMarshal(javax::ejb::EJBHome_ptr, STAR::OutputStreamImpl*);
void STARUnmarshal(javax::ejb::EJBHome_ptr&, STAR::InputStreamImpl*);

void STARDuplicate(javax::ejb::EJBObject_ptr);
void STARRelease(javax::ejb::EJBObject_ptr);

void STARMarshal(javax::ejb::EJBObject_ptr, STAR::OutputStreamImpl*);
void STARUnmarshal(javax::ejb::EJBObject_ptr&, STAR::InputStreamImpl*);

void STARDuplicate(Dinner::Observing_ptr);
void STARRelease(Dinner::Observing_ptr);

void STARMarshal(Dinner::Observing_ptr, STAR::OutputStreamImpl*);
void STARUnmarshal(Dinner::Observing_ptr&, STAR::InputStreamImpl*);

void STARDuplicate(Dinner::ObservingHome_ptr);
void STARRelease(Dinner::ObservingHome_ptr);

void STARMarshal(Dinner::ObservingHome_ptr, STAR::OutputStreamImpl*);
void STARUnmarshal(Dinner::ObservingHome_ptr&, STAR::InputStreamImpl*);

namespace javax
{

namespace ejb
{

typedef STAR::ObjVar< EJBHome > EJBHome_var;
typedef STAR::ObjOut< EJBHome > EJBHome_out;

typedef STAR::ObjVar< EJBObject > EJBObject_var;
typedef STAR::ObjOut< EJBObject > EJBObject_out;

} // End of namespace ejb

} // End of namespace javax

namespace Dinner
{

typedef STAR::ObjVar< Observing > Observing_var;
typedef STAR::ObjOut< Observing > Observing_out;

typedef STAR::ObjVar< ObservingHome > ObservingHome_var;
typedef STAR::ObjOut< ObservingHome > ObservingHome_out;

} // End of namespace Dinner

//
// Module declare ::javax
//
namespace javax
{

//
// Module declare ::javax::ejb
//
namespace ejb
{

//
// Interface declare ::javax::ejb::EJBHome
//
class EJBHome : virtual public CORBA::Object
{
    EJBHome(const EJBHome&);
    void operator=(const EJBHome&);

protected:

    static const char* ids_[];

public:

    EJBHome() { }
    virtual ~EJBHome() { }

    typedef EJBHome_ptr _ptr_type;
    typedef EJBHome_var _var_type;

    static inline EJBHome_ptr
    _duplicate(EJBHome_ptr p)
    {
        if(p)
            p -> _STAR_incRef();
        return p;
    }

    static inline EJBHome_ptr
    _nil()
    {
        return 0;
    }

    static EJBHome_ptr _narrow(CORBA::Object_ptr);
    static EJBHome_ptr _narrow(CORBA::AbstractBase_ptr);

    static EJBHome_ptr _unchecked_narrow(CORBA::AbstractBase_ptr);
    static EJBHome_ptr _unchecked_narrow(CORBA::Object_ptr);

    static const char** _STAR_staticIds();
};


class EJBHome_broker : virtual public javax::ejb::EJBHome,
                       virtual public STARCORBA::Object
{
    EJBHome_broker(const EJBHome_broker&);
    void operator=(const EJBHome_broker&);

public:

    EJBHome_broker() { }
    virtual ~EJBHome_broker() { }

    virtual const char** _STAR_ids() const;
};

//
// Interface declare ::javax::ejb::EJBObject
//
class EJBObject : virtual public CORBA::Object
{
    EJBObject(const EJBObject&);
    void operator=(const EJBObject&);

protected:

    static const char* ids_[];

public:

    EJBObject() { }
    virtual ~EJBObject() { }

    typedef EJBObject_ptr _ptr_type;
    typedef EJBObject_var _var_type;

    static inline EJBObject_ptr
    _duplicate(EJBObject_ptr p)
    {
        if(p)
            p -> _STAR_incRef();
        return p;
    }

    static inline EJBObject_ptr
    _nil()
    {
        return 0;
    }

    static EJBObject_ptr _narrow(CORBA::Object_ptr);
    static EJBObject_ptr _narrow(CORBA::AbstractBase_ptr);

    static EJBObject_ptr _unchecked_narrow(CORBA::AbstractBase_ptr);
    static EJBObject_ptr _unchecked_narrow(CORBA::Object_ptr);

    static const char** _STAR_staticIds();

    //
    // Attribute Get declare ::javax::ejb::EJBObject::EJBHome
    //
    virtual EJBHome_ptr EJBHome() = 0;

    //
    // Operation declare ::javax::ejb::EJBObject::remove
    //
    virtual void remove() = 0;

    //
    // Operation declare ::javax::ejb::EJBObject::isIdentical
    //
    virtual CORBA::Boolean isIdentical(::javax::ejb::EJBObject_ptr arg0) = 0;
};


class EJBObject_broker : virtual public javax::ejb::EJBObject,
                         virtual public STARCORBA::Object
{
    EJBObject_broker(const EJBObject_broker&);
    void operator=(const EJBObject_broker&);

public:

    EJBObject_broker() { }
    virtual ~EJBObject_broker() { }

    virtual const char** _STAR_ids() const;

    //
    // Attribute Get declare ::javax::ejb::EJBObject_broker::EJBHome
    //
    ::javax::ejb::EJBHome_ptr EJBHome();

    //
    // Operation declare ::javax::ejb::EJBObject_broker::remove
    //
    void remove();

    //
    // Operation declare ::javax::ejb::EJBObject_broker::isIdentical
    //
    CORBA::Boolean isIdentical(::javax::ejb::EJBObject_ptr arg0);
};

} // End of namespace ejb

} // End of namespace javax

//
// Module declare ::Dinner
//
namespace Dinner
{

//
// Interface declare ::Dinner::Observing
//
class Observing : virtual public javax::ejb::EJBObject
{
    Observing(const Observing&);
    void operator=(const Observing&);

protected:

    static const char* ids_[];

public:

    Observing() { }
    virtual ~Observing() { }

    typedef Observing_ptr _ptr_type;
    typedef Observing_var _var_type;

    static inline Observing_ptr
    _duplicate(Observing_ptr p)
    {
        if(p)
            p -> _STAR_incRef();
        return p;
    }

    static inline Observing_ptr
    _nil()
    {
        return 0;
    }

    static Observing_ptr _narrow(CORBA::Object_ptr);
    static Observing_ptr _narrow(CORBA::AbstractBase_ptr);

    static Observing_ptr _unchecked_narrow(CORBA::AbstractBase_ptr);
    static Observing_ptr _unchecked_narrow(CORBA::Object_ptr);

    static const char** _STAR_staticIds();

    //
    // Operation declare ::Dinner::Observing::change_status
    //
    virtual void change_status(CORBA::Long arg0,
                               ::CORBA::WStringValue* arg1) = 0;
};


class Observing_broker : virtual public Dinner::Observing,
                         virtual public javax::ejb::EJBObject_broker
{
    Observing_broker(const Observing_broker&);
    void operator=(const Observing_broker&);

public:

    Observing_broker() { }
    virtual ~Observing_broker() { }

    virtual const char** _STAR_ids() const;

    //
    // Operation declare ::Dinner::Observing_broker::change_status
    //
    void change_status(CORBA::Long arg0,
                       ::CORBA::WStringValue* arg1);
};

//
// Interface declare ::Dinner::ObservingHome
//
class ObservingHome : virtual public javax::ejb::EJBHome
{
    ObservingHome(const ObservingHome&);
    void operator=(const ObservingHome&);

protected:

    static const char* ids_[];

public:

    ObservingHome() { }
    virtual ~ObservingHome() { }

    typedef ObservingHome_ptr _ptr_type;
    typedef ObservingHome_var _var_type;

    static inline ObservingHome_ptr
    _duplicate(ObservingHome_ptr p)
    {
        if(p)
            p -> _STAR_incRef();
        return p;
    }

    static inline ObservingHome_ptr
    _nil()
    {
        return 0;
    }

    static ObservingHome_ptr _narrow(CORBA::Object_ptr);
    static ObservingHome_ptr _narrow(CORBA::AbstractBase_ptr);

    static ObservingHome_ptr _unchecked_narrow(CORBA::AbstractBase_ptr);
    static ObservingHome_ptr _unchecked_narrow(CORBA::Object_ptr);

    static const char** _STAR_staticIds();

    //
    // Operation declare ::Dinner::ObservingHome::create
    //
    virtual ::Dinner::Observing_ptr create() = 0;
};


class ObservingHome_broker : virtual public Dinner::ObservingHome,
                             virtual public javax::ejb::EJBHome_broker
{
    ObservingHome_broker(const ObservingHome_broker&);
    void operator=(const ObservingHome_broker&);

public:

    ObservingHome_broker() { }
    virtual ~ObservingHome_broker() { }

    virtual const char** _STAR_ids() const;

    //
    // Operation declare ::Dinner::ObservingHome_broker::create
    //
    ::Dinner::Observing_ptr create();
};

} // End of namespace Dinner

//
// Module declare ::javax
//
namespace OBV_javax
{

//
// Module declare ::javax::ejb
//
namespace ejb
{

} // End of namespace ejb

} // End of namespace OBV_javax

//
// Module declare ::Dinner
//
namespace OBV_Dinner
{

} // End of namespace OBV_Dinner

//
// Interface declare ::javax::ejb::EJBHome
//
namespace CORBA
{

inline void
release(javax::ejb::EJBHome_ptr p)
{
    if(p)
        p -> _STAR_decRef();
}

inline Boolean
is_nil(javax::ejb::EJBHome_ptr p)
{
    return p == 0;
}

} // End of namespace CORBA

void operator<<=(CORBA::Any&, javax::ejb::EJBHome_ptr*);
void operator<<=(CORBA::Any&, javax::ejb::EJBHome_ptr);
CORBA::Boolean operator>>=(const CORBA::Any&, javax::ejb::EJBHome_ptr&);

inline void
operator<<=(CORBA::Any_var& any, javax::ejb::EJBHome_ptr* val)
{
    any.inout() <<= val;
}

inline void
operator<<=(CORBA::Any_var& any, javax::ejb::EJBHome_ptr val)
{
    any.inout() <<= val;
}

inline CORBA::Boolean
operator>>=(const CORBA::Any_var& any, javax::ejb::EJBHome_ptr& val)
{
    return any.in() >>= val;
}

//
// Interface declare ::javax::ejb::EJBObject
//
namespace CORBA
{

inline void
release(javax::ejb::EJBObject_ptr p)
{
    if(p)
        p -> _STAR_decRef();
}

inline Boolean
is_nil(javax::ejb::EJBObject_ptr p)
{
    return p == 0;
}

} // End of namespace CORBA

void operator<<=(CORBA::Any&, javax::ejb::EJBObject_ptr*);
void operator<<=(CORBA::Any&, javax::ejb::EJBObject_ptr);
CORBA::Boolean operator>>=(const CORBA::Any&, javax::ejb::EJBObject_ptr&);

inline void
operator<<=(CORBA::Any_var& any, javax::ejb::EJBObject_ptr* val)
{
    any.inout() <<= val;
}

inline void
operator<<=(CORBA::Any_var& any, javax::ejb::EJBObject_ptr val)
{
    any.inout() <<= val;
}

inline CORBA::Boolean
operator>>=(const CORBA::Any_var& any, javax::ejb::EJBObject_ptr& val)
{
    return any.in() >>= val;
}

//
// Interface declare ::Dinner::Observing
//
namespace CORBA
{

inline void
release(Dinner::Observing_ptr p)
{
    if(p)
        p -> _STAR_decRef();
}

inline Boolean
is_nil(Dinner::Observing_ptr p)
{
    return p == 0;
}

} // End of namespace CORBA

void operator<<=(CORBA::Any&, Dinner::Observing_ptr*);
void operator<<=(CORBA::Any&, Dinner::Observing_ptr);
CORBA::Boolean operator>>=(const CORBA::Any&, Dinner::Observing_ptr&);

inline void
operator<<=(CORBA::Any_var& any, Dinner::Observing_ptr* val)
{
    any.inout() <<= val;
}

inline void
operator<<=(CORBA::Any_var& any, Dinner::Observing_ptr val)
{
    any.inout() <<= val;
}

inline CORBA::Boolean
operator>>=(const CORBA::Any_var& any, Dinner::Observing_ptr& val)
{
    return any.in() >>= val;
}

//
// Interface declare ::Dinner::ObservingHome
//
namespace CORBA
{

inline void
release(Dinner::ObservingHome_ptr p)
{
    if(p)
        p -> _STAR_decRef();
}

inline Boolean
is_nil(Dinner::ObservingHome_ptr p)
{
    return p == 0;
}

} // End of namespace CORBA

void operator<<=(CORBA::Any&, Dinner::ObservingHome_ptr*);
void operator<<=(CORBA::Any&, Dinner::ObservingHome_ptr);
CORBA::Boolean operator>>=(const CORBA::Any&, Dinner::ObservingHome_ptr&);

inline void
operator<<=(CORBA::Any_var& any, Dinner::ObservingHome_ptr* val)
{
    any.inout() <<= val;
}

inline void
operator<<=(CORBA::Any_var& any, Dinner::ObservingHome_ptr val)
{
    any.inout() <<= val;
}

inline CORBA::Boolean
operator>>=(const CORBA::Any_var& any, Dinner::ObservingHome_ptr& val)
{
    return any.in() >>= val;
}

#endif
