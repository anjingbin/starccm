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
// IDL:omg.org/Components/ComponentPortDescription:1.0
//
final public class ComponentPortDescriptionHelper
{
    public static void
    insert(org.omg.CORBA.Any any, ComponentPortDescription val)
    {
        any.insert_Value(val, type());
    }

    public static ComponentPortDescription
    extract(org.omg.CORBA.Any any)
    {
        if(any.type().equivalent(type()))
        {
            java.io.Serializable _star_v = any.extract_Value();
            if(_star_v == null || _star_v instanceof ComponentPortDescription)
                return (ComponentPortDescription)_star_v;
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
            org.omg.CORBA.ValueMember[] members = new org.omg.CORBA.ValueMember[5];

            members[0] = new org.omg.CORBA.ValueMember();
            members[0].name = "facets";
            members[0].type = FacetDescriptionsHelper.type();
            members[0].access = org.omg.CORBA.PUBLIC_MEMBER.value;

            members[1] = new org.omg.CORBA.ValueMember();
            members[1].name = "receptacles";
            members[1].type = ReceptacleDescriptionsHelper.type();
            members[1].access = org.omg.CORBA.PUBLIC_MEMBER.value;

            members[2] = new org.omg.CORBA.ValueMember();
            members[2].name = "consumers";
            members[2].type = ConsumerDescriptionsHelper.type();
            members[2].access = org.omg.CORBA.PUBLIC_MEMBER.value;

            members[3] = new org.omg.CORBA.ValueMember();
            members[3].name = "emitters";
            members[3].type = EmitterDescriptionsHelper.type();
            members[3].access = org.omg.CORBA.PUBLIC_MEMBER.value;

            members[4] = new org.omg.CORBA.ValueMember();
            members[4].name = "publishers";
            members[4].type = PublisherDescriptionsHelper.type();
            members[4].access = org.omg.CORBA.PUBLIC_MEMBER.value;

            typeCode_ = orb.create_value_tc(id(), "ComponentPortDescription", org.omg.CORBA.VM_NONE.value, null, members);
        }

        return typeCode_;
    }

    public static String
    id()
    {
        return "IDL:omg.org/Components/ComponentPortDescription:1.0";
    }

    public static ComponentPortDescription
    read(org.omg.CORBA.portable.InputStream in)
    {
        if(!(in instanceof org.omg.CORBA_2_3.portable.InputStream))
            throw new org.omg.CORBA.BAD_PARAM();
        return (ComponentPortDescription)((org.omg.CORBA_2_3.portable.InputStream)in).read_value(id());
    }

    public static void
    write(org.omg.CORBA.portable.OutputStream out, ComponentPortDescription val)
    {
        if(!(out instanceof org.omg.CORBA_2_3.portable.OutputStream))
            throw new org.omg.CORBA.BAD_PARAM();
        ((org.omg.CORBA_2_3.portable.OutputStream)out).write_value(val, id());
    }
}
