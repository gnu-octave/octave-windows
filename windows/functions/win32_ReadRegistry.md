---
layout: "default"
permalink: "/functions/18_win32ReadRegistry/"
pkg_name: "windows"
pkg_version: "1.6.5"
pkg_description: "Provides COM interface and additional functionality on Windows"
title: "Windows Toolkit - win32_ReadRegistry"
category: "Windows Utilities"
func_name: "win32_ReadRegistry"
navigation:
- id: "overview"
  name: "Overview"
  url: "/index"
- id: "Functions"
  name: "Function Reference"
  url: "/functions"
- id: "17_WindowsUtilities"
  name: "&nbsp;&nbsp;Windows Utilities"
  url: "/functions/#17_WindowsUtilities"
- id: "13_COMInterface"
  name: "&nbsp;&nbsp;COM Interface"
  url: "/functions/#13_COMInterface"
- id: "8_Features"
  name: "&nbsp;&nbsp;Features"
  url: "/functions/#8_Features"
- id: "8_Examples"
  name: "&nbsp;&nbsp;Examples"
  url: "/functions/#8_Examples"
- id: "news"
  name: "News"
  url: "/news"
- id: "manual"
  name: "Manual"
  url: "/manual"
---
<dl class="first-deftypefn">
<dt class="deftypefn" id="index-win32_005fReadRegistry"><span class="category-def">Loadable Function: </span><span><code class="def-type">[ <var class="var">rv</var>, <var class="var">code</var> ] =</code> <strong class="def-name">win32_ReadRegistry</strong> <code class="def-code-arguments">(<var class="var">key</var>, <var class="var">subkey</var>, <var class="var">value</var>)</code><a class="copiable-link" href="#index-win32_005fReadRegistry"></a></span></dt>
<dd> 
<p>Read a value from the Windows registry.
</p> 
<p>Example:
 </p><div class="example">
<pre class="example-preformatted"> key='SOFTWARE\\Cygnus Solutions\\Cygwin\\mounts v2';
 win32_ReadRegistry('HKLM',key,'cygdrive prefix')
 </pre></div>
 
<p>key must be one of the following strings:
 </p><dl class="table">
<dt>HKCR</dt>
<dd><p>HKEY_CLASSES_ROOT
 </p></dd>
<dt>HKCU</dt>
<dd><p>HKEY_CURRENT_USER
 </p></dd>
<dt>HKLM</dt>
<dd><p>HKEY_LOCAL_MACHINE
 </p></dd>
<dt>HKU</dt>
<dd><p>HKEY_USERS
 </p></dd>
</dl>
 
<p><var class="var">rv</var> is an octave string of the returned bytes.
 This is a natural format for REG_SZ data; however, 
 if the registry data was in another format, REG_DWORD
 then the calling program will need to process them
</p> 
<p><var class="var">code</var> is the success code. Values correspond to the
 codes in the winerror.h header file. The code of 0 is
 success, while other codes indicate failure
 In the case of failure, &rsquo;rv&rsquo; will be empty
 </p></dd></dl>