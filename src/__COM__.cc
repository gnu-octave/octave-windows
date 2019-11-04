/*

  Copyright (C) 2006-2018 Michael Goffioul <michael.goffioul@swing.be>
  Copyright (C) 2019 John DOnoghue <john.donoghue@ieee.org>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; If not, see <http://www.gnu.org/licenses/>.

*/

#include <octave/oct.h>
#include <octave/ov.h>
#include <octave/ov-base.h>
#include <octave/ov-typeinfo.h>
#include <octave/defun-dld.h>
#include <octave/Cell.h>
#include <octave/parse.h>

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#ifdef USING_WINDOWS
#include <windows.h>
#include <ocidl.h>

#if 0
#define DEBUGF(x) printf x
#else
#define DEBUGF(x)
#endif

static std::wstring
string_to_wstring (const std::string& s)
{
  const char* cs = s.c_str ();
  int len = s.length ();

  if (len == 0)
    return std::wstring (L"");

  std::vector<wchar_t> tmp (len);
  std::use_facet<std::ctype<wchar_t> >(std::locale ()).widen (
    cs, cs+len, &tmp[0]);
  return std::wstring (&tmp[0], len);
}

static std::string
wstring_to_string (const std::wstring& ws)
{
  const wchar_t* wcs = ws.c_str ();
  int len = ws.length ();

  if (len == 0)
    return std::string ("");

  std::vector<char> tmp (len);
  std::use_facet<std::ctype<wchar_t> >(std::locale ()).narrow (
      wcs, wcs+len, ' ', &tmp[0]);
  return std::string (&tmp[0], len);
}

class octave_com_object : public octave_base_value
{
public:
  // Constructors
  octave_com_object (void)
  : iface (NULL), com_typename ("")
  {
  }

  octave_com_object (const octave_com_object& obj)
  : iface (obj.iface), com_typename (obj.com_typename)
  {
    iface->AddRef ();
  }

  octave_com_object (IDispatch *disp, bool ref = true)
  : iface (disp), com_typename ("")
  {
    if (ref)
      iface->AddRef ();
    init_typename ();
  }

  // Destructor
  ~octave_com_object (void)
  {
    com_delete ();
  }

  // Utility methods
  IDispatch* com_iface (void) const { return iface; }

  void com_release (void)
  {
    if (iface != NULL)
      {
        DEBUGF (("releasing COM object: 0x%p\n", iface));
        iface->Release ();
        iface = NULL;
        com_typename = "";
      }
  }

  void com_delete (void)
  {
    // For the time being, simply release the current interface.
    // In the long term, this function should free all interface
    // held for the current COM object (if it's a COM object)
    com_release ();
  }

  void init_typename (void)
  {
    if (iface == NULL)
      return;

    ITypeInfo *ti = NULL;
    IProvideClassInfo *ci = NULL;
    unsigned int tiCount = 0;
    HRESULT hr;

    if ((hr=iface->QueryInterface (IID_IProvideClassInfo, (void**)&ci)) == S_OK)
      {
        ci->GetClassInfo (&ti);
        ci->Release ();
      }
    if (ti == NULL && (hr=iface->GetTypeInfoCount (&tiCount)) == S_OK && tiCount == 1)
      {
        hr = iface->GetTypeInfo (0, LOCALE_USER_DEFAULT, &ti);
      }

    if (ti != NULL)
      {
        BSTR name;
        if ((hr=ti->GetDocumentation (MEMBERID_NIL, &name, NULL, NULL, NULL)) == S_OK)
          {
            com_typename = wstring_to_string (std::wstring(name));
            SysFreeString (name);
          }
        ti->Release ();
      }
  }

  // octave_base_value members overloading
  octave_base_value* clone (void) const { return new octave_com_object(*this); }
  octave_base_value* empty_clone (void) const { return new octave_com_object(); }

  bool is_defined (void) const { return true; }

  bool isobject (void) const { return true; }

  bool is_map (void) const { return true; }

  string_vector map_keys (void) const;

  dim_vector dims (void) const { static dim_vector dv(1, 1); return dv; }

  void print_raw (std::ostream& os, bool pr_as_read_syntax = false) const
  {
    os << "<COM object " 
       << (com_typename.empty () ? std::string("Unknown") : com_typename) 
       << " (0x" << (void*)iface << ")>";
  }

