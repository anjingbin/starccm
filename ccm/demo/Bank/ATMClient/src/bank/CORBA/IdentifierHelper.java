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
// IDL:omg.org/CORBA/Identifier:1.0
//
final public class IdentifierHelper
{
    public static void
    insert(org.omg.CORBA.Any any, String val)
    {
        org.omg.CORBA.portable.OutputStream out = any.create_output_stream();
        write(out, val);
        any.read_value(out.create_input_stream(), type());
    }

    public static String
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
            typeCode_ = orb.create_alias_tc(id(), "Identifier", orb.get_primitive_tc(org.omg.CORBA.TCKind.tk_string));
        }

        return typeCode_;
    }

    public static String
    id()
    {
        return "IDL:omg.org/CORBA/Identifier:1.0";
    }

    public static String
    read(org.omg.CORBA.portable.InputStream in)
    {
        String _star_v;
        _star_v = in.read_string();
        return _star_v;
    }

    public static void
    write(org.omg.CORBA.portable.OutputStream out, String val)
    {
        out.write_string(val);
    }
}
