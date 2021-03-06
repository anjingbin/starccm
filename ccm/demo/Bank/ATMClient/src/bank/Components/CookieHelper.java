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
// IDL:omg.org/Components/Cookie:1.0
//
final public class CookieHelper
{
    public static void
    insert(org.omg.CORBA.Any any, Cookie val)
    {
        any.insert_Value(val, type());
    }

    public static Cookie
    extract(org.omg.CORBA.Any any)
    {
        if(any.type().equivalent(type()))
        {
            java.io.Serializable _star_v = any.extract_Value();
            if(_star_v == null || _star_v instanceof Cookie)
                return (Cookie)_star_v;
        }

        throw new org.omg.CORBA.BAD_OPERATION();
    }

    private static org.omg.CORBA.TypeCode typeCode_;

    public static org.omg.CORBA.TypeCode
    type()
    {
        if(typeCode_ == null)
        {
            org.omg.CORBA.ORB orb = org.omg.CORBA.ORB.init();
            org.omg.CORBA.ValueMember[] members = new org.omg.CORBA.ValueMember[1];

            members[0] = new org.omg.CORBA.ValueMember();
            members[0].name = "cookieValue";
            members[0].type = bank.CORBA.OctetSeqHelper.type();
            members[0].access = org.omg.CORBA.PRIVATE_MEMBER.value;

            typeCode_ = orb.create_value_tc(id(), "Cookie", org.omg.CORBA.VM_NONE.value, null, members);
        }

        return typeCode_;
    }

    public static String
    id()
    {
        return "IDL:omg.org/Components/Cookie:1.0";
    }

    public static Cookie
    read(org.omg.CORBA.portable.InputStream in)
    {
        if(!(in instanceof org.omg.CORBA_2_3.portable.InputStream))
            throw new org.omg.CORBA.BAD_PARAM();
        return (Cookie)((org.omg.CORBA_2_3.portable.InputStream)in).read_value(id());
    }

    public static void
    write(org.omg.CORBA.portable.OutputStream out, Cookie val)
    {
        if(!(out instanceof org.omg.CORBA_2_3.portable.OutputStream))
            throw new org.omg.CORBA.BAD_PARAM();
        ((org.omg.CORBA_2_3.portable.OutputStream)out).write_value(val, id());
    }
}
