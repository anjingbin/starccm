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
// IDL:omg.org/Components/StandardConfigurator:1.0
//
public class _StandardConfiguratorStub extends org.omg.CORBA.portable.ObjectImpl
                                       implements StandardConfigurator
{
    private static final String[] _star_ids_ =
    {
        "IDL:omg.org/Components/StandardConfigurator:1.0",
        "IDL:omg.org/Components/Configurator:1.0"
    };

    public String[]
    _ids()
    {
        return _star_ids_;
    }

    final public static java.lang.Class _star_opsClass = StandardConfiguratorOperations.class;

    //
    // IDL:omg.org/Components/StandardConfigurator/set_configuration:1.0
    //
    public void
    set_configuration(ConfigValue[] _star_a0)
    {
        while(true)
        {
            if(!this._is_local())
            {
                org.omg.CORBA.portable.OutputStream out = null;
                org.omg.CORBA.portable.InputStream in = null;
                try
                {
                    out = _request("set_configuration", true);
                    ConfigValuesHelper.write(out, _star_a0);
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
                org.omg.CORBA.portable.ServantObject _star_so = _servant_preinvoke("set_configuration", _star_opsClass);
                if(_star_so == null)
                    continue;
                StandardConfiguratorOperations _star_self = (StandardConfiguratorOperations)_star_so.servant;
                try
                {
                    org.omg.CORBA.portable.OutputStream _star_out = _orb().create_output_stream();
                    ConfigValuesHelper.write(_star_out, _star_a0);
                    org.omg.CORBA.portable.InputStream _star_in = _star_out.create_input_stream();
                    _star_a0 = ConfigValuesHelper.read(_star_in);
                    _star_self.set_configuration(_star_a0);
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
    // IDL:omg.org/Components/Configurator/configure:1.0
    //
    public void
    configure(CCMObject _star_a0)
        throws WrongComponentType
    {
        while(true)
        {
            if(!this._is_local())
            {
                org.omg.CORBA.portable.OutputStream out = null;
                org.omg.CORBA.portable.InputStream in = null;
                try
                {
                    out = _request("configure", true);
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

                    if(_star_id.equals(WrongComponentTypeHelper.id()))
                        throw WrongComponentTypeHelper.read(in);
                    throw new org.omg.CORBA.UNKNOWN("Unexpected User Exception: " + _star_id);
                }
                finally
                {
                    _releaseReply(in);
                }
            }
            else
            {
                org.omg.CORBA.portable.ServantObject _star_so = _servant_preinvoke("configure", _star_opsClass);
                if(_star_so == null)
                    continue;
                StandardConfiguratorOperations _star_self = (StandardConfiguratorOperations)_star_so.servant;
                try
                {
                    _star_self.configure(_star_a0);
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
