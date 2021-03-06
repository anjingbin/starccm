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

#ifndef ___Dinner_h__
#define ___Dinner_h__

#include <Components.h>
#include <Observing.h>

namespace Dinner
{

class Fork;
typedef Fork* Fork_ptr;
typedef Fork* ForkRef;

extern STAR::TypeCodeConst _tc_Fork;

class Named;
typedef Named* Named_ptr;
typedef Named* NamedRef;

extern STAR::TypeCodeConst _tc_Named;

class Philosopher;
typedef Philosopher* Philosopher_ptr;
typedef Philosopher* PhilosopherRef;

extern STAR::TypeCodeConst _tc_Philosopher;

class Cutlery;
typedef Cutlery* Cutlery_ptr;
typedef Cutlery* CutleryRef;

extern STAR::TypeCodeConst _tc_Cutlery;

class PhilosopherHomeExplicit;
typedef PhilosopherHomeExplicit* PhilosopherHomeExplicit_ptr;
typedef PhilosopherHomeExplicit* PhilosopherHomeExplicitRef;

extern STAR::TypeCodeConst _tc_PhilosopherHomeExplicit;

class PhilosopherHomeImplicit;
typedef PhilosopherHomeImplicit* PhilosopherHomeImplicit_ptr;
typedef PhilosopherHomeImplicit* PhilosopherHomeImplicitRef;

extern STAR::TypeCodeConst _tc_PhilosopherHomeImplicit;

class PhilosopherHome;
typedef PhilosopherHome* PhilosopherHome_ptr;
typedef PhilosopherHome* PhilosopherHomeRef;

extern STAR::TypeCodeConst _tc_PhilosopherHome;

class CutleryHomeExplicit;
typedef CutleryHomeExplicit* CutleryHomeExplicit_ptr;
typedef CutleryHomeExplicit* CutleryHomeExplicitRef;

extern STAR::TypeCodeConst _tc_CutleryHomeExplicit;

class CutleryHomeImplicit;
typedef CutleryHomeImplicit* CutleryHomeImplicit_ptr;
typedef CutleryHomeImplicit* CutleryHomeImplicitRef;

extern STAR::TypeCodeConst _tc_CutleryHomeImplicit;

class CutleryHome;
typedef CutleryHome* CutleryHome_ptr;
typedef CutleryHome* CutleryHomeRef;

extern STAR::TypeCodeConst _tc_CutleryHome;

} // End of namespace Dinner

void STARDuplicate(Dinner::Fork_ptr);
void STARRelease(Dinner::Fork_ptr);

void STARMarshal(Dinner::Fork_ptr, STAR::OutputStreamImpl*);
void STARUnmarshal(Dinner::Fork_ptr&, STAR::InputStreamImpl*);

void STARDuplicate(Dinner::Named_ptr);
void STARRelease(Dinner::Named_ptr);

void STARMarshal(Dinner::Named_ptr, STAR::OutputStreamImpl*);
void STARUnmarshal(Dinner::Named_ptr&, STAR::InputStreamImpl*);

void STARDuplicate(Dinner::Philosopher_ptr);
void STARRelease(Dinner::Philosopher_ptr);

void STARMarshal(Dinner::Philosopher_ptr, STAR::OutputStreamImpl*);
void STARUnmarshal(Dinner::Philosopher_ptr&, STAR::InputStreamImpl*);

void STARDuplicate(Dinner::Cutlery_ptr);
void STARRelease(Dinner::Cutlery_ptr);

void STARMarshal(Dinner::Cutlery_ptr, STAR::OutputStreamImpl*);
void STARUnmarshal(Dinner::Cutlery_ptr&, STAR::InputStreamImpl*);

void STARDuplicate(Dinner::PhilosopherHomeExplicit_ptr);
void STARRelease(Dinner::PhilosopherHomeExplicit_ptr);

void STARMarshal(Dinner::PhilosopherHomeExplicit_ptr, STAR::OutputStreamImpl*);
void STARUnmarshal(Dinner::PhilosopherHomeExplicit_ptr&, STAR::InputStreamImpl*);

void STARDuplicate(Dinner::PhilosopherHomeImplicit_ptr);
void STARRelease(Dinner::PhilosopherHomeImplicit_ptr);

void STARMarshal(Dinner::PhilosopherHomeImplicit_ptr, STAR::OutputStreamImpl*);
void STARUnmarshal(Dinner::PhilosopherHomeImplicit_ptr&, STAR::InputStreamImpl*);

void STARDuplicate(Dinner::PhilosopherHome_ptr);
void STARRelease(Dinner::PhilosopherHome_ptr);

void STARMarshal(Dinner::PhilosopherHome_ptr, STAR::OutputStreamImpl*);
void STARUnmarshal(Dinner::PhilosopherHome_ptr&, STAR::InputStreamImpl*);

void STARDuplicate(Dinner::CutleryHomeExplicit_ptr);
void STARRelease(Dinner::CutleryHomeExplicit_ptr);

void STARMarshal(Dinner::CutleryHomeExplicit_ptr, STAR::OutputStreamImpl*);
void STARUnmarshal(Dinner::CutleryHomeExplicit_ptr&, STAR::InputStreamImpl*);

void STARDuplicate(Dinner::CutleryHomeImplicit_ptr);
void STARRelease(Dinner::CutleryHomeImplicit_ptr);

void STARMarshal(Dinner::CutleryHomeImplicit_ptr, STAR::OutputStreamImpl*);
void STARUnmarshal(Dinner::CutleryHomeImplicit_ptr&, STAR::InputStreamImpl*);

