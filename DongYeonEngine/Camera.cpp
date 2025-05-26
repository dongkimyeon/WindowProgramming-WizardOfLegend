#include "Camera.h"
#include "Time.h"
Camera::Camera()
    : mX(0)
    , mY(0)
    , mTarget(nullptr)
{
}

Camera::~Camera()
{
}

void Camera::Update()
{
    if (mTarget)
    {
        // Center camera on player (viewport 1280x720)
        mX = mTarget->GetPositionX() - 640.0f; // 1280 / 2
        mY = mTarget->GetPositionY() - 360.0f; // 720 / 2
        // Clamp to map bounds (5000x5000)
        mX = max(0.0f, min(mX, 2000.0f - 1280.0f));
        mY = max(0.0f, min(mY, 2000.0f - 720.0f));
    }
}

void Camera::SetTarget(Player* target)
{
    mTarget = target;
}


