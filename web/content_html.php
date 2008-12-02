<html lang="en">
<head>
<title>MPC 0.5.2</title>
<meta http-equiv="Content-Type" content="text/html">
<meta name="description" content="MPC 0.5.2">
<meta name="generator" content="makeinfo 4.11">
<link title="Top" rel="top" href="#Top">
<link href="http://www.gnu.org/software/texinfo/" rel="generator-home" title="Texinfo Homepage">
<!--


   Copyright (C) 2002, 2003, 2004, 2005, 2007, 2008 Andreas Enge, Philippe The'veny, Paul Zimmermann



   Permission is granted to make and distribute verbatim copies of
this manual provided the copyright notice and this permission notice
are preserved on all copies.

Permission is granted to copy and distribute modified versions of this
manual under the conditions for verbatim copying, provided that the entire
resulting derived work is distributed under the terms of a permission
notice identical to this one.

Permission is granted to copy and distribute translations of this manual
into another language, under the above conditions for modified versions.-->
<meta http-equiv="Content-Style-Type" content="text/css">
<style type="text/css"><!--
  pre.display { font-family:inherit }
  pre.format  { font-family:inherit }
  pre.smalldisplay { font-family:inherit; font-size:smaller }
  pre.smallformat  { font-family:inherit; font-size:smaller }
  pre.smallexample { font-size:smaller }
  pre.smalllisp    { font-size:smaller }
  span.sc    { font-variant:small-caps }
  span.roman { font-family:serif; font-weight:normal; } 
  span.sansserif { font-family:sans-serif; font-weight:normal; } 
--></style>
</head>
<body>
<h1 class="settitle">MPC 0.5.2</h1>
<div class="node">
<p><hr>
<a name="Top"></a>
Next:&nbsp;<a rel="next" accesskey="n" href="#Copying">Copying</a>,
Previous:&nbsp;<a rel="previous" accesskey="p" href="#dir">(dir)</a>,
Up:&nbsp;<a rel="up" accesskey="u" href="#dir">(dir)</a>

</div>

<h2 class="unnumbered">MPC</h2>

<p>This manual documents how to install and use the Multiple Precision
Complex Library, version 0.5.2

<ul class="menu">
<li><a accesskey="1" href="#Copying">Copying</a>:                      MPC Copying Conditions (LGPL). 
<li><a accesskey="2" href="#Introduction-to-MPC">Introduction to MPC</a>:          Brief introduction to MPC. 
<li><a accesskey="3" href="#Installing-MPC">Installing MPC</a>:               How to configure and compile the MPC library. 
<li><a accesskey="4" href="#Reporting-Bugs">Reporting Bugs</a>:               How to usefully report bugs. 
<li><a accesskey="5" href="#MPC-Basics">MPC Basics</a>:                   What every MPC user should know. 
<li><a accesskey="6" href="#Complex-Functions">Complex Functions</a>:            Functions for arithmetic on complex numbers.

<li><a accesskey="7" href="#Contributors">Contributors</a>
<li><a accesskey="8" href="#References">References</a>
<li><a accesskey="9" href="#Concept-Index">Concept Index</a>
<li><a href="#Function-Index">Function Index</a>

</li></ul>
<p>--- The Detailed Node Listing ---

<p>Complex Functions

</p>
<ul class="menu">
<li><a href="#Initializing-Complex-Numbers">Initializing Complex Numbers</a>
<li><a href="#Assigning-Complex-Numbers">Assigning Complex Numbers</a>
<li><a href="#Simultaneous-Init-_0026-Assign">Simultaneous Init &amp; Assign</a>
<li><a href="#Complex-Comparison">Complex Comparison</a>
<li><a href="#Projection-_0026-Decomposing">Projection &amp; Decomposing</a>
<li><a href="#Basic-Arithmetic">Basic Arithmetic</a>
<li><a href="#Power-Functions-and-Logarithm">Power Functions and Logarithm</a>
<li><a href="#Trigonometric-Functions">Trigonometric Functions</a>
<li><a href="#I_002fO-of-Complex-Numbers">I/O of Complex Numbers</a>
<li><a href="#Miscellaneous-Complex-Functions">Miscellaneous Complex Functions</a>
<li><a href="#Internals">Internals</a>

   </ul>

<!-- @times{} made available as a "*" in info and html (already works in tex). -->
<div class="node">
<p><hr>
<a name="Copying"></a>
Next:&nbsp;<a rel="next" accesskey="n" href="#Introduction-to-MPC">Introduction to MPC</a>,
Previous:&nbsp;<a rel="previous" accesskey="p" href="#Top">Top</a>,
Up:&nbsp;<a rel="up" accesskey="u" href="#Top">Top</a>

</div>

<!-- node-name, next, previous,  up -->
<h2 class="unnumbered">MPC Copying Conditions</h2>

<p><a name="index-Copying-conditions-1"></a><a name="index-Conditions-for-copying-MPC-2"></a>
The MPC Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or (at your
option) any later version, see the file COPYING.LIB.

   <p>The MPC Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

<div class="node">
<p><hr>
<a name="Introduction-to-MPC"></a>
Next:&nbsp;<a rel="next" accesskey="n" href="#Installing-MPC">Installing MPC</a>,
Previous:&nbsp;<a rel="previous" accesskey="p" href="#Copying">Copying</a>,
Up:&nbsp;<a rel="up" accesskey="u" href="#Top">Top</a>

</div>

<!-- node-name,  next,  previous,  up -->
<h2 class="chapter">1 Introduction to MPC</h2>

<p>MPC is a portable library written in C for arbitrary precision arithmetic
on complex numbers providing correct rounding. Ultimately, it
should implement a multiprecision equivalent of the C99 standard. 
It builds upon the GNU MP and the MPFR libraries.

<h3 class="section">1.1 How to use this Manual</h3>

<p>Everyone should read <a href="#MPC-Basics">MPC Basics</a>.  If you need to install the library
yourself, you need to read <a href="#Installing-MPC">Installing MPC</a>, too.

   <p>The remainder of the manual can be used for later reference, although it is
probably a good idea to skim through it.

<div class="node">
<p><hr>
<a name="Installing-MPC"></a>
Next:&nbsp;<a rel="next" accesskey="n" href="#Reporting-Bugs">Reporting Bugs</a>,
Previous:&nbsp;<a rel="previous" accesskey="p" href="#Introduction-to-MPC">Introduction to MPC</a>,
Up:&nbsp;<a rel="up" accesskey="u" href="#Top">Top</a>

</div>

<!-- node-name,  next,  previous,  up -->
<h2 class="chapter">2 Installing MPC</h2>

<p><a name="index-Installation-3"></a>
To build MPC, you first have to install GNU MP (version 4.2.2 or higher) and
MPFR (version 2.3.1 or higher) on your computer.  You need a C compiler,
preferably GCC, but any reasonable compiler should work. And you need a
standard Unix &lsquo;<samp><span class="samp">make</span></samp>&rsquo; program, plus some other standard Unix utility
programs.

   <p>Here are the steps needed to install the library on Unix systems:

     <ol type=1 start=1>
<li>&lsquo;<samp><span class="samp">tar xzf mpc-0.5.2.tar.gz</span></samp>&rsquo;

     <li>&lsquo;<samp><span class="samp">cd mpc-0.5.2</span></samp>&rsquo;

     <li>&lsquo;<samp><span class="samp">./configure</span></samp>&rsquo;

     <p>if GMP and MPFR are installed into standard directories, that is, directories
that are searched by default by the compiler and the linking tools.

     <p>&lsquo;<samp><span class="samp">./configure --with-gmp=&lt;gmp_install_dir&gt;</span></samp>&rsquo;

     <p>is used to indicate a different location where GMP is
installed. Alternatively, you can specify directly GMP include and GMP lib
directories with &lsquo;<samp><span class="samp">./configure --with-gmp-lib=&lt;gmp_lib_dir&gt;
--with-gmp-include=&lt;gmp_include_dir&gt;</span></samp>&rsquo;.

     <p>&lsquo;<samp><span class="samp">./configure --with-mpfr=&lt;mpfr_install_dir&gt;</span></samp>&rsquo;

     <p>is used to indicate a different location where MPFR is
installed. Alternatively, you can specify directly MPFR include and MPFR lib
directories with &lsquo;<samp><span class="samp">./configure --with-mpf-lib=&lt;mpfr_lib_dir&gt;
--with-mpfr-include=&lt;mpfr_include_dir&gt;</span></samp>&rsquo;.

     <li>&lsquo;<samp><span class="samp">make</span></samp>&rsquo;

     <p>This compiles MPC in the working directory.

     <li>&lsquo;<samp><span class="samp">make check</span></samp>&rsquo;

     <p>This will make sure MPC was built correctly.

     <p>If you get error messages, please report them to
&lsquo;<samp><span class="samp">mpc-discuss@lists.gforge.inria.fr</span></samp>&rsquo; (See <a href="#Reporting-Bugs">Reporting Bugs</a>, for
information on what to include in useful bug reports).

     <li>&lsquo;<samp><span class="samp">make install</span></samp>&rsquo;

     <p>This will copy the file <samp><span class="file">mpc.h</span></samp> to the directory
<samp><span class="file">/usr/local/include</span></samp>, the file <samp><span class="file">libmpc.a</span></samp> to the directory
<samp><span class="file">/usr/local/lib</span></samp>, and the file <samp><span class="file">mpc.info</span></samp> to the directory
<samp><span class="file">/usr/local/share/info</span></samp> (or if you passed the &lsquo;<samp><span class="samp">--prefix</span></samp>&rsquo; option to
<samp><span class="file">configure</span></samp>, using the prefix directory given as argument to
&lsquo;<samp><span class="samp">--prefix</span></samp>&rsquo; instead of <samp><span class="file">/usr/local</span></samp>). Note: you need write permissions
on these directories.

        </ol>

