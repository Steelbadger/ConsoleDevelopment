#include "3dCircleSprite.h"



PS2CircleSprite3D::PS2CircleSprite3D():
	conversionMatrix(Matrix4x4::IDENTITY)
{
	Initialise(50.0);
}

PS2CircleSprite3D::PS2CircleSprite3D(float r):
	conversionMatrix(Matrix4x4::IDENTITY)
{
	Initialise(r);
}

PS2CircleSprite3D::~PS2CircleSprite3D()
{
}

void PS2CircleSprite3D::Initialise(float r)
{
	m_CentreWorld.x = 0.0f;
	m_CentreWorld.y = 800.0f;
	m_CentreWorld.z = 1000.0f;				//  Create circle, centre at arbitrary point on plane of board
	m_CentreWorld.w = 1.0f;
	m_z = 128.0f;
	rad = r;
	
	colours[0][0] = 70;
	colours[0][1] = 52;
	colours[0][2] = 10;
	colours[0][3] = 127;
	colours[1][0] = 0;
	colours[1][1] = 0;
	colours[1][2] = 0;
	colours[1][3] = 127;				//  Hardcode colours incase I forget to set them.
	
	for (int i = 0; i < SIDES; i++) {
		m_CircumferenceWorld[i].x = -rad*Sin(i*(360/SIDES)) + m_CentreWorld.x;
		m_CircumferenceWorld[i].z = rad*Cos(i*(360/SIDES)) + m_CentreWorld.z;		// calc all the points on the circumference
		m_CircumferenceWorld[i].y = 0.0f + m_CentreWorld.y;							
		m_CircumferenceWorld[i].w = 1.0f;
	}
}

void PS2CircleSprite3D::MoveTo(const float x, const float y, const float z)
//  Move the circle to a new location.  Badly done, should use a delta distance vector and subtract rather than use sin/cos.
//  But does not slow down framerate and only very rarely used.
{
	m_CentreWorld.x = x;
	m_CentreWorld.y = y;
	m_CentreWorld.z = z;	
	m_CentreWorld.w = 1.0f;
	
	for (int i = 0; i < SIDES; i++) {
		m_CircumferenceWorld[i].x = -rad*Sin(i*(360/SIDES)) + m_CentreWorld.x;
		m_CircumferenceWorld[i].z = rad*Cos(i*(360/SIDES)) + m_CentreWorld.z;
		m_CircumferenceWorld[i].y = 0.0f + m_CentreWorld.y;
	}	
}

void PS2CircleSprite3D::SetTransformation(Matrix4x4 t)
//  Transform all our world points using the given matrix.  Homogeneous coords.
{
	m_CentreWorld = t * m_CentreWorld;
	m_CentreWorld /= m_CentreWorld.w;
								
	for (int i = 0; i < SIDES; i++) {
		m_CircumferenceWorld[i] = t * m_CircumferenceWorld[i];
		m_CircumferenceWorld[i] /= m_CircumferenceWorld[i].w;
	}	
}
void PS2CircleSprite3D::MoveDeltaX(float dx)
{
	m_CentreWorld.x+=dx;
	for (int i = 0; i < SIDES; i++) {
		m_CircumferenceWorld[i].x += dx;
	}		
}

void PS2CircleSprite3D::MoveDeltaY(float dy)
{
	m_CentreWorld.y+=dy;
	for (int i = 0; i < SIDES; i++) {
		m_CircumferenceWorld[i].y += dy;
	}		
}

void PS2CircleSprite3D::MoveDeltaZ(float dz)
{
	m_CentreWorld.z+=dz;
	for (int i = 0; i < SIDES; i++) {
		m_CircumferenceWorld[i].z += dz;
	}		
}

void PS2CircleSprite3D::SetPerspective(float x, float y, float z, float a, float b, float c, float d)
//  No longer used, can create it's own perspective matrix based on camera at (x, y, z)
//  on a plane ax + by + cz = d
{
	Matrix4x4 temp		(d-(b*y+c*z),	a*y,	a*z,		a,
						   b*x,	   d-(a*x+c*z), b*z,		b,
						   c*x,			c*y,  d-(a*x+b*y), 	c,
						  -d*x, 	   -d*y,   -d*z, -(a*x+b*y+c*z));
						  
	conversionMatrix = temp;
}

void PS2CircleSprite3D::SetPerspective(Matrix4x4 PerspectiveMatrix)
//  Used version takes a precalculated matrix and saves to conversionMatrix.
{
	conversionMatrix = PerspectiveMatrix;
}

