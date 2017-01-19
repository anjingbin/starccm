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
// IDL:omg.org/Components/SubscriberDescriptions:1.0
//
final public class SubscriberDescriptionsHelper
{
    public static void
    insert(org.omg.CORBA.Any any, SubscriberDescription[] val)
    {
        org.omg.CORBA.portable.OutputStream out = any.create_output_stream();
        write(out, val);
        any.read_value(out.create_input_stream(), type());
    }

    public static SubscriberDescription[]
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
            typeCode_ = orb.create_alias_tc(id(), "SubscriberDescriptions", orb.create_sequence_tc(0, SubscriberDescriptionHelper.type()));
        }

        return typeCode_;
    }

    public static String
    id()
    {
        return "IDL:omg.org/Components/SubscriberDescriptions:1.0";
    }

    public static SubscriberDescription[]
    read(org.omg.CORBA.portable.InputStream in)
    {
        SubscriberDescription[] _star_v;
        int len0 = in.read_ulong();
        _star_v = new SubscriberDescription[len0];
        for(int i0 = 0 ; i0 < len0 ; i0++)
            _star_v[i0] = SubscriberDescriptionHelper.read(in);
        return _star_v;
    }

    public static void
    write(org.omg.CORBA.portable.OutputStream out, SubscriberDescription[] val)
    {
        int len0 = val.length;
        out.write_ulong(len0);
        for(int i0 = 0 ; i0 < len0 ; i0++)
            SubscriberDescriptionHelper.write(out, val[i0]);
    }
}
