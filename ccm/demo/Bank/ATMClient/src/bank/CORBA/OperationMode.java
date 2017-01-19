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
// IDL:omg.org/CORBA/OperationMode:1.0
//
/***/

public class OperationMode implements org.omg.CORBA.portable.IDLEntity
{
    private static OperationMode [] values_ = new OperationMode[2];
    private int value_;

    public final static int _OP_NORMAL = 0;
    public final static OperationMode OP_NORMAL = new OperationMode(_OP_NORMAL);
    public final static int _OP_ONEWAY = 1;
    public final static OperationMode OP_ONEWAY = new OperationMode(_OP_ONEWAY);

    protected
    OperationMode(int value)
    {
        values_[value] = this;
        value_ = value;
    }

    public int
    value()
    {
        return value_;
    }

    public static OperationMode
    from_int(int value)
    {
        if(value < values_.length)
            return values_[value];
        else
            throw new org.omg.CORBA.BAD_PARAM("Value (" + value  + ") out of range", 25, org.omg.CORBA.CompletionStatus.COMPLETED_NO);
    }

    private java.lang.Object
    readResolve()
        throws java.io.ObjectStreamException
    {
        return from_int(value());
    }
}