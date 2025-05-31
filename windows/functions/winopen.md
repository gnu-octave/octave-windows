---
layout: "default"
permalink: "/functions/7_winopen/"
pkg_name: "windows"
pkg_version: "1.6.5"
pkg_description: "Provides COM interface and additional functionality on Windows"
title: "Windows Toolkit - winopen"
category: "Windows Utilities"
func_name: "winopen"
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
<dt class="deftypefn" id="index-_0028name_0029"><span class="category-def">Loadable Function: </span><span><code class="def-type">winopen</code> <strong class="def-name">(<var class="var">name</var>)</strong><a class="copiable-link" href="#index-_0028name_0029"></a></span></dt>
<dd> 
<p>Open the file or directory <var class="var">name</var> in the windows registered
 application for the file, using shell open command.
</p> 
<p>Examples:
</p> 
<p>Open file document.docx in the docx viewer:
 </p><div class="example">
<pre class="example-preformatted"> winopen (&quot;document.docx&quot;);
 </pre></div>
 
<p>Open the current directory in explorer:
 </p><div class="example">
<pre class="example-preformatted"> winopen (pwd);
 </pre></div>
 
 
</dd></dl>