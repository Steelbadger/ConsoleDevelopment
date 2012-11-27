#include <sps2lib.h>
#include <sps2tags.h>
#include <sps2util.h>
#include <sps2regstructs.h>
#include "PS2Defines.h"
#include "sps2wrap.h"
#include "dma.h"
#include "primitives.h"
#include "ps2matrix4x4.h"
#include "ps2vector4.h"
#include "ps2maths.h"


PS2Sprite::PS2Sprite()
{
	m_x = m_y = 0.0f;
	Initialise();
}

PS2Sprite::PS2Sprite(float x, float y)
{
	m_x = x; m_y = y;
	Initialise();
}

PS2Sprite::~PS2Sprite()
{
	// Nothing to clean-up
}

void PS2Sprite::Initialise(void)
{
	m_z = 128.0f;
	m_w = m_h = m_w_orig = m_h_orig = 32.0f;
	m_r = m_g = m_b = m_a = 0x80;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			colours[i][j] = m_r;
		}
	}
	
	m_Vx = m_Vy = 0.2f;
	
	m_TopLeft.x 		= -m_w/2.0f;
	m_TopLeft.y 		= -m_h/2.0f;
	m_TopLeft.z 		= 0.0f;
	m_TopLeft.w 		= 1.0f;
	m_BottomLeft.x 		= -m_w/2.0f;
	m_BottomLeft.y 		=  m_h/2.0f;
	m_BottomLeft.z 		= 0.0f;
	m_BottomLeft.w 		= 1.0f;	
	m_TopRight.x 		=  m_w/2.0f;
	m_TopRight.y 		= -m_h/2.0f;
	m_TopRight.z 		= 0.0f;
	m_TopRight.w 		= 1.0f;	
	m_BottomRight.x 	=  m_w/2.0f;
	m_BottomRight.y 	=  m_h/2.0f;
	m_BottomRight.z 	= 0.0f;
	m_BottomRight.w 	= 1.0f;	
}

void PS2Sprite::MoveTo(const float x, const float y){
	m_x = x;
	m_y = y;
}
	
void PS2Sprite::MoveDeltaX(const float x){
	m_x += x;
}
	
void PS2Sprite::MoveDeltaY(const float y){
	m_y += y;
}

void PS2Sprite::Move(void) {

	if (MaxX() > 320 && m_Vx > 0)
	{
		m_Vx *= -1;
	} else if (MinX() < -320 && m_Vx < 0)
	{
		m_Vx *= -1;
	} else {
		m_x += m_Vx;
	}

	if (MaxY() > 256 && m_Vy > 0)
	{
		m_Vy *= -1;
	} else if (MinY() < -256 && m_Vy < 0)
	{
		m_Vy *= -1;
	} else {
		m_y += m_Vy;
	}	
}
void PS2Sprite::ChangeDeltaVX(const float x){
	m_Vx += x;
}
void PS2Sprite::ChangeDeltaVY(const float y){
	m_Vy += y;
}

float PS2Sprite::GetX(void) const{
	return m_x;
}

float PS2Sprite::GetY(void) const{
	return m_y;
}

void PS2Sprite::SetVelocity(float Vx, float Vy) {
	m_Vx = Vx;
	m_Vy = Vy;
}

void PS2Sprite::SetColour(uint8 R, uint8 G, uint8 B){
	m_r = R;
	m_g = G;
	m_b = B;
}

void PS2Sprite::Scale(const float s) {
	m_w = s * m_w_orig;
	m_h = s * m_h_orig;
	
	m_TopLeft *= s ;
	m_BottomLeft *= s ;
	m_TopRight *= s;
	m_BottomRight *= s;	
	
}

void PS2Sprite::ScaleX(const float s) {
	m_w = s * m_w_orig;
}
void PS2Sprite::ScaleY(const float s) {
	m_h = s * m_h_orig;
}
void PS2Sprite::Rotate(const float a) {
	Matrix4x4 rotationMatrix(1.0f,0.0f,0.0f,0.0f,
							0.0f,1.0f,0.0f,0.0f,
							0.0f,0.0f,1.0f,0.0f,
							0.0f,0.0f,0.0f,1.0f);
	
	
	rotationMatrix.RotationZ(a);
	m_TopLeft = rotationMatrix * m_TopLeft ;
	m_BottomLeft = rotationMatrix * m_BottomLeft ;
	m_TopRight = rotationMatrix * m_TopRight;
	m_BottomRight = rotationMatrix * m_BottomRight;
}

