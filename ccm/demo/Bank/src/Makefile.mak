# **********************************************************************
#  Copyright (c) 2003
#  StarMiddleware.net
#  www.StarMiddleware.net
#
#  All Rights Reserved
#
#  Author: Huang Jie	huangjie@email.com
# **********************************************************************



top_srcdir	= ..\..\..

GENERATED 	= 
GENERATED1  = Account.idl \
		Account_impl.h \
		Account_impl.cpp \
		Account_ValueType_impl.h \
		Account_ValueType_impl.cpp \
		AccountDll.cpp \
		StdAccountDll.cpp \
		Account.cpf \
		Account.h \
		Account.cpp \
		Account_skel.cpp \
		Account_skel.h \
		CCM_Account_Context_impl.h \
		CCM_Account_Context_impl.cpp \
		CCM_Account.idl \
		Account.ccd \
		CCM_Account.cpp \
		CCM_Account.h \
		CCM_Account_skel.cpp \
		CCM_Account_skel.h 
GENERATED2  = DepositComp.idl \
		DepositComp_impl.h \
		DepositComp_impl.cpp \
		DepositComp_ValueType_impl.h \
		DepositComp_ValueType_impl.cpp \
		DepositCompDll.cpp \
		StdDepositCompDll.cpp \
		DepositComp.cpf \
		DepositComp.h \
		DepositComp.cpp \
		DepositComp_skel.cpp \
		DepositComp_skel.h \
		CCM_DepositComp_Context_impl.h \
		CCM_DepositComp_Context_impl.cpp \
		CCM_DepositComp.idl \
		DepositComp.ccd \
		CCM_DepositComp.cpp \
		CCM_DepositComp.h \
		CCM_DepositComp_skel.cpp \
		CCM_DepositComp_skel.h 
GENERATED3  = Authentication.idl \
		Authentication_impl.h \
		Authentication_impl.cpp \
		Authentication_ValueType_impl.h \
		Authentication_ValueType_impl.cpp \
		AuthenticationDll.cpp \
		StdAuthenticationDll.cpp \
		Authentication.cpf \
		Authentication.h \
		Authentication.cpp \
		Authentication_skel.cpp \
		Authentication_skel.h \
		CCM_Authentication_Context_impl.h \
		CCM_Authentication_Context_impl.cpp \
		CCM_Authentication.idl \
		Authentication.ccd \
		CCM_Authentication.cpp \
		CCM_Authentication.h \
		CCM_Authentication_skel.cpp \
		CCM_Authentication_skel.h 
GENERATED4  = QueryComp.idl \
		QueryComp_impl.h \
		QueryComp_impl.cpp \
		QueryComp_ValueType_impl.h \
		QueryComp_ValueType_impl.cpp \
		QueryCompDll.cpp \
		StdQueryCompDll.cpp \
		QueryComp.cpf \
		QueryComp.h \
		QueryComp.cpp \
		QueryComp_skel.cpp \
		QueryComp_skel.h \
		CCM_QueryComp_Context_impl.h \
		CCM_QueryComp_Context_impl.cpp \
		CCM_QueryComp.idl \
		QueryComp.ccd \
		CCM_QueryComp.cpp \
		CCM_QueryComp.h \
		CCM_QueryComp_skel.cpp \
		CCM_QueryComp_skel.h 
GENERATED5  = InterestComputer.idl \
		InterestComputer_impl.h \
		InterestComputer_impl.cpp \
		InterestComputer_ValueType_impl.h \
		InterestComputer_ValueType_impl.cpp \
		InterestComputerDll.cpp \
		StdInterestComputerDll.cpp \
		InterestComputer.cpf \
		InterestComputer.h \
		InterestComputer.cpp \
		InterestComputer_skel.cpp \
		InterestComputer_skel.h \
		CCM_InterestComputer_Context_impl.h \
		CCM_InterestComputer_Context_impl.cpp \
		CCM_InterestComputer.idl \
		InterestComputer.ccd \
		CCM_InterestComputer.cpp \
		CCM_InterestComputer.h \
		CCM_InterestComputer_skel.cpp \
		CCM_InterestComputer_skel.h 
