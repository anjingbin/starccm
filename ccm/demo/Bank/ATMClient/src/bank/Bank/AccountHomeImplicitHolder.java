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
// IDL:Bank/AccountHomeImplicit:1.0
//
final public class AccountHomeImplicitHolder implements org.omg.CORBA.portable.Streamable
{
    public AccountHomeImplicit value;

    public
    AccountHomeImplicitHolder()
    {
    }

    public
    AccountHomeImplicitHolder(AccountHomeImplicit initial)
    {
        value = initial;
    }

    public void
    _read(org.omg.CORBA.portable.InputStream in)
    {
        value = AccountHomeImplicitHelper.read(in);
    }

    public void
    _write(org.omg.CORBA.portable.OutputStream out)
    {
        AccountHomeImplicitHelper.write(out, value);
    }

    public org.omg.CORBA.TypeCode
    _type()
    {
        return AccountHomeImplicitHelper.type();
    }
}