  void print (std::ostream& os, bool pr_as_read_syntax = false)
  {
    print_raw (os, pr_as_read_syntax);
    newline (os);
  }

  void print (std::ostream& os, bool pr_as_read_syntax = false) const
  {
    print_raw (os, pr_as_read_syntax);
    newline (os);
  }

  octave_value_list subsref (const std::string& type, const std::list<octave_value_list>& idx, int nargout);

  octave_value subsref (const std::string& type, const std::list<octave_value_list>& idx)
  {
    octave_value_list retval = subsref (type, idx, 1);
    return (retval.length () > 0 ? retval(0) : octave_value ());
  }

  octave_value subsasgn (const std::string& type, const std::list<octave_value_list>& idx, const octave_value& rhs);

private:
  // regular octave value declarations
#if defined (DEFINE_OCTAVE_ALLOCATOR)	
  DECLARE_OCTAVE_ALLOCATOR
#endif

  DECLARE_OV_TYPEID_FUNCTIONS_AND_DATA

  // specific COM declarations
  IDispatch *iface;
  std::string com_typename;
};

#if defined (DEFINE_OCTAVE_ALLOCATOR)
DEFINE_OCTAVE_ALLOCATOR (octave_com_object);
#endif

DEFINE_OV_TYPEID_FUNCTIONS_AND_DATA (
    octave_com_object,
    "octave_com_object",
    "octave_com_object");

#define OV_COMOBJ(ov) dynamic_cast<octave_com_object*>((ov).internal_rep())

octave_value_list
octave_com_object::subsref (const std::string& type, const std::list<octave_value_list>& idx, int nargout)
{
  octave_value_list retval;
  int skip = 1;

  switch (type[0])
  {
  default:
    error ("COM object cannot be indexed with %c", type[0]);
    break;
  case '.':
    if (type.length() > 1 && type[1] == '(')
      {
        // normal method case
        octave_value_list ovl;
        count++;
        ovl(0) = octave_value (this);
        ovl(1) = (idx.front ()) (0);
        std::list<octave_value_list>::const_iterator it = idx.begin ();
        ovl.append (*++it);
        retval = OCTAVE__FEVAL (std::string ("com_invoke"), ovl, 1);
        skip++;
      }
    else
      {
        // normal property case
        octave_value_list ovl;
        count++;
        ovl(0) = octave_value (this);
        ovl(1) = (idx.front ())(0);
        retval = OCTAVE__FEVAL (std::string ("com_get"), ovl, 1);
      }
    break;
  }

  if (idx.size () > 1 && type.length () > 1)
    retval = retval (0).next_subsref (nargout, type, idx, skip);

  return retval;
}

octave_value
octave_com_object::subsasgn (const std::string& type, const std::list<octave_value_list>& idx, const octave_value& rhs)
{
  octave_value retval;

  switch (type[0])
  {
  default:
    error ("COM object cannot be indexed with %c", type[0]);
    break;
  case '.':
    if (type.length () == 1)
      {
        // property assignment
        octave_value_list ovl;
        count++;
        ovl (0) = octave_value (this);
        ovl (1) = (idx.front ()) (0);
        ovl (2) = rhs;
        OCTAVE__FEVAL ("com_set", ovl, 0);
        count++;
        retval = octave_value (this);
      }
    else if (type.length () > 2 && type[1] == '(')
      {
        // invoke method and continue assignment
        std::list<octave_value_list> new_idx;
        std::list<octave_value_list>::const_iterator it = idx.begin ();
        new_idx.push_back (*it++);
        new_idx.push_back (*it++);
        octave_value_list u = subsref (type.substr (0, 2), new_idx, 1);
        if (u.length())
          {
            std::list<octave_value_list> next_idx (idx);
            next_idx.erase (next_idx.begin ());
            next_idx.erase (next_idx.begin ());
            u (0).subsasgn (type.substr (2), next_idx, rhs);
            count++;
            retval = octave_value (this);
          }
      }
    else if (type.length () > 1 && type[1] == '.')
      {
        // get property and continue assignment
        octave_value_list u = subsref (type.substr (0, 1), idx, 1);
        if (u.length())
          {
            std::list<octave_value_list> next_idx (idx);
            next_idx.erase (next_idx.begin ());
            u (0).subsasgn(type.substr (1), next_idx, rhs);
            count++;
            retval = octave_value (this);
          }
      }
    else
      error ("Invalid assignment on COM object");
    break;
  }

  return retval;
}

