// Game/game.c
#include "game.h"
#include "../Engine/core.h"
#include "../Engine/utils.h"
#include "../Engine/colors.h"
#include "../Engine/render.h"
#include "../Engine/input.h"
#include"../Engine/UI.h"
#include "../Engine/Physics.h"
#include <stdio.h>


// --- DURUM MAKİNESİ (STATE MACHINE) ---
typedef enum { EKRAN_MENU, EKRAN_OYUN } OyunDurumu;
static OyunDurumu aktif_ekran = EKRAN_MENU; // Oyun ilk açıldığında menü ile başlar!

static float oyuncu_x = 10, oyuncu_y = 10;
static int canavar_x = 30, canavar_y = 10;
float hız=50.0f;
bool etkilendi = false;

void Oyun_Baslat(void) {
    // Yorumlayıcı çalışır ve txt dosyasını okuyup RAM'e kaydeder.
    Typewriter_MsGuncelle(50); // Daktilo hızını güncelle (örneğin, 300 ms yap)
    UI_LoadMenu("Game/menu.txt"); 
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

         // Ayrıca Ekran Sınırlarını da kontrol edelim (Dışarı çıkmasın)
        bool ekranDisiMi = (hedef_x < 0 || hedef_x >= SCR_WIDTH || 
                    hedef_y < 0 || hedef_y >= SCR_HEIGHT);

        // 4. ADIM: EĞER GÜVENLİYSE HAREKETİ ONAYLA
        if (!carpistiMi && !ekranDisiMi) {
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
    setTargetFPS(60); // FPS'i 60'a sabitle (opsiyonel, ama genellikle iyi bir fikir)
    if (aktif_ekran == EKRAN_MENU) {
        // Sadece butonları çizmekle kalma, menüdeki tüm elemanları çiz (LABEL'lar da dahil)
        UI_Render(); 
    } 
    else if (aktif_ekran == EKRAN_OYUN) {

        int oyuncuCizim_x = (int)oyuncu_x;
        int oyuncuCizim_y = (int)oyuncu_y;

        Render_DrawPixel(oyuncuCizim_x, oyuncuCizim_y, '@', SARI);
        DrawFPS(69, 1, deltaTime); // FPS'i sağ üst köşeye çiz
        Render_DrawText(2, 0, "WASD ile hareket, Q ile cikis, F/G ile hiz artir/azalt, E ile etkilesim", SARI);

        Render_DrawText(3, 2, "Hiz: ", MAVI);// hız göstergesi basılamıyor malesef, çünkü Render_DrawText fonksiyonu şu anda sadece sabit metinler için tasarlandı. Hızı dinamik olarak göstermek için önce hızı bir string'e çevirmemiz gerekiyor. İşte nasıl yapabileceğimiz:
        char hiz_str[10];
        sprintf(hiz_str, "%.1f", hız); // Hızı string'e çevir (örneğin, 1.0, 1.5 gibi)
        Render_DrawText(8, 2, hiz_str, SARI); // Hızı ekrana yazdır

        if(!etkilendi) Render_DrawText(oyuncuCizim_x + 1, oyuncuCizim_y - 1, "Burasi neresi?", BEYAZ); 
        Render_DrawPixel(canavar_x, canavar_y, 'M', KIRMIZI);
        Render_DrawText(2, 1, "voidEngine v0.3", CYAN);

        if(etkilendi && oyuncuCizim_x > canavar_x - 2 && oyuncuCizim_x < canavar_x + 2 && oyuncuCizim_y > canavar_y - 2 && oyuncuCizim_y < canavar_y + 2    ){
            Render_DrawText(oyuncuCizim_x+1, oyuncuCizim_y-1, "Sende kimsin?", BOLD);
        }
        else{
            Render_DrawText(oyuncuCizim_x + 1, oyuncuCizim_y - 1, "Burasi neresi?", BEYAZ);
            etkilendi=false; 
        } 
    }
}
