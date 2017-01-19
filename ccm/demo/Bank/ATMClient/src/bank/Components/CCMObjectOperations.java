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
// IDL:omg.org/Components/CCMObject:1.0
//
/***/

public interface CCMObjectOperations extends NavigationOperations,
                                             ReceptaclesOperations,
                                             EventsOperations
{
    //
    // IDL:omg.org/Components/CCMObject/get_component_def:1.0
    //
    /***/

    bank.CORBA.IRObject
    get_component_def();

    //
    // IDL:omg.org/Components/CCMObject/get_ccm_home:1.0
    //
    /***/

    CCMHome
    get_ccm_home();

    //
    // IDL:omg.org/Components/CCMObject/get_primary_key:1.0
    //
    /***/

    PrimaryKeyBase
    get_primary_key()
        throws NoKeyAvailable;

    //
    // IDL:omg.org/Components/CCMObject/configuration_complete:1.0
    //
    /***/

    void
    configuration_complete()
        throws InvalidConfiguration;

    //
    // IDL:omg.org/Components/CCMObject/remove:1.0
    //
    /***/

    void
    remove()
        throws RemoveFailure;

    //
    // IDL:omg.org/Components/CCMObject/get_all_ports:1.0
    //
    /***/

    ComponentPortDescription
    get_all_ports();
}
