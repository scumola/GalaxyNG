<?php echo '<?xml version="1.0" encoding="UTF-8"?>'; ?> 
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN" 
   "http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en-US">

<!-- $Id: index.php,v 1.2 2004/03/23 03:40:56 christhecat Exp $ -->

	<head>
		<title>Galaxy Hall of Fame Administration</title>
		<link rel="stylesheet" href="admin.css" />
	</head>

	<body>
		<h1>Galaxy Hall of Fame Administration</h1>

<?php
// connect to the database
	$link = mysql_connect("mysql.sourceforge.net", "galaxyng", "halloffame") or die("Could not connect: " . mysql_error());
// make galaxyng the current db
	mysql_select_db("galaxyng", $link) or die("Can\'t use galaxyng : " . mysql_error());
?>

<p>Compare <a href="races.php">races and players and email addresses</a></p>

<table>
<tr><th>Games</th><th>Players</th><th>Countries</th><th>Variants</th></tr>
<tr><td class="main"><a href="game.php">New Game</a><hr />

<?php
// select the games
	$games = mysql_query("SELECT id,name FROM game ORDER BY LOWER(name)") or die("Invalid query: " . mysql_error());
// loop through the games
	while ($game = mysql_fetch_array($games)) {
		echo "<br /><a href=\"game.php?id=$game[id]\">$game[name]</a> (<a href=\"score.php?game=$game[id]\">scores</a>)";
	}
?>

</td><td class="main"><a href="player.php">New Player</a><hr />

<?php
// select the players
	$players = mysql_query("SELECT id,first,last,nick FROM player ORDER BY last, first, nick") or die("Invalid query: " . mysql_error());
// loop through the players
	while ($player = mysql_fetch_array($players)) {
		echo "<br /><a href=\"player.php?id=$player[id]\">";
		if ($player["first"] or $player["last"]) {
			echo "$player[first] $player[last]";
		} else {
			echo "($player[nick])";
		}
		echo "</a>";
	}
?>

</td><td class="main"><a href="country.php">New Country</a><hr />

<?php
// select the countries
	$countries = mysql_query("SELECT id,name FROM country ORDER BY LOWER(name)") or die("Invalid query: " . mysql_error());
// loop through the countries
	while ($country = mysql_fetch_array($countries)) {
		echo "<br /><a href=\"country.php?id=$country[id]\">$country[name]</a>";
	}
?>

</td><td class="main"><a href="variant.php">New Variant</a><hr />

<?php
// select the countries
	$variants = mysql_query("SELECT id,name FROM variant ORDER BY LOWER(name)") or die("Invalid query: " . mysql_error());
// loop through the countries
	while ($variant = mysql_fetch_array($variants)) {
		echo "<br /><a href=\"variant.php?id=$variant[id]\">$variant[name]</a>";
	}
?>

</td></tr></table>

<?php

// disconnect from the database
	mysql_close($link);
?>
					
 	</body>
</html>
