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
// IDL:omg.org/Components/CCMExceptionReason:1.0
//
final public class CCMExceptionReasonHelper
{
    public static void
    insert(org.omg.CORBA.Any any, CCMExceptionReason val)
    {
        org.omg.CORBA.portable.OutputStream out = any.create_output_stream();
        write(out, val);
        any.read_value(out.create_input_stream(), type());
    }

    public static CCMExceptionReason
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
            String[] members = new String[7];
            members[0] = "SYSTEM_ERROR";
            members[1] = "CREATE_ERROR";
            members[2] = "REMOVE_ERROR";
            members[3] = "DUPLICATE_KEY";
            members[4] = "FIND_ERROR";
            members[5] = "OBJECT_NOT_FOUND";
            members[6] = "NO_SUCH_ENTITY";
            typeCode_ = orb.create_enum_tc(id(), "CCMExceptionReason", members);
        }

        return typeCode_;
    }

    public static String
    id()
    {
        return "IDL:omg.org/Components/CCMExceptionReason:1.0";
    }

    public static CCMExceptionReason
    read(org.omg.CORBA.portable.InputStream in)
    {
        CCMExceptionReason _star_v;
        _star_v = CCMExceptionReason.from_int(in.read_ulong());
        return _star_v;
    }

    public static void
    write(org.omg.CORBA.portable.OutputStream out, CCMExceptionReason val)
    {
        out.write_ulong(val.value());
    }
}
