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
// IDL:omg.org/CORBA/UnionMemberSeq:1.0
//
final public class UnionMemberSeqHolder implements org.omg.CORBA.portable.Streamable
{
    public UnionMember[] value;

    public
    UnionMemberSeqHolder()
    {
    }

    public
    UnionMemberSeqHolder(UnionMember[] initial)
    {
        value = initial;
    }

    public void
    _read(org.omg.CORBA.portable.InputStream in)
    {
        value = UnionMemberSeqHelper.read(in);
    }

    public void
    _write(org.omg.CORBA.portable.OutputStream out)
    {
        UnionMemberSeqHelper.write(out, value);
    }

    public org.omg.CORBA.TypeCode
    _type()
    {
        return UnionMemberSeqHelper.type();
    }
}