<h3 class="section">2.1 Other `make' Targets</h3>

<p>There are some other useful make targets:

     <ul>
<li>&lsquo;<samp><span class="samp">info</span></samp>&rsquo;

     <p>Create an info version of the manual, in <samp><span class="file">mpc.info</span></samp>.

     <li>&lsquo;<samp><span class="samp">pdf</span></samp>&rsquo;

     <p>Create a PDF version of the manual, in <samp><span class="file">doc/mpc.pdf</span></samp>.

     <li>&lsquo;<samp><span class="samp">dvi</span></samp>&rsquo;

     <p>Create a DVI version of the manual, in <samp><span class="file">doc/mpc.dvi</span></samp>.

     <li>&lsquo;<samp><span class="samp">ps</span></samp>&rsquo;

     <p>Create a Postscript version of the manual, in <samp><span class="file">doc/mpc.ps</span></samp>.

     <li>&lsquo;<samp><span class="samp">html</span></samp>&rsquo;

     <p>Create an HTML version of the manual, in several pages in the
directory <samp><span class="file">doc/mpc.html</span></samp>; if you want only one output HTML file,
then type &lsquo;<samp><span class="samp">makeinfo --html --no-split mpc.texi</span></samp>&rsquo; instead.

     <li>&lsquo;<samp><span class="samp">clean</span></samp>&rsquo;

     <p>Delete all object files and archive files, but not the configuration files.

     <li>&lsquo;<samp><span class="samp">distclean</span></samp>&rsquo;

     <p>Delete all files not included in the distribution.

     <li>&lsquo;<samp><span class="samp">uninstall</span></samp>&rsquo;

     <p>Delete all files copied by &lsquo;<samp><span class="samp">make install</span></samp>&rsquo;. 
</ul>

<h3 class="section">2.2 Known Build Problems</h3>

<p>No build problems are known. Please report any problems you encounter to
&lsquo;<samp><span class="samp">mpc-discuss@lists.gforge.inria.fr</span></samp>&rsquo;. 
See <a href="#Reporting-Bugs">Reporting Bugs</a>.

<div class="node">
<p><hr>
<a name="Reporting-Bugs"></a>
Next:&nbsp;<a rel="next" accesskey="n" href="#MPC-Basics">MPC Basics</a>,
Previous:&nbsp;<a rel="previous" accesskey="p" href="#Installing-MPC">Installing MPC</a>,
Up:&nbsp;<a rel="up" accesskey="u" href="#Top">Top</a>

</div>

<!-- node-name,  next,  previous,  up -->
<h2 class="chapter">3 Reporting Bugs</h2>

<p><a name="index-Reporting-bugs-4"></a>
If you think you have found a bug in the MPC library,
please investigate
and report it. We have made this library available to you, and it is not to ask
too much from you, to ask you to report the bugs that you find.

   <p>There are a few things you should think about when you put your bug report
together.

   <p>You have to send us a test case that makes it possible for us to reproduce the
bug.  Include instructions on how to run the test case.

   <p>You also have to explain what is wrong; if you get a crash, or if the results
printed are incorrect and in that case, in what way.

   <p>Please include compiler version information in your bug report. 
This can be extracted using &lsquo;<samp><span class="samp">gcc -v</span></samp>&rsquo;,
or &lsquo;<samp><span class="samp">cc -V</span></samp>&rsquo; on some machines. 
Also, include the output from &lsquo;<samp><span class="samp">uname -a</span></samp>&rsquo;.

   <p>If your bug report is good, we will do our best to help you to get a corrected
version of the library; if the bug report is poor, we will not do anything about
it (aside of chiding you to send better bug reports).

   <p>Send your bug report to: &lsquo;<samp><span class="samp">mpc-discuss@lists.gforge.inria.fr</span></samp>&rsquo;.

   <p>If you think something in this manual is unclear, or downright incorrect, or if
the language needs to be improved, please send a note to the same address.

<div class="node">
<p><hr>
<a name="MPC-Basics"></a>
Next:&nbsp;<a rel="next" accesskey="n" href="#Complex-Functions">Complex Functions</a>,
Previous:&nbsp;<a rel="previous" accesskey="p" href="#Reporting-Bugs">Reporting Bugs</a>,
Up:&nbsp;<a rel="up" accesskey="u" href="#Top">Top</a>

</div>

<!-- node-name,  next,  previous,  up -->
<h2 class="chapter">4 MPC Basics</h2>

<p><a name="index-g_t_0040file_007bmpc_002eh_007d-5"></a>All declarations needed to use MPC are collected in the include file
<samp><span class="file">mpc.h</span></samp>.  It is designed to work with both C and C++ compilers. 
You should include that file in any program using the MPC library
by adding the line
<pre class="example">        #include "mpc.h"
</pre>
   <h3 class="section">4.1 Nomenclature and Types</h3>

<p><a name="index-Complex-number-6"></a><a name="index-g_t_0040code_007bmpc_005ft_007d-7"></a><dfn>Complex number</dfn> or <dfn>Complex</dfn> for short, is a pair of two
arbitrary precision floating-point numbers (for the real and imaginary parts). 
The C data type for such objects is <code>mpc_t</code>.

   <p><a name="index-Precision-8"></a><a name="index-g_t_0040code_007bmp_005fprec_005ft_007d-9"></a>The <dfn>Precision</dfn> is the number of bits used to represent the mantissa
of the real and imaginary parts;
the corresponding C data type is <code>mp_prec_t</code>. 
See the MPFR documentation for more details on the allowed precision range.

   <p><a name="index-Rounding-Mode-10"></a><a name="index-g_t_0040code_007bmpc_005frnd_005ft_007d-11"></a>The <dfn>rounding mode</dfn> specifies the way to round the result of a
complex operation, in case the exact result can not be represented
exactly in the destination mantissa;
the corresponding C data type is <code>mpc_rnd_t</code>. 
A complex rounding mode is a pair of two rounding modes: one for the real
part, one for the imaginary part.

<h3 class="section">4.2 Function Classes</h3>

<p>There is only one class of functions in the MPC library, namely functions for
complex arithmetic. The function names begin with <code>mpc_</code>. The
associated type is <code>mpc_t</code>.

<h3 class="section">4.3 MPC Variable Conventions</h3>

<p>As a general rule, all MPC functions expect output arguments before input
arguments.  This notation is based on an analogy with the assignment operator.

   <p>MPC allows you to use the same variable for both input and output in the same
expression.  For example, the main function for floating-point multiplication,
<code>mpc_mul</code>, can be used like this: <code>mpc_mul (x, x, x, rnd_mode)</code>. 
This
computes the square of <var>x</var> with rounding mode <code>rnd_mode</code>
and puts the result back in <var>x</var>.

   <p>Before you can assign to an MPC variable, you need to initialize it by calling
one of the special initialization functions.  When you are done with a
variable, you need to clear it out, using one of the functions for that
purpose.

   <p>A variable should only be initialized once, or at least cleared out between
each initialization.  After a variable has been initialized, it may be
assigned to any number of times.

   <p>For efficiency reasons, avoid to initialize and clear out a variable in loops. 
Instead, initialize it before entering the loop, and clear it out after the
loop has exited.

   <p>You do not need to be concerned about allocating additional space for MPC
variables, since each of its real and imaginary part
has a mantissa of fixed size. 
Hence unless you change its precision, or clear and reinitialize it,
a complex variable will have the same allocated space during all its
life.

<h3 class="section">4.4 Rounding Modes</h3>

<p>A complex rounding mode is of the form <code>MPC_RNDxy</code> where
<code>x</code> and <code>y</code> are one of <code>N</code> (to nearest), <code>Z</code> (towards
zero), <code>U</code> (towards plus infinity), <code>D</code> (towards minus infinity). 
The first letter refers to the rounding mode for the real part,
and the second one for the imaginary part. 
For example <code>MPC_RNDZU</code> indicates to round the real part towards zero,
and the imaginary part towards plus infinity.

   <p>The &lsquo;<samp><span class="samp">round to nearest</span></samp>&rsquo; mode works as in the IEEE P754 standard: in case
the number to be rounded lies exactly in the middle of two representable
numbers, it is rounded to the one with the least significant bit set to zero. 
For example, the number 5, which is represented by (101) in binary, is rounded
to (100)=4 with a precision of two bits, and not to (110)=6.

   <p>Most MPC functions have a return value of type <code>int</code>, which is used
to indicate the position of the rounded real or imaginary parts with respect
to the exact (infinite precision) values. 
If this integer is <code>i</code>, the macros <code>MPC_INEX_RE(i)</code> and
<code>MPC_INEX_IM(i)</code> give 0 if the corresponding rounded value is exact,
a negative value if the rounded value is less than the exact one,
and a positive value if it is greater than the exact one. 
However, some functions do not completely fulfill this:
in some cases the sign is not guaranteed, and in some cases a non-zero value
is returned although the result is exact; in these cases the
function documentation explains the exact meaning of the return value. 
However, the return value never wrongly indicates an exact computation.

<div class="node">
<p><hr>
<a name="Complex-Functions"></a>
Next:&nbsp;<a rel="next" accesskey="n" href="#Contributors">Contributors</a>,
Previous:&nbsp;<a rel="previous" accesskey="p" href="#MPC-Basics">MPC Basics</a>,
Up:&nbsp;<a rel="up" accesskey="u" href="#Top">Top</a>

</div>

<!-- node-name,  next,  previous,  up -->
<h2 class="chapter">5 Complex Functions</h2>

<p><a name="index-Complex-functions-12"></a>
The complex functions expect arguments of type <code>mpc_t</code>.

   <p>The MPC floating-point functions have an interface that is similar to the
GNU MP
integer functions.  The function prefix for operations on complex numbers is
<code>mpc_</code>.

   <p><a name="index-User_002ddefined-precision-13"></a>The precision of a computation is defined as follows: Compute the requested
operation exactly (with &ldquo;infinite precision&rdquo;), and round the result to
the destination variable precision with the given rounding mode.

   <p>The MPC complex functions are intended to be a smooth extension
of the IEEE P754 arithmetic. The results obtained on one
computer should not differ from the results obtained on a computer with a
different word size.

<ul class="menu">
<li><a accesskey="1" href="#Initializing-Complex-Numbers">Initializing Complex Numbers</a>
<li><a accesskey="2" href="#Assigning-Complex-Numbers">Assigning Complex Numbers</a>
<li><a accesskey="3" href="#Simultaneous-Init-_0026-Assign">Simultaneous Init &amp; Assign</a>
<li><a accesskey="4" href="#Complex-Comparison">Complex Comparison</a>
<li><a accesskey="5" href="#Projection-_0026-Decomposing">Projection &amp; Decomposing</a>
<li><a accesskey="6" href="#Basic-Arithmetic">Basic Arithmetic</a>
<li><a accesskey="7" href="#Power-Functions-and-Logarithm">Power Functions and Logarithm</a>
<li><a accesskey="8" href="#Trigonometric-Functions">Trigonometric Functions</a>
<li><a accesskey="9" href="#I_002fO-of-Complex-Numbers">I/O of Complex Numbers</a>
<li><a href="#Miscellaneous-Complex-Functions">Miscellaneous Complex Functions</a>
<li><a href="#Internals">Internals</a>
</ul>

<div class="node">
<p><hr>
<a name="Initializing-Complex-Numbers"></a>
Next:&nbsp;<a rel="next" accesskey="n" href="#Assigning-Complex-Numbers">Assigning Complex Numbers</a>,
Previous:&nbsp;<a rel="previous" accesskey="p" href="#Complex-Functions">Complex Functions</a>,
Up:&nbsp;<a rel="up" accesskey="u" href="#Complex-Functions">Complex Functions</a>

</div>

<!-- node-name,  next,  previous,  up -->
<h3 class="section">5.1 Initialization Functions</h3>

<div class="defun">
&mdash; Function: void <b>mpc_set_default_prec</b> (<var>mp_prec_t prec</var>)<var><a name="index-mpc_005fset_005fdefault_005fprec-14"></a></var><br>
<blockquote><p>Set the default precision to be <strong>exactly</strong> <var>prec</var> bits. 
All subsequent calls to <code>mpc_init</code> will use this precision, but previously
initialized variables are unaffected. 
This default precision is set to 53 bits initially. 
It is valid for the real and the imaginary parts alike. 
</p></blockquote></div>

<div class="defun">
&mdash; Function: mp_prec_t <b>mpc_get_default_prec</b> ()<var><a name="index-mpc_005fget_005fdefault_005fprec-15"></a></var><br>
<blockquote><p>Returns the default MPC precision in bits. 
</p></blockquote></div>

   <p>An <code>mpc_t</code> object must be initialized before storing the first value in
it.  The functions <code>mpc_init</code>, <code>mpc_init2</code> and <code>mpc_init3</code>
are used for that purpose.

<div class="defun">
&mdash; Function: void <b>mpc_init</b> (<var>mpc_t z</var>)<var><a name="index-mpc_005finit-16"></a></var><br>
<blockquote><p>Initialize <var>z</var>, and set its real and imaginary parts to NaN.

        <p>Normally, a variable should be initialized once only
or at least be cleared, using <code>mpc_clear</code>, between initializations.  The
precision of <var>x</var> is the default precision, which can be changed
by a call to <code>mpc_set_default_prec</code>. 
</p></blockquote></div>

<div class="defun">
&mdash; Function: void <b>mpc_init2</b> (<var>mpc_t z, mp_prec_t prec</var>)<var><a name="index-mpc_005finit2-17"></a></var><br>
<blockquote><p>Initialize <var>z</var>, set its precision to be
<var>prec</var> bits, and set its real and imaginary parts to NaN. 
</p></blockquote></div>

<div class="defun">
&mdash; Function: void <b>mpc_init3</b> (<var>mpc_t z, mp_prec_t prec_r, mp_prec_t prec_i</var>)<var><a name="index-mpc_005finit3-18"></a></var><br>
<blockquote><p>Initialize <var>z</var>, set the precision of its real part to <var>prec_r</var> bits,
the precision of its imaginary part to <var>prec_i</var> bits,
and set its real and imaginary parts to NaN. 
</p></blockquote></div>

<div class="defun">
&mdash; Function: void <b>mpc_clear</b> (<var>mpc_t z</var>)<var><a name="index-mpc_005fclear-19"></a></var><br>
<blockquote><p>Free the space occupied by <var>z</var>.  Make sure to call this function for all
<code>mpc_t</code> variables when you are done with them. 
</p></blockquote></div>

   <p>Here is an example on how to initialize complex variables:
<pre class="example">     {
       mpc_t x, y, z;
       mpc_init  (x);		/* use default precision */
       mpc_init2 (y, 256);		/* precision <em>exactly</em> 256 bits */
       mpc_init3 (z, 100, 50);	/* 100/50 bits for the real/imaginary part */
       ...
       /* Unless the program is about to exit, do ... */
       mpc_clear (x);
       mpc_clear (y);
       mpc_clear (z);
     }
</pre>
   <p>The following function is useful for changing the precision during a
calculation.  A typical use would be for adjusting the precision gradually in
iterative algorithms like Newton-Raphson, making the computation precision
closely match the actual accurate part of the numbers.

<div class="defun">
&mdash; Function: void <b>mpc_set_prec</b> (<var>mpc_t x, mp_prec_t prec</var>)<var><a name="index-mpc_005fset_005fprec-20"></a></var><br>
<blockquote><p>Reset the precision of <var>x</var> to be <strong>exactly</strong> <var>prec</var> bits,
and set its real/imaginary parts to NaN. 
The previous value stored in <var>x</var> is lost. It is equivalent to
a call to <code>mpc_clear(x)</code> followed by a call to
<code>mpc_init2(x, prec)</code>, but more efficient as no allocation is done in
case the current allocated space for the mantissa of <var>x</var> is sufficient. 
</p></blockquote></div>

<div class="defun">
&mdash; Function: mp_prec_t <b>mpc_get_prec</b> (<var>mpc_t x</var>)<var><a name="index-mpc_005fget_005fprec-21"></a></var><br>
<blockquote><p>If the real and imaginary part of <var>x</var> have the same precision, it is returned,
otherwise, 0 is returned. 
</p></blockquote></div>

<div class="defun">
&mdash; Function: void <b>mpc_get_prec2</b> (<var>mp_prec_t* pr, mp_prec_t* pi, mpc_t x</var>)<var><a name="index-mpc_005fget_005fprec2-22"></a></var><br>
<blockquote><p>Returns the precision of the real part of <var>x</var> via <var>pr</var> and of its imaginary part
via <var>pi</var>. 
</p></blockquote></div>

<div class="node">
<p><hr>
<a name="Assigning-Complex-Numbers"></a>
Next:&nbsp;<a rel="next" accesskey="n" href="#Simultaneous-Init-_0026-Assign">Simultaneous Init &amp; Assign</a>,
Previous:&nbsp;<a rel="previous" accesskey="p" href="#Initializing-Complex-Numbers">Initializing Complex Numbers</a>,
Up:&nbsp;<a rel="up" accesskey="u" href="#Complex-Functions">Complex Functions</a>

</div>

<!-- node-name,  next,  previous,  up -->
<h3 class="section">5.2 Assignment Functions</h3>

<p><a name="index-Complex-assignment-functions-23"></a>
These functions assign new values to already initialized complex numbers
(see <a href="#Initializing-Complex-Numbers">Initializing Complex Numbers</a>).

<div class="defun">
&mdash; Function: int <b>mpc_set</b> (<var>mpc_t rop, mpc_t op, mpc_rnd_t rnd</var>)<var><a name="index-mpc_005fset-24"></a></var><br>
&mdash; Macro: int <b>mpc_set_ui</b> (<var>mpc_t rop, unsigned long int op, mpc_rnd_t rnd</var>)<var><a name="index-mpc_005fset_005fui-25"></a></var><br>
&mdash; Macro: int <b>mpc_set_si</b> (<var>mpc_t rop, long int op, mpc_rnd_t rnd</var>)<var><a name="index-mpc_005fset_005fsi-26"></a></var><br>
&mdash; Macro: int <b>mpc_set_d</b> (<var>mpc_t rop, double op, mpc_rnd_t rnd</var>)<var><a name="index-mpc_005fset_005fd-27"></a></var><br>
&mdash; Macro: int <b>mpc_set_fr</b> (<var>mpc_t rop, mpfr_t op, mpc_rnd_t rnd</var>)<var><a name="index-mpc_005fset_005ffr-28"></a></var><br>
<blockquote><p>Set the value of <var>rop</var> from <var>op</var>, rounded to the precision of <var>rop</var>
with the given rounding mode <var>rnd</var>. 
Except for <code>mpc_set</code>, the argument <var>op</var> is interpreted as real,
so the imaginary part of <var>rop</var> is set to zero with a positive sign. 
Please note that even a <code>long int</code> may have to be rounded,
if the destination precision is less than the machine word width. 
For <code>mpc_set_d</code>, be careful that the input number <var>op</var>
may not be exactly representable as a double-precision number (this happens for
0.1 for instance), in which case it is first
rounded by the C compiler to a double-precision number, and then only
to a complex number. 
</p></blockquote></div>

<div class="defun">
&mdash; Function: int <b>mpc_set_d_d</b> (<var>mpc_t rop, double op1, double op2, mpc_rnd_t rnd</var>)<var><a name="index-mpc_005fset_005fd_005fd-29"></a></var><br>
&mdash; Function: int <b>mpc_set_ui_ui</b> (<var>mpc_t rop, unsigned long int op1, unsigned long int op2, mpc_rnd_t rnd</var>)<var><a name="index-mpc_005fset_005fui_005fui-30"></a></var><br>
&mdash; Function: int <b>mpc_set_si_si</b> (<var>mpc_t rop, long int op1, long int op2, mpc_rnd_t rnd</var>)<var><a name="index-mpc_005fset_005fsi_005fsi-31"></a></var><br>
&mdash; Function: int <b>mpc_set_fr_fr</b> (<var>mpc_t rop, mpfr_t op1, mpfr_t op2, mpc_rnd_t rnd</var>)<var><a name="index-mpc_005fset_005ffr_005ffr-32"></a></var><br>
&mdash; Function: int <b>mpc_set_ui_fr</b> (<var>mpc_t rop, unsigned long int op1, mpfr_t op2, mpc_rnd_t rnd</var>)<var><a name="index-mpc_005fset_005fui_005ffr-33"></a></var><br>
<blockquote><p>Set the real part of <var>rop</var> from <var>op1</var>, and its imaginary part from
<var>op2</var>, according to the rounding mode <var>rnd</var>. 
</p></blockquote></div>

<div class="node">
<p><hr>
<a name="Simultaneous-Init-%26-Assign"></a>
<a name="Simultaneous-Init-_0026-Assign"></a>
Next:&nbsp;<a rel="next" accesskey="n" href="#Complex-Comparison">Complex Comparison</a>,
Previous:&nbsp;<a rel="previous" accesskey="p" href="#Assigning-Complex-Numbers">Assigning Complex Numbers</a>,
Up:&nbsp;<a rel="up" accesskey="u" href="#Complex-Functions">Complex Functions</a>

</div>

<!-- node-name,  next,  previous,  up -->
<h3 class="section">5.3 Combined Initialization and Assignment Functions</h3>

<p><a name="index-Initialization-and-assignment-functions-34"></a>

<div class="defun">
&mdash; Macro: int <b>mpc_init_set</b> (<var>mpc_t rop, mpc_t op, mpc_rnd_t rnd</var>)<var><a name="index-mpc_005finit_005fset-35"></a></var><br>
&mdash; Macro: int <b>mpc_init_set_ui</b> (<var>mpc_t rop, unsigned long int op, mpc_rnd_t rnd</var>)<var><a name="index-mpc_005finit_005fset_005fui-36"></a></var><br>
<blockquote><p>Initialize <var>rop</var> and set its value from <var>op</var>, rounded with the
rounding mode <var>rnd</var>. 
The precision of <var>rop</var> will be taken from the active default precision,
as set by <code>mpc_set_default_prec</code>. 
</p></blockquote></div>

<div class="defun">
&mdash; Macro: int <b>mpc_init_set_ui_ui</b> (<var>mpc_t rop, unsigned long int op1, unsigned long int op2, mpc_rnd_t rnd</var>)<var><a name="index-mpc_005finit_005fset_005fui_005fui-37"></a></var><br>
&mdash; Macro: int <b>mpc_init_set_ui_fr</b> (<var>mpc_t rop, unsigned long int op1, mpfr_t op2, mpc_rnd_t rnd</var>)<var><a name="index-mpc_005finit_005fset_005fui_005ffr-38"></a></var><br>
&mdash; Macro: int <b>mpc_init_set_si_si</b> (<var>mpc_t rop, long int op1, long int op2, mpc_rnd_t rnd</var>)<var><a name="index-mpc_005finit_005fset_005fsi_005fsi-39"></a></var><br>
<blockquote><p>Initialize <var>rop</var>, set its real part from <var>op1</var>, and its imaginary
part from <var>op2</var>, rounded with the rounding mode <var>rnd</var>. 
The precision of <var>rop</var> will be taken from the active default precision,
as set by <code>mpc_set_default_prec</code>. 
</p></blockquote></div>

<div class="node">
<p><hr>
<a name="Complex-Comparison"></a>
Next:&nbsp;<a rel="next" accesskey="n" href="#Projection-_0026-Decomposing">Projection &amp; Decomposing</a>,
Previous:&nbsp;<a rel="previous" accesskey="p" href="#Simultaneous-Init-_0026-Assign">Simultaneous Init &amp; Assign</a>,
Up:&nbsp;<a rel="up" accesskey="u" href="#Complex-Functions">Complex Functions</a>

</div>

<!-- node-name,  next,  previous,  up -->
<h3 class="section">5.4 Comparison Functions</h3>

<p><a name="index-Complex-comparisons-functions-40"></a><a name="index-Comparison-functions-41"></a>

<div class="defun">
&mdash; Function: int <b>mpc_cmp</b> (<var>mpc_t op1, mpc_t op2</var>)<var><a name="index-mpc_005fcmp-42"></a></var><br>
&mdash; Function: int <b>mpc_cmp_si_si</b> (<var>mpc_t op1, long int op2r, long int op2i</var>)<var><a name="index-mpc_005fcmp_005fsi_005fsi-43"></a></var><br>
&mdash; Macro: int <b>mpc_cmp_si</b> (<var>mpc_t op1, long int op2</var>)<var><a name="index-mpc_005fcmp_005fsi-44"></a></var><br>
<blockquote>
        <p>Compare <var>op1</var> and <var>op2</var>, where in the case of <code>mpc_cmp_si_si</code>,
<var>op2</var> is taken to be <var>op2r</var> + i <var>op2i</var>. 
The return value <var>c</var> can be decomposed into <code>x = MPC_INEX_RE(c)</code>
and <code>y = MPC_INEX_IM(c)</code>, such that <var>x</var> is
positive if the real part of <var>op1</var> is greater than that of <var>op2</var>,
zero if both real parts are equal, and negative if the real part of <var>op1</var>
is less than that of <var>op2</var>, and likewise for <var>y</var>. 
Both <var>op1</var> and <var>op2</var> are considered to their full own precision,
which may differ. 
It is not allowed that one of the operands has a NaN (Not-a-Number) part.

        <p>The storage of the return value is such that equality can be simply checked
with <code>mpc_cmp (op1, op2) == 0</code>. 
</p></blockquote></div>

<div class="node">
<p><hr>
<a name="Projection-%26-Decomposing"></a>
<a name="Projection-_0026-Decomposing"></a>
Next:&nbsp;<a rel="next" accesskey="n" href="#Basic-Arithmetic">Basic Arithmetic</a>,
Previous:&nbsp;<a rel="previous" accesskey="p" href="#Complex-Comparison">Complex Comparison</a>,
Up:&nbsp;<a rel="up" accesskey="u" href="#Complex-Functions">Complex Functions</a>

</div>

<!-- node-name,  next,  previous,  up -->
<h3 class="section">5.5 Projection and Decomposing Functions</h3>

<p><a name="index-Projection-and-Decomposing-Functions-45"></a>

<div class="defun">
&mdash; Function: int <b>mpc_real</b> (<var>mpfr_t rop, mpc_t op, mpfr_rnd_t rnd</var>)<var><a name="index-mpc_005freal-46"></a></var><br>
<blockquote><p>Set <var>rop</var> to the value of the real part of <var>op</var> rounded
in the direction <var>rnd</var>. 
</p></blockquote></div>

<div class="defun">
&mdash; Function: int <b>mpc_imag</b> (<var>mpfr_t rop, mpc_t op, mpfr_rnd_t rnd</var>)<var><a name="index-mpc_005fimag-47"></a></var><br>
<blockquote><p>Set <var>rop</var> to the value of the imaginary part of <var>op</var> rounded in the
direction <var>rnd</var>. 
</p></blockquote></div>

<div class="defun">
&mdash; Macro: mpfr_t <b>mpc_realref</b> (<var>mpc_t op</var>)<var><a name="index-mpc_005frealref-48"></a></var><br>
&mdash; Macro: mpfr_t <b>mpc_imagref</b> (<var>mpc_t op</var>)<var><a name="index-mpc_005fimagref-49"></a></var><br>
<blockquote><p>Return a reference to the real part and imaginary part of <var>op</var>,
respectively. The <code>mpfr</code> functions can be used on the result of these
macros. 
</p></blockquote></div>

<div class="defun">
&mdash; Function: int <b>mpc_arg</b> (<var>mpfr_t rop, mpc_t op, mpc_rnd_t rnd</var>)<var><a name="index-mpc_005farg-50"></a></var><br>
<blockquote><p>Set <var>rop</var> to the argument of <var>op</var>, with a branch cut along the
negative real axis. 
</p></blockquote></div>

<div class="defun">
&mdash; Function: int <b>mpc_proj</b> (<var>mpc_t rop, mpc_t op, mpc_rnd_t rnd</var>)<var><a name="index-mpc_005fproj-51"></a></var><br>
<blockquote><p>Compute a projection of <var>op</var> onto the Riemann sphere. Set <var>rop</var> to
<var>op</var> rounded in the direction <var>rnd</var>, except when at least one part of
<var>op</var> is infinite (even if the other part is a NaN) in which case the real
part of <var>rop</var> is set to plus infinity and its imaginary part to a signed
zero with the same sign as the imaginary part of <var>op</var>. 
</p></blockquote></div>

<div class="node">
<p><hr>
<a name="Basic-Arithmetic"></a>
Next:&nbsp;<a rel="next" accesskey="n" href="#Power-Functions-and-Logarithm">Power Functions and Logarithm</a>,
Previous:&nbsp;<a rel="previous" accesskey="p" href="#Projection-_0026-Decomposing">Projection &amp; Decomposing</a>,
Up:&nbsp;<a rel="up" accesskey="u" href="#Complex-Functions">Complex Functions</a>

</div>

<!-- node-name,  next,  previous,  up -->
<h3 class="section">5.6 Basic Arithmetic Functions</h3>

<p><a name="index-Complex-arithmetic-functions-52"></a><a name="index-Arithmetic-functions-53"></a>
All the following functions are designed in such a way that, when working
with real numbers instead of complex numbers, their complexity should
essentially be the same as with the MPFR library, with only a marginal
overhead due to the MPC layer.

<div class="defun">
&mdash; Function: int <b>mpc_add</b> (<var>mpc_t rop, mpc_t op1, mpc_t op2, mpc_rnd_t rnd</var>)<var><a name="index-mpc_005fadd-54"></a></var><br>
&mdash; Function: int <b>mpc_add_ui</b> (<var>mpc_t rop, mpc_t op1, unsigned long int op2, mpc_rnd_t rnd</var>)<var><a name="index-mpc_005fadd_005fui-55"></a></var><br>
&mdash; Function: int <b>mpc_add_fr</b> (<var>mpc_t rop, mpc_t op1, mpfr_t op2, mpc_rnd_t rnd</var>)<var><a name="index-mpc_005fadd_005ffr-56"></a></var><br>
<blockquote><p>Set <var>rop</var> to <var>op1</var> + <var>op2</var> rounded according to <var>rnd</var>. 
</p></blockquote></div>

<div class="defun">
&mdash; Function: int <b>mpc_sub</b> (<var>mpc_t rop, mpc_t op1, mpc_t op2, mpc_rnd_t rnd</var>)<var><a name="index-mpc_005fsub-57"></a></var><br>
&mdash; Function: int <b>mpc_sub_fr</b> (<var>mpc_t rop, mpc_t op1, mpfr_t op2, mpc_rnd_t rnd</var>)<var><a name="index-mpc_005fsub_005ffr-58"></a></var><br>
&mdash; Function: int <b>mpc_fr_sub</b> (<var>mpc_t rop, mpfr_t op1, mpc_t op2, mpc_rnd_t rnd</var>)<var><a name="index-mpc_005ffr_005fsub-59"></a></var><br>
&mdash; Function: int <b>mpc_sub_ui</b> (<var>mpc_t rop, mpc_t op1, unsigned long int op2, mpc_rnd_t rnd</var>)<var><a name="index-mpc_005fsub_005fui-60"></a></var><br>
&mdash; Macro: int <b>mpc_ui_sub</b> (<var>mpc_t rop, unsigned long int op1, mpc_t op2, mpc_rnd_t rnd</var>)<var><a name="index-mpc_005fui_005fsub-61"></a></var><br>
&mdash; Function: int <b>mpc_ui_ui_sub</b> (<var>mpc_t rop, unsigned long int re1, unsigned long int im1, mpc_t op2, mpc_rnd_t rnd</var>)<var><a name="index-mpc_005fui_005fui_005fsub-62"></a></var><br>
<blockquote><p>Set <var>rop</var> to <var>op1</var> &minus; <var>op2</var> rounded according to <var>rnd</var>. 
For <code>mpc_ui_ui_sub</code>, <var>op1</var> is <var>re1</var> + <var>im1</var>. 
</p></blockquote></div>

<div class="defun">
&mdash; Function: int <b>mpc_mul</b> (<var>mpc_t rop, mpc_t op1, mpc_t op2, mpc_rnd_t rnd</var>)<var><a name="index-mpc_005fmul-63"></a></var><br>
&mdash; Function: int <b>mpc_mul_ui</b> (<var>mpc_t rop, mpc_t op1, unsigned long int op2, mpc_rnd_t rnd</var>)<var><a name="index-mpc_005fmul_005fui-64"></a></var><br>
&mdash; Function: int <b>mpc_mul_si</b> (<var>mpc_t rop, mpc_t op1, long int op2, mpc_rnd_t rnd</var>)<var><a name="index-mpc_005fmul_005fsi-65"></a></var><br>
&mdash; Function: int <b>mpc_mul_fr</b> (<var>mpc_t rop, mpc_t op1, mpfr_t op2, mpc_rnd_t rnd</var>)<var><a name="index-mpc_005fmul_005ffr-66"></a></var><br>
<blockquote><p>Set <var>rop</var> to <var>op1</var> times <var>op2</var> rounded according to <var>rnd</var>. 
</p></blockquote></div>

<div class="defun">
&mdash; Function: int <b>mpc_mul_i</b> (<var>mpc_t rop, mpc_t op, int sgn, mpc_rnd_t rnd</var>)<var><a name="index-mpc_005fmul_005fi-67"></a></var><br>
<blockquote><p>Set <var>rop</var> to <var>op</var> times the imaginary unit i if <var>sgn</var> is
non-negative, set <var>rop</var> to <var>op</var> times -i otherwise,
in both cases rounded according to <var>rnd</var>. 
</p></blockquote></div>

<div class="defun">
&mdash; Function: int <b>mpc_sqr</b> (<var>mpc_t rop, mpc_t op, mpc_rnd_t rnd</var>)<var><a name="index-mpc_005fsqr-68"></a></var><br>
<blockquote><p>Set <var>rop</var> to the square of <var>op</var> rounded according to <var>rnd</var>. 
</p></blockquote></div>

<div class="defun">
&mdash; Function: int <b>mpc_div</b> (<var>mpc_t rop, mpc_t op1, mpc_t op2, mpc_rnd_t rnd</var>)<var><a name="index-mpc_005fdiv-69"></a></var><br>
&mdash; Function: int <b>mpc_div_ui</b> (<var>mpc_t rop, mpc_t op1, unsigned long int op2, mpc_rnd_t rnd</var>)<var><a name="index-mpc_005fdiv_005fui-70"></a></var><br>
&mdash; Function: int <b>mpc_ui_div</b> (<var>mpc_t rop, unsigned long int op1, mpc_t op2, mpc_rnd_t rnd</var>)<var><a name="index-mpc_005fui_005fdiv-71"></a></var><br>
&mdash; Function: int <b>mpc_div_fr</b> (<var>mpc_t rop, mpc_t op1, mpfr_t op2, mpc_rnd_t rnd</var>)<var><a name="index-mpc_005fdiv_005ffr-72"></a></var><br>
&mdash; Function: int <b>mpc_fr_div</b> (<var>mpc_t rop, mpfr_t op1, mpc_t op2, mpc_rnd_t rnd</var>)<var><a name="index-mpc_005ffr_005fdiv-73"></a></var><br>
<blockquote><p>Set <var>rop</var> to <var>op1</var>/<var>op2</var> rounded according to <var>rnd</var>. 
For <code>mpc_div</code> and <code>mpc_ui_div</code>, the return value may fail to
recognize some exact results. The sign of returned value is
significant only for <code>mpc_div_ui</code>. 
</p></blockquote></div>

<div class="defun">
&mdash; Function: int <b>mpc_neg</b> (<var>mpc_t rop, mpc_t op, mpc_rnd_t rnd</var>)<var><a name="index-mpc_005fneg-74"></a></var><br>
<blockquote><p>Set <var>rop</var> to &minus;<var>op</var> rounded according to <var>rnd</var>. 
Just changes the sign if <var>rop</var> and <var>op</var> are the same variable. 
</p></blockquote></div>

<div class="defun">
&mdash; Function: int <b>mpc_conj</b> (<var>mpc_t rop, mpc_t op, mpc_rnd_t rnd</var>)<var><a name="index-mpc_005fconj-75"></a></var><br>
<blockquote><p>Set <var>rop</var> to the conjugate of <var>op</var> rounded according to <var>rnd</var>. 
Just changes the sign of the imaginary part
if <var>rop</var> and <var>op</var> are the same variable. 
</p></blockquote></div>

<div class="defun">
&mdash; Function: int <b>mpc_abs</b> (<var>mpfr_t rop, mpc_t op, mp_rnd_t rnd</var>)<var><a name="index-mpc_005fabs-76"></a></var><br>
<blockquote><p>Set the floating-point number <var>rop</var> to the absolute value of <var>op</var>,
rounded in the direction <var>rnd</var>. 
The returned value is zero iff the result is exact. 
Note the destination is of type <code>mpfr_t</code>, not <code>mpc_t</code>. 
</p></blockquote></div>

<div class="defun">
&mdash; Function: int <b>mpc_norm</b> (<var>mpfr_t rop, mpc_t op, mp_rnd_t rnd</var>)<var><a name="index-mpc_005fnorm-77"></a></var><br>
<blockquote><p>Set the floating-point number <var>rop</var> to the norm of <var>op</var>
(i.e. the square of its absolute value),
rounded in the direction <var>rnd</var>. 
The returned value is zero iff the result is exact. 
Note that the destination is of type <code>mpfr_t</code>, not <code>mpc_t</code>. 
</p></blockquote></div>

<div class="defun">
&mdash; Function: int <b>mpc_mul_2exp</b> (<var>mpc_t rop, mpc_t op1, unsigned long int op2, mpc_rnd_t rnd</var>)<var><a name="index-mpc_005fmul_005f2exp-78"></a></var><br>
<blockquote><p>Set <var>rop</var> to <var>op1</var> times 2 raised to <var>op2</var>
rounded according to <var>rnd</var>. Just increases the exponents
of the real and imaginary parts by <var>op2</var>
when <var>rop</var> and <var>op1</var> are identical. 
</p></blockquote></div>

<div class="defun">
&mdash; Function: int <b>mpc_div_2exp</b> (<var>mpc_t rop, mpc_t op1, unsigned long int op2, mpc_rnd_t rnd</var>)<var><a name="index-mpc_005fdiv_005f2exp-79"></a></var><br>
<blockquote><p>Set <var>rop</var> to <var>op1</var> divided by 2 raised to <var>op2</var>
rounded according to <var>rnd</var>. Just decreases the exponents
of the real and imaginary parts by <var>op2</var>
when <var>rop</var> and <var>op1</var> are identical. 
</p></blockquote></div>

<div class="node">
<p><hr>
<a name="Power-Functions-and-Logarithm"></a>
Next:&nbsp;<a rel="next" accesskey="n" href="#Trigonometric-Functions">Trigonometric Functions</a>,
Previous:&nbsp;<a rel="previous" accesskey="p" href="#Basic-Arithmetic">Basic Arithmetic</a>,
Up:&nbsp;<a rel="up" accesskey="u" href="#Complex-Functions">Complex Functions</a>

</div>

<!-- node-name,  next,  previous,  up -->
<h3 class="section">5.7 Power Functions and Logarithm</h3>

<p><a name="index-Power-functions-80"></a><a name="index-Logarithm-81"></a>

<div class="defun">
&mdash; Function: int <b>mpc_sqrt</b> (<var>mpc_t rop, mpc_t op, mpc_rnd_t rnd</var>)<var><a name="index-mpc_005fsqrt-82"></a></var><br>
<blockquote><p>Set <var>rop</var> to the square root of <var>op</var> rounded according to <var>rnd</var>. 
Here, when the return value is 0, it means the result is exact, but the
contrary may be false. 
</p></blockquote></div>

<div class="defun">
&mdash; Function: void <b>mpc_exp</b> (<var>mpc_t rop, mpc_t op, mpc_rnd_t rnd</var>)<var><a name="index-mpc_005fexp-83"></a></var><br>
<blockquote><p>Set <var>rop</var> to the exponential of <var>op</var>,
rounded according to <var>rnd</var> with the precision of <var>rop</var>. 
</p></blockquote></div>

<div class="defun">
&mdash; Function: void <b>mpc_log</b> (<var>mpc_t rop, mpc_t op, mpc_rnd_t rnd</var>)<var><a name="index-mpc_005flog-84"></a></var><br>
<blockquote><p>Set <var>rop</var> to the logarithm of <var>op</var>,
rounded according to <var>rnd</var> with the precision of <var>rop</var>. 
The principal branch is chosen, with the branch cut on the negative real axis,
so that the imaginary part of the result lies in
]-\pi , \pi]. 
</p></blockquote></div>

<div class="node">
<p><hr>
<a name="Trigonometric-Functions"></a>
Next:&nbsp;<a rel="next" accesskey="n" href="#I_002fO-of-Complex-Numbers">I/O of Complex Numbers</a>,
Previous:&nbsp;<a rel="previous" accesskey="p" href="#Power-Functions-and-Logarithm">Power Functions and Logarithm</a>,
Up:&nbsp;<a rel="up" accesskey="u" href="#Complex-Functions">Complex Functions</a>

</div>

<!-- node-name,  next,  previous,  up -->
<h3 class="section">5.8 Trigonometric Functions</h3>

<p><a name="index-Trigonometric-functions-85"></a>

<div class="defun">
&mdash; Function: void <b>mpc_sin</b> (<var>mpc_t rop, mpc_t op, mpc_rnd_t rnd</var>)<var><a name="index-mpc_005fsin-86"></a></var><br>
<blockquote><p>Set <var>rop</var> to the sine of <var>op</var>,
rounded according to <var>rnd</var> with the precision of <var>rop</var>. 
</p></blockquote></div>

<div class="defun">
&mdash; Function: void <b>mpc_cos</b> (<var>mpc_t rop, mpc_t op, mpc_rnd_t rnd</var>)<var><a name="index-mpc_005fcos-87"></a></var><br>
<blockquote><p>Set <var>rop</var> to the cosine of <var>op</var>,
rounded according to <var>rnd</var> with the precision of <var>rop</var>. 
</p></blockquote></div>

<div class="defun">
&mdash; Function: void <b>mpc_tan</b> (<var>mpc_t rop, mpc_t op, mpc_rnd_t rnd</var>)<var><a name="index-mpc_005ftan-88"></a></var><br>
<blockquote><p>Set <var>rop</var> to the tangent of <var>op</var>,
rounded according to <var>rnd</var> with the precision of <var>rop</var>. 
</p></blockquote></div>

<div class="defun">
&mdash; Function: void <b>mpc_sinh</b> (<var>mpc_t rop, mpc_t op, mpc_rnd_t rnd</var>)<var><a name="index-mpc_005fsinh-89"></a></var><br>
<blockquote><p>Set <var>rop</var> to the hyperbolic sine of <var>op</var>,
rounded according to <var>rnd</var> with the precision of <var>rop</var>. 
</p></blockquote></div>

<div class="defun">
&mdash; Function: void <b>mpc_cosh</b> (<var>mpc_t rop, mpc_t op, mpc_rnd_t rnd</var>)<var><a name="index-mpc_005fcosh-90"></a></var><br>
<blockquote><p>Set <var>rop</var> to the hyperbolic cosine of <var>op</var>,
rounded according to <var>rnd</var> with the precision of <var>rop</var>. 
</p></blockquote></div>

<div class="defun">
&mdash; Function: void <b>mpc_tanh</b> (<var>mpc_t rop, mpc_t op, mpc_rnd_t rnd</var>)<var><a name="index-mpc_005ftanh-91"></a></var><br>
<blockquote><p>Set <var>rop</var> to the hyperbolic tangent of <var>op</var>,
rounded according to <var>rnd</var> with the precision of <var>rop</var>. 
</p></blockquote></div>

<div class="node">
<p><hr>
<a name="I%2fO-of-Complex-Numbers"></a>
<a name="I_002fO-of-Complex-Numbers"></a>
Next:&nbsp;<a rel="next" accesskey="n" href="#Miscellaneous-Complex-Functions">Miscellaneous Complex Functions</a>,
Previous:&nbsp;<a rel="previous" accesskey="p" href="#Trigonometric-Functions">Trigonometric Functions</a>,
Up:&nbsp;<a rel="up" accesskey="u" href="#Complex-Functions">Complex Functions</a>

</div>

<!-- node-name,  next,  previous,  up -->
<h3 class="section">5.9 Input and Output Functions</h3>

<p><a name="index-Complex-input-and-output-functions-92"></a><a name="index-Input-functions-93"></a><a name="index-Output-functions-94"></a><a name="index-I_002fO-functions-95"></a>
Functions that perform input from a standard input/output
stream, and functions that output to
a standard input/output stream. 
Passing a null pointer for a <var>stream</var> argument to any of
these functions will make them read from <code>stdin</code> and write to
<code>stdout</code>, respectively.

   <p>When using any of these functions, you need to include <samp><span class="file">stdio.h</span></samp>
before <samp><span class="file">mpc.h</span></samp>.

<div class="defun">
&mdash; Function: size_t <b>mpc_out_str</b> (<var>FILE *stream, int base, size_t n_digits, mpc_t op, mpc_rnd_t rnd</var>)<var><a name="index-mpc_005fout_005fstr-96"></a></var><br>
<blockquote><p>Output <var>op</var> on stdio stream <var>stream</var>, in
base <var>base</var>, rounded according to <var>rnd</var>. 
First the real part is printed, then <code>+I*</code> followed by the imaginary part. 
The base may vary from 2 to 36.  Print at most
<var>n_digits</var> significant digits for each part,
or if <var>n_digits</var> is 0, enough digits so that
<var>op</var> can be read back exactly rounding to nearest.

        <p>In addition to the significant digits, a decimal point at the right of the
first digit and a
trailing exponent, in the form &lsquo;<samp><span class="samp">eNNN</span></samp>&rsquo;, are printed.  If <var>base</var>
is greater than 10, &lsquo;<samp><span class="samp">@</span></samp>&rsquo; will be used instead of &lsquo;<samp><span class="samp">e</span></samp>&rsquo; as
exponent delimiter.

        <p>Return the number of characters written. 
</p></blockquote></div>

<div class="defun">
&mdash; Function: size_t <b>mpc_inp_str</b> (<var>mpc_t rop, FILE *stream, int base, mpc_rnd_t rnd</var>)<var><a name="index-mpc_005finp_005fstr-97"></a></var><br>
<blockquote><p>Input a string in base <var>base</var> from stdio stream <var>stream</var>,
rounded according to <var>rnd</var>, and put the
read complex in <var>rop</var>. 
Each of the real and imaginary part should be of the form &lsquo;<samp><span class="samp">M@N</span></samp>&rsquo; or,
if the base is 10 or less, alternatively &lsquo;<samp><span class="samp">MeN</span></samp>&rsquo; or &lsquo;<samp><span class="samp">MEN</span></samp>&rsquo;. 
&lsquo;<samp><span class="samp">M</span></samp>&rsquo; is the mantissa and
&lsquo;<samp><span class="samp">N</span></samp>&rsquo; is the exponent.  The mantissa is always in the specified base.  The
exponent is always read in decimal. 
This function first reads the real part, then <code>+</code> or <code>-</code> followed by
<code>I*</code> or <code>i*</code> and the absolute value of the imaginary part.

        <p>The argument <var>base</var> may be in the range 2 to 36.

        <p>Note: since the real part is read with <code>mpfr_inp_str</code>, it should be
followed by a whitespace, thus <code>17+I*42</code> is invalid, but
<code>17 +I*42</code> is valid.

        <p>Return the number of bytes read, or if an error occurred, return 0. 
</p></blockquote></div>

<div class="node">
<p><hr>
<a name="Miscellaneous-Complex-Functions"></a>
Next:&nbsp;<a rel="next" accesskey="n" href="#Internals">Internals</a>,
Previous:&nbsp;<a rel="previous" accesskey="p" href="#I_002fO-of-Complex-Numbers">I/O of Complex Numbers</a>,
Up:&nbsp;<a rel="up" accesskey="u" href="#Complex-Functions">Complex Functions</a>

</div>

<!-- node-name,  next,  previous,  up -->
<h3 class="section">5.10 Miscellaneous Functions</h3>

<p><a name="index-Miscellaneous-complex-functions-98"></a>

<div class="defun">
&mdash; Function: int <b>mpc_urandom</b> (<var>mpc_t rop, gmp_randstate_t state</var>)<var><a name="index-mpc_005furandom-99"></a></var><br>
<blockquote><p>Generate a uniformly distributed random complex in the unit square [0,
1] * [0, 1]. Return 0, unless an exponent in the real or imaginary part
is not in the current exponent range, in which case that part is set to NaN
and a zero value is returned. The second argument is a <code>gmp_randstate_t</code>
structure which should be created using the GMP <code>rand_init</code> function, see
the GMP manual. 
</p></blockquote></div>

<div class="defun">
&mdash; Function: void <b>mpc_random</b> (<var>mpc_t rop</var>)<var><a name="index-mpc_005frandom-100"></a></var><br>
<blockquote><p>Generate a random complex, with real and imaginary parts uniformly
distributed in the interval -1 &lt; X &lt; 1. 
</p></blockquote></div>

<div class="defun">
&mdash; Function: void <b>mpc_random2</b> (<var>mpc_t rop, mp_size_t max_size, mp_exp_t max_exp</var>)<var><a name="index-mpc_005frandom2-101"></a></var><br>
<blockquote><p>Generate a random complex, with real and imaginary part
of at most <var>max_size</var> limbs, with long strings of
zeros and ones in the binary representation. The exponent of the
real (resp. imaginary) part is in
the interval &minus;<var>exp</var> to <var>exp</var>. 
This function is useful for
testing functions and algorithms, since this kind of random numbers have
proven to be more likely to trigger corner-case bugs. 
Negative parts are generated when <var>max_size</var> is negative. 
</p></blockquote></div>

<div class="defun">
&mdash; Function: const char * <b>mpc_get_version</b> (<var>void</var>)<var><a name="index-mpc_005fget_005fversion-102"></a></var><br>
<blockquote><p>Return the MPC version, as a null-terminated string. 
</p></blockquote></div>

<div class="defun">
&mdash; Macro: <b>MPC_VERSION</b><var><a name="index-MPC_005fVERSION-103"></a></var><br>
&mdash; Macro: <b>MPC_VERSION_MAJOR</b><var><a name="index-MPC_005fVERSION_005fMAJOR-104"></a></var><br>
&mdash; Macro: <b>MPC_VERSION_MINOR</b><var><a name="index-MPC_005fVERSION_005fMINOR-105"></a></var><br>
&mdash; Macro: <b>MPC_VERSION_PATCHLEVEL</b><var><a name="index-MPC_005fVERSION_005fPATCHLEVEL-106"></a></var><br>
&mdash; Macro: <b>MPC_VERSION_STRING</b><var><a name="index-MPC_005fVERSION_005fSTRING-107"></a></var><br>
<blockquote><p><code>MPC_VERSION</code> is the version of MPC as a preprocessing constant. 
<code>MPC_VERSION_MAJOR</code>, <code>MPC_VERSION_MINOR</code> and
<code>MPC_VERSION_PATCHLEVEL</code> are respectively the major, minor and
patch level of MPC version, as preprocessing constants. 
<code>MPC_VERSION_STRING</code> is the version as a string constant, which
can be compared to the result of <code>mpc_get_version</code> to check at
run time the header file and library used match:
     <pre class="example">          if (strcmp (mpc_get_version (), MPC_VERSION_STRING))
            fprintf (stderr, "Warning: header and library do not match\n");
</pre>
        <p>Note: Obtaining different strings is not necessarily an error, as in
general, a program compiled with some old MPC version can be
dynamically linked with a newer MPC library version (if allowed by the
library versioning system). 
</p></blockquote></div>

<div class="defun">
&mdash; Macro: long <b>MPC_VERSION_NUM</b> (<var>major, minor, patchlevel</var>)<var><a name="index-MPC_005fVERSION_005fNUM-108"></a></var><br>
<blockquote><p>Create an integer in the same format as used by <code>MPC_VERSION</code> from the
given <var>major</var>, <var>minor</var> and <var>patchlevel</var>. 
Here is an example of how to check the MPC version at compile time:
     <pre class="example">          #if (!defined(MPC_VERSION) || (MPC_VERSION&lt;MPC_VERSION_NUM(2,1,0)))
          # error "Wrong MPC version."
          #endif
</pre>
        </blockquote></div>

<div class="node">
<p><hr>
<a name="Internals"></a>
Previous:&nbsp;<a rel="previous" accesskey="p" href="#Miscellaneous-Complex-Functions">Miscellaneous Complex Functions</a>,
Up:&nbsp;<a rel="up" accesskey="u" href="#Complex-Functions">Complex Functions</a>

</div>

<!-- node-name,  next,  previous,  up -->
<h3 class="section">5.11 Internals</h3>

<p>These types and
functions were mainly designed for the implementation of MPC,
but may be useful for users too. 
However no upward compatibility is guaranteed. 
You need to include <code>mpc-impl.h</code> to use them.

   <p>The <code>mpc_t</code> type consists of two fields of type <code>mpfr_t</code>,
one for the real part, one for the imaginary part. 
These fields can be accessed through <code>MPC_RE(z)</code> and <code>MPC_IM(z)</code>.

   <p>Normally the real and imaginary part have the same precision, but
the function <code>mpc_init3</code> enables one to have different precisions,
and the user may also use <code>mpfr_set_prec</code> to change their precision. 
The macro <code>MPC_MAX_PREC(z)</code> gives the maximum of the precisions
of the real and imaginary parts.

<div class="node">
<p><hr>
<a name="Contributors"></a>
Next:&nbsp;<a rel="next" accesskey="n" href="#References">References</a>,
Previous:&nbsp;<a rel="previous" accesskey="p" href="#Complex-Functions">Complex Functions</a>,
Up:&nbsp;<a rel="up" accesskey="u" href="#Top">Top</a>

</div>

<!-- node-name,  next,  previous,  up -->
<h2 class="unnumbered">Contributors</h2>

<p>The main developers of the MPC library are Andreas Enge,
Philippe Th&eacute;veny and Paul Zimmermann. 
Patrick P&eacute;lissier has helped cleaning up the code. 
Marc Helbling contributed the <code>mpc_ui_sub</code> and
<code>mpc_ui_ui_sub</code> functions.

<div class="node">
<p><hr>
<a name="References"></a>
Next:&nbsp;<a rel="next" accesskey="n" href="#Concept-Index">Concept Index</a>,
Previous:&nbsp;<a rel="previous" accesskey="p" href="#Contributors">Contributors</a>,
Up:&nbsp;<a rel="up" accesskey="u" href="#Top">Top</a>

</div>

<!-- node-name,  next,  previous,  up -->
<h2 class="unnumbered">References</h2>

     <ul>
<li>Torbj&ouml;rn Granlund et al. 
<code>gmp</code> &ndash; GNU multiprecision library. 
Version 4.2.2, <a href="http://gmplib.org/">http://gmplib.org/</a>.

     <li>Guillaume Hanrot, Vincent Lef&egrave;vre, Patrick P&eacute;lissier, Paul Zimmermann et al. 
<code>mpfr</code> &ndash; A library for multiple-precision floating-point               computations with exact rounding. 
Version 2.3.1, <a href="http://www.mpfr.org">http://www.mpfr.org</a>.

     <li>IEEE standard for binary floating-point arithmetic, Technical Report
ANSI-IEEE Standard 754-1985, New York, 1985. 
Approved March 21, 1985: IEEE Standards Board; approved July 26,
  1985: American National Standards Institute, 18 pages.

     <li>Donald E. Knuth, "The Art of Computer Programming", vol 2,
"Seminumerical Algorithms", 2nd edition, Addison-Wesley, 1981.

</ul>

<div class="node">
<p><hr>
<a name="Concept-Index"></a>
Next:&nbsp;<a rel="next" accesskey="n" href="#Function-Index">Function Index</a>,
Previous:&nbsp;<a rel="previous" accesskey="p" href="#References">References</a>,
Up:&nbsp;<a rel="up" accesskey="u" href="#Top">Top</a>

</div>

<!-- node-name,  next,  previous,  up -->
<h2 class="unnumbered">Concept Index</h2>

<ul class="index-cp" compact>
<li><a href="#index-Arithmetic-functions-53">Arithmetic functions</a>: <a href="#Basic-Arithmetic">Basic Arithmetic</a></li>
<li><a href="#index-Comparison-functions-41">Comparison functions</a>: <a href="#Complex-Comparison">Complex Comparison</a></li>
<li><a href="#index-Complex-arithmetic-functions-52">Complex arithmetic functions</a>: <a href="#Basic-Arithmetic">Basic Arithmetic</a></li>
<li><a href="#index-Complex-assignment-functions-23">Complex assignment functions</a>: <a href="#Assigning-Complex-Numbers">Assigning Complex Numbers</a></li>
<li><a href="#index-Complex-comparisons-functions-40">Complex comparisons functions</a>: <a href="#Complex-Comparison">Complex Comparison</a></li>
<li><a href="#index-Complex-functions-12">Complex functions</a>: <a href="#Complex-Functions">Complex Functions</a></li>
<li><a href="#index-Complex-input-and-output-functions-92">Complex input and output functions</a>: <a href="#I_002fO-of-Complex-Numbers">I/O of Complex Numbers</a></li>
<li><a href="#index-Complex-number-6">Complex number</a>: <a href="#MPC-Basics">MPC Basics</a></li>
<li><a href="#index-Conditions-for-copying-MPC-2">Conditions for copying MPC</a>: <a href="#Copying">Copying</a></li>
<li><a href="#index-Copying-conditions-1">Copying conditions</a>: <a href="#Copying">Copying</a></li>
<li><a href="#index-I_002fO-functions-95">I/O functions</a>: <a href="#I_002fO-of-Complex-Numbers">I/O of Complex Numbers</a></li>
<li><a href="#index-Initialization-and-assignment-functions-34">Initialization and assignment functions</a>: <a href="#Simultaneous-Init-_0026-Assign">Simultaneous Init &amp; Assign</a></li>
<li><a href="#index-Input-functions-93">Input functions</a>: <a href="#I_002fO-of-Complex-Numbers">I/O of Complex Numbers</a></li>
<li><a href="#index-Installation-3">Installation</a>: <a href="#Installing-MPC">Installing MPC</a></li>
<li><a href="#index-Logarithm-81">Logarithm</a>: <a href="#Power-Functions-and-Logarithm">Power Functions and Logarithm</a></li>
<li><a href="#index-Miscellaneous-complex-functions-98">Miscellaneous complex functions</a>: <a href="#Miscellaneous-Complex-Functions">Miscellaneous Complex Functions</a></li>
<li><a href="#index-g_t_0040file_007bmpc_002eh_007d-5"><samp><span class="file">mpc.h</span></samp></a>: <a href="#MPC-Basics">MPC Basics</a></li>
<li><a href="#index-Output-functions-94">Output functions</a>: <a href="#I_002fO-of-Complex-Numbers">I/O of Complex Numbers</a></li>
<li><a href="#index-Power-functions-80">Power functions</a>: <a href="#Power-Functions-and-Logarithm">Power Functions and Logarithm</a></li>
<li><a href="#index-Precision-8">Precision</a>: <a href="#MPC-Basics">MPC Basics</a></li>
<li><a href="#index-Projection-and-Decomposing-Functions-45">Projection and Decomposing Functions</a>: <a href="#Projection-_0026-Decomposing">Projection &amp; Decomposing</a></li>
<li><a href="#index-Reporting-bugs-4">Reporting bugs</a>: <a href="#Reporting-Bugs">Reporting Bugs</a></li>
<li><a href="#index-Rounding-Mode-10">Rounding Mode</a>: <a href="#MPC-Basics">MPC Basics</a></li>
<li><a href="#index-Trigonometric-functions-85">Trigonometric functions</a>: <a href="#Trigonometric-Functions">Trigonometric Functions</a></li>
<li><a href="#index-User_002ddefined-precision-13">User-defined precision</a>: <a href="#Complex-Functions">Complex Functions</a></li>
</ul><div class="node">
<p><hr>
<a name="Function-Index"></a>
Previous:&nbsp;<a rel="previous" accesskey="p" href="#Concept-Index">Concept Index</a>,
Up:&nbsp;<a rel="up" accesskey="u" href="#Top">Top</a>

</div>

<!-- node-name,  next,  previous,  up -->
<h2 class="unnumbered">Function and Type Index</h2>



<ul class="index-fn" compact>
<li><a href="#index-g_t_0040code_007bmp_005fprec_005ft_007d-9"><code>mp_prec_t</code></a>: <a href="#MPC-Basics">MPC Basics</a></li>
<li><a href="#index-mpc_005fabs-76"><code>mpc_abs</code></a>: <a href="#Basic-Arithmetic">Basic Arithmetic</a></li>
<li><a href="#index-mpc_005fadd-54"><code>mpc_add</code></a>: <a href="#Basic-Arithmetic">Basic Arithmetic</a></li>
<li><a href="#index-mpc_005fadd_005ffr-56"><code>mpc_add_fr</code></a>: <a href="#Basic-Arithmetic">Basic Arithmetic</a></li>
<li><a href="#index-mpc_005fadd_005fui-55"><code>mpc_add_ui</code></a>: <a href="#Basic-Arithmetic">Basic Arithmetic</a></li>
<li><a href="#index-mpc_005farg-50"><code>mpc_arg</code></a>: <a href="#Projection-_0026-Decomposing">Projection &amp; Decomposing</a></li>
<li><a href="#index-mpc_005fclear-19"><code>mpc_clear</code></a>: <a href="#Initializing-Complex-Numbers">Initializing Complex Numbers</a></li>
<li><a href="#index-mpc_005fcmp-42"><code>mpc_cmp</code></a>: <a href="#Complex-Comparison">Complex Comparison</a></li>
<li><a href="#index-mpc_005fcmp_005fsi-44"><code>mpc_cmp_si</code></a>: <a href="#Complex-Comparison">Complex Comparison</a></li>
<li><a href="#index-mpc_005fcmp_005fsi_005fsi-43"><code>mpc_cmp_si_si</code></a>: <a href="#Complex-Comparison">Complex Comparison</a></li>
<li><a href="#index-mpc_005fconj-75"><code>mpc_conj</code></a>: <a href="#Basic-Arithmetic">Basic Arithmetic</a></li>
<li><a href="#index-mpc_005fcos-87"><code>mpc_cos</code></a>: <a href="#Trigonometric-Functions">Trigonometric Functions</a></li>
<li><a href="#index-mpc_005fcosh-90"><code>mpc_cosh</code></a>: <a href="#Trigonometric-Functions">Trigonometric Functions</a></li>
<li><a href="#index-mpc_005fdiv-69"><code>mpc_div</code></a>: <a href="#Basic-Arithmetic">Basic Arithmetic</a></li>
<li><a href="#index-mpc_005fdiv_005f2exp-79"><code>mpc_div_2exp</code></a>: <a href="#Basic-Arithmetic">Basic Arithmetic</a></li>
<li><a href="#index-mpc_005fdiv_005ffr-72"><code>mpc_div_fr</code></a>: <a href="#Basic-Arithmetic">Basic Arithmetic</a></li>
<li><a href="#index-mpc_005fdiv_005fui-70"><code>mpc_div_ui</code></a>: <a href="#Basic-Arithmetic">Basic Arithmetic</a></li>
<li><a href="#index-mpc_005fexp-83"><code>mpc_exp</code></a>: <a href="#Power-Functions-and-Logarithm">Power Functions and Logarithm</a></li>
<li><a href="#index-mpc_005ffr_005fdiv-73"><code>mpc_fr_div</code></a>: <a href="#Basic-Arithmetic">Basic Arithmetic</a></li>
<li><a href="#index-mpc_005ffr_005fsub-59"><code>mpc_fr_sub</code></a>: <a href="#Basic-Arithmetic">Basic Arithmetic</a></li>
<li><a href="#index-mpc_005fget_005fdefault_005fprec-15"><code>mpc_get_default_prec</code></a>: <a href="#Initializing-Complex-Numbers">Initializing Complex Numbers</a></li>
<li><a href="#index-mpc_005fget_005fprec-21"><code>mpc_get_prec</code></a>: <a href="#Initializing-Complex-Numbers">Initializing Complex Numbers</a></li>
<li><a href="#index-mpc_005fget_005fprec2-22"><code>mpc_get_prec2</code></a>: <a href="#Initializing-Complex-Numbers">Initializing Complex Numbers</a></li>
<li><a href="#index-mpc_005fget_005fversion-102"><code>mpc_get_version</code></a>: <a href="#Miscellaneous-Complex-Functions">Miscellaneous Complex Functions</a></li>
<li><a href="#index-mpc_005fimag-47"><code>mpc_imag</code></a>: <a href="#Projection-_0026-Decomposing">Projection &amp; Decomposing</a></li>
<li><a href="#index-mpc_005fimagref-49"><code>mpc_imagref</code></a>: <a href="#Projection-_0026-Decomposing">Projection &amp; Decomposing</a></li>
<li><a href="#index-mpc_005finit-16"><code>mpc_init</code></a>: <a href="#Initializing-Complex-Numbers">Initializing Complex Numbers</a></li>
<li><a href="#index-mpc_005finit2-17"><code>mpc_init2</code></a>: <a href="#Initializing-Complex-Numbers">Initializing Complex Numbers</a></li>
<li><a href="#index-mpc_005finit3-18"><code>mpc_init3</code></a>: <a href="#Initializing-Complex-Numbers">Initializing Complex Numbers</a></li>
<li><a href="#index-mpc_005finit_005fset-35"><code>mpc_init_set</code></a>: <a href="#Simultaneous-Init-_0026-Assign">Simultaneous Init &amp; Assign</a></li>
<li><a href="#index-mpc_005finit_005fset_005fsi_005fsi-39"><code>mpc_init_set_si_si</code></a>: <a href="#Simultaneous-Init-_0026-Assign">Simultaneous Init &amp; Assign</a></li>
<li><a href="#index-mpc_005finit_005fset_005fui-36"><code>mpc_init_set_ui</code></a>: <a href="#Simultaneous-Init-_0026-Assign">Simultaneous Init &amp; Assign</a></li>
<li><a href="#index-mpc_005finit_005fset_005fui_005ffr-38"><code>mpc_init_set_ui_fr</code></a>: <a href="#Simultaneous-Init-_0026-Assign">Simultaneous Init &amp; Assign</a></li>
<li><a href="#index-mpc_005finit_005fset_005fui_005fui-37"><code>mpc_init_set_ui_ui</code></a>: <a href="#Simultaneous-Init-_0026-Assign">Simultaneous Init &amp; Assign</a></li>
<li><a href="#index-mpc_005finp_005fstr-97"><code>mpc_inp_str</code></a>: <a href="#I_002fO-of-Complex-Numbers">I/O of Complex Numbers</a></li>
<li><a href="#index-mpc_005flog-84"><code>mpc_log</code></a>: <a href="#Power-Functions-and-Logarithm">Power Functions and Logarithm</a></li>
<li><a href="#index-mpc_005fmul-63"><code>mpc_mul</code></a>: <a href="#Basic-Arithmetic">Basic Arithmetic</a></li>
<li><a href="#index-mpc_005fmul_005f2exp-78"><code>mpc_mul_2exp</code></a>: <a href="#Basic-Arithmetic">Basic Arithmetic</a></li>
<li><a href="#index-mpc_005fmul_005ffr-66"><code>mpc_mul_fr</code></a>: <a href="#Basic-Arithmetic">Basic Arithmetic</a></li>
<li><a href="#index-mpc_005fmul_005fi-67"><code>mpc_mul_i</code></a>: <a href="#Basic-Arithmetic">Basic Arithmetic</a></li>
<li><a href="#index-mpc_005fmul_005fsi-65"><code>mpc_mul_si</code></a>: <a href="#Basic-Arithmetic">Basic Arithmetic</a></li>
<li><a href="#index-mpc_005fmul_005fui-64"><code>mpc_mul_ui</code></a>: <a href="#Basic-Arithmetic">Basic Arithmetic</a></li>
<li><a href="#index-mpc_005fneg-74"><code>mpc_neg</code></a>: <a href="#Basic-Arithmetic">Basic Arithmetic</a></li>
<li><a href="#index-mpc_005fnorm-77"><code>mpc_norm</code></a>: <a href="#Basic-Arithmetic">Basic Arithmetic</a></li>
<li><a href="#index-mpc_005fout_005fstr-96"><code>mpc_out_str</code></a>: <a href="#I_002fO-of-Complex-Numbers">I/O of Complex Numbers</a></li>
<li><a href="#index-mpc_005fproj-51"><code>mpc_proj</code></a>: <a href="#Projection-_0026-Decomposing">Projection &amp; Decomposing</a></li>
<li><a href="#index-mpc_005frandom-100"><code>mpc_random</code></a>: <a href="#Miscellaneous-Complex-Functions">Miscellaneous Complex Functions</a></li>
<li><a href="#index-mpc_005frandom2-101"><code>mpc_random2</code></a>: <a href="#Miscellaneous-Complex-Functions">Miscellaneous Complex Functions</a></li>
<li><a href="#index-mpc_005freal-46"><code>mpc_real</code></a>: <a href="#Projection-_0026-Decomposing">Projection &amp; Decomposing</a></li>
<li><a href="#index-mpc_005frealref-48"><code>mpc_realref</code></a>: <a href="#Projection-_0026-Decomposing">Projection &amp; Decomposing</a></li>
<li><a href="#index-g_t_0040code_007bmpc_005frnd_005ft_007d-11"><code>mpc_rnd_t</code></a>: <a href="#MPC-Basics">MPC Basics</a></li>
<li><a href="#index-mpc_005fset-24"><code>mpc_set</code></a>: <a href="#Assigning-Complex-Numbers">Assigning Complex Numbers</a></li>
<li><a href="#index-mpc_005fset_005fd-27"><code>mpc_set_d</code></a>: <a href="#Assigning-Complex-Numbers">Assigning Complex Numbers</a></li>
<li><a href="#index-mpc_005fset_005fd_005fd-29"><code>mpc_set_d_d</code></a>: <a href="#Assigning-Complex-Numbers">Assigning Complex Numbers</a></li>
<li><a href="#index-mpc_005fset_005fdefault_005fprec-14"><code>mpc_set_default_prec</code></a>: <a href="#Initializing-Complex-Numbers">Initializing Complex Numbers</a></li>
<li><a href="#index-mpc_005fset_005ffr-28"><code>mpc_set_fr</code></a>: <a href="#Assigning-Complex-Numbers">Assigning Complex Numbers</a></li>
<li><a href="#index-mpc_005fset_005ffr_005ffr-32"><code>mpc_set_fr_fr</code></a>: <a href="#Assigning-Complex-Numbers">Assigning Complex Numbers</a></li>
<li><a href="#index-mpc_005fset_005fprec-20"><code>mpc_set_prec</code></a>: <a href="#Initializing-Complex-Numbers">Initializing Complex Numbers</a></li>
<li><a href="#index-mpc_005fset_005fsi-26"><code>mpc_set_si</code></a>: <a href="#Assigning-Complex-Numbers">Assigning Complex Numbers</a></li>
<li><a href="#index-mpc_005fset_005fsi_005fsi-31"><code>mpc_set_si_si</code></a>: <a href="#Assigning-Complex-Numbers">Assigning Complex Numbers</a></li>
<li><a href="#index-mpc_005fset_005fui-25"><code>mpc_set_ui</code></a>: <a href="#Assigning-Complex-Numbers">Assigning Complex Numbers</a></li>
<li><a href="#index-mpc_005fset_005fui_005ffr-33"><code>mpc_set_ui_fr</code></a>: <a href="#Assigning-Complex-Numbers">Assigning Complex Numbers</a></li>
<li><a href="#index-mpc_005fset_005fui_005fui-30"><code>mpc_set_ui_ui</code></a>: <a href="#Assigning-Complex-Numbers">Assigning Complex Numbers</a></li>
<li><a href="#index-mpc_005fsin-86"><code>mpc_sin</code></a>: <a href="#Trigonometric-Functions">Trigonometric Functions</a></li>
<li><a href="#index-mpc_005fsinh-89"><code>mpc_sinh</code></a>: <a href="#Trigonometric-Functions">Trigonometric Functions</a></li>
<li><a href="#index-mpc_005fsqr-68"><code>mpc_sqr</code></a>: <a href="#Basic-Arithmetic">Basic Arithmetic</a></li>
<li><a href="#index-mpc_005fsqrt-82"><code>mpc_sqrt</code></a>: <a href="#Power-Functions-and-Logarithm">Power Functions and Logarithm</a></li>
<li><a href="#index-mpc_005fsub-57"><code>mpc_sub</code></a>: <a href="#Basic-Arithmetic">Basic Arithmetic</a></li>
<li><a href="#index-mpc_005fsub_005ffr-58"><code>mpc_sub_fr</code></a>: <a href="#Basic-Arithmetic">Basic Arithmetic</a></li>
<li><a href="#index-mpc_005fsub_005fui-60"><code>mpc_sub_ui</code></a>: <a href="#Basic-Arithmetic">Basic Arithmetic</a></li>
<li><a href="#index-g_t_0040code_007bmpc_005ft_007d-7"><code>mpc_t</code></a>: <a href="#MPC-Basics">MPC Basics</a></li>
<li><a href="#index-mpc_005ftan-88"><code>mpc_tan</code></a>: <a href="#Trigonometric-Functions">Trigonometric Functions</a></li>
<li><a href="#index-mpc_005ftanh-91"><code>mpc_tanh</code></a>: <a href="#Trigonometric-Functions">Trigonometric Functions</a></li>
<li><a href="#index-mpc_005fui_005fdiv-71"><code>mpc_ui_div</code></a>: <a href="#Basic-Arithmetic">Basic Arithmetic</a></li>
<li><a href="#index-mpc_005fui_005fsub-61"><code>mpc_ui_sub</code></a>: <a href="#Basic-Arithmetic">Basic Arithmetic</a></li>
<li><a href="#index-mpc_005fui_005fui_005fsub-62"><code>mpc_ui_ui_sub</code></a>: <a href="#Basic-Arithmetic">Basic Arithmetic</a></li>
<li><a href="#index-mpc_005furandom-99"><code>mpc_urandom</code></a>: <a href="#Miscellaneous-Complex-Functions">Miscellaneous Complex Functions</a></li>
<li><a href="#index-MPC_005fVERSION-103"><code>MPC_VERSION</code></a>: <a href="#Miscellaneous-Complex-Functions">Miscellaneous Complex Functions</a></li>
<li><a href="#index-MPC_005fVERSION_005fMAJOR-104"><code>MPC_VERSION_MAJOR</code></a>: <a href="#Miscellaneous-Complex-Functions">Miscellaneous Complex Functions</a></li>
<li><a href="#index-MPC_005fVERSION_005fMINOR-105"><code>MPC_VERSION_MINOR</code></a>: <a href="#Miscellaneous-Complex-Functions">Miscellaneous Complex Functions</a></li>
<li><a href="#index-MPC_005fVERSION_005fNUM-108"><code>MPC_VERSION_NUM</code></a>: <a href="#Miscellaneous-Complex-Functions">Miscellaneous Complex Functions</a></li>
<li><a href="#index-MPC_005fVERSION_005fPATCHLEVEL-106"><code>MPC_VERSION_PATCHLEVEL</code></a>: <a href="#Miscellaneous-Complex-Functions">Miscellaneous Complex Functions</a></li>
<li><a href="#index-MPC_005fVERSION_005fSTRING-107"><code>MPC_VERSION_STRING</code></a>: <a href="#Miscellaneous-Complex-Functions">Miscellaneous Complex Functions</a></li>
   </ul></body></html>

