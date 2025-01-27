# Elevator-Group-7

This is the Git repo for the group 7

Leader:Jonathan Kyle Francisco
Coleen Raye Magbag
Gabriel Rosaceña
John Kerby Lola



The System works by checking for request and destination landing,

Clicking [1] On the main menu would ask the user on which floor would the request be made and to what direction they'd want to go. 
Clicking [2] Would mean there is a user inside the elevator that would want to land on a certain floor. 
Clicking [3] would manually move the elevator to the next state. 
Clicking [4] Would display the current request from the different floors
Clicking [5] Would exit the system

It uses a priority queue in handling which of the request would be handled first, given that the request must be of the same direction as the elevator and closer proximity than the original landing destination. If all of this were true it'll be put at the front.

If however the request is on the opposite direction, It would be prioritized last.

