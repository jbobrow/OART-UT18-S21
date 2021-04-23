# 05-Simulated Systems

### Assignment
This week I chose to adapt the Predator-Prey model to blinks. An example of the simulation can be seen here: [Predator-Prey Model](https://ncase.me/sim/?s=prey).
There is no setup required, once the blink is loaded up, it will start automatcally, and should be seeded randomly as either a fox or a rabbit (the code for seeding the simulation could probably be better; currently it's just an even 33% chance to either be a fox, rabbit, or empty).
Foxes can be manually spawned by double clicking a blink.
Rabbits can be manually spawned by single clicking a blink.

The simulation runs, but I'm not 100% certain it is completely faithful to the simulation example shown in the link above. There might be some places where the logic I used to get it working on blinks differs from the model in the example. Also I was not sure on what values to give for a rabbit's chance to leave behind offspring or a fox's chance to die.

It took me a little why to get the sim up and running for a single blink so I have not yet approached the challenge of using a single face on a blink as a cell as opposed to an entire blink being a cell. I do know what direction I would go in if I were to make this change. Currently each blink has an array that stores the valid faces that can be jumped to every time the step timer expires. I would probably have to adapt this array into a nested array to represent the valid faces each individual face on a blink has available to it.

The simulation I implemented has the unique characteristic that foxes and rabbits jump randomly everytime the step timer expires. Datagrams were very useful in adapting this behaviour to blinks.