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
// IDL:omg.org/CORBA/UnionDef:1.0
//
/***/

public interface UnionDefOperations extends TypedefDefOperations,
                                            ContainerOperations
{
    //
    // IDL:omg.org/CORBA/UnionDef/discriminator_type:1.0
    //
    /***/

    org.omg.CORBA.TypeCode
    discriminator_type();

    //
    // IDL:omg.org/CORBA/UnionDef/discriminator_type_def:1.0
    //
    /***/

    IDLType
    discriminator_type_def();

    void
    discriminator_type_def(IDLType val);

    //
    // IDL:omg.org/CORBA/UnionDef/members:1.0
    //
    /***/

    UnionMember[]
    members();

    void
    members(UnionMember[] val);
}