GENERATED6  = WithdrawComp.idl \
		WithdrawComp_impl.h \
		WithdrawComp_impl.cpp \
		WithdrawComp_ValueType_impl.h \
		WithdrawComp_ValueType_impl.cpp \
		WithdrawCompDll.cpp \
		StdWithdrawCompDll.cpp \
		WithdrawComp.cpf \
		WithdrawComp.h \
		WithdrawComp.cpp \
		WithdrawComp_skel.cpp \
		WithdrawComp_skel.h \
		CCM_WithdrawComp_Context_impl.h \
		CCM_WithdrawComp_Context_impl.cpp \
		CCM_WithdrawComp.idl \
		WithdrawComp.ccd \
		CCM_WithdrawComp.cpp \
		CCM_WithdrawComp.h \
		CCM_WithdrawComp_skel.cpp \
		CCM_WithdrawComp_skel.h 
GENERATED7  = Delegations.idl \
		Delegations_impl.h \
		Delegations_impl.cpp \
		Delegations_ValueType_impl.h \
		Delegations_ValueType_impl.cpp \
		DelegationsDll.cpp \
		StdDelegationsDll.cpp \
		Delegations.cpf \
		Delegations.h \
		Delegations.cpp \
		Delegations_skel.cpp \
		Delegations_skel.h \
		CCM_Delegations_Context_impl.h \
		CCM_Delegations_Context_impl.cpp \
		CCM_Delegations.idl \
		Delegations.ccd \
		CCM_Delegations.cpp \
		CCM_Delegations.h \
		CCM_Delegations_skel.cpp \
		CCM_Delegations_skel.h 
GENERATED8  = ATM.idl \
		ATM_impl.h \
		ATM_impl.cpp \
		ATM_ValueType_impl.h \
		ATM_ValueType_impl.cpp \
		ATMDll.cpp \
		StdATMDll.cpp \
		ATM.cpf \
		ATM.h \
		ATM.cpp \
		ATM_skel.cpp \
		ATM_skel.h \
		CCM_ATM_Context_impl.h \
		CCM_ATM_Context_impl.cpp \
		CCM_ATM.idl \
		ATM.ccd \
		CCM_ATM.cpp \
		CCM_ATM.h \
		CCM_ATM_skel.cpp \
		CCM_ATM_skel.h 
GENERATED9  = OpenAccountComp.idl \
		OpenAccountComp_impl.h \
		OpenAccountComp_impl.cpp \
		OpenAccountComp_ValueType_impl.h \
		OpenAccountComp_ValueType_impl.cpp \
		OpenAccountCompDll.cpp \
		StdOpenAccountCompDll.cpp \
		OpenAccountComp.cpf \
		OpenAccountComp.h \
		OpenAccountComp.cpp \
		OpenAccountComp_skel.cpp \
		OpenAccountComp_skel.h \
		CCM_OpenAccountComp_Context_impl.h \
		CCM_OpenAccountComp_Context_impl.cpp \
		CCM_OpenAccountComp.idl \
		OpenAccountComp.ccd \
		CCM_OpenAccountComp.cpp \
		CCM_OpenAccountComp.h \
		CCM_OpenAccountComp_skel.cpp \
		CCM_OpenAccountComp_skel.h 
GENERATED10  = DelAccountComp.idl \
		DelAccountComp_impl.h \
		DelAccountComp_impl.cpp \
		DelAccountComp_ValueType_impl.h \
		DelAccountComp_ValueType_impl.cpp \
		DelAccountCompDll.cpp \
		StdDelAccountCompDll.cpp \
		DelAccountComp.cpf \
		DelAccountComp.h \
		DelAccountComp.cpp \
		DelAccountComp_skel.cpp \
		DelAccountComp_skel.h \
		CCM_DelAccountComp_Context_impl.h \
		CCM_DelAccountComp_Context_impl.cpp \
		CCM_DelAccountComp.idl \
		DelAccountComp.ccd \
		CCM_DelAccountComp.cpp \
		CCM_DelAccountComp.h \
		CCM_DelAccountComp_skel.cpp \
		CCM_DelAccountComp_skel.h 