float PS2Sprite::MinX()
{
	float i = Min(m_x + m_TopLeft.x, m_x + m_BottomLeft.x);
	i = Min(i, m_x + m_TopRight.x);
	i = Min(i, m_x + m_BottomRight.x);
	return i;
}

float PS2Sprite::MaxX()
{
	float i = Max(m_x + m_TopLeft.x, m_x + m_BottomLeft.x);
	i = Max(i, m_x + m_TopRight.x);
	i = Max(i, m_x + m_BottomRight.x);
	return i;
}

float PS2Sprite::MinY()
{
	float i = Min(m_y + m_TopLeft.y, m_y + m_BottomLeft.y);
	i = Min(i, m_y + m_TopRight.y);
	i = Min(i, m_y + m_BottomRight.y);
	return i;
}

float PS2Sprite::MaxY()
{
	float i = Max(m_y + m_TopLeft.y, m_y + m_BottomLeft.y);
	i = Max(i, m_y + m_TopRight.y);
	i = Max(i, m_y + m_BottomRight.y);
	return i;
}

// ********* Don't Change anything in this method
void PS2Sprite::Render(void) const
{
	VIFDynamicDMA.StartDirect();
	
	// Add the GIFTag
	VIFDynamicDMA.Add128(GS_GIFTAG_BATCH(	4,							// NLOOP
											1,							// EOP
											1,							// PRE
											GS_PRIM(PRIM_TRI_STRIP, 	// PRIM
												PRIM_IIP_GOURAUD, 
												PRIM_TME_OFF,
												PRIM_FGE_OFF, 
												PRIM_ABE_OFF, 
												PRIM_AA1_OFF, 
												PRIM_FST_UV, 
												PRIM_CTXT_CONTEXT1, 
												PRIM_FIX_NOFIXDDA),
											GIF_FLG_PACKED,					//FLG
											GS_BATCH_2(	GIF_REG_RGBAQ, 
														GIF_REG_XYZ2)));	//BATCH	
		
	// Add the colour (Top Left)
	VIFDynamicDMA.Add128(PACKED_RGBA(colours[0][0] ,colours[0][1], colours[0][2], colours[0][3]));
		    	
	// Add the position (Top Left)  	   	
	VIFDynamicDMA.Add128(PACKED_XYZ2((int)((2048.0f + m_x + m_TopLeft.x)*16.0f), 
									 (int)((2048.0f + m_y + m_TopLeft.y)*16.0f),  
									 (int)m_z, 
									  0));
	
	// Add the colour (Bottom Left)
	VIFDynamicDMA.Add128(PACKED_RGBA(colours[1][0] ,colours[1][1], colours[1][2], colours[1][3]));
		    	
	// Add the position (Bottom Left)  	
	VIFDynamicDMA.Add128(PACKED_XYZ2((int)((2048.0f + m_x + m_BottomLeft.x)*16.0f), 
									 (int)((2048.0f + m_y + m_BottomLeft.y)*16.0f),  
									 (int)m_z, 
									  0));
									  
	
	// Add the colour (Top Right)
	VIFDynamicDMA.Add128(PACKED_RGBA(colours[2][0] ,colours[2][1], colours[2][2], colours[2][3]));
		    	
	// Add the position (Top Right)  	
	VIFDynamicDMA.Add128(PACKED_XYZ2((int)((2048.0f + m_x + m_TopRight.x)*16.0f), 
									 (int)((2048.0f + m_y + m_TopRight.y)*16.0f),  
									 (int)m_z, 
									  0));

	// Add the colour (Bottom Right)
	VIFDynamicDMA.Add128(PACKED_RGBA(colours[3][0] ,colours[3][1], colours[3][2], colours[3][3]));
		    	
	// Add the position  (Bottom Right)  	
	VIFDynamicDMA.Add128(PACKED_XYZ2((int)((2048.0f + m_x + m_BottomRight.x)*16.0f), 
									 (int)((2048.0f + m_y + m_BottomRight.y)*16.0f),  
									 (int)m_z, 
									  0));
	
	
	VIFDynamicDMA.EndDirect();
}