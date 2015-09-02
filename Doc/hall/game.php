<?php echo '<?xml version="1.0" encoding="UTF-8"?>'; ?> 
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN" 
   "http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en-US">

<!-- $Id: game.php,v 1.4 2004/09/28 17:46:28 christhecat Exp $ -->

<?php
// connect to the database
$link = mysql_connect("mysql.sourceforge.net", "galaxyng", "halloffame") or die("Could not connect " . mysql_error());
// make galaxyng the current db
mysql_select_db("galaxyng", $link) or die("Can\'t use galaxyng  " . mysql_error());
// select the game
$games = mysql_query("SELECT game.*, variant.name AS variantname, player.first, player.last, SUM(score.eind) as eind FROM game LEFT JOIN variant ON (game.variant=variant.id) LEFT JOIN player ON (game.gm=player.id) LEFT JOIN score ON game.id=score.game WHERE game.id=$g and score.win='y' GROUP BY game.id") or die("Can\'t select game " . mysql_error());
// if no game found, try again without any winners
if (mysql_num_rows($games)==0) {
	$games = mysql_query("SELECT game.*, variant.name AS variantname, player.first, player.last, SUM(score.eind) as eind FROM game LEFT JOIN variant ON (game.variant=variant.id) LEFT JOIN player ON (game.gm=player.id) LEFT JOIN score ON game.id=score.game WHERE game.id=$g GROUP BY game.id") or die("Can\'t select game " . mysql_error()); }
// if still no game found, give up
if (mysql_num_rows($games)==0) {
	echo"<head><link rel=\"stylesheet\" href=\"../main.css\" /><title>No scores found for this game</title></head><body>";include '../menu.php';echo"<h1>No scores found for this game.</h1><p><a href=\"/hall/\">Return to the Hall of Fame</a></p>";
	
} else {

$game = mysql_fetch_array($games) or die("Can\'t make array of game " . mysql_error());
$title="Galaxy Hall of Fame: $game[name]";
?>
		
	<head>
		<link rel="stylesheet" href="../main.css" />
		<title><?php echo $title; ?></title>
	</head>

	<body>
	
<?php
// page menu	
include '../menu.php';

// game info
echo "<h2>Game Information</h2>" 
. "<table summary=\"Information about the game\">"
. "<tr><td class=\"hallright\">Name</td><td class=\"hall\">$game[name]</td></tr>"
. "<tr><td class=\"hallright\">Variant</td><td class=\"hall\">$game[variantname]</td></tr>"
. "<tr><td class=\"hallright\">GM</td><td class=\"hall\"><a href=\"player.php?p=$game[gm]\">$game[first] $game[last]</a></td></tr>"
. "<tr><td class=\"hallright\">Players</td><td class=\"hall\">$game[players]</td></tr>"
. "<tr><td class=\"hallright\">Turns</td><td class=\"hall\">$game[turns]</td></tr>"
. "<tr><td class=\"hallright\">Year</td><td class=\"hall\">$game[year]</td></tr>"
. "<tr><td class=\"hallright\">Web&nbsp;Site</td><td class=\"hall\">";
if ($game['web']) { echo "<a href=\"$game[web]\">$game[web]</a>"; }
echo "</td></tr>"
. "<tr><td class=\"halltopright\">Notes</td><td class=\"hall\">$game[notes]</td></tr>"
. "</table>";

// scores
echo "<hr />"
. "<h2>Scores</h2>"
. "<table summary=\"Scores in the game\">"
. "<tr><th></th><th class=\"hallhead\">Player</th><th class=\"hallhead\">Win</th><th class=\"hallhead\">Race</th><th class=\"hallhead\">Pop</th><th class=\"hallhead\">Ind</th><th class=\"hallhead\">eInd</th><th class=\"hallhead\">Tech</th><th class=\"hallhead\">Planets</th><th class=\"hallhead\">Standard<br />Victory<br />Points</th><th class=\"hallhead\">Industry<br />Victory<br />Points</th></tr>";
$scores = mysql_query("SELECT score.*, player.first, player.last, player.nick FROM score LEFT JOIN player ON score.player=player.id WHERE score.game=$g ORDER BY score.eind DESC, score.tech DESC, score.race ASC") or die("Couldn\'t select scores " . mysql_error());
$i=1;
while ($score = mysql_fetch_array($scores)) {
	echo "<tr";
	if ($score[win]=="y") { echo " class=\"hallwin\""; }
	echo "><td>";
	if ($tie != $score['eind']) { echo $i; }
	$tie = $score['eind'];
	echo "</td><td class=\"hall\"><a href=\"player.php?p=$score[player]\"";
	if ($score[win]=="y") { echo " class=\"hallwin\""; }
	echo ">";	
	if ($score['first'] and $score['last']) {
		echo "$score[first] $score[last]";
	} else if ($score['first']) {
		echo "$score[first]";
	} else if ($score['last']) {
		echo "$score[last]";
	} else if ($score['nick']) {
		echo "($score[nick])";
	}
	echo "</a></td><td class=\"hallcenter\">$score[win]</td><td class=\"hall\">$score[race]</td><td class=\"hallright\">$score[pop]</td><td class=\"hallright\">$score[ind]</td><td class=\"hallright\">$score[eind]</td><td class=\"hallright\">$score[tech]</td><td class=\"hallright\">$score[planets]</td><td class=\"hallright\">";
	if ($score[win]=="y") {
		$standard = round($game['players']/$i,2);
		echo sprintf("%01.2f", $standard);
	} else {
		echo "0.00";
	}
	echo "</td><td class=\"hallright\">";
	if ($score[win]=="y") {
		$industry = round(($game['players']*$score['eind'])/$game['eind'],2);
		echo sprintf("%01.2f", $industry);
	} else {
		echo "0.00";
	}
	echo "</td></tr>";
	$i++;
}
echo "</table>";

} //close no scores for this game

echo "<hr /><p><a href=\"/hall/\">Hall of Fame</a></p>";

// page footer
include '../footer.php';
// disconnect from the database
mysql_close($link);
?> 		

 	</body>
</html>