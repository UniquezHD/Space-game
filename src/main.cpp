#include <raylib.h>
#include <iostream>
#include <string>
#include "variables.h"
#include "main.h"

const int MAX_ASTROIDS = 8;
const int MAX_LASERBEAMS = 8;

int currentScore = 0;

float astroidTimer;

std::string score;

typedef struct Astroid {
    bool isActive;
    Vector2 position;
    float meteorSpeed;
    float rotation;
} Astroid;

typedef struct LaserBeam {
    bool isActive;
    Vector2 position;
    float speed;
    float rotation;
} LaserBeam;

LaserBeam laserBeams[MAX_LASERBEAMS];
Astroid astroids[MAX_ASTROIDS];

int main()
{
    Image icon = LoadImage("resources/png/alien.png");

    InitWindow(Game::screenWidth, Game::screenHeight, "UFO Made by UniquezHD");
    SetWindowIcon(icon);
    SetTargetFPS(60);
    SetExitKey(KEY_END);

    InitAudioDevice();

    Init();

    while (!WindowShouldClose())
    {
        UpdateDraw();
    }

    StopMusicStream(music);

    CloseAudioDevice();

    CloseWindow();
    return 0;
}

void Init(void){
    music = LoadMusicStream("resources/sounds/music.mp3");
    PlayMusicStream(music);
    SetMusicVolume(music, 0.5);

    laserSound = LoadSound("resources/sounds/laser.ogg");

    cannonSound = LoadSound("resources/sounds/cannon.ogg");

    hitSound = LoadSound("resources/sounds/hit.ogg");

    switchSound = LoadSound("resources/sounds/switch.ogg");

    explotion = LoadTexture("resources/png/explotion.png");
    explotion.height = 50;
    explotion.width = 50;

    lives = LoadTexture("resources/png/heart.png");
    lives.height = 25;
    lives.width = 25;

    laser = LoadTexture("resources/png/laser.png");
    laser.height = 50;
    laser.width = 50;

    cannon = LoadTexture("resources/png/cannon.png");
    cannon.height = 50;
    cannon.width = 50;

    meteor = LoadTexture("resources/png/meteor.png");
    meteor.height = 50;
    meteor.width = 50;

    ship = LoadTexture("resources/png/ship.png");
    ship.height = 100;
    ship.width = 100;

    spaceBg = LoadTexture("resources/png/space.png");

    SetGameState(START);
}

void UpdateDraw(void){
    BeginDrawing();

    UpdateMusicStream(music);

    DrawTexture(spaceBg, 0, 0, Green);

    if(GetGameState() == START){
        
        InitGame();

    } else if(GetGameState() == END){

        Ended();

    } else if(GetGameState() == PLAYING) {

        Playing();

    } else if(GetGameState() == PAUSE){

        Paused();

    } else if(GetGameState() == CONTROLLS){

        ShowControlls();

    }

    DrawText("1.0.0", Game::screenWidth - 30, Game::screenHeight - 20, 15, WHITE);
    EndDrawing();
}

