# Project 2 - GACHA!

Principle: 
- Core Mechanic : Memorization and simplicity
- Number of Players : 1-2
- Number of Blinks : 7
- Time to play : depends on the player

### Description
Inspired by the simple yet difficiult gameplay from "Rhythm Doctor" utilizing color and time as a core mechanic in the game. This game was meant to introduce player with 
difficult yet simple game play using similar concept being time and color. Challenge yourself to get more points than your opponent or simply test out your ability on how many stages you can advance in this game. Good Luck!
## Game Instructions


1.set up 7 blinks like this.
2.double click on the middle blink to start the game
3.when the middle blink is double clicked, it will show colors around the 6 blinks. REMEMBER what color was on what blink
4.After 2 second, the colors will disappear and another color will show up in the middle.
5.Find the blink that showed that same color and click on the blink.
6.If you get the answer right, the blink will shine green and reset.
7.If you don't get the answer right, the blink will shine red and reset.
8.if you take more than 3 second, the stage automatically fails and the game will reset.
9.time yourself and see how many stage you can pass. Correct answer counts as +1 point, a fail counts as -1 point, taking more than 3 second and being reset counts as 0 point.
### Setup
![image](https://user-images.githubusercontent.com/79310685/114896561-6b029800-9e4b-11eb-8953-1ec34c45b4d6.png)

1. Set up the blinks as the above image
2. The game will work only with 7 blinks.
3. No need to do anything for any sort of restart since the game will automatically do it for you.

### Gameplay
1. double click on the middle blink to start the game
2. Challenge yourself and see if you can remember the colors in limited time
3. Count up the points you have and compare it with others.

### Win Condition
Correct answer : +1
Missed answer : -1
Expired stage : 0 

therefore it is better for you to let the time expire if you don't know rather than clicking on a wrong blink which will net you negative points.

## Process
- Inspiration
Rhythm Doctor, complex yet simple game based on color and time.
- Design Process
Started off by making different game states and two seperate chunks of code for the blink in the middle and the 6 blinks around it. The blink in the middle acts 
as a key blink that will send values that will randomize the colors for the 6 blinks around it. Timers were set to turn off the blinks and it's display of colors.
The 6 blinks around it will check if it's values are same with the value in the middle blink and check the answer. After all the process, it will reset the game so the player cna continue.
- Learning, Questions, Notes
I was able to learn more about communications and coding in general. It was very hard and I ran into many problems but I am happy that it turned out quite nice.
