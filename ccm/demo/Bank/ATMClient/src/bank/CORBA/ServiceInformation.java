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
// IDL:omg.org/CORBA/ServiceInformation:1.0
//
/***/

final public class ServiceInformation implements org.omg.CORBA.portable.IDLEntity
{
    private static final String _star_id = "IDL:omg.org/CORBA/ServiceInformation:1.0";

    public
    ServiceInformation()
    {
    }

    public
    ServiceInformation(int[] service_options,
                       ServiceDetail[] service_details)
    {
        this.service_options = service_options;
        this.service_details = service_details;
    }

    public int[] service_options;
    public ServiceDetail[] service_details;
}
