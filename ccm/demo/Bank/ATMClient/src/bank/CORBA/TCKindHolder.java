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
// IDL:omg.org/CORBA/TCKind:1.0
//
final public class TCKindHolder implements org.omg.CORBA.portable.Streamable
{
    public TCKind value;

    public
    TCKindHolder()
    {
    }

    public
    TCKindHolder(TCKind initial)
    {
        value = initial;
    }

    public void
    _read(org.omg.CORBA.portable.InputStream in)
    {
        value = TCKindHelper.read(in);
    }

    public void
    _write(org.omg.CORBA.portable.OutputStream out)
    {
        TCKindHelper.write(out, value);
    }

    public org.omg.CORBA.TypeCode
    _type()
    {
        return TCKindHelper.type();
    }
}