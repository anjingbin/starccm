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
// IDL:omg.org/CORBA/ValueMemberDef:1.0
//
final public class ValueMemberDefHolder implements org.omg.CORBA.portable.Streamable
{
    public ValueMemberDef value;

    public
    ValueMemberDefHolder()
    {
    }

    public
    ValueMemberDefHolder(ValueMemberDef initial)
    {
        value = initial;
    }

    public void
    _read(org.omg.CORBA.portable.InputStream in)
    {
        value = ValueMemberDefHelper.read(in);
    }

    public void
    _write(org.omg.CORBA.portable.OutputStream out)
    {
        ValueMemberDefHelper.write(out, value);
    }

    public org.omg.CORBA.TypeCode
    _type()
    {
        return ValueMemberDefHelper.type();
    }
}
