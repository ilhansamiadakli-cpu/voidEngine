// Game/game.c
#include "game.h"
#include "../Engine/core.h"
#include "../Engine/utils.h"
#include "../Engine/colors.h"
#include <stdio.h>

// Oyun nesnelerimiz
Typewriter hikaye_kutusu;
int oyuncu_x = 5;
int oyuncu_y = 5;

void Oyun_Baslat(void) {
    // Motoru 30 FPS'e sabitleyelim (Retro hissi için)
    setTargetFPS(30); 
    
    // Daktiloyu kur (100ms hızında yazsın)
    Typewriter_Baslat(&hikaye_kutusu, "VoidEngine 0.2 Sistemleri Aktif... Dusmanlar yaklasiyor!", 100);
}

void Oyun_Guncelle(char tus) {
    // 1. Daktiloyu ilerlet (Bloklamaz, sadece zamanı gelince bir harf açar)
    Typewriter_Guncelle(&hikaye_kutusu);

    // 2. Oyuncu hareketleri (Daktilo yazarken bile çalışır!)
    if (tus == 'w') oyuncu_y--;
    if (tus == 's') oyuncu_y++;
    if (tus == 'a') oyuncu_x--;
    if (tus == 'd') oyuncu_x++;
    
    if (tus == 'q') Engine_Stop();
}

void Oyun_Ekrana_Ciz(void) {
    // Oyuncuyu çiz
    imleci_tasi(oyuncu_x, oyuncu_y);
    printf("%s@%s", YESIL, BEYAZ);

    // Daktilo Metnini Alt Kısma Çiz
    imleci_tasi(2, 20);
    printf("%s[MESAJ]: %s", SARI, BEYAZ);
    for(int i = 0; i < hikaye_kutusu.su_anki_indis; i++) {
        putchar(hikaye_kutusu.metin[i]);
    }
    
    // FPS bilgisini köşeye yazalım
    imleci_tasi(2, 2);
    printf("%sFPS Hedefi: %d%s", SIYAH_ARKA, 1000/target_ms, BEYAZ);
}
 

