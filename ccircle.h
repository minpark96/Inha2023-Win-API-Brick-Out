#pragma once

#include "framework.h"

class CCircle
{
private:
	double cx, cy;
	double vx, vy;
	double radius;
	bool isMove;
public:
	CCircle();
	~CCircle();

	bool Update(RECT* rectView);
	void Draw(HDC hdc);
	void Reset();
	double distance(const double x, const double y) const;

	double GetCX() const;
	double GetCY() const;
	double GetVX() const;
	double GetVY() const;
	double GetRadius() const;

	void Move();
	void SetCX(double x);
	void SetCY(double y);
	void SetPosition(int flag);
};

