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

package bank.Components;

//
// IDL:omg.org/Components/CreateFailure:1.0
//
final public class CreateFailureHolder implements org.omg.CORBA.portable.Streamable
{
    public CreateFailure value;

    public
    CreateFailureHolder()
    {
    }

    public
    CreateFailureHolder(CreateFailure initial)
    {
        value = initial;
    }

    public void
    _read(org.omg.CORBA.portable.InputStream in)
    {
        value = CreateFailureHelper.read(in);
    }

    public void
    _write(org.omg.CORBA.portable.OutputStream out)
    {
        CreateFailureHelper.write(out, value);
    }

    public org.omg.CORBA.TypeCode
    _type()
    {
        return CreateFailureHelper.type();
    }
}