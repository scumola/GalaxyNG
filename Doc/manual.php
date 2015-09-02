<?php echo '<?xml version="1.0" encoding="iso-8859-1"?>'; ?> 
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN" 
   "http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en-US">

<!-- $Id: manual.php,v 1.52 2004/11/25 01:05:55 christhecat Exp $ -->

	<head>
		<link rel="stylesheet" href="main.css" />
		<title>GalaxyNG Players' Manual</title>
		<?php $title="Players' Manual"; ?>
	</head>

	<body>
		<?php include 'menu.php'; ?>

			<h2 id="toc">Table of Contents</h2>
				<ul>
					<li><a href="#intro">Introduction</a>
						<ul>
							<li><a href="#galaxy">The Galaxy</a></li>
							<li><a href="#players">Players</a></li>
							<li><a href="#victory">Victory and Defeat</a></li>
							<li><a href="#names">Names</a></li>
							<li><a href="#units">Units of Measure</a></li>
						</ul>
					</li>
					<li><a href="#planets">Planets</a>
						<ul>
							<li><a href="#owner">Owner</a></li>
							<li><a href="#size">Size</a></li>
							<li><a href="#pop">Population</a></li>
							<li><a href="#ind">Industry</a></li>
							<li><a href="#res">Resources</a></li>
							<li><a href="#stock">Stockpiles</a></li>
						</ul>
					</li>
					<li><a href="#ships">Ships</a>
						<ul>
							<li><a href="#types">Ship Types</a></li>
							<li><a href="#groups">Groups</a></li>
							<li><a href="#fleets">Fleets</a></li>
							<li><a href="#tech">Technology</a></li>
							<li><a href="#mass">Ship Mass</a></li>
							<li><a href="#move">Movement</a></li>
							<li><a href="#speed">Speed</a></li>
							<li><a href="#locate">Locating Ships</a></li>
							<li><a href="#cargo">Cargo</a></li>
							<li><a href="#routes">Routes</a></li>
						</ul>
					</li>
					<li><a href="#prod">Production</a>
						<ul>
							<li><a href="#mat">Materials</a></li>
							<li><a href="#cap">Capital</a></li>
							<li><a href="#tres">Technology Research</a></li>
							<li><a href="#build">Ship Building</a></li>
							<li><a href="#upgrade">Upgrading Groups</a></li>
						</ul>
					</li>
					<li><a href="#combat">Combat</a>
						<ul>
							<li><a href="#war">War and Alliances</a></li>
							<li><a href="#bat">Fighting Battles</a></li>
							<li><a href="#bomb">Bombing Planets</a></li>
						</ul>
					</li>
					<li><a href="#turn">Turn Sequence</a></li>
					<li><a href="#rep">Turn Report</a>
						<ul>
							<li><a href="#repbul">Bulletins</a></li>
							<li><a href="#repglo">Global Messages</a></li>
							<li><a href="#repper">Personal Messages</a></li>
							<li><a href="#repopt">Your Options</a></li>
							<li><a href="#repord">Orders Received</a></li>
							<li><a href="#repmis">Mistakes</a></li>
							<li><a href="#repstat">Status of Players</a></li>
							<li><a href="#repst">Your Ship Types</a></li>
							<li><a href="#repast">Alien Ship Types</a></li>
							<li><a href="#repbat">Battles</a></li>
							<li><a href="#repbom">Bombings</a></li>
							<li><a href="#repmap">Map</a></li>
							<li><a href="#repinc">Incoming Groups</a></li>
							<li><a href="#reppla">Your Planets</a></li>
							<li><a href="#repsip">Ships In Production</a></li>
							<li><a href="#reproute">Your Routes</a></li>
							<li><a href="#repapla">Alien Planets</a></li>
							<li><a href="#repupla">Unidentified Planets</a></li>
							<li><a href="#repunpla">Uninhabited Planets</a></li>
							<li><a href="#repfleets">Your Fleets</a></li>
							<li><a href="#repagroups">Alien Groups</a></li>
						</ul>
					</li>
					<li><a href="#orders">Orders</a>
						<ul>
							<li><a href="#send">Sending Orders</a></li>
							<li><a href="#fore">Forecast and Errors</a></li>
							<li><a href="#adv">Advance Orders</a></li>
							<li><a href="#inactive">Inactivity</a></li>
							<li><a href="#ex">Example Orders</a></li>
							<li><a href="#form">Order Format</a></li>
							<li><a href="#ordA">@ Send Message</a></li>
							<li><a href="#ordE">= Set Player Name</a></li>
							<li><a href="#orda">a Declare an Alliance</a></li>
							<li><a href="#ordb">b Break from Group or Fleet</a></li>
							<li><a href="#ordc">c Change Race Name</a></li>
							<li><a href="#ordd">d Design a Ship or Fleet</a></li>
							<li><a href="#orde">e Eliminate a Ship Type or Fleet</a></li>
							<li><a href="#ordf">f Find Player Email Address</a></li>
							<li><a href="#ordg">g Upgrade Ships</a></li>
							<li><a href="#ordh">h Recall a Group or Fleet</a></li>
							<li><a href="#ordi">i Intercept Ships</a></li>
							<li><a href="#ordj">j Add a Group to a Fleet or Merge Fleets</a></li>
							<li><a href="#ordl">l Load Cargo</a></li>
							<li><a href="#ordm">m Change ASCII Map</a></li>
							<li><a href="#ordn">n Name a Planet</a></li>
							<li><a href="#ordo">o Set Options</a></li>
							<li><a href="#ordp">p Set Planet Production</a></li>
							<li><a href="#ordq">q Quit</a></li>
							<li><a href="#ordr">r Set a Route</a></li>
							<li><a href="#ords">s Send a Group or Fleet</a></li>
							<li><a href="#ordt">t Rename a Ship Type or Fleet</a></li>
							<li><a href="#ordu">u Unload Cargo</a></li>
							<li><a href="#ordv">v Claim a Planet</a></li>
							<li><a href="#ordw">w Declare War</a></li>
							<li><a href="#ordx">x Scrap Ships</a></li>
							<li><a href="#ordy">y Change Password</a></li>
							<li><a href="#ordz">z Change Email Address</a></li>
						</ul>
					</li>
					<li><a href="#misc">Miscellaneous</a>
						<ul>
							<li><a href="#email">Sending Messages to Other Players</a></li>
							<li><a href="#ascii">Plain ASCII Text</a></li>
						</ul>
					</li>
				</ul>

			<hr />

			<h2 id="intro">Introduction</h2>
				<p>GalaxyNG is a multiplayer game of interstellar war.  The objective of the game is to conquer the galaxy.</p>
				<p>Players email a GalaxyNG server to register for games, submit orders and receive turn reports.  The server automatically processes turns and responds to player email.  Most games run one to five turns per week and can last from a few months to a year or more.  The Game Master (GM) maintains the server, selects game options, creates the galaxy, answers player questions and solves problems.</p>

				<h3 id="galaxy">The Galaxy</h3>
					<p>The area of the galaxy in which the game is played is a square (for the sake of simplicity the third dimension is ignored) which contains a number of habitable planets.  Planets are used to build ships, conduct technology research, produce capital or produce materials.  Ships explore the galaxy, colonize uninhabited planets, haul cargo, conduct warfare and conquer planets inhabited by other races.  Technology improvements allow ships to fly faster and farther, haul more cargo and fight better.</p>
					
				<h3 id="players">Players</h3>
					<p>Each player is the leader of one race.  Players can communicate by sending <a href="#ordA">messages</a> or <a href="#email">email</a> to each other.  Players are <a href="#ordo">anonymous</a> but can choose to reveal their identities to other players.  Players can <a href="#ordE">provide their real name</a> to the GM for inclusion in the <a href="http://www.refs-home.net/galaxyng/hof/">Hall of Fame</a> without breaking their anonymity.</p>

				<h3 id="victory">Victory and Defeat</h3>
					<p>The game can be won by a single race or by an alliance of races, depending upon the game settings.  Races are ranked based on the total production of their planets.  A race is eliminated if it owns no planets and has no ships.  The game is complete when all surviving players agree to end it and notify the game master.</p>

				<h3 id="names">Names</h3>
					<p>Races, planets, ship types and fleets can be named.  Names may be no more than twenty characters (a character is a letter, digit, or underscore) and may not include spaces.  Names must be unique.  For example, a race cannot have the same name as a fleet.  Each player should <a href="#ordc">provide a name</a> by which their race will be known, e.g. Mutant_Camels or Zzyaxians - if this is not done they will simply be known by number, e.g. Race_4 or Race_5.  Ship types are given names when <a href="#ordd">designed</a> and can be <a href="#ordt">renamed</a>.  Planets are numbered at the start of the game and can be <a href="#ordn">given new names</a> by their current owner.</p>
					
				<h3 id="units">Units of Measure</h3>
					<p>Distances are measured in light-years.  Each unit of population represents ten million people and each unit of capital or materials represents ten million tons.  Each turn represents about four years of time.</p>
					<p>The game engine stores numbers in double precision format but <strong>truncates</strong> values reported to players to two decimal places.  For example, drive technology 2.4389 will be reported as 2.43 and a ship designed with shields mass 18.11999999 will be reported as shields mass 18.11.  Note that galaxy size, planet coordinates and resources are the exceptions - a planet reported at 138.44,43.29 with 4.30 resources is actually at that location and will not be found at 138.4426,43.28934 with 4.3032 resources.  Negative numbers are not used in GalaxyNG.</p>

			<hr />

			<h2 id="planets">Planets</h2>
					<p>Planets are located in the galaxy by X and Y coordinates.  Planets have seven characteristics: owner, size, population, industry, resources, production and stockpiles.</p>

				<h3 id="owner">Owner</h3>
					<p>Inhabited planets are owned by one race at a time (except in the special case of a standoff when an inhabited planet is unowned).  Uninhabited planets are not owned by any race.</p>

				<h3 id="size">Size</h3>
					<p>Planets vary in size from 0.01 to 1000.00 (GMs can create larger planets).  The size of a planet reflects the habitability of the terrain, the suitability of the climate for agriculture, and other features.  At the beginning of the game, the galaxy is usually divided into inhabited planets (home planets), uninhabited development planets (size 200.00 to 1000.00) and uninhabited stuff planets (size 0.01 to 200.00).</p>

				<h3 id="pop">Population</h3>
					<p>Each inhabited planet has a population, which can never be larger than the planet's size but may be smaller.  A planet with zero population is uninhabited.  Home planets are fully populated at the beginning of the game.  Each planet's population grows by 8% per turn.  Excess population is stockpiled as colonists.  Each unit of colonists represents eight population.</p>

				<h3 id="ind">Industry</h3>
					<p>Each inhabited planet has industry, which can never be larger than the planet's population but may be smaller.  If population exceeds industry, the industry may be increased by adding capital.  If there is not an existing stockpile of capital, it may be <a href="#ordp">produced</a> at the planet or be shipped in from another planet by cargo ships.  For example, if a size 500 planet with 500 population and 200 industry produces 75 capital, the industry will increase to 275.  If a size 500 planet with 200 population and 200 industry has a stockpile of 100 capital, on the following turn the population and industry will both increase to 216 and there will be 84 capital left (because population grows by 8% per turn).</p>

				<h3 id="res">Resources</h3>
					<p>Each planet has a resources value which indicates how rich it is in metals, coal, oil, wood and other products.  Home planets have a resources value of 10.00.  Other planets have a resources value between 0.01 and 10.00, with the average being 1.00.  Planets high in resources can easily <a href="#ordp">produce</a> materials such as sheet steel, copper wire and plastics.  Materials are necessary to build ships and produce capital.</p>

				<h3 id="stock">Stockpiles</h3>
					<p>Each planet may have stockpiles of colonists, capital and materials.  Population increases beyond the planet's size are converted into colonists.  Eight population are converted into one colonist.  These are people automatically stored in containers in deep freeze.  Excess capital and materials are stockpiled at a one-to-one ratio.  Stockpiles may be saved for later use or transported to other planets by cargo ships.</p>

			<hr />

			<h2 id="ships">Ships</h2>
				<p>Ships are organized into groups and fleets which are used for exploration, scouting, attack, defence and cargo transport.  Ships have the following characteristics: ship type (name, drive mass, attacks, weapons mass, shields mass, cargo mass and ship mass), group number, fleet name, drive technology, weapons technology, shields technology, cargo technology, cargo carried type, cargo carried mass, destination planet, and distance to destination planet.  Ships can be <a href="#ordp">built</a> at planets.</p>

				<h3 id="types">Ship Types</h3>
					<p>Each player can <a href="#ordd">design</a> ship types, which are unique to that player, by specifying: name, drive mass (the power of the hyperdrive engines), attacks (the number of guns), weapons mass (the strength of the guns), shields mass (the power of the shields generator), and cargo mass (the size of the cargo bay).  Drive mass, weapons mass, shields mass, and cargo mass must equal zero or be equal to or greater than one.  Thus, drive mass 0.00 and 1.50 are allowed but drive mass 0.75 is not.  Attacks must be an integer.  Some example ship types are:</p>
