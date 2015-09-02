<?php echo '<?xml version="1.0" encoding="UTF-8"?>'; ?> 
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN" 
   "http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en-US">

<!-- $Id: score.php,v 1.1 2004/03/15 01:08:41 christhecat Exp $ -->

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
	mysql_select_db("galaxyng", $link) or die("Can not use galaxyng : " . mysql_error());
?>

<form action="editscore.php" method="post">

<?php
// Find the game
	$games = mysql_query("SELECT * FROM game WHERE id='$game'") or die("Invalid query " . mysql_error());
// make an array
	$game = mysql_fetch_array($games);
// Game selected?	
if (!$game['id']) {
	echo "<p><a href=\"index.php\">Select a Game</a></p>";
} else {
?>

<h2>Edit Scores for <?php echo $game['name']; ?></h2>	
<input type="hidden" name="game" value="<?php echo $game['id']; ?>">

<table>
<tr><th class="score2">Player</th><th class="score2">Nick</th><th class="score2">Email</th><th class="score2">Win</th><th class="score">Race</th><th class="score">Pop</th><th class="score">Ind</th><th class="score">eInd</th><th class="score">Tech</th><th class="score">Planets</th><th class="score2">Notes</th></tr>

<?php
// get the scores for this game
	$scores = mysql_query("SELECT * FROM score WHERE game=$game[id] ORDER BY eind DESC") or die("Invalid query: " . mysql_error());
// loop through the scores
	for ($i = 1; $i <= $game['players']; $i++) {
// set the array
	$score = mysql_fetch_array($scores);
?>

<tr>
<td class="right"><input type="hidden" name="id[<?php echo $i; ?>]" value="<?php echo $score['id']; ?>"><select name="player[<?php echo $i; ?>]"><option value=""></option>
<?php
// select the players
	$players = mysql_query("SELECT id,last,first,nick FROM player ORDER BY last,first, nick") or die("Invalid query: " . mysql_error());
// loop through the players
	while ($player = mysql_fetch_array($players)) {
		echo "<option";
		if ($player['id']==$score['player']) { echo " selected"; }
		echo" value=\"$player[id]\">";
		if ($player['first'] and $player['last']) {
			echo "$player[first] $player[last]";
		} else if ($player['first']) {
			echo "$player[first]";
		} else if ($player['last']) {
			echo "$player[last]";
		} else {
			echo "($player[nick])";
		}
		echo "</option>";
	}
?>
</select></td>
<td class="right"><select name="nick[<?php echo $i; ?>]"><option value=""></option>
<?php
// select the players
	$players = mysql_query("SELECT id,nick FROM player ORDER BY nick") or die("Invalid query: " . mysql_error());
// loop through the players
	while ($player = mysql_fetch_array($players)) {
		echo "<option";
		if ($player['id']==$score['player']) { echo " selected"; }
		echo" value=\"$player[id]\">$player[nick]</option>";
	}
?>
</select></td>
<td class="right"><input type="text" size="10" name="email[<?php echo $i; ?>]" value="<?php echo $score['email']; ?>"></td>
<td><input type="radio" name="win[<?php echo $i; ?>]" value="y"<?php if ($score['win']=="y") { echo " checked"; } ?>>y <input type="radio" name="win[<?php echo $i; ?>]" value="n"<?php if ($score['win']!="y") { echo " checked"; } ?>>n</td>
<td class="right"><input class="right" type="text" size="15" name="race[<?php echo $i; ?>]" value="<?php if ($score['race']) { echo $score['race']; } else { echo ""; } ?>"></td>
<td class="right"><input class="right" type="text" size="6" name="pop[<?php echo $i; ?>]" value="<?php echo $score['pop']; ?>"></td>
<td class="right"><input class="right" type="text" size="6" name="ind[<?php echo $i; ?>]" value="<?php echo $score['ind']; ?>"></td>
<td class="right"><input class="right" type="text" size="6" name="eind[<?php echo $i; ?>]" value="<?php echo $score['eind']; ?>"></td>
<td class="right"><input class="right" type="text" size="6" name="tech[<?php echo $i; ?>]" value="<?php echo $score['tech']; ?>"></td>
<td class="right"><input class="right" type="text" size="6" name="planets[<?php echo $i; ?>]" value="<?php echo $score['planets']; ?>"></td>
<td class="right"><input class="right" type="text" size="10" name="notes[<?php echo $i; ?>]" value="<?php echo $score['notes']; ?>"></td>
</tr>

<?php } ?>

</table>

<p><input type="reset" value="Reset"> <input type="submit" value="Save"></p>

</form>

<?php
}
?>

<p><a href="index.php">Administration</a></p>

<?php
// disconnect from the database	mysql_close($link);
?>
					
 	</body>
</html>
