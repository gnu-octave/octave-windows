/*
 * Get graphical coordinates from screen
 * 
 * This contains bits of code hacked from the
 * X Consortium and from Octave. Please see the
 * appropriate licences. The rest is mine, and
 * you can do what you want with that part.
 * 
 * Copyright (C) 1997 Andy Adler <adler@ncf.ca>
 * 
 * Compile like this
 * mkoctfile -L/usr/X11R6/lib -lX11 -I/usr/X11R6/include/ ginput.cc 
 *
 * Please excuse the ugly code. I wrote while I was learning C.
 */

/*
 * Copyright (C) 2001 Laurent Mazet <mazet@crm.mot.com>
 *
 * Fix error handler to avoid octave core-dump.
 * Change to avoid the input limit.
 * Minimize the number of cliks for full x-y axis definitions.
 * Make the code a bit less ugly.
 */

/*
 * Andy Adler <adler@ncf.ca> 2002
 *
 * Modifications to run under windoze
 * $Id$
 */



#include <octave/oct.h>
#include <iostream>
#include "sysdep.h"
#include <octave/pager.h>

int grab_win32_getmousepos ( int * xpt, int * ypt );

#define maxpoints 20

DEFUN_DLD (grab, args, nargout,
           "[...] = grab (...)\n"
           "\n"
           "grab: grab positions of landmarks on the screen\n"
           " \n"
           "[x,y]= grab(axis)\n"
           " x -> x coordinates of the points\n"
           " y -> y coordinates of the points\n"
           "\n"
           " axis -> if specified then the first 2 clicks\n"
           "      must be on the appropriate axes. x and y (or just x\n"
           "      if only 2 points specified ) will then be normalised.\n"
           "\n"
           "for example: x=grab([1 10]) \n"
           "   the first two clicks should correspond to x=1 and x=10 \n"
           "   subsequent clicks will then be normalized to graph units.  \n"
           "\n"
           "for example: [x,y]=grab; \n"
           "   gives x and y in screen pixel units (upper left = 0,0 ) \n"
           "\n"
           "select points by positioning the cursor over the points\n"
           "and clicking <SPACE>. 'q' or <ESC> quits")
{
  ColumnVector axis;
  ColumnVector xaxis(2);
  ColumnVector yaxis(2);
  int nc;
  
  switch (args.length()) {
    case 0:
      nc = 0;
      break;
    case 1:
        { // we need to do this to allow arbitrary orientation
           ColumnVector tmp( args(0).vector_value() );
           if (error_state) return octave_value_list();
           axis = tmp;
        }
        nc = axis.length ();
        if ((nc == 2) || (nc == 4))
          break;
    default:
      print_usage ();
      return octave_value_list();
  }
    
  switch (nc) {
    case 2:
      octave_stdout << "First click on x-axis " << axis(0) << std::endl;
      octave_stdout << "Then click on x-axis " << axis(1) << std::endl;
      flush_octave_stdout();
      break;
    case 4:
      octave_stdout << "First click on point "
                    << "(" << axis(0) << "," << axis(2) << ")" << std::endl;
      octave_stdout << "Then click on point "
                    << "(" << axis(1) << "," << axis(3) << ")" << std::endl;
      flush_octave_stdout();
      break;
  }


  if (nc != 0) {
    int axispoints=0;
    while ( axispoints < 2 ) {
      int ch;
      int xpt; int ypt;

      ch= octave_kbhit( 0 );
      grab_win32_getmousepos ( & xpt, & ypt );

      if (ch == ' ') {
        xaxis (axispoints) = (double)xpt;
        yaxis (axispoints) = (double)ypt;
        axispoints++;
      }

    }
  }


  /* Wait for a click */
  MArray<int> xc(maxpoints);
  MArray<int> yc(maxpoints);

  int nb_elements = 0;
  while (1) {
    int ch;
    int xpt, ypt;

    ch= octave_kbhit( 0 );
    grab_win32_getmousepos ( & xpt, & ypt );

    if (ch == ' ') {
      xc (nb_elements) = xpt;
      yc (nb_elements) = ypt;
      nb_elements++;
    }
    else break;
    
    if (nb_elements == xc.length()) {
      xc.resize (xc.length()+maxpoints);
      yc.resize (yc.length()+maxpoints);
    }
  }

  
  double xb=0, xm=1, yb=0, ym=1;
  if ((nc == 2) || (nc == 4)) {
    double xdiff = xaxis(1) - xaxis(0);
    xm = -(axis(0)-axis(1)) / xdiff;
    xb = (xaxis(1)*axis(0)-xaxis(0)*axis(1)) / xdiff;
    if (nc == 4) {
      double ydiff = yaxis(1) - yaxis(0);
      ym = -(axis(2)-axis(3)) / ydiff;
      yb = (yaxis(1)*axis(2)-yaxis(0)*axis(3)) / ydiff;
    }
  }

  ColumnVector x(nb_elements), y(nb_elements);
  for(int i=0; i<nb_elements; i++) {
    x(i) = xc(i)*xm + xb;
    y(i) = yc(i)*ym + yb;
  }

  octave_value_list retval;
  retval (0) = x;
  if (nargout == 2) 
      retval(1) = y;
  
  return retval;
}
