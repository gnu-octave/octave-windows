---
layout: "default"
permalink: "/functions/18_win32RegEnumValue/"
pkg_name: "windows"
pkg_version: "1.7.0"
pkg_description: "Provides COM interface and additional functionality on Windows"
title: "Windows Toolkit - win32_RegEnumValue"
category: "Examples"
func_name: "win32_RegEnumValue"
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
  subitems:
- id: "13_COMInterface"
  name: "&nbsp;&nbsp;COM Interface"
  url: "/functions/#13_COMInterface"
  subitems:
- id: "8_Features"
  name: "&nbsp;&nbsp;Features"
  url: "/functions/#8_Features"
  subitems:
- id: "8_Examples"
  name: "&nbsp;&nbsp;Examples"
  url: "/functions/#8_Examples"
  subitems:
- id: "news"
  name: "News"
  url: "/news"
- id: "manual"
  name: "Manual"
  url: "/manual"
---
<dl class="first-deftypefn def-block">
<dt class="deftypefn def-line" id="index-win32_005fRegEnumValue"><span class="category-def">: </span><span><code class="def-type">[ <var class="var">rv</var>, <var class="var">code</var> ] =</code> <strong class="def-name">win32_RegEnumValue</strong> <code class="def-code-arguments">(<var class="var">key</var>, <var class="var">subkey</var>)</code><a class="copiable-link" href="#index-win32_005fRegEnumValue"></a></span></dt>
<dd> 
<p>Read value names from from the Windows registry.
</p> 
<p>Example:
 </p><div class="example">
<pre class="example-preformatted"> key='SOFTWARE\\Cygnus Solutions\\Cygwin\\mounts v2';
 win32_RegEnumValue('HKLM',key)
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
 
<p><var class="var">rv</var> is an array of value strings for the name of values
 for a given key and subkey.
</p> 
<p><var class="var">code</var> is the success code. Values correspond to the
 codes in the winerror.h header file. The code of 0 is
 success, while other codes indicate failure
 In the case of failure, &rsquo;rv&rsquo; will be empty
</p> 

<p><strong class="strong">See also:</strong> winqueryreg.
 </p></dd></dl>