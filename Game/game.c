// Game/game.c
#include "game.h"
#include "../Engine/core.h"
#include "../Engine/utils.h"
#include "../Engine/colors.h"
#include "../Engine/render.h"
#include "../Engine/input.h"
#include"../Engine/UI.h"
#include "../Engine/Physics.h"
#include "../Engine/Map.h"
#include <stdio.h>


// --- DURUM MAKİNESİ (STATE MACHINE) ---
typedef enum { EKRAN_MENU, EKRAN_OYUN } OyunDurumu;
static OyunDurumu aktif_ekran = EKRAN_MENU; // Oyun ilk açıldığında menü ile başlar!

static float oyuncu_x = 3, oyuncu_y = 3;
static int canavar_x = 10, canavar_y = 5;
float hız=50.0f;
bool etkilendi = false;

void Oyun_Baslat(void) {
    setTargetFPS(120);
    Typewriter_MsGuncelle(50); 
    UI_LoadMenu("Resources/menu1.txt"); 
    Map_Load("Resources/harita4.txt"); 
}

void Oyun_Guncelle(int tus, float deltaTime) {
    
    if(tus == 'q') Engine_Stop();

        
       switch (aktif_ekran)
       {
       case  EKRAN_MENU:
        {
            ButtonAction action = UI_Update(tus);
            if (action == ACTION_START_GAME) {
                aktif_ekran = EKRAN_OYUN; // Oyunu başlat
            } else if (action == ACTION_QUIT) {
                Engine_Stop(); // Motoru durdur
            }
        }
        break;
       case EKRAN_OYUN:
        {
        float hedef_x = oyuncu_x;
        float hedef_y = oyuncu_y;

        if(tus == 'f') hız+=10.0f;
        if(tus == 'g') hız-=10.0f;
        if(hız < 10.0f) hız = 10.0f; // Minimum hız sınırı
        // 1. ADIM: OYUNCU NEREYE GİTMEK İSTİYOR? (Sadece hedefi belirliyoruz, henüz gitmiyoruz)
        if (tus == 'w' || tus == KEY_UP)    hedef_y -= hız * deltaTime;
        if (tus == 's' || tus == KEY_DOWN)  hedef_y += hız * deltaTime;
        if (tus == 'a' || tus == KEY_LEFT)  hedef_x -= hız * deltaTime;
        if (tus == 'd' || tus == KEY_RIGHT) hedef_x += hız * deltaTime;

        // 2. ADIM: FİZİK KUTULARINI OLUŞTUR
        // Terminalde 1 harf, 1 birimlik yer kapladığı için genişlik ve yükseklik 1.0f
        // Hedef pozisyonu geçici olarak oyuncunun yeni pozisyonuymuş gibi düşünelim
        AABB oyuncuKutusu = {(float)((int)hedef_x), (float)((int)hedef_y), 1.0f, 1.0f}; 
        AABB canavarKutusu = {(float)canavar_x, (float)canavar_y, 1.0f, 1.0f};

        // 3. ADIM: ÇARPIŞMA KONTROLÜ (Geleceği simüle et)
        bool carpistiMi = Physics_CheckCollision(oyuncuKutusu, canavarKutusu);
        bool duvaraCarpti = Map_CheckCollision(oyuncuKutusu);

         // Ayrıca Ekran Sınırlarını da kontrol edelim (Dışarı çıkmasın)
        bool ekranDisiMi = (hedef_x < 0 || hedef_x >= SCR_WIDTH || hedef_y < 0 || hedef_y >= SCR_HEIGHT);

        // 4. ADIM: EĞER GÜVENLİYSE HAREKETİ ONAYLA
        if (!carpistiMi && !ekranDisiMi && !duvaraCarpti) {
        oyuncu_x = hedef_x;
        oyuncu_y = hedef_y;
        } else if (carpistiMi) {
        // Canavara çarptığında ekranda bir mesaj verdirebiliriz!
        etkilendi = true; 
        } else {
        etkilendi = false;
        }
        }
        break;
       
       default:
        break;
       }

}

void Oyun_Ekrana_Ciz(float deltaTime) {
    // Motor tuvali zaten temizledi. Şimdi ne çizeceğimize karar verelim:
    if (aktif_ekran == EKRAN_MENU) {
        // Sadece butonları çizmekle kalma, menüdeki tüm elemanları çiz (LABEL'lar da dahil)
        UI_Render(); 
    } 
    else if (aktif_ekran == EKRAN_OYUN) {
        Map_Render(MAVI); // Haritayı çiz (duvarlar, boşluklar vs.)
        int oyuncuCizim_x = (int)oyuncu_x;
        int oyuncuCizim_y = (int)oyuncu_y;

        Render_DrawPixel(oyuncuCizim_x, oyuncuCizim_y, '@', SARI);
        DrawFPS(1, 22, deltaTime); // FPS'i sağ üst köşeye çiz
        char hızMetni[20];
        sprintf(hızMetni, " %.1f", hız);
        Render_DrawText(69,22,"Hiz:", SARI);
        Render_DrawText(72, 22, hızMetni, CYAN);



        Render_DrawPixel(canavar_x, canavar_y, 'M', KIRMIZI);
        //Render_DrawText(65, 3, "voidEngine v0.3", CYAN);

        if (etkilendi) {
            Render_DrawText(13, 22, "CANAVARA CARPTIN!", KIRMIZI);
        }
        else {
            Render_DrawText(13, 22, "                   ", BEYAZ); // Mesajı temizle
        }
    }
}
