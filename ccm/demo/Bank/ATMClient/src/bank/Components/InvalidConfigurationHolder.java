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
// IDL:omg.org/Components/InvalidConfiguration:1.0
//
final public class InvalidConfigurationHolder implements org.omg.CORBA.portable.Streamable
{
    public InvalidConfiguration value;

    public
    InvalidConfigurationHolder()
    {
    }

    public
    InvalidConfigurationHolder(InvalidConfiguration initial)
    {
        value = initial;
    }

    public void
    _read(org.omg.CORBA.portable.InputStream in)
    {
        value = InvalidConfigurationHelper.read(in);
    }

    public void
    _write(org.omg.CORBA.portable.OutputStream out)
    {
        InvalidConfigurationHelper.write(out, value);
    }

    public org.omg.CORBA.TypeCode
    _type()
    {
        return InvalidConfigurationHelper.type();
    }
}