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
// IDL:Bank/DepositOp:1.0
//
public class _DepositOpStub extends org.omg.CORBA.portable.ObjectImpl
                            implements DepositOp
{
    private static final String[] _star_ids_ =
    {
        "IDL:Bank/DepositOp:1.0",
    };

    public String[]
    _ids()
    {
        return _star_ids_;
    }

    final public static java.lang.Class _star_opsClass = DepositOpOperations.class;

    //
    // IDL:Bank/DepositOp/deposit:1.0
    //
    public void
    deposit(String _star_a0,
            int _star_a1)
    {
        while(true)
        {
            if(!this._is_local())
            {
                org.omg.CORBA.portable.OutputStream out = null;
                org.omg.CORBA.portable.InputStream in = null;
                try
                {
                    out = _request("deposit", true);
                    out.write_string(_star_a0);
                    out.write_ulong(_star_a1);
                    in = _invoke(out);
                    return;
                }
                catch(org.omg.CORBA.portable.RemarshalException _star_ex)
                {
                    continue;
                }
                catch(org.omg.CORBA.portable.ApplicationException _star_aex)
                {
                    final String _star_id = _star_aex.getId();
                    in = _star_aex.getInputStream();

                    throw new org.omg.CORBA.UNKNOWN("Unexpected User Exception: " + _star_id);
                }
                finally
                {
                    _releaseReply(in);
                }
            }
            else
            {
                org.omg.CORBA.portable.ServantObject _star_so = _servant_preinvoke("deposit", _star_opsClass);
                if(_star_so == null)
                    continue;
                DepositOpOperations _star_self = (DepositOpOperations)_star_so.servant;
                try
                {
                    _star_self.deposit(_star_a0, _star_a1);
                    return;
                }
                finally
                {
                    _servant_postinvoke(_star_so);
                }
            }
        }
    }
}