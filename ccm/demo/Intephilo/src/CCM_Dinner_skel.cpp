// **********************************************************************
//
// Generated by the CIDL Translator
//
// Copyright (c) 2001-2004
// StarMiddleware Group
// www.StarMiddleware.net
//
// All Rights Reserved
//
// Author: Huang Jie        huangjie@email.com
// Author: Chang Junsheng   cjs7908@163.com
// Author: Zhen Xianrong
//
// **********************************************************************

#include <CORBA.h>
#include <CCM_Dinner_skel.h>

Dinner::PhilosopherImpl::CCM_Philosopher_skel::CCM_Philosopher_skel()
{
}

Dinner::PhilosopherImpl::CCM_Philosopher_skel::~CCM_Philosopher_skel()
{
}

::CORBA::OctetSeq*
Dinner::PhilosopherImpl::CCM_Philosopher_skel::get_pid()
{
    return 0;
}

Dinner::PhilosopherImpl::CCM_PhilosopherHome_skel::CCM_PhilosopherHome_skel()
{
}

Dinner::PhilosopherImpl::CCM_PhilosopherHome_skel::~CCM_PhilosopherHome_skel()
{
}

::CORBA::OctetSeq*
Dinner::PhilosopherImpl::CCM_PhilosopherHome_skel::get_pid()
{
    return 0;
}

void
Dinner::PhilosopherImpl::CCM_PhilosopherHome_skel::set_context(Components::CCM2Context_ptr ctx)
{
    ctx_ = Components::CCM2Context::_duplicate(ctx);
}

Dinner::CCM_Fork_ptr 
Dinner::CutleryImpl::CCM_Cutlery_skel::get_the_fork()
    throw(CORBA::SystemException)
{
    return Dinner::CCM_Fork::_duplicate(this);
}

Dinner::CutleryImpl::CCM_Cutlery_skel::CCM_Cutlery_skel()
{
}

Dinner::CutleryImpl::CCM_Cutlery_skel::~CCM_Cutlery_skel()
{
}

::CORBA::OctetSeq*
Dinner::CutleryImpl::CCM_Cutlery_skel::get_pid()
{
    return 0;
}

Dinner::CutleryImpl::CCM_CutleryHome_skel::CCM_CutleryHome_skel()
{
}

Dinner::CutleryImpl::CCM_CutleryHome_skel::~CCM_CutleryHome_skel()
{
}

::CORBA::OctetSeq*
Dinner::CutleryImpl::CCM_CutleryHome_skel::get_pid()
{
    return 0;
}

void
Dinner::CutleryImpl::CCM_CutleryHome_skel::set_context(Components::CCM2Context_ptr ctx)
{
    ctx_ = Components::CCM2Context::_duplicate(ctx);
}

