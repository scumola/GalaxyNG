<?php echo '<?xml version="1.0" encoding="iso-8859-1"?>'; ?> 
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN" 
   "http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en-US">

<!-- $Id: beginner.php,v 1.1 2004/04/04 23:57:13 christhecat Exp $ -->

	<?php $title="Beginner's Guide to Galaxy"; ?>

	<head>
		<link rel="stylesheet" href="/main.css" />
		<title>GalaxyNG: Beginner's Guide to Galaxy</title>
	</head>

	<body>
		<?php include '../menu.php'; ?>
		
		<p>By Greg Lindahl</p>
		
		<p class="note">This guide was written for the original version of Galaxy, so some of the calculations don't apply to GalaxyNG.
		<br />Originally appeared in PBEM Fanzine v92 n03 (15Sep92)</p>
		
<p>So you signed up for a game of Galaxy, and then you read the rules,
and then you wondered, "How on earth do I play this game, anyway?"
Or, like I did, perhaps you thought it was all obvious. Then you lost
your first game or two, and finally realized that you don't know it
all.</p>

<p>For those of you who've reached this stage, here's a little sage
advice from another beginner.</p>

<p>First off, let's talk about economics. In order to win, you're going
to eventually build up your population and industry to many times its
starting value. Since your home world starts off fully developed, you
must start colonies on other worlds. The best worlds to colonize are
ones which you can reach quickly, that are relatively large (more than
500 possible population), and that have Natural Resources ratings of
at least 1.</p>

<p>Once you've picked several worlds to colonize, you must bring in
colonists and capital. Since 1 unit of frozen colonists unfreezes into
8 colonists, it is relatively cheap to ship in colonists and
relatively expensive to ship in capital. The best cargo ship is one
that is as small as possible, i.e. with a cargo capacity of 1. I
design my cargo ships with no shields, and with engines just big
enough to reach several close-by planets in one turn. I pick this
engine size by drawing up a map of local planets and figuring out the
size of circle needed to enclose a reasonable number of worlds.
Remember that a full cargo hold weighs twice what it does empty, and
take this into account when you are picking the engine size.</p>

<p>Once you start colonizing a planet, it will start growing its own
colonists, at 10% per turn, and more capital, at 14-16% per turn,
depending on the Natural Resources rating. Note that a Natural
Resources rating above 1 really doesn't help you much more, but a
rating blow 0.5 really begins to hurt. I generally only bring in 1/4
the maximum number of colonists and capital; then the world can fill
itself up in about 14 more turns, while I colonize more worlds. (Clue
for the non-financially-minded: divide 72 by the growth rate in
percent to find the doubling time. It's called the "rule of 72".)</p>

<p>In order to be able to colonize new worlds, you have to be able to
defend them and capture territory, if necessary. So, we now arrive at
the general subject of ship design. The first time that I tried to
design a ship, I totally goofed it up, so I'd advise playing with the
numbers a bit.</p>

<p>The first thing to note is that shields become less effective as ships
become larger and larger: the number of shields is divided by the cube
root of the ship mass. Since a ship with an attack of 10 has a 50%
chance of hitting a ship of size 30 with 10 shields, this means that
ships smaller than 30 will have shields stronger than an attacking
weapon of the same size, and ships larger than size 30 will have
shields weaker than an attacking weapon of the same size.</p>

<p>A corollary of this point is that a very small ship with a shield of
1 can barely be hit by a very small ship with a weapon of 1. So, if
you want to design some small armed ships, it is much better to put a
small shield on the ship than to put no shield on the ship.</p>

<p>So, let's say that you want to design a big ship with no engines to
defend your home world. If you have 1000 industry and a Natural
Resources rating of 10, the biggest ship that you can build is of size
90. Let's say that we want to design this ship so that if a similar
ship attacked it, each ship would have a 50% chance of destroying each
other -- hence, you want an equal effective attack and defense rating.</p>

<p>This isn't what happens if we have 1 weapon of strength 45 and a
shield of strength 45. The mass of our ship is 90, so the effective
strength of the shield will be:</p>

    <pre>strength = 45 / 90^(1/3) * 30^(1/3) = 30.8</pre>

<p>So, a weapon of strength 31 would have a 50% chance of destroying our
ship with a shield of 45 -- so let's build a bigger shield and a
smaller weapon.</p>

<p>If you play around with the math a bit, you will discover that a
shield size of 53 and weapon size of 37 is optimal. The effective
shield strength is:</p>

     <pre>53 / 90^(1/3) * 30^(1/3) = 36.7</pre>

<p>Now, you may be wondering, why am I multiplying by 30^(1/3) above?
This is a fudge factor that takes into account the statement in the
rules that a ship of size 30 with 10 shields and a weapon of size 10
has a 50% chance of hitting itself.</p>

<p>Designing a ship with engines is a similar process. Generally I pick
the speed that I want first, and then buy the weapons and shields to
be equal.</p>

<p>Once the game has been running for a while, you will want to purchase
higher technology. I would advise waiting to do this until the price,
5000 for one level, is no longer many times one turn's production. The
first level is the most important, as it doubles your movement,
firepower, etc. Once you are spending a lot of income buying ships, it
is often more economical to buy tech and ships than just ships alone.
Eventually, the smaller players without advanced tech will find
themselves easily wiped out.</p>

<p>Now, I'll pass along a few pieces of sage advice, mostly courtesy of
Howard Bampton. Armed scouts can get you into trouble in the early
game because many players won't declare peace until they have a reason
-- which means you'll be in a war with them if you encounter any of
their armed ships. I generally declare peace on everyone, but I seem
to be unusual. Ships that move will not fight any battles at the
planet that they start the turn at.  Finally, you cannot load a group,
move it to a new planet, and unload it all in one turn. Ships do not
arrive at their destinations until after all your orders are
processed, so the 'U' order must be given on the next turn. It is
possible to unload cargo, load new cargo, and move all in the same
turn.</p>

<p>Hopefully these Galaxy tips will help you survive those first dozen
turns or so. Good luck.</p>


		<?php include '../footer.php'; ?>
	</body>
</html>