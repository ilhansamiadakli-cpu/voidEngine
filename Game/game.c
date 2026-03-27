// Game/game.c
#include "game.h"
#include "../Engine/core.h"
#include "../Engine/utils.h"
#include "../Engine/colors.h"
#include "../Engine/render.h"
#include "../Engine/input.h"
#include"../Engine/UI.h"
#include <stdio.h>


// --- DURUM MAKİNESİ (STATE MACHINE) ---
typedef enum { EKRAN_MENU, EKRAN_OYUN } OyunDurumu;
static OyunDurumu aktif_ekran = EKRAN_MENU; // Oyun ilk açıldığında menü ile başlar!

static int oyuncu_x = 10, oyuncu_y = 10;
static int canavar_x = 30, canavar_y = 10;

void Oyun_Baslat(void) {
    // Yorumlayıcı çalışır ve txt dosyasını okuyup RAM'e kaydeder.
    Typewriter_MsGuncelle(50); // Daktilo hızını güncelle (örneğin, 300 ms yap)
    UI_LoadMenu("Game/menu.txt"); 
}

void Oyun_Guncelle(char tus) {
    

    if (tus == 'q') Engine_Stop();

    // 1. DURUM: EĞER MENÜDEYSEK
    if (aktif_ekran == EKRAN_MENU) {
        // Tuşları oyun karakterine değil, UI yorumlayıcısına gönder
        ButtonAction secilen_gorev = UI_Update(tus); 
        
        if (secilen_gorev == ACTION_START_GAME) {
            aktif_ekran = EKRAN_OYUN; // Vites at! Oyuna geç!
        } 
        else if (secilen_gorev == ACTION_QUIT) {
            Engine_Stop(); // Çıkış görevini seçtiyse motoru durdur
        }
    }
    // 2. DURUM: EĞER OYUNDAYSAK
    else if (aktif_ekran == EKRAN_OYUN) {
        if (tus == 'w' && oyuncu_y > 0) oyuncu_y--;
        if (tus == 's' && oyuncu_y < SCR_HEIGHT - 1) oyuncu_y++;
        if (tus == 'a' && oyuncu_x > 0) oyuncu_x--;
        if (tus == 'd' && oyuncu_x < SCR_WIDTH - 1) oyuncu_x++;
    }
}

void Oyun_Ekrana_Ciz(void) {
    // Motor tuvali zaten temizledi. Şimdi ne çizeceğimize karar verelim:
    setTargetFPS(60);
    if (aktif_ekran == EKRAN_MENU) {
        // Sadece butonları çizmekle kalma, menüdeki tüm elemanları çiz (LABEL'lar da dahil)
        UI_Render(); 
    } 
    else if (aktif_ekran == EKRAN_OYUN) {
        // Menüyü unut, haritayı ve oyuncuyu çiz
        Render_DrawPixel(oyuncu_x, oyuncu_y, '@', SARI);
        Render_DrawText(2, 0, "Oyunda! (WASD ile hareket, Q ile cikis)", SARI);
        Render_DrawText(oyuncu_x + 1, oyuncu_y - 1, "Burasi neresi?", BEYAZ); 
        Render_DrawPixel(canavar_x, canavar_y, 'M', KIRMIZI);
        Render_DrawText(2, 1, "voidEngine v0.3", CYAN);
    }
}
