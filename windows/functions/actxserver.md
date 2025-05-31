---
layout: "default"
permalink: "/functions/10_actxserver/"
pkg_name: "windows"
pkg_version: "1.6.5"
pkg_description: "Provides COM interface and additional functionality on Windows"
title: "Windows Toolkit - actxserver"
category: "COM Interface"
func_name: "actxserver"
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
<dt class="deftypefn" id="index-actxserver"><span class="category-def">Loadable Function: </span><span><code class="def-type"><var class="var">h</var> =</code> <strong class="def-name">actxserver</strong> <code class="def-code-arguments">(<var class="var">progid</var>)</code><a class="copiable-link" href="#index-actxserver"></a></span></dt>
<dd> 
<p>Create a COM server using the <var class="var">progid</var> identifier.
</p> 
<p>Returns <var class="var">h</var>, a handle to the default interface of the COM server.
</p> 
<p>Example:
</p> 
<div class="example">
<pre class="example-preformatted"> </pre><div class="group"><pre class="example-preformatted"> # create a COM server running Microsoft Excel
 app = actxserver ('Excel.Application');
 # free the object
 destroy (app); 
 </pre></div><pre class="example-preformatted"> </pre></div>
</dd></dl>