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


#ifndef __CCM_test_impl_h__
#define __CCM_test_impl_h__

#include <CCM.h>

#include <CCM_test_skel.h>
#include <test_ValueType_impl.h>


namespace Test
{
    namespace TalkImpl
    {
        class CCM_Talk_impl : virtual public Test::TalkImpl::CCM_Talk_skel,
                              public Components::SessionComponent
        {
            CCM_Talk_impl(const CCM_Talk_impl&);
            void operator=(const CCM_Talk_impl&);

        protected:
            Test::CCM_Talk_Context_var ctx_;
        public:

            CCM_Talk_impl();
            ~CCM_Talk_impl();

            virtual void configuration_complete();
           virtual void set_session_context(Components::SessionContext*)
                throw(Components::CCMException,
                      CORBA::SystemException);
            virtual void ccm_activate()
                throw(Components::CCMException,
                      CORBA::SystemException);
            virtual void ccm_passivate()
                throw(Components::CCMException,
                      CORBA::SystemException);
            virtual void ccm_remove()
                throw(Components::CCMException,
                      CORBA::SystemException);
            #ifdef TAO
            virtual const char* _interface_repository_id() const { return 0; }
            virtual void* _tao_QueryInterface(ptr_arith_t type) { return 0; }
            #endif

            virtual char* sayHello()
                throw(CORBA::SystemException);
        };

        class CCM_TalkHome_impl : virtual public Test::TalkImpl::CCM_TalkHome_skel
        {
            CCM_TalkHome_impl(const CCM_TalkHome_impl&);
            void operator=(const CCM_TalkHome_impl&);

        public:

            CCM_TalkHome_impl();
            ~CCM_TalkHome_impl();

            virtual Components::EnterpriseComponent_ptr create()
                throw(Components::CCMException,
                      CORBA::SystemException);

            virtual char* get_storage_home_id();
        };


    }//End of namespace TalkImpl


    namespace PersonImpl
    {
        class CCM_Person_impl : virtual public Test::PersonImpl::CCM_Person_skel,
                                public Components::SessionComponent
        {
            CCM_Person_impl(const CCM_Person_impl&);
            void operator=(const CCM_Person_impl&);

        protected:
            Test::CCM_Person_Context_var ctx_;
        public:

            CCM_Person_impl();
            ~CCM_Person_impl();

            virtual void configuration_complete();
           virtual void set_session_context(Components::SessionContext*)
                throw(Components::CCMException,
                      CORBA::SystemException);
            virtual void ccm_activate()
                throw(Components::CCMException,
                      CORBA::SystemException);
            virtual void ccm_passivate()
                throw(Components::CCMException,
                      CORBA::SystemException);
            virtual void ccm_remove()
                throw(Components::CCMException,
                      CORBA::SystemException);
            #ifdef TAO
            virtual const char* _interface_repository_id() const { return 0; }
            virtual void* _tao_QueryInterface(ptr_arith_t type) { return 0; }
            #endif
        };

        class CCM_PersonHome_impl : virtual public Test::PersonImpl::CCM_PersonHome_skel
        {
            CCM_PersonHome_impl(const CCM_PersonHome_impl&);
            void operator=(const CCM_PersonHome_impl&);

        public:

            CCM_PersonHome_impl();
            ~CCM_PersonHome_impl();

            virtual Components::EnterpriseComponent_ptr create()
                throw(Components::CCMException,
                      CORBA::SystemException);

            virtual char* get_storage_home_id();
        };


    }//End of namespace PersonImpl


}//End of namespace Test


#endif
