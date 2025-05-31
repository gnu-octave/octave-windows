---
layout: "default"
permalink: "/functions/9_clipboard/"
pkg_name: "windows"
pkg_version: "1.6.5"
pkg_description: "Provides COM interface and additional functionality on Windows"
title: "Windows Toolkit - clipboard"
category: "Windows Utilities"
func_name: "clipboard"
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
<dt class="deftypefn" id="index-_0028_0027copy_0027_002c"><span class="category-def">Loadable Function: </span><span><code class="def-type">clipboard</code> <strong class="def-name">(<var class="var">'copy'</var>,</strong> <code class="def-code-arguments"><var class="var">data</var>)</code><a class="copiable-link" href="#index-_0028_0027copy_0027_002c"></a></span></dt>
<dt class="deftypefnx def-cmd-deftypefn" id="index-_003d"><span class="category-def">Loadable Function: </span><span><code class="def-type"><var class="var">txt</var></code> <strong class="def-name">=</strong> <code class="def-code-arguments">clipboard (<var class="var">'paste'</var>)</code><a class="copiable-link" href="#index-_003d"></a></span></dt>
<dd> 
<p>Insert or get data from the clipboard.
</p> 
<p><var class="var">&rsquo;copy&rsquo;</var> or <var class="var">&rsquo;paste&rsquo;</var> is the required operation to perform.
 where &rsquo;copy&rsquo; will copy data to the clipboard, and paste will paste data from
 the clipboard to a variable.
</p> 
<p><var class="var">data</var> is the data to copy to the clipboard.
</p> 
<p><var class="var">txt</var> is the text from the clipboard or an empty string it it can  not convert to text..
</p> 
<p>Examples:
</p> 
<p>Copy a string to the clipboard:
 </p><div class="example">
<pre class="example-preformatted"> clipboard('copy', 'hello world');
 </pre></div>
 
<p>Get a string from the clipboard:
 </p><div class="example">
<pre class="example-preformatted"> txt = clipboard('paste');
 </pre></div>
 
</dd></dl>