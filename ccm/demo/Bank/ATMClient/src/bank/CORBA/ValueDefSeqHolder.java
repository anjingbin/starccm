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
// IDL:omg.org/CORBA/ValueDefSeq:1.0
//
final public class ValueDefSeqHolder implements org.omg.CORBA.portable.Streamable
{
    public ValueDef[] value;

    public
    ValueDefSeqHolder()
    {
    }

    public
    ValueDefSeqHolder(ValueDef[] initial)
    {
        value = initial;
    }

    public void
    _read(org.omg.CORBA.portable.InputStream in)
    {
        value = ValueDefSeqHelper.read(in);
    }

    public void
    _write(org.omg.CORBA.portable.OutputStream out)
    {
        ValueDefSeqHelper.write(out, value);
    }

    public org.omg.CORBA.TypeCode
    _type()
    {
        return ValueDefSeqHelper.type();
    }
}
