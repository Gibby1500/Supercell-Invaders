#include "Timer.h"

void Timer::start(float durationSeconds) {
    startTime = ofGetElapsedTimef();
    duration = durationSeconds;
    running = true;
}

void Timer::stop() { // stops timer
    running = false;
}

bool Timer::isRunning() const { // getter for if its running
    return running;
}

bool Timer::isDone() const { // checks if timer is done
    if (!running){
        return false;
    }
    
    float currentTime = ofGetElapsedTimef(); 
    float elapsedTime = currentTime - startTime;
    
    return (elapsedTime >= duration);
}

float Timer::getElapsed() const { // getter for elapsed time
    if (!running){
        return 0.0f;
    }
    
    float currentTime = ofGetElapsedTimef();
    return currentTime - startTime;
}

float Timer::getRemaining() const { // getter for remaining time
    if (!running){
        return 0.0f;
    }
    
    float elapsed = getElapsed();
    return duration - elapsed;
}