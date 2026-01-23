---
layout: "default"
permalink: "/functions/7_mat2xls/"
pkg_name: "windows"
pkg_version: "1.7.0"
pkg_description: "Provides COM interface and additional functionality on Windows"
title: "Windows Toolkit - mat2xls"
category: "Examples"
func_name: "mat2xls"
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
<dt class="deftypefn def-line" id="index-mat2xls"><span class="category-def">: </span><span><strong class="def-name">mat2xls</strong> <code class="def-code-arguments">(<var class="var">obj</var>,<var class="var">filename</var>)</code><a class="copiable-link" href="#index-mat2xls"></a></span></dt>
<dd><p>Save <var class="var">obj</var> as an Excel sheet into the file <var class="var">filename</var>. The
 object <var class="var">obj</var> must be either a cell matrix or a real matrix, that
 is a 2-dimensional object. All elements of the matrix are converted
 to Excel cells and put into the first worksheet, starting at cell A1.
 Supported types are real values and strings.
</p>
<p>If <var class="var">filename</var> does not contain any directory, the file is saved
 in the current directory.
</p>
<p>This function is intended to demonstrate the use of the COM interface
 within octave. You need Excel installed on your computer to make this
 function work properly.
</p>
<p>Examples:
</p>
<div class="example">
<pre class="example-preformatted">   mat2xls (rand (10, 10), 'test1.xls');
   mat2xls ({'This', 'is', 'a', 'string'}, 'test2.xls');
 </pre></div>

</dd></dl>