<?php echo '<?xml version="1.0" encoding="UTF-8"?>'; ?> 
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN" 
   "http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en-US">

<!-- $Id: player.php,v 1.3 2004/09/28 17:46:29 christhecat Exp $ -->

<?php
// connect to the database
$link = mysql_connect("mysql.sourceforge.net", "galaxyng", "halloffame") or die("Could not connect " . mysql_error());
// make galaxyng the current db
mysql_select_db("galaxyng", $link) or die("Can't use galaxyng  " . mysql_error());
$players = mysql_query("SELECT player.*, country.name AS countryname FROM player LEFT JOIN country ON player.country=country.id WHERE player.id='$p'") or die("Can't find player  " . mysql_error());
$player = mysql_fetch_array($players) or die("Can't array player  " . mysql_error());
?>

<?php $title="Galaxy Hall of Fame: $player[first] $player[last]"; ?>
		
	<head>
		<link rel="stylesheet" href="../main.css" />
		<title><?php echo $title; ?></title>
	</head>

	<body>
	
<?php
// page menu	
include '../menu.php';

// player info
echo "<h2>Player Information</h2>"
. "<table summary=\"Basic information about the player.\">"
. "<tr><td class=\"hallheadright\">Name</td><td class=\"hall\">";
if ($player['first'] and $player['last']) {
	echo "$player[first] $player[last]";
} else if ($player['first']) {
	echo "$player[first]";
} else if ($player['last']) {
	echo "$player[last]";
} else {
	echo "($player[nick])";
}
echo "</td></tr>"
. "<tr><td class=\"hallheadright\">Country</td><td class=\"hall\">$player[countryname]</td></tr>"
. "<tr><td class=\"hallheadright\">Web Site</td><td class=\"hall\"><a href=\"$player[web]\">$player[web]</a></td></tr>";
echo "</table>";
if ($player['picture']) {
	echo "<p><img src=\"images/$player[picture]\" alt=\"player picture\" /></p>";
}


// scores
$scores = mysql_query("SELECT score.*, game.name as gamename, game.players FROM score LEFT JOIN game ON score.game=game.id WHERE score.player=$p ORDER BY game.name") or die("Couldn\'t select scores " . mysql_error());
if (mysql_num_rows($scores)) {

echo "<hr />"
. "<h2>Scores</h2>"
. "<table summary=\"Scores for games played by this player\">"
. "<tr><td class=\"hallhead\">Game</td><td class=\"hallhead\">Win</td><td class=\"hallhead\">Race</td><td class=\"hallhead\">Pop</td><td class=\"hallhead\">Ind</td><td class=\"hallhead\">eInd</td><td class=\"hallhead\">Tech</td><td class=\"hallhead\">Planets</td><td class=\"hallhead\">Rank</td><td class=\"hallhead\">Standard<br />Victory<br />Points</td><td class=\"hallhead\">Industry<br />Victory<br />Points</td></tr>";
while ($score = mysql_fetch_array($scores)) {

echo "<tr";
	if ($score[win]=="y") { echo " class=\"hallwin\""; }
	echo "><td class=\"hall\"><a href=\"game.php?g=$score[game]\"";
	if ($score[win]=="y") { echo " class=\"hallwin\""; }
	echo ">$score[gamename]</a></td><td class=\"hallcenter\">$score[win]</td><td class=\"hall\">$score[race]</td><td class=\"hallright\">$score[pop]</td><td class=\"hallright\">$score[ind]</td><td class=\"hallright\">$score[eind]</td><td class=\"hallright\">$score[tech]</td><td class=\"hallright\">$score[planets]</td><td class=\"hallright\">";

// find rank
	$gamescores = mysql_query("select * from score where score.game='$score[game]' order by eind DESC");
	$i = 1;
	while ($gamescore = mysql_fetch_array($gamescores)) {
		if ($score['id'] == $gamescore['id']) { echo $i; $rank=$i; }
		$i++;
	}
	
	echo "</td><td class=\"hallright\">";

// if winner, find standard victory points
	if ($score['win']=="y") {
		$svp=sprintf("%01.2f",round($score['players']/$rank,2));
		echo $svp;
	} else {
// else 0 standard victory points
		echo "0.00";
	}

	echo "</td><td class=\"hallright\">";

// if winner, find industry victory points
	if ($score['win']=="y") {
		$games = mysql_query("SELECT SUM(score.eind) AS eind FROM game LEFT JOIN score ON game.id=score.game WHERE score.win='y' and game.id='$score[game]' GROUP BY game.id");
		$game = mysql_fetch_array($games);
		$ivp = sprintf("%01.2f",round(($score['players']*$score['eind'])/$game['eind'],2));
		echo $ivp;
	} else {
// else 0 industry victory points
		echo "0.00";
	}
	echo "</td></tr>";
}
echo "</table>";
}

// GM
if ($player['gm'] == "y") {
	echo "<hr /><h2>Game Master</h2>";
	echo "<table summary=\"This player was game master for these games\">";
	echo "<tr><th class=\"hallhead\">Name</th><th class=\"hallhead\">Players</th><th class=\"hallhead\">Variant</th><th class=\"hallhead\">Year</th></tr>";
	$games = mysql_query("SELECT game.*, variant.name AS variantname FROM game LEFT JOIN variant ON game.variant=variant.id WHERE gm='$player[id]' ORDER BY name");
	while ($game = mysql_fetch_array($games)) {
		echo "\n<tr><td class=\"hall\"><a href=\"game.php?g=$game[id]\">$game[name]</a></td><td class=\"hallright\">$game[players]</td><td class=\"hall\">$game[variantname]</td><td class=\"hall\"></td><td class=\"hallright\">$game[year]</td></tr>";
	}
// end game table
echo "</table>";
}

echo "<hr /><p><a href=\"/hall/\">Hall of Fame</a></p>";

// page footer
include '../footer.php';
// disconnect from the database
mysql_close($link);
?> 		

 	</body>
</html> 	