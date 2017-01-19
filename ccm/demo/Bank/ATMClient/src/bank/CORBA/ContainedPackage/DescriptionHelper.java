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

package bank.CORBA.ContainedPackage;

//
// IDL:omg.org/CORBA/Contained/Description:1.0
//
final public class DescriptionHelper
{
    public static void
    insert(org.omg.CORBA.Any any, Description val)
    {
        org.omg.CORBA.portable.OutputStream out = any.create_output_stream();
        write(out, val);
        any.read_value(out.create_input_stream(), type());
    }

    public static Description
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
            members[0].name = "kind";
            members[0].type = bank.CORBA.DefinitionKindHelper.type();

            members[1] = new org.omg.CORBA.StructMember();
            members[1].name = "value";
            members[1].type = orb.get_primitive_tc(org.omg.CORBA.TCKind.tk_any);

            typeCode_ = orb.create_struct_tc(id(), "Description", members);
        }

        return typeCode_;
    }

    public static String
    id()
    {
        return "IDL:omg.org/CORBA/Contained/Description:1.0";
    }

    public static Description
    read(org.omg.CORBA.portable.InputStream in)
    {
        Description _star_v = new Description();
        _star_v.kind = bank.CORBA.DefinitionKindHelper.read(in);
        _star_v.value = in.read_any();
        return _star_v;
    }

    public static void
    write(org.omg.CORBA.portable.OutputStream out, Description val)
    {
        bank.CORBA.DefinitionKindHelper.write(out, val.kind);
        out.write_any(val.value);
    }
}