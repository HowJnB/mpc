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
<a href="?content=download">Version 0.5</a>,
"Aconitum neomontanum", released in September 2008,
comes with the following new features:
</p>
<ul>
<li>
Support for <code>autotools</code>
</li>
<li>
New functions
<ul>
<li>
<code>log</code>
</li>
<li>
trigonometric functions: <code>cos</code>, <code>tan</code>
(<code>sin</code> was already there)
</li>
<li>
hyperbolic functions: <code>cosh</code>, <code>sinh</code>,
<code>tanh</code>
</li>
</ul>
</li>
<li>
Bug fixes:
<ul>
<li>
<code>sqrt</code> with directed rounding
</li>
</ul>
</li>
</ul>


<h2>How to properly cite <?php code ("Mpc"); ?></h2>
<pre>
@manual {mpc,
   author       = {Andreas Enge and Philippe Th\'eveny and Paul Zimmermann},
   title        = {mpc --- A library for multiprecision complex
                   arithmetic with exact rounding},
   organization = {INRIA},
   edition      = {0.5},
   year         = {2008},
   month        = {sep},
   note         = {\url{http://www.multiprecision.org/mpc/}}
}
</pre>
