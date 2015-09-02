
<!-- start menu.php -->

<!-- $Id: menu.php,v 1.10 2004/03/24 13:40:56 christhecat Exp $ -->

<p class="menu"><?php if ($_SERVER['PHP_SELF'] == "/index.php") { ?><span class="bold">[&nbsp;GalaxyNG&nbsp;Home&nbsp;]</span><?php } else { ?>[&nbsp;<a href="/">GalaxyNG&nbsp;Home</a>&nbsp;]<?php } ?>	

<?php if ($_SERVER['PHP_SELF'] == "/start.php") { ?><span class="bold">[&nbsp;Getting&nbsp;Started&nbsp;]</span><?php } else { ?>[&nbsp;<a href="/start.php">Getting&nbsp;Started</a>&nbsp;]<?php } ?>

<?php if ($_SERVER['PHP_SELF'] == "/manual.php") { ?><span class="bold">[&nbsp;Players'&nbsp;Manual&nbsp;]</span><?php } else { ?>[&nbsp;<a href="/manual.php">Players'&nbsp;Manual</a>&nbsp;]<?php } ?>

<?php if ($_SERVER['PHP_SELF'] == "/quickref.php") { ?><span class="bold">[&nbsp;Quick&nbsp;Reference&nbsp;]</span><?php } else { ?>[&nbsp;<a href="/quickref.php">Quick&nbsp;Reference</a>&nbsp;]<?php } ?>

[&nbsp;<a href="http://galaxyview.sourceforge.net/tools.html">Tools</a>&nbsp;]

<?php if ($_SERVER['PHP_SELF'] == "/guides/index.php") { ?><span class="bold">[&nbsp;Guides&nbsp;and&nbsp;Reports&nbsp;]</span><?php } else { ?>[&nbsp;<a href="/guides/">Guides&nbsp;and&nbsp;Reports</a>&nbsp;]<?php } ?>

<?php if ($_SERVER['PHP_SELF'] == "/faq.php") { ?><span class="bold">[&nbsp;FAQ&nbsp;]</span><?php } else { ?>[&nbsp;<a href="/faq.php">FAQ</a>&nbsp;]<?php } ?>

[&nbsp;<a href="http://lists.sourceforge.net/lists/listinfo/galaxyng-players">Mailing&nbsp;List</a>&nbsp;]

<?php if ($_SERVER['PHP_SELF'] == "/hall/index.php") { ?><span class="bold">[&nbsp;Hall&nbsp;Of&nbsp;Fame&nbsp;]</span><?php } else { ?>[&nbsp;<a href="/hall/">Hall&nbsp;Of&nbsp;Fame</a>&nbsp;]<?php } ?>

<?php if ($_SERVER['PHP_SELF'] == "/links.php") { ?><span class="bold">[&nbsp;Links&nbsp;]</span><?php } else { ?>[&nbsp;<a href="/links.php">Links</a>&nbsp;]<?php } ?>

<?php if ($_SERVER['PHP_SELF'] == "/server.php") { ?><span class="bold">[&nbsp;Server&nbsp;Manual&nbsp;]</span><?php } else { ?>[&nbsp;<a href="/server.php">Server&nbsp;Manual</a>&nbsp;]<?php } ?>

[&nbsp;<a href="http://sourceforge.net/projects/galaxyng/">Development</a>&nbsp;]

<?php if ($_SERVER['PHP_SELF'] == "/variants.php") { ?><span class="bold">[&nbsp;Galaxy&nbsp;Variants&nbsp;]</span><?php } else { ?>[&nbsp;<a href="/variants.php">Galaxy&nbsp;Variants</a>&nbsp;]<?php } ?>

<?php if ($_SERVER['PHP_SELF'] == "/whyng.php") { ?><span class="bold">[&nbsp;Why&nbsp;GalaxyNG&nbsp;]</span><?php } else { ?>[&nbsp;<a href="/whyng.php">Why&nbsp;GalaxyNG</a>&nbsp;]<?php } ?>

<?php if ($_SERVER['PHP_SELF'] == "/credits.php") { ?><span class="bold">[&nbsp;Credits&nbsp;and&nbsp;Licenses&nbsp;]</span><?php } else { ?>[&nbsp;<a href="/credits.php">Credits and Licenses</a>&nbsp;]<?php } ?></p>

<table class="wide" summary="This table contains the title of the page and the GalaxyNG logo">
<tr>
<td><h1><?php echo $title; ?></h1></td>
<td class="right"><?php
if ($_SERVER['PHP_SELF'] == "/index.php") { ?><img src="galaxyng.png" alt="GalaxyNG" /><?php } else { ?><a href="/"><img src="/galaxyngsmall.png" alt="GalaxyNG" /></a><?php } ?></td>
</tr>
</table>

<hr class="menu" />

<!-- end menu.php -->
