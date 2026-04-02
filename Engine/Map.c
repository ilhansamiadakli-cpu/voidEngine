// Engine/Map.c
#include "Map.h"
#include "render.h"
#include <stdio.h>
#include <string.h>

// --- MOTORUN GİZLİ İÇ ORGANLARI ---
// 1. Fiziksel Duvarlar (Çarpışma için)
static AABB duvarlar[MAX_WALLS];
static int duvar_sayisi = 0;

// 2. Görsel Harita (Ekrana çizmek için)
static char harita_gorsel[SCR_HEIGHT][SCR_WIDTH];

void Map_Load(const char* dosya_yolu) {
    FILE* dosya = fopen(dosya_yolu, "r");
    if (!dosya) {
        printf("Harita dosyasi bulunamadi: %s\n", dosya_yolu);
        return;
    }

    duvar_sayisi = 0;
    char satir[SCR_WIDTH + 2]; 
    int y = 0;

    // Önce haritayı boşluklarla (uzayla) dolduralım
    for (int i = 0; i < SCR_HEIGHT; i++) {
        for (int j = 0; j < SCR_WIDTH; j++) {
            harita_gorsel[i][j] = ' ';
        }
    }

    // Dosyayı satır satır oku
    while (fgets(satir, sizeof(satir), dosya) && y < SCR_HEIGHT) {
        for (int x = 0; x < SCR_WIDTH && satir[x] != '\0' && satir[x] != '\n'; x++) {
            harita_gorsel[y][x] = satir[x]; // Görsel kopyayı kaydet

            // SİHİRLİ KISIM: Eğer okuduğun harf '#' ise, onu fizik kutusuna çevir!
            if (satir[x] == '#') { 
                if (duvar_sayisi < MAX_WALLS) {
                    duvarlar[duvar_sayisi].x = (float)x;
                    duvarlar[duvar_sayisi].y = (float)y;
                    duvarlar[duvar_sayisi].width = 1.0f;
                    duvarlar[duvar_sayisi].height = 1.0f;
                    duvar_sayisi++;
                }
            }
        }
        y++;
    }
    fclose(dosya);
}

bool Map_CheckCollision(AABB hedefKutu) {
    // Verilen hedef kutuyu, haritadaki tüm duvarlarla tek tek test et
    for (int i = 0; i < duvar_sayisi; i++) {
        if (Physics_CheckCollision(hedefKutu, duvarlar[i])) {
            return true; // Herhangi birine bile çarparsa hareketi iptal et!
        }
    }
    return false; // Yol açık
}

void Map_Render(char *color) {
    for (int y = 0; y < SCR_HEIGHT; y++) {
        for (int x = 0; x < SCR_WIDTH; x++) {
            if (harita_gorsel[y][x] != ' ') {
                Render_DrawPixel(x, y, harita_gorsel[y][x], color);
            }
        }
    }
}