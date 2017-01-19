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
// IDL:omg.org/Components/ComponentPortDescription:1.0
//
/***/

public abstract class ComponentPortDescription implements org.omg.CORBA.portable.StreamableValue
{
    //
    // IDL:omg.org/Components/ComponentPortDescription/facets:1.0
    //
    /***/

    public FacetDescription[] facets;

    //
    // IDL:omg.org/Components/ComponentPortDescription/receptacles:1.0
    //
    /***/

    public ReceptacleDescription[] receptacles;

    //
    // IDL:omg.org/Components/ComponentPortDescription/consumers:1.0
    //
    /***/

    public ConsumerDescription[] consumers;

    //
    // IDL:omg.org/Components/ComponentPortDescription/emitters:1.0
    //
    /***/

    public EmitterDescription[] emitters;

    //
    // IDL:omg.org/Components/ComponentPortDescription/publishers:1.0
    //
    /***/

    public PublisherDescription[] publishers;

    private static String[] _STAR_truncatableIds_ =
    {
        ComponentPortDescriptionHelper.id()
    };

    public String[]
    _truncatable_ids()
    {
        return _STAR_truncatableIds_;
    }

    public void
    _read(org.omg.CORBA.portable.InputStream in)
    {
        facets = FacetDescriptionsHelper.read(in);
        receptacles = ReceptacleDescriptionsHelper.read(in);
        consumers = ConsumerDescriptionsHelper.read(in);
        emitters = EmitterDescriptionsHelper.read(in);
        publishers = PublisherDescriptionsHelper.read(in);
    }

    public void
    _write(org.omg.CORBA.portable.OutputStream out)
    {
        FacetDescriptionsHelper.write(out, facets);
        ReceptacleDescriptionsHelper.write(out, receptacles);
        ConsumerDescriptionsHelper.write(out, consumers);
        EmitterDescriptionsHelper.write(out, emitters);
        PublisherDescriptionsHelper.write(out, publishers);
    }

    public org.omg.CORBA.TypeCode
    _type()
    {
        return ComponentPortDescriptionHelper.type();
    }
}
