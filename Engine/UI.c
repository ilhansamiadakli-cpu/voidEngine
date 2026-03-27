#include "UI.h"
#include "render.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h> // atoi() fonksiyonu için

static UIElement menu_elements[20]; // Kapasiteyi biraz artırdık
static int element_count = 0;
static int current_selection = 0;

static int daktiloMs = 50; // Daktilo güncellemesi için zaman sayacı

// Metin olarak gelen renk adını, ANSI renk koduna çeviren Sözlük
static const char* RengiCoz(const char* renk_adi) {
    if (strcmp(renk_adi, "KIRMIZI") == 0) return KIRMIZI;
    if (strcmp(renk_adi, "YESIL") == 0)   return YESIL;
    if (strcmp(renk_adi, "SARI") == 0)    return SARI;
    if (strcmp(renk_adi, "MAVI") == 0)    return MAVI;
    if (strcmp(renk_adi, "CYAN") == 0)    return CYAN;
    if (strcmp(renk_adi, "MAGENTA")== 0)  return MAGENTA;
    return BEYAZ; // Hiçbiri eşleşmezse varsayılan beyaz olsun
}

void Typewriter_MsGuncelle(int ms) {
    daktiloMs = ms;
    if (daktiloMs < 0) {
        daktiloMs = 0;
    }
}



void UI_LoadMenu(const char* dosya_yolu) {
    FILE* file = fopen(dosya_yolu, "r");
    
    if (!file) {
        sprintf(menu_elements[0].text, "HATA: MENU DOSYASI YOK!");
        menu_elements[0].type = UI_BUTTON;
        menu_elements[0].x = 10;
        menu_elements[0].y = 5;
        menu_elements[0].action = ACTION_QUIT;
        element_count = 1;
        current_selection = 0;
        return;
    }

    element_count = 0;
    char type_str[16];
    char x_str[16];
    char renk_str[16];

    // Dosyayı kelime kelime oku
    while (fscanf(file, "%s", type_str) != EOF) {
        UIElement* el = &menu_elements[element_count];

    // 1. TÜRÜ BELİRLE VE GERİ KALANI OKU
        if (strcmp(type_str, "BUTTON") == 0) {
            el->type = UI_BUTTON;
            // Sıra: X, Y, RENK, METİN, AKSİYON
            fscanf(file, "%s %d %s %s %d", x_str, &el->y, renk_str, el->text, (int*)&el->action);
        } 
        else if (strcmp(type_str, "LABEL") == 0) {
            el->type = UI_LABEL;
            el->action = ACTION_NONE;
            // Sıra: X, Y, RENK, METİN
            fscanf(file, "%s %d %s %s", x_str, &el->y, renk_str, el->text);
        }
        else if (strcmp(type_str, "TWLABEL") == 0) { 
            el->type = UI_TWLABEL;
            el->action = ACTION_NONE;
            // Sıra: X, Y, RENK, METİN
            fscanf(file, "%s %d %s %s", x_str, &el->y, renk_str, el->text);
        }
        else {
            // Eğer geliştirici uydurma bir kelime yazdıysa, o satırı yut ve çöpe at!
            char cop[128];
            fgets(cop, sizeof(cop), file); // Satırın sonuna kadar her şeyi oku ve at
            continue; // Bu elemanı sayma, bir sonraki satıra geç
        }


        // Okuduğumuz "KIRMIZI" gibi metni, sözlükten geçirip kalıcı renge dönüştür
        el->renk_kodu = RengiCoz(renk_str);

        // 2. BOŞLUKLARI DÜZELT (Alt tireleri boşluğa çevir)
        for(int i = 0; el->text[i] != '\0'; i++) {
            if(el->text[i] == '_') el->text[i] = ' ';
        }

        // 3. X KOORDİNATI ZEKASI ("CENTER" MATEMATİĞİ)
        if (strcmp(x_str, "CENTER") == 0) {
            int uzunluk = strlen(el->text);
            el->x = (SCR_WIDTH - uzunluk) / 2; // Tam ortaya hizala!
        } else {
            el->x = atoi(x_str); // Normal sayıysa sayıyı al
        }

        if (el->type == UI_LABEL) {
            
            Typewriter_Baslat(&el->tw, el->text, daktiloMs); 
        }
        else if (el->type == UI_TWLABEL) {
            Typewriter_Baslat(&el->tw, el->text, daktiloMs);
        }

        element_count++;
    }
    fclose(file);

    // Başlangıçta imleci ilk BUTTON neresiyse oraya koy (LABEL'da durmasın)
    current_selection = 0;
    for(int i = 0; i < element_count; i++) {
        if(menu_elements[i].type == UI_BUTTON) {
            current_selection = i;
            break;
        }
    }
}

// Yardımcı Fonksiyon: Sadece BUTTON'lar arasında zıpla
static void SonrakiButonuBul(int yon) {
    int baslangic = current_selection;
    do {
        current_selection = (current_selection + yon + element_count) % element_count;
        if (menu_elements[current_selection].type == UI_BUTTON) return;
    } while (current_selection != baslangic);
}

ButtonAction UI_Update(char tus) {

// Sadece TWLABEL olanların daktilosunu güncelle
    for (int i = 0; i < element_count; i++) {
        if (menu_elements[i].type == UI_TWLABEL) {
            Typewriter_Guncelle(&menu_elements[i].tw);
        }
    }

    if (tus == 'w') SonrakiButonuBul(-1); // Yukarı
    if (tus == 's') SonrakiButonuBul(1);  // Aşağı
    
    if (tus == '\n' || tus == 'e') { 
        return menu_elements[current_selection].action;
    }
    return ACTION_NONE;
}

void UI_Render(void) {
    for (int i = 0; i < element_count; i++) {
        char buffer[128];
        const char* renk;

        if (menu_elements[i].type == UI_LABEL) {
            // sprintf yerine snprintf! (Boyut kontrolü eklendi)
            snprintf(buffer, sizeof(buffer), "%s", menu_elements[i].text);
            renk = menu_elements[i].renk_kodu;
        }
        else if (menu_elements[i].type == UI_TWLABEL) {
            snprintf(buffer, sizeof(buffer), "%.*s", menu_elements[i].tw.su_anki_indis, menu_elements[i].text);
            renk = menu_elements[i].renk_kodu; // Kendi rengini kullan!
        }
        else if (menu_elements[i].type == UI_BUTTON) {
            if (i == current_selection) {
                snprintf(buffer, sizeof(buffer), "> %s <", menu_elements[i].text);
                renk = YESIL; // Seçiliyse parlasın (Bunu da istersen ileride txt'ye bağlayabiliriz)
            } else {
                snprintf(buffer, sizeof(buffer), "  %s  ", menu_elements[i].text);
                renk = menu_elements[i].renk_kodu; // Seçili değilse kendi renginde dursun
            }
        }
        
        Render_DrawText(menu_elements[i].x, menu_elements[i].y, buffer, renk);
    }
}