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
// IDL:Bank/WithdrawComp:1.0
//
final public class WithdrawCompHolder implements org.omg.CORBA.portable.Streamable
{
    public WithdrawComp value;

    public
    WithdrawCompHolder()
    {
    }

    public
    WithdrawCompHolder(WithdrawComp initial)
    {
        value = initial;
    }

    public void
    _read(org.omg.CORBA.portable.InputStream in)
    {
        value = WithdrawCompHelper.read(in);
    }

    public void
    _write(org.omg.CORBA.portable.OutputStream out)
    {
        WithdrawCompHelper.write(out, value);
    }

    public org.omg.CORBA.TypeCode
    _type()
    {
        return WithdrawCompHelper.type();
    }
}
