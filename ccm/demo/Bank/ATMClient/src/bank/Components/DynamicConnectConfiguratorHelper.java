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
// IDL:omg.org/Components/DynamicConnectConfigurator:1.0
//
final public class DynamicConnectConfiguratorHelper
{
    public static void
    insert(org.omg.CORBA.Any any, DynamicConnectConfigurator val)
    {
        any.insert_Object(val, type());
    }

    public static DynamicConnectConfigurator
    extract(org.omg.CORBA.Any any)
    {
        if(any.type().equivalent(type()))
            return narrow(any.extract_Object());

        throw new org.omg.CORBA.BAD_OPERATION();
    }

    private static org.omg.CORBA.TypeCode typeCode_;

    public static org.omg.CORBA.TypeCode
    type()
    {
        if(typeCode_ == null)
        {
            org.omg.CORBA.ORB orb = org.omg.CORBA.ORB.init();
            typeCode_ = orb.create_interface_tc(id(), "DynamicConnectConfigurator");
        }

        return typeCode_;
    }

    public static String
    id()
    {
        return "IDL:omg.org/Components/DynamicConnectConfigurator:1.0";
    }

    public static DynamicConnectConfigurator
    read(org.omg.CORBA.portable.InputStream in)
    {
        org.omg.CORBA.Object _star_v = in.read_Object();

        try
        {
            return (DynamicConnectConfigurator)_star_v;
        }
        catch(ClassCastException ex)
        {
        }

        org.omg.CORBA.portable.ObjectImpl _star_impl;
        _star_impl = (org.omg.CORBA.portable.ObjectImpl)_star_v;
        _DynamicConnectConfiguratorStub _star_stub = new _DynamicConnectConfiguratorStub();
        _star_stub._set_delegate(_star_impl._get_delegate());
        return _star_stub;
    }

    public static void
    write(org.omg.CORBA.portable.OutputStream out, DynamicConnectConfigurator val)
    {
        out.write_Object(val);
    }

    public static DynamicConnectConfigurator
    narrow(org.omg.CORBA.Object val)
    {
        if(val != null)
        {
            try
            {
                return (DynamicConnectConfigurator)val;
            }
            catch(ClassCastException ex)
            {
            }

            if(val._is_a(id()))
            {
                org.omg.CORBA.portable.ObjectImpl _star_impl;
                _DynamicConnectConfiguratorStub _star_stub = new _DynamicConnectConfiguratorStub();
                _star_impl = (org.omg.CORBA.portable.ObjectImpl)val;
                _star_stub._set_delegate(_star_impl._get_delegate());
                return _star_stub;
            }

            throw new org.omg.CORBA.BAD_PARAM();
        }

        return null;
    }

    public static DynamicConnectConfigurator
    unchecked_narrow(org.omg.CORBA.Object val)
    {
        if(val != null)
        {
            try
            {
                return (DynamicConnectConfigurator)val;
            }
            catch(ClassCastException ex)
            {
            }

            org.omg.CORBA.portable.ObjectImpl _star_impl;
            _DynamicConnectConfiguratorStub _star_stub = new _DynamicConnectConfiguratorStub();
            _star_impl = (org.omg.CORBA.portable.ObjectImpl)val;
            _star_stub._set_delegate(_star_impl._get_delegate());
            return _star_stub;
        }

        return null;
    }
}
