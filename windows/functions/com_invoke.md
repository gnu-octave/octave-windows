---
layout: "default"
permalink: "/functions/10_cominvoke/"
pkg_name: "windows"
pkg_version: "1.6.5"
pkg_description: "Provides COM interface and additional functionality on Windows"
title: "Windows Toolkit - com_invoke"
category: "COM Interface"
func_name: "com_invoke"
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
<dt class="deftypefn" id="index-com_005finvoke"><span class="category-def">Loadable Function: </span><span><code class="def-type"><var class="var">result</var> =</code> <strong class="def-name">com_invoke</strong> <code class="def-code-arguments">(<var class="var">obj</var>)</code><a class="copiable-link" href="#index-com_005finvoke"></a></span></dt>
<dt class="deftypefnx def-cmd-deftypefn" id="index-com_005finvoke-1"><span class="category-def">Loadable Function: </span><span><code class="def-type"><var class="var">result</var> =</code> <strong class="def-name">com_invoke</strong> <code class="def-code-arguments">(<var class="var">obj</var>, <var class="var">method</var> )</code><a class="copiable-link" href="#index-com_005finvoke-1"></a></span></dt>
<dd> 
<p>Call invoke on <var class="var">obj</var> to run a method, or obtain a list of all methods.
</p> 
<p><code class="code">com_invoke (<var class="var">obj</var>)</code> returns a list of all methods available for object <var class="var">obj</var> in <var class="var">result</var>.
</p> 
<p><code class="code">com_invoke (<var class="var">obj</var>, <var class="var">method</var> )</code> invokes <var class="var">method</var> method for object <var class="var">obj</var> and returns result <var class="var">result</var>.
</p> 
</dd></dl>