# Project 1 - Title

Game Selected
- Number of Players- 1 - 3 (more people could play as a team)
- Number of Blinks - 10+ (ideally, could easily be played with 2 but the more the merrier)
- Time to play - ~3 min

Link: https://youtu.be/oX9wa_-wPLU 

### Description

Test your touch and timing in a fast paced game of Jackpot. Race to accrue the most points before the timer expires or hit the Jackpot blink to win it all!

## Game Instructions- READ THROUGH BEFORE PLAYING

### Setup

Setup all the blinks you want to play with in a cluster. When the game is loaded up, all the blinks will be white. Click a blink to cycle between the 3 team colors and triple click it if you want to switch it back to white. **If it is on a team color, we will call this a slider blink. If it is not, this is a spinner.** After the team blinks have been selected, keep them connected to the cluster, position them to the outside, and double click any blink in the cluster.

This will then switch to a jackpot selection mode. Here, click on a blink that is off to turn it into a jackpot blink and click again if you want to turn it off. If you are playing somebody, make sure you both agree on the placement of the jackpot blink. Then double click a blink in the back that will not get hit (towards the back). There is a weird bug that I am still working out and in order for the game to properly reset, click a blink in the back to make sure it can reset. 


### Gameplay

Once you double click, quickly remove your slider blinks, slide the cluser away from you, and start playing! Slide your slider blink across the table and try to attatch it at the right time to the spinner blink. If you properly attatch it, the spinner blink will turn green. Remove the spinner and slider, detatch the spinner and set it beside you and keep playing. If you do not attatch it successfully , the blink will simply stop spinning. If this happens, remove your slider and try again! Keep playing until the jackpot blink is hit or the time expires. 


### Win Condition

Whoever hits the jackpot blink wins automatically! (All the blinks will turn yellow)

If the timer expires before the jackpot blink is hit, then count your score. Magenta blinks are worth 2 points and blue blinks are worth 1 point. (All the blinks will turn red)

**Note**: I have bug to where not all blinks know when the time is expired. If you see a blink turn red, this means the time has expired. To reset the game, put all the blinks back onto the cluster and press the red blink. This resets the game. If no blinks turn red after a significant amount of time, remove the jackpot blink, set it so it still touches the cluster but has an exposed side. Then try to attatch the slider to the jackpot so that all the blinks turn yellow (no need to slide across in this case).

Regardless, to play again, reattatch all blinks and single click any red or yellow blink and replay. 

## Process

- Inspiration & design process

For inspiration and design process: Check out this link which holds my files for these topics. It features the initial rule set and the document where I came up with the idea as well as my alternative game if Jackpot didnt work out. Those are my only two documents of ideation: https://www.notion.so/Jackpot-8740f37133e9430f9a7d2fdb7c99ea3a

- Learning, Questions, Notes

    - At the time I am writing this (6:40 PM Thur), the program works but it does not have a cool jackpot win animation nor are the game mode names intuitive. However, comments make this program understandable. I wanted to submit first and then another copy may be submitted later with better naming and a cool win animation. 

    - An interesting problem that I encountered was when the timer was set when the game entered mode 3 (i.e. when you play the game), I initially just set the timer to start, rather than setting the timer to never expire then to start. This caused the blinks to be out of sync, rather than all switching to mode 3. (It is a little difficult to explain over text but I can explain it in person)

    - Another bug that I am running into is that not all blinks turn red when the time is up. This does not make the game unplayable but just makes it not perfect.

    - Thanks Professor Bobrow for the suggestion to implement the communication between slider blinks and spinning blinks in the sendData byte via bit shifting. It really saved my whole program.

    - This project is part creation and part collage. I modified lots of given code but created the game interactions myself. 

