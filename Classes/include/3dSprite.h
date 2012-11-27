#ifndef _3D_SPRITE_H_
#define _3D_SPRITE_H_

#include "primitives.h"
#include "ps2vector4.h"
#include "ps2matrix4x4.h"

class PS2Sprite3D : public PS2Sprite
{
public:
	PS2Sprite3D();
	PS2Sprite3D(Vector4 Vertex1, Vector4 Vertex2, Vector4 Vertex3, Vector4 Vertex4);
	
	void Initialise(Vector4 Vertex1, Vector4 Vertex2, Vector4 Vertex3, Vector4 Vertex4);		//  Vertices should be entered CLOCKWISE
	
	void SetColours(Vector4 Vert1Colour, Vector4 Vert2Colour, Vector4 Vert3Colour, Vector4 Vert4Colour);   //  colours for vertices, again, clockwise.
	void SetColours();			//  No longer used, debug to set hard-coded colour.
	
	void MoveTo(float x, float y, float z);
	
	void SetPerspective(float x, float y, float z, float a, float b, float c, float d);  //  No longer used as manager calcs this matrix
	void SetPerspective(Matrix4x4 PerspectiveMatrix);									//  and passes it directly
	void SetTransformation(Matrix4x4 TransformationMatrix);			//  This MOVES the 3D vertex points in 3D space to generate rotation.
	void WorldToScreen();			//  Map 3D verts to 2D verts in the base PS2Sprite class using conversionmatrix
	
	float GetCentreX();				//  Gets the AVERAGE of the x coords of all the vertices
	float GetCentreY();				//  Gets the AVERAGE of the y coords of all the vertices
	float GetCentreZ();				//  Gets the AVERAGE of the z coords of all the vertices

	void MoveDeltaX(float x);
	void MoveDeltaZ(float z);
	void MoveDeltaY(float y);
	
private:	
	Vector4 m_TopLeftWorld;			// Homogeneous coords for 3D vertex positions
	Vector4	m_BottomLeftWorld;
	Vector4	m_TopRightWorld;
	Vector4	m_BottomRightWorld;	
	
	Matrix4x4 conversionMatrix;		//  Remember the matrix to use to convert 3D verts to 2D points.

};

#endif