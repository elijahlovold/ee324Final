
# Git repo for 324 final project. 

Target is snake game with extra features including portals, teleporting fruit, and power ups. Additionally, multiple levels will be present in the game. 

# Features to implement: 
##  Hardware 
        • Grid sprite controller - splits screen into grid where each element is an address to a sprite ROM
        • Sprite ROM - rom containing all the sprites used in the game (or multiple roms for different object types)
        • Sprite to .coe tool - python or cpp tool to convert a sprite image to .coe (check if provided) - Provided by Dr. Murray
        • Audio to .coe tool - python or cpp tool to convert an audio file to .coe (check if provided) - Provided by Dr. Murray
        • Audio controller - used to play audio samples or background music
        • I/O interface - block to recieve user inputs from advanced I/O
    
##  Software 
        • TBD
         

## Sprites
        • Head: UpHead, DownHead, LeftHead, RightHead, TurnRight, TurnLeft, TurnUp, TurnDown, HeadIntoPortal1 (4 angles), HeadOutPortal2 (4 angles)
        • Body: TurnUp, TurnDown, TurnLeft, TurnRight, VerticalTravel, HorizontalTravel
        • Tail: UpTail, DownTail, LeftTail, RightTail, TurnRight, TurnLeft, TurnUp, TurnDown
        • Gameplay Sprites: Food, Portal1, Portal2, (Potential Powerups?)

# Block diagram
![block diagram.](/vivado/shared_source/elijah_block_diagram.png)

Note, our local projects are excluded from the repo. Thus, make sure as many source files are in the shared_sources folder as possible. 
