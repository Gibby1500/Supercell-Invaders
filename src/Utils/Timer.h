//Created timer file to facilitate dealing with time based things like powerups and the shield
#pragma once
#include "ofMain.h"

class Timer {
private:
    float startTime = 0.0; 
    float duration = 0.0; 
    bool running = false;
public:
    void start(float durationSeconds); // starts the timer
    void stop(); // stops the timer
    bool isRunning() const; // checks if timer is running
    bool isDone() const; // checks if timer is done
    float getElapsed() const; // getter for time elapsed
    float getRemaining() const; // getter for time remaining
};