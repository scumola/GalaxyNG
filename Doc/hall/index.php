<?php echo '<?xml version="1.0" encoding="UTF-8"?>'; ?> 
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN" 
   "http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en-US">

<!-- $Id: index.php,v 1.4 2004/09/28 14:34:26 christhecat Exp $ -->

<?php $title="GalaxyNG Hall of Fame"; ?>
		
	<head>
		<link rel="stylesheet" href="../main.css" />
		<title>GalaxyNG Hall of Fame</title>
	</head>

	<body>

<?php
// connect to the database
$link = mysql_connect("mysql.sourceforge.net", "galaxyng", "halloffame") or die("Could not connect " . mysql_error());
// make galaxyng the current db
mysql_select_db("galaxyng", $link) or die("Can\'t use galaxyng  " . mysql_error());
// page menu	
include '../menu.php';

// table of contents

echo "<ul>";
echo "<li><a href=\"";
	if ($p) { echo "?p=$p"; }
	if ($p and $g) { echo "&amp;"; }
	if ($g) { echo "g=$g"; }
	echo "#i\">Introduction</a></li>";
echo "<li><a href=\"";
	if ($p) { echo "?p=$p"; }
	if ($p and $g) { echo "&amp;"; }
	if ($g) { echo "g=$g"; }
	echo "#p\">Players</a></li>";
echo "<li><a href=\"";
	if ($p) { echo "?p=$p"; }
	if ($p and $g) { echo "&amp;"; }
	if ($g) { echo "g=$g"; }
	echo "#g\">Games</a></li>";
echo "</ul>";
?>

<hr id="i" />

<h2>Introduction</h2>

<p>Players are awarded victory points for Galaxy and GalaxyNG games based on the number of players in each game and their effective industry at the end of the game.  Only active members of the winning alliance receive victory points.  If the winning alliance is unknown, all active surviving players receive victory points.</p>

<p>Standard victory points are calculated as number-of-players/finish-position.  For example, in a game with 20 players, first place receives 20/1=20 points, second place receives 20/2=10 points, third place receives 20/3=6.66 points, and so forth.  Standard victory points awarded for a game increase if more players are in the winning alliance.  For example, a 40 player game with one winner awards 40 points to the victor.  The same game won by a five-player alliance awards 40 points to first place, 20 points to second place, 13.33 points to third place, 10 points to fourth place, and 8 points to fifth place for a total of 91.33 points.</p>

<p>Industry victory points are calculated as (number-of-players*effective-industry)/total-effective-industry where total effective industry is the sum of the effective industry of winning players.  For example, in a game with 20 players, three winning players with 50,000, 30,000 and 2,000 effective industry would receive 20*50,000/82,000=12.19 points, 20*30,000/82,000=7.32 points, and 20*2,000/82,000=0.49 points, respectively. Industry victory points awarded for a game always equal the number of players in the game.  For example, a 40 player game with one winner awards 40 points to the victor.  A five-player alliance winning the same game share 40 points based on each player's percentage of the total effective industry.</p>

<p>The GalaxyNG Hall of Fame includes Galaxy and GalaxyNG games.  <a href="http://sourceforge.net/mail/?group_id=48224">Please let us know</a> if you have any additional information about the games in the Hall of Fame or if you would like to add a game.  You might also want to visit the <a href="http://www.cs.utk.edu/~bampton/blind/fame.html">Blind Galaxy Hall of Fame</a>, the <a href="http://racelist.uplanet.ru/">GalaxyPlus Race List</a>, the <a href="http://bannikov.omegaplus.ru/racelist/">Orioner RaceList</a>, the <a href="http://explorer.sourceforge.net/hall.html">Galaxy Explorer Hall of Fame</a> and the <a href="http://galaxy.fzu.cz/galaxywww/HallOfFame">Galaxy PBW Hall of Fame</a>.</p>

<hr id="p" />

