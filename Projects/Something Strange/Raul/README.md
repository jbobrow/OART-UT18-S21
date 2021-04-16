# Project 2 - Provenance

Principle: 
- Core Mechanic: Tradeoff
- Prototype of Core Mechanic: 

- Number of Players: 1 
- Number of Blinks: Intended for 6, could add more
- Time to play: 2-6 minutes

### Description

Who knows what to do in order to get home? What is home? Explore these topics in an offbeat puzzle game.

## Game Instructions

### Setup

Line 6+ blinks up in a line and program the game. The game starts immediately.

### Gameplay

Experiment and explore how to get to a home state. Just remember to click the blinks to restore their brightness. 

### Win Condition

If the blinks are in the correct state, they will all turn green. If one of their blinks loses their brightness completely, all the blinks will turn red. Rearrange in a line and single click any blink to start the game over again.

## Process
- Inspiration

    My main inspiration was Rymdkapsel by Grapefrukt Studio and Florence by Ken Wong. Rymdkapsel was my inspiration for the tradeoff mechanic. Since Florence uses color in its scenes and they get colored in as the game progresses, Andrew gave me the idea to color in the blinks. I ended up flipping this and making it so that you need to click to maintain the color. I just had the idea to do something with the arrangement of blinks so I incorporated this into the game. 

- Design Process

    I built all of the pieces separately after examining my thoughts on paper, as shown here: https://www.notion.so/Provenance-7b345d166e9c4877859dc5e82f58c734. Check that link for more documentation as well.

- Learning, Questions, Notes

    - If I had more time, I would have implemented fancy graphics and fades between the gamestates. Also, in the end conditions of the game, I would have made it so that all the blinks need to be off in order to restart the game

    - Though it's not second nature just yet, I learned how to incorporate the communication I needed into the existing framework for gamestate switching. 
    
    - Note: I could only use a limited amount of colors due to the limited number of bits available. I used off as a color so it would be a kind of placeholder so that when other blinks are communicating and they receive 0 (the place of off in the index) they won't change to that color