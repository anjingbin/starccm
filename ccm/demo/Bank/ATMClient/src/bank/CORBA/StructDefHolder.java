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
// IDL:omg.org/CORBA/StructDef:1.0
//
final public class StructDefHolder implements org.omg.CORBA.portable.Streamable
{
    public StructDef value;

    public
    StructDefHolder()
    {
    }

    public
    StructDefHolder(StructDef initial)
    {
        value = initial;
    }

    public void
    _read(org.omg.CORBA.portable.InputStream in)
    {
        value = StructDefHelper.read(in);
    }

    public void
    _write(org.omg.CORBA.portable.OutputStream out)
    {
        StructDefHelper.write(out, value);
    }

    public org.omg.CORBA.TypeCode
    _type()
    {
        return StructDefHelper.type();
    }
}