<pre>                  D    A     W      S      C
Drone            1.00   0   0.00   0.00   0.00
Flak             1.00   0   0.00   2.00   0.00
FastFlak         1.01   0   0.00   1.01   0.00
Fighter          2.48   1   1.20   1.27   0.00
Gunship          4.00   2   2.00   4.00   0.00
Destroyer        6.00   3   4.00   4.00   0.00
Cruiser         16.50  30   1.50   9.75   0.00
BattleCruiser   49.50  25   3.00   9.50   1.00
Battleship      33.00   3  25.00  16.00   1.00
BattleStation   99.00   1  50.00  49.00   0.00
OrbitalFort      0.00  11  10.00  39.00   0.00
SpaceGun         0.00   1   9.90   9.90   0.00
Hauler           2.00   0   0.00   0.00   1.00
Freighter       30.00   0   0.00   9.50  10.00
MegaFreighter  120.00   0   0.00  38.43  39.57</pre>

					<p>Players can <a href="#ordt">change</a> the name of an existing ship type.  Ship types can be <a href="#orde">eliminated</a> if no ships of that type exist or are under construction.</p>

				<h3 id="groups">Groups</h3>
					<p>A race can have hundreds or even thousands of ships, which would be inconvenient to handle individually.  Hence ships are handled in groups, which can contain one or more ships all of the same type, in the same place, carrying the same quantity and type of cargo (if any) and built with the same technology levels.</p>
					<p>Groups can be ordered to <a href="#ords">travel</a> to another planet, <a href="#ordi">intercept</a> other ships, <a href="#ordh">reverse</a> course, <a href="#ordb">break</a> off ships into a new group, <a href="#ordj">join</a> a fleet, <a href="#ordb">leave</a> a fleet, <a href="#ordg">upgrade</a> technology levels, <a href="#ordl">load</a> cargo, or <a href="#ordu">unload</a> cargo.  Groups may also be <a href="#ordx">scrapped</a>, which automatically unloads all cargo and deposits the materials used to produce the ships at the current planet.  If <a href="#ordr">routes have been established</a>, groups will follow them.</p>
					<p>Each group is assigned a number, which is used to issue orders to ships in the corresponding group.  When new groups are created, they are assigned a number N+1, where N is the highest numbered group in existence.  Whenever a group number is required as an order parameter, the keyword MAX may be used instead. This will apply the order to the group with the highest group number, i.e. the most recently created group.</p>
					<p>In several phases of the turn, groups containing identical ships, carrying identical cargo (if any), in the same place and in the same fleet (if applicable) will be merged using the lower group number.  For example, if group 5 containing 48 ships is merged with group 12 containing 52 ships, group 5 will contain 100 ships and group 12 will be eliminated.</p>
					<p>If the SortGroups <a href="#ordo">option</a> is turned on, at the end of each turn groups will be automatically sorted and renumbered as follows: Owner's groups at owner's planets; Owner's groups at other race's planets; Owner's groups at uninhabited planets; Owner's groups in fleets.</p>

				<h3 id="fleets">Fleets</h3>
					<p>Fleets contain groups, allowing players to gather different ship types together in a single unit.  Many of the orders for groups can also be used for fleets.  Players can <a href="#ordd">create</a>, <a href="#orde">eliminate</a>, <a href="#ordj">merge</a> and <a href="#ordt">rename</a> fleets.  Fleets can be ordered to <a href="#ords">travel</a> to another planet, <a href="#ordi">intercept</a> other ships, or <a href="#ordh">reverse</a> course.  The slowest group in the fleet sets the maximum speed for the entire fleet.</p>
					<p>A group in a fleet that is given a <a href="#ords">send</a> or <a href="#ordi">intercept</a> order is automatically removed from the fleet.  Ships in a fleet that are <a href="#ordb">broken</a> from groups are also removed from the fleet.  Groups in fleets that are ordered to <a href="#ordg">upgrade</a>, <a href="#ordl">load</a> or <a href="#ordu">unload</a> cargo remain in the fleet.  Groups in fleets will not <a href="#ordr">travel on routes</a>.</p>

				<h3 id="tech">Technology</h3>
					<p>Technology determines the effectiveness of ships.  There are four technologies: drive, weapons, shields and cargo.  Each race begins the game with 1.00 levels in each technology.  Technology can be increased by ordering planets to conduct <a href="#ordp">research</a>.</p>
					<p>Ships are assigned the technology levels of the race at the time they are <a href="#ordp">produced</a>.  As the race's technology level increases, ships can be <a href="#ordg">upgraded</a> at planets owned by the player.</p>
					<p>A ship with drive technology 4.00 is twice as fast as an equivalent ship with drive technology 2.00, a ship with weapons technology 1.50 has a 50% more powerful attack than the same ship with weapons technology 1.00, and so forth.  Ships without a component are considered to have a matching technology of zero.  For example, a ship with weapons mass zero is considered to have weapons technology zero.</p>
  
  			<h3 id="mass">Ship Mass</h3>
					<p>Ship types with zero or one attack have a ship mass of drive mass + weapon mass + shield mass + cargo mass.  Each additional attack beyond the first adds 50% of the weapon mass to the ship mass.  For example:</p>
<pre>                  D   +     W   +    Additional    +     S   +   C   =  Ship
                                      Attacks                           Mass
Drone            1.00 +    0.00 +        0         +    0.00 +  0.00 =   1.00
Flak             1.00 +    0.00 +        0         +    2.00 +  0.00 =   3.00
FastFlak         1.01 +    0.00 +        0         +    1.01 +  0.00 =   2.02
Fighter          2.48 +    1.20 +        0         +    1.27 +  0.00 =   4.95
Gunship          4.00 +    2.00 + ( 1 *  2.00 / 2) +    4.00 +  0.00 =  11.00
Destroyer        6.00 +    4.00 + ( 2 *  4.00 / 2) +    4.00 +  0.00 =  18.00
Cruiser         16.50 +    1.50 + (29 *  1.50 / 2) +    9.75 +  0.00 =  49.50
BattleCruiser   49.50 +    3.00 + (24 *  3.00 / 2) +    9.50 +  1.00 =  99.00
Battleship      33.00 +   25.00 + ( 2 * 25.00 / 2) +   16.00 +  1.00 =  99.00
BattleStation   99.00 +   50.00 +        0         +   49.00 +  0.00 = 198.00
OrbitalFort      0.00 +   10.00 + (10 * 10.00 / 2) +   39.00 +  0.00 =  99.00
SpaceGun         0.00 +    9.90 +        0         +    9.90 +  0.00 =  19.80
Hauler           2.00 +    0.00 +        0         +    0.00 +  1.00 =   3.00
Freighter       30.00 +    0.00 +        0         +    9.50 + 10.00 =  49.50
MegaFreighter  120.00 +    0.00 +        0         +   38.43 + 39.57 = 198.00</pre>
  
				<h3 id="move">Movement</h3>
					<p>Ships are equipped with hyperspace drives, except ships with a drive mass of zero which remain forever at the planet where they were built.  Hyperspace travel is only possible from one planet to another.  Groups always travel at maximum speed in hyperspace, unless they are part of a fleet.  Groups in hyperspace cannot participate in combat.  Groups in hyperspace cannot be issued any orders, except <a href="#ordh">reverse</a>.</p>
					<p>Groups can be <a href="#ords">sent</a> to other planets.</p>	
					<p>Groups can be ordered to <a href="#ordi">intercept</a> alien ships at a target planet.  Intercepting groups will attempt to follow alien ships leaving the target planet, but can only intercept at destination planets within two turns range of the intercepting group.  The intercepting group will be sent toward whichever destination planet is within two turns range and has the largest total mass (ship mass plus cargo carried mass) of alien ships departing toward it from the target planet that turn.  If all destination planets are farther than two turns range, or if no alien groups leave the target planet, the intercepting group will be sent to the target planet.  Note that the total mass of ships leaving the target planet is the sole determinant of potential interception locations.  If a single Probe departs a target planet and 2,000 ships remain behind, the intercepting group will follow the Probe.</p>
					<p>Groups in hyperspace that are more than four turns from their destination planet may be ordered to <a href="#ordh">reverse</a> course and return to their planet of origin.  This is useful for retrieving groups accidentally sent to the wrong planet.</p>

				<h3 id="speed">Speed</h3>
					<p>Ships move a number of light years per turn according to the following formula:</p>
<pre>Speed = 20 * drive technology * (drive mass / (ship mass + effective cargo carried mass)).</pre>
					<p>Note that unless your drive technology is very high, large ships should have correspondingly large drives or they will be very slow.  On the other hand the fastest ships you can possibly build (all numbers except drive mass being zero in the design) can only travel at a speed of twenty times your drive technology.  For example, using the ship types above and assuming drive technology 1.00:</p>
<pre>               20 * Drive * (  Drive / (   Ship +   Effective   )) = Speed
                     Tech       Mass       Mass   Cargo Carried
