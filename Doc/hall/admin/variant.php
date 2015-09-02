<?php echo '<?xml version="1.0" encoding="UTF-8"?>'; ?> 
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN" 
   "http//www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">

<html xmlns="http//www.w3.org/1999/xhtml" xmllang="en-US">

<!-- $Id: variant.php,v 1.1 2004/03/15 01:08:41 christhecat Exp $ -->

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

<form action="editvariant.php" method="post">

<?php
// new variant?
	if (!$id) {
		$variant['id']="new";
		echo "<h2>Add variant</h2>";
		$submit="Add variant";
// existing variant?
	} else {
// select the variant
		$variants = mysql_query("SELECT * FROM variant WHERE id='$id'") or die("Invalid query " . mysql_error());
// make an array
		$variant = mysql_fetch_array($variants);
		echo "<h2>Edit variant</h2>";
		$submit="Edit variant";
	}
?>

<input type="hidden" name="id" value="<?php echo $variant['id']; ?>">

<table>
	<tr><td class="right">variant ID:</td><td class="id"><?php echo $variant['id']; ?></td></tr>
	<tr><td class="right">Name:</td><td><input name="name" size="50" value="<?php echo $variant['name']; ?>"></td></tr>
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
