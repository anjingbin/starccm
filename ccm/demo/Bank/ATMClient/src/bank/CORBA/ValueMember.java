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
// IDL:omg.org/CORBA/ValueMember:1.0
//
/***/

final public class ValueMember implements org.omg.CORBA.portable.IDLEntity
{
    private static final String _star_id = "IDL:omg.org/CORBA/ValueMember:1.0";

    public
    ValueMember()
    {
    }

    public
    ValueMember(String name,
                String id,
                String defined_in,
                String version,
                org.omg.CORBA.TypeCode type,
                IDLType type_def,
                short access)
    {
        this.name = name;
        this.id = id;
        this.defined_in = defined_in;
        this.version = version;
        this.type = type;
        this.type_def = type_def;
        this.access = access;
    }

    public String name;
    public String id;
    public String defined_in;
    public String version;
    public org.omg.CORBA.TypeCode type;
    public IDLType type_def;
    public short access;
}
