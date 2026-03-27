#ifndef UI_H
#define UI_H

#include "colors.h"
#include "utils.h" // Daktilo yapısı için utils'i dahil ettik!

typedef enum {
    UI_LABEL = 0,       // Sıkıcı, normal, hareketsiz başlık
    UI_BUTTON = 1,      // Tıklanabilir tuş
    UI_TWLABEL = 2      // YENİ: Havalı, daktilo efektli başlık (TypeWriter Label)
} ElementType;

typedef enum {
    ACTION_NONE = 0,
    ACTION_START_GAME = 1,
    ACTION_QUIT = 2
} ButtonAction;

typedef struct {
    ElementType type;
    int x, y;
    const char* renk_kodu; // Renk kodu!
    char text[64];
    ButtonAction action;
    Typewriter tw; 
} UIElement;

// ... (Geri kalan prototipler aynı)
void UI_Init(void);
void UI_LoadMenu(const char* dosya_yolu);
ButtonAction UI_Update(char tus);
void UI_Render(void);
void Typewriter_MsGuncelle(int ms); // Daktilo hızını güncellemek için yeni fonksiyon

#endif // UI_H