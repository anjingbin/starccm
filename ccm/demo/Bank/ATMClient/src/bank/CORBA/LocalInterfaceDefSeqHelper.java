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
// IDL:omg.org/CORBA/LocalInterfaceDefSeq:1.0
//
final public class LocalInterfaceDefSeqHelper
{
    public static void
    insert(org.omg.CORBA.Any any, LocalInterfaceDef[] val)
    {
        org.omg.CORBA.portable.OutputStream out = any.create_output_stream();
        write(out, val);
        any.read_value(out.create_input_stream(), type());
    }

    public static LocalInterfaceDef[]
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
            typeCode_ = orb.create_alias_tc(id(), "LocalInterfaceDefSeq", orb.create_sequence_tc(0, LocalInterfaceDefHelper.type()));
        }

        return typeCode_;
    }

    public static String
    id()
    {
        return "IDL:omg.org/CORBA/LocalInterfaceDefSeq:1.0";
    }

    public static LocalInterfaceDef[]
    read(org.omg.CORBA.portable.InputStream in)
    {
        LocalInterfaceDef[] _star_v;
        int len0 = in.read_ulong();
        _star_v = new LocalInterfaceDef[len0];
        for(int i0 = 0 ; i0 < len0 ; i0++)
            _star_v[i0] = LocalInterfaceDefHelper.read(in);
        return _star_v;
    }

    public static void
    write(org.omg.CORBA.portable.OutputStream out, LocalInterfaceDef[] val)
    {
        int len0 = val.length;
        out.write_ulong(len0);
        for(int i0 = 0 ; i0 < len0 ; i0++)
            LocalInterfaceDefHelper.write(out, val[i0]);
    }
}