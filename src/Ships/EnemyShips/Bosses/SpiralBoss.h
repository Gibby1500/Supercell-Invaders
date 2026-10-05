#pragma once
#include "Boss.h"

class SpiralBoss : public Boss {
    public:
    SpiralBoss(int xpos, int ypos, string name);
    void update(const ofPoint& playerPos) override;
    void draw() override;
    void shoot() override;
    
private:
    float spiralAngle;
    float shootTimer;
    ofPoint shootingPoint;
};