static bool initialized = false;

static void
initialize_com()
{
  static bool type_registered = false;
  if (!initialized)
    {
      // COM initialization
      CoInitialize (NULL);

      if (! type_registered)
        {
          // Register new octave object class
          type_registered = true;
          octave_com_object::register_type ();
        }

      initialized = true;
    }
}

static void
terminate_com()
{
  if (initialized)
    {
      // COM uninitialization
      CoUninitialize ();

      initialized = false;
    }
}
#endif

// PKG_ADD: autoload ("com_atexit", which ("__COM__"));
// xPKG_ADD: #atexit ("com_atexit");
DEFUN_DLD(com_atexit, args, , 
          "-*- texinfo -*-\n \
@deftypefn {Loadable Function} {} com_atexit ()\n \
Close down all GNU Octave managed COM handles.\n \
\n \
Called during pkg unload.\n \
@end deftypefn")
{
#ifndef USING_WINDOWS
  error ("com_atexit: Your system doesn't support the COM interface");
#else
  terminate_com ();
#endif
  return octave_value ();
}

// PKG_ADD: autoload ("actxserver", which ("__COM__"));
DEFUN_DLD(actxserver, args, , 
          "-*- texinfo -*-\n \
@deftypefn {Loadable Function} {@var{h} =} actxserver (@var{progid})\n \
\n \
Create a COM server using the @var{progid} identifier.\n \
\n \
Returns @var{h}, a handle to the default interface of the COM server.\n \
\n \
Example:\n \
\n \
@example\n \
@group\n \
# create a COM server running Microsoft Excel\n \
app = actxserver ('Excel.Application');\n \
# free the object\n \
destroy (app); \n \
@end group\n \
@end example\n \
@end deftypefn")
{
  octave_value retval;

#ifndef USING_WINDOWS
  error ("actxserver: Your system doesn't support the COM interface");
#else
  initialize_com ();

  if (args.length () == 1)
    {
      if (args (0).is_string ())
        {
          std::wstring progID = string_to_wstring (args (0).string_value ());
          CLSID clsID;
          IDispatch *disp;

          if (CLSIDFromProgID (progID.c_str (), &clsID) != S_OK)
            {
              error ("actxserver: unknown ActiveX server `%s'", args (0).string_value ().c_str ());
              return retval;
            }
          if (CoCreateInstance (clsID, NULL, CLSCTX_INPROC_SERVER|CLSCTX_LOCAL_SERVER,
                                IID_IDispatch, (void**)&disp) != S_OK)
            {
              error ("actxserver: unable to create server instance for `%s'",
                      args (0).string_value ().c_str ());
              return retval;
            }

          retval = octave_value (new octave_com_object (disp, false));
        }
      else
        error ("actxserver: invalid ActiveX server name");
    }
  else
    print_usage ();
#endif
  return retval;
}

#ifdef USING_WINDOWS

template<typename T>
octave_value 
safearray_to_octave_value(const dim_vector& dv, SAFEARRAY *parray)
{
  T *pvalues;
  HRESULT hr;
  Array<T> cell(dv);

  hr = SafeArrayAccessData(parray, (void **)&pvalues);
  if (FAILED(hr))
  {
    warning("failed accessing array data");
    return octave_value(Matrix()); /*return invalid data */
  }

  for (int k = 0; k < cell.numel(); k++) {
    cell(k) = pvalues[k];
  }
  SafeArrayUnaccessData(parray);

  return octave_value(cell);
}

