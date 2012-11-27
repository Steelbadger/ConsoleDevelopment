#include "3dSprite.h"


PS2Sprite3D::PS2Sprite3D():
	conversionMatrix(Matrix4x4::IDENTITY)
{
	Vector4 temp1(-1,-1,0,1);
	Vector4 temp2(1,-1,0,1);
	Vector4 temp3(1,1,0,1);
	Vector4 temp4(-1,1,0,1);
	Initialise(temp1, temp2, temp3, temp4);
}

PS2Sprite3D::PS2Sprite3D(Vector4 Vertex1, Vector4 Vertex2, Vector4 Vertex3, Vector4 Vertex4):
	conversionMatrix(Matrix4x4::IDENTITY)
{
	Initialise(Vertex1, Vertex2, Vertex3, Vertex4);
}

void PS2Sprite3D::Initialise(Vector4 Vertex_1, Vector4 Vertex_2, Vector4 Vertex_3, Vector4 Vertex_4)
//  Initialise the vertex points.  m_z, m_x, m_y etc are now not used.
//  Set the perspective matrix to the identity matrix.
{
	m_z = 128.0f;
	m_x = 0.0f;
	m_y = 0.0f;
	m_w = m_h = m_w_orig = m_h_orig = 32.0f;
	m_r = m_g = m_b = m_a = 0x80;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			colours[i][j] = m_r;
		}
	}

	m_TopLeftWorld = Vertex_1;
	m_BottomLeftWorld = Vertex_4;
	m_TopRightWorld = Vertex_2;
	m_BottomRightWorld = Vertex_3;
	
	SetPerspective(Matrix4x4::IDENTITY);
}

void PS2Sprite3D::WorldToScreen()
//  Use homogeneous coordinates to convert 3D points to 2D points
{
	m_TopLeft = conversionMatrix * m_TopLeftWorld;
	m_TopRight = conversionMatrix * m_TopRightWorld;
	m_BottomLeft = conversionMatrix * m_BottomLeftWorld;
	m_BottomRight = conversionMatrix * m_BottomRightWorld;
	m_TopLeft /= m_TopLeft.w;
	m_TopRight /= m_TopRight.w;
	m_BottomLeft /= m_BottomLeft.w;
	m_BottomRight /= m_BottomRight.w;
}

void PS2Sprite3D::MoveTo(float x, float y, float z)
//  Move sprite to new location, maintain orientation
//  just subtract a vector from the position vectors to
//  find new points.
{
	float dx = m_TopLeftWorld.x - x;
	float dy = m_TopLeftWorld.y - y;
	float dz = m_TopLeftWorld.z - z;
	
	Vector4 temp(dx, dy, dz, 1);
	
	m_TopLeftWorld = m_TopLeftWorld - temp;
	m_TopRightWorld = m_TopRightWorld - temp;
	m_BottomLeftWorld = m_BottomLeftWorld - temp;
	m_BottomRightWorld = m_BottomRightWorld - temp;
	
	WorldToScreen();		//  Recalculate the screen coordinates
}

void PS2Sprite3D::SetPerspective(float x, float y, float z, float a, float b, float c, float d)
//  Create the transformation matrix corresponding to a camera position at (x, y, z) on the plane
//  ax + by + cz = d.
{
	Matrix4x4 temp		(d-(b*y+c*z),	a*y,	a*z,		a,
						   b*x,	   d-(a*x+c*z), b*z,		b,
						   c*x,			c*y,  d-(a*x+b*y), 	c,
						  -d*x, 	   -d*y,   -d*z, -(a*x+b*y+c*z));
						  
	conversionMatrix = temp;
}

void PS2Sprite3D::SetPerspective(Matrix4x4 PerspectiveMatrix)
//  Receive the perspective matrix from class higher up the hierarchy
{
	conversionMatrix = PerspectiveMatrix;
}