void PS2CircleSprite3D::WorldToScreen()
//  Calculate all 2D coordinates from 3D vertex coordinates and the perspective matrix.
{								
	m_Centre = conversionMatrix * m_CentreWorld;
	m_Centre /= m_Centre.w;
								
	for (int i = 0; i < SIDES; i++) {
		m_Circumference[i] = conversionMatrix * m_CircumferenceWorld[i];
		m_Circumference[i] /= m_Circumference[i].w;
	}								
}

void PS2CircleSprite3D::SetColours(Vector4 CentC, Vector4 CircC)
//  Centre becomes colour in first Vector, points on circumference become colour of second.
{
	colours[0][0] = static_cast<uint8>(CentC.x);
	colours[0][1] = static_cast<uint8>(CentC.y);
	colours[0][2] = static_cast<uint8>(CentC.z);	
	colours[1][0] = static_cast<uint8>(CircC.x);
	colours[1][1] = static_cast<uint8>(CircC.y);
	colours[1][2] = static_cast<uint8>(CircC.z);

}

void PS2CircleSprite3D::SetColoursWithTransparency(Vector4 CentC, Vector4 CircC)
//  Can set alpha values in here, though they don't really work.
//  Like set colours but uses the alpha value of the colours passed in
{
	SetColours(CentC, CircC);
	colours[0][3] = static_cast<uint8>(CentC.w);
	colours[1][3] = static_cast<uint8>(CircC.w);
}

void PS2CircleSprite3D::RenderWithAlpha() const
{
	VIFDynamicDMA.StartDirect();
	
	// Add the GIFTag
	VIFDynamicDMA.Add128(GS_GIFTAG_BATCH(	SIDES+1,							// NLOOP
											1,							// EOP
											1,							// PRE
											GS_PRIM(PRIM_TRI_FAN, 	// PRIM
												PRIM_IIP_GOURAUD, 
												PRIM_TME_OFF,
												PRIM_FGE_OFF, 
												PRIM_ABE_ON, 
												PRIM_AA1_OFF, 
												PRIM_FST_UV, 
												PRIM_CTXT_CONTEXT1, 
												PRIM_FIX_NOFIXDDA),
											GIF_FLG_PACKED,					//FLG
											GS_BATCH_2(	GIF_REG_RGBAQ, 
														GIF_REG_XYZ2)));	//BATCH	
		
	// Add the colour (Centre)
	VIFDynamicDMA.Add128(PACKED_RGBA(colours[0][0] ,colours[0][1], colours[0][2], colours[0][3]));
		    	
	// Add the position (Centre)  	   	
	VIFDynamicDMA.Add128(PACKED_XYZ2((int)((2048.0f + m_Centre.x)*16.0f), 
									 (int)((2048.0f + m_Centre.y)*16.0f),  
									 (int)m_z, 
									  0));
	
	
	for (int i = 0; i < SIDES; i++) {
		// Add the colour (Circumference)
		VIFDynamicDMA.Add128(PACKED_RGBA(colours[1][0] ,colours[1][1], colours[1][2], colours[1][3]));
					
		// Add the position (Circumference)  	
		VIFDynamicDMA.Add128(PACKED_XYZ2((int)((2048.0f + m_Circumference[i].x)*16.0f), 
										 (int)((2048.0f + m_Circumference[i].y)*16.0f),  
										 (int)m_z, 
										  0));
	}
	VIFDynamicDMA.EndDirect();
}

void PS2CircleSprite3D::Render() const
{
	VIFDynamicDMA.StartDirect();
	
	// Add the GIFTag
	VIFDynamicDMA.Add128(GS_GIFTAG_BATCH(	SIDES+1,							// NLOOP
											1,							// EOP
											1,							// PRE
											GS_PRIM(PRIM_TRI_FAN, 	// PRIM
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
		
	// Add the colour (Centre)
	VIFDynamicDMA.Add128(PACKED_RGBA(colours[0][0] ,colours[0][1], colours[0][2], colours[0][3]));
		    	
	// Add the position (Centre)  	   	
	VIFDynamicDMA.Add128(PACKED_XYZ2((int)((2048.0f + m_Centre.x)*16.0f), 
									 (int)((2048.0f + m_Centre.y)*16.0f),  
									 (int)m_z, 
									  0));
	
	
	for (int i = 0; i < SIDES; i++) {
		// Add the colour (Circumference)
		VIFDynamicDMA.Add128(PACKED_RGBA(colours[1][0] ,colours[1][1], colours[1][2], colours[1][3]));
					
		// Add the position (Circumference)  	
		VIFDynamicDMA.Add128(PACKED_XYZ2((int)((2048.0f + m_Circumference[i].x)*16.0f), 
										 (int)((2048.0f + m_Circumference[i].y)*16.0f),  
										 (int)m_z, 
										  0));
	}
	VIFDynamicDMA.EndDirect();
}