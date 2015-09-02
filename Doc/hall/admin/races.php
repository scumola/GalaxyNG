<?php echo '<?xml version="1.0" encoding="UTF-8"?>'; ?> 
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN" 
   "http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en-US">

<!-- $Id: races.php,v 1.3 2004/10/02 17:24:08 christhecat Exp $ -->

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

// sort order
if ($s=="p") { $order="player.last, player.first"; }
else if ($s=="r") { $order="score.race"; }
else if ($s=="n") { $order="score.nick"; }
else if ($s=="g") { $order="game.name"; }
else { $order="score.email"; }

// list races
$races = mysql_query("SELECT score.*, player.first, player.last, player.nick, game.name as gamename FROM score LEFT JOIN player ON score.player=player.id LEFT JOIN game ON score.game=game.id ORDER BY $order") or die("Couldn't select races " . mysql_error());
?>
<table>
<tr><th><a href="?s=r">Race</a></th><th><a href="?s=p">Player</a></th><th><a href="?s=e">Email</a></th><th><a href="?s=n">Nickname</a></th><th><a href="?s=g">Game</a></th></tr>
<?php
while ($race = mysql_fetch_array($races)) {
	$nation = substr($race[race], 0, 7);
	if ($race[race]) {
		echo "<tr><td>$race[race]</td><td><a href=\"player.php?id=$race[player]\">$race[first] $race[last]</a></td><td>$race[email]</td><td>$race[nick]</td><td><a href=\"score.php?game=$race[game]\">$race[gamename]</a></td></tr>\n";
	}
}
echo "</table>";
?>

<p><a href="index.php">Administration</a></p>

<?php
// disconnect from the database	mysql_close($link);
?>
					
 	</body>
</html> 	