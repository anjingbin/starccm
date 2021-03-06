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
// IDL:omg.org/CORBA/SequenceDef:1.0
//
final public class SequenceDefHelper
{
    public static void
    insert(org.omg.CORBA.Any any, SequenceDef val)
    {
        any.insert_Object(val, type());
    }

    public static SequenceDef
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
            typeCode_ = orb.create_interface_tc(id(), "SequenceDef");
        }

        return typeCode_;
    }

    public static String
    id()
    {
        return "IDL:omg.org/CORBA/SequenceDef:1.0";
    }

    public static SequenceDef
    read(org.omg.CORBA.portable.InputStream in)
    {
        org.omg.CORBA.Object _star_v = in.read_Object();

        try
        {
            return (SequenceDef)_star_v;
        }
        catch(ClassCastException ex)
        {
        }

        org.omg.CORBA.portable.ObjectImpl _star_impl;
        _star_impl = (org.omg.CORBA.portable.ObjectImpl)_star_v;
        _SequenceDefStub _star_stub = new _SequenceDefStub();
        _star_stub._set_delegate(_star_impl._get_delegate());
        return _star_stub;
    }

    public static void
    write(org.omg.CORBA.portable.OutputStream out, SequenceDef val)
    {
        out.write_Object(val);
    }

    public static SequenceDef
    narrow(org.omg.CORBA.Object val)
    {
        if(val != null)
        {
            try
            {
                return (SequenceDef)val;
            }
            catch(ClassCastException ex)
            {
            }

            if(val._is_a(id()))
            {
                org.omg.CORBA.portable.ObjectImpl _star_impl;
                _SequenceDefStub _star_stub = new _SequenceDefStub();
                _star_impl = (org.omg.CORBA.portable.ObjectImpl)val;
                _star_stub._set_delegate(_star_impl._get_delegate());
                return _star_stub;
            }

            throw new org.omg.CORBA.BAD_PARAM();
        }

        return null;
    }

    public static SequenceDef
    unchecked_narrow(org.omg.CORBA.Object val)
    {
        if(val != null)
        {
            try
            {
                return (SequenceDef)val;
            }
            catch(ClassCastException ex)
            {
            }

            org.omg.CORBA.portable.ObjectImpl _star_impl;
            _SequenceDefStub _star_stub = new _SequenceDefStub();
            _star_impl = (org.omg.CORBA.portable.ObjectImpl)val;
            _star_stub._set_delegate(_star_impl._get_delegate());
            return _star_stub;
        }

        return null;
    }
}
