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
// IDL:omg.org/CORBA/ServiceDetail:1.0
//
/***/

final public class ServiceDetail implements org.omg.CORBA.portable.IDLEntity
{
    private static final String _star_id = "IDL:omg.org/CORBA/ServiceDetail:1.0";

    public
    ServiceDetail()
    {
    }

    public
    ServiceDetail(int service_detail_type,
                  byte[] service_detail)
    {
        this.service_detail_type = service_detail_type;
        this.service_detail = service_detail;
    }

    public int service_detail_type;
    public byte[] service_detail;
}
