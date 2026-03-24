#ifndef RENDER_H
#define RENDER_H

#define SCR_WIDTH 80// türkçesi: ekran genişliği
#define SCR_HEIGHT 24// türkçesi: ekran yüksekliği 

void Render_BeginDrawing(void);
void Render_ClearBuffer(void);
void Render_DrawPixel(int x, int y, char c, const char* color);
void Render_DrawText(int x, int y, const char* text, const char* color);
void Render_EndDrawing(void); 
void DrawFPS(int fps, int fps_x, int fps_y); // FPS'i ekrana çizmek için yeni fonksiyon

#endif