<?php
// create temporary table playerinfo
mysql_query("CREATE TEMPORARY TABLE playerinfo (player INT(10), standard FLOAT(10,2), industry FLOAT(10,2), firsts INT(10), wins INT(10), games INT(10), sort INT(1))") or die("Could not create table " . mysql_error());

// create the records in playerinfo, count games played and indicate if player has real name
$players = mysql_query("SELECT score.player, COUNT(score.game) AS games, player.last, player.first FROM score LEFT JOIN player ON score.player=player.id GROUP BY score.player");
while ($player = mysql_fetch_array($players)) {
	if ($player['last']) { $sort=0; } else if ($player['first']) { $sort=1; } else { $sort=2; }
	mysql_query("INSERT INTO playerinfo (player,games,firsts,wins,sort) VALUES ('$player[player]','$player[games]', '0', '0', '$sort')") or die("Could not add player " . mysql_error());
}

// select games (to calculate standard victory points, industry victory points and first place finishes)
$games = mysql_query("SELECT game.id, game.players, SUM(score.eind) AS eind FROM game LEFT JOIN score ON game.id=score.game WHERE score.win='y' GROUP BY game.id");
while ($game = mysql_fetch_array($games)) {
// select scores for each game
	$scores = mysql_query("SELECT player,eind FROM score WHERE game='$game[id]' and win='y' ORDER BY eind DESC");
	$i = 1;
	while ($score = mysql_fetch_array($scores)) {
// select the player from playerinfo if player is not null
		if ($score[player]) {
			$playerinfos = mysql_query("SELECT * FROM playerinfo WHERE player = $score[player]") or die("couldn't select player from playerinfo " . mysql_error());
			$playerinfo = mysql_fetch_array($playerinfos);		
// add win
			$wins = $playerinfo['wins'] + 1;
			mysql_query("UPDATE playerinfo SET wins='$wins' WHERE player='$score[player]'") or die("Could not set wins " . mysql_error());		
// add standard victory points
			$standard = $playerinfo['standard'] + ($game['players']/$i);
			mysql_query("UPDATE playerinfo SET standard='$standard' WHERE player='$score[player]'") or die("Could not update standard victory points " . mysql_error()); 
// add industry victory points
			$industry = $playerinfo['industry'] + (($game['players']*$score['eind'])/$game['eind']);
			mysql_query("UPDATE playerinfo SET industry='$industry' WHERE player='$score[player]'") or die("Could not update industry victory points " . mysql_error());
// add first place finish
			if ($i == 1) {
				$firsts = $playerinfo['firsts'] + 1;
				mysql_query("UPDATE playerinfo SET firsts='$firsts' WHERE player='$score[player]'") or die("Could not set firsts " . mysql_error());
			}
		}
		$i++;
	}
}

// display header and set sort order for player query
if ($p=="n") {
	echo "<h2>Players Sorted by Name</h2>";
	$order = "ORDER BY playerinfo.sort ASC, player.last ASC, player.first ASC, player.nick ASC";
} else if ($p=="" or $p=="s") {
	echo "<h2>Players Sorted by Standard Victory Points</h2>";
	$order = "ORDER BY playerinfo.standard DESC, player.last ASC, player.first ASC";
} else if ($p=="i") {
	echo "<h2>Players Sorted by Industry Victory Points</h2>";
	$order = "ORDER BY playerinfo.industry DESC, player.last ASC, player.first ASC";
} else if ($p=="f") {
	echo "<h2>Players Sorted by First Place Finishes</h2>";
	$order = "ORDER BY playerinfo.firsts DESC, player.last ASC, player.first ASC";
} else if ($p=="w") {
	echo "<h2>Players Sorted by Games Won</h2>";
	$order = "ORDER BY playerinfo.wins DESC, player.last ASC, player.first ASC";
} else if ($p=="g") {
	echo "<h2>Players Sorted by Games Played</h2>";
	$order = "ORDER BY playerinfo.games DESC, player.last ASC, player.first ASC";
}

