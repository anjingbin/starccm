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
// IDL:Bank/DepositCompHomeExplicit:1.0
//
final public class DepositCompHomeExplicitHolder implements org.omg.CORBA.portable.Streamable
{
    public DepositCompHomeExplicit value;

    public
    DepositCompHomeExplicitHolder()
    {
    }

    public
    DepositCompHomeExplicitHolder(DepositCompHomeExplicit initial)
    {
        value = initial;
    }

    public void
    _read(org.omg.CORBA.portable.InputStream in)
    {
        value = DepositCompHomeExplicitHelper.read(in);
    }

    public void
    _write(org.omg.CORBA.portable.OutputStream out)
    {
        DepositCompHomeExplicitHelper.write(out, value);
    }

    public org.omg.CORBA.TypeCode
    _type()
    {
        return DepositCompHomeExplicitHelper.type();
    }
}
