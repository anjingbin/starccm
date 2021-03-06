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
// IDL:omg.org/Components/KeylessCCMHome:1.0
//
public class _KeylessCCMHomeStub extends org.omg.CORBA.portable.ObjectImpl
                                 implements KeylessCCMHome
{
    private static final String[] _star_ids_ =
    {
        "IDL:omg.org/Components/KeylessCCMHome:1.0",
    };

    public String[]
    _ids()
    {
        return _star_ids_;
    }

    final public static java.lang.Class _star_opsClass = KeylessCCMHomeOperations.class;

    //
    // IDL:omg.org/Components/KeylessCCMHome/create_component:1.0
    //
    public CCMObject
    create_component()
        throws CreateFailure
    {
        while(true)
        {
            if(!this._is_local())
            {
                org.omg.CORBA.portable.OutputStream out = null;
                org.omg.CORBA.portable.InputStream in = null;
                try
                {
                    out = _request("create_component", true);
                    in = _invoke(out);
                    CCMObject _star_r = CCMObjectHelper.read(in);
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

                    if(_star_id.equals(CreateFailureHelper.id()))
                        throw CreateFailureHelper.read(in);
                    throw new org.omg.CORBA.UNKNOWN("Unexpected User Exception: " + _star_id);
                }
                finally
                {
                    _releaseReply(in);
                }
            }
            else
            {
                org.omg.CORBA.portable.ServantObject _star_so = _servant_preinvoke("create_component", _star_opsClass);
                if(_star_so == null)
                    continue;
                KeylessCCMHomeOperations _star_self = (KeylessCCMHomeOperations)_star_so.servant;
                try
                {
                    return _star_self.create_component();
                }
                finally
                {
                    _servant_postinvoke(_star_so);
                }
            }
        }
    }
}