GENERATED11  = Manager.idl \
		Manager_impl.h \
		Manager_impl.cpp \
		Manager_ValueType_impl.h \
		Manager_ValueType_impl.cpp \
		ManagerDll.cpp \
		StdManagerDll.cpp \
		Manager.cpf \
		Manager.h \
		Manager.cpp \
		Manager_skel.cpp \
		Manager_skel.h \
		CCM_Manager_Context_impl.h \
		CCM_Manager_Context_impl.cpp \
		CCM_Manager.idl \
		Manager.ccd \
		CCM_Manager.cpp \
		CCM_Manager.h \
		CCM_Manager_skel.cpp \
		CCM_Manager_skel.h 


!include $(top_srcdir)\config\Make.rules.mak

DLLS = Account StdAccount \
	DepositComp  StdDepositComp \
	Authentication  StdAuthentication \
	QueryComp  StdQueryComp \
	InterestComputer  StdInterestComputer \
	WithdrawComp  StdWithdrawComp  \
	Delegation  StdDelegation \
	ATM   StdATM \
	OpenAccountComp  StdOpenAccountComp \
	DelAccountComp  StdDelAccountComp \
	Manager  StdManager

clean-all:: clean
	@for %i in ( $(GENERATED1) ) do \
	    @if exist %i del %i
	@for %i in ( $(GENERATED2) ) do \
	    @if exist %i del %i
	@for %i in ( $(GENERATED3) ) do \
	    @if exist %i del %i
	@for %i in ( $(GENERATED4) ) do \
	    @if exist %i del %i
	@for %i in ( $(GENERATED5) ) do \
	    @if exist %i del %i
	@for %i in ( $(GENERATED6) ) do \
	    @if exist %i del %i
	@for %i in ( $(GENERATED7) ) do \
	    @if exist %i del %i
	@for %i in ( $(GENERATED8) ) do \
	    @if exist %i del %i
	@for %i in ( $(GENERATED9) ) do \
	    @if exist %i del %i
	@for %i in ( $(GENERATED10) ) do \
	    @if exist %i del %i
	@for %i in ( $(GENERATED11) ) do \
	    @if exist %i del %i

clean-all:: clean
	-del *.cpf

CCMDIR=$(prefix)
ORBDIR=$(OB_HOME)

install all:: CCM_Account.idl \
		CCM_DepositComp.idl \
		CCM_Authentication.idl \
		CCM_QueryComp.idl \
		CCM_InterestComputer.idl \
		CCM_WithdrawComp.idl \
		CCM_Delegations.idl \
		CCM_ATM.idl \
		CCM_OpenAccountComp.idl \
		CCM_DelAccountComp.idl \
		CCM_Manager.idl

CCM_IDL3FLAGS = $(CCM_IDL3FLAGS) --std-dll-entry
CCM_CIDLFLAGS = $(CCM_CIDLFLAGS) --std-dll-entry


!if "$(ORB_NAME)" == "tao"
OTS_IDLDIR = $(CCM_HOME)\idl\tao
OB_IDLDIR = $(TAO_ROOT)

CCM_Account.idl: Account.idl3
	$(IDL3) -I. $(CCM_IDL3FLAGS) -I$(OB_IDLDIR) -I$(OB_IDLDIR)/tao -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) Account.idl3
	$(IDL)  -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) Account.idl
	$(CIDL) -I. $(CCM_CIDLFLAGS) -I$(OB_IDLDIR) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) Account.cidl
!else
CCM_Account.idl: Account.idl3
	$(IDL3) -I. $(CCM_IDL3FLAGS) -I$(OB_IDLDIR) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) Account.idl3
	$(IDL)  -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) Account.idl
	$(CIDL) -I. $(CCM_CIDLFLAGS) -I$(OB_IDLDIR) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) Account.cidl
!endif

!if "$(ORB_NAME)" == "ob"
	$(IDL) -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) --no-skeletons CCM_Account.idl
!endif

