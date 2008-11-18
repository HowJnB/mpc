<p>
<?php code("Mpc"); ?> is
a C library for the arithmetic of complex numbers with arbitrarily
high precision and correct rounding of the result.
It is built upon and follows the same principles as
<a href="http://www.mpfr.org/"><?php code ("Mpfr"); ?></a>.
The library is  written by
<a href="http://www.lix.polytechnique.fr/Labo/Andreas.Enge">Andreas
Enge</a>,
<a href="http://www.loria.fr/~thevenyp/">Philippe Théveny</a> and
<a href="http://www.loria.fr/~zimmerma/">Paul Zimmermann</a>
and is distributed under the
<a href="http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html">
<?php code ("Gnu"); ?> Lesser General Public License</a>,
either version 2.1 of the license, or (at your option) any later version.
The <?php code ("Mpc"); ?> library has been registered in France by the
<a href="http://app.legalis.net/">Agence pour la Protection des
Programmes</a> on 2003-02-05
under the number IDDN FR 001 060029 000 R P 2003 000 10000.
</p>

<h2>News</h2>
<p>
<a href="?content=download">Version 0.5.1</a>,
"Aconitum neomontanum", released in November 2008,
comes with the following new features:
</p>
<ul>
<li>
New functions
<ul>
<li>
<code>set_fr_fr</code>
</li>
<li>
<code>real</code>, <code>imag</code>
</li>
<li>
<code>arg</code>, <code>proj</code>
</li>
</ul>
</li>
<li>
New macros
<ul>
<li>
Version number: <code>MPC_VERSION_MAJOR</code>,
<code>MPC_VERSION_MINOR</code>, <code>MPC_VERSION_PATCHLEVEL</code>,
<code>MPC_VERSION_STRING</code>
</li>
<li>
References to number parts:
<code>realref</code>, <code>imagref</code>
</li>
</ul>
</li>
<li>
Rewritten test framework, leading to many...
</li>
<li>
Bug fixes
<ul>
<li>
<code>configure</code> looks for <code>gmp</code> (>= 4.2) first,
then for <code>mpfr</code> (>= 2.3.1)
</li>
<li>
<code>cos</code>, <code>div</code>, <code>div_fr</code>, <code>fr_sub</code>,
<code>mul_fr</code>, <code>set_fr</code>, <code>sqr</code>
</li>
<li>
fixed handling of special values:
<code>exp</code>, <code>log</code>, <code>mul</code>, <code>norm</code>,
<code>sqr</code>, <code>sqrt</code>
</li>
</ul>
</li>
<li>
Optimisations:
<code>mul_fr</code>, <code>sin</code>, <code>cos</code>
</li>
<li>
New <code>configure</code> options:
<code>--with-gmp-lib</code>, <code>--with-gmp-include</code>,
<code>--with-mpfr-lib</code>, <code>--with-mpfr-include</code>
</li>
<li>
Export declarations for MSWindows, <code>makefile.vc</code> updated
(contributed by Mickaël Gastineau)
</li>
</ul>


<h2>How to properly cite <?php code ("Mpc"); ?></h2>
<pre>
@manual {mpc,
   author       = {Andreas Enge and Philippe Th\'eveny and Paul Zimmermann},
   title        = {mpc --- A library for multiprecision complex
                   arithmetic with exact rounding},
   organization = {INRIA},
   edition      = {0.5.1},
   year         = {2008},
   month        = nov,
   note         = {\url{http://www.multiprecision.org/mpc/}}
}
</pre>
