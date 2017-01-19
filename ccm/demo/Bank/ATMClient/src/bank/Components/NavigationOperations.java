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
// IDL:omg.org/Components/Navigation:1.0
//
/***/

public interface NavigationOperations
{
    //
    // IDL:omg.org/Components/Navigation/provide_facet:1.0
    //
    /***/

    org.omg.CORBA.Object
    provide_facet(String name)
        throws InvalidName;

    //
    // IDL:omg.org/Components/Navigation/get_all_facets:1.0
    //
    /***/

    FacetDescription[]
    get_all_facets();

    //
    // IDL:omg.org/Components/Navigation/get_named_facets:1.0
    //
    /***/

    FacetDescription[]
    get_named_facets(String[] names)
        throws InvalidName;

    //
    // IDL:omg.org/Components/Navigation/same_component:1.0
    //
    /***/

    boolean
    same_component(org.omg.CORBA.Object object_ref);
}