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
// IDL:omg.org/Components/ConfigValue:1.0
//
/***/

public abstract class ConfigValue implements org.omg.CORBA.portable.StreamableValue
{
    //
    // IDL:omg.org/Components/ConfigValue/name:1.0
    //
    /***/

    public String name;

    //
    // IDL:omg.org/Components/ConfigValue/value:1.0
    //
    /***/

    public org.omg.CORBA.Any value;

    private static String[] _STAR_truncatableIds_ =
    {
        ConfigValueHelper.id()
    };

    public String[]
    _truncatable_ids()
    {
        return _STAR_truncatableIds_;
    }

    public void
    _read(org.omg.CORBA.portable.InputStream in)
    {
        name = FeatureNameHelper.read(in);
        value = in.read_any();
    }

    public void
    _write(org.omg.CORBA.portable.OutputStream out)
    {
        FeatureNameHelper.write(out, name);
        out.write_any(value);
    }

    public org.omg.CORBA.TypeCode
    _type()
    {
        return ConfigValueHelper.type();
    }
}