void Playing(){

    DrawAstroids();

    UpdateAstroid();

    DrawLaserBeam();

    UpdateLaserBeam();

    DrawRectangle(0, 0, Game::screenWidth, 40, BLACK);

    DrawText("Lives: ", 10, 10, 20, WHITE);

    score = "Score: " + std::to_string(currentScore);

    DrawText(score.c_str(), Game::screenWidth / 2 - MeasureText(score.c_str(), 20), 10, 20, WHITE);

    std::string weaponInfo = "Weapon: " + Weapons[Player::currentWeapon];
    DrawText(weaponInfo.c_str(), 10, 50, 20, WHITE);

    std::string debugInfo = "shipX: " + std::to_string(Player::shipX);
    DrawText(debugInfo.c_str(), 10, 90, 20, WHITE);

    DrawText("Switch Weapon: C", 10, Game::screenHeight - 25, 18, WHITE);
        
    if(Player::shipLives == 3){
        DrawTexture(lives, 70, 10, WHITE);
        DrawTexture(lives, 100, 10, WHITE);
        DrawTexture(lives, 130, 10, WHITE);
    }

    if(Player::shipLives == 2){
        DrawTexture(lives, 70, 10, WHITE);
        DrawTexture(lives, 100, 10, WHITE);
    }

    if(Player::shipLives == 1){
        DrawTexture(lives, 70, 10, WHITE);
    }

    DrawTexture(ship, Player::shipX, Game::screenHeight - 100, WHITE);

    if(Player::shipLives == 0){
        SetGameState(END);
    }

    //DrawTextureEx(laser, {Laser::laserX, Laser::laserY}, 180, 1, WHITE);

    if(IsKeyPressed(KEY_SPACE)){  
        if(Player::currentWeapon == 0){
            PlaySound(laserSound);
        } else if (Player::currentWeapon == 1){
            PlaySound(cannonSound);
        }
        SpawnLaserBeam();
        //Laser::laserY = 520;          
        //Laser::laserX = Player::shipX + 75;     
    }

    //Laser::laserY = Laser::laserY - Laser::laserSpeed;

    if(IsKeyPressed(KEY_ESCAPE)){
        SetGameState(PAUSE);
        PauseMusicStream(music);
    }

    if(IsKeyDown(KEY_A)){
       MoveLeft();
    }

    if(IsKeyDown(KEY_LEFT)){
       MoveLeft();
    }

    if(IsKeyDown(KEY_D)){
        MoveRight();
    }

    if(IsKeyDown(KEY_RIGHT)){
        MoveRight();
    }

    if(IsKeyPressed(KEY_C)){
        PlaySound(switchSound);
        if(Player::currentWeapon == 1){
            Player::currentWeapon = 0;
        } else {
            Player::currentWeapon = Player::currentWeapon + 1;
        }
    }

    if(IsKeyPressed(KEY_M)){
        ToggleMusic();
    }
}

void InitGame(){
    DrawRectangle(0, 0, Game::screenWidth, Game::screenHeight, Fade(PURPLE, 0.2f));
	DrawText("UFO", Game::screenWidth / 2 - MeasureText("UFO", 40) / 2, 50, 40, PURPLE);
	DrawText("Press Enter to start or C for controlls", Game::screenWidth / 2 - MeasureText("Press Enter to start or C for controlls", 20) / 2, Game::screenHeight * 0.75f - 10, 20, PURPLE);
        
    if(IsKeyPressed(KEY_ENTER)){
        Meteor::meteorY = 0;
        Player::shipLives = 3;
        SetGameState(PLAYING);
    }

    if(IsKeyPressed(KEY_C)){
        SetGameState(CONTROLLS);
    }

    if(IsKeyPressed(KEY_M)){
        ToggleMusic();
    }

    astroidTimer = Meteor::meteorCooldown;

    currentScore = 0;

    for (int i = 0; i < MAX_ASTROIDS; i++)
    {
        UnsetAstroid(i);
    }

    for (int i = 0; i < MAX_LASERBEAMS; i++)
    {
        UnsetLaserBeam(i);
    }
}

void Paused(){
    DrawRectangle(0, 0, Game::screenWidth, Game::screenHeight, Fade(PURPLE, 0.2f));
	DrawText("Game Paused", Game::screenWidth / 2 - MeasureText("Game Paused", 40) / 2, 50, 40, PURPLE);
	DrawText("Press Enter/Escape to resume", Game::screenWidth / 2 - MeasureText("Press Enter/Escape to resume", 20) / 2, Game::screenHeight * 0.75f - 10, 20, PURPLE);
    
    if(IsKeyPressed(KEY_ENTER)){
        SetGameState(PLAYING);
        ResumeMusicStream(music);   
    }

    if(IsKeyPressed(KEY_ESCAPE)){
        SetGameState(PLAYING);
        ResumeMusicStream(music);
    }
}

