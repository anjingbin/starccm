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

package bank.Bank;

//
// IDL:Bank/DepositComp:1.0
//
final public class DepositCompHolder implements org.omg.CORBA.portable.Streamable
{
    public DepositComp value;

    public
    DepositCompHolder()
    {
    }

    public
    DepositCompHolder(DepositComp initial)
    {
        value = initial;
    }

    public void
    _read(org.omg.CORBA.portable.InputStream in)
    {
        value = DepositCompHelper.read(in);
    }

    public void
    _write(org.omg.CORBA.portable.OutputStream out)
    {
        DepositCompHelper.write(out, value);
    }

    public org.omg.CORBA.TypeCode
    _type()
    {
        return DepositCompHelper.type();
    }
}