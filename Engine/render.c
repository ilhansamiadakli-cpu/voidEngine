#include "render.h"
#include"colors.h"
#include <stdio.h>
#include <string.h>

static char screen_buffer[SCR_HEIGHT][SCR_WIDTH];
static const char* color_buffer[SCR_HEIGHT][SCR_WIDTH];

void Render_Init(void) {
    Render_ClearBuffer();
}

void Render_ClearBuffer(void) {
    for (int y = 0; y < SCR_HEIGHT; y++) {
        for (int x = 0; x < SCR_WIDTH; x++) {
            screen_buffer[y][x] = ' ';
            color_buffer[y][x] = BEYAZ;// temizleme işlemi için beyaz(reset color)
        }
    }
}

void Render_DrawPixel(int x, int y, char c, const char* color) {
    //ekran sınırlarını kontrol
    //Not:ekran sınırlarını kontrol etmezsek oyun donar veya çökebilir 
    if (x >= 0 && x < SCR_WIDTH && y >= 0 && y < SCR_HEIGHT) {
        screen_buffer[y][x] = c;
        color_buffer[y][x] = color;
    }
    else{
        //ekran sınırları dışına çizim yapma
        //Not:ekran sınırları dışına çizim yaparsak oyun donar veya çökebilir 
    }
}

void Render_Present(void) {
    printf("\033[H"); // İmleci başa al
    for (int y = 0; y < SCR_HEIGHT; y++) {
        for (int x = 0; x < SCR_WIDTH; x++) {
            // Renk ve karakteri beraber bas
            fputs(color_buffer[y][x], stdout);
            putchar(screen_buffer[y][x]);
        }
        putchar('\n');
    }
    fflush(stdout);
}