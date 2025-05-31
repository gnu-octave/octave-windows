---
layout: "default"
permalink: "/functions/@octave_com_object/25_octavecomobjectinvoke/"
pkg_name: "windows"
pkg_version: "1.6.5"
pkg_description: "Provides COM interface and additional functionality on Windows"
title: "Windows Toolkit - @octave_com_object/invoke"
category: "COM Interface"
func_name: "@octave_com_object/invoke"
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
<dt class="deftypefn" id="index-invoke"><span class="category-def">: </span><span><strong class="def-name">invoke</strong> <code class="def-code-arguments">(<var class="var">obj</var>)</code><a class="copiable-link" href="#index-invoke"></a></span></dt>
<dt class="deftypefnx def-cmd-deftypefn" id="index-invoke-1"><span class="category-def">: </span><span><code class="def-type"><var class="var">S</var> =</code> <strong class="def-name">invoke</strong> <code class="def-code-arguments">(<var class="var">obj</var>, <var class="var">methodname</var>)</code><a class="copiable-link" href="#index-invoke-1"></a></span></dt>
<dt class="deftypefnx def-cmd-deftypefn" id="index-invoke-2"><span class="category-def">: </span><span><code class="def-type"><var class="var">S</var> =</code> <strong class="def-name">invoke</strong> <code class="def-code-arguments">(<var class="var">obj</var>, <var class="var">methodname</var>, <var class="var">arg1</var>, &hellip;, <var class="var">argN</var>)</code><a class="copiable-link" href="#index-invoke-2"></a></span></dt>
<dd><p>Invoke a method on a COM object.
</p>
<p>When called with just the single <var class="var">obj</var>, invoke displays the methods available to the object.
 When called with <var class="var">methodname</var>, invoke will invoke the method with optional args and return
 the result in <var class="var">S</var>.
</p>

<p><strong class="strong">See also:</strong> com_invoke, methods.
 </p></dd></dl>