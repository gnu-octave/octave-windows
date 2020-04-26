Introduction
============

The Octave Windows package provides COM interface and additional functionality on Windows for GNU Octave.

Requirements
============

* Octave >= 3.6.0+

* Microsoft Windows, or wine.


Installing
==========

To install, run the octave package manager:

1. If running Windows, the package may already be installed:

   pkg list windows

2. To install from source forge:
   pkg install -forge windows

3. To install from a local tarball.
   pkg install windows-XXXXXXX.tar.gz
   Where XXXXXXX is the version of the the downloaded tarball.

Usage:
======

1. Load the package.
   pkg load windows
   (Required each time Octave is started)

2. Type help actxserver to provide a starting place for creating a COM object.

Documentation
==============

See the function list for octave-windows on [octave forge](https://octave.sourceforge.io/windows/overview.html) for function documentation.

See the online manual for octave-windows on [octave forge](https://octave.sourceforge.io/windows/package_doc/) for complete documentation.

Read the Octave [Octave windows package wiki](https://wiki.octave.org/Windows_package) for install tips and examples.

Known limitations and bugs
==========================

Currenty package does not support user data types.
