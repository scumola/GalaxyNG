<?php echo '<?xml version="1.0" encoding="UTF-8"?>'; ?> 
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN" 
   "http//www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">

<html xmlns="http//www.w3.org/1999/xhtml" xmllang="en-US">

<!-- $Id: editscore.php,v 1.1 2004/03/15 01:08:41 christhecat Exp $ -->

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

<?php
// Find the game
	$games = mysql_query("SELECT * FROM game WHERE id='$game'") or die("Invalid query " . mysql_error());
// make an array
	$game = mysql_fetch_array($games);
// Game selected?	
if (!$game['id']) {
	echo "<p><a href=\"index.php\">Select a Game</a></p>";
} else {
	echo "<h2>Updating scores for $game[name]</h2>";

// loop through the scores
	for ($i = 1; $i <= $game['players']; $i++) {	
// new score
		if (!$id[$i]) {
			$query="INSERT INTO score (player, email, race, pop, ind, eind, tech, planets, notes, win, game) VALUES (";
			if ($player[$i]) { $query.="'$player[$i]', "; } else if ($nick[$i]) { $query.="'$nick[$i]', "; } else { $query.="NULL, "; }
			if ($email[$i]) { $query.="'$email[$i]', "; } else { $query.="NULL, "; }
			if ($race[$i]) { $query.="'$race[$i]', "; } else { $query.="NULL, "; }
			if ($pop[$i] or $pop[$i]=="0") { $query.="'$pop[$i]', "; } else { $query.="NULL, "; }
			if ($ind[$i] or $ind[$i]=="0") { $query.="'$ind[$i]', "; } else { $query.="NULL, "; }
			if ($eind[$i] or $eind[$i]=="0") { $query.="'$eind[$i]', "; } else { $query.="NULL, "; }
			if ($tech[$i] or $tech[$i]=="0") { $query.="'$tech[$i]', "; } else { $query.="NULL, "; }
			if ($planets[$i] or $planets[$i]=="0") { $query.="'$planets[$i]', "; } else { $query.="NULL, "; }
			if ($notes[$i]) { $query.="'$notes[$i]', "; } else { $query.="NULL, "; }
			$query.="'$win[$i]', '$game[id]')";
			$insert=mysql_query($query) or die("Problem updating " . mysql_error());
			if ($race[$i]) {
				echo "<p>Score for $race[$i] added.</p>";
			} else {
				echo "<p>Score for unknown race added";
			}

// edit existing score
		} else {
			$query="UPDATE score SET player=";
			if ($player[$i]) { $query.="'$player[$i]', "; } else if ($nick[$i]) { $query.="'$nick[$i]', "; } else { $query.="NULL, "; }
			$query.="email=";
			if ($email[$i]) { $query.="'$email[$i]', "; } else { $query.="NULL, "; }
			$query.="race=";
			if ($race[$i]) { $query.="'$race[$i]', "; } else { $query.="NULL, "; }
			$query.="pop=";
			if ($pop[$i] or $pop[$i]=="0") { $query.="'$pop[$i]', "; } else { $query.="NULL, "; }
			$query.="ind=";
			if ($ind[$i] or $ind[$i]=="0") { $query.="'$ind[$i]', "; } else { $query.="NULL, "; }
			$query.="eind=";
			if ($eind[$i] or $eind[$i]=="0") { $query.="'$eind[$i]', "; } else { $query.="NULL, "; }
			$query.="tech=";
			if ($tech[$i] or $tech[$i]=="0") { $query.="'$tech[$i]', "; } else { $query.="NULL, "; }
			$query.="planets=";
			if ($planets[$i] or $planets[$i]=="0") { $query.="'$planets[$i]', "; } else { $query.="NULL, "; }
			$query.="notes=";
			if ($notes[$i]) { $query.="'$notes[$i]', "; } else { $query.="NULL, "; }
			$query.="win='$win[$i]', game='$game[id]' WHERE id='$id[$i]'";
			$update=mysql_query($query) or die("Problem updating " . mysql_error());
			if ($race[$i]) {
				echo "<p>Score for $race[$i] updated.</p>";
			} else {
				echo "<p>Score for unknown race updated";
			}
		}
	}
}
?>

<p><a href="index.php">Administration</a></p>

<?php
// disconnect from the database
	mysql_close($link);
?>
					
 	</body>
</html>
 	