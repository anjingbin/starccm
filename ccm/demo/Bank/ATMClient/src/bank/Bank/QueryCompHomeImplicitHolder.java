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
// IDL:Bank/QueryCompHomeImplicit:1.0
//
final public class QueryCompHomeImplicitHolder implements org.omg.CORBA.portable.Streamable
{
    public QueryCompHomeImplicit value;

    public
    QueryCompHomeImplicitHolder()
    {
    }

    public
    QueryCompHomeImplicitHolder(QueryCompHomeImplicit initial)
    {
        value = initial;
    }

    public void
    _read(org.omg.CORBA.portable.InputStream in)
    {
        value = QueryCompHomeImplicitHelper.read(in);
    }

    public void
    _write(org.omg.CORBA.portable.OutputStream out)
    {
        QueryCompHomeImplicitHelper.write(out, value);
    }

    public org.omg.CORBA.TypeCode
    _type()
    {
        return QueryCompHomeImplicitHelper.type();
    }
}
