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
// IDL:omg.org/CORBA/ArrayDef:1.0
//
final public class ArrayDefHolder implements org.omg.CORBA.portable.Streamable
{
    public ArrayDef value;

    public
    ArrayDefHolder()
    {
    }

    public
    ArrayDefHolder(ArrayDef initial)
    {
        value = initial;
    }

    public void
    _read(org.omg.CORBA.portable.InputStream in)
    {
        value = ArrayDefHelper.read(in);
    }

    public void
    _write(org.omg.CORBA.portable.OutputStream out)
    {
        ArrayDefHelper.write(out, value);
    }

    public org.omg.CORBA.TypeCode
    _type()
    {
        return ArrayDefHelper.type();
    }
}