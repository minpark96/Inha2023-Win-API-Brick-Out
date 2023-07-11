#include "ccircle.h"

CCircle::CCircle()
{
    cx = 700;
    cy = 629;
    radius = 10;
    vx = 2;
    vy = -3;
    isMove = false;
}

CCircle::~CCircle()
{
}

bool CCircle::Update(RECT* rectView)
{
    // 왼쪽
    if (cx - radius <= rectView->left)
    {
        vx *= -1;
        cx = rectView->left + radius;
    }
    // 오른쪽
    if (cx + radius >= rectView->right)
    {
        vx *= -1;
        cx = rectView->right - radius;
    }
    // 위
    if (cy - radius <= rectView->top)
    {
        vy *= -1;
        cy = rectView->top + radius;
    }
    // 아래
    if (cy - radius >= rectView->bottom)
    {
        return true;
    }

    if (isMove)
    {
        cx += vx;
        cy += vy;
    }

    return false;
}

void CCircle::Draw(HDC hdc)
{
    Ellipse(hdc, round(cx - radius), round(cy - radius), round(cx + radius), round(cy + radius));
}

void CCircle::Reset()
{
    cx = 700;
    cy = 629;
    vx = 2;
    vy = -3;
    isMove = false;
}

double CCircle::distance(const double x, const double y) const
{
    return sqrt((cx - x) * (cx - x) + (cy - y) * (cy - y));
}

double CCircle::GetCX() const
{
    return cx;
}

double CCircle::GetCY() const
{
    return cy;
}

double CCircle::GetVX() const
{
    return vx;
}

double CCircle::GetVY() const
{
    return vy;
}

double CCircle::GetRadius() const
{
    return radius;
}

void CCircle::Move()
{
    isMove = true;
}

void CCircle::SetCX(double x)
{
    cx = x;
}

void CCircle::SetCY(double y)
{
    cy = y;
}

void CCircle::SetPosition(int flag)
{
    if (flag == TOP || flag == BOTTOM)
        vy *= -1;
    else if(flag == LEFT || flag == RIGHT)
        vx *= -1;
    else
    {
        vx *= -1;
        vy *= -1;
    }
}