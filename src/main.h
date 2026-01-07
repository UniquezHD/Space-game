typedef enum GameState {
    PLAYING,
	END,
    PAUSE,
    START,
    CONTROLLS,
} GameState;

GameState GetGameState(void);
void SetGameState(GameState);

GameState state;

GameState GetGameState(void)
{
	return state;
}

void SetGameState(GameState State)
{
	state = State;
}

std::string Weapons[2] = {
    "Laser",
    "Cannon"
};

Texture2D lives;
Texture2D laser;
Texture2D meteor;
Texture2D ship;
Texture2D spaceBg;
Texture2D explotion;
Texture2D cannon;

Sound laserSound;
Sound cannonSound;
Sound hitSound;
Sound switchSound;

Music music;

Color Green = Color{20, 160, 133, 255};

void Init(void);

void InitGame(void);
void Playing(void);
void Paused(void);
void Ended(void);

void ShowControlls(void);

void ToggleMusic(void);

void UpdateDraw(void);

void MoveLeft(void);
void MoveRight(void);

void DrawAstroids(void);
void UpdateAstroid(void);
void SpawnAstroid(void);
void UnsetAstroid(int);
void SetAstroid(int, Vector2, float);

void UnsetLaserBeam(int);
void SetLaserBeam(int, Vector2, float);
void DrawLaserBeam(void);
void UpdateLaserBeam(void);
void SpawnLaserBeam(void);