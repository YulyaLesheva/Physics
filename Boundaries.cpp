#include "stdafx.h"
#include "Boundaries.h"
#include "Math.h"

Boundaries::Boundaries(int thickPx)
	:thick_px(thickPx)
{
	//SetTexture(Core::resourceManager.Get<Render::Texture>("Boundaries"));
	w = Render::device.Width();
	h = Render::device.Height();
	//width = FPoint(texture->Width(), texture->Height());
	mass = 0;
	anchored = false;
	//float xStart, float xEnd, float yStart, float yEnd
	up    = IRect(FRect(0.0, w, h-thick_px, h));
	down  = IRect(FRect(0.0, w, 0.0, thick_px));
	left  = IRect(FRect(0.0, thick_px, 0.0, h));
	right = IRect(FRect(w-thick_px, w, 0.0, h));

	allBoundaries = {
		up, down, left, right
	};
}
Boundaries* Boundaries::Create(int thickPx) {
	return new Boundaries(thickPx);
}

void Boundaries::Draw() {
	Render::device.PushMatrix();
	Render::BeginColor(Color(100, 200, 100, 255)); // Color(Red, Green, Blue, Alpha)
	//Render::device.MatrixTranslate(GetWidth(UP).x * -0.5f, GetWidth(UP).y * -0.5f, 0);
	Render::DrawRect(up);
	Render::DrawRect(down);
	Render::DrawRect(left);
	Render::DrawRect(right);
	Render::EndColor();
	Render::device.PopMatrix();
}

FPoint Boundaries::GetWidth(BOUNDARY type) {

	switch (type)
	{
	case UP:{
		return (FPoint(up.width, up.height));
	}
		break;
	case DOWN:{
		return (FPoint(down.width, down.height));
	}
		break;
	case LEFT:{
		return (FPoint(left.width, left.height));
	}
		break;
	case RIGHT:{
		return (FPoint(right.width, right.height));
	}
		break;
	}
}

FPoint Boundaries::GetWidth(IRect rect) {
	return FPoint(rect.width, rect.height);
}

FPoint Boundaries::GetPosition(IRect rect) {
	return (FPoint(rect.LeftBottom().x + rect.width * 0.5, rect.LeftBottom().y + rect.height * 0.5));
}