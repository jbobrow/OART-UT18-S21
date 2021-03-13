# 02-Hello Player

Video: https://youtu.be/aORXz8oAnCs

**What I made**

I made a timing game. To play it, make sure the blinks logo on the back is closest to the pad of the middle finger of the left hand when the middle finger is wrapped under two opposite corners of a blink. To then play, double click the middle button and try to time the blue light so that it is at the top position and single click at the right moment. Don’t take too long though as you only have 15 seconds to do this. In order to play again, just double click the button. 

**The Process**

Though most of the process was straightforward, I struggled to implement things since void loop iterates through 30 times per second. It is a different sort of thinking that I am not used to yet. Also, the addition of the is_spinning bool was something I did not think about in order to manage the different states that exist in this game. The process was straightforward because I had used similar animations in the past so it was quick to implement. However, as mentioned before, the different interactions that I had to consider due to void loop was difficult. Along with the is_spinning bool, I also implemented a game state int in order to implement and idle animation. 

**Working & Not Working**

I am very happy that the different states register properly and interact with the timer in the background. There is also an idle state that works, which I am very happy about. The blinking for when the correct light is hit does not work unfortunately. I had changed it to be a stationary green light rather than a blinking one. 