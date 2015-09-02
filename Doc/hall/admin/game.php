<?php echo '<?xml version="1.0" encoding="UTF-8"?>'; ?> 
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN" 
   "http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en-US">

<!-- $Id: game.php,v 1.1 2004/03/15 01:08:41 christhecat Exp $ -->

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

<form action="editgame.php" method="post">

<?php
// new game?
	if (!$id) {
		$game['id']="new";
		echo "<h2>Add Game</h2>";
		$submit="Add Game";
// existing game?
	} else {
// select the game
		$games = mysql_query("SELECT * FROM game WHERE id='$id'") or die("Invalid query " . mysql_error());
// make an array
		$game = mysql_fetch_array($games);
		echo "<h2>Edit Game</h2>";
		$submit="Edit Game";
	}
?>

<input type="hidden" name="id" value="<?php echo $game['id']; ?>">

<table>
	<tr><td class="right">Game ID:</td><td class="id"><?php echo $game['id']; ?></td></tr>
	<tr><td class="right">Name:</td><td><input name="name" size="50" value="<?php echo $game['name']; ?>"></td></tr>
	<tr><td class="right">Variant:</td><td><select name="variant"><option value=""></option>
<?php
// select the variants
	$variants = mysql_query("SELECT * FROM variant ORDER BY name") or die("Invalid query: " . mysql_error());
// loop through the variants
	while ($variant = mysql_fetch_array($variants)) {
		echo "<option";
		if ($variant['id']==$game['variant']) { echo " selected"; }
		echo" value=\"$variant[id]\">$variant[name]</option>";
	}
?> 
?>
	</select></td></tr>
	<tr><td class="right">Game Master:</td><td><select name="gm"><option value=""></option>
<?php
// select the types
	$gms = mysql_query("SELECT * FROM player WHERE gm='y' ORDER BY last,first") or die("Invalid query: " . mysql_error());
// loop through the types
	while ($gm = mysql_fetch_array($gms)) {
		echo "<option";
		if ($gm['id']==$game['gm']) { echo " selected"; }
		echo" value=\"$gm[id]\">$gm[first] $gm[last]</option>";
	}
?>
	</select></td></tr>
	<tr><td class="right">Number of Players:</td><td><input name="players" size="50" value="<?php echo $game['players']; ?>"></td></tr>
	<tr><td class="right">Number of Turns:</td><td><input name="turns" size="50" value="<?php echo $game['turns']; ?>"></td></tr>
	<tr><td class="right">Web Address:</td><td><input name="web" size="50" value="<?php echo $game['web']; ?>"></td></tr>
	<tr><td class="right">Year:</td><td><input name="year" size="4" value="<?php echo $game['year']; ?>"></td></tr>
	<tr><td class="right">Notes:</td><td><textarea name="notes" rows="5" cols="60"><?php echo $game['notes']; ?></textarea></td></tr>
</table>

<p><input type="reset"> <input type="submit" value="<?php echo $submit; ?>"></p>

</form>

<?php
if ($game["id"] != "new") {
	echo "<p><a href=\"score.php?game=$game[id]\">Edit $game[name] Scores</a></p>";
}
?>

<p><a href="index.php">Administration</a></p>

<?php
// disconnect from the database	mysql_close($link);
?>
					
 	</body>
</html>
