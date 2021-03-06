// **********************************************************************
//
// This File Is Automatically Generated by the InterBus IDL-to-Java Compiler !
//
// Copyright (c) 2003
// Intervision Software Co., Ltd.
// Haidian District, Beijing.
//
// All Rights Reserved
//
// **********************************************************************

// Version: 5.0a

package bank.Bank;

//
// IDL:Bank/DepositComp:1.0
//
/***/

public interface DepositCompOperations extends bank.Components.CCMObjectOperations
{
    //
    // IDL:Bank/DepositComp/provide_deposit_op:1.0
    //
    /***/

    DepositOp
    provide_deposit_op();

    //
    // IDL:Bank/DepositComp/connect_the_account:1.0
    //
    /***/

    void
    connect_the_account(AccountOp conn)
        throws bank.Components.AlreadyConnected,
               bank.Components.InvalidConnection;

    //
    // IDL:Bank/DepositComp/disconnect_the_account:1.0
    //
    /***/

    AccountOp
    disconnect_the_account()
        throws bank.Components.NoConnection;

    //
    // IDL:Bank/DepositComp/get_connection_the_account:1.0
    //
    /***/

    AccountOp
    get_connection_the_account();
}
