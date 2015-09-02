<?php echo '<?xml version="1.0" encoding="UTF-8"?>'; ?> 
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN" 
   "http//www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">

<html xmlns="http//www.w3.org/1999/xhtml" xmllang="en-US">

<!-- $Id: editplayer.php,v 1.1 2004/03/15 01:08:41 christhecat Exp $ -->

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
// problem
if (!$id) {
	echo "<p>Select a <a href=\"index.php\">player</a> to edit.</p>";
// new player
} else if ($id=="new") {
// check for duplicate
	$duplicates=mysql_query("SELECT id FROM player WHERE first='$first' and last='$last'");
	$n=mysql_num_rows($duplicates);
	if ($n > 0 and $sure!="yes") {
		echo "<form action=\"editplayer.php\" method=\"post\">";
		$duplicate=mysql_fetch_array($duplicates);
		echo "<p>Duplicate name <a href=\"player.php?id=$duplicate[id]\">$first $last</a> detected, are you sure you want to add a new player with the same name?";
		echo "<input type=\"hidden\" name=\"id\" value=\"$id\">";
		echo "<input type=\"hidden\" name=\"nick\" value=\"$nick\">";
		echo "<input type=\"hidden\" name=\"first\" value=\"$first\">";
		echo "<input type=\"hidden\" name=\"last\" value=\"$last\">";
		echo "<input type=\"hidden\" name=\"country\" value=\"$country\">";
		echo "<input type=\"hidden\" name=\"picture\" value=\"$picture\">";
		echo "<input type=\"hidden\" name=\"web\" value=\"$web\">";
		echo "<input type=\"hidden\" name=\"publicemail\" value=\"$publicemail\">";
		echo "<input type=\"hidden\" name=\"notes\" value=\"$notes\">";
		echo "<input type=\"hidden\" name=\"secret\" value=\"$secret\">";
		echo "<input type=\"hidden\" name=\"gm\" value=\"$gm\">";
		echo "<input type=\"hidden\" name=\"sure\" value=\"yes\">";
		echo "<br /><input type=\"submit\" value=\"Yes\"></p></form>";
	} else {
		$query="INSERT INTO player (nick, first, last, country, picture, web, publicemail, notes, secret, gm) VALUES (";
		if ($nick) { $query.="'$nick', "; } else { $query.="NULL, "; }
		if ($first) { $query.="'$first', "; } else { $query.="NULL, "; }
		if ($last) { $query.="'$last', "; } else { $query.="NULL, "; }
		if ($country) { $query.="'$country', "; } else { $query.="NULL, "; }
		if ($picture) { $query.="'$picture', "; } else { $query.="NULL, "; }
		if ($web) { $query.="'$web', "; } else { $query.="NULL, "; }
		if ($publicemail) { $query.="'$publicemail', "; } else { $query.="NULL, "; }
		if ($notes) { $query.="'$notes', "; } else { $query.="NULL, "; }
		if ($secret) { $query.="'$secret', "; } else { $query.="NULL, "; }
		$query.="'$gm')";
		$insert=mysql_query($query) or die("Problem adding " . mysql_error() . " $query");
		echo "$first $last added.";
	}
// edit player
} else {
	$query="UPDATE player SET nick=";
	if ($nick) { $query.="'$nick', "; } else { $query.="NULL, "; }	
	$query.="first=";
	if ($first) { $query.="'$first', "; } else { $query.="NULL, "; }
	$query.="last=";
	if ($last) { $query.="'$last', "; } else { $query.="NULL, "; }
	$query.="country=";
	if ($country) { $query.="'$country', "; } else { $query.="NULL, "; }
	$query.="picture=";
	if ($picture) { $query.="'$picture', "; } else { $query.="NULL, "; }
	$query.="web=";
	if ($web) { $query.="'$web', "; } else { $query.="NULL, "; }
	$query.="publicemail=";
	if ($publicemail) { $query.="'$publicemail', "; } else { $query.="NULL, "; }
	$query.="notes=";
	if ($notes) { $query.="'$notes', "; } else { $query.="NULL, "; }
	$query.="secret=";
	if ($secret) { $query.="'$secret', "; } else { $query.="NULL, "; }
	$query.="gm='$gm' WHERE id='$id'";
	$update=mysql_query($query) or die("Problem updating " . mysql_error());
	echo "$first $last updated.";
}
?>

<p><a href="index.php">Administration</a></p>

<?php
// disconnect from the database
	mysql_close($link);
?>
					
 	</body>
</html>
