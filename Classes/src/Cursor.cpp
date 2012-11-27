#include "Cursor.h"

Cursor::Cursor()
{
}

Cursor::~Cursor()
{
}

void Cursor::Initialise()
{
	float rad = 40.0f;
	float yOffset = 0.0f;
	Vector4 coloursCentre(115,115,115,90);
	Vector4 coloursCirc(100, 100, 100, 127);
	
	for (int i = 0; i < 5; i++) {
		sprites[i].Initialise(rad);												//  Initialise the 5 circle primitives that make up
		sprites[i].MoveTo(0.0f, 800.0f - yOffset, 1000.0f);						//  the cursor.  Taper added by altering rad with each loop
		sprites[i].SetColoursWithTransparency(coloursCentre, coloursCirc);
		rad /= 1.5f;
		yOffset+=50.0f;
	}
	WorldToScreen();					//  Get the cursor ready for rendering
}

void Cursor::SetColourToWhite()
{
	Vector4 coloursCentre(115,115,115,90);
	Vector4 coloursCirc(100, 100, 100, 127);
	
	for (int i = 0; i < 5; i++) {
		sprites[i].SetColoursWithTransparency(coloursCentre, coloursCirc);		//  Attempt at using alpha, doesn't work.
	}

}

void Cursor::SetColourToBlack()
{
	Vector4 coloursCentre(127,127,127, 127);
	Vector4 coloursCirc(100, 100, 100, 70);
	
	for (int i = 0; i < 5; i++) {
		sprites[i].SetColoursWithTransparency(coloursCentre, coloursCirc);
	}
}

void Cursor::MoveTo(float x, float y, float z)
{
	float yOffset = 0.0f;
	for (int i = 0; i < 5; i++) {
		sprites[i].MoveTo(0.0f, 800.0f - yOffset, 1000.0f);
		yOffset+=25.0f;
	}

}

void Cursor::MoveDeltaX(float dx)
{
	for (int i = 0; i < 5; i++) {
		sprites[i].MoveDeltaX(dx);
		WorldToScreen();
	}
}

void Cursor::MoveDeltaY(float dy)
{
	for (int i = 0; i < 5; i++) {
		sprites[i].MoveDeltaY(dy);
		WorldToScreen();
	}
}
void Cursor::MoveDeltaZ(float dz)
{
	for (int i = 0; i < 5; i++) {
		sprites[i].MoveDeltaZ(dz);
		WorldToScreen();
	}
}

void Cursor::DrawBase()
{
	sprites[0].RenderWithAlpha();
}

void Cursor::DrawSpire()
{
	for (int i = 1; i < 5; i++) {
		sprites[i].RenderWithAlpha();
	}
}

void Cursor::WorldToScreen()
{
	for (int i = 0; i < 5; i++) {
		sprites[i].WorldToScreen();
	}
}

void Cursor::SetPerspective(Matrix4x4 m)
{
	for (int i = 0; i < 5; i++) {
		sprites[i].SetPerspective(m);
	}
}

void Cursor::SetTransformation(Matrix4x4 m)
{
	for (int i = 0; i < 5; i++) {
		sprites[i].SetTransformation(m);
	}
}
