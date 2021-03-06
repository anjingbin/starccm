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
// IDL:omg.org/CORBA/Repository:1.0
//
/***/

public interface RepositoryOperations extends ContainerOperations
{
    //
    // IDL:omg.org/CORBA/Repository/lookup_id:1.0
    //
    /***/

    Contained
    lookup_id(String search_id);

    //
    // IDL:omg.org/CORBA/Repository/get_canonical_typecode:1.0
    //
    /***/

    org.omg.CORBA.TypeCode
    get_canonical_typecode(org.omg.CORBA.TypeCode tc);

    //
    // IDL:omg.org/CORBA/Repository/get_primitive:1.0
    //
    /***/

    PrimitiveDef
    get_primitive(PrimitiveKind kind);

    //
    // IDL:omg.org/CORBA/Repository/create_string:1.0
    //
    /***/

    StringDef
    create_string(int bound);

    //
    // IDL:omg.org/CORBA/Repository/create_wstring:1.0
    //
    /***/

    WstringDef
    create_wstring(int bound);

    //
    // IDL:omg.org/CORBA/Repository/create_sequence:1.0
    //
    /***/

    SequenceDef
    create_sequence(int bound,
                    IDLType element_type);

    //
    // IDL:omg.org/CORBA/Repository/create_array:1.0
    //
    /***/

    ArrayDef
    create_array(int length,
                 IDLType element_type);

    //
    // IDL:omg.org/CORBA/Repository/create_fixed:1.0
    //
    /***/

    FixedDef
    create_fixed(short digits,
                 short scale);
}
