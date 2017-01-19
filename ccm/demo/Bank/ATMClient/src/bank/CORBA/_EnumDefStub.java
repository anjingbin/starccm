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
// IDL:omg.org/CORBA/EnumDef:1.0
//
public class _EnumDefStub extends org.omg.CORBA.portable.ObjectImpl
                          implements EnumDef
{
    private static final String[] _star_ids_ =
    {
        "IDL:omg.org/CORBA/EnumDef:1.0",
        "IDL:omg.org/CORBA/TypedefDef:1.0",
        "IDL:omg.org/CORBA/Contained:1.0",
        "IDL:omg.org/CORBA/IRObject:1.0",
        "IDL:omg.org/CORBA/IDLType:1.0"
    };

    public String[]
    _ids()
    {
        return _star_ids_;
    }

    final public static java.lang.Class _star_opsClass = EnumDefOperations.class;

    //
    // IDL:omg.org/CORBA/EnumDef/members:1.0
    //
    public String[]
    members()
    {
        while(true)
        {
            if(!this._is_local())
            {
                org.omg.CORBA.portable.OutputStream out = null;
                org.omg.CORBA.portable.InputStream in = null;
                try
                {
                    out = _request("_get_members", true);
                    in = _invoke(out);
                    String[] _star_r = EnumMemberSeqHelper.read(in);
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
                org.omg.CORBA.portable.ServantObject _star_so = _servant_preinvoke("members", _star_opsClass);
                if(_star_so == null)
                    continue;
                EnumDefOperations _star_self = (EnumDefOperations)_star_so.servant;
                try
                {
                    return _star_self.members();
                }
                finally
                {
                    _servant_postinvoke(_star_so);
                }
            }
        }
    }

    public void
    members(String[] _star_a)
    {
        while(true)
        {
            if(!this._is_local())
            {
                org.omg.CORBA.portable.OutputStream out = null;
                org.omg.CORBA.portable.InputStream in = null;
                try
                {
                    out = _request("_set_members", true);
                    EnumMemberSeqHelper.write(out, _star_a);
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
                org.omg.CORBA.portable.ServantObject _star_so = _servant_preinvoke("members", _star_opsClass);
                if(_star_so == null)
                    continue;
                EnumDefOperations _star_self = (EnumDefOperations)_star_so.servant;
                try
                {
                    _star_self.members(_star_a);
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
    // IDL:omg.org/CORBA/Contained/id:1.0
    //
    public String
    id()
    {
        while(true)
        {
            if(!this._is_local())
            {
                org.omg.CORBA.portable.OutputStream out = null;
                org.omg.CORBA.portable.InputStream in = null;
                try
                {
                    out = _request("_get_id", true);
                    in = _invoke(out);
                    String _star_r = RepositoryIdHelper.read(in);
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
                org.omg.CORBA.portable.ServantObject _star_so = _servant_preinvoke("id", _star_opsClass);
                if(_star_so == null)
                    continue;
                EnumDefOperations _star_self = (EnumDefOperations)_star_so.servant;
                try
                {
                    return _star_self.id();
                }
                finally
                {
                    _servant_postinvoke(_star_so);
                }
            }
        }
    }

    public void
    id(String _star_a)
    {
        while(true)
        {
            if(!this._is_local())
            {
                org.omg.CORBA.portable.OutputStream out = null;
                org.omg.CORBA.portable.InputStream in = null;
                try
                {
                    out = _request("_set_id", true);
                    RepositoryIdHelper.write(out, _star_a);
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
                org.omg.CORBA.portable.ServantObject _star_so = _servant_preinvoke("id", _star_opsClass);
                if(_star_so == null)
                    continue;
                EnumDefOperations _star_self = (EnumDefOperations)_star_so.servant;
                try
                {
                    _star_self.id(_star_a);
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
    // IDL:omg.org/CORBA/Contained/name:1.0
    //
    public String
    name()
    {
        while(true)
        {
            if(!this._is_local())
            {
                org.omg.CORBA.portable.OutputStream out = null;
                org.omg.CORBA.portable.InputStream in = null;
                try
                {
                    out = _request("_get_name", true);
                    in = _invoke(out);
                    String _star_r = IdentifierHelper.read(in);
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
                org.omg.CORBA.portable.ServantObject _star_so = _servant_preinvoke("name", _star_opsClass);
                if(_star_so == null)
                    continue;
                EnumDefOperations _star_self = (EnumDefOperations)_star_so.servant;
                try
                {
                    return _star_self.name();
                }
                finally
                {
                    _servant_postinvoke(_star_so);
                }
            }
        }
    }

    public void
    name(String _star_a)
    {
        while(true)
        {
            if(!this._is_local())
            {
                org.omg.CORBA.portable.OutputStream out = null;
                org.omg.CORBA.portable.InputStream in = null;
                try
                {
                    out = _request("_set_name", true);
                    IdentifierHelper.write(out, _star_a);
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
                org.omg.CORBA.portable.ServantObject _star_so = _servant_preinvoke("name", _star_opsClass);
                if(_star_so == null)
                    continue;
                EnumDefOperations _star_self = (EnumDefOperations)_star_so.servant;
                try
                {
                    _star_self.name(_star_a);
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
    // IDL:omg.org/CORBA/Contained/version:1.0
    //
    public String
    version()
    {
        while(true)
        {
            if(!this._is_local())
            {
                org.omg.CORBA.portable.OutputStream out = null;
                org.omg.CORBA.portable.InputStream in = null;
                try
                {
                    out = _request("_get_version", true);
                    in = _invoke(out);
                    String _star_r = VersionSpecHelper.read(in);
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
                org.omg.CORBA.portable.ServantObject _star_so = _servant_preinvoke("version", _star_opsClass);
                if(_star_so == null)
                    continue;
                EnumDefOperations _star_self = (EnumDefOperations)_star_so.servant;
                try
                {
                    return _star_self.version();
                }
                finally
                {
                    _servant_postinvoke(_star_so);
                }
            }
        }
    }

    public void
    version(String _star_a)
    {
        while(true)
        {
            if(!this._is_local())
            {
                org.omg.CORBA.portable.OutputStream out = null;
                org.omg.CORBA.portable.InputStream in = null;
                try
                {
                    out = _request("_set_version", true);
                    VersionSpecHelper.write(out, _star_a);
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
                org.omg.CORBA.portable.ServantObject _star_so = _servant_preinvoke("version", _star_opsClass);
                if(_star_so == null)
                    continue;
                EnumDefOperations _star_self = (EnumDefOperations)_star_so.servant;
                try
                {
                    _star_self.version(_star_a);
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
    // IDL:omg.org/CORBA/Contained/defined_in:1.0
    //
    public Container
    defined_in()
    {
        while(true)
        {
            if(!this._is_local())
            {
                org.omg.CORBA.portable.OutputStream out = null;
                org.omg.CORBA.portable.InputStream in = null;
                try
                {
                    out = _request("_get_defined_in", true);
                    in = _invoke(out);
                    Container _star_r = ContainerHelper.read(in);
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
                org.omg.CORBA.portable.ServantObject _star_so = _servant_preinvoke("defined_in", _star_opsClass);
                if(_star_so == null)
                    continue;
                EnumDefOperations _star_self = (EnumDefOperations)_star_so.servant;
                try
                {
                    return _star_self.defined_in();
                }
                finally
                {
                    _servant_postinvoke(_star_so);
                }
            }
        }
    }

    //
    // IDL:omg.org/CORBA/Contained/absolute_name:1.0
    //
    public String
    absolute_name()
    {
        while(true)
        {
            if(!this._is_local())
            {
                org.omg.CORBA.portable.OutputStream out = null;
                org.omg.CORBA.portable.InputStream in = null;
                try
                {
                    out = _request("_get_absolute_name", true);
                    in = _invoke(out);
                    String _star_r = ScopedNameHelper.read(in);
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
                org.omg.CORBA.portable.ServantObject _star_so = _servant_preinvoke("absolute_name", _star_opsClass);
                if(_star_so == null)
                    continue;
                EnumDefOperations _star_self = (EnumDefOperations)_star_so.servant;
                try
                {
                    return _star_self.absolute_name();
                }
                finally
                {
                    _servant_postinvoke(_star_so);
                }
            }
        }
    }

    //
    // IDL:omg.org/CORBA/Contained/containing_repository:1.0
    //
    public Repository
    containing_repository()
    {
        while(true)
        {
            if(!this._is_local())
            {
                org.omg.CORBA.portable.OutputStream out = null;
                org.omg.CORBA.portable.InputStream in = null;
                try
                {
                    out = _request("_get_containing_repository", true);
                    in = _invoke(out);
                    Repository _star_r = RepositoryHelper.read(in);
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
                org.omg.CORBA.portable.ServantObject _star_so = _servant_preinvoke("containing_repository", _star_opsClass);
                if(_star_so == null)
                    continue;
                EnumDefOperations _star_self = (EnumDefOperations)_star_so.servant;
                try
                {
                    return _star_self.containing_repository();
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
                EnumDefOperations _star_self = (EnumDefOperations)_star_so.servant;
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
                EnumDefOperations _star_self = (EnumDefOperations)_star_so.servant;
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
    // IDL:omg.org/CORBA/Contained/describe:1.0
    //
    public bank.CORBA.ContainedPackage.Description
    describe()
    {
        while(true)
        {
            if(!this._is_local())
            {
                org.omg.CORBA.portable.OutputStream out = null;
                org.omg.CORBA.portable.InputStream in = null;
                try
                {
                    out = _request("describe", true);
                    in = _invoke(out);
                    bank.CORBA.ContainedPackage.Description _star_r = bank.CORBA.ContainedPackage.DescriptionHelper.read(in);
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
                org.omg.CORBA.portable.ServantObject _star_so = _servant_preinvoke("describe", _star_opsClass);
                if(_star_so == null)
                    continue;
                EnumDefOperations _star_self = (EnumDefOperations)_star_so.servant;
                try
                {
                    return _star_self.describe();
                }
                finally
                {
                    _servant_postinvoke(_star_so);
                }
            }
        }
    }

    //
    // IDL:omg.org/CORBA/Contained/move:1.0
    //
    public void
    move(Container _star_a0,
         String _star_a1,
         String _star_a2)
    {
        while(true)
        {
            if(!this._is_local())
            {
                org.omg.CORBA.portable.OutputStream out = null;
                org.omg.CORBA.portable.InputStream in = null;
                try
                {
                    out = _request("move", true);
                    ContainerHelper.write(out, _star_a0);
                    IdentifierHelper.write(out, _star_a1);
                    VersionSpecHelper.write(out, _star_a2);
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
                org.omg.CORBA.portable.ServantObject _star_so = _servant_preinvoke("move", _star_opsClass);
                if(_star_so == null)
                    continue;
                EnumDefOperations _star_self = (EnumDefOperations)_star_so.servant;
                try
                {
                    _star_self.move(_star_a0, _star_a1, _star_a2);
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
                EnumDefOperations _star_self = (EnumDefOperations)_star_so.servant;
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
