
<!-- begin footer.php -->

<!-- $Id: footer.php,v 1.21 2004/06/05 18:12:43 christhecat Exp $ -->

<hr />

<p><?php echo $title; ?>
<br /><?php echo "Last modified " . date( "F d, Y", getlastmod() ); ?>
<br /><?php if ($_SERVER['PHP_SELF'] == "/index.php") { echo "http://galaxyng.sourceforge.net/"; } else { echo "http://galaxyng.sourceforge.net$_SERVER[PHP_SELF]"; } ?></p>
<p><?php if ($_SERVER['PHP_SELF'] == "/index.php") { ?>GalaxyNG 6.4.6<?php } else { ?><a href="/">GalaxyNG 6.4.6</a>.<?php } ?>
<!-- Creative Commons License -->
<br /><a href="http://creativecommons.org/licenses/by-nc-sa/2.0/"><img alt="Creative Commons License" src="http://creativecommons.org/images/public/somerights.gif" /></a>
<br />This web site is licensed under a <a href="http://creativecommons.org/licenses/by-nc-sa/2.0/">Creative Commons License</a>.
<br />This software is licensed under the <a href="http://www.gnu.org/copyleft/gpl.html">GNU General Public License</a>.<?php if ($_SERVER['PHP_SELF'] != "/credits.php") { ?><br /><a href="/credits.php">Credits and Licenses</a>.<?php } ?></p>
<!-- /Creative Commons License -->
<p><a href="http://sourceforge.net/"><img src="http://sourceforge.net/sflogo.php?group_id=48224&amp;type=6" width="210" height="62" alt="SourceForge.net Logo" /></a></p>

<!-- end footer.php -->