!if "$(ORB_NAME)" == "star"
	$(IDL) -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) --no-skel CCM_Account.idl
!endif

!if "$(ORB_NAME)" == "tao"
	$(IDL) -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) CCM_Account.idl
!endif


!if "$(ORB_NAME)" == "tao"
OTS_IDLDIR = $(CCM_HOME)\idl\tao
OB_IDLDIR = $(TAO_ROOT)

CCM_DepositComp.idl: DepositComp.idl3
	$(IDL3) -I. $(CCM_IDL3FLAGS) -I$(OB_IDLDIR) -I$(OB_IDLDIR)/tao -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) DepositComp.idl3
	$(IDL)  -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) DepositComp.idl
	$(CIDL) -I. $(CCM_CIDLFLAGS) -I$(OB_IDLDIR) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) DepositComp.cidl
!else
CCM_DepositComp.idl: DepositComp.idl3
	$(IDL3) -I. $(CCM_IDL3FLAGS) -I$(OB_IDLDIR) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) DepositComp.idl3
	$(IDL)  -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) DepositComp.idl
	$(CIDL) -I. $(CCM_CIDLFLAGS) -I$(OB_IDLDIR) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) DepositComp.cidl
!endif

!if "$(ORB_NAME)" == "ob"
	$(IDL) -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) --no-skeletons CCM_DepositComp.idl
!endif

!if "$(ORB_NAME)" == "star"
	$(IDL) -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) --no-skel CCM_DepositComp.idl
!endif

!if "$(ORB_NAME)" == "tao"
	$(IDL) -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) CCM_DepositComp.idl
!endif


!if "$(ORB_NAME)" == "tao"
OTS_IDLDIR = $(CCM_HOME)\idl\tao
OB_IDLDIR = $(TAO_ROOT)

CCM_Authentication.idl: Authentication.idl3
	$(IDL3) -I. $(CCM_IDL3FLAGS) -I$(OB_IDLDIR) -I$(OB_IDLDIR)/tao -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) Authentication.idl3
	$(IDL)  -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) Authentication.idl
	$(CIDL) -I. $(CCM_CIDLFLAGS) -I$(OB_IDLDIR) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) Authentication.cidl
!else
CCM_Authentication.idl: Authentication.idl3
	$(IDL3) -I. $(CCM_IDL3FLAGS) -I$(OB_IDLDIR) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) Authentication.idl3
	$(IDL)  -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) Authentication.idl
	$(CIDL) -I. $(CCM_CIDLFLAGS) -I$(OB_IDLDIR) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) Authentication.cidl
!endif

!if "$(ORB_NAME)" == "ob"
	$(IDL) -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) --no-skeletons CCM_Authentication.idl
!endif

!if "$(ORB_NAME)" == "star"
	$(IDL) -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) --no-skel CCM_Authentication.idl
!endif

!if "$(ORB_NAME)" == "tao"
	$(IDL) -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) CCM_Authentication.idl
!endif


!if "$(ORB_NAME)" == "tao"
OTS_IDLDIR = $(CCM_HOME)\idl\tao
OB_IDLDIR = $(TAO_ROOT)

CCM_QueryComp.idl: QueryComp.idl3
	$(IDL3) -I. $(CCM_IDL3FLAGS) -I$(OB_IDLDIR) -I$(OB_IDLDIR)/tao -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) QueryComp.idl3
	$(IDL)  -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) QueryComp.idl
	$(CIDL) -I. $(CCM_CIDLFLAGS) -I$(OB_IDLDIR) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) QueryComp.cidl
!else
CCM_QueryComp.idl: QueryComp.idl3
	$(IDL3) -I. $(CCM_IDL3FLAGS) -I$(OB_IDLDIR) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) QueryComp.idl3
	$(IDL)  -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) QueryComp.idl
	$(CIDL) -I. $(CCM_CIDLFLAGS) -I$(OB_IDLDIR) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) QueryComp.cidl
!endif

!if "$(ORB_NAME)" == "ob"
	$(IDL) -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) --no-skeletons CCM_QueryComp.idl
!endif

