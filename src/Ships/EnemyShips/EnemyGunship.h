#pragma once
#include "Enemy.h"
#include "ofMain.h"

class EnemyGunship : public EnemyShip {
public:
    EnemyGunship(int xpos, int ypos);
    void update(const ofPoint& playerPos) override;
    void draw() override;
    void shoot() override;
    void takeDamage(int damage) override;
    bool isDead() const;  // Check if the ship is dead
    
private:
    int health;  // Add health tracking
    float timeSinceLastHit;  // For visual feedback
    ofColor damageColor;  // Flash color when hit
};