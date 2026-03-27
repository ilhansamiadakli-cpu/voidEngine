
#include "input.h"
#include "utils.h"
#include <stdio.h>
#include <stdbool.h>
#ifdef _WIN32
    #include <conio.h>
#else
    #include <unistd.h>
#endif

char Input_GetRawKey(void) {
#ifdef _WIN32
    if (_kbhit()) {
        return _getch();
    }
    return 0; 
#else
    char c = 0;
    // Terminal VMIN=0 ayarlandığı için read fonksiyonu bloklamaz
    if (read(STDIN_FILENO, &c, 1) < 0) {
        return 0; 
    }
    return c;
#endif
}
// Bu fonksiyon, tuş gelene kadar bekler ve ardından o tuşu döndürür
char Input_GetPressed(void) {
    char tus = 0;
    // Tuş gelene kadar döngüyü işlet
    while (tus == 0) {
        tus = Input_GetRawKey();
        
        // Döngü CPU'yu %100 yakmasın diye 1 milisaniye dinlendir
        if (tus == 0) {
            ms_bekle(1); 
        }
    }
    return tus;
}