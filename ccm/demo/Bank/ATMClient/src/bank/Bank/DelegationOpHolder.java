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
// IDL:Bank/DelegationOp:1.0
//
final public class DelegationOpHolder implements org.omg.CORBA.portable.Streamable
{
    public DelegationOp value;

    public
    DelegationOpHolder()
    {
    }

    public
    DelegationOpHolder(DelegationOp initial)
    {
        value = initial;
    }

    public void
    _read(org.omg.CORBA.portable.InputStream in)
    {
        value = DelegationOpHelper.read(in);
    }

    public void
    _write(org.omg.CORBA.portable.OutputStream out)
    {
        DelegationOpHelper.write(out, value);
    }

    public org.omg.CORBA.TypeCode
    _type()
    {
        return DelegationOpHelper.type();
    }
}
