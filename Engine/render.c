#include <stdio.h>
#include "render.h"
#include "colors.h"


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

void DrawFPS(int fps, int fps_x, int fps_y) {
    char* color;
    if (fps_x < 0) fps_x = 0;
    if (fps_x > SCR_WIDTH - 10) fps_x = SCR_WIDTH - 10; // FPS metni 10 karakter uzunluğunda, sınırları aşmamak için -10
    if (fps_y < 0) fps_y = 0;
    if (fps_y > SCR_HEIGHT - 1) fps_y = SCR_HEIGHT - 1; // FPS metni tek satır, sınırları aşmamak için -1
    
    if (fps >= 60) {
        color = YESIL; // Yeşil: Harika performans

    }
    else if (fps >= 30) {
        color = SARI; // Sarı: Kabul edilebilir performans
    } else {
        color = KIRMIZI; // Kırmızı: Düşük performans
    }
    char fps_text[20];
    snprintf(fps_text, sizeof(fps_text), "[FPS: %d]", fps);
    Render_DrawText(fps_x, fps_y, fps_text, color);
}
