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
// IDL:omg.org/Components/ConsumerDescription:1.0
//
final public class ConsumerDescriptionHolder implements org.omg.CORBA.portable.Streamable
{
    public ConsumerDescription value;

    public
    ConsumerDescriptionHolder()
    {
    }

    public
    ConsumerDescriptionHolder(ConsumerDescription initial)
    {
        value = initial;
    }

    public void
    _read(org.omg.CORBA.portable.InputStream in)
    {
        value = ConsumerDescriptionHelper.read(in);
    }

    public void
    _write(org.omg.CORBA.portable.OutputStream out)
    {
        ConsumerDescriptionHelper.write(out, value);
    }

    public org.omg.CORBA.TypeCode
    _type()
    {
        return ConsumerDescriptionHelper.type();
    }
}