void STARDuplicate(Dinner::CutleryHome_ptr);
void STARRelease(Dinner::CutleryHome_ptr);

void STARMarshal(Dinner::CutleryHome_ptr, STAR::OutputStreamImpl*);
void STARUnmarshal(Dinner::CutleryHome_ptr&, STAR::InputStreamImpl*);

namespace Dinner
{

typedef STAR::ObjVar< Fork > Fork_var;
typedef STAR::ObjOut< Fork > Fork_out;

typedef STAR::ObjVar< Named > Named_var;
typedef STAR::ObjOut< Named > Named_out;

typedef STAR::ObjVar< Philosopher > Philosopher_var;
typedef STAR::ObjOut< Philosopher > Philosopher_out;

typedef STAR::ObjVar< Cutlery > Cutlery_var;
typedef STAR::ObjOut< Cutlery > Cutlery_out;

typedef STAR::ObjVar< PhilosopherHomeExplicit > PhilosopherHomeExplicit_var;
typedef STAR::ObjOut< PhilosopherHomeExplicit > PhilosopherHomeExplicit_out;

typedef STAR::ObjVar< PhilosopherHomeImplicit > PhilosopherHomeImplicit_var;
typedef STAR::ObjOut< PhilosopherHomeImplicit > PhilosopherHomeImplicit_out;

typedef STAR::ObjVar< PhilosopherHome > PhilosopherHome_var;
typedef STAR::ObjOut< PhilosopherHome > PhilosopherHome_out;

typedef STAR::ObjVar< CutleryHomeExplicit > CutleryHomeExplicit_var;
typedef STAR::ObjOut< CutleryHomeExplicit > CutleryHomeExplicit_out;

typedef STAR::ObjVar< CutleryHomeImplicit > CutleryHomeImplicit_var;
typedef STAR::ObjOut< CutleryHomeImplicit > CutleryHomeImplicit_out;

typedef STAR::ObjVar< CutleryHome > CutleryHome_var;
typedef STAR::ObjOut< CutleryHome > CutleryHome_out;

} // End of namespace Dinner

//
// Module declare ::Dinner
//
namespace Dinner
{

//
// Exception declare ::Dinner::ForkNotAvailable
//
struct ForkNotAvailable : public CORBA::UserException
{
    ForkNotAvailable() { }
    ForkNotAvailable(const ForkNotAvailable&);
    ForkNotAvailable& operator=(const ForkNotAvailable&);

    static ForkNotAvailable* _downcast(CORBA::Exception*);
    static const ForkNotAvailable* _downcast(const CORBA::Exception*);
    virtual const char* _name() const;
    virtual void _raise() const { throw *this; }
    virtual const char* _rep_id() const;
    virtual char* _to_string() const;

    virtual CORBA::Exception* _STAR_clone() const;
    virtual void _STAR_insertInAny(CORBA::Any&);

    static void _STAR_unmarshal(ForkNotAvailable&, STAR::InputStreamImpl*);
};

extern STAR::TypeCodeConst _tc_ForkNotAvailable;

//
// Exception declare ::Dinner::NotTheEater
//
struct NotTheEater : public CORBA::UserException
{
    NotTheEater() { }
    NotTheEater(const NotTheEater&);
    NotTheEater& operator=(const NotTheEater&);

    static NotTheEater* _downcast(CORBA::Exception*);
    static const NotTheEater* _downcast(const CORBA::Exception*);
    virtual const char* _name() const;
    virtual void _raise() const { throw *this; }
    virtual const char* _rep_id() const;
    virtual char* _to_string() const;

    virtual CORBA::Exception* _STAR_clone() const;
    virtual void _STAR_insertInAny(CORBA::Any&);

    static void _STAR_unmarshal(NotTheEater&, STAR::InputStreamImpl*);
};

extern STAR::TypeCodeConst _tc_NotTheEater;

//
// Interface declare ::Dinner::Fork
//
class Fork : virtual public CORBA::Object
{
    Fork(const Fork&);
    void operator=(const Fork&);

protected:

    static const char* ids_[];

public:

    Fork() { }
    virtual ~Fork() { }

    typedef Fork_ptr _ptr_type;
    typedef Fork_var _var_type;

    static inline Fork_ptr
    _duplicate(Fork_ptr p)
    {
        if(p)
            p -> _STAR_incRef();
        return p;
    }

    static inline Fork_ptr
    _nil()
    {
        return 0;
    }

    static Fork_ptr _narrow(CORBA::Object_ptr);
    static Fork_ptr _narrow(CORBA::AbstractBase_ptr);

    static Fork_ptr _unchecked_narrow(CORBA::AbstractBase_ptr);
    static Fork_ptr _unchecked_narrow(CORBA::Object_ptr);

    static const char** _STAR_staticIds();

    //
    // Operation declare ::Dinner::Fork::obtain_fork
    //
    virtual ::Components::Cookie* obtain_fork() = 0;

    //
    // Operation declare ::Dinner::Fork::release_fork
    //
    virtual void release_fork(::Components::Cookie* ck) = 0;
};


class Fork_broker : virtual public Dinner::Fork,
                    virtual public STARCORBA::Object
{
    Fork_broker(const Fork_broker&);
    void operator=(const Fork_broker&);

public:

    Fork_broker() { }
    virtual ~Fork_broker() { }

    virtual const char** _STAR_ids() const;

    //
    // Operation declare ::Dinner::Fork_broker::obtain_fork
    //
    ::Components::Cookie* obtain_fork();

    //
    // Operation declare ::Dinner::Fork_broker::release_fork
    //
    void release_fork(::Components::Cookie* ck);
};

//
// Interface declare ::Dinner::Named
//
class Named : virtual public CORBA::Object
{
    Named(const Named&);
    void operator=(const Named&);

protected:

