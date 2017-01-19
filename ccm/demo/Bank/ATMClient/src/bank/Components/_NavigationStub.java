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
// IDL:omg.org/Components/Navigation:1.0
//
public class _NavigationStub extends org.omg.CORBA.portable.ObjectImpl
                             implements Navigation
{
    private static final String[] _star_ids_ =
    {
        "IDL:omg.org/Components/Navigation:1.0",
    };

    public String[]
    _ids()
    {
        return _star_ids_;
    }

    final public static java.lang.Class _star_opsClass = NavigationOperations.class;

    //
    // IDL:omg.org/Components/Navigation/provide_facet:1.0
    //
    public org.omg.CORBA.Object
    provide_facet(String _star_a0)
        throws InvalidName
    {
        while(true)
        {
            if(!this._is_local())
            {
                org.omg.CORBA.portable.OutputStream out = null;
                org.omg.CORBA.portable.InputStream in = null;
                try
                {
                    out = _request("provide_facet", true);
                    FeatureNameHelper.write(out, _star_a0);
                    in = _invoke(out);
                    org.omg.CORBA.Object _star_r = in.read_Object();
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

                    if(_star_id.equals(InvalidNameHelper.id()))
                        throw InvalidNameHelper.read(in);
                    throw new org.omg.CORBA.UNKNOWN("Unexpected User Exception: " + _star_id);
                }
                finally
                {
                    _releaseReply(in);
                }
            }
            else
            {
                org.omg.CORBA.portable.ServantObject _star_so = _servant_preinvoke("provide_facet", _star_opsClass);
                if(_star_so == null)
                    continue;
                NavigationOperations _star_self = (NavigationOperations)_star_so.servant;
                try
                {
                    return _star_self.provide_facet(_star_a0);
                }
                finally
                {
                    _servant_postinvoke(_star_so);
                }
            }
        }
    }

    //
    // IDL:omg.org/Components/Navigation/get_all_facets:1.0
    //
    public FacetDescription[]
    get_all_facets()
    {
        while(true)
        {
            if(!this._is_local())
            {
                org.omg.CORBA.portable.OutputStream out = null;
                org.omg.CORBA.portable.InputStream in = null;
                try
                {
                    out = _request("get_all_facets", true);
                    in = _invoke(out);
                    FacetDescription[] _star_r = FacetDescriptionsHelper.read(in);
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
                org.omg.CORBA.portable.ServantObject _star_so = _servant_preinvoke("get_all_facets", _star_opsClass);
                if(_star_so == null)
                    continue;
                NavigationOperations _star_self = (NavigationOperations)_star_so.servant;
                try
                {
                    FacetDescription[] _star_r = _star_self.get_all_facets();
                    org.omg.CORBA.portable.OutputStream _star_out = _orb().create_output_stream();
                    int len0 = _star_r.length;
                    _star_out.write_ulong(len0);
                    for(int i0 = 0 ; i0 < len0 ; i0++)
                        FacetDescriptionHelper.write(_star_out, _star_r[i0]);
                    org.omg.CORBA.portable.InputStream _star_in = _star_out.create_input_stream();
                    int len1 = _star_in.read_ulong();
                    _star_r = new FacetDescription[len1];
                    for(int i1 = 0 ; i1 < len1 ; i1++)
                        _star_r[i1] = FacetDescriptionHelper.read(_star_in);
                    return _star_r;
                }
                finally
                {
                    _servant_postinvoke(_star_so);
                }
            }
        }
    }

    //
    // IDL:omg.org/Components/Navigation/get_named_facets:1.0
    //
    public FacetDescription[]
    get_named_facets(String[] _star_a0)
        throws InvalidName
    {
        while(true)
        {
            if(!this._is_local())
            {
                org.omg.CORBA.portable.OutputStream out = null;
                org.omg.CORBA.portable.InputStream in = null;
                try
                {
                    out = _request("get_named_facets", true);
                    NameListHelper.write(out, _star_a0);
                    in = _invoke(out);
                    FacetDescription[] _star_r = FacetDescriptionsHelper.read(in);
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

                    if(_star_id.equals(InvalidNameHelper.id()))
                        throw InvalidNameHelper.read(in);
                    throw new org.omg.CORBA.UNKNOWN("Unexpected User Exception: " + _star_id);
                }
                finally
                {
                    _releaseReply(in);
                }
            }
            else
            {
                org.omg.CORBA.portable.ServantObject _star_so = _servant_preinvoke("get_named_facets", _star_opsClass);
                if(_star_so == null)
                    continue;
                NavigationOperations _star_self = (NavigationOperations)_star_so.servant;
                try
                {
                    FacetDescription[] _star_r = _star_self.get_named_facets(_star_a0);
                    org.omg.CORBA.portable.OutputStream _star_out = _orb().create_output_stream();
                    int len0 = _star_r.length;
                    _star_out.write_ulong(len0);
                    for(int i0 = 0 ; i0 < len0 ; i0++)
                        FacetDescriptionHelper.write(_star_out, _star_r[i0]);
                    org.omg.CORBA.portable.InputStream _star_in = _star_out.create_input_stream();
                    int len1 = _star_in.read_ulong();
                    _star_r = new FacetDescription[len1];
                    for(int i1 = 0 ; i1 < len1 ; i1++)
                        _star_r[i1] = FacetDescriptionHelper.read(_star_in);
                    return _star_r;
                }
                finally
                {
                    _servant_postinvoke(_star_so);
                }
            }
        }
    }

    //
    // IDL:omg.org/Components/Navigation/same_component:1.0
    //
    public boolean
    same_component(org.omg.CORBA.Object _star_a0)
    {
        while(true)
        {
            if(!this._is_local())
            {
                org.omg.CORBA.portable.OutputStream out = null;
                org.omg.CORBA.portable.InputStream in = null;
                try
                {
                    out = _request("same_component", true);
                    out.write_Object(_star_a0);
                    in = _invoke(out);
                    boolean _star_r = in.read_boolean();
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
                org.omg.CORBA.portable.ServantObject _star_so = _servant_preinvoke("same_component", _star_opsClass);
                if(_star_so == null)
                    continue;
                NavigationOperations _star_self = (NavigationOperations)_star_so.servant;
                try
                {
                    return _star_self.same_component(_star_a0);
                }
                finally
                {
                    _servant_postinvoke(_star_so);
                }
            }
        }
    }
}