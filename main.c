#include<stdio.h>
#include <stdlib.h>
#include <time.h>
#include "raylib.h"

#include "particle.h"

#if defined(PLATFORM_DESKTOP)
#define GLSL_VERSION            330
#else   // PLATFORM_RPI, PLATFORM_ANDROID, PLATFORM_WEB
#define GLSL_VERSION            100
#endif

#define N_PARTICLES 10000

#define RAYGUI_IMPLEMENTATION
#define RAYGUI_SUPPORT_ICONS
#include "extras/raygui.h"

typedef struct particle_options {
    float sensor_angle;
    float sensor_distance;
    float speed;
}particle_options_t;

int main()
{
    srand(time(0));

//    SetTraceLogLevel(LOG_FATAL);
    particle_t particle = {
            .position = {.x=100,.y=100},
            .heading = { .x=1,.y=0},
            .sensor_angle = 30.0,
            .sensor_distance = 50.0,
            .speed = 10,
            .deposit_amount = 0.3
    };

    particle_t particle2 = {
            .position = {.x=200,.y=200},
            .heading = { .x=0,.y=1},
            .sensor_angle = 30.0,
            .sensor_distance = 50.0,
            .speed = 10,
            .deposit_amount = 0.8
    };
    particle_options_t particle_options = {
            .sensor_angle = 20,
            .sensor_distance = 10,
            .speed = 5
    };
    particle_t* particles = (particle_t*)malloc(N_PARTICLES*sizeof(particle_t));

    // Initialization
    //--------------------------------------------------------------------------------------
    int screenWidth = 1920;
    int screenHeight = 1080;

    for(int i = 0; i <N_PARTICLES; i++) {
        init_particle(&particles[i],screenWidth,screenHeight);
    }

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
    Shader blur_shader = LoadShader(0, TextFormat("blur.fs", GLSL_VERSION));
    Shader color_shader = LoadShader(0, TextFormat("coloring.fs", GLSL_VERSION));

    RenderTexture2D trails = LoadRenderTexture(screenWidth, screenHeight);

    Image img_data = LoadImageFromTexture(trails.texture);
    int strideSrc = GetPixelDataSize(img_data.width, 1, img_data.format);
    int bytesPerPixelSrc = strideSrc/(img_data.width);
    SetTargetFPS(30);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        //
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
//        BeginTextureMode(trails);       // Enable drawing to texture
//        ClearBackground(BLACK);     // Clear the render texture
//
//        // Draw a rectangle in shader mode to be used as shader canvas
//        // NOTE: Rectangle uses font white character texture coordinates,
//        // so shader can not be applied here directly because input vertexTexCoord
//        // do not represent full screen coordinates (space where want to apply shader)
//        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLACK);
//        EndTextureMode();
        BeginTextureMode(trails);
//        ClearBackground(BLACK);
        BeginShaderMode(blur_shader);
            // WARNING: If FLAG_WINDOW_HIGHDPI is enabled, HighDPI monitor scaling should be considered
            // when rendering the RenderTexture2D to fit in the HighDPI scaled Window
            DrawTextureRec(trails.texture, (Rectangle){ 0, 0, (float)trails.texture.width, (float)-trails.texture.height }, (Vector2){ 0, 0 }, WHITE);
            EndShaderMode();
        EndTextureMode();
        img_data = LoadImageFromTexture(trails.texture);



//        Color c = GetPixelColor(img_data.data + (100*strideSrc)+100*bytesPerPixelSrc ,img_data.format);
//        SetPixelColor(img_data.data + (100*strideSrc)+100*bytesPerPixelSrc,RED,img_data.format);
//        if(c.r!=0 || c.g !=0 ||c.b !=0)
//        printf("c  [%d]-> (%d,%d,%d)",(100*strideSrc)+100*bytesPerPixelSrc,c.r,c.g,c.b);
//


        for(int i = 0; i <N_PARTICLES; i++) {
            particle_update(&particles[i], &img_data,strideSrc,bytesPerPixelSrc);
            particle_check_bounds(&particles[i],screenWidth-1,screenHeight-1);

//            particle_show(&particles[i]);
        }
        BeginTextureMode(trails);
        for(int i = 0; i <N_PARTICLES; i++) {
            particle_deposit(&particles[i],&trails);
        }
    EndTextureMode();

        BeginDrawing();


        ClearBackground(BLACK);
//        particle_sense(&particle);
//        particle_update(&particle, &img_data,strideSrc,bytesPerPixelSrc);
//        particle_check_bounds(&particle,screenWidth-1,screenHeight-1);
//        particle_deposit(&particle,&trails);
//        particle_show(&particle);
//
//        particle_update(&particle2, &img_data,strideSrc,bytesPerPixelSrc);
//        particle_check_bounds(&particle2,screenWidth-1,screenHeight-1);
//        particle_deposit(&particle2,&trails);
//        particle_show(&particle2);



        BeginShaderMode(color_shader);
        DrawTextureRec(trails.texture, (Rectangle) { 0, 0, (float)trails.texture.width, (float)-trails.texture.height }, (Vector2) { 0, 0 }, WHITE);
        EndShaderMode();
        DrawFPS(10,30);

        GuiGroupBox((Rectangle){ 25, 310, 125, 150 }, "STATES");
//        GuiValueBox((Rectangle){ 1700, 175, 125, 30 }, "Sensor Angle", &particle_options.sensor_angle, 0, 359, true);
        particle_options.sensor_angle = GuiSlider((Rectangle){ 1600, 400, 200, 20 }, "Sensor Angle", TextFormat("%2.2f", (float)particle_options.sensor_angle), particle_options.sensor_angle, 0, 359);
        particle_options.sensor_distance = GuiSlider((Rectangle){ 1600, 440, 200, 20 }, "Sensor Distance", TextFormat("%2.2f", (float)particle_options.sensor_distance), particle_options.sensor_distance, 0, 30);
        particle_options.speed = GuiSlider((Rectangle){ 1600, 500, 200, 20 }, "Speed", TextFormat("%2.2f", (float)particle_options.speed), particle_options.speed, 0, 30);

        EndDrawing();

        UnloadImage(img_data);

        if (particles[0].sensor_angle != particle_options.sensor_angle) {
            printf("UPDATE sensor_angle\n");
            // valore modificato, aggiornare tutte le particelle
            for (int i = 0; i < N_PARTICLES; ++i) {
                particles[i].sensor_angle = particle_options.sensor_angle;
            }
        }

        if (particles[0].sensor_distance != particle_options.sensor_distance) {
            printf("UPDATE sensor_distance\n");
            // valore modificato, aggiornare tutte le particelle
            for (int i = 0; i < N_PARTICLES; ++i) {
                particles[i].sensor_distance = particle_options.sensor_distance;
            }
        }

        if (particles[0].speed != particle_options.speed) {
            printf("UPDATE speed\n");
            // valore modificato, aggiornare tutte le particelle
            for (int i = 0; i < N_PARTICLES; ++i) {
                particles[i].speed = particle_options.speed;
            }
        }
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    free(particles);
    //--------------------------------------------------------------------------------------

    return 0;
}