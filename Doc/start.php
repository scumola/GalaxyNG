<?php echo '<?xml version="1.0" encoding="iso-8859-1"?>'; ?> 
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN" 
   "http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en-US">

<!-- $Id: start.php,v 1.9 2004/06/07 11:22:08 christhecat Exp $ -->

	<head>
		<link rel="stylesheet" href="main.css" />
		<title>GalaxyNG Getting Started</title>
		<?php $title="Getting Started"; ?>
	</head>

	<body>
		<?php include 'menu.php'; ?>
			
			<h2>Join a new game:</h2>
					<p>To join a new game send an email to the GalaxyNG game server (e.g. <tt>server@somewhere.com</tt>) with the subject "<tt>Join GameName</tt>." You will receive a confirmation email from the server.  To find a game, <a href="http://lists.sourceforge.net/lists/listinfo/galaxyng-players">subscribe to GalaxyNG-Players</a> or visit a GalaxyNG server:</p>
				
			<?php include 'newgames.php'; ?>
				
			<hr />
			
			<h2>Get a turn viewer:</h2>
				<p>To display the map and create orders, download a <a href="http://galaxyview.sourceforge.net/tools.html">turn viewer</a>. Galaxyview and MkGal are very popular.  Download some <a href="turns.php">turn reports and orders</a> so you can experiment with the turn viewer.</p>
			
			<hr />
			
			<h2>Learn about the game:</h2>
				<ul>
					<li><a href="manual.php">Players Manual</a></li>
					<li><a href="quickref.php">Quick Reference</a></li>
					<li><a href="faq.php">FAQ</a></li>
					<li><a href="guides/">Guides and Reports</a></li>
				</ul>
			
			<hr />
			
			<h2>Ask for help:</h2>
				<p>Players and GMs alike will be happy to help you.  Ask questions on the <a href="http://lists.sourceforge.net/lists/listinfo/galaxyng-players">GalaxyNG-Players mailing list</a> or send an email to your GM.</p>
			
			<hr />
			
			<h2>Submit orders for the first turn:</h2>
					<p>When the game begins, you will receive a Turn 0 report. You <span class="bold">must</span> send your first <a href="http://galaxyng.sourceforge.net/manual.php#orders">set of orders</a> prior to Turn 1 deadline, and continue sending orders on subsequent turns, otherwise you will be <a href="http://galaxyng.sourceforge.net/manual.php#inactive">eliminated from the game</a>. Send an email to the GalaxyNG server with the subject "<tt>orders</tt>." The first line of your orders should be "<tt>#GALAXY <em>GameName</em> <em>RaceName</em> <em>Password</em></tt>" and the last line should be "<tt>#END</tt>."</p>
					
				<hr />
				
				<h2>Send anonymous email to other races:</h2>
					<p>Send an email to the GalaxyNG server with the subject "<tt>relay <em>RaceName</em></tt>". The first line of the message should be "<tt>#GALAXY <em>GameName</em> <em>RaceName</em> <em>Password</em></tt>."  The body of your message <a href="http://galaxyng.sourceforge.net/manual.php">will be relayed</a> to the other player by the server. Remember to end your message with "<tt>#END</tt>" to prevent your opponents from reading your signature. Also, if you send your message in HTML or HTML+text, the other player will get a copy of your email address, racename and password - causing all kinds of havoc! Make sure that your email client <a href="http://www.expita.com/nomime.html">sends plain ASCII text only</a>.</p>
					
				<hr />
				
				<h2>Conquer the galaxy:</h2>
					<p>Hard work and experience will pay off in the end, as you conquer the galaxy and destroy your enemies!</p>
 		
 		<?php include 'footer.php'; ?>
 	</body>
</html> 	
