#pragma once
namespace Game {
    const int screenWidth = 800;
    const int screenHeight = 600; 
}

namespace Player {
    float shipX = 350;
    float shipSpeed = 20;
    int currentWeapon = 0;
    int shipLives = 3;
}

namespace Meteor {
    float meteorCooldown = 1.0f;
    float meteorX = 250;
    float meteorY = 0;
    float meteorSpeed = 5;
    float meteorRotation = 0;
    float meteorRotationSpeed = 2;
}

namespace Laser {
    float laserX = 0;
    float laserY = 520;
    float laserSpeed = 10;
}
