<?php echo '<?xml version="1.0" encoding="UTF-8"?>'; ?> 
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN" 
   "http//www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">

<html xmlns="http//www.w3.org/1999/xhtml" xmllang="en-US">

<!-- $Id: player.php,v 1.1 2004/03/15 01:08:41 christhecat Exp $ -->

	<head>
		<title>Galaxy Hall of Fame Administration</title>
		<link rel="stylesheet" href="admin.css" />
	</head>

	<body>
		<h1>Galaxy Hall of Fame Administration</h1>

<?php
// connect to the database
	$link = mysql_connect("mysql.sourceforge.net", "galaxyng", "halloffame") or die("Could not connect " . mysql_error());
// make galaxyng the current db
	mysql_select_db("galaxyng", $link) or die("Can\'t use galaxyng  " . mysql_error());
?>

<form action="editplayer.php" method="post">

<?php
// new player?
	if (!$id) {
		$player['id']="new";
		echo "<h2>Add Player</h2>";
		$submit="Add Player";
// existing player?
	} else {
// select the player
		$players = mysql_query("SELECT * FROM player WHERE id='$id'") or die("Invalid query " . mysql_error());
// make an array
		$player = mysql_fetch_array($players);
		echo "<h2>Edit Player</h2>";
		$submit="Edit Player";
	}
?>

<input type="hidden" name="id" value="<?php echo $player['id']; ?>">

<table>
	<tr><td class="right">Player ID:</td><td class="id"><?php echo $player['id']; ?></td></tr>
	<tr><td class="right">Nick:</td><td><input name="nick" size="50" value="<?php echo $player['nick']; ?>"></td></tr>
	<tr><td class="right">First Name:</td><td><input name="first" size="50" value="<?php echo $player['first']; ?>"></td></tr>
	<tr><td class="right">Last Name:</td><td><input name="last" size="50" value="<?php echo $player['last']; ?>"></td></tr>
	<tr><td class="right">Country:</td><td><select name="country"><option value=""></option>
<?php
// select the countries
	$countries = mysql_query("SELECT * FROM country ORDER BY name") or die("Invalid query: " . mysql_error());
// loop through the countries
	while ($country = mysql_fetch_array($countries)) {
		echo "<option";
		if ($country['id']==$player['country']) { echo " selected"; }
		echo" value=\"$country[id]\">$country[name]</option>";
	}
?> 
?>
	</select></td></tr>
	<tr><td class="right">Picture File Name:</td><td><input name="picture" size="50" value="<?php echo $player['picture']; ?>"></td></tr>
	<tr><td class="right">Web Address:</td><td><input name="web" size="50" value="<?php echo $player['web']; ?>"></td></tr>
	<tr><td class="right">Public Email:</td><td><input name="publicemail" size="50" value="<?php echo $player['publicemail']; ?>"></td></tr>
	<tr><td class="right">Notes:</td><td><input name="notes" size="50" value="<?php echo $player['notes']; ?>"></td></tr>
	<tr><td class="right">Secret Info:</td><td><input name="secret" size="50" value="<?php echo $player['secret']; ?>"></td></tr>
	<tr><td class="right">GM:</td><td><select name="gm"><option value="n">No</option><option value="y"<?php if ($player['gm']=="y") { echo " selected"; } ?>>Yes</option></select>
	</table>

<p><input type="reset"> <input type="submit" value="<?php echo $submit; ?>"></p>

</form>

<p><a href="index.php">Administration</a></p>

<?php
// disconnect from the database
	mysql_close($link);
?>
					
 	</body>
</html>
