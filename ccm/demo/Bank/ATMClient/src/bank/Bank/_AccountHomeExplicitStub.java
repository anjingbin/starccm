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
// IDL:Bank/AccountHomeExplicit:1.0
//
public class _AccountHomeExplicitStub extends org.omg.CORBA.portable.ObjectImpl
                                      implements AccountHomeExplicit
{
    private static final String[] _star_ids_ =
    {
        "IDL:Bank/AccountHomeExplicit:1.0",
        "IDL:omg.org/Components/HomeConfiguration:1.0",
        "IDL:omg.org/Components/CCMHome:1.0"
    };

    public String[]
    _ids()
    {
        return _star_ids_;
    }

    final public static java.lang.Class _star_opsClass = AccountHomeExplicitOperations.class;

    //
    // IDL:omg.org/Components/HomeConfiguration/set_configurator:1.0
    //
    public void
    set_configurator(bank.Components.Configurator _star_a0)
    {
        while(true)
        {
            if(!this._is_local())
            {
                org.omg.CORBA.portable.OutputStream out = null;
                org.omg.CORBA.portable.InputStream in = null;
                try
                {
                    out = _request("set_configurator", true);
                    bank.Components.ConfiguratorHelper.write(out, _star_a0);
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
                org.omg.CORBA.portable.ServantObject _star_so = _servant_preinvoke("set_configurator", _star_opsClass);
                if(_star_so == null)
                    continue;
                AccountHomeExplicitOperations _star_self = (AccountHomeExplicitOperations)_star_so.servant;
                try
                {
                    _star_self.set_configurator(_star_a0);
                    return;
                }
                finally
                {
                    _servant_postinvoke(_star_so);
                }
            }
        }
    }

    //
    // IDL:omg.org/Components/HomeConfiguration/set_configuration_values:1.0
    //
    public void
    set_configuration_values(bank.Components.ConfigValue[] _star_a0)
    {
        while(true)
        {
            if(!this._is_local())
            {
                org.omg.CORBA.portable.OutputStream out = null;
                org.omg.CORBA.portable.InputStream in = null;
                try
                {
                    out = _request("set_configuration_values", true);
                    bank.Components.ConfigValuesHelper.write(out, _star_a0);
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
                org.omg.CORBA.portable.ServantObject _star_so = _servant_preinvoke("set_configuration_values", _star_opsClass);
                if(_star_so == null)
                    continue;
                AccountHomeExplicitOperations _star_self = (AccountHomeExplicitOperations)_star_so.servant;
                try
                {
                    org.omg.CORBA.portable.OutputStream _star_out = _orb().create_output_stream();
                    bank.Components.ConfigValuesHelper.write(_star_out, _star_a0);
                    org.omg.CORBA.portable.InputStream _star_in = _star_out.create_input_stream();
                    _star_a0 = bank.Components.ConfigValuesHelper.read(_star_in);
                    _star_self.set_configuration_values(_star_a0);
                    return;
                }
                finally
                {
                    _servant_postinvoke(_star_so);
                }
            }
        }
    }

    //
    // IDL:omg.org/Components/HomeConfiguration/complete_component_configuration:1.0
    //
    public void
    complete_component_configuration(boolean _star_a0)
    {
        while(true)
        {
            if(!this._is_local())
            {
                org.omg.CORBA.portable.OutputStream out = null;
                org.omg.CORBA.portable.InputStream in = null;
                try
                {
                    out = _request("complete_component_configuration", true);
                    out.write_boolean(_star_a0);
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
                org.omg.CORBA.portable.ServantObject _star_so = _servant_preinvoke("complete_component_configuration", _star_opsClass);
                if(_star_so == null)
                    continue;
                AccountHomeExplicitOperations _star_self = (AccountHomeExplicitOperations)_star_so.servant;
                try
                {
                    _star_self.complete_component_configuration(_star_a0);
                    return;
                }
                finally
                {
                    _servant_postinvoke(_star_so);
                }
            }
        }
    }

    //
    // IDL:omg.org/Components/HomeConfiguration/disable_home_configuration:1.0
    //
    public void
    disable_home_configuration()
    {
        while(true)
        {
            if(!this._is_local())
            {
                org.omg.CORBA.portable.OutputStream out = null;
                org.omg.CORBA.portable.InputStream in = null;
                try
                {
                    out = _request("disable_home_configuration", true);
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
                org.omg.CORBA.portable.ServantObject _star_so = _servant_preinvoke("disable_home_configuration", _star_opsClass);
                if(_star_so == null)
                    continue;
                AccountHomeExplicitOperations _star_self = (AccountHomeExplicitOperations)_star_so.servant;
                try
                {
                    _star_self.disable_home_configuration();
                    return;
                }
                finally
                {
                    _servant_postinvoke(_star_so);
                }
            }
        }
    }

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
                AccountHomeExplicitOperations _star_self = (AccountHomeExplicitOperations)_star_so.servant;
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
                AccountHomeExplicitOperations _star_self = (AccountHomeExplicitOperations)_star_so.servant;
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
    remove_component(bank.Components.CCMObject _star_a0)
        throws bank.Components.RemoveFailure
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
                    bank.Components.CCMObjectHelper.write(out, _star_a0);
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

                    if(_star_id.equals(bank.Components.RemoveFailureHelper.id()))
                        throw bank.Components.RemoveFailureHelper.read(in);
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
                AccountHomeExplicitOperations _star_self = (AccountHomeExplicitOperations)_star_so.servant;
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