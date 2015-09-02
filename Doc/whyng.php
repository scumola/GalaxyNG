<?php echo '<?xml version="1.0" encoding="iso-8859-1"?>'; ?> 
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN" 
   "http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en-US">

<!-- $Id: whyng.php,v 1.12 2004/04/04 23:37:41 christhecat Exp $ -->

	<head>
		<link rel="stylesheet" href="main.css" />
		<title>Why GalaxyNG?</title>
		<?php $title="Why GalaxyNG?"; ?>
	</head>

	<body>
		<?php include 'menu.php'; ?>
			
			<p>Frans Slothouber's reasons for creating GalaxyNG are presented below.</p>
			
			<hr />
			
			<p>Galaxy has been around for a while, it's a great, well balanced, and addictive game, but it can be improved since:</p>
			<ol>
				<li>The game can be slow, dragging on for more than 60 turns, without coming to a conclusion.</li>
				<li>The algorithm that generates the layout of the galaxy, that is the position of the planets, is rather arbitrary, leaving some players in disadvantageous positions.</li>
				<li>The code contains core-dump-producing bugs, and is hard to maintain.</li>
			</ol>
			<p>Galaxy Next Generation tries to solve these problems.</p>
			
			<hr />

			<h2>1.1 Code Rewrite</h2>
				<p>About 70 percent of the orginal Galaxy code was rewritten. This solved a number of bugs that cause the code to core dump. The code is still a bit of a mess though. Someday someone with a lot of spare time should completely rewrite it in C++ :)</p>
				<p>The bug fixes also cause GalaxyNG to behave slightly different from Galaxy. That is:</p>
				<ul>
					<li>Upgrading very large ships:
						<br />There was a bug that prevented very large ships from being upgraded. To be more specific: when you had <strong>one</strong> very large ship orbiting a planet, and that planet's industry was not enough for a full upgrade, the ship would not be upgraded at all.  Even worse, even though no upgrade took place, the program still marked all industry points of that planet as spent. This bug has been fixed; the ship will now be partially upgraded. All tech-levels will be increased by an equal amount, depending on the available industry. This is more practical: one can build mass 10,000 ships and keep them in service until eternity by upgrading them.</li>
					<li>Bombing of planets:
						<br />There was a bug that prevented armed ships that had been given a Send order from bombing the planet they were orbiting before the hyper-space jump. This bug has been fixed and it is now possible to bomb two planets in the same turn, with the same ship or fleet.</li>
					<li>Production Bug:
						<br />There was a bug that prevented some ships from being built on a planet, even though it had enough industry and, in particular, enough material. The cause of this bug was that the program did not take in account that having material stockpiled frees industry points that otherwise would have to be spent on building material. This bug has been fixed.</li>
					<li>AUTOUNLOAD option:
						<br />The autounload option did not work with empty planets.  That is, when you sent cargo ships to a empty planet you had to manually unload them, which is silly since when you use routes Galaxy automatically unloads cargo on empty planets. This bug is fixed. You can send cargo ships to an empty planet and any cargo they carry is automatically unloaded upon arrival (that is, provided you have activated the autounload option). Of course cargo is also automatically unloaded at planets a player owns. However no autounload takes place at planets of other nations.</li>
				</ul>

			<hr />

			<h2>Changes to the Original Rules</h2>
				
				<p>These are the changes to the standard rules of Galaxy:</p>
				<ol>
					<li>Cargo:
						<br />One can unload any type of cargo on planets of other nations.</li>
					<li>Bombing:
						<br />When a player bombs an enemy planet that player become the owner of that planet. The industry and population are reduced to 25% of their current value.</li>
					<li>You will ask, who gets the planet when two friendly nations have ships above the same planet? Normally this is the nation that comes first in the list with nations (as shown in the turn report). This is unfair of course and allies will like to divide the planets they conquer amongst each other. This is possible with a new command. It is used to indicate that a player claims ownership of a planet when it is bombed. Of course, it only works when the player has armed ships left above the planet when it is bombed. In case two or more nations claim the same planet, nobody gets the planet.  It has the following syntax:</li>
				</ol>
<pre>V [planet_name]</pre>

			<hr />

			<h2>New Map Layout</h2>
				<p>What was wrong with the original layout used in Galaxy? Sometimes the layout created left some nations in rather unfair positions, that is, some nations would have their home planet located far away from other planets, while others nations' home planets were located near a whole bunch of good planets. The new code tries to provide all players with an equal chance on success, while still keeping the game diverse and interesting. How is this accomplished?</p>
				<ol>
					<li>Empty Planets:
						<br />Each nation is given the same number of empty development planets. That is within a circle of a given radius around the home planet of a nation, a fixed number of planets is randomly allocated. The size of these planets differ, but the total sum of the sizes is the same for all nations. The resources of the planets are still chosen at random, though experience shows that the total ship mass the planets can produce is about the same for all nations.</li>
					<li>Home Planet Spacing:
						<br />The minimum distance between home world can be specified. Home worlds can therefore be widely spaced from each other, ensuring that a nation isn't kicked out in the first few turns.</li>
					<li>Stuff Planets:
						<br />To improve the tactical possibilities, a number of stuff planets can be scattered across the galaxy. These are small useless planets that provide players with different routes of attack.</li>
					<li>Additional Home Planets:
						<br />In addition, it is possible to let nations start with more than one home planet.  The additional home planets are located within a radius of 3 ly from the original home planet of the nations. Starting with multiple home planets speeds up the game considerably.</li>
				</ol>
			
			<hr />
			
			<h2>Orders Checker and Forecaster</h2>
				<p>The orders checker has been integrated with the game code, so both will use the same algorithm and produce the same results. The orders checker has also been extended with a forecaster, which generates a forecast of the sitation of the planets and groups as it will be the next turn.</p>

		<?php include 'footer.php'; ?>
	</body>
</html>