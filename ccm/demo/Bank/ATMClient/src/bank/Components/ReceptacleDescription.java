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
/***/

public abstract class ReceptacleDescription extends PortDescription
{
    //
    // IDL:omg.org/Components/ReceptacleDescription/is_multiple:1.0
    //
    /***/

    public boolean is_multiple;

    //
    // IDL:omg.org/Components/ReceptacleDescription/connections:1.0
    //
    /***/

    public ConnectionDescription[] connections;

    private static String[] _STAR_truncatableIds_ =
    {
        ReceptacleDescriptionHelper.id()
    };

    public String[]
    _truncatable_ids()
    {
        return _STAR_truncatableIds_;
    }

    public void
    _read(org.omg.CORBA.portable.InputStream in)
    {
        super._read(in);
        is_multiple = in.read_boolean();
        connections = ConnectionDescriptionsHelper.read(in);
    }

    public void
    _write(org.omg.CORBA.portable.OutputStream out)
    {
        super._write(out);
        out.write_boolean(is_multiple);
        ConnectionDescriptionsHelper.write(out, connections);
    }

    public org.omg.CORBA.TypeCode
    _type()
    {
        return ReceptacleDescriptionHelper.type();
    }
}
