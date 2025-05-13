/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

For a C++ project simply rename the file to .cpp and re-run the build script 

-- Copyright (c) 2020-2024 Jeffery Myers
--
--This software is provided "as-is", without any express or implied warranty. In no event 
--will the authors be held liable for any damages arising from the use of this software.

--Permission is granted to anyone to use this software for any purpose, including commercial 
--applications, and to alter it and redistribute it freely, subject to the following restrictions:

--  1. The origin of this software must not be misrepresented; you must not claim that you 
--  wrote the original software. If you use this software in a product, an acknowledgment 
--  in the product documentation would be appreciated but is not required.
--
--  2. Altered source versions must be plainly marked as such, and must not be misrepresented
--  as being the original software.
--
--  3. This notice may not be removed or altered from any source distribution.

*/

#include "raylib.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir
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
	
	while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
	{
		// drawing
		BeginDrawing();

		// Setup the backbuffer for drawing (clear color and depth buffers)
		ClearBackground(Color { 20, 20, 20, 00 } );

		// draw some text using the default font
		// DrawText("Hello Raylib", 200,200,20,WHITE);
		// draw our texture to the screen
		//DrawTexture(bg, 0, 0, WHITE);
        DrawTextureEx(bg, Vector2 { 0, 0 }, 0.0f, SCALE, WHITE );
        //DrawPixel(i, i , WHITE);
	    
        for(int u = 0; u < 26/*core::grid.sectors*/; ++u)
        {
            DrawRectangleLines
            (
                core::grid.sector[u].offset.x * SCALE + core::grid.bounds.x * SCALE,
                core::grid.sector[u].offset.y * SCALE + core::grid.bounds.y * SCALE,
                core::grid.sector[u].descriptor->constrain->w * SCALE,
                core::grid.sector[u].descriptor->constrain->h * SCALE,
                Color { 0xFF, 0xFF, 0xFF, 0xDD }
            );
                for(int i = 0; i < *core::grid.sector[u].descriptor->cv[core::map::cv::c]; ++i)
                {
                    DrawRectangle     
                    (
                        core::grid.sector[u].constrain_c[i].x * SCALE,
                        core::grid.sector[u].constrain_c[i].y * SCALE,
                        core::grid.sector[u].constrain_c[i].w * SCALE,
                        core::grid.sector[u].constrain_c[i].h * SCALE,
                        Color { 0x80, 0xFF, 0x80, 0x40 }
                    );
                }
                for(int i = 0; i < *core::grid.sector[u].descriptor->cv[core::map::cv::i]; ++i)
                {
                    DrawRectangle     
                    (
                        core::grid.sector[u].constrain_i[i].x * SCALE,
                        core::grid.sector[u].constrain_i[i].y * SCALE,
                        core::grid.sector[u].constrain_i[i].w * SCALE,
                        core::grid.sector[u].constrain_i[i].h * SCALE,
                        Color { 0xFF, 0x80, 0x80, 0x40 }
                    );
                }
                for(int i = 0; i < *core::grid.sector[u].descriptor->cv[core::map::cv::o]; ++i)
                {
                    DrawRectangle     
                    (
                        core::grid.sector[u].constrain_o[i].x * SCALE,
                        core::grid.sector[u].constrain_o[i].y * SCALE,
                        core::grid.sector[u].constrain_o[i].w * SCALE,
                        core::grid.sector[u].constrain_o[i].h * SCALE,
                        Color { 0x80, 0x80, 0xFF, 0x40 }
                    );
                }

        }
		EndDrawing();
	}
	UnloadTexture(bg);
	CloseWindow();
	return 0;
}
