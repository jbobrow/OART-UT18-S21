# Project 1 - Mini Golf

Game Selected
- Number of Players: 1
- Number of Blinks: 6+
- Time to play: 1 minute 

### Description
Classical minigolf. Putt the ball and try to get it into the hole in one shot! 

## Game Instructions
Once the game loads, all the blinks will be green. Keep them all connected.
1. Single click 1 blink to be the ball. It will turn white. 
2. Double click 1 blink to be the hole. It will turn red. 
3. Then,  position the ball and the hole on opposite sides of the remaining green blinks. 
4. Single click the white ball to putt.
5. If it goes into the hole, it will turn Cyan and the game is over. 
6. To play again, reupload the game to blinks. 

### Setup
Keep the blinks all together until a ball and hole are declared. Then, seperate them and construct the course. 
### Gameplay
Simple: Line up the blinks so that (in the case of 6 blinks) there are 4 blinks in between the ball and the hold. 
<img width="502" alt="Screen Shot 2021-03-13 at 4 57 46 PM" src="https://user-images.githubusercontent.com/78987969/111050895-5599e880-841d-11eb-9cec-e506162ce5e9.png">


  EXTREME COURSE VARIATION:
  Place the hole alone after its declared. Then, slide all of the green blinks against the hole blink to construct the course. After that, slide in the ball to connect to one of the green blinks and shoot from there. 
 <img width="619" alt="mmidterm e xtrama pic" src="https://user-images.githubusercontent.com/78987969/111050927-837f2d00-841d-11eb-8dbf-8cb96db478e5.png">


### Win Condition
Line up the blinks so that the ball and  the hole are on opposite sides of all the blinks. Single click the ball and win.  

## Process

- Inspiration

Golf is one of the great games and I was hoping to recreate a highly concentrated version of it in mini golf. 

- Design Process

Step one: Create a system where when you start the game the hole and the ball are assigned randomly each time. 
  I was unable to ever figure this out and had to abandon it in favor of having the user decide where the ball and hole were. 
  Reasons for abandonment: duplicate randomization (2 holes, 3 balls, etc..) Unable to control the randomization. Communicating the randomization across all the  blinks. 
  
  Step two: Create a system where you can shoot the ball across all the blinks via light. This worked well. 
  
  Step three: Create system states to communicate changes across all blinks. Issue with this was that it broke step two. Had to fix it by changing where signalStates were switched.  This is very confusing code and I wish it was taught clearer. 
  
  Step four: attempt a restart function but had to abandon it. 
  
  Goal was to additionally have a hazard blink that would block your shot, but I ran out of time. 

- Learning, Questions, Notes

It took two weeks to finally begin to understand signalStates. Part of the reason for the confusing was that it was explained as centering around Inert + Go functions. But this is highly misleading.  The core of these are the signalStates. The naming of the functions matter not. The secondary core is calling the signal states in For_Each loops. This was not explained and only assumed. A great challenge. I barely understand them but got it enough to get some communication functioning. 

Having a loop to reset the game and its variables was very difficult and I could not figure it out. The closest I got was resetting the hole back to the original state, but not its neighbors. I still struggle greatly with signalStates as we've only glossed over it and it is highly advanced. 
