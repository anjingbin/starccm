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
// IDL:omg.org/Components/Configurator:1.0
//
final public class ConfiguratorHolder implements org.omg.CORBA.portable.Streamable
{
    public Configurator value;

    public
    ConfiguratorHolder()
    {
    }

    public
    ConfiguratorHolder(Configurator initial)
    {
        value = initial;
    }

    public void
    _read(org.omg.CORBA.portable.InputStream in)
    {
        value = ConfiguratorHelper.read(in);
    }

    public void
    _write(org.omg.CORBA.portable.OutputStream out)
    {
        ConfiguratorHelper.write(out, value);
    }

    public org.omg.CORBA.TypeCode
    _type()
    {
        return ConfiguratorHelper.type();
    }
}
