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
// IDL:omg.org/Components/CCMHome:1.0
//
public class _CCMHomeStub extends org.omg.CORBA.portable.ObjectImpl
                          implements CCMHome
{
    private static final String[] _star_ids_ =
    {
        "IDL:omg.org/Components/CCMHome:1.0",
    };

    public String[]
    _ids()
    {
        return _star_ids_;
    }

    final public static java.lang.Class _star_opsClass = CCMHomeOperations.class;

    //
    // IDL:omg.org/Components/CCMHome/get_component_def:1.0
    //
    public bank.CORBA.IRObject
    get_component_def()
    {
        while(true)
        {
            if(!this._is_local())
            {
                org.omg.CORBA.portable.OutputStream out = null;
                org.omg.CORBA.portable.InputStream in = null;
                try
                {
                    out = _request("get_component_def", true);
                    in = _invoke(out);
                    bank.CORBA.IRObject _star_r = bank.CORBA.IRObjectHelper.read(in);
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
                org.omg.CORBA.portable.ServantObject _star_so = _servant_preinvoke("get_component_def", _star_opsClass);
                if(_star_so == null)
                    continue;
                CCMHomeOperations _star_self = (CCMHomeOperations)_star_so.servant;
                try
                {
                    return _star_self.get_component_def();
                }
                finally
                {
                    _servant_postinvoke(_star_so);
                }
            }
        }
    }

    //
    // IDL:omg.org/Components/CCMHome/get_home_def:1.0
    //
    public bank.CORBA.IRObject
    get_home_def()
    {
        while(true)
        {
            if(!this._is_local())
            {
                org.omg.CORBA.portable.OutputStream out = null;
                org.omg.CORBA.portable.InputStream in = null;
                try
                {
                    out = _request("get_home_def", true);
                    in = _invoke(out);
                    bank.CORBA.IRObject _star_r = bank.CORBA.IRObjectHelper.read(in);
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
                org.omg.CORBA.portable.ServantObject _star_so = _servant_preinvoke("get_home_def", _star_opsClass);
                if(_star_so == null)
                    continue;
                CCMHomeOperations _star_self = (CCMHomeOperations)_star_so.servant;
                try
                {
                    return _star_self.get_home_def();
                }
                finally
                {
                    _servant_postinvoke(_star_so);
                }
            }
        }
    }

    //
    // IDL:omg.org/Components/CCMHome/remove_component:1.0
    //
    public void
    remove_component(CCMObject _star_a0)
        throws RemoveFailure
    {
        while(true)
        {
            if(!this._is_local())
            {
                org.omg.CORBA.portable.OutputStream out = null;
                org.omg.CORBA.portable.InputStream in = null;
                try
                {
                    out = _request("remove_component", true);
                    CCMObjectHelper.write(out, _star_a0);
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

                    if(_star_id.equals(RemoveFailureHelper.id()))
                        throw RemoveFailureHelper.read(in);
                    throw new org.omg.CORBA.UNKNOWN("Unexpected User Exception: " + _star_id);
                }
                finally
                {
                    _releaseReply(in);
                }
            }
            else
            {
                org.omg.CORBA.portable.ServantObject _star_so = _servant_preinvoke("remove_component", _star_opsClass);
                if(_star_so == null)
                    continue;
                CCMHomeOperations _star_self = (CCMHomeOperations)_star_so.servant;
                try
                {
                    _star_self.remove_component(_star_a0);
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
