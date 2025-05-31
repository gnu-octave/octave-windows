---
layout: "default"
permalink: "/functions/19_win32WriteRegistry/"
pkg_name: "windows"
pkg_version: "1.6.5"
pkg_description: "Provides COM interface and additional functionality on Windows"
title: "Windows Toolkit - win32_WriteRegistry"
category: "Windows Utilities"
func_name: "win32_WriteRegistry"
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
<dt class="deftypefn" id="index-win32_005fWriteRegistry"><span class="category-def">Loadable Function: </span><span><code class="def-type"><var class="var">code</var> =</code> <strong class="def-name">win32_WriteRegistry</strong> <code class="def-code-arguments">(<var class="var">key</var>, <var class="var">subkey</var>, <var class="var">valuename</var>, <var class="var">value</var>)</code><a class="copiable-link" href="#index-win32_005fWriteRegistry"></a></span></dt>
<dd> 
<p>Write a value to the Windows registry.
</p> 
<p>Example:
 </p><div class="example">
<pre class="example-preformatted"> key='test\\temp';
 win32_WriteRegistry('HKLM',key,'test_value', 0)
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
 
<p><var class="var">subkey</var> is the subkey to the registry value.
</p> 
<p><var class="var">valuename</var> is the name of the value to write to the registry.
</p> 
<p><var class="var">value</var> is the value to write. It must be a a string or an integer value.
</p> 
<p><var class="var">code</var> is the success code. Values correspond to the
 codes in the winerror.h header file. The code of 0 is
 success, while other codes indicate failure
 </p></dd></dl>