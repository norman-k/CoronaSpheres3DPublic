CoronaSpheres3D
===============

For those who like to think ahead.

### Rotation

*Added by Elvin* 

Generalized Tilted Axis Rotation

Currently has parameters for axis, degrees of tilting, and direction

Right now, it is programmed to begin stopping after specified degrees of rotation rather than stop entirely.

In the future, desired angular velocity and general speed optimizations will be added.

The algorithm relies on vector algebra (namely the cross product). 

There's also some general vector functions that I've broken up that you can use: **angleBetween** and **angleBetweenAtt**.
They'll give you the angle between generalized vectors and normalized vectors, respectively (in 3-space). 

### Revolution

*Added by Elvin* 

Attempt #1 at general revolution. 
Code runs successfully in some situations.
In others, it only revolves properly for some of the time or gets stuck in the setup process. 
The main issue is preventing the radius from decreasing as time passes.

### Revolution 2

*Added by Elvin* 

Attempt #2 at general revolution.
A work in progress - does not work/run properly right now. 
Some of the code in it is scratch work and includes attempts at uniform circular motion. 
To be scrapped for a Revolution 3! 

### FindTangPoint

*Added by Aaron* 

Finds the tangent point on the asteroid between your location and a given target. Useful for movement. It is important to note that if the sphere can move to a point without going around the asteroid, this method will NOT take that into account. Requires **angleBetween**.




