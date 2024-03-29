README
# Final Project - Doctor Defense....by, Andrew, Brandon, & Carly 

Principle: 
- Core Mechanic: Level creation, placement strategies, epidemic simulation

- Number of Players: 2
- Number of Blinks: Greater than or equal to 18
- Time to play: 5 minutes

### Description
Doctor defense is a two player game in which players compete to spread the virus or stop the virus from spreading to the end of the course.

### Story
As a member of the World Doctor Defense Force, you must train to deploy resources effectively. What is the best way to deploy doctors to stop the spread of a virus? Through this training simulator, you will find out. No pressure. But if you lose, you may be out of a job. 

## Game Instructions
### Setup
Doctor defense is best played with at least 18 blinks. There should be one doctor blink for every 5 non-doctor blinks.
Setup of the game can be split into two main parts: one for the virus/'attacking' player and one for defending player. First, the player whose goal is to spread the virus to the end of the course must create a path of blinks. No blink in the pathway should be connected on more than four of its faces. The last blink in the pathway should be double click to declare it as the goal. The goal blink can only have ONE neighbor. 
Next, the defending player may select the blinks that they want to use as doctors. This can be done by multi clicking the desired blinks. The number of doctors a player is allowed to place is dependent on the number of blinks used in the path. The defending player may not place a doctor blink that stops the pathway from being continuous. A doctor blink cannot be connected to the goal blink. 
Once that is done, the virus player should single click a blink on the opposite end of the pathway from the goal.

### Gameplay
After single clicking a blink, it will become sick and the virus will begin to spread. If it reaches the goal blink declared during setup, the virus player wins. If the defending player wipes out all the virus blinks, then they win. Similar to an endless mode in a standard tower defense game, the players may make new paths and place doctors for as long as they want.

Color Legend:
|State|Color|
|--|--|
|Susceptible|Orange|
|Immune|Yellow|
|Sick|Green|
|Recovered|Cyan|
|Dead|Red|
|Doctors|Purple|
|Goal Blink|White|

### Win Condition
Virus: Win by spreading all the way to the goal blink.
Defender: Win by eradicating the virus.

## Process
- Inspiration:
The main source of inspiration for this game was the traditional tower defense games like Bloons Tower Defense. There is also the ongoing COVID situation which easily followed from the core epidemic simulation that was drawn from to create the game.

- Design Process
The idea for this game began in a breakout room during class. Andrew had spent the previous week working on porting the epidemic simulation to blinks and came up with the idea that the simulation might work well in the context of a tower defense.
- Learning, Questions, Notes
Epidemic Emoji Simulator: https://ncase.me/sim/?s=sick
