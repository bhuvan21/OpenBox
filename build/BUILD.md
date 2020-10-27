# OpenBox Build

This is actually a relatively simple build! This cost me about £50 to produce, though there are slightly higher costs if these boxes are made not by me (there are one time costs for the first box, like soldering equipment, programming cable etc.)

Though not present here yet, I plan to write a full build guide so that others can produce this box for themselves! I'm holding off on doing that until I'm fully satisfied with the featureset of the box's software and the sourcing of the components.

## Files in the folder
topbox.stl and bottombox.stl are the two stl files which must be 3D printed for the box's housing. I printed these in PLA with standard supports, at a layer height of 0.2mm.  
halfdiffuser.stl and quarterdiffuser.stl are two stl files for the light diffuser caps present on the top of the box. These must be printed in white PLA, two of each are needed.  
schem.sch and board.brd are the Eagle PCB files which make up the custom PCB control board used for this project.  

## Hardware Overview  
The basic overview of this board is as follows. The main control board starts with the custom PCB present in this folder. Once molex headers, regular pin headers and a screw terminal are soldered in, an Arduino Mini Pro and S7V8F5 5V Regulator can be inserted into the board, through the pin headers soldered on. Some resistors need to be soldered on too.  
  
A 6xAA battery holder attached to the screw terminal which is VIN for the board. This line is regulated by the 5V regulator, and the output of this powers the Arduino.  

The external components (rotary encoder, LCD, Neopixels, Piezo, Banan Plugs) must be soldered to crimped molex wires, which in turn form molex connectors to connect to the main board. The pins for this are detailed in the board schematic file.  

The external components are held in place in the 3D printed chassis via nuts and bolts, or mechanical solutions (board slides into rails, light caps are a friction fit).  

That's about it! There's not much complicated wiring here as everything happens on the control board.  