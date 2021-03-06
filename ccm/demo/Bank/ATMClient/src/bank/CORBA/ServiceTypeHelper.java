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
// IDL:omg.org/CORBA/ServiceType:1.0
//
final public class ServiceTypeHelper
{
    public static void
    insert(org.omg.CORBA.Any any, short val)
    {
        org.omg.CORBA.portable.OutputStream out = any.create_output_stream();
        write(out, val);
        any.read_value(out.create_input_stream(), type());
    }

    public static short
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
            typeCode_ = orb.create_alias_tc(id(), "ServiceType", orb.get_primitive_tc(org.omg.CORBA.TCKind.tk_ushort));
        }

        return typeCode_;
    }

    public static String
    id()
    {
        return "IDL:omg.org/CORBA/ServiceType:1.0";
    }

    public static short
    read(org.omg.CORBA.portable.InputStream in)
    {
        short _star_v;
        _star_v = in.read_ushort();
        return _star_v;
    }

    public static void
    write(org.omg.CORBA.portable.OutputStream out, short val)
    {
        out.write_ushort(val);
    }
}
