//include the component head files.
//---------------------------------------------------------------------------

#ifndef searchFH
#define searchFH
//---------------------------------------------------------------------------
#include "corba.h"
#include "CosNaming_c.hh"


#include "PetStore_c.h"
#include "CCMHome_c.h"
#include "CCMObject_c.h"
#include "HomeFinder_c.h"
#include "CCMCommon_c.h"

#define debug  true

extern Components::HomeFinder_var HomeFinder_;
extern PetStore::BusinessOp_var businessOp;
extern PetStore::Sales_var sales;
extern PetStore::Cart_var cart;    
extern CORBA::ORB_var orb;

void __fastcall getComponents(void);
//---------------------------------------------------------------------------
#endif
