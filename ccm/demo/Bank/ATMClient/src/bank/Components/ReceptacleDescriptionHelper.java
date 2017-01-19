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
// IDL:omg.org/Components/ReceptacleDescription:1.0
//
final public class ReceptacleDescriptionHelper
{
    public static void
    insert(org.omg.CORBA.Any any, ReceptacleDescription val)
    {
        any.insert_Value(val, type());
    }

    public static ReceptacleDescription
    extract(org.omg.CORBA.Any any)
    {
        if(any.type().equivalent(type()))
        {
            java.io.Serializable _star_v = any.extract_Value();
            if(_star_v == null || _star_v instanceof ReceptacleDescription)
                return (ReceptacleDescription)_star_v;
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
            org.omg.CORBA.ValueMember[] members = new org.omg.CORBA.ValueMember[2];

            members[0] = new org.omg.CORBA.ValueMember();
            members[0].name = "is_multiple";
            members[0].type = orb.get_primitive_tc(org.omg.CORBA.TCKind.tk_boolean);
            members[0].access = org.omg.CORBA.PUBLIC_MEMBER.value;

            members[1] = new org.omg.CORBA.ValueMember();
            members[1].name = "connections";
            members[1].type = ConnectionDescriptionsHelper.type();
            members[1].access = org.omg.CORBA.PUBLIC_MEMBER.value;

            org.omg.CORBA.TypeCode baseType = PortDescriptionHelper.type();

            typeCode_ = orb.create_value_tc(id(), "ReceptacleDescription", org.omg.CORBA.VM_NONE.value, baseType, members);
        }

        return typeCode_;
    }

    public static String
    id()
    {
        return "IDL:omg.org/Components/ReceptacleDescription:1.0";
    }

    public static ReceptacleDescription
    read(org.omg.CORBA.portable.InputStream in)
    {
        if(!(in instanceof org.omg.CORBA_2_3.portable.InputStream))
            throw new org.omg.CORBA.BAD_PARAM();
        return (ReceptacleDescription)((org.omg.CORBA_2_3.portable.InputStream)in).read_value(id());
    }

    public static void
    write(org.omg.CORBA.portable.OutputStream out, ReceptacleDescription val)
    {
        if(!(out instanceof org.omg.CORBA_2_3.portable.OutputStream))
            throw new org.omg.CORBA.BAD_PARAM();
        ((org.omg.CORBA_2_3.portable.OutputStream)out).write_value(val, id());
    }
}
