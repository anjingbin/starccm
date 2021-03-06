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
// IDL:omg.org/CORBA/ArrayDef:1.0
//
public class _ArrayDefStub extends org.omg.CORBA.portable.ObjectImpl
                           implements ArrayDef
{
    private static final String[] _star_ids_ =
    {
        "IDL:omg.org/CORBA/ArrayDef:1.0",
        "IDL:omg.org/CORBA/IDLType:1.0",
        "IDL:omg.org/CORBA/IRObject:1.0"
    };

    public String[]
    _ids()
    {
        return _star_ids_;
    }

    final public static java.lang.Class _star_opsClass = ArrayDefOperations.class;

    //
    // IDL:omg.org/CORBA/ArrayDef/length:1.0
    //
    public int
    length()
    {
        while(true)
        {
            if(!this._is_local())
            {
                org.omg.CORBA.portable.OutputStream out = null;
                org.omg.CORBA.portable.InputStream in = null;
                try
                {
                    out = _request("_get_length", true);
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
                    throw new org.omg.CORBA.UNKNOWN("Unexpected User Exception: " + _star_id);
                }
                finally
                {
                    _releaseReply(in);
                }
            }
            else
            {
                org.omg.CORBA.portable.ServantObject _star_so = _servant_preinvoke("length", _star_opsClass);
                if(_star_so == null)
                    continue;
                ArrayDefOperations _star_self = (ArrayDefOperations)_star_so.servant;
                try
                {
                    return _star_self.length();
                }
                finally
                {
                    _servant_postinvoke(_star_so);
                }
            }
        }
    }

    public void
    length(int _star_a)
    {
        while(true)
        {
            if(!this._is_local())
            {
                org.omg.CORBA.portable.OutputStream out = null;
                org.omg.CORBA.portable.InputStream in = null;
                try
                {
                    out = _request("_set_length", true);
                    out.write_ulong(_star_a);
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
                    throw new org.omg.CORBA.UNKNOWN("Unexpected User Exception: " + _star_id);
                }
                finally
                {
                    _releaseReply(in);
                }
            }
            else
            {
                org.omg.CORBA.portable.ServantObject _star_so = _servant_preinvoke("length", _star_opsClass);
                if(_star_so == null)
                    continue;
                ArrayDefOperations _star_self = (ArrayDefOperations)_star_so.servant;
                try
                {
                    _star_self.length(_star_a);
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
    // IDL:omg.org/CORBA/ArrayDef/element_type:1.0
    //
    public org.omg.CORBA.TypeCode
    element_type()
    {
        while(true)
        {
            if(!this._is_local())
            {
                org.omg.CORBA.portable.OutputStream out = null;
                org.omg.CORBA.portable.InputStream in = null;
                try
                {
                    out = _request("_get_element_type", true);
                    in = _invoke(out);
                    org.omg.CORBA.TypeCode _star_r = in.read_TypeCode();
                    return _star_r;
                }
                catch(org.omg.CORBA.portable.RemarshalException _star_ex)
                {
                    continue;
                }
                catch(org.omg.CORBA.portable.ApplicationException _star_aex)
                {
                    final String _star_id = _star_aex.getId();
                    throw new org.omg.CORBA.UNKNOWN("Unexpected User Exception: " + _star_id);
                }
                finally
                {
                    _releaseReply(in);
                }
            }
            else
            {
                org.omg.CORBA.portable.ServantObject _star_so = _servant_preinvoke("element_type", _star_opsClass);
                if(_star_so == null)
                    continue;
                ArrayDefOperations _star_self = (ArrayDefOperations)_star_so.servant;
                try
                {
                    return _star_self.element_type();
                }
                finally
                {
                    _servant_postinvoke(_star_so);
                }
            }
        }
    }

    //
    // IDL:omg.org/CORBA/ArrayDef/element_type_def:1.0
    //
    public IDLType
    element_type_def()
    {
        while(true)
        {
            if(!this._is_local())
            {
                org.omg.CORBA.portable.OutputStream out = null;
                org.omg.CORBA.portable.InputStream in = null;
                try
                {
                    out = _request("_get_element_type_def", true);
                    in = _invoke(out);
                    IDLType _star_r = IDLTypeHelper.read(in);
                    return _star_r;
                }
                catch(org.omg.CORBA.portable.RemarshalException _star_ex)
                {
                    continue;
                }
                catch(org.omg.CORBA.portable.ApplicationException _star_aex)
                {
                    final String _star_id = _star_aex.getId();
                    throw new org.omg.CORBA.UNKNOWN("Unexpected User Exception: " + _star_id);
                }
                finally
                {
                    _releaseReply(in);
                }
            }
            else
            {
                org.omg.CORBA.portable.ServantObject _star_so = _servant_preinvoke("element_type_def", _star_opsClass);
                if(_star_so == null)
                    continue;
                ArrayDefOperations _star_self = (ArrayDefOperations)_star_so.servant;
                try
                {
                    return _star_self.element_type_def();
                }
                finally
                {
                    _servant_postinvoke(_star_so);
                }
            }
        }
    }

    public void
    element_type_def(IDLType _star_a)
    {
        while(true)
        {
            if(!this._is_local())
            {
                org.omg.CORBA.portable.OutputStream out = null;
                org.omg.CORBA.portable.InputStream in = null;
                try
                {
                    out = _request("_set_element_type_def", true);
                    IDLTypeHelper.write(out, _star_a);
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
                    throw new org.omg.CORBA.UNKNOWN("Unexpected User Exception: " + _star_id);
                }
                finally
                {
                    _releaseReply(in);
                }
            }
            else
            {
                org.omg.CORBA.portable.ServantObject _star_so = _servant_preinvoke("element_type_def", _star_opsClass);
                if(_star_so == null)
                    continue;
                ArrayDefOperations _star_self = (ArrayDefOperations)_star_so.servant;
                try
                {
                    _star_self.element_type_def(_star_a);
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
    // IDL:omg.org/CORBA/IDLType/type:1.0
    //
    public org.omg.CORBA.TypeCode
    type()
    {
        while(true)
        {
            if(!this._is_local())
            {
                org.omg.CORBA.portable.OutputStream out = null;
                org.omg.CORBA.portable.InputStream in = null;
                try
                {
                    out = _request("_get_type", true);
                    in = _invoke(out);
                    org.omg.CORBA.TypeCode _star_r = in.read_TypeCode();
                    return _star_r;
                }
                catch(org.omg.CORBA.portable.RemarshalException _star_ex)
                {
                    continue;
                }
                catch(org.omg.CORBA.portable.ApplicationException _star_aex)
                {
                    final String _star_id = _star_aex.getId();
                    throw new org.omg.CORBA.UNKNOWN("Unexpected User Exception: " + _star_id);
                }
                finally
                {
                    _releaseReply(in);
                }
            }
            else
            {
                org.omg.CORBA.portable.ServantObject _star_so = _servant_preinvoke("type", _star_opsClass);
                if(_star_so == null)
                    continue;
                ArrayDefOperations _star_self = (ArrayDefOperations)_star_so.servant;
                try
                {
                    return _star_self.type();
                }
                finally
                {
                    _servant_postinvoke(_star_so);
                }
            }
        }
    }

    //
    // IDL:omg.org/CORBA/IRObject/def_kind:1.0
    //
    public DefinitionKind
    def_kind()
    {
        while(true)
        {
            if(!this._is_local())
            {
                org.omg.CORBA.portable.OutputStream out = null;
                org.omg.CORBA.portable.InputStream in = null;
                try
                {
                    out = _request("_get_def_kind", true);
                    in = _invoke(out);
                    DefinitionKind _star_r = DefinitionKindHelper.read(in);
                    return _star_r;
                }
                catch(org.omg.CORBA.portable.RemarshalException _star_ex)
                {
                    continue;
                }
                catch(org.omg.CORBA.portable.ApplicationException _star_aex)
                {
                    final String _star_id = _star_aex.getId();
                    throw new org.omg.CORBA.UNKNOWN("Unexpected User Exception: " + _star_id);
                }
                finally
                {
                    _releaseReply(in);
                }
            }
            else
            {
                org.omg.CORBA.portable.ServantObject _star_so = _servant_preinvoke("def_kind", _star_opsClass);
                if(_star_so == null)
                    continue;
                ArrayDefOperations _star_self = (ArrayDefOperations)_star_so.servant;
                try
                {
                    return _star_self.def_kind();
                }
                finally
                {
                    _servant_postinvoke(_star_so);
                }
            }
        }
    }

    //
    // IDL:omg.org/CORBA/IRObject/destroy:1.0
    //
    public void
    destroy()
    {
        while(true)
        {
            if(!this._is_local())
            {
                org.omg.CORBA.portable.OutputStream out = null;
                org.omg.CORBA.portable.InputStream in = null;
                try
                {
                    out = _request("destroy", true);
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
                org.omg.CORBA.portable.ServantObject _star_so = _servant_preinvoke("destroy", _star_opsClass);
                if(_star_so == null)
                    continue;
                ArrayDefOperations _star_self = (ArrayDefOperations)_star_so.servant;
                try
                {
                    _star_self.destroy();
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
