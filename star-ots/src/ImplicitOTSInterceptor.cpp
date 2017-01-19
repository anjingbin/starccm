// **********************************************************************
//
// Copyright (c) 2002
// Network Information Security Institute,Computer College,NUDT.
// ChangSha,Hunan,PRC
//
// All Rights Reserved
//
// **********************************************************************

#include <STAR/CORBA.h>

#include <STAR/PI_init.h>
#include <STAR/PICurrent.h>
#include <STAR/STARORBInit.h>

#include "ImplicitOTSInterceptor.h"

#include <OTSServerInterceptor_impl.h>
#include <OTSClientInterceptor_impl.h>
#include <OTSIORInterceptor_impl.h>
#include <OTSPolicyFactory_impl.h>
#include <CosTransactionsCurrent_impl.h>

#define cfgDefaultTimeout 300//renyi 6 7:added

#ifdef HAVE_FSTREAM
#   include <fstream>
#else
#   include <fstream.h>
#endif

#ifdef HAVE_STD_IOSTREAM
using namespace std;
#endif

//renyi 6 17
#ifndef _DEBUG_
#define _DEBUG_
#endif

using namespace STAR;

namespace PortableInterceptor
{

//
// The OTS ORB initializer
//
class OTSClientORBInitializer_impl : public PortableInterceptor::ORBInitializer,
	                                            CORBA::LocalObject //renyi 12 2 : added
{
    //
    // We want to avoid retaining state in the ORBinitializer since
    // there are no guarantees in the specification regarding
    // concurrency of pre_init and post_init calls.
    //

public:

    //SuLiang modify 2003-7-1
	virtual bool _STAR_isLC() const { return true; };
	//end modify. 2003-7-1


    OTSClientORBInitializer_impl()
    {
    }
    
    ~OTSClientORBInitializer_impl()
    {
    }

    //
    // Standard IDL to C++ Mapping
    //
    virtual void
    pre_init(PortableInterceptor::ORBInitInfo_ptr info)
    {
		#ifdef _DEBUG_
			cout << "[OTSClientORBInitializer_impl::pre_init begin!]" << endl;
		#endif

		//
		// Create the CDR codec
		//
        IOP::CodecFactory_var codecFactory = info -> codec_factory();

        IOP::Encoding encoding;
        encoding.format = IOP::ENCODING_CDR_ENCAPS;
        encoding.major_version = 1;
        encoding.minor_version = 2;
        IOP::Codec_var codec = codecFactory -> create_codec(encoding);

		PortableInterceptor::SlotId slotId = info -> allocate_state_slot();

		//
		// Register the TransactionCurrent
		//
        PortableInterceptor::Current_var piCurrent =
            PortableInterceptor::Current::_narrow(
		CORBA::Object_var(
		    info -> resolve_initial_references("PICurrent")));

		CosTransactions::Current_var current = 
	    new CosTransactions::Current_impl(
		piCurrent, slotId, cfgDefaultTimeout);//renyi 6 9:STARCosTransactions--->CosTransactions

        info -> register_initial_reference("TransactionCurrent", current);

		//
		// Register the server side interceptor
		//
		//renyi 6 7:changed
		PortableInterceptor::ServerRequestInterceptor_var srvInterceptor =
			new PortableInterceptor::OTSServerInterceptor_impl(slotId, codec);
        info -> add_server_request_interceptor(srvInterceptor);

		//
		// Register the client side interceptor
		//

        PortableInterceptor::ClientRequestInterceptor_var cltInterceptor =
			new PortableInterceptor::OTSClientInterceptor_impl(slotId, codec);
        info -> add_client_request_interceptor(cltInterceptor);

		//
		// Register the OTS IOR interceptor
		//
        PortableInterceptor::IORInterceptor_var iorInterceptor =
			new PortableInterceptor::OTSIORInterceptor_impl(codec);
        info -> add_ior_interceptor(iorInterceptor);

		//
		// Register a policy factory to create the OTS policies
		//
        PortableInterceptor::PolicyFactory_var txnPolicyFactory =
			new PortableInterceptor::OTSPolicyFactory_impl();
        info -> register_policy_factory(CosTransactions::OTS_POLICY_TYPE, txnPolicyFactory);
        info -> register_policy_factory(CosTransactions::INVOCATION_POLICY_TYPE, txnPolicyFactory);
        info -> register_policy_factory(CosTransactions::NON_TX_TARGET_POLICY_TYPE, txnPolicyFactory);
		
		#ifdef _DEBUG_
			cout << "[OTSClientORBInitializer_impl::pre_init end!]" << endl;
		#endif
    }
    
    virtual void
    post_init(PortableInterceptor::ORBInitInfo_ptr info)
    {
		//
		// This is retained because it might be necessary to setup the
		// TransactionFactory in the pre_init call for interposition.
		//

		//renyi 6 7:STARCosTransactions-->CosTransactions
		#ifdef _DEBUG_
			cout << "[OTSClientORBInitializer_impl::post_init begin!]" << endl;
		#endif
		
		//renyi 6 20:added

		STARPortableInterceptor::ORBInitInfo_var starinfo =
		    STARPortableInterceptor::ORBInitInfo::_narrow(info);
		CORBA::ORB_var orb = starinfo -> orb();
		
		//
		// Get  objects of OTS: CosTransactions::TransactionFactory
		//
		
//		const char* path = getenv("CCM_HOME");
//
//		CORBA::String_var ots_refFile = path;
//		ots_refFile += "\\ots.ref";

		const char* ots_refFile = "ots.ref";
		ifstream ots_in(ots_refFile);
		
		if(ots_in.fail())
		{
			//renyi 6 20:changed

		    cerr << ": can't open `" << ots_refFile << "': "
			<< "    ImplicitOTSInterceptor.cpp(For Link Error 2001):strerror(errno) not printed in this version......" << endl;

		}

		char S_TF[1000];

		ots_in >> S_TF;
		ots_in.close();
			
		CORBA::Object_var TF = orb -> string_to_object(S_TF);
		assert(!CORBA::is_nil(TF));
		
		CosTransactions::TransactionFactory_var TF_var
				= CosTransactions::TransactionFactory::_narrow(TF);
		assert(!CORBA::is_nil(TF_var));

		CosTransactions::Current_var current =
			CosTransactions::Current::_narrow(
				CORBA::Object_var(
				    info -> resolve_initial_references("TransactionCurrent")));
		    
	    //renyi 6 7:added
	    CosTransactions::Current_impl* currentImpl =
		    dynamic_cast<CosTransactions::Current_impl*>(current.in());
	    currentImpl -> setTransactionFactory(TF_var);

		#ifdef _DEBUG_
			cout << "[OTSClientORBInitializer_impl::post_init end!]" << endl;
		#endif

    }
};

} // End namespace PortableInterceptor

namespace STAR
{

	static bool OTSInitCalled = false;

};

//
// Initialize the OTS client side runtime
//
void
STAR::OTSInit(int& argc, char** argv)
{
    #ifdef _DEBUG_
		cout << "[STAR::OTSInit begin!]" << endl;
	#endif

	assert(!STAR::OTSInitCalled);
    STAR::OTSInitCalled = true;

    //
    // Register the OTS ORB initializer
    //
    PortableInterceptor::ORBInitializer_var orbInitializer =
        new PortableInterceptor::OTSClientORBInitializer_impl;

    PortableInterceptor::register_orb_initializer(orbInitializer);

	#ifdef _DEBUG_
		cout << "[STAR::OTSInit end!]" << endl;
	#endif

}