    static const char* ids_[];

public:

    Named() { }
    virtual ~Named() { }

    typedef Named_ptr _ptr_type;
    typedef Named_var _var_type;

    static inline Named_ptr
    _duplicate(Named_ptr p)
    {
        if(p)
            p -> _STAR_incRef();
        return p;
    }

    static inline Named_ptr
    _nil()
    {
        return 0;
    }

    static Named_ptr _narrow(CORBA::Object_ptr);
    static Named_ptr _narrow(CORBA::AbstractBase_ptr);

    static Named_ptr _unchecked_narrow(CORBA::AbstractBase_ptr);
    static Named_ptr _unchecked_narrow(CORBA::Object_ptr);

    static const char** _STAR_staticIds();

    //
    // Attribute Get declare ::Dinner::Named::name
    //
    virtual char* name() = 0;

    //
    // Attribute Set declare ::Dinner::Named::name
    //
    virtual void name(const char*) = 0;
};


class Named_broker : virtual public Dinner::Named,
                     virtual public STARCORBA::Object
{
    Named_broker(const Named_broker&);
    void operator=(const Named_broker&);

public:

    Named_broker() { }
    virtual ~Named_broker() { }

    virtual const char** _STAR_ids() const;

    //
    // Attribute Get declare ::Dinner::Named_broker::name
    //
    char* name();

    //
    // Attribute Set declare ::Dinner::Named_broker::name
    //
    void name(const char*);
};

//
// Interface declare ::Dinner::Philosopher
//
class Philosopher : virtual public Dinner::Named,
                    virtual public Components::CCMObject
{
    Philosopher(const Philosopher&);
    void operator=(const Philosopher&);

protected:

    static const char* ids_[];

public:

    Philosopher() { }
    virtual ~Philosopher() { }

    typedef Philosopher_ptr _ptr_type;
    typedef Philosopher_var _var_type;

    static inline Philosopher_ptr
    _duplicate(Philosopher_ptr p)
    {
        if(p)
            p -> _STAR_incRef();
        return p;
    }

    static inline Philosopher_ptr
    _nil()
    {
        return 0;
    }

    static Philosopher_ptr _narrow(CORBA::Object_ptr);
    static Philosopher_ptr _narrow(CORBA::AbstractBase_ptr);

    static Philosopher_ptr _unchecked_narrow(CORBA::AbstractBase_ptr);
    static Philosopher_ptr _unchecked_narrow(CORBA::Object_ptr);

    static const char** _STAR_staticIds();

    //
    // Attribute Get declare ::Dinner::Philosopher::thinking_seconds
    //
    virtual CORBA::ULong thinking_seconds() = 0;

    //
    // Attribute Set declare ::Dinner::Philosopher::thinking_seconds
    //
    virtual void thinking_seconds(CORBA::ULong) = 0;

    //
    // Attribute Get declare ::Dinner::Philosopher::eating_seconds
    //
    virtual CORBA::ULong eating_seconds() = 0;

    //
    // Attribute Set declare ::Dinner::Philosopher::eating_seconds
    //
    virtual void eating_seconds(CORBA::ULong) = 0;

    //
    // Attribute Get declare ::Dinner::Philosopher::sleeping_seconds
    //
    virtual CORBA::ULong sleeping_seconds() = 0;

    //
    // Attribute Set declare ::Dinner::Philosopher::sleeping_seconds
    //
    virtual void sleeping_seconds(CORBA::ULong) = 0;

    //
    // Operation declare ::Dinner::Philosopher::connect_left_hand
    //
    virtual void connect_left_hand(::Dinner::Fork_ptr conn) = 0;

    //
    // Operation declare ::Dinner::Philosopher::disconnect_left_hand
    //
    virtual ::Dinner::Fork_ptr disconnect_left_hand() = 0;

    //
    // Operation declare ::Dinner::Philosopher::get_connection_left_hand
    //
    virtual ::Dinner::Fork_ptr get_connection_left_hand() = 0;

    //
    // Operation declare ::Dinner::Philosopher::connect_right_hand
    //
    virtual void connect_right_hand(::Dinner::Fork_ptr conn) = 0;

    //
    // Operation declare ::Dinner::Philosopher::disconnect_right_hand
    //
    virtual ::Dinner::Fork_ptr disconnect_right_hand() = 0;

    //
    // Operation declare ::Dinner::Philosopher::get_connection_right_hand
    //
    virtual ::Dinner::Fork_ptr get_connection_right_hand() = 0;

    //
    // Operation declare ::Dinner::Philosopher::connect_observer
    //
    virtual void connect_observer(::Dinner::Observing_ptr conn) = 0;

    //
    // Operation declare ::Dinner::Philosopher::disconnect_observer
    //
    virtual ::Dinner::Observing_ptr disconnect_observer() = 0;

    //
    // Operation declare ::Dinner::Philosopher::get_connection_observer
    //
    virtual ::Dinner::Observing_ptr get_connection_observer() = 0;
};


class Philosopher_broker : virtual public Dinner::Philosopher,
                           virtual public Dinner::Named_broker,
                           virtual public Components::CCMObject_broker
{
    Philosopher_broker(const Philosopher_broker&);
    void operator=(const Philosopher_broker&);

public:

    Philosopher_broker() { }
    virtual ~Philosopher_broker() { }

    virtual const char** _STAR_ids() const;

    //
    // Attribute Get declare ::Dinner::Philosopher_broker::thinking_seconds
    //
    CORBA::ULong thinking_seconds();

    //
    // Attribute Set declare ::Dinner::Philosopher_broker::thinking_seconds
    //
    void thinking_seconds(CORBA::ULong);

    //
    // Attribute Get declare ::Dinner::Philosopher_broker::eating_seconds
    //
    CORBA::ULong eating_seconds();

    //
    // Attribute Set declare ::Dinner::Philosopher_broker::eating_seconds
    //
    void eating_seconds(CORBA::ULong);

    //
    // Attribute Get declare ::Dinner::Philosopher_broker::sleeping_seconds
    //
    CORBA::ULong sleeping_seconds();

    //
    // Attribute Set declare ::Dinner::Philosopher_broker::sleeping_seconds
    //
    void sleeping_seconds(CORBA::ULong);

    //
    // Operation declare ::Dinner::Philosopher_broker::connect_left_hand
    //
    void connect_left_hand(::Dinner::Fork_ptr conn);

    //
    // Operation declare ::Dinner::Philosopher_broker::disconnect_left_hand
    //
    ::Dinner::Fork_ptr disconnect_left_hand();

    //
    // Operation declare ::Dinner::Philosopher_broker::get_connection_left_hand
    //
    ::Dinner::Fork_ptr get_connection_left_hand();

    //
    // Operation declare ::Dinner::Philosopher_broker::connect_right_hand
    //
    void connect_right_hand(::Dinner::Fork_ptr conn);

    //
    // Operation declare ::Dinner::Philosopher_broker::disconnect_right_hand
    //
    ::Dinner::Fork_ptr disconnect_right_hand();

    //
    // Operation declare ::Dinner::Philosopher_broker::get_connection_right_hand
    //
    ::Dinner::Fork_ptr get_connection_right_hand();

    //
    // Operation declare ::Dinner::Philosopher_broker::connect_observer
    //
    void connect_observer(::Dinner::Observing_ptr conn);

    //
    // Operation declare ::Dinner::Philosopher_broker::disconnect_observer
    //
    ::Dinner::Observing_ptr disconnect_observer();

    //
    // Operation declare ::Dinner::Philosopher_broker::get_connection_observer
    //
    ::Dinner::Observing_ptr get_connection_observer();
};

//
// Interface declare ::Dinner::Cutlery
//
class Cutlery : virtual public Components::CCMObject
{
    Cutlery(const Cutlery&);
    void operator=(const Cutlery&);

protected:

