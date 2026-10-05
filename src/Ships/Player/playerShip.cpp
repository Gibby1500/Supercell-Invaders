#include "playerShip.h"


//Parametrized Constructor for the playerShip
Player::Player(int Xposition, int Yposition){
    pos.x = Xposition;
    pos.y = Yposition;
    health = 100;
    velocity.set(0, 0);
    this->defaultShip.load("ShipModels/shipModel2.png");
    this->upgradedShip.load("ShipModels/secondShip.png");
    this->shipOrientation = 0;
    accelerationAmount = 5.0; // Adjust the value as needed
    score = 0;
    if(!this->hasUpgradedShip){
        shipSprite = defaultShip;
    }
    else{
        shipSprite = upgradedShip;
    }
    hitBox =  HitBox(pos, shipSprite.getWidth() * 0.25, shipSprite.getHeight() * 0.15);
    lastShotTime = 0;
    shotCooldown = 0.1;  // Set the cooldown duration to 0.5 seconds (adjust as needed)

    shieldCharge = 0.0f;
    isShieldActive = false;
    isColliding = false;
    collisionDamageTimer = 0.0f;

}

Player::Player() : Player(ofGetWidth() / 2, ofGetHeight() / 2) {}

float Player::getShieldCharge(){ // getter for shield charge
    return shieldCharge;
}
bool Player::getIsShieldActive(){ // getter for isShieldActive
    return isShieldActive;
}
void Player::setIsShieldActive(bool active){
    isShieldActive = active;
}
void Player::setShieldCharge(float newCharge){
    shieldCharge = newCharge;
}
int Player::getScore() { return score; }
void Player::setScore(int score) { this->score = score; }

void Player::bombSetup(){
    bombSprite.load("CompressedImages/Bomb-min.png"); // loads the shield sprite
    if(getHasBomb()) {
        bombSprite.draw(ofGetWidth() - 150, 30, 50, 50);
    }
}
bool Player::getHasBomb(){
    return hasBomb;
}

void Player::setHasBomb(bool availability){
    hasBomb = availability;
}

void Player::shieldSetup(){
    shieldSprite.load("CompressedImages/ForceShield.png"); // loads the shield sprite
    drawShield();
}
void Player::drawShield() {
    if (isShieldActive) {
        shieldSprite.draw(pos.x - 40, pos.y - 40, 80, 80);  // makes it so that the shield is on the players position
    }
}

void Player::addShieldCharge(float amount){
    if(!getIsShieldActive()){
        setShieldCharge(std::min(shieldCharge + amount, 100.0f)); // the min caps it at 100%
    }
}

void Player::activateShield(){
    if(getShieldCharge() >= 100.0 && !shieldTimer.isRunning()){
        setIsShieldActive(true);
        shieldTimer.start(shieldDuration); // starts the 5 second countdown
    }
}

void Player::updateShield() {
    if (isShieldActive) {
        shieldCharge = 100.0 * (1.0 - (shieldTimer.getElapsed() / shieldDuration)); // updates the shield charge
        if (shieldTimer.isDone()) {//checks if timer is over
            isShieldActive = false;
            shieldCharge = 0.0f;
            shieldTimer.stop();
        }
    }
}

// Player::Player() : hasUpgradedShip == false {
//     // Load ship images
//     defaultShip.load("images/player/default.png");
//     upgradedShip.load("images/player/upgraded.png");
    
    // Error handling if images fail to load
// }

void Player::draw() {
    ofPushMatrix();
    ofTranslate(pos.x, pos.y);
    ofRotateDeg(shipOrientation);
    
    // Draw appropriate ship
    if(hasUpgradedShip && upgradedShip.isAllocated()) {
        upgradedShip.draw(-32, -32, 64, 64);
    } 
    else if(defaultShip.isAllocated()) {
        defaultShip.draw(-32, -32, 64, 64);
    }
    
    ofPopMatrix();
                
        // Draw the hitbox around the player ship. Uncomment this line for testing purposes
            if(showHitbox)  this->hitBox.draw();

            if (isColliding) {
                ofPushStyle();
                float alpha = 100 + 100 * sin(ofGetElapsedTimef() * 10);
                ofSetColor(255, 0, 0, alpha);
                ofNoFill();
                ofDrawCircle(pos.x, pos.y, hitBox.box.getWidth() * 0.5f * 1.3f);
                ofPopStyle();
            }
}