static octave_value
com_to_octave (VARIANT *var)
{
  octave_value retval;

  switch (var->vt)
  {
  case VT_DISPATCH:
    retval = octave_value (new octave_com_object (var->pdispVal, true));
    break;
  case VT_BOOL:
    retval = octave_value (var->boolVal == VARIANT_TRUE ? true : false);
    break;
  case VT_I1:
    retval = octave_value (var->cVal);
    break;
  case VT_UI1:
    retval = octave_value (var->bVal);
    break;
  case VT_I2:
    retval = octave_value (var->iVal);
    break;
  case VT_UI2:
    retval = octave_value (var->uiVal);
    break;
  case VT_I4:
    retval = octave_value (var->lVal);
    break;
  case VT_UI4:
    retval = octave_value (var->ulVal);
    break;
  case VT_I8:
    retval = octave_value (var->llVal);
    break;
  case VT_INT:
    retval = octave_value (var->intVal);
    break;
  case VT_UINT:
    retval = octave_value (var->uintVal);
    break;
  case VT_BSTR:
    retval = octave_value (wstring_to_string (std::wstring (var->bstrVal)));
    break;
  case VT_R8:
    retval = octave_value (var->dblVal);
    break;
  case VT_DATE:
    retval = octave_value (var->dblVal);
    break;
  case VT_R4:
    retval = octave_value (var->fltVal);
    break;
  case VT_EMPTY:
    retval = octave_value (Matrix());
    break;
  case VT_ERROR:
    retval = octave_value (Matrix());
    break;
  default:
    if (var->vt & VT_ARRAY)
      {
        int subtype = var->vt & VT_TYPEMASK;
        SAFEARRAY *arr = var->parray;
        dim_vector dv;
        int dimCount;

        dimCount = SafeArrayGetDim (arr);
        dv.resize (dimCount+1);
        if (dimCount>2)
          warning ("maximum array dimensions supported is 2");

        for (int k=0; k<dimCount; k++)
          {
            long lb, ub;
            SafeArrayGetLBound (arr, k+1, &lb);
            SafeArrayGetUBound (arr, k+1, &ub);
            dv (k) = ub-lb+1;
          }
        dv (dimCount) = 1; // allows 1-D arrays to work

        switch (subtype)
        {
        case VT_BSTR:
          {
            VARIANT *pvar;
            HRESULT hr;
            BSTR cellVal;
            long arrDims[2] = {0,0};
            Cell cell (dv);

            hr=SafeArrayAccessData (arr, (void **)&pvar);

            if (FAILED (hr))
              {
                warning("failed accessing array data");
                retval = octave_value (Matrix ()); // return invalid data
                break;
              }
            else
              {
                for (int k=0; k<cell.numel (); k++)
                  {
                    arrDims[0] = k;
                    if (SafeArrayGetElement (arr,arrDims,&cellVal)==S_OK)
                      {
                       // retval=octave_value (cellVal); // retval used as temporary value
                       cell (k) = octave_value (wstring_to_string (std::wstring ( cellVal)));
                      }
                    else 
                      {
                        warning ("error accessing value (%d)", k);
                        // returns whatever data we already have
                        break;
                      }
                  }

                retval = octave_value (cell); // return invalid data
              }

          }
          break;
        case VT_VARIANT:
          {
            VARIANT *pvar;
            HRESULT hr;
            Cell cell (dv);

            hr=SafeArrayAccessData (arr, (void **)&pvar);
            if (FAILED (hr))
              {
                warning ("failed accessing array data");
                retval = octave_value (Matrix ()); // return invalid data
                break;
              }
            else
              {
                for (int k=0; k<cell.numel (); k++)
                  {
                    cell (k) = com_to_octave (&pvar[k]);
                  }
                SafeArrayUnaccessData (arr);
              }
            retval = octave_value (cell);
          }
          break;
        case VT_R4:
          retval = safearray_to_octave_value<float>(dv, arr);
          break;
        case VT_R8:
          retval = safearray_to_octave_value<double>(dv, arr);
          break;
        case VT_I1:
          retval = safearray_to_octave_value<octave_int8>(dv, arr);
          break;
        case VT_UI1:
          retval = safearray_to_octave_value<octave_uint8>(dv, arr);
          break;
        case VT_I2:
          retval = safearray_to_octave_value<octave_int16>(dv, arr);
          break;
        case VT_UI2:
          retval = safearray_to_octave_value<octave_uint16>(dv, arr);
          break;
        case VT_I4:
          retval = safearray_to_octave_value<octave_int32>(dv, arr);
          break;
        case VT_UI4:
          retval = safearray_to_octave_value<octave_uint32>(dv, arr);
          break;
        default:
          {
            warning ("cannot convert COM array of type `0x%x' to octave object", subtype);
            retval = octave_value (Matrix ()); // return invalid data
          }
          break;
        }
      }
    else // if not (var->vt & VT_ARRAY)
      {
        warning ("cannot convert COM variant of type `0x%x' to octave object", var->vt);
        retval = octave_value (Matrix ()); // return invalid data
      }
    break;
  }

  return retval;
}