Drone          20 *  1.00 * (   1.00 / (   1.00 +      0.00     )) = 20.00
Flak           20 *  1.00 * (   1.00 / (   3.00 +      0.00     )) =  6.66
FastFlak       20 *  1.00 * (   1.01 / (   2.02 +      0.00     )) = 10.00
Fighter        20 *  1.00 * (   2.48 / (   4.95 +      0.00     )) = 10.02
Gunship        20 *  1.00 * (   4.00 / (  11.00 +      0.00     )) =  7.27
Destroyer      20 *  1.00 * (   6.00 / (  18.00 +      0.00     )) =  6.66
Cruiser        20 *  1.00 * (  16.50 / (  49.50 +      0.00     )) =  6.66
BattleCruiser  20 *  1.00 * (  49.50 / (  99.00 +      0.00     )) = 10.00
Battleship     20 *  1.00 * (  33.00 / (  99.00 +      0.00     )) =  6.66
BattleStation  20 *  1.00 * (  99.00 / ( 198.00 +      0.00     )) = 10.00
OrbitalFort    20 *  1.00 * (   0.00 / (  99.00 +      0.00     )) =  0.00
SpaceGun       20 *  1.00 * (   0.00 / (  19.80 +      0.00     )) =  0.00
Hauler         20 *  1.00 * (   2.00 / (   3.00 +      0.00     )) = 13.33
Freighter      20 *  1.00 * (  30.00 / (  49.50 +      0.00     )) = 12.12
MegaFreighter  20 *  1.00 * ( 120.00 / ( 198.00 +      0.00     )) = 12.12</pre>
					<p>Note that, when fully loaded, cargo ships can be much slower.  For example:</p>
<pre>BattleCruiser  20 *  1.00 * (  49.50 / (  99.00 +      1.10     )) =  9.79
Battleship     20 *  1.00 * (  33.00 / (  99.00 +      1.10     )) =  6.52
Hauler         20 *  1.00 * (   2.00 / (   3.00 +      1.10     )) =  9.75
Freighter      20 *  1.00 * (  30.00 / (  49.50 +     20.00     )) =  8.63
MegaFreighter  20 *  1.00 * ( 120.00 / ( 198.00 +    196.14     )) =  6.09</pre>

				<h3 id="locate">Locating Ships</h3>
					<p>Each race's administrative staff will keep a record of its own ships on planets or in hyperspace.  Planet owners have full knowledge of all ships orbiting their planets.  Players also receive complete reports on ships at planets visited by their own ships.</p>
					<p>Locating alien ships in hyperspace is much more problematic.  Detectors to accurately locate the position of alien ships in hyperspace are installed on each planet.  Accurate readings of mass, speed, origin and distance from destination can be obtained for ships heading directly toward a detector (i.e. inbound to a planet).  A rough indication of the location of other alien groups is indicated on the <a href="#ordm">text map</a>, but their mass, speed and direction of travel are unknown.</p>

				<h3 id="cargo">Cargo</h3>
					<p>Cargo ships can <a href="#ordl">load</a> stockpiles of colonists, capital and materials to <a href="#ords">transport</a> them to other planets where they can be <a href="#ordu">unloaded</a>.  Each ship may only carry one type of cargo at a time.  The base amount of cargo a ship can carry is determined by the following formula: base cargo = cargo mass + cargo mass^2/10.  Thus, at cargo technology 1.00, some examples would be:</p>
<pre>               Cargo + Cargo Mass^2/10 =  Base Cargo
                Mass                        Carried
BattleCruiser   1.00 +       0.10      =      1.10
Battleship      1.00 +       0.10      =      1.10
Hauler          1.00 +       0.10      =      1.10
Freighter      10.00 +      10.00      =     20.00
MegaFreighter  39.57 +     156.57      =    196.14</pre>
       <p>Cargo technology increases the amount of cargo that a ship can carry.  Thus, a Hauler with cargo technology 2.00 can carry 2.20 cargo and a Freighter with cargo technology 3.00 can carry 60.00 cargo.  This does not slow down the ships, as the cargo carried is divided by the cargo technology to obtain the effective cargo carried.  Thus, at cargo technology 3.00, a Freighter carrying 60.00 cargo has an effective cargo carried of 20.00, while the same Freighter carrying 35.00 cargo has an effective cargo carried of 11.66.</p>
       <p>Colonists that are unloaded at an uninhabited planet will claim the planet for their race.  Each colonist becomes eight population.  Capital and materials unloaded at an
uninhabited planet will be stockpiled until the planet is colonized.</p>

     <h3 id="routes">Routes</h3>

      <p>Colonists, capital, materials, and empty routes can be <a href="#ordr">established</a> between planets.  Each planet can be the origin for only one route of each type.  A planet can be the destination of an unlimited number of routes.  For example, colonists routes can be set from planets 105, 82 and 243 to planet 56 but planet 105 cannot simultaneously have a colonists route to planet 97.  Players may only establish routes from their own planets, however any planet may be the destination of a route.</p>      
      <p>Cargo ships that are not in fleets and have not been ordered to <a href="#ords">travel</a> between planets will automatically follow routes.  They will load cargos, travel to destination planets, and unload cargos.  Cargo ships that are already loaded will also follow routes.</p>
      <p>Routes are assigned to cargo ships in the following order of priority: colonists, capital, materials and empty.  Ships are used in order by group number to full capacity, if possible.  For example, planet 105 has colonists and capital routes to planet 56, an empty route to planet 74, forty cargo ships with 2.00 capacity each, 10.00 colonists and 31.00 capital.  Five cargo ships will carry colonists to planet 56, sixteen cargo ships will carry capital to planet 56 and the remaining nineteen cargo ships will travel empty to planet 74.  However, if there is an additional cargo ship with capacity 40.00 and a lower group number than the 2.00 capacity ships, it will be used to carry 10.00 colonists to planet 56.  If it has a higher group number, it will travel empty to planet 74.</p>
      <p>All ships, regardless of origin planet, that arrive at a destination planet for a route will be unloaded if they carry the appropriate cargo.  Unloading at route destinations occurs regardless of the status of the <a href="#ordo">autounload</a> option.</p>
     
			<hr />

			<h2 id="prod">Production</h2>
				<p>The productive capacity of a planet is determined mostly by its industry value and partly by its population.  Each unit of industry on a planet yields one production unit, and every four units of population over and above industry yields an additional production unit.  The formula is:</p>
<pre>Production = Industry + (Population - Industry)/4</pre>
<p>or, put another way:</p>
<pre>Production = (Industry * .75) + (Population * .25)</pre>
				<p>For example, a planet with 500.00 population and 500.00 industry has 500.00 production (500 + 0), a planet with 500.00 population and 250.00 industry has 312.50 production (250 + 250/4) and a planet with 500.00 population and 0 industry has 125.00 production (0 + 500/4).</p>
				<p>A planet can be ordered to <a href="#ordp">produce</a> materials, capital, technology research or ships.  Each planet can only perform one type of production per turn, however two planets can produce two things in one turn or one planet can produce two things in two turns.  A planet will continue producing the same thing until it is ordered to change production, thus new production orders are not required for each planet each turn.</p>
				
				<h3 id="mat">Materials</h3>
					<p>Materials production is determined by the resources values of the planet, which equals the number of materials that will be produced per point of production devoted to the task.  For example, a planet with 5.00 resources and 100.00 production will produce 500.00 materials, while a planet with 0.10 resources would only produce 10.00 materials.</p>
				
				<h3 id="cap">Capital</h3>
					<p>Producing 1.00 capital requires 5.00 production and 1.00 materials.  If the planet does not have a stockpile of materials, some production will automatically be diverted to producing materials.  For example, a planet with 1000.00 production and a stockpile of 200.00 materials will produce 200.00 capital.  With no stockpile of materials and 10.00 resources, 196.08 capital will be produced.  With 0.10 resources, the planet will produce 66.67 capital.</p>
				
				<h3 id="tres">Technology</h3>
					<p>A planet can research one of the four technologies each turn.  It costs 5,000.00 production to increase drive, weapons, or shields technology by one point and 2,500.00 production to increase cargo technology by one point.  Fractional increases are effective immediately: if you spend 500.00 production on research into Weapons, your weapons technology will go up by 0.10.  Research takes effect on the following turn and applies to all new and upgraded ships, regardless of which planet conducted the research.</p>

				<h3 id="build">Ship Building</h3>
					<p>A planet can produce one type of ship each turn.  The production cost of a ship is equal to its ship mass times ten.  In addition, one unit of materials is required for every unit of mass.  If the planet does not have a stockpile of materials, some production will automatically be diverted to producing materials.</p>
<pre>                  Mass  Production  Materials
Drone             1.00     10.00       1.00
Flak              3.00     30.00       1.00
FastFlak          2.02     20.20       2.02
Fighter           4.95     49.50       4.95
Gunship          11.00    110.00      11.00
Destroyer        18.00    180.00      18.00
Cruiser          49.50    495.00      49.50
BattleCruiser    99.00    990.00      99.00
Battleship       99.00    990.00      99.00
BattleStation   198.00   1980.00     198.00
OrbitalFort      99.00    990.00      99.00
SpaceGun         19.80    198.00      19.80
Hauler            3.00     30.00       3.00
Freighter        49.50    495.00      49.50
MegaFreighter   198.00   1980.00     198.00	</pre>
					<p>For example: If a planet with 1000.00 production and 10.00 resources with no stockpile of materials was producing Drones, it would produce 99.01 per turn.  About 9.90 production would be diverted to producing 99.01 materials to build the Drones.  The same planet with 0.10 resources would only produce 50.00 Drones per turn.</p>
					<p>Excess ship production is carried forward into the next turn.  If the planet continues producing the same ship type, the fractional production is added to the current turn production.  For example, a planet with 750 production and 10.00 resources with no stockpile of materials would produce 1.50 Cruisers per turn.  One Cruiser would be produced on the first turn and two Cruisers would be produced on the second turn.  A ship which is built over several turns will be assigned the technology levels of the race at the start of the final turn.  If the planet's production is changed, the extra ship production is lost and any materials produced on the previous turn are added to the planet's stockpiles.</p>
					
				<h3 id="upgrade">Upgrading Groups</h3>
					<p>Groups can be <a href="#ordg">upgraded</a> at the owning player's planets.  Groups that are upgraded cannot be ordered to <a href="#ords">travel</a> to another planet or <a href="#ordi">intercept</a> an enemy group, however they will be <a href="#ordr">sent on routes</a> and participate in combat.  Ships in the group will be upgraded to the owning player's technology levels as of the beginning of the turn (if they are already at the current technology levels, nothing will happen). The cost of upgrading a ship is equal to a fraction of the production cost of a new ship of the same type.  Ugrades do not require materials.  The exact formula for the cost is:</p>
<pre>Upgrade cost = 10 * ((1 - ship drive tech / current drive tech) * ship drive mass +
(1 - ship weapons tech / current weapons tech) * ship weapons mass +
(1 - ship shields tech / current shields tech) * ship shields mass +
(1 - ship cargo tech / current cargo tech) * ship cargo mass)</pre>
					<p>If the planet does not have enough production to upgrade the entire group, then as many ships as can be completely upgraded will be <a href="#ordb">broken</a> into a new group and upgraded.  Alternatively, if a specific number of ships are ordered upgraded (including 0 to explicitly specify the entire group), exactly that many ships will be upgraded, even if only enough production points are available to do a partial upgrade.  Groups that are partially upgraded have their each technology increased by the percentage of production points available.  The exact formula for a partial upgrade for each technology is:</p>
