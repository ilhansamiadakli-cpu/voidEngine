// engine/utils.c dosyasının içi
#include "utils.h"
#include "colors.h"
#include "core.h"
#include <stdio.h>
#include <time.h>
#include <stdbool.h>

#ifdef _WIN32
    #include <conio.h>   
    #include <windows.h> 
#else
    #include <termios.h> 
    #include <unistd.h> 
    #include <fcntl.h> 
    static struct termios orig_termios;
    static int is_raw = 0;
#endif


void Typewriter_Baslat(Typewriter* tw, const char* metin, int hiz_ms) {
    tw->metin = metin;
    tw->su_anki_indis = 0;
    tw->hız_ms = hiz_ms;
    tw->sayac_ms = 0;
    tw->bitti_mi = false;
}

void Typewriter_Guncelle(Typewriter* tw) {
    if (tw->bitti_mi) return;

    tw->sayac_ms += target_ms; //Her frame ~target_ms (60 FPS varsayımıyla)

    if (tw->sayac_ms >= tw->hız_ms) {
        tw->su_anki_indis++;
        tw->sayac_ms = 0;
        
        if (tw->metin[tw->su_anki_indis] == '\0') {
            tw->bitti_mi = true;
        }
    }
}

// --- OS AYARLARI ---
void WindowsAyarlariniYap(void) {
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;
    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode)) return;
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
#endif
}

void terminal_raw_moduna_gec(void) {
#ifndef _WIN32
    if (is_raw) return;
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) return;
    struct termios raw = orig_termios;

    raw.c_lflag &= ~(ICANON | ECHO); 
    
    // MOTOR MODU: VMIN = 0. Tuş yoksa anında devam et! (Non-blocking)
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 0;

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) != -1)
        is_raw = 1;
#endif
}

void terminal_normal_moda_don(void) {
#ifndef _WIN32
    if (is_raw) {
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
        is_raw = 0;
    }
#endif
}

void alternatif_ekrana_gec(void) {
    printf("\033[?1049h");
    fflush(stdout);
}

void orijinal_ekrana_don(void) {
    printf("\033[?1049l");
    fflush(stdout);
}

// --- EKRAN VE İMLEÇ ---
// Not: Bu fonksiyonlar döngü içinde çağrılmamalıdır.
void ekrani_temizle(void) {
#ifdef _WIN32
    system("cls");
#else
    printf("\033[H\033[J");
    fflush(stdout);
#endif
}

void imleci_tasi(int x, int y) {
    printf("\033[%d;%dH", y, x);
    fflush(stdout);
}

void imleci_gizle(void) {
    printf("\033[?25l");
    fflush(stdout);
}

void imleci_goster(void) {
    printf("\033[?25h");
    fflush(stdout);
}

// --- ZAMANLAMA ---
void saniye_bekle(double saniye) {
    ms_bekle((int)(saniye * 1000));
}

void ms_bekle(int milisaniye) {
#ifdef _WIN32
    Sleep(milisaniye);
#else
    struct timespec ts;
    ts.tv_sec = milisaniye / 1000;
    ts.tv_nsec = (milisaniye % 1000) * 1000000;
    while (nanosleep(&ts, &ts) == -1);
#endif
}

// --- EFEKTLER ---
// Not: Bu fonksiyonun hızı target_ms ile çarpılarak çalışır.Döngü içinde çağrılmamalıdır.
void Daktilo(const char* metin, int ms, const char* renk) {
    if (renk) fputs(renk, stdout); 
    const unsigned char *p = (const unsigned char *)metin;
    while (*p) {
        int b = 1;
        if ((*p & 0x80) == 0x00) b = 1;
        else if ((*p & 0xe0) == 0xc0) b = 2;
        else if ((*p & 0xf0) == 0xe0) b = 3;
        else if ((*p & 0xf8) == 0xf0) b = 4;

        for (int i = 0; i < b && p[i]; i++) {
            putchar(p[i]);
        }
        fflush(stdout);
        p += b;

        if (ms > 0) ms_bekle(ms);
    }
    fputs(BEYAZ, stdout);
    fflush(stdout);
}