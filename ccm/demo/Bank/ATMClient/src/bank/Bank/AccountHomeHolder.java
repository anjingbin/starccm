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
// IDL:Bank/AccountHome:1.0
//
final public class AccountHomeHolder implements org.omg.CORBA.portable.Streamable
{
    public AccountHome value;

    public
    AccountHomeHolder()
    {
    }

    public
    AccountHomeHolder(AccountHome initial)
    {
        value = initial;
    }

    public void
    _read(org.omg.CORBA.portable.InputStream in)
    {
        value = AccountHomeHelper.read(in);
    }

    public void
    _write(org.omg.CORBA.portable.OutputStream out)
    {
        AccountHomeHelper.write(out, value);
    }

    public org.omg.CORBA.TypeCode
    _type()
    {
        return AccountHomeHelper.type();
    }
}
