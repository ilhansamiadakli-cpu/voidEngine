#include "core.h"
#include "input.h"
#include "render.h"
#include "utils.h"
#include <stddef.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <time.h>
#endif

// İşletim sisteminden "Saniye" cinsinden ultra hassas zamanı çeker
double GetCurrentTimeInSeconds(void) {
#ifdef _WIN32
    LARGE_INTEGER freq, counter;
    QueryPerformanceFrequency(&freq);   // İşlemcinin tik hızını al
    QueryPerformanceCounter(&counter);  // Şu anki tiki al
    return (double)counter.QuadPart / freq.QuadPart;
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts); // Geriye sarılamayan sistem saati
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1000000000.0;
#endif
}

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
void Engine_Run(void (*UpdateFunc)(int, float), void (*RenderFunc)(float)) {
    is_running = true;
    double lastTime = GetCurrentTimeInSeconds();// Zaman ölçümü için başlangıç zamanı

    while (is_running) {
        // ZAMAN (deltaTIME)
        double currentTime = GetCurrentTimeInSeconds();// Şu anki zamanı al
        float deltaTime = (float)(currentTime - lastTime);// İki döngü arasındaki geçen süre (saniye cinsinden)
        lastTime = currentTime;
        
        // 1. GİRİŞ (INPUT)
        int tus = Input_GetRawKey();

        // 2. GÜNCELLEME (UPDATE)
        // Motor oyunun ne olduğunu bilmez, sadece "Oyun geliştiricisi ne verdiyse onu çalıştır" der.
        if (UpdateFunc != NULL) {
            UpdateFunc(tus, deltaTime);
        }

        // 3. ÇİZİM (RENDER)
        if (RenderFunc != NULL) {
            Render_BeginDrawing(); // İmleci başa al
            RenderFunc(deltaTime);        // Oyunun çizim fonksiyonunu çağır (Eski karakterleri ez)
            Render_EndDrawing();   // Ekrana yansıt
        }

        // FPS KİLİDİ (İsteğe bağlı, ama genellikle oyunlarda olur)
        ms_bekle(target_ms); 
    }

    // Oyun bittiğinde/Motor durduğunda etrafı temizle
    Engine_Close();
}