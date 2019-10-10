The Battle of Mars – Savage Swarm is a turn-based single player strategic game written in C (C99) with the use of C standard library designed for Linux operating systems. 

The game is open source and is distributed under MIT licence.

You can see a video of the gameplay on YouTube:
https://www.youtube.com/watch?v=RRmWkSnfJ24&t=40s

INSTALLATION
The installation procedure is very simple. 
In the main directory there is a shell script called make thebattle.sh which compiles all the source files and builds executable binaries, which are placed in the main directory.

DESCRIPTION

This is version 1.0. In this version, AI makes random moves, and if an enemy is in range of fire, it uses the best weapon (if it has enough action points). Units are visible.

There are more versions coming:
The Battle of Mars – Blade Runner
	In this version sensors will be introduced and mechs will not be visible. Opponents will have to use Light Mechs to scout and spot foes. The AI will also be improved.

The Battle of Mars – The King Hunt
	In this version most of the focus will be on the AI. Path finding algorithms will be introduced as well as machine learning, so the right decisions can be made.

In the game, the player leads the yellow team of mechs and has an objective to destroy all enemy mechs (AI, blue team).

There are four main types of mechs: 
(1) Light Mechs
(2) Medium Mechs
(3) Heavy Mech
(4) Super heavy mechs

All have different strengths, range of movement and weapons.

There are four types of weapons:
(1) Guns
(2) Lasers
(3) Rockets
(4) Super rockets

In the game each mech has its own number of action points which may be spent on moving or shooting. After a mech completes a move or shoots, an adequate number of action points are consumed.

Light Mechs are light type units equipped with a gun, which has a moderate range and causes little damage but consumes few action points. Light Mechs may be used for scouting and as support units.
Medium Mechs are general purpose units equipped with a gun and laser. The laser is a very powerful weapon, but it has limited range.
Heavy Mechs are strong units equipped with rockets and lasers. Rockets cause moderate damage but have a long range. Heavy Mechs may be used to support other types of units firing from long distances and open spaces.
Super heavy Mechs are the strongest units equipped with super rockets (very long range) but have limited amount of action points. Use them when everything has gone wrong, but pay attention to their limited action points.

There are two types of obstacles: trees (T) and rocks (R) which are impassable and reduce the visibility of Mechs.

The screen is divided into four parts. In the main part a strategic map is presented where the player can see units and obstacles. Below the strategic map is the menu bar, which will be described later.
The right part contains all the information about the player’s unit. However, if the enemy’s unit is inspected, only the type of unit is shown and minimum information about its health.
The bottom part displays a log dairy where recent events are printed. There is no option to use scroll.

MENU BAR

Main menu
From the Main menu, the player can go to the Map menu, finish their turn, then the AI makes its moves; and the game ends.

Map menu
From the Map menu, the player can go back to the Main menu. In the Map menu, the player can walk in the map by using the "wsad" keys; the actual position on the map is highlighted by a green cursor. If the cursor enters on player's unit an additional option appears, Unit menu, and on the right panel more detailed info about the unit is shown. When the player's unit is inspected, a gray field is shown on the strategic map which shows firing range of the mech. When the player enters the enemy’s unit, only very limited info is shown on the right panel.

Unit menu
From Unit menu, the player can go to the Firing menu and go back to the Map menu. In the Unit menu, the player can move selected units. After each move, action points are reduced by 1. If a unit carries two weapons, an additional option is shown, and the player can change to a secondary weapon.

Firing menu 
From the Firing menu, the player can go back to the Unit menu. Using the "wsad" keys, the player may move the cursor to aim at enemy units. If enemy units are in range (inside the gray field), the firing option appears. If an enemy unit is out of range, an appropriate message shows up in the log panel. 

Player wins if all enemy units are destroyed.

Best of luck to everyone.
