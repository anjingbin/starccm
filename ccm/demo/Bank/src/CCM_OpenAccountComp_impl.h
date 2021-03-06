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


#ifndef __CCM_OpenAccountComp_impl_h__
#define __CCM_OpenAccountComp_impl_h__

#include <CCM.h>

#include <CCM_OpenAccountComp_skel.h>
#include <OpenAccountComp_ValueType_impl.h>



#include <libpq-fe.h>

namespace Bank
{
    namespace OpenAccountCompComposition
    {
        class CCM_OpenAccountComp_impl : virtual public Bank::OpenAccountCompComposition::CCM_OpenAccountComp_skel,
                                         public Components::SessionComponent
        {
            CCM_OpenAccountComp_impl(const CCM_OpenAccountComp_impl&);
            void operator=(const CCM_OpenAccountComp_impl&);

        protected:
            Bank::CCM_OpenAccountComp_Context_var ctx_;
            
            PGconn * database_;
			CORBA::Boolean connect_database()
    			throw(CORBA::SystemException);
        public:

            CCM_OpenAccountComp_impl();
            ~CCM_OpenAccountComp_impl();

	    virtual const char** _STAR_ids() const { return 0; }
            virtual void configuration_complete()
            	throw (Components::InvalidConfiguration, CORBA::SystemException);
            virtual void enter_dyn_deploy()
            	throw(CORBA::SystemException);
            virtual void leave_dyn_deploy()
            	throw(CORBA::SystemException);
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

            virtual void openAccount(const char* id,
                                     const char* password)
                throw(CORBA::SystemException);
        };

        class CCM_OpenAccountCompHome_impl : virtual public Bank::OpenAccountCompComposition::CCM_OpenAccountCompHome_skel
        {
            CCM_OpenAccountCompHome_impl(const CCM_OpenAccountCompHome_impl&);
            void operator=(const CCM_OpenAccountCompHome_impl&);

        public:

            CCM_OpenAccountCompHome_impl();
            ~CCM_OpenAccountCompHome_impl();

            virtual Components::EnterpriseComponent_ptr create()
                throw(Components::CCMException,
                      CORBA::SystemException);

            virtual char* get_storage_home_id();
        };


    }//End of namespace OpenAccountCompComposition


}//End of namespace Bank


#endif
