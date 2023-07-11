#include "crectangle.h"

CRectangle::CRectangle()
{
    width = 150;
    height = 20;
    cx = 700;
    cy = 650;
    left = cx - width / 2;
    right = cx + width / 2;
    top = cy - height / 2;
    bottom = cy + height / 2;
    hp = 0;
    color = RGB(255, 255, 255);
}

CRectangle::CRectangle(double cx, double cy, int hp, int color)
{
    width = 100;
    height = 50;
    this->cx = cx;
    this->cy = cy;
    left = this->cx - width / 2;
    right = this->cx + width / 2;
    top = this->cy - height / 2;
    bottom = this->cy + height / 2;
    this->hp = hp;
    SetColor(color);
}

CRectangle::~CRectangle()
{
}

void CRectangle::Update(RECT* rectView, double v)
{
    // 왼쪽
    if (left <= rectView->left)
    {
        left = rectView->left + 1;
        right = left + width + 1;
        cx = left + width / 2 + 1;
        return;
    }
    // 오른쪽
    if (right >= rectView->right)
    {
        right = rectView->right - 1;
        left = right - width - 1;
        cx = right - width / 2 - 1;
        return;
    }

    left += v;
    right += v;
    cx += v;
}

void CRectangle::Reset()
{
    cx = 700;
    cy = 650;
    left = cx - width / 2;
    right = cx + width / 2;
    top = cy - height / 2;
    bottom = cy + height / 2;
}

void CRectangle::Draw(HDC hdc)
{
    POINT p;
    POINT point[4];

    p.x = round(right);
    p.y = round(bottom);
    point[0] = p;
    p.x = round(left);
    p.y = round(bottom);
    point[1] = p;
    p.x = round(left);
    p.y = round(top);
    point[2] = p;
    p.x = round(right);
    p.y = round(top);
    point[3] = p;

    Polygon(hdc, point, 4);
}

int CRectangle::Collision(CCircle& cir)
{
    int res;

    double x = max(left, min(cir.GetCX(), right));
    double y = max(top, min(cir.GetCY(), bottom));
    double d = cir.distance(x, y);

    if (d <= cir.GetRadius())
    {
        double time;
        double new_cx;
        double new_cY;
        if (hp)
        {
            --hp;
        }

        // 충돌이 발생한 경우 충돌 방향 결정하기
        if (x == left)
        {
            if (y == top || y == bottom)
            {
                
            }
            else
            {
                time = (left - cir.GetRadius() - cir.GetCX()) / cir.GetVX();
                new_cx = cir.GetCX() + cir.GetVX() * time;
                new_cY = cir.GetCY() + cir.GetVY() * time;
                cir.SetCX(new_cx);
                cir.SetCY(new_cY);
                return LEFT;
            }
        }
        else if (x == right)
        {
            if (y == top || y == bottom)
            {
                
            }
            else
            {
                time = (right + cir.GetRadius() - cir.GetCX()) / cir.GetVX();
                new_cx = cir.GetCX() + cir.GetVX() * time;
                new_cY = cir.GetCY() + cir.GetVY() * time;
                cir.SetCX(new_cx);
                cir.SetCY(new_cY);
                return RIGHT;
            }
        }
        else 
        {
            if (y == top)
            {
                time = (top - cir.GetRadius() - cir.GetCY()) / cir.GetVY();
                new_cx = cir.GetCX() + cir.GetVX() * time;
                new_cY = cir.GetCY() + cir.GetVY() * time;
                return TOP;
                
            }
            else if (y == bottom) 
            {
                time = (top - cir.GetRadius() - cir.GetCY()) / cir.GetVY();
                new_cx = cir.GetCX() + cir.GetVX() * time;
                new_cY = cir.GetCY() + cir.GetVY() * time;
                return BOTTOM;
            }
        }

        double CornerX = (abs(cir.GetCX() - left) < abs(cir.GetCX() - right)) ? left : right;
        double CornerY = (abs(cir.GetCY() - top) < abs(cir.GetCY() - bottom)) ? top : bottom;
        double dx = cir.GetCX() - CornerX;
        double dy = cir.GetCY() - CornerY;
        double distance = sqrt(dx * dx + dy * dy);
        double overlap = cir.GetRadius() - distance;
        double angle = atan2(dy, dx);
        new_cx = cir.GetCX() + overlap * cos(angle);
        new_cY = cir.GetCY() + overlap * sin(angle);
        cir.SetCX(new_cx);
        cir.SetCY(new_cY);
        return CORNER; //가장 가까운 점이 모서리인 경우
    }

    return NONE;
}

int CRectangle::GetHp() const
{
    return hp;
}

COLORREF CRectangle::GetColor() const
{
    return color;
}

void CRectangle::SetColor(int color)
{
    switch (color)
    {
    case 1:
    {
        this->color = RGB(255, 0, 0);
    }
    break;
    case 2:
    {
        this->color = RGB(255, 192, 203);
    }
    break;
    case 3:
    {
        this->color = RGB(255, 165, 0);
    }
    break;
    case 4:
    {
        this->color = RGB(144, 238, 144);
    }
    break;
    case 5:
    {
        this->color = RGB(0, 0, 255);
    }
    break;
    case 6:
    {
        this->color = RGB(128, 128, 128);
    }
    break;
    default:
    {
        this->color = RGB(255, 255, 255);
    }
    break;
    }
}

