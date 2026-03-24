// Game/game.c
#include "game.h"
#include "../Engine/core.h"
#include "../Engine/utils.h"
#include "../Engine/colors.h"
#include "../Engine/render.h"
#include "../Engine/input.h"
#include <stdio.h>

    static int oyuncu_x = 10, oyuncu_y = 10;
    static int canavar_x = 30, canavar_y = 10;
void Oyun_Baslat(void) {
    // Oyun başladığında yapılacak ilk ayarlar (varsa)
    // Örneğin, haritayı yükleyebilir veya oyuncunun başlangıç konumunu ayarlayabiliriz.
}

 void Oyun_Guncelle(char tus) {

    if (tus == 'q') {
        Engine_Stop();
    }
    // 1. OYUNCUYU HAREKET ETTİR (Duvarların içinden geçebilir!)
    if (tus == 'w' && oyuncu_y > 0) oyuncu_y--;
    if (tus == 's' && oyuncu_y < SCR_HEIGHT - 1) oyuncu_y++;
    if (tus == 'a' && oyuncu_x > 0) oyuncu_x--;
    if (tus == 'd' && oyuncu_x < SCR_WIDTH - 1) oyuncu_x++;
}
void Oyun_Ekrana_Ciz(){
    // not: Render_DrawPixel ve Render_DrawText fonksiyonları, ekran sınırlarını aşmamak için zaten güvenli hale getirildi.
    // sen bizi bulamazsın, biz seni buluruz! (buffer overflow yok artık)
    Render_DrawPixel(oyuncu_x, oyuncu_y, '@', SARI);
    Render_DrawPixel(canavar_x, canavar_y, 'M', KIRMIZI);


    Render_DrawText(2, 1, "voidEngine v0.3 - Mimar: Ilhan", CYAN);
    Render_DrawText(65, 1, "[FPS: 60]", YESIL);
    Render_DrawText(oyuncu_x + 1, oyuncu_y - 1, "Burasi neresi?", BEYAZ);
}