void Player::upgradeShip() {
    if(hasUpgradedShip) return;  // Prevent multiple upgrades
    
    hasUpgradedShip = true;
    
    // Notify any observers
    ofNotifyEvent(shipUpgradedEvent);
}

void Player::reset(){
    bullets.clear(); // clears the active bullets
    respawn(); // calls the respawn function
}

void Player::respawn(){
    health = 100; // Recovers the health back to 100
    pos.x = ofGetWidth() / 2; // resets x value to center
    pos.y = ofGetHeight() / 2; // resets y value to center
    velocity.set(0, 0); // stops the movement
}

void Player::update() {

    updateShield();

    processPressedKeys();  // Process the pressed keys and calculate orientation change

    velocity.limit(maxSpeed); // Limit the velocity to the maximum speed
            
    pos += velocity; // Update position based on velocity
    this->hitBox.box.setPosition(pos.x - 15, pos.y - 15);
            
    velocity *= damping; // Apply damping to slow down the ship

    draw();  // Draw the ship
    if (isColliding) {
        collisionDamageTimer += ofGetLastFrameTime();
        if (collisionDamageTimer >= 1.0f/COLLISION_DAMAGE_RATE) {
            health = max(health - 1.0f, 0.0f);
            collisionDamageTimer = 0;
            
            // Visual/audio feedback
            if (ofRandom(1.0f) < 0.3f) {
                SoundManager::playSong("collisionWarning", false);
            }
        }
    } else {
        collisionDamageTimer = 0;
    }
}

void Player::shoot() { 
    // Calculate the current time
    float currentTime = ofGetElapsedTimef();

    // Check if enough time has passed since the last shot
        if (currentTime - lastShotTime >= shotCooldown) {

                Projectiles p = Projectiles(ofPoint(this->pos.x, this->pos.y), this->shipOrientation);
                p.setColors(ofColor::azure, ofColor::blueViolet);
                this->bullets.push_back(p);

            // SoundManager::playSong("bulletSound", false);
            SoundManager::playSong("Beam", false);

            // Update the last shot time to the current time
            lastShotTime = currentTime;
        }
        // Removed redundant shoot() function definition
}


void Player::setShotCooldown(float shotCooldown) { this->shotCooldown = shotCooldown; }

void Player::removeMarkedBullets(){
    bullets.erase(remove_if(bullets.begin(), bullets.end(),
                        [](const Projectiles& p) { return p.markedForDeletion; }),
    bullets.end());
}

void Player::addPressedKey(int key) {
    key = tolower(key);

    keyMap[key] = true;
    isMoving = true; // Set the movement flag
}

void Player::processPressedKeys() {
    if(keyMap['w']) movement('w');
    if(keyMap['s']) movement('s');
    if(keyMap['d']) movement('d');
    if(keyMap['a']) movement('a');

    if(keyMap[' ']) shoot();

            
    if (!isMoving) {
        // Apply damping to gradually slow down the ship when no keys are being pressed
        velocity *= damping; 
    }
}      

void Player::removePressedKey(int key) {
    key = tolower(key);
    keyMap[key] = false;
}

void Player::movement(char keyPressed) {
    ofVec2f acceleration; // Declare acceleration at the beginning of the function
    float currentAccel;
    if (getIsSprinting()) {
        currentAccel = accelerationAmount * 2;  // if true it doubles acceleration when sprinting
    } else {
        currentAccel = accelerationAmount;     // if false it sets it to normal speed
    }
    
    if (keyPressed == 'w') { // handles the foward movement thrust
        acceleration.set(
            cos(ofDegToRad(shipOrientation)) * currentAccel,
            sin(ofDegToRad(shipOrientation)) * currentAccel
        );
    }
    if (keyPressed == 's') { // handles the backwards movement thrust
        acceleration.set(
            -cos(ofDegToRad(shipOrientation)) * currentAccel,
            -sin(ofDegToRad(shipOrientation)) * currentAccel
        );
    }
    // Apply force to velocity
    velocity += acceleration;
        
    if (keyPressed == 'a') {
        // Rotate the ship counterclockwise
        shipOrientation -= rotationSpeed;
        }
    if (keyPressed == 'd') {
        // Rotate the ship clockwise
        shipOrientation += rotationSpeed;
        }
    }   