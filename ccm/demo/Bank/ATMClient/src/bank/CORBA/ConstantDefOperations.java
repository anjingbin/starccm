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

package bank.CORBA;

//
// IDL:omg.org/CORBA/ConstantDef:1.0
//
/***/

public interface ConstantDefOperations extends ContainedOperations
{
    //
    // IDL:omg.org/CORBA/ConstantDef/type:1.0
    //
    /***/

    org.omg.CORBA.TypeCode
    type();

    //
    // IDL:omg.org/CORBA/ConstantDef/type_def:1.0
    //
    /***/

    IDLType
    type_def();

    void
    type_def(IDLType val);

    //
    // IDL:omg.org/CORBA/ConstantDef/value:1.0
    //
    /***/

    org.omg.CORBA.Any
    value();

    void
    value(org.omg.CORBA.Any val);
}
