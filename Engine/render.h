#ifndef RENDER_H
#define RENDER_H

#define SCR_WIDTH 80// türkçesi: ekran genişliği
#define SCR_HEIGHT 24// türkçesi: ekran yüksekliği 

void Render_Init(void);
void Render_ClearBuffer(void);
void Render_DrawPixel(int x, int y, char c, const char* color);
void Render_Present(void); 

#endif