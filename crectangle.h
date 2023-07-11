#pragma once

#include "framework.h"

class CCircle;
class CRectangle
{
private:
	double cx, cy;
	double width, height;
	double left, right, top, bottom;
	int hp;
	COLORREF color;
public:
	CRectangle();
	CRectangle(double cx, double cy, int hp, int color);
	~CRectangle();

	void Draw(HDC hdc);
	void Update(RECT* rectview, double v);
	void Reset();
	int Collision(CCircle& cir);

	int GetHp() const;
	COLORREF GetColor() const;

	void SetColor(int hp);
};

