#include "core.h"
#include "input.h"
#include "render.h"
#include "utils.h"
#include <stddef.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

static bool is_running = false;
int target_ms = 16;
// Ctrl+C (SIGINT) sinyalini yakalayan fonksiyon
static void Guvenli_Kapat(int sig) {
    (void)sig; // Kullanılmayan değişken uyarısını önlemek için
    Engine_Close(); // Terminali anında eski sağlıklı haline getir
    exit(0); // Programı güvenlice sonlandır
}
void setTargetFPS(int fps){
 if(fps>0){
    target_ms=1000/fps;
 }
 else{
    target_ms=0;
 }
}

void Engine_Init(void) {
    // Program sonlandığında (Ctrl+C veya normal çıkış) Engine_Close fonksiyonunu otomatik çalıştır
    atexit(Engine_Close);

    signal(SIGINT, Guvenli_Kapat);  // Ctrl+C
    signal(SIGTERM, Guvenli_Kapat); // Kapatma emri
    signal(SIGSEGV, Guvenli_Kapat); // Çökme (Segmentation Fault)
    signal(SIGABRT, Guvenli_Kapat); // Anormal bitiş

    WindowsAyarlariniYap();
    alternatif_ekrana_gec();     // Oyuncunun terminal geçmişini gizle, temiz sayfa aç
    terminal_raw_moduna_gec();   // Beklemesiz moda geç
    imleci_gizle();              // Yanıp sönen gıcık çizgiyi yok et
    ekrani_temizle();            // Başlangıçta bir kere ekranı sil
}
void Engine_Close(void){
    // Bu fonksiyonun sadece bir kez çalışmasını sağla
    static bool already_closed = false;
    if (already_closed) return;
    already_closed = true;
    
    imleci_goster();
    terminal_normal_moda_don();// Beklemesiz moddan normal moda dön
    orijinal_ekrana_don(); // Oyuncunun eski terminal geçmişini geri getir

    fflush(stdout); //ekstra önlem :)
}

void Engine_Stop(void) {
    is_running = false;
}

// İŞTE MOTORUN KALBİ (THE GAME LOOP)
void Engine_Run(void (*UpdateFunc)(char), void (*RenderFunc)(void)) {
    is_running = true;

    while (is_running) {
        // 1. GİRDİ (INPUT)
        char tus = Input_GetRawKey();

        // 2. GÜNCELLEME (UPDATE)
        // Motor oyunun ne olduğunu bilmez, sadece "Oyun geliştiricisi ne verdiyse onu çalıştır" der.
        if (UpdateFunc != NULL) {
            UpdateFunc(tus);
        }

        // 3. ÇİZİM (RENDER)
        if (RenderFunc != NULL) {
            Render_BeginDrawing(); // İmleci başa al
            RenderFunc();        // Oyunun çizim fonksiyonunu çağır (Eski karakterleri ez)
            Render_EndDrawing();   // Ekrana yansıt
        }

        // FPS KİLİDİ
        ms_bekle(target_ms); 
    }

    // Oyun bittiğinde/Motor durduğunda etrafı temizle
    Engine_Close();
}