static SAFEARRAY*
make_safearray_from_dims(const dim_vector& dv, VARTYPE vt)
{
  SAFEARRAY *arr;
  SAFEARRAYBOUND *bounds;

  bounds = (SAFEARRAYBOUND*)LocalAlloc (LMEM_FIXED|LMEM_ZEROINIT, sizeof (SAFEARRAYBOUND)*dv.length ());
  for (int k=0; k<dv.length (); k++)
    bounds[k].cElements = dv (k);
  arr = SafeArrayCreate (vt, dv.length (), bounds);
  LocalFree (bounds);

  return arr;
}

static void
octave_to_com(const octave_value& ov, VARIANT *var)
{
  VariantInit (var);

  if (ov.is_string ())
  {
    var->vt = VT_BSTR;
    var->bstrVal = SysAllocString (string_to_wstring (ov.string_value ()).c_str ());
  }
  else if (ov.is_bool_scalar ())
  {
    var->vt = VT_BOOL;
    var->boolVal = (ov.bool_value () ? VARIANT_TRUE : VARIANT_FALSE);
  }
  else if (ov.is_real_scalar ())
  {
    var->vt = VT_R8;
    var->dblVal = ov.double_value ();
  }
  else if (ov.is_real_matrix () && ov.OV_ISEMPTY ())
  {
    var->vt = VT_EMPTY;
  }
  else if (ov.is_real_matrix ())
  {
    NDArray M = ov.array_value ();
    SAFEARRAY *arr = make_safearray_from_dims (M.dims (), VT_R8);
    double *data;

    SafeArrayAccessData (arr, (void**)&data);
    for (int k=0; k<M.numel (); k++)
      data[k] = M (k);
    SafeArrayUnaccessData (arr);

    var->vt = VT_ARRAY|VT_R8;
    var->parray = arr;
  }
  else if (ov.OV_ISCELL ())
  {
    Cell M = ov.cell_value ();
    SAFEARRAY *arr = make_safearray_from_dims (M.dims (), VT_VARIANT);
    VARIANT *data;

    SafeArrayAccessData (arr, (void**)&data);
    for (int k=0; k<M.numel (); k++)
      octave_to_com (M (k), &data[k]);
    SafeArrayUnaccessData (arr);

    var->vt = VT_ARRAY|VT_VARIANT;
    var->parray = arr;
  }
  else if (ov.class_name () == "octave_com_object")
  {
    var->vt = VT_DISPATCH;
    var->pdispVal = OV_COMOBJ (ov)->com_iface ();
    var->pdispVal->AddRef ();
  }
  else
    warning ("cannot convert octave object of class `%s' into COM variant", ov.class_name ().c_str ());
}

static octave_value
do_invoke (const char *fname, WORD flag, const octave_value_list& args)
{
  octave_value retval;
  octave_com_object *com = OV_COMOBJ (args (0));
  std::wstring method_name;
  wchar_t *method_wstr;
  DISPID method_ID, propPutID = DISPID_PROPERTYPUT;
  VARIANT result, *vargs;
  DISPPARAMS dispParams = { NULL, NULL, 0, 0 };
  HRESULT hr;

  // Get method dispatch ID
  method_name = string_to_wstring (args (1).string_value ());
  if (method_name.length() == 0)
    {
      error ("%s: invalid property/method name as argument 2", fname);
      return retval;
    }
  method_wstr = const_cast<wchar_t*>(method_name.c_str ());
  if (com->com_iface ()->GetIDsOfNames (IID_NULL, &method_wstr, 1, LOCALE_USER_DEFAULT, &method_ID) != S_OK)
    {
      error ("%s: unknown property/method name `%s'", fname, args (1).string_value ().c_str ());
      return retval;
    }

  // Convert arguments to COM equivalent
  dispParams.cArgs = args.length ()-2;
  vargs = (VARIANT*)LocalAlloc (LMEM_FIXED|LMEM_ZEROINIT, sizeof(VARIANT)*dispParams.cArgs);
  for (int k=2, v=dispParams.cArgs-1; v>=0; k++, v--)
    octave_to_com (args (k), &vargs[v]);
  dispParams.rgvarg = vargs;
  if (flag & DISPATCH_PROPERTYPUT)
    {
      dispParams.cNamedArgs = 1;
      dispParams.rgdispidNamedArgs = &propPutID;
    }

  // Invoke property/method on the COM object
  VariantInit (&result);
  if ((hr = com->com_iface ()->Invoke (method_ID, IID_NULL, LOCALE_USER_DEFAULT, flag, &dispParams,
                                       &result, NULL, NULL)) != S_OK)
    {
      error ("%s: property/method invocation on the COM object failed with error `0x%08x'", fname, hr);
      goto cleanup;
    }

  // Convert invocation result into octave object
  retval = com_to_octave (&result);

  // Clean-up
cleanup:
  VariantClear (&result);
  for (int k=0; k<args.length ()-2; k++)
    VariantClear (&vargs[k]);
  LocalFree (vargs);

  return retval;
}

