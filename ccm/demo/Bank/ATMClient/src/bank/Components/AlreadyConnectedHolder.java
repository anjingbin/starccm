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
// IDL:omg.org/Components/AlreadyConnected:1.0
//
final public class AlreadyConnectedHolder implements org.omg.CORBA.portable.Streamable
{
    public AlreadyConnected value;

    public
    AlreadyConnectedHolder()
    {
    }

    public
    AlreadyConnectedHolder(AlreadyConnected initial)
    {
        value = initial;
    }

    public void
    _read(org.omg.CORBA.portable.InputStream in)
    {
        value = AlreadyConnectedHelper.read(in);
    }

    public void
    _write(org.omg.CORBA.portable.OutputStream out)
    {
        AlreadyConnectedHelper.write(out, value);
    }

    public org.omg.CORBA.TypeCode
    _type()
    {
        return AlreadyConnectedHelper.type();
    }
}
