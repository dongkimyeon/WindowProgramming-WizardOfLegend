#pragma once	
#include "CommonInclude.h"

struct Vector2
{
    static Vector2 One;
    static Vector2 Zero;

    float x;
    float y;

    Vector2()
        : x(0.0f)
        , y(0.0f)
    {
    }

    Vector2(float _x, float _y)
        : x(_x)
        , y(_y)
    {
    }

    // ���� ������
    Vector2 operator+(Vector2 other) const
    {
        return Vector2(x + other.x, y + other.y);
    }

    // ���� ������
    Vector2 operator-(Vector2 other) const
    {
        return Vector2(x - other.x, y - other.y);
    }

    // ��Į�� ������ ������
    Vector2 operator/(float value) const
    {
        return Vector2(x / value, y / value);
    }

    // ��Į�� ���� ������ (ī�޶� zoom ����)
    Vector2 operator*(float value) const
    {
        return Vector2(x * value, y * value);
    }

    // ���� �� ��ǥ�� ���� (component-wise multiplication)
    Vector2 operator*(Vector2 other) const
    {
        return Vector2(x * other.x, y * other.y);
    }

    // ���� �� ������
    bool operator==(Vector2 other) const
    {
        return x == other.x && y == other.y;
    }

    // �񵿵� �� ������
    bool operator!=(Vector2 other) const
    {
        return !(*this == other);
    }

    // ���� ������: ����
    Vector2& operator+=(Vector2 other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    // ���� ������: ����
    Vector2& operator-=(Vector2 other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    // ���� ũ�� (Length) ���
    float Length() const
    {
        return sqrtf(x * x + y * y);
    }

    // ���� ����ȭ (���� ���� ��ȯ)
    Vector2 Normalize() const
    {
        float len = Length();
        if (len == 0.0f)
            return Vector2(0.0f, 0.0f);
        return Vector2(x / len, y / len);
    }

    // ���� (Dot Product)
    float Dot(Vector2 other) const
    {
        return x * other.x + y * other.y;
    }
};