// begin player table
echo "<table summary=\"List of players in the hall of fame\">";
echo "<tr><th></th><th class=\"hallhead\">"; if ($p=="n") { echo "Player<br />Name"; } else { echo "<a href=\"?p=n";
if ($g) { echo "&amp;g=$g"; }
echo "#p\">Player<br />Name</a>"; } echo "<br /></th>";
echo "<th class=\"hallhead\">"; if ($p=="" or $p=="s") { echo "Standard<br />Victory<br />Points"; } else { echo "<a href=\"?p=s";
if ($g) { echo "&amp;g=$g"; }
echo "#p\">Standard<br />Victory<br />Points</a>"; } echo "</th>";
echo "<th class=\"hallhead\">"; if ($p=="i") { echo "Industry<br />Victory<br />Points"; } else { echo "<a href=\"?p=i";
if ($g) { echo "&amp;g=$g"; }
echo "#p\">Industry<br />Victory<br />Points</a>"; } echo "</th>";
echo "<th class=\"hallhead\">"; if ($p=="f") { echo "First<br />Place<br />Finishes"; } else { echo "<a href=\"?p=f";
if ($g) { echo "&amp;g=$g"; }
echo "#p\">First<br />Place<br />Finishes</a>"; } echo "</th>";
echo "<th class=\"hallhead\">"; if ($p=="w") { echo "Games<br />Won"; } else { echo "<a href=\"?p=w";
if ($g) { echo "&amp;g=$g"; }
echo "#p\">Games<br />Won</a>"; } echo "</th>";
echo "<th class=\"hallhead\">"; if ($p=="g") { echo "Games<br />Played"; } else { echo "<a href=\"?p=g";
if ($g) { echo "&amp;g=$g"; }
echo "#p\">Games<br />Played</a>"; } echo "</th></tr>";
echo "<tr><td colspan=\"7\"><hr /></td></tr>";

// select the players
	$query = "SELECT playerinfo.*, player.first, player.last, player.nick, player.id FROM playerinfo LEFT JOIN player ON playerinfo.player=player.id WHERE playerinfo.player != '0' $order;";
	$players = mysql_query($query) or die("Could not sort players by $order " . mysql_error());
// display each player
	$i=1; $n=1;
	while ($player=mysql_fetch_array($players)) {
// set nulls to zero
		if (!$player['firsts']) { $player['firsts']=0; }
		if (!$player['standard']) { $player['standard']=0; }
		if (!$player['industry']) { $player['industry']=0; }		
// decide if this is a tie with the previous player
		if ($p=="" or $p=="s") {
			if ($tie==$player['standard']) { $tied="y"; } else { $tied="n"; $n=$i; }
			$tie=$player['standard'];
		} else if ($p=="i") {
			if ($tie==$player['industry']) { $tied="y"; } else { $tied="n"; $n=$i; }
			$tie=$player['industry'];		
		} else if ($p=="f") {
			if ($tie==$player['firsts']) { $tied="y"; } else { $tied="n"; $n=$i; }
			$tie=$player['firsts'];			
		} else if ($p=="w") {
			if ($tie==$player['wins']) { $tied="y"; } else { $tied="n"; $n=$i; }
			$tie=$player['wins'];			
		} else if ($p=="g") {
			if ($tie==$player['games']) { $tied="y"; } else { $tied="n"; $n=$i; }
			$tie=$player['games'];			
		}
				echo "\n<tr";
// if not sorted by name, highlight the first place player(s)
				if ($n==1 and $p!="n") { echo " class=\"hallwin\""; }
				echo "><td class=\"hallright\">";
// don't display number if tied with previous player
				if ($p!="n" and $tied=="n") { echo $n; }
				echo "</td><td class=\"hall\"><a href=\"player.php?p=$player[id]\"";
				if ($n==1 and $p!="n") { echo " class=\"hallwin\""; }
				echo ">";
				if ($player['first'] and $player['last'] and $p=="n") {
					echo "$player[last], $player[first]";
				} else if ($player['first'] and $player['last']) {
					echo "$player[first] $player[last]";
				} else if ($player['first']) {
					echo "$player[first]";
				} else if ($player['last']) {
					echo "$player[last]";
				} else {
					echo "($player[nick])";
				}
				echo "</a></td><td class=\"hallright\">$player[standard]</td><td class=\"hallright\">$player[industry]</td><td class=\"hallright\">$player[firsts]</td><td class=\"hallright\">$player[wins]</td><td class=\"hallright\">$player[games]</td></tr>";
		$i++;
	}

