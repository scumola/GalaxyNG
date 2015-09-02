<?php

include "env.php";

if ($type and $orders) {  

	header('Content-type: text/plain');

	$tmpfname = tempnam("/tmp", "galaxyng");
	
	$fp = fopen($tmpfname, "w"); 
	
	$orders = explode("\n", $_POST["orders"]);
	
	$line1 = explode(" ", $orders[0]);

	for ($i = 0; $i < count($orders); $i++) {
	  $line = rtrim($orders[$i]);
	  fwrite($fp, $line); 
	  fwrite($fp, "\n");
	}
	
	fclose($fp);
	
	switch ($type) {
		case "cmd_orders":
			$filename=$line1[2] . "_forecast_" . $line1[4] . ".txt";
			header("Content-disposition: attachment; filename=\"$filename\"");
			passthru("$NGpath/galaxyng -webcheck < $tmpfname");
		break;
		case "cmd_report":
			$filename=$line1[2] . "_report_" . $line1[4] . ".txt";
			header("Content-disposition: attachment; filename=\"$filename\"");
			passthru("$NGpath/galaxyng -webreport < $tmpfname");
		break;
	}
	
	$command = "rm -f " . $tmpfname;
	system($command);
	exit;
	
} else { ?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
        "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">


	<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en-US">
	<?php include "head.php"; ?>
	<form action="<?php echo $PHP_SELF; ?>" method="post">
	<p><input type="radio" name="type" value="cmd_orders" checked>Orders <input type="radio" name="type" value="cmd_report"> Report
	<br/><textarea name="orders" rows="10" cols="80"></textarea>
	<br/><input type="submit" value="Submit"> <input type="reset"></p>
	</form>

<?php }

include "foot.php";

?>

</body>
</html>
