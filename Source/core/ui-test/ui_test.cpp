#include "raylib.h"
#include "resource_dir.h"	
#include "canvas.hpp"
#include "grid.hpp"
#include "constants.hpp"

int main ()
{
    const int WIDTH = 1060, HEIGHT = 596;
    const float SCALE = 1.5f;
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	InitWindow(WIDTH * SCALE, HEIGHT * SCALE, "Spiro_HUD_TEST");
	SearchAndSetResourceDir("../../Source/assets");
	Texture bg = LoadTexture("bg.png");

    Color col[] { { 0x80, 0xFF, 0x80, 0x40 }, { 0xFF, 0x80, 0x80, 0x40 }, { 0x80, 0x80, 0xFF, 0x40 }};
	
	while (!WindowShouldClose())
	{
		BeginDrawing();
		ClearBackground(Color { 20, 20, 20, 00 } );
        DrawTextureEx(bg, Vector2 { 0, 0 }, 0.0f, SCALE, WHITE );
	    
        for(int u = 0; u < core::grid.sectors; ++u)
        {
            DrawRectangleLines
            (
                core::grid.sector[u].offset.x * SCALE + core::grid.bounds.x * SCALE,
                core::grid.sector[u].offset.y * SCALE + core::grid.bounds.y * SCALE,
                core::grid.sector[u].descriptor->constrain->w * SCALE,
                core::grid.sector[u].descriptor->constrain->h * SCALE,
                Color { 0xFF, 0xFF, 0xFF, 0xDD }
            );
            for(int j = 0; j < core::map::cv::count; ++j)
            {
                for(int i = 0; i < *core::grid.sector[u].descriptor->cv[j]; ++i)
                {
                    DrawRectangle     
                    (
                        core::grid.sector[u].bounds[j][i].x * SCALE + core::grid.bounds.x * SCALE,
                        core::grid.sector[u].bounds[j][i].y * SCALE + core::grid.bounds.y * SCALE,
                        core::grid.sector[u].bounds[j][i].w * SCALE,
                        core::grid.sector[u].bounds[j][i].h * SCALE,
                        col[j]
                    );
                }
            }
        }
		EndDrawing();
	}
	UnloadTexture(bg);
	CloseWindow();
	return 0;
}
