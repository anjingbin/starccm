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
// IDL:omg.org/Components/ConsumerDescription:1.0
//
/***/

public abstract class ConsumerDescription extends PortDescription
{
    //
    // IDL:omg.org/Components/ConsumerDescription/consumer:1.0
    //
    /***/

    public EventConsumerBase consumer;

    private static String[] _STAR_truncatableIds_ =
    {
        ConsumerDescriptionHelper.id()
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
        consumer = EventConsumerBaseHelper.read(in);
    }

    public void
    _write(org.omg.CORBA.portable.OutputStream out)
    {
        super._write(out);
        EventConsumerBaseHelper.write(out, consumer);
    }

    public org.omg.CORBA.TypeCode
    _type()
    {
        return ConsumerDescriptionHelper.type();
    }
}
