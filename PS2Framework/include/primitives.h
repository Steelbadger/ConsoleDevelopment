#ifndef _PRIMITIVES_H_
#define _PRIMITIVES_H_

#include <sps2tags.h>
#include <sps2regstructs.h>
#include "PS2Defines.h"
#include "ps2vector4.h"
#include "ps2matrix4x4.h"

class PS2Sprite 
{
public:

	PS2Sprite();
	PS2Sprite(const float x, const float y);
	~PS2Sprite();
	
	void Render(void) const;
	
	void MoveTo(const float x, const float y);
	void MoveDeltaX(const float x);
	void MoveDeltaY(const float y);	
	void Move(void);
	
	void ChangeDeltaVX(const float x);
	void ChangeDeltaVY(const float y);
	void SetVelocity(float Vx, float Vy);
	
	float GetX(void) const;
	float GetY(void) const;
	
	void SetColour(uint8 R, uint8 G, uint8 B);
	
	void Scale(const float s);
	void ScaleX(const float s);
	void ScaleY(const float s);
	void Rotate(const float a);	
	
	float MinX();
	float MaxX();
	float MinY();
	float MaxY();
	
protected:
	Vector4 m_TopLeft;			// Homogeneous coords for vertex positions
	Vector4	m_BottomLeft;
	Vector4	m_TopRight;
	Vector4	m_BottomRight;
	
	uint8 colours[4][4];
	void Initialise(void);		// Initialise variables
	
	float m_x, m_y;				// position on screen
	float m_z;					// z depth (big = near)
	float m_w, m_h;				// width and height
	float m_w_orig, m_h_orig;	// width and height upon creation
	uint8 m_r, m_g, m_b, m_a;	// colour and alpha	
	
private:

	float m_Vx, m_Vy;			// velocity vector components
};
#endif