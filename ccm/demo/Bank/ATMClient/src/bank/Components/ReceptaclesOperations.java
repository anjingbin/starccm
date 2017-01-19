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

package bank.Components;

//
// IDL:omg.org/Components/Receptacles:1.0
//
/***/

public interface ReceptaclesOperations
{
    //
    // IDL:omg.org/Components/Receptacles/connect:1.0
    //
    /***/

    Cookie
    connect(String name,
            org.omg.CORBA.Object connection)
        throws InvalidName,
               InvalidConnection,
               AlreadyConnected,
               ExceededConnectionLimit;

    //
    // IDL:omg.org/Components/Receptacles/disconnect:1.0
    //
    /***/

    void
    disconnect(String name,
               Cookie ck)
        throws InvalidName,
               InvalidConnection,
               CookieRequired,
               NoConnection;

    //
    // IDL:omg.org/Components/Receptacles/get_connections:1.0
    //
    /***/

    ConnectionDescription[]
    get_connections(String name)
        throws InvalidName;

    //
    // IDL:omg.org/Components/Receptacles/get_all_receptacles:1.0
    //
    /***/

    ReceptacleDescription[]
    get_all_receptacles();

    //
    // IDL:omg.org/Components/Receptacles/get_named_receptacles:1.0
    //
    /***/

    ReceptacleDescription[]
    get_named_receptacles(String[] names)
        throws InvalidName;
}
