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
// IDL:omg.org/Components/CookieRequired:1.0
//
final public class CookieRequiredHelper
{
    public static void
    insert(org.omg.CORBA.Any any, CookieRequired val)
    {
        org.omg.CORBA.portable.OutputStream out = any.create_output_stream();
        write(out, val);
        any.read_value(out.create_input_stream(), type());
    }

    public static CookieRequired
    extract(org.omg.CORBA.Any any)
    {
        if(any.type().equivalent(type()))
            return read(any.create_input_stream());
        else
            throw new org.omg.CORBA.BAD_OPERATION();
    }

    private static org.omg.CORBA.TypeCode typeCode_;

    public static org.omg.CORBA.TypeCode
    type()
    {
        if(typeCode_ == null)
        {
            org.omg.CORBA.ORB orb = org.omg.CORBA.ORB.init();
            org.omg.CORBA.StructMember[] members = new org.omg.CORBA.StructMember[0];

            typeCode_ = orb.create_exception_tc(id(), "CookieRequired", members);
        }

        return typeCode_;
    }

    public static String
    id()
    {
        return "IDL:omg.org/Components/CookieRequired:1.0";
    }

    public static CookieRequired
    read(org.omg.CORBA.portable.InputStream in)
    {
        if(!id().equals(in.read_string()))
            throw new org.omg.CORBA.MARSHAL();

        CookieRequired _star_v = new CookieRequired();
        return _star_v;
    }

    public static void
    write(org.omg.CORBA.portable.OutputStream out, CookieRequired val)
    {
        out.write_string(id());
    }
}