void Ended(){
    DrawRectangle(0, 0, Game::screenWidth, Game::screenHeight, Fade(PURPLE, 0.2f));
	DrawText("Game Over", Game::screenWidth / 2 - MeasureText("Game Over", 40) / 2, 50, 40, PURPLE);

    DrawText(score.c_str(), Game::screenWidth / 2 - MeasureText(score.c_str(), 40) / 2, Game::screenHeight / 2 + 60, 25, PURPLE);
	DrawText("Press Enter to restart or C for controlls", Game::screenWidth / 2 - MeasureText("Press Enter to restart or C for controlls", 20) / 2, Game::screenHeight * 0.75f - 10, 20, PURPLE);
    
    if(IsKeyPressed(KEY_ENTER)){
        Player::shipLives = 3;
        SetGameState(PLAYING);
    }

    if(IsKeyPressed(KEY_C)){
        SetGameState(CONTROLLS);
    }

    for (int i = 0; i < MAX_ASTROIDS; i++)
    {
        UnsetAstroid(i);
    }

    for (int i = 0; i < MAX_LASERBEAMS; i++)
    {
        UnsetLaserBeam(i);
    }

    if(IsKeyPressed(KEY_M)){
        ToggleMusic();
    }

    currentScore = 0;
}

void ShowControlls(){
    DrawRectangle(0, 0, Game::screenWidth, Game::screenHeight, Fade(PURPLE, 0.2f));
	DrawText("Controlls", Game::screenWidth / 2 - MeasureText("Controlls", 40) / 2, 50, 40, PURPLE);
    
    DrawText("Arrow keys or A D to move", Game::screenWidth / 2 - MeasureText("Arrow keys or A D to move", 20) / 2, Game::screenHeight / 2, 20, PURPLE);
    DrawText("Space to shoot", Game::screenWidth / 2 - MeasureText("Space to shoot", 20) / 2, Game::screenHeight / 2 + 30, 20, PURPLE);
    DrawText("C to switch weapon", Game::screenWidth / 2 - MeasureText("C to switch weapon", 20) / 2, Game::screenHeight / 2 + 60, 20, PURPLE);
    DrawText("M to mute music", Game::screenWidth / 2 - MeasureText("M to mute music", 20) / 2, Game::screenHeight / 2 + 90, 20, PURPLE);
    
    DrawText("Press Escape to go back", Game::screenWidth / 2 - MeasureText("Press Escape to go back", 20) / 2, Game::screenHeight * 0.75f - 10, 20, PURPLE);

    if(IsKeyPressed(KEY_ESCAPE)){
        SetGameState(START);
    }

    if(IsKeyPressed(KEY_M)){
        ToggleMusic();
    }
}

void UpdateAstroid(){

    astroidTimer -= GetFrameTime();
    if(astroidTimer < 0){
        astroidTimer = Meteor::meteorCooldown;
        SpawnAstroid();
    } 

    for (int i = 0; i < MAX_ASTROIDS; i++)
    {
        if(!astroids[i].isActive){
            continue;
        }

        if(astroids[i].position.y > Game::screenHeight){
            UnsetAstroid(i);
            continue;
        }

        astroids[i].position.y += Meteor::meteorSpeed;
        astroids[i].rotation += Meteor::meteorRotation + Meteor::meteorRotationSpeed;

        Rectangle playerRect = {
            Player::shipX,
            Game::screenHeight - 100,
            ship.width / 2,
            ship.height/ 2
        };

        //fix hitbox
        if(CheckCollisionPointRec(astroids[i].position, playerRect)){
            PlaySound(hitSound);
            Player::shipLives = Player::shipLives - 1;

            DrawTexture(explotion, Player::shipX + 10, Game::screenHeight - 100, WHITE);

            UnsetAstroid(i);
        }
    } 
}

void SpawnAstroid(){
    int index = -1;

    for (int i = 0; i < MAX_ASTROIDS; i++){

         if(!astroids[i].isActive){
            index = i;
            break;
        }
    }

    if(index == -1){
        return;
    }

    int speed = GetRandomValue(3, 8);
    int posX = GetRandomValue(-10, 710);

    SetAstroid(index, (Vector2){posX, meteor.height}, speed);
}

