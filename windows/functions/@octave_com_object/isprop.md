---
layout: "default"
permalink: "/functions/@octave_com_object/25_octavecomobjectisprop/"
pkg_name: "windows"
pkg_version: "1.7.0"
pkg_description: "Provides COM interface and additional functionality on Windows"
title: "Windows Toolkit - @octave_com_object/isprop"
category: "Examples"
func_name: "@octave_com_object/isprop"
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
<dt class="deftypefn def-line" id="index-isprop"><span class="category-def">: </span><span><code class="def-type"><var class="var">S</var> =</code> <strong class="def-name">isprop</strong> <code class="def-code-arguments">(<var class="var">comobj</var>, <var class="var">property</var>)</code><a class="copiable-link" href="#index-isprop"></a></span></dt>
<dd><p>A isprop override for octave_com_object objects.
</p>
<p>For a string property, the function will return true or false if the 
 property exists for the com object.
</p>
<p>If property is a string array, the function will return an array of same size
 with true/false for each string in the array that is a property.
</p>

<p><strong class="strong">See also:</strong> fieldnames.
 </p></dd></dl>