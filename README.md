
# Git repo for 324 final project. 

Target is snake game with extra features including portals, teleporting fruit, and power ups. Additionally, multiple levels will be present in the game. 

# codebase layout
## Ps4 Interface 
        • contains code to interface with ps4 controller and blackboard

## vitis 
        • shared_source contains all the source files
        • shared_main is called from main.cpp in vitis project

## vivado 
        • shared_source contains all the modules
        • ip_repo contains all AXI modules

# Features to implement: 
##  Hardware 
        • Grid sprite controller - splits screen into grid where each element is an address to a sprite ROM
        • Sprite ROM - rom containing all the sprites used in the game (or multiple roms for different object types)
        • Sprite to .coe tool - python or cpp tool to convert a sprite image to .coe (check if provided) - Provided by Dr. Murray
        • Audio to .coe tool - python or cpp tool to convert an audio file to .coe (check if provided) - Provided by Dr. Murray
        • Audio controller - used to play audio samples or background music
        • I/O interface - block to recieve user inputs from advanced I/O
    
##  Software 
        • Snake node class
        • Snake head -> inherits snake node
        • Snake tail -> inherits snake node
        • Snake -> contains a vector of snake nodes, tail, and head
        • Portals 
        • Food
        • Maps
        • Timer -> Use GTC 
        • Grid controller 
        • Audio controller
        • Color controller 
        • IO controller 
        • UART controller

## Sprites
        • Head: UpHead, DownHead, LeftHead, RightHead, TurnRight, TurnLeft, TurnUp, TurnDown, HeadIntoPortal1 (4 angles), HeadOutPortal2 (4 angles)
        • Body: TurnUp, TurnDown, TurnLeft, TurnRight, VerticalTravel, HorizontalTravel
        • Tail: UpTail, DownTail, LeftTail, RightTail, TurnRight, TurnLeft, TurnUp, TurnDown
        • Gameplay Sprites: Food, Portal1, Portal2, (Potential Powerups?)

# Block diagram
![block diagram.](/vivado/shared_source/elijah_block_diagram.png)

Note, our local projects are excluded from the repo. Thus, make sure as many source files are in the shared_sources folder as possible. 
