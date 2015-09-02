<?php echo '<?xml version="1.0" encoding="iso-8859-1"?>'; ?> 
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN" 
   "http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en-US">

<!-- $Id: faq.php,v 1.18 2004/04/09 01:40:46 christhecat Exp $ -->

	<head>
		<link rel="stylesheet" href="main.css" />
		<title>GalaxyNG FAQ</title>
		<?php $title="FAQ"; ?>
	</head>

	<body>
		<?php include 'menu.php'; ?>

			<h2>Where can I learn more about Galaxy strategy and tactics?</h2>
				<ul>
					<li>Review the <a href="guides/">Guides and Reports</a>.</li>
					<li>Subscribe to the <a href="http://lists.sourceforge.net/lists/listinfo/galaxyng-players">galaxyng-players</a> mailing list and ask questions.</li>
					<li>Read the <a href="start.php">Getting Started</a> guide.</li>
				</ul>
				<p>In the early stages of the game, there will be plenty of planets to spare so there will be little need to fight for territory; however, you should ensure that your homeworlds are defended against sneak attacks by xenophobic aliens. You will need to devote your efforts to exploration, building up colonies, researching technology, and contacting other races with a view to forming alliances.</p>
				<p>The map in your turn report only indicates which planets are occupied by alien races, and the location of alien ships in hyperspace. To learn of enemy fleets which may pose a threat to your safety, you will need to send ships to enemy planets to spy on them.</p>
				<p>If you have an "Incoming Groups" section on your turn report, this probably means that one or more of your planets is under attack, and your first priority should be to ensure that you have an adequate defense. For each incoming group, divide the Distance by Speed to find out how many turns the group will take to reach its target planet. Look at the Total Mass figure - the bigger this is, the greater the potential threat. Of course you don't know whether a very large group is a huge battleship or a fleet of small fighters or anything in between.  You could also try some last-minute diplomacy: the player owning the group cannot turn it back, but he can declare an alliance with you, so that the group will not attack you when it does arrive.</p>
				<p>In the later stages of the game, it is quite likely that one race will develop a dominant position and appear to be heading for victory. At this point, it is vitally important for all the other races to immediately leave aside whatever differences they may have among themselves and launch an attack on this race, because if it is given a chance to pick the others off one by one at his leisure, it is very likely to win.</p>

			<hr />

			<h2>What are the best ship designs?</h2>
				<p>Read <a href="http://www.indyramp.com/galaxy/strategy.html">Ship Design in Galaxy and Blind Galaxy</a>.</p>

			<hr />
				
			<h2>Which turn viewer should I use?</h2>
				<p>It's a matter of personal preference.  <a href="http://jump.to/mkgal">MKGal</a> and <a href="http://galaxyview.sourceforge.net">GalaxyView</a> are very popular GalaxyNG clients, but there are other <a href="http://galaxyview.sourceforge.net/tools.html">Galaxy Tools</a> that you can use.</p>

			<hr />
			
			<h2>How is the galaxy created?  What are growth and stuff planets?</h2>
				<p>Growth planets are size 200 to size 1000.  Stuff planets are size 1 to 200.  The GM has complete control over the distribution of planets in the galaxy.  However, the default settings assume that:</p>
				<ul>
					<li>Each player will have the same size and distribution of home worlds, which will be separated from the other players' homeworlds.</li>
					<li>Each player will have a number of growth planets within a certain radius of their homeworlds.</li>
					<li>There will be a number of stuff planets scattered around the galaxy.</li>
				</ul>
				<p>Thus, a 7 player game advertised with homeworlds 1,000, 500, 250 @ 40LY; 10 growth planets 30LY radius; 25 stuff planets will have the following configuration:</p>
				<ul>
					<li>Each player will have 3 homeworlds with sizes 1,000, 500 and 250 that are at least 40 light years from the other players' homeworlds.</li>
					<li>There will be 10 growth planets within 30 light years of each player's homeworlds (total 7*10=70 growth planets, clustered around homeworlds).</li>
					<li>There will be 25 stuff planets scattered around the galaxy per player (total 7*25=175 stuff planets, randomly distributed in the galaxy).</li>
				</ul>
				<p>Note that growth planet radii can overlap.  GMs can be very creative in manipulating the galaxy creation parameters to create a wide variety of galaxies.  For more information on galaxy creation, refer to the <a href="server.php">Server Manual</a>.</p>

			<hr />
				
			<h2>Is there any point to ordering a very tiny planet to research drive technology?</h2>
				<p>Yes.  The server reports technology levels to two decimal places, but stores numbers in double precision format, so researching 0.001 drive technology will affect ships on the following turn (but not much).  After five turns, drive technology will be 0.005 higher.  Due to rounding, this will be reported to be 0.01 higher.</p>

			<hr />

			<h2>What should I produce at planets with low resources?</h2>
				<p>Planets with low resources can produce anything.  However, they are less efficient at producing ships and capital, so many players use them for research.</p>

			<hr />
				
			<h2>Can I order cargo ships to load cargo, move and unload cargo in one turn?</h2>
				<p>Yes, using the L and S orders (but only if the autounload option is turned on or if the destination planet is the destination for an appropriate route cargo).</p>

			<hr />

			<h2>Can cargo ships be unloaded and sent to another planet in one turn?</h2>
				<p>Yes, using the U and S orders.</p>

			<hr />
				
			<h2>Do I ever need to use the U order if the autounload option is turned on?</h2>
				<p>No.</p>

			<hr />

			<h2>I have 100 cargo ships at planet X and I want to use 50 to transport COL to planet Y and 50 to transport CAP to planet Z.  How do I do this?</h2>
				<p>First option:</p>
