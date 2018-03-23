# 2D Shape Modeler

*You must work individually on this assignment. To receive credit, push all
required materials to your own private fork of this repository by the due date
(see the [syllabus](https://bitbucket.org/msu-cs/csci441-spring2018) for an
up-to-date schedule of assignments and due dates).*

## Summary

Over the first few labs, we saw how to draw (and move) triangles and squares,
but the world is much interesting than these simple shapes.  We could make our
shapes by hand, but in general, it is easier to use programs to create more
interesting shapes (often called a modeler).

In this program, we will build a very simple 2D shape modeler.
The program will have two modes, view and edit.

In view mode, the user may use the mouse to move around the viewport into the
world and the `-` and `=` keys to increase or decrease the viewport size (which
will scale down or up the size of what the user sees).

In edit mode, the user can be in one of three sub-modes:

1. In stamp sub-mode, the user creates objects from primitives.  First, they
   select a primitive from the set of triangle, square, or circle. Second, they
   determine the position of the primitive. Third, they determine the scale of
   the primitive (perhaps giving a different scale to *x* and *y*). Fourth
   determine the rotation of the primitive.  After the fourth step, the
   primitive becomes a shape, which is useful in the other modes.
2. In user defined primitive mode, the user creates a primitive by specifying a
   collection of points.  After creating the primitive, the user can perform all
   the same actions as on a primitive in (1) and the primitive becomes a shape.
3. In group mode, the user can either group or ungroup a collection of shapes.
   When grouping, the user can select a collection of shapes and group shapes
   together to form a more complex shape.  That is, grouping the shapes produces
   the union of the collection of shapes.  When ungrouping, the shape is
   separated into the collection of shapes that created it. Grouping and
   ungrouping should be supported multiple times (that is, you can group a shape
   that itself is a group of shapes. You will need to decide how to handle
   ungrouping a shape that is not formed by grouping a collection of shapes.
4. In modify mode, the user can translate, scale, a rotate a shape. The
   user must be able to specify the origin of the transformation.  Also, the
   user must be able to update the shapes color.

## Required Materials

Your program1 directory must include:

* All source code for the completed program
* A screenshot of your program displaying a shape (or collection of shapes) that
  you created using your software.
* A README.md file with any necessary instructions for using your program

## Detailed Requirements

*Your program must*

*Be an original program written by you.* You may use code from labs as a
starting point. You may talk with other students about the program, but looking
at their code is not allowed.

*Respond to mouse click and drag events.* How and where you use mouse input is
up to you.  You do not need to use mouse input only, but it must be used
somewhere.

*Be able to support different primitives.* At a minimum, you must be able to
support the primitives triangles, squares, and circles. You are encouraged to
include other shapes as well. Responding to keyboard events is probably the
easiest way to allow the user to change what shape is drawn (pressing 's' to
start drawing squares or 'c' to start drawing circles, for example, or cycling
through different available shapes by pressing 's'). It is up to you to decide
how the user is able to change the shape, but be sure to document how it is done
in your README file.

*Be able to perform linear transforms on the shapes.* Transformations to the
current shape can't affect any shapes that have already been drawn.  Be sure to
document how the user is able to transform the shapes in your README file.

*Be able to assign colors to the shapes.* At a minimum, your program must be
able to change the color of an entire shape.  You are encouraged to add
additional functionality to allow for changing the color of portions of a shape.
Be sure to document any extra color features you implement in your README file.

*Allow resizing of the window.* When the window is resized the shapes must
maintain the correct aspect ratio (no non-uniform scaling). You can decide what
coordinate space you want to store your shapes in, but you must use an
orthographic projection matrix to ensure proper scaling.

*Include a README.md file.* The README file must include any special
instructions on how to use your program. It must also include a writeup (about a
paragraph or two long) about the shape you created. Why did you create it? What
about it is interesting? What do you like about it?  You are encouraged to crate
a shape that is meaningful enough that you may want to use it as a test object
in future labs, which will help when answering the questions above.

## Point breakdown

| Points | Description |
|--------|-------------|
| 20     | View behavior
| 10     | Stamp mode behavior
| 10     | User defined primitive mode behavior
| 10     | Transformations in Stamp mode and User Defined Primitive Mode
| 10     | Group mode
| 10     | Transformations in modify mode
| 10     | Color changing in modify mode
| 20     | General (code style, execution, creativity, self expression and README.md) |
| *100*  | *Total points* |