    static const char* ids_[];

public:

    Cutlery() { }
    virtual ~Cutlery() { }

    typedef Cutlery_ptr _ptr_type;
    typedef Cutlery_var _var_type;

    static inline Cutlery_ptr
    _duplicate(Cutlery_ptr p)
    {
        if(p)
            p -> _STAR_incRef();
        return p;
    }

    static inline Cutlery_ptr
    _nil()
    {
        return 0;
    }

    static Cutlery_ptr _narrow(CORBA::Object_ptr);
    static Cutlery_ptr _narrow(CORBA::AbstractBase_ptr);

    static Cutlery_ptr _unchecked_narrow(CORBA::AbstractBase_ptr);
    static Cutlery_ptr _unchecked_narrow(CORBA::Object_ptr);

    static const char** _STAR_staticIds();

    //
    // Operation declare ::Dinner::Cutlery::provide_the_fork
    //
    virtual ::Dinner::Fork_ptr provide_the_fork() = 0;
};


class Cutlery_broker : virtual public Dinner::Cutlery,
                       virtual public Components::CCMObject_broker
{
    Cutlery_broker(const Cutlery_broker&);
    void operator=(const Cutlery_broker&);

public:

    Cutlery_broker() { }
    virtual ~Cutlery_broker() { }

    virtual const char** _STAR_ids() const;

    //
    // Operation declare ::Dinner::Cutlery_broker::provide_the_fork
    //
    ::Dinner::Fork_ptr provide_the_fork();
};

//
// Interface declare ::Dinner::PhilosopherHomeExplicit
//
class PhilosopherHomeExplicit : virtual public Components::HomeConfiguration
{
    PhilosopherHomeExplicit(const PhilosopherHomeExplicit&);
    void operator=(const PhilosopherHomeExplicit&);

protected:

    static const char* ids_[];

public:

    PhilosopherHomeExplicit() { }
    virtual ~PhilosopherHomeExplicit() { }

    typedef PhilosopherHomeExplicit_ptr _ptr_type;
    typedef PhilosopherHomeExplicit_var _var_type;

    static inline PhilosopherHomeExplicit_ptr
    _duplicate(PhilosopherHomeExplicit_ptr p)
    {
        if(p)
            p -> _STAR_incRef();
        return p;
    }

    static inline PhilosopherHomeExplicit_ptr
    _nil()
    {
        return 0;
    }

    static PhilosopherHomeExplicit_ptr _narrow(CORBA::Object_ptr);
    static PhilosopherHomeExplicit_ptr _narrow(CORBA::AbstractBase_ptr);

    static PhilosopherHomeExplicit_ptr _unchecked_narrow(CORBA::AbstractBase_ptr);
    static PhilosopherHomeExplicit_ptr _unchecked_narrow(CORBA::Object_ptr);