<pre>R X COL Y; set a COL route from planet X to planet Y
R X CAP Z; set a CAP route from planet X to planet Z
; The server will send all COL to planet Y,
; then use any remaining ships to send CAP to planet Z</pre>
				<p>Second option:  Assume the 100 ships have group number 20:</p>
<pre>L 20 COL 50; load COL onto 50 ships of group 20
S MAX Y; send the 50 ships that were broken off to planet Y
L 20 CAP; load CAP onto the remaining ships in group 20
S 20 Z; send them to planet Z</pre>

			<hr />

			<h2>I am at war with everybody. I sent a probe to a planet but it was not shot down, how can this be?</h2>
				<p>It is sometimes confusing to determine whether a ship will be shot down, or whether a planet will be bombed. It depends on the peace/war status of all the nations. We will try to clarify this with a number of scenarios:</p>
				<h3>Scenario 1:</h3>
					<ul>
						<li>GoodGuys have declared an alliance with Meanies.  Planet Friendly is owned by GoodGuys. Orbiting Friendly are a number of armed ships owned by GoodGuys.</li>
						<li>Meanies have declared war on GoodGuys.  Planet Evil is owned by Meanies. Orbiting Evil are a number of armed ships owned by Meanies.</li>
					</ul>
					<p>Consider the following situations:</p>
					<ul>
						<li>Meanies send a probe to Friendly. This probe will <strong>not</strong> be shot down by GoodGuys ships, since GoodGuys ships will only fire on Meanies ships in defence.</li>
						<li>GoodGuys send a probe to Evil. This probe will be shot down by Meanies ships.</li>
						<li>Meanies send a big armed ship to Friendly. A battle will take place because the Meanies ship will initiate combat and the GoodGuys ships will defend themselves. If Meanies win, Friendly will be bombed.</li>
						<li>GoodGuys send a big armed ship to Evil. A battle will take place because the Meanies ships will initiate combat and the GoodGuys ships will defend themselves. If GoodGuys wins, Evil will <strong>not</strong> be bombed, since GoodGuys declared an alliance with Meanies. (Of course, GoodGuys will probably declare war on the next turn and bomb Evil!)</li>
					</ul>

				<h3>Scenario 2:</h3>
					<ul>
						<li>Betrayer and Unsuspecting have declared alliances with each other.</li>
						<li>Planets GeeWhiz and Snoody are owned by Unsuspecting.</li>
						<li>Betrayer and Unsuspecting both have armed ships orbiting GeeWhiz.  There are no ships orbiting Snoody.</li>
					</ul>
					<p>Betrayer declares War on Unsuspecting, and orders its ships to travel from GeeWhiz to Snoody.  Before any ships move a battle takes place on Friendly.  The following can happen:</p>
					<ol>
						<li>Betrayer's ships win the battle at GeeWhiz and bomb it.  The Betrayer ships travel to Snoody and bomb it.</li>
						<li>Betrayer's ships win the battle at GeeWhiz and bomb it.  The Betrayer ships travel to Snoody.  Unsuspecting happens to have sent a fleet to Snoody, so another battle occurs.  The Betrayer ships win the second battle and bomb Snoody.</li>
						<li>Betrayer wins the battle at GeeWhiz and bombs it.  The Betrayer ships travel to Snoody.  Unsuspecting happens to have sent a fleet to Snoody, so another battle occurs.  The Betrayer ships lose second the battle and Snoody is not bombed.</li>
						<li>Betrayer loses the battle at GeeWhiz.  GeeWhiz and Snoody are not bombed.</li>
					</ol>

			<hr />

			<h2>The Intercept Command</h2>
				<p>With the intercept command you can order your ships to follow enemy ships from another planet.  It is a handy command.  Be careful though - the sting is in the details. Imagine the following scenario:</p>
				<ul>
					<li>You have some groups at MyPlanet.</li>
					<li>Your enemy has all its groups, including many drones on EnemyPlanet.</li>
					<li>You order all your groups to intercept ships at EnemyPlanet.</li>
					<li>Your enemy gives one of its drones the order to go to SomePlanet.  The remaining enemy ships stay at EnemyPlanet</li>
					<li>SomePlanet just happens to be two turns travel distance for all of your groups.</li>
				</ul>
				<p>What will happen?  All your groups will follow that single probe to SomePlanet!  Your enemy will be free to operate knowing your ships are in hyperspace for a turn, heading to SomePlanet on the fringes of the conflict.</p>
				<p>You might have thought that the intercept order would cause your groups to
go to EnemyPlanet, which has the larger mass of enemy ships.  However, the intercept command only takes in account the mass of groups that <strong>leave</strong> the intercepted planet.</p>
				<p>The situation would be even worse if SomePlanet was within two turn range of some of your groups and more than two turn range of your remaining groups.  In this case, some of your groups will follow the probe, while the others will go to EnemyPlanet, with a much smaller chance of winning the battle.  This problem can be avoided by organizing your groups into fleets.</p>
				<p>The rules apply both ways; if the roles are reversed you can use the same trick on your enemy.</p>
 		
 		<?php include 'footer.php'; ?>
 	</body>
</html> 	