void DrawAstroids(){
    for (int i = 0; i < MAX_ASTROIDS; i++)
    {
        if(!astroids[i].isActive){
            continue;
        }

        Vector2 position = astroids[i].position;
        position.x = meteor.width / 2;
        position.y = meteor.height / 2;
        DrawTextureEx(meteor, astroids[i].position, astroids[i].rotation, 1, WHITE);
    } 
}

void UnsetAstroid(int astroid){

    if(astroid < 0 || astroid >= MAX_ASTROIDS){
        return;
    }

    astroids[astroid].isActive = false;
}

void SetAstroid(int astroid, Vector2 position, float speed){

    if(astroid < 0 || astroid >= MAX_ASTROIDS){
        return;
    }

    astroids[astroid].meteorSpeed = speed;
    astroids[astroid].position = position;
    astroids[astroid].isActive = true;
}

void UpdateLaserBeam(void){
    for (int i = 0; i < MAX_LASERBEAMS; i++)
    {
        if(!laserBeams[i].isActive){
            continue;
        }

        if(laserBeams[i].position.y < 0){
            UnsetLaserBeam(i);
            continue;
        }

        laserBeams[i].position.y -= Laser::laserSpeed;
        //laserBeams[i].position.x = Player::shipX;
        laserBeams[i].rotation = 180;

        for (int j = 0; j < MAX_ASTROIDS; j++)
        {
            if(!astroids[j].isActive){
                continue;
            } 

            Rectangle astroidRect = {
                astroids[j].position.x,
                astroids[j].position.y,
                meteor.width,
                meteor.height
            };

            //Collisions
            if(CheckCollisionPointRec(laserBeams[i].position, astroidRect)){
                PlaySound(hitSound);
                UnsetAstroid(j);
                UnsetLaserBeam(i);

                DrawTexture(explotion, laserBeams[i].position.x - 50, laserBeams[i].position.y - 50, WHITE);

                //setscore
                currentScore += 1;
            }  
        }   
    } 
}

void SpawnLaserBeam(){
    int index = -1;

    for (int i = 0; i < MAX_LASERBEAMS; i++){

         if(!laserBeams[i].isActive){
            index = i;
            break;
        }
    }

    if(index == -1){
        return;
    }

    SetLaserBeam(index, (Vector2){Player::shipX + 75, Game::screenHeight - 70}, Laser::laserSpeed);
}

void DrawLaserBeam(){
    for (int i = 0; i < MAX_LASERBEAMS; i++)
    {
        if(!laserBeams[i].isActive){
            continue;
        }

        if(Player::currentWeapon == 0) {

            Vector2 position = laserBeams[i].position;
            position.x = laser.width / 2;
            position.y = laser.height / 2;
            DrawTextureEx(laser, laserBeams[i].position, laserBeams[i].rotation, 1, WHITE);

        } else if (Player::currentWeapon == 1) {

            Vector2 position = laserBeams[i].position;
            position.x = cannon.width / 2;
            position.y = cannon.height / 2;
            DrawTextureEx(cannon, laserBeams[i].position, 90, 1, WHITE);
        }
    } 
}

void UnsetLaserBeam(int laserBeam){

    if(laserBeam < 0 || laserBeam >= MAX_LASERBEAMS){
        return;
    }

    laserBeams[laserBeam].isActive = false;
}

void SetLaserBeam(int laserBeam, Vector2 position, float speed){

    if(laserBeam < 0 || laserBeam >= MAX_LASERBEAMS){
        return;
    }

    laserBeams[laserBeam].speed = speed;
    laserBeams[laserBeam].position = position;
    laserBeams[laserBeam].isActive = true;
}

void MoveLeft(){
    if(Player::shipX != -10) {
        Player::shipX =+ Player::shipX - Player::shipSpeed;
    }
}

void MoveRight(){
    if(Player::shipX < Game::screenWidth - 100){
        Player::shipX =+ Player::shipX + Player::shipSpeed;
    }
}

void ToggleMusic(){
    if(IsMusicStreamPlaying(music)){
        PauseMusicStream(music);
    } else if (IsMusicStreamPlaying(music) == false){
        ResumeMusicStream(music);
    }
}
