/**
* Author: Janae Lewis
* Assignment: 2d Scene
* Date due: [10/05/2026]
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/


#include "raylib.h"
#include <math.h>

enum AppStatus { TERMINATED, RUNNING };

// Global Constants
constexpr int SCREEN_WIDTH = 1600 / 2;
constexpr int SCREEN_HEIGHT = 900 / 2;
constexpr int FPS = 60;


constexpr Vector2 UNICORN_SIZE = {100.0f, 100.0f};
constexpr Vector2 CLOUD_SIZE = {140.0f, 80.0f};
constexpr Vector2 RAINBOW_SIZE = {420.0f, 280.0f};
constexpr Vector2 GROUND_SIZE = {SCREEN_WIDTH + 100.0f, 150.0f};

constexpr Vector2 ORIGIN = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};

constexpr float GROUND_Y = SCREEN_HEIGHT - 60.0f;
constexpr float UNICORN_BASE_Y = 330.0f;
constexpr Vector2 RAINBOW_CENTER = {200.0f, 270.0f};

// le images
constexpr char UNICORN_FP[] = "assets/game/unicorn.png";
constexpr char RAINBOW_FP[] = "assets/game/rainbow.png";
constexpr char CLOUD_FP[] = "assets/game/cloud.png";
constexpr char GROUND_FP[] = "assets/game/ground.png";

//global Variables
AppStatus gAppStatus = RUNNING;
float gTime = 0.0f;
float gPreviousTicks = 0.0f;

Vector2 gUnicornPosition = {ORIGIN.x, UNICORN_BASE_Y};
Vector2 gUnicornScale = UNICORN_SIZE;
Vector2 gCloudPosition = {ORIGIN.x, 60.0f};
Vector2 gCloudScale = CLOUD_SIZE;
Vector2 gRainbowPosition = {ORIGIN.x, 130.0f};
Vector2 gRainbowScale = RAINBOW_SIZE;
Vector2 gGroundPosition = {ORIGIN.x, GROUND_Y};
Vector2 gGroundScale = GROUND_SIZE;

float gUnicornAngle = 0.0f;
float gCloudAngle = 0.0f;
float gRainbowAngle = 0.0f;
float gGroundAngle = 0.0f;

Color gBackgroundColour = {255, 200, 220, 255};

Texture2D gUnicornTexture;
Texture2D gRainbowTexture;
Texture2D gCloudTexture;
Texture2D gGroundTexture;

// Function Declarations
void initialise();
void processInput();
void update();
void render();
void shutdown();

// Function Definitions
void renderObject(
    const Texture2D *texture,
    const Vector2 *position,
    const Vector2 *scale,
    float angle)
{
    Rectangle textureArea = {0.0f, 0.0f, static_cast<float>(texture->width),static_cast<float>(texture->height)};

    Rectangle destinationArea = {
        position->x,
        position->y,
        static_cast<float>(scale->x),
        static_cast<float>(scale->y)};

    Vector2 originOffset = {
        static_cast<float>(scale->x) / 2.0f,
        static_cast<float>(scale->y) / 2.0f};

    DrawTexturePro(*texture, textureArea, destinationArea, originOffset, angle, WHITE);
}

void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Unicorn");

    gUnicornTexture = LoadTexture(UNICORN_FP);
    gRainbowTexture = LoadTexture(RAINBOW_FP);
    gCloudTexture = LoadTexture(CLOUD_FP);

    gGroundTexture = LoadTexture(GROUND_FP);

    SetTargetFPS(FPS);
}

void processInput()
{
    if (IsKeyPressed(KEY_Q) || WindowShouldClose()) gAppStatus = TERMINATED;
}

void update()
{
    float ticks = static_cast<float>(GetTime());
    float deltaTime = ticks - gPreviousTicks;
    gPreviousTicks = ticks;

    // every formula below runs off this one timer
    gTime += deltaTime;

    // BG, pastel colours that change over time
    gBackgroundColour = {
        static_cast<unsigned char>(200.0f + 50.0f * sinf(gTime * 0.5f)),
        190,
        240,
        255};

    // Unicorn that hops along the ground, twists while it jumps
    gUnicornPosition = {
        ORIGIN.x + 250.0f * sinf(gTime * 0.5f),
        UNICORN_BASE_Y - fabs(sinf(gTime * 3.0f)) * 80.0f};
    gUnicornAngle = 15.0f * cosf(gTime * 3.0f);

    // cloud
    gCloudPosition = {
        ORIGIN.x + 200.0f * cosf(gTime * 0.4f),
        60.0f + 20.0f * sinf(gTime * 0.4f)};

    // Unicorn, same hop as before, tilts while it jumps
    // and its size depends on where the cloud is moving from side to side
    float scaleFactor = 1.0f + (gCloudPosition.x - ORIGIN.x) / 400.0f;
    gUnicornScale = { UNICORN_SIZE.x * scaleFactor, UNICORN_SIZE.y * scaleFactor};

    gUnicornPosition = {
        ORIGIN.x + 250.0f * sinf(gTime * 0.5f),
        UNICORN_BASE_Y - fabs(sinf(gTime * 3.0f)) * 80.0f - (gUnicornScale.y - UNICORN_SIZE.y) / 2.0f};
    gUnicornAngle = 15.0f * cosf(gTime * 3.0f);

    // rainbow stays in the bottom left and moves in a small circle motion
    gRainbowPosition = {RAINBOW_CENTER.x + 15.0f * cosf(gTime * 1.5f), RAINBOW_CENTER.y + 15.0f * sinf(gTime * 1.5f)};
    gRainbowAngle = 5.0f * sinf(gTime);

    // Ground has a small circular wobble
    gGroundPosition = {ORIGIN.x + 8.0f * cosf(gTime), GROUND_Y + 8.0f * sinf(gTime)};
}

void render()
{
    BeginDrawing();
    ClearBackground(gBackgroundColour);

    renderObject(&gCloudTexture, &gCloudPosition, &gCloudScale, gCloudAngle);
    renderObject(&gRainbowTexture, &gRainbowPosition, &gRainbowScale, gRainbowAngle);
    renderObject(&gGroundTexture, &gGroundPosition, &gGroundScale, gGroundAngle);
    renderObject(&gUnicornTexture, &gUnicornPosition, &gUnicornScale, gUnicornAngle);

    EndDrawing();
}

void shutdown()
{
    UnloadTexture(gUnicornTexture);
    UnloadTexture(gRainbowTexture);
    UnloadTexture(gCloudTexture);
    UnloadTexture(gGroundTexture);
    CloseWindow();
}

int main(void)
{
    initialise();

    while (gAppStatus == RUNNING)
    {
        processInput();
        update();
        render();
    }

    shutdown();

    return 0;
}
