# 2D Shape Modeler
Derek Wallace

## Instructions
To use the 5 modes you must press the associated key for the correct mode. 
I set V to go to view mode, P is stamp mode, U is user defined primitive mode, 
G is group mode, M is modify.
View mode controls:
	Click and hold on screen to move viewport in that direction (Mouse input)
	"-": zoom out
	"+" zoom in
	"esc" exit (only works in view mode
	"[" and "]" rotate viewport, WARNING: mouse controls become confusing when viewport is
		rotated. Mouse input goes off window coordinates.
	Window resizing seems to work uniformly, although weird screen sizes tend to force shrinkage.
Stamp Mode controls:
	"C": is the circle primitive but I turned it into an n-gon generator because i thought
		it was more useful. An N higher than 20 appears to be a circle.
	"S": square primitive
	"T": triangle primitive
	NOTE: All primitives are prompted to put in color, and transformation information
		Circle does not have rotation because I replaced that with number of sides, to give
		off a more smooth appearance if wanted, higher n=smooth, lower n is edgy
	NOTE 2: The stamp mode works by using the key press as a trigger, the stamp is placed
		wherever the mouse was hovering at the time of key press, not including translation. (Mouse input)
User Defined Primitive Mode:
	I tried using mouse input to click the points in the shape but it froze alot
		so I went back to entering coordinates by number (x,y)
	"N": Start a new shape
		Prompts user for points until the user inputs q, then generates the shape based
		off points by making triangles with them in sets of 3's. Shared vertices must be 
		entered twice. Shapes can be modified like the others. 
Group Mode: 
	I couldn't get this working without crashing/breaking code so I took it out alltogether.
Modify Mode:
	I took this mode to mean being able to modify the shape you made last.
	"C": Prompts the user to input new color and updates the shapes color.
	Left Arrow: moves the shape left
	Right Arrow: moves shape right
	Up arrow: moves shape up
	Down Arrow: moves shape down
	"R": rotates the shape
	"-":scales shape down
	"=":scales shape up (make big)
	
## Image Explanation
 I made a hexagon surrounded by multicolored triangles because I think it showed off alot 
 about my program. I made the hexagon by giving my circle 6 sides, the triangles were colored and
 rotated on input, then moved and sized into place with modify. I recolored the hexagon with modify
 from black to white, I was aiming for a similar object to the color cube discussed in the book with its use of the 8 basic colors (0,0,0), (1,0,0), etc.
 I got a little frustrated with the spacing/gap between a few spots because I couldn't get it to line up exactly due
 to the translation values I assigned the keys. I think this may be an interesting shape to test lighting on 
 in the future because it has lots of colors, however given that it is flat it may not be the best testing shape.
 If I do use this object for future testing I will likely recreate it by doing the math to figure out the exact coordinates
 instead of manipulating the individual shapes into place so I won't need to deal with gaps.

## Source Notes
Lib files used for this were intentionally left out to prevent copying incase this assignment is used again. Downside of public repos
