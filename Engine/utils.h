// engine/utils.h dosyasının içi
#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

typedef struct {
    const char* metin;
    int su_anki_indis;
    int hız_ms;
    int sayac_ms;
    bool bitti_mi;
} Typewriter;

// Fonksiyon prototipleri
void Typewriter_Baslat(Typewriter* tw, const char* metin, int hiz_ms);
void Typewriter_Guncelle(Typewriter* tw); //Her frame'de çağrılacak

// --- OS ve Terminal Ayarları ---
void WindowsAyarlariniYap(void);
void terminal_raw_moduna_gec(void);
void terminal_normal_moda_don(void);
void alternatif_ekrana_gec(void);
void orijinal_ekrana_don(void);

// --- Çizim ve Ekran Araçları ---
void ekrani_temizle(void);
void imleci_tasi(int x, int y); // Titremesiz çizim (Yeni!)
void imleci_gizle(void);        // Yanıp sönen çizgiyi sakla (Yeni!)
void imleci_goster(void);
void Daktilo(const char* metin, int ms, const char* renk);

// --- Zamanlama ---
void saniye_bekle(double saniye);
void ms_bekle(int milisaniye);  // Game Loop FPS kilidi için (Yeni!)

#endif // UTILS_H