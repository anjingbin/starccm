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
// IDL:omg.org/CORBA/AttributeMode:1.0
//
final public class AttributeModeHelper
{
    public static void
    insert(org.omg.CORBA.Any any, AttributeMode val)
    {
        org.omg.CORBA.portable.OutputStream out = any.create_output_stream();
        write(out, val);
        any.read_value(out.create_input_stream(), type());
    }

    public static AttributeMode
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
            String[] members = new String[2];
            members[0] = "ATTR_NORMAL";
            members[1] = "ATTR_READONLY";
            typeCode_ = orb.create_enum_tc(id(), "AttributeMode", members);
        }

        return typeCode_;
    }

    public static String
    id()
    {
        return "IDL:omg.org/CORBA/AttributeMode:1.0";
    }

    public static AttributeMode
    read(org.omg.CORBA.portable.InputStream in)
    {
        AttributeMode _star_v;
        _star_v = AttributeMode.from_int(in.read_ulong());
        return _star_v;
    }

    public static void
    write(org.omg.CORBA.portable.OutputStream out, AttributeMode val)
    {
        out.write_ulong(val.value());
    }
}
