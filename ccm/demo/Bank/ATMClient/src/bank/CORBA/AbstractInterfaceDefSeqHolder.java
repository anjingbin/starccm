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
// IDL:omg.org/CORBA/AbstractInterfaceDefSeq:1.0
//
final public class AbstractInterfaceDefSeqHolder implements org.omg.CORBA.portable.Streamable
{
    public AbstractInterfaceDef[] value;

    public
    AbstractInterfaceDefSeqHolder()
    {
    }

    public
    AbstractInterfaceDefSeqHolder(AbstractInterfaceDef[] initial)
    {
        value = initial;
    }

    public void
    _read(org.omg.CORBA.portable.InputStream in)
    {
        value = AbstractInterfaceDefSeqHelper.read(in);
    }

    public void
    _write(org.omg.CORBA.portable.OutputStream out)
    {
        AbstractInterfaceDefSeqHelper.write(out, value);
    }

    public org.omg.CORBA.TypeCode
    _type()
    {
        return AbstractInterfaceDefSeqHelper.type();
    }
}
