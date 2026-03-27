#include <stdio.h>
#include "render.h"
#include "colors.h"
#include "core.h"
#define FPS_SAMPLES 30 // Ortalama FPS için kaç karelik veri tutacağımız

// --- MOTORUN İÇ ORGANLARI (Dışarıdan erişilemez!) ---
static char screen_buffer[SCR_HEIGHT][SCR_WIDTH];
static const char* color_buffer[SCR_HEIGHT][SCR_WIDTH];


void Render_BeginDrawing(void) {
    // Başlangıçta ekranı temizleyelim
    Render_ClearBuffer();
}

void Render_ClearBuffer(void) {
    for (int y = 0; y < SCR_HEIGHT; y++) {
        for (int x = 0; x < SCR_WIDTH; x++) {
            screen_buffer[y][x] = ' ';         // Zemin boşluk
            color_buffer[y][x] = RESET; // Renk yok
        }
    }
}

// 2. Güvenli Piksel Çizimi (Sınırları asla aşmaz!)
void Render_DrawPixel(int x, int y, char c, const char* renk) {
    // Ölümcül Buffer Overflow hatasını engelleyen o altın kalkan:
    if (x >= 0 && x < SCR_WIDTH && y >= 0 && y < SCR_HEIGHT) {
        screen_buffer[y][x] = c;
        color_buffer[y][x] = renk;
    }
}

// 3. Menüler ve UI İçin Metin Çizimi
void Render_DrawText(int x, int y, const char* metin, const char* renk) {
    int i = 0;
    // Metnin sonuna (Null-terminator) gelene kadar döngüye devam et
    while (metin[i] != '\0') {
        // Her harfi kendi güvenli fonksiyonumuzla tuvale yolluyoruz
        Render_DrawPixel(x + i, y, metin[i], renk);
        i++;
    }
}

// 4. Tabloyu Ekrana Bas (Sihrin Gerçekleştiği Yer)
void Render_EndDrawing(void) {
    // Terminali silmek yerine imleci sol üst köşeye (1,1) alıyoruz.
    // Bu sayede ekran asla titremez, sadece değişen karakterler üstüne yazılır!
    printf("\033[H"); 

    for (int y = 0; y < SCR_HEIGHT; y++) {
        for (int x = 0; x < SCR_WIDTH; x++) {
            // RAM'deki rengi ve karakteri terminale yolla
            printf("%s%c", color_buffer[y][x], screen_buffer[y][x]);
        }
        
        // Alt satıra geç (Son satırda geçmiyoruz ki ekran aşağı kaymasın)
        if (y < SCR_HEIGHT - 1) {
            printf("\n");
        }
    }
    
    // Rengi normale döndür ve ekran kartına (stdout) "ŞİMDİ ÇİZ!" emrini ver
    printf("%s", RESET);
    fflush(stdout);
}


void DrawFPS(int fps_x, int fps_y, float deltaTime) {
    if (deltaTime <= 0.0f) deltaTime=0.0001; // Sıfıra bölünme koruması

    // --- ORTALAMA FPS HESAPLAMA (CIRCULAR BUFFER) ---
    static float frame_times[FPS_SAMPLES] = {0};
    static int current_sample = 0;
    static bool initialized = false;

    // Motor ilk açıldığında dizi boş olmasın diye varsayılan 60 FPS (0.016) ile dolduruyoruz
    if (!initialized) {
        for(int i = 0; i < FPS_SAMPLES; i++) {
            frame_times[i] = target_ms > 0 ? (1.0f / (1000.0f / target_ms)) : 0.00001f; 
        }
        initialized = true;
    }

    // Yeni gelen deltaTime'ı dizideki sıradaki yere yaz (eskiyi ez)
    frame_times[current_sample] = deltaTime;
    current_sample = (current_sample + 1) % FPS_SAMPLES; // 10'a gelince tekrar 0'a dön

    // Dizideki son 10 karenin toplam zamanını bul
    float sum = 0.0f;
    for (int i = 0; i < FPS_SAMPLES; i++) {
        sum += frame_times[i];
    }
    
    // Toplamı 10'a bölerek Ortalama Delta Time'ı bul
    float avg_deltaTime = sum / FPS_SAMPLES;
    int fps = (int)(1.0f / avg_deltaTime); // Ortalama FPS'i hesapla
    // --------------------------------------------------

    // --- ÇİZİM VE RENK KISMI (Aynı Kaldı) ---
    char* color;
    if (fps_x < 0) fps_x = 0;
    if (fps_x > SCR_WIDTH - 10) fps_x = SCR_WIDTH - 10; 
    if (fps_y < 0) fps_y = 0;
    if (fps_y > SCR_HEIGHT - 1) fps_y = SCR_HEIGHT - 1; 

    if (fps >= 50) {
        color = YESIL; 
    } else if (fps >= 30) {
        color = SARI; 
    } else {
        color = KIRMIZI; 
    }
    
    char fps_text[20];
    snprintf(fps_text, sizeof(fps_text), "[FPS: %d]", fps);
    Render_DrawText(fps_x, fps_y, fps_text, color);
}
