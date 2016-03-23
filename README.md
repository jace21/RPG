#Object Oriented Driven RPG Video Game
Rogue-like element RPG developed using a cross platform library called Allegro. Created in Visual Studios using C++

# Features
This RPG is a time-based video game where movement and basic combat are run by two individual times that check for input at different intervals. The in-game timer stops whenever a battle occurs and a new timer called the battle-timer starts to dictate different commands for command. The internal timer helped manage player input and other elements such as movement. The video game also had basic menu options such as settings, equipments, and save/load options. 

# Design
The design philosophy for the video game was Object Oriented Programming because many of the elements are shared from a master class, which had elements like stats and health/mana pool. The hero class is a special class of the master class because it has elements of the master class but differs in certain areas such as abilities, inventory and character movement from input. Another design implementation was the development of a timer based check system that would process inputs at 1/60th of a second which meant the game ran at 60 FPS. This allowed us to make a time-based game engine that would be the heart of our video game.
