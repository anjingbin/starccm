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
// IDL:Bank/WithdrawOp:1.0
//
public class _WithdrawOpStub extends org.omg.CORBA.portable.ObjectImpl
                             implements WithdrawOp
{
    private static final String[] _star_ids_ =
    {
        "IDL:Bank/WithdrawOp:1.0",
    };

    public String[]
    _ids()
    {
        return _star_ids_;
    }

    final public static java.lang.Class _star_opsClass = WithdrawOpOperations.class;

    //
    // IDL:Bank/WithdrawOp/withdraw:1.0
    //
    public int
    withdraw(String _star_a0,
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
                    out = _request("withdraw", true);
                    out.write_string(_star_a0);
                    out.write_ulong(_star_a1);
                    in = _invoke(out);
                    int _star_r = in.read_ulong();
                    return _star_r;
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
                org.omg.CORBA.portable.ServantObject _star_so = _servant_preinvoke("withdraw", _star_opsClass);
                if(_star_so == null)
                    continue;
                WithdrawOpOperations _star_self = (WithdrawOpOperations)_star_so.servant;
                try
                {
                    return _star_self.withdraw(_star_a0, _star_a1);
                }
                finally
                {
                    _servant_postinvoke(_star_so);
                }
            }
        }
    }
}
