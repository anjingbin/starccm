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
// IDL:omg.org/Components/InvalidName:1.0
//
final public class InvalidNameHolder implements org.omg.CORBA.portable.Streamable
{
    public InvalidName value;

    public
    InvalidNameHolder()
    {
    }

    public
    InvalidNameHolder(InvalidName initial)
    {
        value = initial;
    }

    public void
    _read(org.omg.CORBA.portable.InputStream in)
    {
        value = InvalidNameHelper.read(in);
    }

    public void
    _write(org.omg.CORBA.portable.OutputStream out)
    {
        InvalidNameHelper.write(out, value);
    }

    public org.omg.CORBA.TypeCode
    _type()
    {
        return InvalidNameHelper.type();
    }
}