!if "$(ORB_NAME)" == "star"
	$(IDL) -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) --no-skel CCM_QueryComp.idl
!endif

!if "$(ORB_NAME)" == "tao"
	$(IDL) -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) CCM_QueryComp.idl
!endif



!if "$(ORB_NAME)" == "tao"
OTS_IDLDIR = $(CCM_HOME)\idl\tao
OB_IDLDIR = $(TAO_ROOT)

CCM_InterestComputer.idl: InterestComputer.idl3
	$(IDL3) -I. $(CCM_IDL3FLAGS) -I$(OB_IDLDIR) -I$(OB_IDLDIR)/tao -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) InterestComputer.idl3
	$(IDL)  -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) InterestComputer.idl
	$(CIDL) -I. $(CCM_CIDLFLAGS) -I$(OB_IDLDIR) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) InterestComputer.cidl
!else
CCM_InterestComputer.idl: InterestComputer.idl3
	$(IDL3) -I. $(CCM_IDL3FLAGS) -I$(OB_IDLDIR) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) InterestComputer.idl3
	$(IDL)  -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) InterestComputer.idl
	$(CIDL) -I. $(CCM_CIDLFLAGS) -I$(OB_IDLDIR) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) InterestComputer.cidl
!endif

!if "$(ORB_NAME)" == "ob"
	$(IDL) -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) --no-skeletons CCM_InterestComputer.idl
!endif

!if "$(ORB_NAME)" == "star"
	$(IDL) -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) --no-skel CCM_InterestComputer.idl
!endif

!if "$(ORB_NAME)" == "tao"
	$(IDL) -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) CCM_InterestComputer.idl
!endif

!if "$(ORB_NAME)" == "tao"
OTS_IDLDIR = $(CCM_HOME)\idl\tao
OB_IDLDIR = $(TAO_ROOT)

CCM_WithdrawComp.idl: WithdrawComp.idl3
	$(IDL3) -I. $(CCM_IDL3FLAGS) -I$(OB_IDLDIR) -I$(OB_IDLDIR)/tao -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) WithdrawComp.idl3
	$(IDL)  -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) WithdrawComp.idl
	$(CIDL) -I. $(CCM_CIDLFLAGS) -I$(OB_IDLDIR) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) WithdrawComp.cidl
!else
CCM_WithdrawComp.idl: WithdrawComp.idl3
	$(IDL3) -I. $(CCM_IDL3FLAGS) -I$(OB_IDLDIR) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) WithdrawComp.idl3
	$(IDL)  -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) WithdrawComp.idl
	$(CIDL) -I. $(CCM_CIDLFLAGS) -I$(OB_IDLDIR) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) WithdrawComp.cidl
!endif

!if "$(ORB_NAME)" == "ob"
	$(IDL) -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) --no-skeletons CCM_WithdrawComp.idl
!endif

!if "$(ORB_NAME)" == "star"
	$(IDL) -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) --no-skel CCM_WithdrawComp.idl
!endif

!if "$(ORB_NAME)" == "tao"
	$(IDL) -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) CCM_WithdrawComp.idl
!endif



!if "$(ORB_NAME)" == "tao"
OTS_IDLDIR = $(CCM_HOME)\idl\tao
OB_IDLDIR = $(TAO_ROOT)

CCM_Delegations.idl: Delegations.idl3
	$(IDL3) -I. $(CCM_IDL3FLAGS) -I$(OB_IDLDIR) -I$(OB_IDLDIR)/tao -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) Delegations.idl3
	$(IDL)  -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) Delegations.idl
	$(CIDL) -I. $(CCM_CIDLFLAGS) -I$(OB_IDLDIR) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) Delegations.cidl
!else
CCM_Delegations.idl: Delegations.idl3
	$(IDL3) -I. $(CCM_IDL3FLAGS) -I$(OB_IDLDIR) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) Delegations.idl3
	$(IDL)  -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) Delegations.idl
	$(CIDL) -I. $(CCM_CIDLFLAGS) -I$(OB_IDLDIR) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) Delegations.cidl
!endif