    static const char** _STAR_staticIds();
};


class PhilosopherHomeExplicit_broker : virtual public Dinner::PhilosopherHomeExplicit,
                                       virtual public Components::HomeConfiguration_broker
{
    PhilosopherHomeExplicit_broker(const PhilosopherHomeExplicit_broker&);
    void operator=(const PhilosopherHomeExplicit_broker&);

public:

    PhilosopherHomeExplicit_broker() { }
    virtual ~PhilosopherHomeExplicit_broker() { }

    virtual const char** _STAR_ids() const;
};

//
// Interface declare ::Dinner::PhilosopherHomeImplicit
//
class PhilosopherHomeImplicit : virtual public Components::KeylessCCMHome
{
    PhilosopherHomeImplicit(const PhilosopherHomeImplicit&);
    void operator=(const PhilosopherHomeImplicit&);

protected:

    static const char* ids_[];

public:

    PhilosopherHomeImplicit() { }
    virtual ~PhilosopherHomeImplicit() { }

    typedef PhilosopherHomeImplicit_ptr _ptr_type;
    typedef PhilosopherHomeImplicit_var _var_type;

    static inline PhilosopherHomeImplicit_ptr
    _duplicate(PhilosopherHomeImplicit_ptr p)
    {
        if(p)
            p -> _STAR_incRef();
        return p;
    }

    static inline PhilosopherHomeImplicit_ptr
    _nil()
    {
        return 0;
    }

    static PhilosopherHomeImplicit_ptr _narrow(CORBA::Object_ptr);
    static PhilosopherHomeImplicit_ptr _narrow(CORBA::AbstractBase_ptr);

    static PhilosopherHomeImplicit_ptr _unchecked_narrow(CORBA::AbstractBase_ptr);
    static PhilosopherHomeImplicit_ptr _unchecked_narrow(CORBA::Object_ptr);

    static const char** _STAR_staticIds();

    //
    // Operation declare ::Dinner::PhilosopherHomeImplicit::create
    //
    virtual ::Dinner::Philosopher_ptr create() = 0;
};


class PhilosopherHomeImplicit_broker : virtual public Dinner::PhilosopherHomeImplicit,
                                       virtual public Components::KeylessCCMHome_broker
{
    PhilosopherHomeImplicit_broker(const PhilosopherHomeImplicit_broker&);
    void operator=(const PhilosopherHomeImplicit_broker&);

public:

    PhilosopherHomeImplicit_broker() { }
    virtual ~PhilosopherHomeImplicit_broker() { }

    virtual const char** _STAR_ids() const;

    //
    // Operation declare ::Dinner::PhilosopherHomeImplicit_broker::create
    //
    ::Dinner::Philosopher_ptr create();
};

//
// Interface declare ::Dinner::PhilosopherHome
//
class PhilosopherHome : virtual public Dinner::PhilosopherHomeExplicit,
                        virtual public Dinner::PhilosopherHomeImplicit
{
    PhilosopherHome(const PhilosopherHome&);
    void operator=(const PhilosopherHome&);

protected:

    static const char* ids_[];

public:

    PhilosopherHome() { }
    virtual ~PhilosopherHome() { }

    typedef PhilosopherHome_ptr _ptr_type;
    typedef PhilosopherHome_var _var_type;

    static inline PhilosopherHome_ptr
    _duplicate(PhilosopherHome_ptr p)
    {
        if(p)
            p -> _STAR_incRef();
        return p;
    }

    static inline PhilosopherHome_ptr
    _nil()
    {
        return 0;
    }

    static PhilosopherHome_ptr _narrow(CORBA::Object_ptr);
    static PhilosopherHome_ptr _narrow(CORBA::AbstractBase_ptr);

    static PhilosopherHome_ptr _unchecked_narrow(CORBA::AbstractBase_ptr);
    static PhilosopherHome_ptr _unchecked_narrow(CORBA::Object_ptr);

    static const char** _STAR_staticIds();
};


class PhilosopherHome_broker : virtual public Dinner::PhilosopherHome,
                               virtual public Dinner::PhilosopherHomeExplicit_broker,
                               virtual public Dinner::PhilosopherHomeImplicit_broker
{
    PhilosopherHome_broker(const PhilosopherHome_broker&);
    void operator=(const PhilosopherHome_broker&);

public:

    PhilosopherHome_broker() { }
    virtual ~PhilosopherHome_broker() { }

    virtual const char** _STAR_ids() const;
};

//
// Interface declare ::Dinner::CutleryHomeExplicit
//
class CutleryHomeExplicit : virtual public Components::HomeConfiguration
{
    CutleryHomeExplicit(const CutleryHomeExplicit&);
    void operator=(const CutleryHomeExplicit&);

protected:

    static const char* ids_[];

public:

    CutleryHomeExplicit() { }
    virtual ~CutleryHomeExplicit() { }

    typedef CutleryHomeExplicit_ptr _ptr_type;
    typedef CutleryHomeExplicit_var _var_type;

    static inline CutleryHomeExplicit_ptr
    _duplicate(CutleryHomeExplicit_ptr p)
    {
        if(p)
            p -> _STAR_incRef();
        return p;
    }

    static inline CutleryHomeExplicit_ptr
    _nil()
    {
        return 0;
    }

    static CutleryHomeExplicit_ptr _narrow(CORBA::Object_ptr);
    static CutleryHomeExplicit_ptr _narrow(CORBA::AbstractBase_ptr);

    static CutleryHomeExplicit_ptr _unchecked_narrow(CORBA::AbstractBase_ptr);
    static CutleryHomeExplicit_ptr _unchecked_narrow(CORBA::Object_ptr);

