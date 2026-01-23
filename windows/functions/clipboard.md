---
layout: "default"
permalink: "/functions/9_clipboard/"
pkg_name: "windows"
pkg_version: "1.7.0"
pkg_description: "Provides COM interface and additional functionality on Windows"
title: "Windows Toolkit - clipboard"
category: "Examples"
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
<dt class="deftypefn def-line" id="index-_0028_0027copy_0027_002c"><span class="category-def">: </span><span><code class="def-type">clipboard</code> <strong class="def-name">('copy',</strong> <code class="def-code-arguments"><var class="var">data</var>)</code><a class="copiable-link" href="#index-_0028_0027copy_0027_002c"></a></span></dt>
<dt class="deftypefnx def-cmd-deftypefn def-line" id="index-_003d"><span class="category-def">: </span><span><code class="def-type"><var class="var">txt</var></code> <strong class="def-name">=</strong> <code class="def-code-arguments">clipboard ('paste')</code><a class="copiable-link" href="#index-_003d"></a></span></dt>
<dd> 
<p>Insert or get data from the clipboard.
</p> 
<p>&rsquo;copy&rsquo; or &rsquo;paste&rsquo; is the required operation to perform,
 where &rsquo;copy&rsquo; will copy data to the clipboard, and &rsquo;paste&rsquo; will paste data from
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