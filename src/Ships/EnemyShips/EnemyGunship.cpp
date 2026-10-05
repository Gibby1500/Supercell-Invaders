#include "EnemyGunship.h"

EnemyGunship::EnemyGunship(int xpos, int ypos) 
    : EnemyShip(xpos, ypos, 3.5, 10, 75),
      health(3),  // Takes 3 hits to destroy
      timeSinceLastHit(0),
      damageColor(255, 100, 100) {
    
    amIBoss = false;
    enemyShipSprite.load("ShipModels/newEnemy-min.png");
    enemyShipSprite.rotate90(2);
    enemyHitBox = new HitBox(pos, enemyShipSprite.getWidth() * 0.5, enemyShipSprite.getHeight() * 0.5);
}

void EnemyGunship::update(const ofPoint& playerPos) {
    if (isDead()) return;  // Don't update if dead
    
    // Visual feedback timer
    if (timeSinceLastHit < 1.0f) {
        timeSinceLastHit += ofGetLastFrameTime();
    }
    
    // Original movement code
    ofVec2f toPlayer = playerPos - pos;
    float angleToPlayer = ofRadToDeg(-atan2(toPlayer.x, toPlayer.y));
    shipOrientation = angleToPlayer;
    pos += toPlayer.getNormalized() * speed;
    enemyHitBox->box.setPosition(pos.x - 15, pos.y - 10);

    // Shooting logic
    this->shotTimer++;
    if (shotTimer % 100 == 0) {
        shoot();
        shotTimer = 0;
    }
}

void EnemyGunship::draw() {
    if (isDead()) return;
    
    ofPushMatrix();
    ofTranslate(pos.x, pos.y);
    ofRotateDeg(shipOrientation);
    
    // Flash effect when hit
    if (timeSinceLastHit < 0.3f) {
        ofSetColor(damageColor);
    } else {
        ofSetColor(255, 255, 255);
    }
    
    enemyShipSprite.draw(-32, -32, 60, 60);
    ofPopMatrix();
    
    if (showHitboxes) {
        enemyHitBox->draw();
    }
}

void EnemyGunship::takeDamage(int damage) {
    health -= damage;
    timeSinceLastHit = 0;  // Reset visual feedback timer
    
    // Play hit sound
    SoundManager::playSong("enemyHit", false);
}

bool EnemyGunship::isDead() const {
    return health <= 0;
}

void EnemyGunship::shoot() {
    // Create a new projectile and add it to the list of bullets
    Projectiles p = Projectiles(pos, shipOrientation + 90);
    p.setColors(ofColor::red, ofColor::orange);
    enemyBullets.push_back(p);
    SoundManager::playSong("bulletSound", false);
}