    static const char** _STAR_staticIds();
};


class CutleryHomeExplicit_broker : virtual public Dinner::CutleryHomeExplicit,
                                   virtual public Components::HomeConfiguration_broker
{
    CutleryHomeExplicit_broker(const CutleryHomeExplicit_broker&);
    void operator=(const CutleryHomeExplicit_broker&);

public:

    CutleryHomeExplicit_broker() { }
    virtual ~CutleryHomeExplicit_broker() { }

    virtual const char** _STAR_ids() const;
};

//
// Interface declare ::Dinner::CutleryHomeImplicit
//
class CutleryHomeImplicit : virtual public Components::KeylessCCMHome
{
    CutleryHomeImplicit(const CutleryHomeImplicit&);
    void operator=(const CutleryHomeImplicit&);

protected:

    static const char* ids_[];

public:

    CutleryHomeImplicit() { }
    virtual ~CutleryHomeImplicit() { }

    typedef CutleryHomeImplicit_ptr _ptr_type;
    typedef CutleryHomeImplicit_var _var_type;

    static inline CutleryHomeImplicit_ptr
    _duplicate(CutleryHomeImplicit_ptr p)
    {
        if(p)
            p -> _STAR_incRef();
        return p;
    }

    static inline CutleryHomeImplicit_ptr
    _nil()
    {
        return 0;
    }

    static CutleryHomeImplicit_ptr _narrow(CORBA::Object_ptr);
    static CutleryHomeImplicit_ptr _narrow(CORBA::AbstractBase_ptr);

    static CutleryHomeImplicit_ptr _unchecked_narrow(CORBA::AbstractBase_ptr);
    static CutleryHomeImplicit_ptr _unchecked_narrow(CORBA::Object_ptr);

    static const char** _STAR_staticIds();

    //
    // Operation declare ::Dinner::CutleryHomeImplicit::create
    //
    virtual ::Dinner::Cutlery_ptr create() = 0;
};


class CutleryHomeImplicit_broker : virtual public Dinner::CutleryHomeImplicit,
                                   virtual public Components::KeylessCCMHome_broker
{
    CutleryHomeImplicit_broker(const CutleryHomeImplicit_broker&);
    void operator=(const CutleryHomeImplicit_broker&);

public:

    CutleryHomeImplicit_broker() { }
    virtual ~CutleryHomeImplicit_broker() { }

    virtual const char** _STAR_ids() const;

    //
    // Operation declare ::Dinner::CutleryHomeImplicit_broker::create
    //
    ::Dinner::Cutlery_ptr create();
};

//
// Interface declare ::Dinner::CutleryHome
//
class CutleryHome : virtual public Dinner::CutleryHomeExplicit,
                    virtual public Dinner::CutleryHomeImplicit
{
    CutleryHome(const CutleryHome&);
    void operator=(const CutleryHome&);

protected:

    static const char* ids_[];

public:

    CutleryHome() { }
    virtual ~CutleryHome() { }

    typedef CutleryHome_ptr _ptr_type;
    typedef CutleryHome_var _var_type;

    static inline CutleryHome_ptr
    _duplicate(CutleryHome_ptr p)
    {
        if(p)
            p -> _STAR_incRef();
        return p;
    }

    static inline CutleryHome_ptr
    _nil()
    {
        return 0;
    }

    static CutleryHome_ptr _narrow(CORBA::Object_ptr);
    static CutleryHome_ptr _narrow(CORBA::AbstractBase_ptr);

    static CutleryHome_ptr _unchecked_narrow(CORBA::AbstractBase_ptr);
    static CutleryHome_ptr _unchecked_narrow(CORBA::Object_ptr);

    static const char** _STAR_staticIds();
};


class CutleryHome_broker : virtual public Dinner::CutleryHome,
                           virtual public Dinner::CutleryHomeExplicit_broker,
                           virtual public Dinner::CutleryHomeImplicit_broker
{
    CutleryHome_broker(const CutleryHome_broker&);
    void operator=(const CutleryHome_broker&);

public:

    CutleryHome_broker() { }
    virtual ~CutleryHome_broker() { }

    virtual const char** _STAR_ids() const;
};

} // End of namespace Dinner

//
// Module declare ::Dinner
//
namespace OBV_Dinner
{

} // End of namespace OBV_Dinner

//
// Exception declare ::Dinner::ForkNotAvailable
//
void operator<<=(CORBA::Any&, Dinner::ForkNotAvailable*);
void operator<<=(CORBA::Any&, const Dinner::ForkNotAvailable&);
CORBA::Boolean operator>>=(const CORBA::Any&, const Dinner::ForkNotAvailable*&);

inline void
operator<<=(CORBA::Any_var& any, Dinner::ForkNotAvailable* val)
{
    any.inout() <<= val;
}

inline void
operator<<=(CORBA::Any_var& any, const Dinner::ForkNotAvailable& val)
{
    any.inout() <<= val;
}

inline CORBA::Boolean
operator>>=(const CORBA::Any_var& any, const Dinner::ForkNotAvailable*& val)
{
    return any.in() >>= val;
}

//
// Exception declare ::Dinner::NotTheEater
//
void operator<<=(CORBA::Any&, Dinner::NotTheEater*);
void operator<<=(CORBA::Any&, const Dinner::NotTheEater&);
CORBA::Boolean operator>>=(const CORBA::Any&, const Dinner::NotTheEater*&);

inline void
operator<<=(CORBA::Any_var& any, Dinner::NotTheEater* val)
{
    any.inout() <<= val;
}

inline void
operator<<=(CORBA::Any_var& any, const Dinner::NotTheEater& val)
{
    any.inout() <<= val;
}

