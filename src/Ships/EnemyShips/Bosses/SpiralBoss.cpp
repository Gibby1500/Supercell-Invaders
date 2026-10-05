#include "SpiralBoss.h"

SpiralBoss::SpiralBoss(int xpos, int ypos, string name) : Boss(xpos, ypos, 0.5, 250, name), // 250 HP (between UFO and ORT)
      spiralAngle(0),
      shootTimer(0) {
    // Load spiral boss image from your assets
    enemyShipSprite.load("CompressedImages/Space_Station-min.png");
    
    
    // Centered hitbox
    enemyHitBox = new HitBox(ofPoint(pos.x, pos.y), 
                           enemyShipSprite.getWidth()*0.4f, 
                           enemyShipSprite.getHeight()*0.4f);
    
    // Stationary boss - no movement
    speed = 0;

    shootingPoint = ofPoint(pos.x + enemyHitBox->box.getWidth(), pos.y + enemyHitBox->box.getHeight() / 2);   // Update the shooting point

}

void SpiralBoss::update(const ofPoint& playerPos) {
    if (isDead()) return;
    shootingPoint = ofPoint(pos.x + enemyHitBox->box.getWidth(), pos.y + enemyHitBox->box.getHeight() / 2);   // Update the shooting point

    // Update spiral shooting angle
    spiralAngle += 5.0f; // Rotation speed
    if (spiralAngle >= 360) spiralAngle -= 360;
    
    // Continuous spiral shooting
    shootTimer += ofGetLastFrameTime();
    if (shootTimer >= 0.15f) { // Shoot 6-7 times per second
        shootTimer = 0;
        shoot();
    }

    // Update hitbox position
    enemyHitBox->box.setPosition(pos.x, pos.y);
}

void SpiralBoss::draw() {
    if (isDead() || !enemyHitBox) return;
    
    ofPushMatrix();
    ofTranslate(pos.x, pos.y);
    
    // Draw centered sprite
    if (enemyShipSprite.isAllocated()) {
        enemyShipSprite.draw(-enemyShipSprite.getWidth()/2, -enemyShipSprite.getHeight()/2);
    } else {
        // Fallback drawing
        ofSetColor(100, 200, 255);
        ofDrawCircle(0, 0, 60);
    }
    
    if (showHitboxes && enemyHitBox) {
        enemyHitBox->draw();
    }
    
    ofPopMatrix();
}

void SpiralBoss::shoot() {
    // Fire 3 bullets at slightly different angles
    if(!enemyHitBox) return; // safety check
    for (int i = 0; i < 3; i++) {
        float angle = spiralAngle + (i * 15); // 15 degrees between bullets
        Projectiles bullet(ofPoint(shootingPoint.x, shootingPoint.y), angle);
        bullet.setColors(ofColor::red, ofColor::orange); // Set desired colors
        enemyBullets.push_back(bullet);
        SoundManager::playSong("bulletSound", false);
    }
}