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
// IDL:omg.org/CORBA/ConstantDef:1.0
//
final public class ConstantDefHolder implements org.omg.CORBA.portable.Streamable
{
    public ConstantDef value;

    public
    ConstantDefHolder()
    {
    }

    public
    ConstantDefHolder(ConstantDef initial)
    {
        value = initial;
    }

    public void
    _read(org.omg.CORBA.portable.InputStream in)
    {
        value = ConstantDefHelper.read(in);
    }

    public void
    _write(org.omg.CORBA.portable.OutputStream out)
    {
        ConstantDefHelper.write(out, value);
    }

    public org.omg.CORBA.TypeCode
    _type()
    {
        return ConstantDefHelper.type();
    }
}
