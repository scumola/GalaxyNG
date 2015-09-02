<?php echo '<?xml version="1.0" encoding="UTF-8"?>'; ?> 
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN" 
   "http//www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">

<html xmlns="http//www.w3.org/1999/xhtml" xmllang="en-US">

<!-- $Id: country.php,v 1.1 2004/03/15 01:08:41 christhecat Exp $ -->

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

<form action="editcountry.php" method="post">

<?php
// new country?
	if (!$id) {
		$country['id']="new";
		echo "<h2>Add Country</h2>";
		$submit="Add Country";
// existing country?
	} else {
// select the country
		$countries = mysql_query("SELECT * FROM country WHERE id='$id'") or die("Invalid query " . mysql_error());
// make an array
		$country = mysql_fetch_array($countries);
		echo "<h2>Edit Country</h2>";
		$submit="Edit Country";
	}
?>

<input type="hidden" name="id" value="<?php echo $country['id']; ?>">

<table>
	<tr><td class="right">Country ID:</td><td class="id"><?php echo $country['id']; ?></td></tr>
	<tr><td class="right">Name:</td><td><input name="name" size="50" value="<?php echo $country['name']; ?>"></td></tr>
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
