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
// IDL:omg.org/Components/CreateFailure:1.0
//
/***/

final public class CreateFailure extends org.omg.CORBA.UserException
{
    private static final String _star_id = "IDL:omg.org/Components/CreateFailure:1.0";

    public
    CreateFailure()
    {
        super(_star_id);
    }

    public
    CreateFailure(int reason)
    {
        super(_star_id);
        this.reason = reason;
    }

    public
    CreateFailure(String _reason,
                  int reason)
    {
        super(_star_id + " " + _reason);
        this.reason = reason;
    }

    public int reason;
}