!if "$(ORB_NAME)" == "ob"
	$(IDL) -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) --no-skeletons CCM_Delegations.idl
!endif

!if "$(ORB_NAME)" == "star"
	$(IDL) -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) --no-skel CCM_Delegations.idl
!endif

!if "$(ORB_NAME)" == "tao"
	$(IDL) -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) CCM_Delegations.idl
!endif


!if "$(ORB_NAME)" == "tao"
OTS_IDLDIR = $(CCM_HOME)\idl\tao
OB_IDLDIR = $(TAO_ROOT)

CCM_ATM.idl: ATM.idl3
	$(IDL3) -I. $(CCM_IDL3FLAGS) -I$(OB_IDLDIR) -I$(OB_IDLDIR)/tao -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) ATM.idl3
	$(IDL)  -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) ATM.idl
	$(CIDL) -I. $(CCM_CIDLFLAGS) -I$(OB_IDLDIR) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) ATM.cidl
!else
CCM_ATM.idl: ATM.idl3
	$(IDL3) -I. $(CCM_IDL3FLAGS) -I$(OB_IDLDIR) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) ATM.idl3
	$(IDL)  -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) ATM.idl
	$(CIDL) -I. $(CCM_CIDLFLAGS) -I$(OB_IDLDIR) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) ATM.cidl
!endif


!if "$(ORB_NAME)" == "ob"
	$(IDL) -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) --no-skeletons CCM_ATM.idl
!endif

!if "$(ORB_NAME)" == "star"
	$(IDL) -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) --no-skel CCM_ATM.idl
!endif

!if "$(ORB_NAME)" == "tao"
	$(IDL) -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) CCM_ATM.idl
!endif


!if "$(ORB_NAME)" == "tao"
OTS_IDLDIR = $(CCM_HOME)\idl\tao
OB_IDLDIR = $(TAO_ROOT)

CCM_OpenAccountComp.idl: OpenAccountComp.idl3
	$(IDL3) -I. $(CCM_IDL3FLAGS) -I$(OB_IDLDIR) -I$(OB_IDLDIR)/tao -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) OpenAccountComp.idl3
	$(IDL)  -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) OpenAccountComp.idl
	$(CIDL) -I. $(CCM_CIDLFLAGS) -I$(OB_IDLDIR) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) OpenAccountComp.cidl
!else
CCM_OpenAccountComp.idl: OpenAccountComp.idl3
	$(IDL3) -I. $(CCM_IDL3FLAGS) -I$(OB_IDLDIR) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) OpenAccountComp.idl3
	$(IDL)  -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) OpenAccountComp.idl
	$(CIDL) -I. $(CCM_CIDLFLAGS) -I$(OB_IDLDIR) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) OpenAccountComp.cidl
!endif

!if "$(ORB_NAME)" == "ob"
	$(IDL) -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) --no-skeletons CCM_OpenAccountComp.idl
!endif

!if "$(ORB_NAME)" == "star"
	$(IDL) -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) --no-skel CCM_OpenAccountComp.idl
!endif

!if "$(ORB_NAME)" == "tao"
	$(IDL) -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) CCM_OpenAccountComp.idl
!endif


!if "$(ORB_NAME)" == "tao"
OTS_IDLDIR = $(CCM_HOME)\idl\tao
OB_IDLDIR = $(TAO_ROOT)

CCM_DelAccountComp.idl: DelAccountComp.idl3
	$(IDL3) -I. $(CCM_IDL3FLAGS) -I$(OB_IDLDIR) -I$(OB_IDLDIR)/tao -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) DelAccountComp.idl3
	$(IDL)  -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) DelAccountComp.idl
	$(CIDL) -I. $(CCM_CIDLFLAGS) -I$(OB_IDLDIR) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) DelAccountComp.cidl
!else
CCM_DelAccountComp.idl: DelAccountComp.idl3
	$(IDL3) -I. $(CCM_IDL3FLAGS) -I$(OB_IDLDIR) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) DelAccountComp.idl3
	$(IDL)  -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) DelAccountComp.idl
	$(CIDL) -I. $(CCM_CIDLFLAGS) -I$(OB_IDLDIR) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) DelAccountComp.cidl