inline CORBA::Boolean
operator>>=(const CORBA::Any_var& any, const Dinner::NotTheEater*& val)
{
    return any.in() >>= val;
}

//
// Interface declare ::Dinner::Fork
//
namespace CORBA
{

inline void
release(Dinner::Fork_ptr p)
{
    if(p)
        p -> _STAR_decRef();
}

inline Boolean
is_nil(Dinner::Fork_ptr p)
{
    return p == 0;
}

} // End of namespace CORBA

void operator<<=(CORBA::Any&, Dinner::Fork_ptr*);
void operator<<=(CORBA::Any&, Dinner::Fork_ptr);
CORBA::Boolean operator>>=(const CORBA::Any&, Dinner::Fork_ptr&);

inline void
operator<<=(CORBA::Any_var& any, Dinner::Fork_ptr* val)
{
    any.inout() <<= val;
}

inline void
operator<<=(CORBA::Any_var& any, Dinner::Fork_ptr val)
{
    any.inout() <<= val;
}

inline CORBA::Boolean
operator>>=(const CORBA::Any_var& any, Dinner::Fork_ptr& val)
{
    return any.in() >>= val;
}

//
// Interface declare ::Dinner::Named
//
namespace CORBA
{

inline void
release(Dinner::Named_ptr p)
{
    if(p)
        p -> _STAR_decRef();
}

inline Boolean
is_nil(Dinner::Named_ptr p)
{
    return p == 0;
}

} // End of namespace CORBA

void operator<<=(CORBA::Any&, Dinner::Named_ptr*);
void operator<<=(CORBA::Any&, Dinner::Named_ptr);
CORBA::Boolean operator>>=(const CORBA::Any&, Dinner::Named_ptr&);

inline void
operator<<=(CORBA::Any_var& any, Dinner::Named_ptr* val)
{
    any.inout() <<= val;
}

inline void
operator<<=(CORBA::Any_var& any, Dinner::Named_ptr val)
{
    any.inout() <<= val;
}

inline CORBA::Boolean
operator>>=(const CORBA::Any_var& any, Dinner::Named_ptr& val)
{
    return any.in() >>= val;
}

//
// Interface declare ::Dinner::Philosopher
//
namespace CORBA
{

inline void
release(Dinner::Philosopher_ptr p)
{
    if(p)
        p -> _STAR_decRef();
}

inline Boolean
is_nil(Dinner::Philosopher_ptr p)
{
    return p == 0;
}

} // End of namespace CORBA

void operator<<=(CORBA::Any&, Dinner::Philosopher_ptr*);
void operator<<=(CORBA::Any&, Dinner::Philosopher_ptr);
CORBA::Boolean operator>>=(const CORBA::Any&, Dinner::Philosopher_ptr&);

inline void
operator<<=(CORBA::Any_var& any, Dinner::Philosopher_ptr* val)
{
    any.inout() <<= val;
}

inline void
operator<<=(CORBA::Any_var& any, Dinner::Philosopher_ptr val)
{
    any.inout() <<= val;
}

inline CORBA::Boolean
operator>>=(const CORBA::Any_var& any, Dinner::Philosopher_ptr& val)
{
    return any.in() >>= val;
}

//
// Interface declare ::Dinner::Cutlery
//
namespace CORBA
{

inline void
release(Dinner::Cutlery_ptr p)
{
    if(p)
        p -> _STAR_decRef();
}

inline Boolean
is_nil(Dinner::Cutlery_ptr p)
{
    return p == 0;
}

} // End of namespace CORBA

void operator<<=(CORBA::Any&, Dinner::Cutlery_ptr*);
void operator<<=(CORBA::Any&, Dinner::Cutlery_ptr);
CORBA::Boolean operator>>=(const CORBA::Any&, Dinner::Cutlery_ptr&);

inline void
operator<<=(CORBA::Any_var& any, Dinner::Cutlery_ptr* val)
{
    any.inout() <<= val;
}

inline void
operator<<=(CORBA::Any_var& any, Dinner::Cutlery_ptr val)
{
    any.inout() <<= val;
}

inline CORBA::Boolean
operator>>=(const CORBA::Any_var& any, Dinner::Cutlery_ptr& val)
{
    return any.in() >>= val;
}

//
// Interface declare ::Dinner::PhilosopherHomeExplicit
//
namespace CORBA
{

inline void
release(Dinner::PhilosopherHomeExplicit_ptr p)
{
    if(p)
        p -> _STAR_decRef();
}

inline Boolean
is_nil(Dinner::PhilosopherHomeExplicit_ptr p)
{
    return p == 0;
}

} // End of namespace CORBA

void operator<<=(CORBA::Any&, Dinner::PhilosopherHomeExplicit_ptr*);
void operator<<=(CORBA::Any&, Dinner::PhilosopherHomeExplicit_ptr);
CORBA::Boolean operator>>=(const CORBA::Any&, Dinner::PhilosopherHomeExplicit_ptr&);

inline void
operator<<=(CORBA::Any_var& any, Dinner::PhilosopherHomeExplicit_ptr* val)
{
    any.inout() <<= val;
}

inline void
operator<<=(CORBA::Any_var& any, Dinner::PhilosopherHomeExplicit_ptr val)
{
    any.inout() <<= val;
}

inline CORBA::Boolean
operator>>=(const CORBA::Any_var& any, Dinner::PhilosopherHomeExplicit_ptr& val)
{
    return any.in() >>= val;
}

//
// Interface declare ::Dinner::PhilosopherHomeImplicit
//
namespace CORBA
{

inline void
release(Dinner::PhilosopherHomeImplicit_ptr p)
{
    if(p)
        p -> _STAR_decRef();
}

inline Boolean
is_nil(Dinner::PhilosopherHomeImplicit_ptr p)
{
    return p == 0;
}

} // End of namespace CORBA

