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
// IDL:omg.org/CORBA/AttributeMode:1.0
//
final public class AttributeModeHolder implements org.omg.CORBA.portable.Streamable
{
    public AttributeMode value;

    public
    AttributeModeHolder()
    {
    }

    public
    AttributeModeHolder(AttributeMode initial)
    {
        value = initial;
    }

    public void
    _read(org.omg.CORBA.portable.InputStream in)
    {
        value = AttributeModeHelper.read(in);
    }

    public void
    _write(org.omg.CORBA.portable.OutputStream out)
    {
        AttributeModeHelper.write(out, value);
    }

    public org.omg.CORBA.TypeCode
    _type()
    {
        return AttributeModeHelper.type();
    }
}