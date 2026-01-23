---
layout: "default"
permalink: "/functions/20_actxGetRunningServer/"
pkg_name: "windows"
pkg_version: "1.7.0"
pkg_description: "Provides COM interface and additional functionality on Windows"
title: "Windows Toolkit - actxGetRunningServer"
category: "Examples"
func_name: "actxGetRunningServer"
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
<dt class="deftypefn def-line" id="index-actxGetRunningServer"><span class="category-def">: </span><span><code class="def-type"><var class="var">h</var> =</code> <strong class="def-name">actxGetRunningServer</strong> <code class="def-code-arguments">(<var class="var">progid</var>)</code><a class="copiable-link" href="#index-actxGetRunningServer"></a></span></dt>
<dd> 
<p>Get a running COM server using the <var class="var">progid</var> identifier.
</p> 
<p>Returns <var class="var">h</var>, a handle to the default interface of the COM server.
</p> 
<p>If the server is not already running the function will return an error.
</p> 
<p>Example:
</p> 
<div class="example">
<pre class="example-preformatted"> </pre><div class="group"><pre class="example-preformatted"> # Get the COM server running Microsoft Excel (If running)
 app = actxGetRunningServer ('Excel.Application');
 # list the fields
 f = fieldnames(app)
 </pre></div><pre class="example-preformatted"> </pre></div>

<p><strong class="strong">See also:</strong> actxserver.
 </p></dd></dl>