static string_vector
do_invoke_list (const char *fname, WORD flag, const octave_com_object *com)
{
  ITypeInfo *ti;
  unsigned int tiCount;
  HRESULT hr;
  std::list<std::string> name_list;

  if ((hr=com->com_iface ()->GetTypeInfoCount (&tiCount)) == S_OK && tiCount == 1)
    {
      TYPEATTR *pAttr;

      hr = com->com_iface ()->GetTypeInfo (0, LOCALE_USER_DEFAULT, &ti);
      hr = ti->GetTypeAttr (&pAttr);
      for (int k=0; k<pAttr->cFuncs; k++)
        {
          FUNCDESC *pFuncDesc;
          BSTR name;
          hr = ti->GetFuncDesc (k, &pFuncDesc);
          hr = ti->GetDocumentation (pFuncDesc->memid, &name, NULL, NULL, NULL);
          if (pFuncDesc->invkind & flag)
            name_list.push_back (wstring_to_string (std::wstring (name)));
          SysFreeString (name);
          ti->ReleaseFuncDesc( pFuncDesc);
        }
      ti->ReleaseTypeAttr (pAttr);
    }

  string_vector v (name_list);
  return v.sort (true);
}

string_vector
octave_com_object::map_keys (void) const
{
  return do_invoke_list ("map_keys", DISPATCH_METHOD|DISPATCH_PROPERTYGET|DISPATCH_PROPERTYPUT, this);
}
#endif

// PKG_ADD: autoload ("com_get", which ("__COM__"));
DEFUN_DLD(com_get, args, ,
          "-*- texinfo -*-\n \
@deftypefn {Loadable Function} { @var{S} = } com_get (@var{obj})\n \
Call get function on COM object @var{obj}. Returns any result in @var{S}\n \
\n \
@end deftypefn")
{
  octave_value retval;

#ifndef USING_WINDOWS
  error ("com_get: Your system doesn't support the COM interface");
#else
  initialize_com ();

  if (args.length () < 1 || args (0).class_name () != "octave_com_object")
    {
      print_usage ();
      return retval;
    }

  if (args.length () == 1)
    retval = octave_value (Cell (do_invoke_list ("com_get", DISPATCH_PROPERTYGET, OV_COMOBJ (args (0)))));
  else
    retval = do_invoke ("com_get", DISPATCH_PROPERTYGET, args);
#endif
  return retval;
}

// PKG_ADD: autoload ("com_set", which ("__COM__"));
DEFUN_DLD(com_set, args, , 
          "-*- texinfo -*-\n \
@deftypefn {Loadable Function} { @var{S} = } com_set (@var{obj}, @var{propname}, @var{value})\n \
Call set function on COM object @var{obj} to set property @var{propname} to value @var{value}. Returns any result in @var{S}\n \
\n \
@end deftypefn")
{
  octave_value retval;
#ifndef USING_WINDOWS
  error ("com_set: Your system doesn't support the COM interface");
#else

  initialize_com ();

  if (args.length () < 3 || args (0).class_name () != "octave_com_object")
    {
      print_usage ();
      return retval;
    }

  retval = do_invoke ("com_set", DISPATCH_PROPERTYPUT, args);
#endif
  return retval;
}

