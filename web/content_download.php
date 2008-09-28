<div style="float:right">
<a href="http://de.wikipedia.org/w/index.php?title=Bild:Blauer_Eisenhut_Dolomiten.jpg&amp;filetimestamp=20070808064748"><img alt="Aconitum" 
src="http://upload.wikimedia.org/wikipedia/commons/thumb/e/e5/Blauer_Eisenhut_Dolomiten.jpg/300px-Blauer_Eisenhut_Dolomiten.jpg"/></a>
</div>

<h2>Requirements</h2>
<ul>
<li>
<a href="http://gmplib.org/"><?php code ("Gmp"); ?></a>
version 4.2.2 or higher
</li>
<li>
<a href="http://www.mpfr.org/"><?php code ("Mpfr"); ?></a>
version 2.3.0 or higher
</li>
</ul>

<h2>Latest version</h2>
<a href="download/mpc-0.5.tar.gz">Version 0.5</a> "Aconitum neomontanum"

<h2>Patches</h2>
<p>
The patches are applied as follows:<br/>
<code>cd mpc-0.5</code><br/>
<code>patch -N -p1 -Z -i path_to_patch_file</code></br>
</p>
<ul>
<li>
<a href="download/configure.patch">Patch</a> for correctly
recognising static <?php code("Gmp"); ?> and
<?php code("Mpfr"); ?> libraries
</li>
</ul>
