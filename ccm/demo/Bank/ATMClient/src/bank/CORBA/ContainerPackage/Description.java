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

package bank.CORBA.ContainerPackage;

//
// IDL:omg.org/CORBA/Container/Description:1.0
//
/***/

final public class Description implements org.omg.CORBA.portable.IDLEntity
{
    private static final String _star_id = "IDL:omg.org/CORBA/Container/Description:1.0";

    public
    Description()
    {
    }

    public
    Description(bank.CORBA.Contained contained_object,
                bank.CORBA.DefinitionKind kind,
                org.omg.CORBA.Any value)
    {
        this.contained_object = contained_object;
        this.kind = kind;
        this.value = value;
    }

    public bank.CORBA.Contained contained_object;
    public bank.CORBA.DefinitionKind kind;
    public org.omg.CORBA.Any value;
}