// PKG_ADD: autoload ("com_invoke", which ("__COM__"));
DEFUN_DLD(com_invoke, args, ,
          "-*- texinfo -*-\n \
@deftypefn {Loadable Function} { @var{result} = } com_invoke (@var{obj})\n \
@deftypefnx {Loadable Function} { @var{result} = } com_invoke (@var{obj}, @var{method} )\n \
\n \
Call invoke on @var{obj} to run a method, or obtain a list of all methods.\n \
\n \
@code{com_invoke (@var{obj})} returns a list of all methods available for object @var{obj} in @var{result}.\n \
\n \
@code{com_invoke (@var{obj}, @var{method} )} invokes @var{method} method for object @var{obj} and returns result @var{result}.\n \
\n \
@end deftypefn")
{
  octave_value retval;

#ifndef USING_WINDOWS
  error ("com_invoke: Your system doesn't support the COM interface");
#else
  initialize_com ();

  if (args.length () < 1 || args (0).class_name () != "octave_com_object")
    {
      print_usage();
      return retval;
    }

  if (args.length () == 1)
    retval = octave_value (Cell (do_invoke_list ("com_get", DISPATCH_METHOD, OV_COMOBJ (args (0)))));
  else
    retval = do_invoke ("com_invoke", DISPATCH_METHOD|DISPATCH_PROPERTYGET, args);
#endif
  return retval;
}

// PKG_ADD: autoload ("com_delete", which ("__COM__"));
DEFUN_DLD(com_delete, args, , 
          "-*- texinfo -*-\n \
@deftypefn {Loadable Function} { } com_delete (@var{obj})\n \
Release interfaces from COM object @var{obj} and then delete the COM server\n \
\n \
@end deftypefn")
{
  octave_value retval;

#ifndef USING_WINDOWS
  error ("com_delete: Your system doesn't support the COM interface");
#else
  initialize_com ();

  if (args.length () != 1 || args (0).class_name () != "octave_com_object")
    {
      error ("com_delete: first argument must be a COM object");
      return retval;
    }

  OV_COMOBJ (args (0))->com_delete ();
#endif
  return retval;
}

// PKG_ADD: autoload ("com_release", which ("__COM__"));
DEFUN_DLD(com_release, args, ,
          "-*- texinfo -*-\n \
@deftypefn {Loadable Function} { } com_release (@var{obj})\n \
Release interfaces from COM object @var{obj}\n \
\n \
@end deftypefn")
{
  octave_value retval;

#ifndef USING_WINDOWS
  error ("com_release: Your system doesn't support the COM interface");
#else
  initialize_com ();

  if (args.length () != 1 || args (0).class_name () != "octave_com_object")
    {
      error ("com_release: first argument must be a COM object");
      return retval;
    }

  OV_COMOBJ (args (0))->com_release ();
#endif
  return retval;
}

DEFUN_DLD(__COM__, args, , "internal function")
{
  octave_value retval;
#ifndef USING_WINDOWS
  error ("__COM__: Your system doesn't support the COM interface");
#else
  initialize_com ();
#endif
  return retval;
}

// PKG_ADD: autoload ("__windows_pkg_lock__", which ("__COM__"));
// PKG_ADD: __windows_pkg_lock__(1);
// PKG_DEL: __windows_pkg_lock__(0);
#ifdef DEFMETHOD_DLD
DEFMETHOD_DLD (__windows_pkg_lock__, interp, args, , "internal function")
{
  octave_value retval;
  if (args.length () >= 1)
    {
      if (args(0).int_value () == 1)
        interp.mlock();
      else if (args(0).int_value () == 0 &&  interp.mislocked("__windows_pkg_lock__"))
        interp.munlock("__windows_pkg_lock__");
    }
  return retval;
}
#else
DEFUN_DLD(__windows_pkg_lock__, args, ,  "internal function")
{
  octave_value retval;
  return retval;
}
#endif

#if 0
%!testif HAVE_WINDOWS_H
%! wshell = actxserver ("WScript.Shell");
%! assert (!isempty (wshell));
#! assert (strcmp (wshell.CurrentDirectory, pwd));

%!testif HAVE_WINDOWS_H
%! wshell = actxserver ("WScript.Shell");
%! # type REG_SZ
%! val = wshell.RegRead('HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\CurrentVersion');
%! assert (isa (val, 'char'));
%! # type REG_DWORD
%! val = wshell.RegRead('HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\InstallDate');
%! assert (isscalar (val));
%! # type REG_BINARY
%! val = wshell.RegRead('HKLM\SOFTWARE\Microsoft\DirectX\InstalledVersion');
%! assert (iscell (val));
%! assert (length (val) > 0);
%! # type REG_EXPANDSZ
%! val = wshell.RegRead('HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\ProgramFilesPath');
%! assert (isa (val, 'char'));

%!testif HAVE_WINDOWS_H
%! fail ("actxserver(0)", "invalid ActiveX server name");
#endif
