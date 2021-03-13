**PROGRAM I: Split Flap**

Video: https://youtu.be/dUKpC2Uktg8

In airports and train stations there would be a plit flap informing people of where things are, much like tvs do now. I was inspired by [this video] (https://www.youtube.com/watch?v=j6KeAlTQCk0&list=PLNZrOW6Nuocrj0KvUxGGYIk6aX6P2TMAw&index=6&ab_channel=ChrisRamsay) (a fantaastic video on its own)to try to make a split flap type thing on blinks.

I knew I could start from the coin flip in the widgets in order to make a flipping light display. However, the challenge came with separating this code and implementing a sequence of colors rather than just two colors. The flipping display and communication works, which is nice. However, when you try to flip all of them for a second time, they never stop. This is because all of the blinks are sending 1 repeatedly and as a result never stop. This is where the program doesnt work. I also was aiming to 


**PROGRAM II: winnerOrLoser**

This program is a very stripped down version of the game I want to make next week. Two blinks are selected and when play begins, they touch other blinks and turn them the same color as themselves. Once the timer is up, the remaining blinks would display the winner. The issue described below was bottlenecking the rest of the program. If I get this down, the rest of the program will be a breeze. 

The issue I was running into was to try to identify a colored blink as itself. There was no way in my mind to do this because I do not know of a way to say "if a blink is within this array, do this" so I just got stuck. The issue is on line 78. If anybody has any suggestions, I would be super open to it. 