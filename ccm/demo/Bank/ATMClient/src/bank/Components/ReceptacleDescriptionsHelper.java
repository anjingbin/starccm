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
// IDL:omg.org/Components/ReceptacleDescriptions:1.0
//
final public class ReceptacleDescriptionsHelper
{
    public static void
    insert(org.omg.CORBA.Any any, ReceptacleDescription[] val)
    {
        org.omg.CORBA.portable.OutputStream out = any.create_output_stream();
        write(out, val);
        any.read_value(out.create_input_stream(), type());
    }

    public static ReceptacleDescription[]
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
            typeCode_ = orb.create_alias_tc(id(), "ReceptacleDescriptions", orb.create_sequence_tc(0, ReceptacleDescriptionHelper.type()));
        }

        return typeCode_;
    }

    public static String
    id()
    {
        return "IDL:omg.org/Components/ReceptacleDescriptions:1.0";
    }

    public static ReceptacleDescription[]
    read(org.omg.CORBA.portable.InputStream in)
    {
        ReceptacleDescription[] _star_v;
        int len0 = in.read_ulong();
        _star_v = new ReceptacleDescription[len0];
        for(int i0 = 0 ; i0 < len0 ; i0++)
            _star_v[i0] = ReceptacleDescriptionHelper.read(in);
        return _star_v;
    }

    public static void
    write(org.omg.CORBA.portable.OutputStream out, ReceptacleDescription[] val)
    {
        int len0 = val.length;
        out.write_ulong(len0);
        for(int i0 = 0 ; i0 < len0 ; i0++)
            ReceptacleDescriptionHelper.write(out, val[i0]);
    }
}
