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
// IDL:omg.org/CORBA/PolicyError:1.0
//
/***/

final public class PolicyError extends org.omg.CORBA.UserException
{
    private static final String _star_id = "IDL:omg.org/CORBA/PolicyError:1.0";

    public
    PolicyError()
    {
        super(_star_id);
    }

    public
    PolicyError(short reason)
    {
        super(_star_id);
        this.reason = reason;
    }

    public
    PolicyError(String _reason,
                short reason)
    {
        super(_star_id + " " + _reason);
        this.reason = reason;
    }

    public short reason;
}
