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
// IDL:omg.org/Components/ConnectionDescription:1.0
//
/***/

public abstract class ConnectionDescription implements org.omg.CORBA.portable.StreamableValue
{
    //
    // IDL:omg.org/Components/ConnectionDescription/ck:1.0
    //
    /***/

    public Cookie ck;

    //
    // IDL:omg.org/Components/ConnectionDescription/objref:1.0
    //
    /***/

    public org.omg.CORBA.Object objref;

    private static String[] _STAR_truncatableIds_ =
    {
        ConnectionDescriptionHelper.id()
    };

    public String[]
    _truncatable_ids()
    {
        return _STAR_truncatableIds_;
    }

    public void
    _read(org.omg.CORBA.portable.InputStream in)
    {
        ck = CookieHelper.read(in);
        objref = in.read_Object();
    }

    public void
    _write(org.omg.CORBA.portable.OutputStream out)
    {
        CookieHelper.write(out, ck);
        out.write_Object(objref);
    }

    public org.omg.CORBA.TypeCode
    _type()
    {
        return ConnectionDescriptionHelper.type();
    }
}