<pre>Tech increase = (production available / full upgrade cost) * (current tech - ship tech)</pre>
					<p>Production spent on upgrading groups during a turn is deducted from the planet's production for that turn.  Thus, if a planet uses 35% of its production on upgrades, it can only produce 65% as many materials, capital, technology or ships as normal.  If a planet uses its entire production on upgrades, it produces nothing that turn.</p>

			<hr />

			<h2 id="combat">Combat</h2>

				<h3 id="war">War and Alliances</h3>
					<p>At the start of the game all races are assumed to be at war with all the other races (except in team games).  <a href="#orda">Alliances</a> may be declared at the beginning of any turn.  Ships will not initiate combat with allied ships, nor will they bomb allied planets.  However, there is no way to tell if the allied race has also declared an alliance until warships are encountered.  Ships will always shoot back if fired on (battles will be fought just as if both sides were at war with each other; declaring an alliance puts warships at no disadvantage in combat).  Having declared an alliance, <a href="#ordw">war</a> may be declared again at the start of any subsequent turn and vice versa.</p>

				<h3 id="bat">Fighting Battles</h3>
				 <p>Battles occur whenever a ship with weapons encounters an enemy ship at a planet.  Ships in hyperspace cannot participate in battles.  In each battle round, randomly select a ship from all surviving ships that have not yet attacked (note that if a ship is destroyed before it gets a chance to fire it will not attack).  For each gun on the ship, randomly select a surviving enemy ship and fire a shot.  Repeat until all ships have fired.  Battle rounds continue until the battle is either a standoff or a win.  A battle is a standoff if all remaining ships are invulnerable to enemy attacks.  A battle is won if all remaining ships belong to races that are allied with each other.</p>
					<p>Attack and defence strengths are calculated using the following forumlae:</p>
<pre>attack strength = (weapons mass * weapons technology)
defence strength = ((shield mass * shields technology) / (ship mass + effective cargo carried mass)^(1/3)) * 30^(1/3).</pre>
					<p>If a shot is successful, the enemy ship is destroyed.  The attack forumula is:</p>
<pre>p[kill] = (log[4](attack strength / defence strength) + 1) / 2</pre>
					<p>Where log[4](x)  is the log with base 4 of x, which can be computed with log(x)/log(4).  If the attack strength is four times as strong as the defence strength, the attack will always succeed.  If the defence strength is four times as strong as the attack strength, the attack will always fail.  The numbers are calculated so that if a ship type 10 1 10 10 0 fires at an identical ship, it will have a 50% chance of destroying the target.</p>

				<h3 id="bomb">Bombing Planets</h3>	
					<p>After all battles are resolved, ships with weapons bomb enemy planets, reducing population and industry by 75%.  Bombed planets produce capital until ordered to produce something else on later turns.</p>
					<p>If only one race has ships with weapons orbiting a bombed planet, that race becomes the new owner of the planet.  If two or more allied races have ships with weapons orbiting a bombed planet, the race that issued a <a href="#ordv">victory</a> command becomes the new owner.  If no race issued a victory command, the race that appears first in the races table of the turn report receives the planet.  If more than one race issued a victory command, or if there was a standoff, the planet becomes unowned and produces nothing until it is claimed by the first race that unloads colonists at the planet.</p>

			<hr />

			<h2 id="turn">Turn Sequence</h2>

				<ol>
					<li>Planetary <a href="#ordp">production orders</a> are assigned.  Note that production occurs later in the turn.</li>
					<li><a href="#ordA">Messages</a> are sent.</li>
					<li><a href="#orda">Alliances</a> and <a href="#ordw">war</a> are declared.</li>
					<li>Groups with weapons attack enemy ships, causing combat.  This can happen if a player declares war on the current turn.  It can also happen if a player built a ship with weapons at a planet with enemy ships in orbit at the end of the previous turn.</li>
					<li>Groups with weapons bomb enemy planets.  This can happen if a player declares war on the current turn.</li>
					<li>Groups <a href="#ordl">load</a> or <a href="#ordu">unload</a> cargo.</li>
					<li>Groups are <a href="#ordg">upgraded</a>.</li>
					<li>Groups and fleets <a href="#ords">sent</a> to planets enter hyperspace.</li>
					<li><a href="#ordr">Routes are assigned</a> to planets. Cargo ships are assigned cargos and destinations, load cargo (if necessary) and enter hyperspace.</li>
					<li>Groups and fleets with <a href="#ordi">intercept</a> orders are assigned destinations and enter hyperspace.</li>
					<li>Groups and fleets move through hyperspace, possibly arriving at planets.</li>
					<li>Groups with weapons attack enemy ships, causing combat.</li>
					<li>Groups with weapons bomb enemy planets.</li>
					<li>Planets <a href="#ordp">produce</a> materials or capital, conduct research, or build ships.</li>
					<li>Population growth occurs.</li>
					<li>Ships at route destinations unload cargo.  All ships unload cargo if the <a href="#ordo">autounload option</a> is turned on.</li>
					<li>Identical groups are merged.</li>
					<li>Groups are renumbered if the <a href="#ordo">sortgroups</a> option is turned on.</li>
					<li><a href="#ordc">Races</a>, <a href="#ordn">planets</a>, <a href="#ordt">ships</a> and <a href="#ordt">fleets</a> are renamed.</li>
				</ol>
				<p>A number of things follow from this:</p>
				<ul>
					<li>COL, CAP and MAT transported to a planet do not effect the planet's production until the following turn.</li>
					<li>A cargo ship can be loaded, sent to another planet and unloaded in a single turn.</li>
					<li>One ship can bomb two planets in the same turn (though this is rare).</li>
				</ul>

			<hr />

			<h2 id="rep">Turn Report</h2>
				<p>Players receive a report at the beginning of the game and after each turn.  The elements of the text report are described below.  The XML and machine reports provide exactly the same information in different formats.</p>

				<p>Players can obtain copies of current and previous turn reports by sending an email to the GalaxyNG server using this format:</p>
<pre>To: server@somewhere.org
From: player@someplace.com
Subject: report

#GALAXY &lt;game name&gt; &lt;player's race name&gt; &lt;password&gt;  &lt;turn number&gt;</pre>

				<h3 id="repbul">Bulletins</h3>
					<p>The bulletins are provided by the GM.  They usually include a ranked list of races, announcements about the game, holiday notices and other server updates, server instructions, GM contact information, and more.</p>
<pre>Bulletins for Galaxy Game Daily8 Turn 12

Daily8 Top 15 of turn 12 which ran on 18:00:02 Mon Feb 17 2003

 # lt  Nation        Pop   dlt    Ind   dlt   eInd   dlt  tech  dlt   #  dlt
 1   0 Farmers      3607   267   3240   202   3332   218  5.11 0.50  11   0
 2   0 SiliconFun   3504   175   3205   241   3280   224  5.92 0.00  22   4
 3   0 NULLoids     3467   302   2618    60   2830   120  6.60 0.48  14   0
 4   1 Vegetaborg   3695   299   2495   211   2795   233  5.88 0.19  19   0
 5  -1 Valdorians   3562   185   2524   144   2784   154  5.16 0.26  18   0
 6   0 Nation_15    3534   348   2389    49   2675   124  5.06 0.51   6   0
 7   0 Nation_3     3627   267   2265   155   2605   183  4.50 0.00  15   0
 8   0 Santa_s      3402   229   2025    49   2369    94  5.85 0.00  12   0
 9   0 Nation_2     3237   276   2009   108   2316   150  6.01 0.05  13   0
10   0 Outvaders    3127   260   1986   137   2271   168  5.62 0.00   9   0
11   1 Nation_14    2178   168   1831   124   1918   135  4.61 0.21  14   0
12  -1 Tarmangani   2361     4   1648     8   1827     7  5.84 0.12   8   0
13   0 Nation_1        0     0      0     0      0     0  5.03 0.00   0   0
14   0 Nation_10       0     0      0     0      0     0  4.32 0.00   0   0
15   0 Nation_11       0     0      0     0      0     0  4.32 0.00   0   0

***********************************************************************
  o     o    +   .  +    o       +      +  o       o           o
    o      -   ______ o    __        +   -  + _   ________   o    +  o
 o  .         / ____/___ _/ /___ __  ____  __/ | / / ____/         +
      .*  -  / / __/ __ `/ / __ `/ |/_/ / / /  |/ / / __      +
   *        / /_/ / /_/ / / /_/ /&gt;  &lt;/ /_/ / /|  / /_/ / o    -    -
    .  o    \____/\__,_/_/\__,_/_/|_|\__, /_/ |_/\____/ .    -  o     o
     *           . +        o       /____/   +   +          -
 o         o          +       -                    o         -     o
   .  -        http://www.badcheese.com/~steve/pbm.shtml       +
***********************************************************************

-*- SENDING ORDERS -*-

Orders should be send to:   steve@badcheese.com
with a subject line of:     "order" or "orders"
All orders should start with:
  #GALAXY &lt;game name&gt; &lt;your nation name&gt; &lt;your password&gt; &lt;turn number&gt;
and end with:
  #END

The game name is case sensitive, so use Jangi and not jangi.  Please
send in orders in time. Even empty orders will do.  If you miss 2
turns in a row during the first 12 turns your nation self destructs,
and that is not a pretty sight.  After turn 12 you can miss 5 turns in
a row before your nation self-destructs.  If you fail to send in your
turn0 or turn1 orders, your nation will be reset.  NOTE: It's better
to send in blank orders, than no orders at all - especially in the
beginning turns of the game.  The beginning orders are really easy
anyway, so there's no reason for people to be kicked out of the game
for any reason other than they weren't serious about playing the game
in the first place.

You can send in advance orders by adding the turn number to subject
line. For instance, to send in orders for turn 55, use a subject line
of "orders 55".  Advance orders are not checked, you only get a
confirmation that they were stored on disk. Also do _NOT_ change
your password or your nation name right before sending in advanced orders.
The engine will then not accept the orders once the advance turn is
run unless you remember to use your new password in the advance orders
too.

You can send in orders as many times as you please. The last set of
valid orders is used when the turn is run.  You should get an almost
immediate reply to your email. The message contains a report on any
mistakes you made in your orders and a short forecast about your
situation in the next turn.  Not everything will be reflected in the
forecast of course - things like battles and the other players'
decisions will affect your final turn report when the turns are run.

PLEASE SEND ORDERS AND RELAYS IN PLAINTEXT ONLY!!!  NO HTML!!!
Instructions for turning off HTML in most mail clients are here:
http://www.expita.com/nomime.html

-*- GETTING A COPY OF A TURN REPORT -*-

You can get a copy of any of the previous turn reports by sending a
set of orders that contains just one line:
  #GALAXY &lt;game name&gt; &lt;your nation name&gt; &lt;your password&gt;  &lt;turn number&gt;
to steve@badcheese.com with a Subject line of "report"
Use the nation name and password of the last turn, not the
name and password you had at the turn you request.

-*- ANONYMOUS IN-GAME EMAIL -*-

You can send a anonymous email to another nation by sending a
message with the following format
  #GALAXY &lt;game name&gt; &lt;your nation name&gt; &lt;your password&gt;
  &lt;text of your message&gt;
  .
  .
to steve@badcheese.com with as Subject: relay &lt;destination nation name&gt;
If you get a message like this you can just hit the reply button
and write and send the reply. For your convenience the
#GALAXY... line is included in the message, you probably have
to remove the "&gt; " your mail program added in front of it.
Be careful with signatures if you want to stay anonymous.

If you want to relay a message to all players in the game, rather than
to a single player, the subject of your message should be
"relay &lt;gamename&gt;" - thus, to relay to all players in the
game Distance the subject should be "relay Distance"

-*- TIME AND DUE DATES FOR ORDERS -*-

