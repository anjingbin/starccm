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

#ifndef ___Resource_h__
#define ___Resource_h__

#include <PostgreSQLResource.h>

class ResourceFactory;
typedef ResourceFactory* ResourceFactory_ptr;
typedef ResourceFactory* ResourceFactoryRef;

extern STAR::TypeCodeConst _tc_ResourceFactory;

class Resource;
typedef Resource* Resource_ptr;
typedef Resource* ResourceRef;

extern STAR::TypeCodeConst _tc_Resource;

void STARDuplicate(ResourceFactory_ptr);
void STARRelease(ResourceFactory_ptr);

void STARMarshal(ResourceFactory_ptr, STAR::OutputStreamImpl*);
void STARUnmarshal(ResourceFactory_ptr&, STAR::InputStreamImpl*);

void STARDuplicate(Resource_ptr);
void STARRelease(Resource_ptr);

void STARMarshal(Resource_ptr, STAR::OutputStreamImpl*);
void STARUnmarshal(Resource_ptr&, STAR::InputStreamImpl*);

typedef STAR::ObjVar< ResourceFactory > ResourceFactory_var;
typedef STAR::ObjOut< ResourceFactory > ResourceFactory_out;

typedef STAR::ObjVar< Resource > Resource_var;
typedef STAR::ObjOut< Resource > Resource_out;

//
// Interface declare ::ResourceFactory
//
class ResourceFactory : virtual public CORBA::Object
{
    ResourceFactory(const ResourceFactory&);
    void operator=(const ResourceFactory&);

protected:

    static const char* ids_[];

public:

    ResourceFactory() { }
    virtual ~ResourceFactory() { }

    typedef ResourceFactory_ptr _ptr_type;
    typedef ResourceFactory_var _var_type;

    static inline ResourceFactory_ptr
    _duplicate(ResourceFactory_ptr p)
    {
        if(p)
            p -> _STAR_incRef();
        return p;
    }

    static inline ResourceFactory_ptr
    _nil()
    {
        return 0;
    }

    static ResourceFactory_ptr _narrow(CORBA::Object_ptr);
    static ResourceFactory_ptr _narrow(CORBA::AbstractBase_ptr);

    static const char** _STAR_staticIds();

    //
    // Operation declare ::ResourceFactory::create
    //
    virtual Resource_ptr create(CORBA::Short dbtype) = 0;
};


class ResourceFactory_broker : virtual public ResourceFactory,
                               virtual public STARCORBA::Object
{
    ResourceFactory_broker(const ResourceFactory_broker&);
    void operator=(const ResourceFactory_broker&);

public:

    ResourceFactory_broker() { }
    virtual ~ResourceFactory_broker() { }

    virtual const char** _STAR_ids() const;

    //
    // Operation declare ::ResourceFactory_broker::create
    //
    Resource_ptr create(CORBA::Short dbtype);
};

//
// Interface declare ::Resource
//
class Resource : virtual public PG_Resource
{
    Resource(const Resource&);
    void operator=(const Resource&);

protected:

    static const char* ids_[];

public:

    Resource() { }
    virtual ~Resource() { }

    typedef Resource_ptr _ptr_type;
    typedef Resource_var _var_type;

    static inline Resource_ptr
    _duplicate(Resource_ptr p)
    {
        if(p)
            p -> _STAR_incRef();
        return p;
    }

    static inline Resource_ptr
    _nil()
    {
        return 0;
    }

    static Resource_ptr _narrow(CORBA::Object_ptr);
    static Resource_ptr _narrow(CORBA::AbstractBase_ptr);

    static const char** _STAR_staticIds();

    //
    // Operation declare ::Resource::prepare
    //
    virtual CosTransactions::Vote prepare() = 0;

    //
    // Operation declare ::Resource::rollback
    //
    virtual void rollback() = 0;

    //
    // Operation declare ::Resource::commit
    //
    virtual void commit() = 0;

    //
    // Operation declare ::Resource::commit_one_phase
    //
    virtual void commit_one_phase() = 0;

    //
    // Operation declare ::Resource::forget
    //
    virtual void forget() = 0;

    //
    // Operation declare ::Resource::release
    //
    virtual void release() = 0;

    //
    // Operation declare ::Resource::BeginTrans
    //
    virtual CORBA::Boolean BeginTrans() = 0;

    //
    // Operation declare ::Resource::set_recoverycoordinator
    //
    virtual void set_recoverycoordinator(CosTransactions::RecoveryCoordinator_ptr rc) = 0;

    //
    // Operation declare ::Resource::heuristic_decision
    //
    virtual void heuristic_decision() = 0;
};


class Resource_broker : virtual public Resource,
                        virtual public PG_Resource_broker
{
    Resource_broker(const Resource_broker&);
    void operator=(const Resource_broker&);

public:

    Resource_broker() { }
    virtual ~Resource_broker() { }

    virtual const char** _STAR_ids() const;

    //
    // Operation declare ::Resource_broker::prepare
    //
    CosTransactions::Vote prepare();

    //
    // Operation declare ::Resource_broker::rollback
    //
    void rollback();

    //
    // Operation declare ::Resource_broker::commit
    //
    void commit();

    //
    // Operation declare ::Resource_broker::commit_one_phase
    //
    void commit_one_phase();

    //
    // Operation declare ::Resource_broker::forget
    //
    void forget();

    //
    // Operation declare ::Resource_broker::release
    //
    void release();

    //
    // Operation declare ::Resource_broker::BeginTrans
    //
    CORBA::Boolean BeginTrans();

    //
    // Operation declare ::Resource_broker::set_recoverycoordinator
    //
    void set_recoverycoordinator(CosTransactions::RecoveryCoordinator_ptr rc);

    //
    // Operation declare ::Resource_broker::heuristic_decision
    //
    void heuristic_decision();
};

//
// Interface declare ::ResourceFactory
//
namespace CORBA
{

inline void
release(ResourceFactory_ptr p)
{
    if(p)
        p -> _STAR_decRef();
}

inline Boolean
is_nil(ResourceFactory_ptr p)
{
    return p == 0;
}

} // End of namespace CORBA

void operator<<=(CORBA::Any&, ResourceFactory_ptr*);
void operator<<=(CORBA::Any&, ResourceFactory_ptr);
CORBA::Boolean operator>>=(const CORBA::Any&, ResourceFactory_ptr&);

inline void
operator<<=(CORBA::Any_var& any, ResourceFactory_ptr* val)
{
    any.inout() <<= val;
}

inline void
operator<<=(CORBA::Any_var& any, ResourceFactory_ptr val)
{
    any.inout() <<= val;
}

inline CORBA::Boolean
operator>>=(const CORBA::Any_var& any, ResourceFactory_ptr& val)
{
    return any.in() >>= val;
}

//
// Interface declare ::Resource
//
namespace CORBA
{

inline void
release(Resource_ptr p)
{
    if(p)
        p -> _STAR_decRef();
}

inline Boolean
is_nil(Resource_ptr p)
{
    return p == 0;
}

} // End of namespace CORBA

void operator<<=(CORBA::Any&, Resource_ptr*);
void operator<<=(CORBA::Any&, Resource_ptr);
CORBA::Boolean operator>>=(const CORBA::Any&, Resource_ptr&);

inline void
operator<<=(CORBA::Any_var& any, Resource_ptr* val)
{
    any.inout() <<= val;
}

inline void
operator<<=(CORBA::Any_var& any, Resource_ptr val)
{
    any.inout() <<= val;
}

inline CORBA::Boolean
operator>>=(const CORBA::Any_var& any, Resource_ptr& val)
{
    return any.in() >>= val;
}

#endif