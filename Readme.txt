This is the submission for our raytracer assignment. To run the program, follow the instructions as per the original Readme.MD file. Four shapes have been added, a plane, a quad, a triangle and a obj model. They are used in the included Scenes 3 and 4.

They accept the following options:

triangle:
	p0, p1, p2 are the vertices of the triangle

quad:
	p0, p1, p2, p3 are the vertices of the quad. p1 and p2 will form a diagonal of the quad.

plane: 
	position defines a point on the plane.
	normal defines the normal vector to the plane.

mesh:
	filename is the file location of the .obj file relative to the executable.
	scale is the scalar scale of the mesh. (i.e. all coordinates will be multiplied by this value)
	position is the vector position of the mesh. (i.e. all coordinates are added to these values)