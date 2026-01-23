---
layout: "default"
permalink: "/functions/16_win32MessageBox/"
pkg_name: "windows"
pkg_version: "1.7.0"
pkg_description: "Provides COM interface and additional functionality on Windows"
title: "Windows Toolkit - win32_MessageBox"
category: "Examples"
func_name: "win32_MessageBox"
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
<dt class="deftypefn def-line" id="index-win32_005fMessageBox"><span class="category-def">: </span><span><code class="def-type"><var class="var">rv</var> =</code> <strong class="def-name">win32_MessageBox</strong> <code class="def-code-arguments">(<var class="var">title</var>, <var class="var">text</var>)</code><a class="copiable-link" href="#index-win32_005fMessageBox"></a></span></dt>
<dt class="deftypefnx def-cmd-deftypefn def-line" id="index-win32_005fMessageBox-1"><span class="category-def">: </span><span><code class="def-type"><var class="var">rv</var> =</code> <strong class="def-name">win32_MessageBox</strong> <code class="def-code-arguments">(<var class="var">title</var>, <var class="var">text</var>, <var class="var">MboxType</var>)</code><a class="copiable-link" href="#index-win32_005fMessageBox-1"></a></span></dt>
<dd> 
<p>Display a message box using the win32 API.
</p> 
<p><var class="var">title</var> MessageBox title string
</p> 
<p><var class="var">text</var> MessageBox text string
</p> 
<p><var class="var">MBoxType</var> can be an integer or a string. 
</p> 
<p>For integer values, consult &lt;windows.h&gt;
</p> 
<p>The following string values are recognized:
 </p><ul class="itemize mark-bullet">
<li><code class="code">MB_OK</code>
 </li><li><code class="code">MB_OKCANCEL</code>
 </li><li><code class="code">MB_ABORTRETRYIGNORE</code>
 </li><li><code class="code">MB_YESNOCANCEL</code>
 </li><li><code class="code">MB_YESNO</code>
 </li><li><code class="code">MB_RETRYCANCEL</code>
 </li></ul>
<p>Default is MB_OK
</p> 
<p>Returns a value <var class="var">rv</var>: 
 </p><dl class="table">
<dt>1</dt>
<dd><p>User Clicked OK
 </p></dd>
<dt>2</dt>
<dd><p>User Clicked Cancel
 </p></dd>
<dt>3</dt>
<dd><p>User Clicked Abort
 i</p></dd>
<dt>4</dt>
<dd><p>User Clicked Retry
 </p></dd>
<dt>5</dt>
<dd><p>User Clicked Ignore
 </p></dd>
<dt>6</dt>
<dd><p>User Clicked Yes
 </p></dd>
<dt>7</dt>
<dd><p>User Clicked No
 </p></dd>
<dt>10</dt>
<dd><p>User Clicked Try Again
 </p></dd>
<dt>11</dt>
<dd><p>User Clicked Continue
 </p></dd>
</dl>
</dd></dl>