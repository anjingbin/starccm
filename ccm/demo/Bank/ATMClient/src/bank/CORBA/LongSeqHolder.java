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
// IDL:omg.org/CORBA/LongSeq:1.0
//
final public class LongSeqHolder implements org.omg.CORBA.portable.Streamable
{
    public int[] value;

    public
    LongSeqHolder()
    {
    }

    public
    LongSeqHolder(int[] initial)
    {
        value = initial;
    }

    public void
    _read(org.omg.CORBA.portable.InputStream in)
    {
        value = LongSeqHelper.read(in);
    }

    public void
    _write(org.omg.CORBA.portable.OutputStream out)
    {
        LongSeqHelper.write(out, value);
    }

    public org.omg.CORBA.TypeCode
    _type()
    {
        return LongSeqHelper.type();
    }
}
