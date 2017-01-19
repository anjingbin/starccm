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
// IDL:omg.org/CORBA/Initializer:1.0
//
final public class InitializerHelper
{
    public static void
    insert(org.omg.CORBA.Any any, Initializer val)
    {
        org.omg.CORBA.portable.OutputStream out = any.create_output_stream();
        write(out, val);
        any.read_value(out.create_input_stream(), type());
    }

    public static Initializer
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
            org.omg.CORBA.StructMember[] members = new org.omg.CORBA.StructMember[2];

            members[0] = new org.omg.CORBA.StructMember();
            members[0].name = "members";
            members[0].type = StructMemberSeqHelper.type();

            members[1] = new org.omg.CORBA.StructMember();
            members[1].name = "name";
            members[1].type = IdentifierHelper.type();

            typeCode_ = orb.create_struct_tc(id(), "Initializer", members);
        }

        return typeCode_;
    }

    public static String
    id()
    {
        return "IDL:omg.org/CORBA/Initializer:1.0";
    }

    public static Initializer
    read(org.omg.CORBA.portable.InputStream in)
    {
        Initializer _star_v = new Initializer();
        _star_v.members = StructMemberSeqHelper.read(in);
        _star_v.name = IdentifierHelper.read(in);
        return _star_v;
    }

    public static void
    write(org.omg.CORBA.portable.OutputStream out, Initializer val)
    {
        StructMemberSeqHelper.write(out, val.members);
        IdentifierHelper.write(out, val.name);
    }
}