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

package bank.CORBA.ContainerPackage;

//
// IDL:omg.org/CORBA/Container/DescriptionSeq:1.0
//
final public class DescriptionSeqHolder implements org.omg.CORBA.portable.Streamable
{
    public Description[] value;

    public
    DescriptionSeqHolder()
    {
    }

    public
    DescriptionSeqHolder(Description[] initial)
    {
        value = initial;
    }

    public void
    _read(org.omg.CORBA.portable.InputStream in)
    {
        value = DescriptionSeqHelper.read(in);
    }

    public void
    _write(org.omg.CORBA.portable.OutputStream out)
    {
        DescriptionSeqHelper.write(out, value);
    }

    public org.omg.CORBA.TypeCode
    _type()
    {
        return DescriptionSeqHelper.type();
    }
}