Most turns get run at midnight "Mountain" time or "Colorado" time
(That's GMT-7 or GMT-6 depending on the "Daylight time") except the
"Daily" games which are run at 6pm every day.  For turn frequency
and the official galaxy engine clock, please visit the web page (below).

-*- OTHER INFORMATION -*-

Game Stats:                     http://badcheese.com/~steve/pbm.shtml
For information about GalaxyNG:       http://galaxyng.sourceforge.net
The manual is at:    http://galaxyng.sourceforge.net/ManGalaxyNG.html

-*- BUGS and Other Problems -*-

Send bug reports about the server to steve@badcheese.com.
Send bug reports about the game engine to fslothouber@acm.org.

For all "how do I ..." questions, see the documentation or send them
to the mailinglist: galaxy@badcheese.com.

-*- ENDING A GAME -*-

To end a game I need an email from all remaining players, saying they
agree to end the game.  The result will be entered into the Hall of
Fame and Shame, and an announcement will be made on galaxy@badcheese.com
mailing list.

    ____        _ __      ____ 
   / __ \____ _(_) /_  __( __ )
  / / / / __ `/ / / / / / __  |
 / /_/ / /_/ / / / /_/ / /_/ / 
/_____/\__,_/_/_/\__, /\____/  
                /____/         
See http://badcheese.com/~steve/index.php?page_title=Galaxy for
game runtimes and frequency.


		Production Status
Sofar you have:
  produced a total shipmass of 1118.42
  lost a total shipmass of     0.00


		Real Name
  Your real name is none.


End of the Bulletins</pre>

				<h3 id="repglo">Global Messages</h3>
					<p>All global messages are reported, including those sent by the player.</p>
<pre>GLOBAL Messages
-*-*-*-
Nation_10 had an unfortunate accident and was obliterated.
-*-*-*-

-*-*-*-
Nation_11 had an unfortunate accident and was obliterated.
-*-*-*-

-message starts-
Heya! Hello Everyone! 
-message ends-
-message starts-
AND SO IT BEGINS..... 
-message ends-
-message starts-
A long time ago in a Galaxy .... 
 ... Game just like this, there lived a quiet unassuming race 
called the Fluffy_Hobbits. They stretched out from their 
homeworld of Bag_End across the void to nearby planets 
and were mercilessly squashed by Obsidian and Xenox. 
So they changed their name to NSF_Hobbits (Not So 
Fluffy) and moved to a new galaxy.  Now, a few galaxies 
later, we are in our new home and would like to say a 
big hello to all intelligent races out there. 
 
Incidentally NSF_Hobbits are being played from England, 
so bear this in mind if you need to contact me close to the 
deadline (which is 7AM(ish) my time). 

Stuart (Supreme Hobbit) 

-message ends-</pre>

				<h3 id="repper">Personal Messages</h3>
					<p>Personal messages received from other players are reported.</p>
<pre>PERSONAL Messages for Farmers

-message starts-
The Vegetaborg send our regards. 
-message ends-</pre>

				<h3 id="repopt">Your Options</h3>
					<p>The status of all the player's options (on | off) is reported.</p>
<pre>		Your Options

N                 S  
Anonymous        ON  
AutoUnload       ON  
ProdTable        ON  
SortGroups       ON  
GroupForeCast    ON  
PlanetForeCast   ON  
ShipTypeForecast ON  
RoutesForecast   ON  
Compress         OFF 
Gplus            OFF 
MachineReport    OFF 
BattleProtocol   OFF 
XMLReport        OFF</pre>
					
				<h3 id="repord">Orders Received</h3>
					<p>The final set of orders from the player that were received by the server is reported.</p>
<pre>ORDERS RECEIVED

> L 2 COL 2 
> S MAX 105 
> L 2 COL 1 
> S MAX 51 
> L 1 COL 1 
> S MAX 51 
> L 1 COL 2 
> S MAX 223 
> L 1 COL 2 
> S MAX 133 
> L 1 COL 2 
> S MAX 95 
> L 1 COL 2 
> S MAX 194 
> L 3 COL 2 
> S MAX 180 
> L 3 COL 2 
> S MAX 236 
> L 1 COL 1 
> S MAX 236 
> S 1 Peas 29 
> S 3 Peas 9 
> P Peas CAP 
> D Cargo3 69.98 0 0.00 0.00 29.02 
> P Corn Cargo3 
> P Carrots CAP
> @ Tarmangani
> Howdy, 
>  
> Always nice to see someone willing to take the time to name themselves and  
> their planets. 
>  
> Seems Nation_3, a race lacking in imagination, is between us.  Interested  
> in future collaboration on that front? 
>  
> Farmer </pre>

				<h3 id="repmis">Mistakes</h3>
					<p>Errors resulting from the player's orders are reported.</p>
<pre>MISTAKES

P 659 Drive
Planet is not owned by you.
S 53 42
Group is in hyperspace.
G 509 
Planet has no remaining industry.</pre>

				<h3 id="repstat">Status of Players</h3>
					<p>The status of all players is reported: race name, drive technology, weapons technology, shields technology, cargo technology, total population, total industry, number of inhabited planets and alliance/war.</p>
<pre>		Status of Players

N              D    W    S    C     P       I     #  R  
Nation_1     1.00 2.03 1.00 1.00    0.00    0.00  0 War 
Nation_2     2.84 1.05 1.12 1.00 3237.40 2008.65 13 War 
Nation_3     1.50 1.00 1.00 1.00 3626.57 2265.05 15 War 
Valdorians   2.16 1.00 1.00 1.00 3562.07 2524.05 18 War 
SiliconFungi 2.92 1.00 1.00 1.00 3504.22 3204.68 22 War 
Santa_s      2.85 1.00 1.00 1.00 3401.92 2024.78 12 War 
NULLoids     1.60 1.48 2.28 1.24 3467.43 2618.08 14 War 
Tarmangani   2.17 1.67 1.00 1.00 2361.15 1648.30  8 War 
Farmers      2.09 1.00 1.01 1.00 3607.00 3239.78 11 -   
Nation_10    1.32 1.00 1.00 1.00    0.00    0.00  0 War 
Nation_11    1.32 1.00 1.00 1.00    0.00    0.00  0 War 
Vegetaborg   2.31 1.58 1.00 1.00 3694.72 2495.30 19 War 
Outvaders    1.47 1.20 1.95 1.00 3126.61 1985.52  9 War 
Nation_14    1.61 1.00 1.00 1.00 2178.18 1831.18 14 War 
Nation_15    1.55 1.51 1.00 1.00 3534.16 2388.89  6 War</pre>

				<h3 id="repst">Your Ship Types</h3>
					<p>All the player's ship types are reported: ship type name, drive mass, attacks, weapons mass, shields mass, cargo mass, ship mass, speed (at drive technology 1.00 with no cargo) and effective defence (at shields technology 1.00).</p>
<pre>		Your Ship Types

N            D    A   W     S     C    Mass Speed  Def  
Cargo0      1.02  0  0.00  0.00  1.00  2.02 10.10  0.00 
Cargo1      1.03  0  0.00  0.00  1.00  2.03 10.15  0.00 
Cargo2      1.06  0  0.00  0.00  1.00  2.06 10.29  0.00 
Cargo3     69.98  0  0.00  0.00 29.02 99.00 14.14  0.00 
Rake0      33.00 29  2.00 35.00  1.00 99.00  6.67 23.51 
Axe0       33.00  3 15.00 35.00  1.00 99.00  6.67 23.51 
Flak0       1.00  0  0.00  2.00  0.00  3.00  6.67  4.31 
Scarecrow0  1.00  1  5.00  3.00  0.00  9.00  2.22  4.48 
Seed        1.00  0  0.00  0.00  0.00  1.00 20.00  0.00 
Rake1      28.88 40  2.00 15.75  1.00 86.63  6.67 11.06 
Axe1       27.84  1 35.00 19.66  1.00 83.50  6.67 13.98 
Axe3        8.92  1 10.00  7.83  0.00 26.75  6.67  8.14 
Rake2      43.32 25  2.00 16.31  1.00 86.63 10.00 11.45 
Axe2       41.75  3 12.00 16.75  1.00 83.50 10.00 11.91 
Flak1       1.00  0  0.00  1.00  0.00  2.00 10.00  2.47 
Scarecrow1  8.91  1  3.00  5.91  0.00 17.82 10.00  7.03 
Scarecrow3  2.00  1  4.00  4.70  0.00 10.70  3.74  6.63</pre>

				<h3 id="repast">Alien Ship Types</h3>
					<p>Other races' ship types observed during the turn are reported.  For each race: ship type name, drive mass, attacks, weapons mass, shields mass, cargo mass, ship mass, speed (at drive technology 1.00 with no cargo) and effective defence (at shields technology 1.00).</p>
<pre>		SiliconFungi Ship Types

N        D   A   W    S    C   Mass Speed  Def 
Drone   1.00 0 0.00 0.00 0.00  1.00 20.00 0.00 
ATT_F1  2.50 1 1.50 1.50 0.00  5.50  9.09 2.64 
ATT_F3 12.25 3 3.00 6.00 0.00 24.25 10.10 6.44 

		NULLoids Ship Types

N    D  A   W    S    C  Mass Speed  Def 
F1 1.00 1 1.00 1.00 1.00 4.00  5.00 1.96 

		Vegetaborg Ship Types

N                 D   A   W     S    C   Mass Speed  Def 
Onion-24.75     0.00  3  8.25 8.25 0.00 24.75  0.00 8.80 
Garlic          0.00  1  1.00 0.00 0.00  1.00  0.00 0.00 
Onion-25.47     0.00  3  8.49 8.49 0.00 25.47  0.00 8.97 
Onion-23.79     0.00  3  7.93 7.93 0.00 23.79  0.00 8.57 
Onion-23.61     0.00  3  7.87 7.87 0.00 23.61  0.00 8.52 
Artichoke-24.75 0.00 16  1.94 8.26 0.00 24.75  0.00 8.81 
Mushroom-23.61  0.00  1 18.89 4.72 0.00 23.61  0.00 5.11 
Mushroom-32.47  0.00  1 25.98 6.49 0.00 32.47  0.00 6.32 
Mushroom-24.75  0.00  1 19.80 4.95 0.00 24.75  0.00 5.28 
Mushroom-27.18  0.00  1 21.74 5.44 0.00 27.18  0.00 5.62 </pre>

				<h3 id="repbat">Battles</h3>
					<p>All battles observed during the turn are reported.  For each group in each battle: number of ships participating in battle, ship type name, drive technology, weapons technology, shields technology, cargo technology, cargo type, cargo quantity per ship, number of surviving ships.</p>
<pre>		Battle at 22

		Your Groups

#      T     D    W    S    C  T   Q  L 
  1 Axe3   7.11 3.44 4.50 0.00 - 0.00 0 
100 Flak1  7.11 0.00 4.50 0.00 - 0.00 0 
  1 Rake10 7.11 3.44 4.50 0.00 - 0.00 0 
  1 Axe9   7.11 3.44 4.50 0.00 - 0.00 0 

		Nation_15 Groups

#     T       D    W    S    C  T   Q  L 
2 Cargo_Pod 4.56 0.00 0.00 1.00 - 0.00 0 
4 Cargo_Orb 4.98 0.00 0.00 1.00 - 0.00 0 
3 Cargo_Orb 4.00 0.00 0.00 1.00 - 0.00 0 
3 Probe     3.14 0.00 0.00 0.00 - 0.00 0 
1 Cargo_Pod 2.70 0.00 0.00 1.00 - 0.00 0 
1 Beam      4.98 3.50 3.52 0.00 - 0.00 0 
1 Sunlight  4.00 2.76 2.76 0.00 - 0.00 0 
1 Rainbow   0.00 3.50 4.10 0.00 - 0.00 1 
5 Spark     3.14 0.00 2.00 0.00 - 0.00 0</pre>
					<p>If the battleprotocol option is on, for each battle, for each shot fired: attacking ship type name, defending ship type name, result (shields | destroyed).</p>
<pre>		Battle Protocol

Farmers Axe9 fires on Nation_15 Cargo_Orb : Destroyed
Nation_15 Beam fires on Farmers Flak1 : Shields
Nation_15 Beam fires on Farmers Flak1 : Shields
Nation_15 Beam fires on Farmers Flak1 : Destroyed
Nation_15 Beam fires on Farmers Flak1 : Destroyed
Nation_15 Beam fires on Farmers Flak1 : Shields
Nation_15 Beam fires on Farmers Flak1 : Shields
Nation_15 Sunlight fires on Farmers Flak1 : Destroyed
Nation_15 Rainbow fires on Farmers Flak1 : Destroyed
Farmers Rake10 fires on Nation_15 Beam : Shields
Farmers Rake10 fires on Nation_15 Cargo_Orb : Destroyed
Farmers Rake10 fires on Nation_15 Cargo_Pod : Destroyed
Farmers Rake10 fires on Nation_15 Beam : Shields

...hundreds of lines snipped...

Nation_15 Rainbow fires on Farmers Flak1 : Destroyed
Nation_15 Rainbow fires on Farmers Flak1 : Destroyed
Nation_15 Rainbow fires on Farmers Flak1 : Destroyed
Nation_15 Rainbow fires on Farmers Flak1 : Destroyed
Nation_15 Rainbow fires on Farmers Flak1 : Destroyed
Nation_15 Rainbow fires on Farmers Flak1 : Destroyed
Nation_15 Rainbow fires on Farmers Flak1 : Destroyed</pre>

				<h3 id="repbom">Bombings</h3>
					<p>All bombings observed during the turn are reported.  For each bombing: new owner, previous owner, planet name, previous population, previous industry, what the planet was ordered to produce that turn and capital, material and colonist stockpiles.</p>
<pre>		Bombings

W                 O         N        P      I      P      $      M     C   
SiliconFungi Valdorians Zephir    134.22  62.69 Weapons  0.00   0.00  9.71 
SiliconFungi The_Grinch 62         30.92  20.07 Shields  0.00   0.00  0.00 
Farmers      Valdorians 90        528.09 355.29 Probe    0.00   0.00  0.00 
Farmers      Valdorians 232        53.53  53.53 Weapons 17.56   0.00  0.00 
Farmers      Santa_s    225         9.50   0.40 CAP      0.00   0.00  0.00 
Farmers      Valdorians 198        28.50  22.68 Weapons  0.00   0.00  0.00 
Farmers      Nation_15  98        745.52 745.52 Spark   29.37   0.00 35.36 
Farmers      Nation_15  246       600.61 600.61 Probe    1.37 119.96 18.02 
Farmers      Nation_15  Arrowroot  79.39  12.07 Shields  0.00   0.00  0.00 
Nation_15    Farmers    205       211.21 211.21 Seed    38.06   0.00  0.00</pre>

				<h3 id="repmap">Map</h3>
					<p>An text map of the galaxy or a section of the galaxy.  The player can change the size and extent of the map.  Symbols on the map have the following meanings:
<br />* The player's planet
<br />o An uninhabited planet
<br />+ Another race's planet
<br />. The player's group in hyperspace
<br />- Another race's group in hyperspace</p>

<pre>50.00,0.00  200.00,0.00
--------------------------------------------------------------------------------
                                                                         *      
                                                                                
                                                                                
                                   *                     *                      
                                                                                
               **   *                    *                                   *  
            *                  *                                                
                 * *                                                        *   
                *** *               *                                    *      
               ** **                                                            
            *            *               *            *                         
                                                            *         *         
                                            *                           *   * * 
               *                        *        * *                            
                                           *   *                                
        *                        *           .* *   * *                      *  
 *..                  *                       .          *                      
. .                                               **                           
.                       *              .                      .      *          
  .                                          *                .                 
           ..                               *                                   
                                                                                
                                                                    +           
                            ..                        *         +   +   +       
                         *                                                      
*                                      *    *                      ++    *      
                                 * **                             -++ .         
                           +     * **                            - *+           
                            *  **   *                          + ++         o   
    **                                                             +            
      **                     *      *                                           
       ****                                                                     
          *                *                                                    
        * *  *                                                                  
                                                                  +             
             *                           .         +  +                         
*                           *                                                   
                                                 ++                             
        *     o                 *                  ++                           
                                                  +  +                          
--------------------------------------------------------------------------------
50.00,150.00  200.00,150.00</pre>

				<h3 id="repinc">Incoming Groups</h3>
					<p>All alien ships in hyperspace travelling toward the player's planets.  Multiple groups that have the same origin planet, destination planet, distance remaining and speed (e.g. a fleet) will be reported collectively as a single group.  For each incoming group: origin planet, destination planet, distance remaining, speed and total mass.</p>
<pre>		Incoming Groups

O    D      R     S      M   
73  188   34.19 21.02  19.80
383 Apple 18.02 24.33 284.33</pre>

				<h3 id="reppla">Your Planets</h3>
					<p>All planets owned by the player.  For each planet: planet name, x coordinate, y coordinate, size, population, industry, resources, production, capital stockpile, material stockpile, colonist stockpile, effective industry.</p>
<pre>		Your Planets

N                     X      Y      S       P       I      R        P        $      M      C      L    
Peach               74.16  23.86  464.15  464.15  464.15  1.65 Rake         0.00    0.28  5.85  464.15 
Daisy              133.09  46.93  831.34  831.34  831.34  7.50 CAP          2.78    0.00  8.31  831.34 
Marigolds           79.58  51.29   99.82   99.82   99.82  0.59 Shields      2.01    0.00  1.00   99.82 
Curry              208.47  64.46   61.94   11.09    1.32  0.92 Cargo0       0.00    0.00  0.00    3.76 
14                 194.38  20.04    5.90    5.90    0.87  1.28 CAP          0.00    0.00  0.18    2.13 
17                 177.32 101.70  442.38    8.80    0.00  0.61 CAP          0.00    0.00  0.00    2.20 
Lima_Bean           79.61  19.20  501.30  501.30  501.30  1.31 Weapons      0.11    0.81 19.82  501.30 
Brush              163.97  41.69   61.14   17.59    5.68  1.73 Drive        0.00    0.00  0.00    8.66 </pre>
				
				<h3 id="repsip">Ships In Production</h3>
					<p>All planets owned by the player that are producing ships.  If no ships are being produced, or if the ProdTable option is turned off, Ships in Production will not be reported.  For each planet: planet name, ship type name, production cost, excess ship production.</p>
<pre>		Ships In Production

N          P       N     U   
Aster Scarecrow0 90.90  1.21 
141   Seed       10.18  5.76 
Curry Cargo0     21.82 10.06 
Corn  Scarecrow0 90.90  1.36 
Flax  Cargo4     23.35  7.70 
296   Seed       10.00  3.60</pre>

				<h3 id="reproute">Your Routes</h3>
					<p>All the player's planets that have routes set.  For each planet: capital route destination planet, materials route destination planet, colonists route destination planet and empty route destination plant.  A - (hyphen) indicates no route is set for the specified commodity.</p>
<pre>		Your Routes

N            $         M C                E          
Peach        Soy       - Soy              -          
Daisy        -         - -                Broccoli   
Lima_Bean    -         - Monkey_Grass     -          
Apricot      -         - -                Grapefruit 
Aster        -         - -                Grapefruit 
Dogwood      -         - -                Canteloupe 
Grape        -         - Soy              -          
Daylilly     -         - Foxglove         Grapefruit</pre>

				<h3 id="repapla">Alien Planets</h3>
					<p>All planets owned by other players at which the player has a ship in orbit.  For each race, for each planet: planet name, x coordinate, y coordinate, size, population, industry, resources, production, capital stockpile, material stockpile, colonist stockpile, effective industry.</p>
<pre>		SiliconFungi Planets

N         X      Y      S     P     I     R   P     $     M    C    L   
62     116.24 200.05  66.83  8.35  6.05 1.99 CAP   0.00 0.00 0.00  6.63 
73     128.73 205.75 706.09  5.51  5.51 4.96 CAP 108.04 0.00 0.00  5.51 
80     141.26 203.61 733.01 84.80 62.95 3.51 CAP   0.00 0.00 0.00 68.41 
Zephir  29.96 218.01 134.22 36.24 19.12 1.19 CAP   0.00 0.00 0.00 23.40 
178    128.98 206.46 501.09 67.61 31.16 0.73 CAP   0.00 0.00 0.00 40.27 

		NULLoids Planets

N     X     Y     S    P    I    R   P    $    M    C    L  
37 227.09 71.09 2.77 2.77 2.77 0.69 CAP 0.59 0.00 1.41 2.77 
88 222.13 79.56 9.23 9.23 9.23 0.60 CAP 2.76 0.00 1.59 9.23 

		Outvaders Planets

N       X      Y      S     P     I    R   P    $    M    C    L   
C103 145.15 141.84 770.36 25.48 9.29 6.34 CAP 0.00 0.00 0.00 13.34</pre>

				<h3 id="repupla">Unidentified Planets</h3>
					<p>All planets owned by other players at which the player does not have a ships\ in orbit.  For each planet: planet name, x coordinate, y coordinate.</p>
<pre>		Unidentified Planets

N           X      Y   
C1       145.53 141.86 
C2       145.70 144.83 
C3       147.85 143.21 
6         91.88 222.99 
7         80.39 225.80 
9         58.75 214.28 
C12      172.14 106.54 
15        79.33 165.86 
16        89.27 176.56 
22        67.28 118.70 
NULL25   230.03 108.78</pre>

				<h3 id="repunpla">Uninhabited Planets</h3>
					<p>All uninhabited planets.  For each planet: planet name, x coordinate, y coordinate.  If the player has a ship in orbit at the planet, size and resources will also be given.</p>
<pre>		Uninhabited Planets

N      X      Y      S     R     $    M  
10   35.63  86.87                        
11   11.52 111.02
20  192.69 107.48  28.16  0.90 0.00 0.00 
31    7.81 103.75
36   14.30 107.37
41    9.77 143.99 130.33  0.55 0.00 0.00 
68    4.16 116.78 456.46  1.03 0.00 0.00</pre>

				<h3 id="repgroups">Your Groups</h3>
					<p>All groups owned by the player which are not part of a fleet.  For each group: group number, number of ships, ship type name, drive technology, weapons technology, shields technology, cargo technology, cargo type carried, cargo mass carried per ship, destination planet, distance to destination, origin planet.</p>
<pre>		Your Groups

G    #       T       D    W    S    C   T    Q           D           R        O     
  1   1 Scarecrow0 7.11 3.44 4.50 0.00 -   0.00 Peach                               
  2   1 Seed       1.40 0.00 0.00 0.00 -   0.00 Peach                               
  3   3 Cargo1     7.11 0.00 0.00 1.00 -   0.00 Peach                               
  4   1 Cargo2     7.11 0.00 0.00 1.00 -   0.00 Peach                               

...snip...
                     
 15   1 Seed       1.40 0.00 0.00 0.00 -   0.00 Lima_Bean                           
 16  22 Cargo1     7.11 0.00 0.00 1.00 -   0.00 Lima_Bean                           
 17   2 Cargo2     7.11 0.00 0.00 1.00 -   0.00 Lima_Bean                           
 18   1 Scarecrow0 7.11 3.44 4.50 0.00 -   0.00 Lima_Bean                           
 19   1 Flak0      7.11 0.00 4.50 0.00 -   0.00 Lima_Bean                           
 20   2 Cargo0     7.11 0.00 0.00 1.00 -   0.00 Lima_Bean                           

...snip...
 
 34   1 Axe2       7.11 3.44 4.50 1.00 -   0.00 Aster                               
 35   1 Scarecrow3 7.11 3.44 4.50 0.00 -   0.00 Aster                               
 36   1 Flak0      7.11 0.00 4.50 0.00 -   0.00 Aster                               
 37   1 Rake4      7.11 3.44 4.50 1.00 -   0.00 Aster                               
 38   1 Axe7       7.11 3.44 4.50 1.00 -   0.00 Aster                               
 39   2 Rake3      7.11 3.44 4.50 1.00 -   0.00 Aster                               
 40  65 Flak1      7.11 0.00 4.50 0.00 -   0.00 Aster                               
 41   1 Rake2      7.11 3.44 4.50 1.00 -   0.00 Aster                               
 42   1 Cargo1     5.74 0.00 0.00 1.00 -   0.00 Aster                               
 43   1 Rake7      7.11 3.44 4.50 0.00 -   0.00 Aster                               
 44   1 Axe11      7.11 3.44 4.50 0.00 -   0.00 Aster                               
 45   1 Axe14      7.11 3.44 4.50 0.00 -   0.00 Aster                               
 46  24 Flak1      7.11 0.00 4.59 0.00 -   0.00 Aster                               
 47  11 Scarecrow0 7.11 3.44 4.60 0.00 -   0.00 Aster                               
 48   1 Seed       4.60 0.00 0.00 0.00 -   0.00 Foxglove                            

...snip...
   
 67   1 Flak1      7.11 0.00 4.50 0.00 -   0.00 59                                  
 68   1 Seed       7.11 0.00 0.00 0.00 -   0.00 59                                  
 69   1 Seed       4.60 0.00 0.00 0.00 -   0.00 Hops                                
 70  11 Cargo0     7.11 0.00 0.00 1.00 COL 1.10 Hops               15.36 Lima_Bean  
 71   2 Cargo1     7.11 0.00 0.00 1.00 COL 1.10 Hops               16.20 Apple      
 72   5 Cargo0     7.11 0.00 0.00 1.00 COL 1.10 Hops               16.51 Apple</pre>
 
				<h3 id="repfleets">Your Fleets</h3>
					<p>All fleets owned by the player.  For each fleet: fleet name, fleet speed.  For each group within a fleet: group number, number of ships, ship type name, drive technology, weapons technology, shields technology, cargo technology, cargo type carried, cargo mass carried per ship, destination planet, distance to destination, origin planet.</p>
<pre>		Fleet Defenders (speed 6.67)

G   #   T     D    W    S    C  T   Q    D  R O 
64  1 Axe0  1.00 1.00 1.00 1.00 - 0.00 Corn     
65  1 Rake0 1.00 1.00 1.00 1.00 - 0.00 Corn     
66 41 Flak0 1.00 0.00 1.00 0.00 - 0.00 Corn     
67  9 Flak0 1.40 0.00 1.00 0.00 - 0.00 Corn     
68  9 Flak0 1.60 0.00 1.00 0.00 - 0.00 Corn     
69  5 Flak0 1.20 0.00 1.00 0.00 - 0.00 Corn</pre>

				<h3 id="repagroups">Alien Groups</h3>
					<p>All groups owned by other players that are at planets where the player has a ship in orbit.  For each group: number of ships, ship type name, drive technology, weapons technology, shields technology, cargo technology, cargo type carried, cargo mass carried per ship, destination (i.e. current) planet.</p>		
<pre>		SiliconFungi Groups

#     T     D    W    S    C  T   Q     D   
 2 ATT_F1 6.18 3.28 2.88 0.00 - 0.00 167    
 1 ATT_F3 6.18 3.28 2.88 0.00 - 0.00 Zephir 
 1 ATT_F3 6.18 4.62 4.64 0.00 - 0.00 62     
11 Drone  6.18 0.00 0.00 0.00 - 0.00 62     
11 Drone  6.18 0.00 0.00 0.00 - 0.00 178    
 1 Drone  6.18 0.00 0.00 0.00 - 0.00 118    
 1 ATT_F3 6.18 4.62 4.64 0.00 - 0.00 178    

		NULLoids Groups

#  T   D    W    S    C  T   Q   D 
1 F1 1.60 1.48 2.28 1.24 - 0.00 37 
1 F1 1.60 1.48 2.28 1.24 - 0.00 88 

		Outvaders Groups

#       T         D    W    S    C  T   Q    D  
1 1_0_0_0_0     1.00 0.00 0.00 0.00 - 0.00 17   
1 1_0_0_0_0     1.00 0.00 0.00 0.00 - 0.00 20   
2 10_10_10_10_0 3.23 1.45 2.61 0.00 - 0.00 C103 
2 10_10_10_10_0 3.35 1.45 2.61 0.00 - 0.00 C103 
2 10_10_10_10_0 3.63 1.45 2.61 0.00 - 0.00 20   
2 10_10_10_10_0 3.84 1.45 2.61 0.00 - 0.00 20</pre>

			<hr />

			<h2 id="orders">Orders</h2>
				<p>Players command their race's activities by sending orders to the Galaxy server.  Players can only issue orders for their own groups and planets.</p>
				<p>In the orders documentation below, parameters are indicated as follows:</p>
<pre>&lt;required&gt;
[optional]
option1 | option2 | option3</pre>
				<p>When orders conflict, such as two races trying to unload colonists onto an uninhabited planet simultaneously at the start of the turn, whichever race is first in the list of races will have its order executed first and colonize the planet. In practice this does not happen often enough to be relevant.</p>
				<p>Because names are changed at the end of the turn, entities which are renamed in a turn must be referred to by their old names for all orders issued during the turn.</p>

				<h3 id="send">Sending Orders</h3>
					<p>Players send orders to the GalaxyNG server using this format:</p>
<pre>To: server@somewhere.org
From: player@someplace.com
Subject: orders

#GALAXY &lt;game name&gt; &lt;player's race name&gt; &lt;password&gt;  &lt;turn number&gt;
...orders...
#END</pre>

					<p>The subject should be "orders" and the #GALAXY line should be in the body of the message, not in the subject.  It is very important to type this line correctly, as otherwise all of your orders will be discarded.  Everything before the #GALAXY line or after the #END line in the message is ignored by the server.  Replacement sets of orders may be sent anytime before the turn deadline. The last set of orders received will be used by the server when the turn is run.</p>
					<p>Players can send empty orders (e.g. #GALAXY and #END lines with no contents) to avoid being removed from the game for <a href="#inactive">inactivity</a>.</p>
					<p>If the GM chooses, the game can run when all players have submitted their orders or when a certain number of hours have passed since the last turn ran.  To send final orders and indicate you are ready for the turn to run, change the #GALAXY line to read:</p>
					<pre>#GALAXY &lt;game name&gt; &lt;player's race name&gt; &lt;password&gt; &lt;turn number&gt; FinalOrders</pre>
					<p>If all players submit final orders, the turn will run immediately.  If not, it will run when # hours have passed since the previous turn.  For example, the GM could run the turn when all players have submitted final orders or when 48 hours have passed since the previous turn.</p>
     
				<h3 id="fore">Forecast and Errors</h3>
					<p>The server sends a forecast and error report to the email address used to send orders.  The forecast is a prediction of the results of the orders sent, assuming no other races exist in the galaxy.  It contains only information about the player's planets and groups, and does not contain information about other races or uninhabited planets.  Errors with the email or with specific orders will be reported to the player so they can be corrected and resent.</p>

				<h3 id="adv">Advance Orders</h3>
					<p>Players can send advance orders if they will be away from email for some time.  Simply change the turn number on the #GALAXY line to the appropriate turn.  The server will store the orders (but not check them) and use them when the turn runs.  When sending advance orders, players should <strong>not</strong> change their <a href="#ordn">race name</a> or <a href="#ordy">password</a>, as this can cause orders to be rejected when the turn runs.</p>

				<h3 id="inactive">Inactivity</h3>
					<p>Players are removed from the game if they do not send orders for the first turn, do not send orders for three turns in a row during the first ten turns, or do not send in orders for six turns in a row after turn ten.  Players are also removed from the game if they do not own any planets and do not send in orders on a single turn.  Planets are reduced to 0 population and industry and all ships are destroyed at the beginning of the turn in which the player is removed.  (Note that players can send empty orders and <a href="#adv">advance orders</a> to avoid being removed for inactivity.)</p>

				<h3 id="ex">Example Set of Orders</h3>
					<p>Writing orders for the first time can be a daunting task, so here is an example set of orders for turn 1.</p>
<pre>#GALAXY Jangi race_5 P981231231 1
c Vogons           ; Change the player's race name.
y towel            ; Change password.
= "Arthur Dent"    ; Set player name for Hall of Fame.
n 67 Vogonia       ; Name first home world.
n 68 Ford          ; Name second home world.
n 69 Prefect       ; Name third home world.
d Haul 2 0 0 0 1   ; Design a cargo ship.
p 67 Haul          ; Produce cargo ships at the first home planet.
                   ; Notice that the old name of the planet is used
                   ; as the name change won't go in effect until the
                   ; end of the turn.
p 68 DRIVE         ; Research drive technology at the second planet.
d Probe 1 0 0 0 0  ; Design a ship for scouting.
p 69 Probe         ; Produce scouting ships at the third planet.
#END</pre>

				<h3 id="form">Order Format</h3>
					<p>Each order is designated by giving a letter or keyword as the first non-blank character on the line, followed by a space.  The program only checks the first letter, so you can either give a whole word or just the letter.  Parameters follow the order, separated by spaces or tabs.  Blank lines are permitted, as are comments; anything (except <a href="#ordA">messages</a>) after a semicolon on a line is treated as a comment and ignored.</p>
					<p>Parameter keywords must be given precisely; unlike orders they cannot be abbreviated or expanded.  The following parameter keywords are used: AMOUNT, CAP, COL, EMP, MAT, DRIVE, WEAPONS, SHIELDS, CARGO, MAX, FLEET.</p>
					<p>All orders involving groups can take an optional last parameter giving the number of ships to be used. If this parameter is given, the indicated number of ships will be <a href="#ordb">broken</a> off into a separate group first, and the order applied only to that separate group.  If the parameter is omitted or if it is 0 (zero), the order will be applied to the entire group.</p>
					<p>The parser is not case sensitive, so all commands and names may be given in upper case, lower case or a mixture of the two. However, when supplying names containing spaces, underscore characters must be used in place of spaces in the name.  Examples of correct order lines:</p>
<pre>Send 100 Zzyax_Prime
send 100 zzyax_prime
s 100 zzyax_prime	; Attack the Zzyaxian homeplanet</pre>
					<p>Examples of incorrect order lines:</p>
<pre>Send 100 Zzyax Prime
s 100 "zzyax prime"	Attack the Zzyaxian homeplanet</pre>

				<h3 id="ordA">@ [alien race name ...]</h3>
					<p>Send a message to one or more players within their turn reports.  All lines between the order and the next line with an @ symbol as its first non-blank character are treated as message text.  Semicolons between the @ symbols are treated as part of the message, and not as comments.  The server does not indicate the sender, so unsigned messages are anonymous and messages can be forged.  Note that races usually send each other <a href="#email">email</a> if they want to verify the authenticity of their messages.  The alien race name parameter should be a list of race names separated by spaces.  If no race name is provided, or if a race name is provided with no space between it and the @ symbol, the message will be sent to all races.  This message is sent to all races anonymously:</p>
<pre>@
Hey, everyone, SpaceManSpiff breaks alliances!
@</pre>
					<p>This message is sent to SpaceManSpiff and is signed:</p>
<pre>@ SpaceManSpiff
I'll never trust you again!
- Sparky
@</pre>

				<h3 id="ordE">= &lt;FirstName LastName&gt;</h3>
					<p>Set the player's <a href="#players">real name</a> for use in the <a href="http://www.refs-home.net/galaxyng/hof/">Hall of Fame</a>.</p>

				<h3 id="orda">a &lt;alien race name&gt;</h3>
					<p>Declare an <a href="#war">alliance</a> with another race.</p>

				<h3 id="ordb">b &lt;group number&gt; &lt;number of ships&gt;
				<br />b &lt;group number&gt; FLEET</h3>
					<p>Break off a number of ships from a group, forming a new <a href="#groups">group</a>.
					<br /><strong>or</strong>
					<br />Remove a group from its current <a href="#fleets">fleet</a>.</p>

				<h3 id="ordc">c &lt;new player race name&gt;</h3>
					<p>Change the player's race <a href="#names">name</a>.</p>

				<h3 id="ordd">d &lt;name&gt; &lt;drive mass&gt; &lt;attacks&gt; &lt;weapons mass&gt; &lt;shields mass&gt; &lt;cargo mass&gt;
				<br />d FLEET &lt;fleet name&gt;</h3>
					<p>Design a new <a href="#types">ship type</a> with the given attributes.
					<br /><strong>or</strong>
					<br />Create a new <a href="#fleets">fleet</a>.</p>

				<h3 id="orde">e &lt;ship type name&gt;
				<br />e &lt;fleet name&gt;</h3>
					<p>Eliminate a <a href="#types">ship type</a>.
					<br /><strong>or</strong>
					<br />Eliminate a <a href="#fleets">fleet</a>.</p>

				<h3 id="ordf">f &lt;alien race name&gt;</h3>
					<p>Request a <a href="#player">player's</a> email address. If no email address is returned, the player is <a href="#ordo">anonymous</a>, the race is not being played, or the race is being played by the GM.</p>

				<h3 id="ordg">g &lt;group number&gt; [number of ships]</h3>
					<p><a href="#upgrade">Upgrade a group</a> of ships.</p>
     
				<h3 id="ordh">h &lt;group number&gt;
				<br />h &lt;fleet name&gt;</h3>
					<p><a href="#move">Reverse the course</a> of a group in hyperspace.
					<br /><strong>or</strong>
					<br />Reverse the course of a fleet in hyperspace.</p>

				<h3 id="ordi">i &lt;group number&gt; &lt;planet name&gt; [number of ships]
				<br />i &lt;fleet name&gt; &lt;planet name&gt;</h3>
					<p><a href="#move">Intercept alien groups</a> with a group.
					<br /><strong>or</strong>
					<br />Intercept alien groups with a fleet.</p>
				
				<h3 id="ordj">j &lt;group number&gt; &lt;fleet name&gt; [number of ships]
				<br />j &lt;fleet name&gt; &lt;fleet name&gt;</h3>
					<p>Have a group join a <a href="#fleets">fleet</a>.
					<br /><strong>or</strong>
					<br />Move all groups from the first fleet to the second fleet, leaving the first fleet empty.</p>
						
				<h3 id="ordl">l &lt;group number&gt; &lt;CAP | COL | MAT&gt; [number of ships] [AMOUNT &lt;amount&gt;]</h3>
					<p>Load <a href="#cargo">cargo</a> onto a group of ships.  Ships will be fully loaded if sufficient cargo is present, otherwise the load will be split evenly between the ships.  The optional AMOUNT parameter specifies an amount of cargo to be loaded per ship.  Specifying an amount of less than 0.01 per ship or more than the cargo capacity of a ship in the group will result in an error.</p>

				<h3 id="ordm">m &lt;x&gt; &lt;y&gt; &lt;extent&gt;</h3>
					<p>Change the area covered by the text map.  The text map is a square with the top left at (x,y) and the extent of each side in light years.  For example, m 10 20 50 will result in a text map with corners at (10,20), (10,70), (60,20) and (60,70).  The <a href="#locate">location</a> of enemy ships in hyperspace is the only unique element on the text map.</p>

				<h3 id="ordn">n &lt;current planet name&gt; &lt;new planet name&gt;</h3>
					<p>Rename a planet.</p>

				<h3 id="ordo">o [NO] &lt;anonymous | autounload | battleprotocol | compress | gplus | groupforecast | machinereport | planetforecast | prodtable | routesforecast | shiptypeforecast | sortgroups | txtreport | xmlreport&gt;</h3>
					<p>Set a game option.  If the NO parameter is included, the option is turned off, otherwise it is turned on.  The following options are turned on at the beginning of the game:</p>
					<ul>
						<li>Anonymous: The player's email address is <a href="#ordf">not available</a> to other players.</li>
						<li>AutoUnload: All <a href="#cargo">cargo</a> will be automatically unloaded at the player's planets and uninhabited planets.</li>
						<li>GroupForecast: The forecast will include a prediction of the position of each group.</li>
						<li>PlanetForecast: The forecast will include a prediction of the status of each planet owned by the player, as well as any planets the might be colonized by the player.</li>
						<li>ProdTable: The report and forecast will include a Ships in Production table.</li>
						<li>RoutesForecast: The forecast will include a prediction of the player's routes.</li>
						<li>ShipTypeForecast: The forecast will include a prediction of the player's ship types.</li>
						<li>SortGroups: <a href="#groups">Groups</a> will be renumbered at the end of the turn.</li>
						<li>TXTReport: Copies of the forecast and report will be sent to the player in ASCII text format.</li>
						<li>XMLReport: Copies of the forecast and report will be sent to the player in XML format.</li>
					</ul>
					<p>The following options are turned off at the beginning of the game:</p>
					<ul>					
						<li>BattleProtocol: The report will include an extra section detailing each shot fired in each battle.  Some turn viewers use this feature to analyze and animate battles.  Turning this option on can significantly increase the size of turn reports.</li>
						<li>Compress: The forecast and report will be compressed (zipped) before being sent to the player.  The GM should be consulted before turning this option on, as it is not supported by all GalaxyNG servers.</li>
						<li>GPlus: The forecast and report will be sent to the player in Galaxy G+ format.  This allows players to use Galaxy G+ turn viewers when playing GalaxyNG.</li>
						<li>MachineReport: Copies of the forecast and report will be sent to the player in a rudimentary machine-readable format.</li>
					</ul>

				<h3 id="ordp">p &lt;planet name&gt; &lt;CAP | MAT | DRIVE | WEAPONS | SHIELDS | CARGO | ship type name&gt;</h3>
					<p>Set <a href="#prod">production</a> for a planet.</p>

				<h3 id="ordq">q &lt;player's race name&gt;</h3>
					<p>Quit the game. The player's race name is required as a parameter to protect players from accidentally quitting.</p>

				<h3 id="ordr">r &lt;origin planet name&gt; &lt;CAP | COL | EMP | MAT&gt; [destination planet name]</h3>
					<p>Set a <a href="#route">route</a>.  Omitting the destination planet cancels an existing route.</p>

				<h3 id="ords">s &lt;group number&gt; &lt;planet name&gt; [number of ships]
				<br />s &lt;fleet name&gt; &lt;planet name&gt;</h3>
					<p><a href="#move">Send</a> a group to a planet.
					<br /><strong>or</strong>
					<br />Send a fleet to a planet.</p>

				<h3 id="ordt">t &lt;current ship type name&gt; &lt;new ship type name&gt;
				<br />t &lt;current fleet name&gt; &lt;new fleet name&gt;</h3>
					<p>Change the name of a <a href="#types">ship type</a>.
					<br /><strong>or</strong>
					<br />Change the name of a <a href="#fleets">fleet</a>.</p>

				<h3 id="ordu">u &lt;group number&gt; [number of ships] [AMOUNT &lt;amount&gt;]</h3>
					<p>Unload <a href="#cargo">cargo</a> from a group of ships.  The optional AMOUNT parameter specifies an amount of cargo to be unloaded per ship.  Specifying an amount of less than 0.01 per ship or more than the cargo currently carried by each ship in the group will result in an error.</p>

				<h3 id="ordv">v &lt;planet name&gt;</h3>
					<p>Claim victory over an enemy planet if it is <a href="#bomb">bombed</a>.</p>

				<h3 id="ordw">w &lt;alien race name&gt;</h3>
					<p>Cancel an alliance with another race, <a href="#war">declaring war</a>.</p>

				<h3 id="ordx">x &lt;group number&gt; [number of ships]</h3>
					<p><a href="#groups">Scrap</a> a group, converting the ships into materials.  Scrapping ship that are carrying colonists or orbiting an alien planet will result in an error.</p>

				<h3 id="ordy">y &lt;new password&gt;</h3>
					<p><a href="#orders">Change the player's password</a>.</p>

				<h3 id="ordz">z &lt;new email address&gt;</h3>
					<p>Change the player's <a href="#orders">email address</a>.  Note that players can have reports sent to multiple addresses by separating them with commas, e.g. &lt;email address 1&gt;,&lt;email address 2&gt;.</p>

			<hr />

			<h2 id="misc">Miscellaneous</h2>

				<h3 id="email">Sending Messages to Other Races</h3>
					<p>Players can send messages to other races by sending an email to the GalaxyNG server using this format:</p>
					
<pre>To: server@somewhere.org
From: player@someplace.com
Subject: relay &lt;alien race name&gt;

#GALAXY &lt;game name&gt; &lt;player's race name&gt; &lt;password&gt;
...message...
#END</pre>

					<p>To relay a message to all players in the game, rather than to a
single player, the subject of the message should be "relay &lt;gamename&gt;" - thus, to relay to all players in Distance the subject should be "relay Distance"</p>

					<p>Players should be sure to end their messages with #END if they want to prevent their email signature from being sent to the recipient.  Messages should <strong>never</strong> be sent in HTML format, as this will expose the player's password to the message recipient.</p>
					
					<p>Note that players can also <a href="#ordA">send messages</a> to other races in their turn reports.</p>
				
				<h3 id="ascii">Plain ASCII Text</h3>
					<p>All email to the server must be sent in plain ASCII text.  The server cannot interpret HTML formatted email.  Multipart Text+HTML email can be used, however HTML messages sent to other players by the server will display the sender's true identity and password to the recipient!  See <a href="http://www.expita.com/nomime.html">Configuring Mail Clients to Send Plain ASCII Text</a> for more information.</p>
					<p></p>
 		
 		<?php include 'footer.php'; ?>
 	</body>
</html> 	