void PS2Sprite3D::SetColours()
//  Debug for setting hard-coded colour
{
	colours[0][0] = 127;
	colours[0][1] = 0;
	colours[0][2] = 0;
	colours[1][0] = 0;
	colours[1][1] = 127;
	colours[1][2] = 0;
	colours[2][0] = 0;
	colours[2][1] = 0;
	colours[2][2] = 127;
}
void PS2Sprite3D::SetColours(Vector4 Vert1Colour, Vector4 Vert2Colour, Vector4 Vert3Colour, Vector4 Vert4Colour)
//  Use vector4s for colours for simplicity, cast to uint8s then assign to the colour array
//  alpha not supported
{
	colours[0][0] = static_cast<uint8>(Vert1Colour.x);
	colours[0][1] = static_cast<uint8>(Vert1Colour.y);
	colours[0][2] = static_cast<uint8>(Vert1Colour.z);
	colours[1][0] = static_cast<uint8>(Vert2Colour.x);
	colours[1][1] = static_cast<uint8>(Vert2Colour.y);
	colours[1][2] = static_cast<uint8>(Vert2Colour.z);
	colours[2][0] = static_cast<uint8>(Vert3Colour.x);
	colours[2][1] = static_cast<uint8>(Vert3Colour.y);
	colours[2][2] = static_cast<uint8>(Vert3Colour.z);
	colours[3][0] = static_cast<uint8>(Vert4Colour.x);
	colours[3][1] = static_cast<uint8>(Vert4Colour.y);
	colours[3][2] = static_cast<uint8>(Vert4Colour.z);	
}
void PS2Sprite3D::MoveDeltaX(float x)
//  Move then recalculate 2D screen coords
{
	m_TopLeftWorld.x+=x;
	m_TopRightWorld.x+=x;
	m_BottomRightWorld.x+=x;
	m_BottomLeftWorld.x+=x;

	WorldToScreen();
}
void PS2Sprite3D::MoveDeltaY(float y)
//  Move then recalculate 2D screen coords
{
	m_TopLeftWorld.y+=y;
	m_TopRightWorld.y+=y;
	m_BottomRightWorld.y+=y;
	m_BottomLeftWorld.y+=y;
	
	WorldToScreen();
}

void PS2Sprite3D::MoveDeltaZ(float z)
//  Move then recalculate 2D screen coords
{
	m_TopLeftWorld.z+=z;
	m_TopRightWorld.z+=z;
	m_BottomRightWorld.z+=z;
	m_BottomLeftWorld.z+=z;
	
	WorldToScreen();
}

void PS2Sprite3D::SetTransformation(Matrix4x4 t)
//  Use homogeneous coordinates to move the 3D coordinates in 3D space
{
	m_TopLeftWorld = t * m_TopLeftWorld;
	m_TopRightWorld = t * m_TopRightWorld;
	m_BottomLeftWorld = t * m_BottomLeftWorld;
	m_BottomRightWorld = t * m_BottomRightWorld;
	m_TopLeftWorld /= m_TopLeftWorld.w;
	m_TopRightWorld /= m_TopRightWorld.w;
	m_BottomLeftWorld /= m_BottomLeftWorld.w;
	m_BottomRightWorld /= m_BottomRightWorld.w;
}

float PS2Sprite3D::GetCentreX()
//  Get average of x of all verts
{
	return ((m_TopLeftWorld.x + m_TopRightWorld.x + m_BottomLeftWorld.x + m_BottomRightWorld.x)/4);
}

float PS2Sprite3D::GetCentreY()
//  Get average of y of all verts
{
	return((m_TopLeftWorld.y + m_TopRightWorld.y + m_BottomLeftWorld.y + m_BottomRightWorld.y)/4);
}

float PS2Sprite3D::GetCentreZ()
//  Get average of z of all verts
{
	return((m_TopLeftWorld.z + m_TopRightWorld.z + m_BottomLeftWorld.z + m_BottomRightWorld.z)/4);
}