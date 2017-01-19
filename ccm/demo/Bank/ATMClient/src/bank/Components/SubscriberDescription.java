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
// IDL:omg.org/Components/SubscriberDescription:1.0
//
/***/

public abstract class SubscriberDescription implements org.omg.CORBA.portable.StreamableValue
{
    //
    // IDL:omg.org/Components/SubscriberDescription/ck:1.0
    //
    /***/

    public Cookie ck;

    //
    // IDL:omg.org/Components/SubscriberDescription/consumer:1.0
    //
    /***/

    public EventConsumerBase consumer;

    private static String[] _STAR_truncatableIds_ =
    {
        SubscriberDescriptionHelper.id()
    };

    public String[]
    _truncatable_ids()
    {
        return _STAR_truncatableIds_;
    }

    public void
    _read(org.omg.CORBA.portable.InputStream in)
    {
        ck = CookieHelper.read(in);
        consumer = EventConsumerBaseHelper.read(in);
    }

    public void
    _write(org.omg.CORBA.portable.OutputStream out)
    {
        CookieHelper.write(out, ck);
        EventConsumerBaseHelper.write(out, consumer);
    }

    public org.omg.CORBA.TypeCode
    _type()
    {
        return SubscriberDescriptionHelper.type();
    }
}
