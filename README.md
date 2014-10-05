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

### FindTangPoint

*Added by Aaron* 

Finds the tangent point on the asteroid between your location and a given target. Useful for movement. It is important to note that if the sphere can move to a point without going around the asteroid, this method will NOT take that into account. Requires **angleBetween**.




