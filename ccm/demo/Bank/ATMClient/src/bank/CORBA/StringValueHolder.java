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
// IDL:omg.org/CORBA/StringValue:1.0
//
final public class StringValueHolder implements org.omg.CORBA.portable.Streamable
{
    public String value;

    public
    StringValueHolder()
    {
    }

    public
    StringValueHolder(String initial)
    {
        value = initial;
    }

    public void
    _read(org.omg.CORBA.portable.InputStream in)
    {
        value = StringValueHelper.read(in);
    }

    public void
    _write(org.omg.CORBA.portable.OutputStream out)
    {
        StringValueHelper.write(out, value);
    }

    public org.omg.CORBA.TypeCode
    _type()
    {
        return StringValueHelper.type();
    }
}