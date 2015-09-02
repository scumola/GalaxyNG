<?php echo '<?xml version="1.0" encoding="UTF-8"?>'; ?> 
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN" 
   "http//www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">

<html xmlns="http//www.w3.org/1999/xhtml" xmllang="en-US">

<!-- $Id: editgame.php,v 1.1 2004/03/15 01:08:41 christhecat Exp $ -->

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

// problem
if (!$id) {
	echo "<p>Select a <a href=\"index.php\">game</a> to edit.</p>";

// new game
} else if ($id=="new") {
	$query="INSERT INTO game (variant, gm, players, turns, year, web, notes, name) VALUES (";
	if ($variant) { $query.="'$variant', "; } else { $query.="NULL, "; }
	if ($gm) { $query.="'$gm', "; } else { $query.="NULL, "; }
	if ($players) { $query.="'$players', "; } else { $query.="NULL, "; }
	if ($turns) { $query.="'$turns', "; } else { $query.="NULL, "; }
	if ($year) { $query.="'$year', "; } else { $query.="NULL, "; }
	if ($web) { $query.="'$web', "; } else { $query.="NULL, "; }
	if ($notes) { $query.="'$notes', "; } else { $query.="NULL, "; }
	$query.="'$name')";
	$insert=mysql_query($query) or die("Problem adding game " . mysql_error());
	$id = mysql_insert_id();
	echo "<p><a href=\"game.php?id=$id\">$name</a> added.<br /><a href=\"score.php?game=$id\">Edit $name scores</a></p>";

// edit game
} else {
	$query="UPDATE game SET variant=";
	if ($variant) { $query.="'$variant', "; } else { $query.="NULL, "; }
	$query.="gm=";
	if ($gm) { $query.="'$gm', "; } else { $query.="NULL, "; }
	$query.="players=";
	if ($players) { $query.="'$players', "; } else { $query.="NULL, "; }
	$query.="turns=";
	if ($turns) { $query.="'$turns', "; } else { $query.="NULL, "; }
	$query.="year=";
	if ($year) { $query.="'$year', "; } else { $query.="NULL, "; }
	$query.="web=";
	if ($web) { $query.="'$web', "; } else { $query.="NULL, "; }
	$query.="notes=";
	if ($notes) { $query.="'$notes', "; } else { $query.="NULL, "; }
	$query.="name='$name' WHERE id='$id'";
	$update=mysql_query($query) or die("Problem updating game " . mysql_error());
	echo "<p><a href=\"game.php?id=$id\">$name</a> updated.<br /><a href=\"score.php?game=$id\">Edit $name scores</a></p>";
}

// disconnect from the database
	mysql_close($link);
?>

		<p><a href="index.php">Administration</a></p>
					
 	</body>
</html>
 	