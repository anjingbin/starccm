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

package bank.Bank;

//
// IDL:Bank/QueryOp:1.0
//
final public class QueryOpHolder implements org.omg.CORBA.portable.Streamable
{
    public QueryOp value;

    public
    QueryOpHolder()
    {
    }

    public
    QueryOpHolder(QueryOp initial)
    {
        value = initial;
    }

    public void
    _read(org.omg.CORBA.portable.InputStream in)
    {
        value = QueryOpHelper.read(in);
    }

    public void
    _write(org.omg.CORBA.portable.OutputStream out)
    {
        QueryOpHelper.write(out, value);
    }

    public org.omg.CORBA.TypeCode
    _type()
    {
        return QueryOpHelper.type();
    }
}
