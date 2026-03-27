
#include "input.h"
#include "utils.h"
#include <stdio.h>
#include <stdbool.h>
#ifdef _WIN32
    #include <conio.h>
#else
    #include <unistd.h>
#endif

int Input_GetRawKey(void) {
#ifdef _WIN32
    if (_kbhit()) {
        int c = _getch();
        // Windows Ok Tuşu Yakalayıcı: Önce 0 veya 224 gelir
        if (c == 0 || c == 224) {
            c = _getch(); // Asıl yön kodunu oku
            if (c == 72) return KEY_UP;
            if (c == 80) return KEY_DOWN;
            if (c == 75) return KEY_LEFT;
            if (c == 77) return KEY_RIGHT;
        }
        return c; // Normal tuşsa aynen yolla
    }
    return 0; 
#else
    char c = 0;
    if (read(STDIN_FILENO, &c, 1) > 0) {
        // Mac/Linux Ok Tuşu Yakalayıcı: Önce ESC (\033) gelir
        if (c == '\033') { 
            char seq[2];
            // Sonraki 2 baytı ( [ ve Harf ) okumaya çalış
            if (read(STDIN_FILENO, &seq[0], 1) == 0) return '\033';
            if (read(STDIN_FILENO, &seq[1], 1) == 0) return '\033';

            if (seq[0] == '[') {
                if (seq[1] == 'A') return KEY_UP;
                if (seq[1] == 'B') return KEY_DOWN;
                if (seq[1] == 'C') return KEY_RIGHT;
                if (seq[1] == 'D') return KEY_LEFT;
            }
        }
        return c; // Normal tuşsa aynen yolla
    }
    return 0;
#endif
}

// Bu fonksiyonu da int yapıyoruz
int Input_GetPressed(void) {
    int tus = 0;
    while (tus == 0) {
        tus = Input_GetRawKey();
        if (tus == 0) {
            ms_bekle(1); 
        }
    }
    return tus;
}