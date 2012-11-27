#ifndef _3D_CIRCLE_SPRITE_H_
#define _3D_CIRCLE_SPRITE_H_

#include <sps2tags.h>
#include <sps2regstructs.h>
#include "PS2Defines.h"
#include "ps2vector4.h"
#include "ps2matrix4x4.h"
#include "dma.h"


class PS2CircleSprite3D
{
public:
	PS2CircleSprite3D();
	PS2CircleSprite3D(float Radius);
	
	void Initialise(float Radius);
	
	~PS2CircleSprite3D();
	
	void Render() const;				//  Modification of normal sprite render to handle more vertices and triangle fan
	void RenderWithAlpha() const;		//  Draw circle and use alpha values.  Doesn't work.
	
	void MoveTo(const float x, const float y, const float z);
	void MoveDeltaX(float dx);
	void MoveDeltaY(float dy);
	void MoveDeltaZ(float dz);
	
	void SetPerspective(float x, float y, float z, float a, float b, float c, float d);  //  No longer used as manager calcs this matrix
	void SetPerspective(Matrix4x4 PerspectiveMatrix);									//  and passes it directly
	void SetTransformation(Matrix4x4 TransformationMatrix);			//  This MOVES the 3D vertex points in 3D space to generate rotation.
	void WorldToScreen();			//  Map 3D verts to 2D verts in the base PS2Sprite class using conversionmatrix
	
	void SetColours(Vector4 CentreColour, Vector4 CircumferenceColour);						//  Set colours, no alpha
	void SetColoursWithTransparency(Vector4 CentreColour, Vector4 CircumferenceColour);		//  Set colours with alpha, still doesn't work.
	
	
	float GetCentreX(){return m_CentreWorld.x;}
	float GetCentreY(){return m_CentreWorld.y;}			//  Get the centre of the circle
	float GetCentreZ(){return m_CentreWorld.z;}

private:
	const static int SIDES = 15;			//  define the number of points on the circumference of my circles

	Vector4 m_Centre;
	Vector4 m_Circumference[SIDES];			//  2D screen coordinates for the circle
	uint8 colours[2][4];
	
	float m_z;
	float rad;
	
	Matrix4x4 conversionMatrix;				//  Store the world to screen conversion matrix

	Vector4 m_CentreWorld;					//  3D space coordinates for the circle.
	Vector4 m_CircumferenceWorld[SIDES];
};

#endif