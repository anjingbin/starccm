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
// IDL:Bank/WithdrawCompHomeImplicit:1.0
//
final public class WithdrawCompHomeImplicitHolder implements org.omg.CORBA.portable.Streamable
{
    public WithdrawCompHomeImplicit value;

    public
    WithdrawCompHomeImplicitHolder()
    {
    }

    public
    WithdrawCompHomeImplicitHolder(WithdrawCompHomeImplicit initial)
    {
        value = initial;
    }

    public void
    _read(org.omg.CORBA.portable.InputStream in)
    {
        value = WithdrawCompHomeImplicitHelper.read(in);
    }

    public void
    _write(org.omg.CORBA.portable.OutputStream out)
    {
        WithdrawCompHomeImplicitHelper.write(out, value);
    }

    public org.omg.CORBA.TypeCode
    _type()
    {
        return WithdrawCompHomeImplicitHelper.type();
    }
}