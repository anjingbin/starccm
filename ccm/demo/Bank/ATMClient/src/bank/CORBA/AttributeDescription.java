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
// IDL:omg.org/CORBA/AttributeDescription:1.0
//
/***/

final public class AttributeDescription implements org.omg.CORBA.portable.IDLEntity
{
    private static final String _star_id = "IDL:omg.org/CORBA/AttributeDescription:1.0";

    public
    AttributeDescription()
    {
    }

    public
    AttributeDescription(String name,
                         String id,
                         String defined_in,
                         String version,
                         org.omg.CORBA.TypeCode type,
                         AttributeMode mode)
    {
        this.name = name;
        this.id = id;
        this.defined_in = defined_in;
        this.version = version;
        this.type = type;
        this.mode = mode;
    }

    public String name;
    public String id;
    public String defined_in;
    public String version;
    public org.omg.CORBA.TypeCode type;
    public AttributeMode mode;
}