void operator<<=(CORBA::Any&, Dinner::PhilosopherHomeImplicit_ptr*);
void operator<<=(CORBA::Any&, Dinner::PhilosopherHomeImplicit_ptr);
CORBA::Boolean operator>>=(const CORBA::Any&, Dinner::PhilosopherHomeImplicit_ptr&);

inline void
operator<<=(CORBA::Any_var& any, Dinner::PhilosopherHomeImplicit_ptr* val)
{
    any.inout() <<= val;
}

inline void
operator<<=(CORBA::Any_var& any, Dinner::PhilosopherHomeImplicit_ptr val)
{
    any.inout() <<= val;
}

inline CORBA::Boolean
operator>>=(const CORBA::Any_var& any, Dinner::PhilosopherHomeImplicit_ptr& val)
{
    return any.in() >>= val;
}

//
// Interface declare ::Dinner::PhilosopherHome
//
namespace CORBA
{

inline void
release(Dinner::PhilosopherHome_ptr p)
{
    if(p)
        p -> _STAR_decRef();
}

inline Boolean
is_nil(Dinner::PhilosopherHome_ptr p)
{
    return p == 0;
}

} // End of namespace CORBA

void operator<<=(CORBA::Any&, Dinner::PhilosopherHome_ptr*);
void operator<<=(CORBA::Any&, Dinner::PhilosopherHome_ptr);
CORBA::Boolean operator>>=(const CORBA::Any&, Dinner::PhilosopherHome_ptr&);

inline void
operator<<=(CORBA::Any_var& any, Dinner::PhilosopherHome_ptr* val)
{
    any.inout() <<= val;
}

inline void
operator<<=(CORBA::Any_var& any, Dinner::PhilosopherHome_ptr val)
{
    any.inout() <<= val;
}

inline CORBA::Boolean
operator>>=(const CORBA::Any_var& any, Dinner::PhilosopherHome_ptr& val)
{
    return any.in() >>= val;
}

//
// Interface declare ::Dinner::CutleryHomeExplicit
//
namespace CORBA
{

inline void
release(Dinner::CutleryHomeExplicit_ptr p)
{
    if(p)
        p -> _STAR_decRef();
}

inline Boolean
is_nil(Dinner::CutleryHomeExplicit_ptr p)
{
    return p == 0;
}

} // End of namespace CORBA

void operator<<=(CORBA::Any&, Dinner::CutleryHomeExplicit_ptr*);
void operator<<=(CORBA::Any&, Dinner::CutleryHomeExplicit_ptr);
CORBA::Boolean operator>>=(const CORBA::Any&, Dinner::CutleryHomeExplicit_ptr&);

inline void
operator<<=(CORBA::Any_var& any, Dinner::CutleryHomeExplicit_ptr* val)
{
    any.inout() <<= val;
}

inline void
operator<<=(CORBA::Any_var& any, Dinner::CutleryHomeExplicit_ptr val)
{
    any.inout() <<= val;
}

inline CORBA::Boolean
operator>>=(const CORBA::Any_var& any, Dinner::CutleryHomeExplicit_ptr& val)
{
    return any.in() >>= val;
}

//
// Interface declare ::Dinner::CutleryHomeImplicit
//
namespace CORBA
{

inline void
release(Dinner::CutleryHomeImplicit_ptr p)
{
    if(p)
        p -> _STAR_decRef();
}

inline Boolean
is_nil(Dinner::CutleryHomeImplicit_ptr p)
{
    return p == 0;
}

} // End of namespace CORBA

void operator<<=(CORBA::Any&, Dinner::CutleryHomeImplicit_ptr*);
void operator<<=(CORBA::Any&, Dinner::CutleryHomeImplicit_ptr);
CORBA::Boolean operator>>=(const CORBA::Any&, Dinner::CutleryHomeImplicit_ptr&);

inline void
operator<<=(CORBA::Any_var& any, Dinner::CutleryHomeImplicit_ptr* val)
{
    any.inout() <<= val;
}

inline void
operator<<=(CORBA::Any_var& any, Dinner::CutleryHomeImplicit_ptr val)
{
    any.inout() <<= val;
}

inline CORBA::Boolean
operator>>=(const CORBA::Any_var& any, Dinner::CutleryHomeImplicit_ptr& val)
{
    return any.in() >>= val;
}

//
// Interface declare ::Dinner::CutleryHome
//
namespace CORBA
{

inline void
release(Dinner::CutleryHome_ptr p)
{
    if(p)
        p -> _STAR_decRef();
}

inline Boolean
is_nil(Dinner::CutleryHome_ptr p)
{
    return p == 0;
}

} // End of namespace CORBA

void operator<<=(CORBA::Any&, Dinner::CutleryHome_ptr*);
void operator<<=(CORBA::Any&, Dinner::CutleryHome_ptr);
CORBA::Boolean operator>>=(const CORBA::Any&, Dinner::CutleryHome_ptr&);

inline void
operator<<=(CORBA::Any_var& any, Dinner::CutleryHome_ptr* val)
{
    any.inout() <<= val;
}

inline void
operator<<=(CORBA::Any_var& any, Dinner::CutleryHome_ptr val)
{
    any.inout() <<= val;
}

inline CORBA::Boolean
operator>>=(const CORBA::Any_var& any, Dinner::CutleryHome_ptr& val)
{
    return any.in() >>= val;
}

#endif