// end player table
echo "</table>";
?>

<hr id="g" />

<?php
// display header and set sort order for games query
if ($g=="" or $g=="n") {
	echo "<h2>Games Sorted by Name</h2>";
	$order = "ORDER BY game.name, game.year";
} else if ($g=="p") {
	echo "<h2>Games Sorted by Number of Players</h2>";
	$order = "ORDER BY game.players DESC, game.name, game.year";
} else if ($g=="v") {
	echo "<h2>Games Sorted by Variant</h2>";
	$order = "ORDER BY game.variant, game.name DESC";
} else if ($g=="g") {
	echo "<h2>Games Sorted by Game Master</h2>";
	$order = "ORDER BY player.last, player.first, game.name, game.year";
} else if ($g=="y") {
	echo "<h2>Games Sorted by Year</h2>";
	$order = "ORDER BY game.year DESC, game.name";
}

// begin game table
echo "<table summary=\"List of games in the hall of fame\">";
echo "\n<tr><th class=\"hallhead\">"; if ($g=="" or $g=="n") { echo "Game<br />Name"; } else { echo "<a href=\"?";
if ($p) { echo "p=$p&amp;"; }
echo "g=n#g\">Game<br />Name</a>"; } echo "&nbsp;</th>";
echo "<th class=\"hallhead\">"; if ($g=="p") { echo "Number<br />of<br />Players"; } else { echo "<a href=\"?";
if ($p) { echo "p=$p&amp;"; }
echo "g=p#g\">Number<br />of<br />Players</a>"; } echo "</th>";
echo "<th class=\"hallhead\">"; if ($g=="v") { echo "Variant"; } else { echo "<a href=\"?";
if ($p) { echo "p=$p&amp;"; }
echo "g=v#g\">Variant</a>"; } echo "</th>";
echo "<th class=\"hallhead\">"; if ($g=="g") { echo "Game&nbsp;Master"; } else { echo "<a href=\"?";
if ($p) { echo "p=$p&amp;"; }
echo "g=g#g\">Game&nbsp;Master</a>"; } echo "</th>";
echo "<th class=\"hallhead\">"; if ($g=="y") { echo "Year"; } else { echo "<a href=\"?";
if ($p) { echo "p=$p&amp;"; }
echo "g=y#g\">Year</a>"; } echo "</th></tr>";
echo "<tr><td colspan=\"5\"><hr /></td></tr>";

// someone should figure out how to make this sort properly - currently SmallGame21 appears before SmallGame3

// select games
$games = mysql_query("SELECT game.*, player.first, player.last, player.id as gmid, variant.name as variantname FROM game LEFT JOIN variant ON (game.variant=variant.id) LEFT JOIN player ON (game.gm=player.id) $order");
// display each game
while ($game = mysql_fetch_array($games)) {
	echo "\n<tr><td class=\"hall\"><a href=\"game.php?g=$game[id]\">$game[name]</a></td><td class=\"hallright\">$game[players]</td><td class=\"hall\">$game[variantname]</td><td class=\"hall\"><a href=\"player.php?p=$game[gmid]\">$game[first] $game[last]</a></td><td class=\"hallright\">$game[year]</td></tr>";
}
// end game table
echo "</table>";
// page footer
include '../footer.php';
// disconnect from the database
mysql_close($link);
?> 		
<table></table>
 	</body>
</html>