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

package bank.CORBA.ValueDefPackage;

//
// IDL:omg.org/CORBA/ValueDef/FullValueDescription:1.0
//
final public class FullValueDescriptionHolder implements org.omg.CORBA.portable.Streamable
{
    public FullValueDescription value;

    public
    FullValueDescriptionHolder()
    {
    }

    public
    FullValueDescriptionHolder(FullValueDescription initial)
    {
        value = initial;
    }

    public void
    _read(org.omg.CORBA.portable.InputStream in)
    {
        value = FullValueDescriptionHelper.read(in);
    }

    public void
    _write(org.omg.CORBA.portable.OutputStream out)
    {
        FullValueDescriptionHelper.write(out, value);
    }

    public org.omg.CORBA.TypeCode
    _type()
    {
        return FullValueDescriptionHelper.type();
    }
}