!endif

!if "$(ORB_NAME)" == "ob"
	$(IDL) -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) --no-skeletons CCM_DelAccountComp.idl
!endif

!if "$(ORB_NAME)" == "star"
	$(IDL) -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) --no-skel CCM_DelAccountComp.idl
!endif

!if "$(ORB_NAME)" == "tao"
	$(IDL) -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) CCM_DelAccountComp.idl
!endif


!if "$(ORB_NAME)" == "tao"
OTS_IDLDIR = $(CCM_HOME)\idl\tao
OB_IDLDIR = $(TAO_ROOT)

CCM_Manager.idl: Manager.idl3
	$(IDL3) -I. $(CCM_IDL3FLAGS) -I$(OB_IDLDIR) -I$(OB_IDLDIR)/tao -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) Manager.idl3
	$(IDL)  -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) Manager.idl
	$(CIDL) -I. $(CCM_CIDLFLAGS) -I$(OB_IDLDIR) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) Manager.cidl
!else
CCM_Manager.idl: Manager.idl3
	$(IDL3) -I. $(CCM_IDL3FLAGS) -I$(OB_IDLDIR) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) Manager.idl3
	$(IDL)  -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) Manager.idl
	$(CIDL) -I. $(CCM_CIDLFLAGS) -I$(OB_IDLDIR) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) Manager.cidl
!endif

!if "$(ORB_NAME)" == "ob"
	$(IDL) -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) --no-skeletons CCM_Manager.idl
!endif

!if "$(ORB_NAME)" == "star"
	$(IDL) -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) -I$(OTS_IDLDIR) -I$(PSS_IDLDIR) --no-skel CCM_Manager.idl
!endif

!if "$(ORB_NAME)" == "tao"
	$(IDL) -I. $(OB_IDLFLAGS) -I$(CCM_IDLDIR) CCM_Manager.idl
!endif


$(EVERYTHING)::
	@for %i in ( $(DLLS) ) do \
	    @if exist %i.mak cmd /c $(MAKE) /NOLOGO /f %i.mak $@"

install:: all
	copy ..\bin\*Account.dll ..\Bank\Account\93e1e9a6-2880-11d7-9c64-f6a003291bf5
	copy ..\bin\*DepositComp.dll ..\Bank\DepositComp\d35275cc-286a-11d7-8c80-8fca68dc242e
	copy ..\bin\*Authentication.dll ..\Bank\Authentication\1408d24a-2886-11d7-8609-d971ee33e54a
	copy ..\bin\*QueryComp.dll ..\Bank\QueryComp\fcda6382-287e-11d7-9c64-f6a003291bf5
	copy ..\bin\*InterestComputer.dll ..\Bank\InterestComputer\744697e3-287f-11d7-9c64-f6a003291bf5
	copy ..\bin\*WithdrawComp.dll ..\Bank\WithdrawComp\f8f3fc5d-4725-4c5b-b814-157c0f71c589
	copy ..\bin\*Delegation.dll ..\Bank\Delegation\8f42239b-1e67-11d7-b81f-dc8107a7889d
	copy ..\bin\*ATM.dll ..\Bank\ATM\d31e39d4-287f-11d7-9c64-f6a003291bf5
	copy ..\bin\*OpenAccountComp.dll ..\Bank\OpenAccountComp\dd16da20-577d-4275-96fa-705384422a1f
	copy ..\bin\*DelAccountComp.dll ..\Bank\DelAccountComp\83f9e630-dace-40a7-854f-f6089f3c7364
	copy ..\bin\*Manager.dll ..\Bank\Manager\2aa2dbc5-2880-11d7-9c64-f6a003291bf5
	cd ..\Bank && $(top_srcdir)\..\ccm\bin\zip.exe  -r Bank.zip * -x CVS  && move Bank.zip $(bindir)
	
	
mkdep::
	$(MKDEP) /V /W /E /O.windep /I. $(CCM_INCLUDE) *.cpp
	@if exist MkDep.log \
		@del MkDep.log









