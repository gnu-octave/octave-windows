---
layout: "default"
permalink: "/functions/4_grab/"
pkg_name: "windows"
pkg_version: "1.6.5"
pkg_description: "Provides COM interface and additional functionality on Windows"
title: "Windows Toolkit - grab"
category: "Windows Utilities"
func_name: "grab"
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
<dt class="deftypefn" id="index-grab"><span class="category-def">Loadable Function: </span><span><code class="def-type">[<var class="var">x</var>,<var class="var">y</var>] =</code> <strong class="def-name">grab</strong> <code class="def-code-arguments">(<var class="var">axis</var>)</code><a class="copiable-link" href="#index-grab"></a></span></dt>
<dd> 
<p>Grab positions of landmarks on the screen.
</p> 
<p><var class="var">x</var> is the x coordinates of the points.
</p> 
<p><var class="var">y</var> is the y coordinates of the points.
</p> 
<p><var class="var">axis</var> (optional) if specified then the first 2 clicks
 must be on the appropriate axes. x and y (or just x
 if only 2 points specified ) will then be normalised.
</p> 
<p>for example: 
 </p><div class="example">
<pre class="example-preformatted"> x=grab([1 10]) 
 </pre></div>
 
<p>the first two clicks should correspond to x=1 and x=10 
 subsequent clicks will then be normalized to graph units.  
</p> 
<p>for example:
 </p><div class="example">
<pre class="example-preformatted"> [x,y]=grab; 
 </pre></div>
 
<p>gives x and y in screen pixel units (upper left = 0,0 ) 
</p> 
<p>Select points by positioning the cursor over the points
 and clicking &lt;SPACE&gt;. &rsquo;q&rsquo; or &lt;ESC&gt; quits
 </p></dd></dl>