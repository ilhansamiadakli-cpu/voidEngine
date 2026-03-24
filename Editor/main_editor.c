// Editor/main_editor.c dosyasının içi
#include "../Engine/core.h"
#include "../Engine/utils.h"
#include "../Engine/colors.h"
#include <stdio.h>
#include <string.h>

#define GENISLIK 40
#define YUKSEKLIK 15

char harita[YUKSEKLIK][GENISLIK];

// Editör Fırçası ve Seçili Obje
int firca_x = GENISLIK / 2;
int firca_y = YUKSEKLIK / 2;
char secili_obje = '#'; // Başlangıçta duvar çizmeye ayarlı

void Editor_Baslat(void) {
    // Başlangıçta tuvali boşluklarla (space) doldur
    for (int y = 0; y < YUKSEKLIK; y++) {
        for (int x = 0; x < GENISLIK; x++) {
            harita[y][x] = ' ';
        }
    }
    
    // Etrafına varsayılan bir sınır çizelim işimiz kolaylaşsın
    for(int x = 0; x < GENISLIK; x++) {
        harita[0][x] = '#';
        harita[YUKSEKLIK-1][x] = '#';
    }
    for(int y = 0; y < YUKSEKLIK; y++) {
        harita[y][0] = '#';
        harita[y][GENISLIK-1] = '#';
    }
}

void Haritayi_Kaydet(void) {
    // Çizdiğimiz haritayı oyunun okuyacağı txt dosyasına yazdırıyoruz
    FILE *dosya = fopen("Game/bolum1.txt", "w");
    if (dosya == NULL) return;

    for (int y = 0; y < YUKSEKLIK; y++) {
        for (int x = 0; x < GENISLIK; x++) {
            fputc(harita[y][x], dosya);
        }
        fputc('\n', dosya); // Satır sonu
    }
    fclose(dosya);
}

void Editor_Guncelle(char tus) {
    if (tus == 'q') {
        Engine_Stop();
        return;
    }

    // 1. FIRÇAYI HAREKET ETTİR (Duvarların içinden geçebilir!)
    if (tus == 'w' && firca_y > 0) firca_y--;
    if (tus == 's' && firca_y < YUKSEKLIK - 1) firca_y++;
    if (tus == 'a' && firca_x > 0) firca_x--;
    if (tus == 'd' && firca_x < GENISLIK - 1) firca_x++;

    // 2. PALETTEN OBJE SEÇ
    if (tus == '1') secili_obje = '#'; // Duvar
    if (tus == '2') secili_obje = ' '; // Silgi (Boşluk)
    if (tus == '3') secili_obje = '@'; // Oyuncu Başlangıç Noktası
    if (tus == '4') secili_obje = 'M'; // Canavar

    // 3. TUVALE BOYA SÜR
    if (tus == ' ') { 
        harita[firca_y][firca_x] = secili_obje;
    }

    // 4. KAYDET (SERIALIZATION)
    if (tus == 'k') {
        Haritayi_Kaydet();
    }
}

void Editor_Ciz(void) {
    for (int y = 0; y < YUKSEKLIK; y++) {
        for (int x = 0; x < GENISLIK; x++) {
            
            // Eğer fırça buradaysa, ARKA PLANI SARI yap ki imleci görelim
            if (x == firca_x && y == firca_y) {
                printf("%s%c%s", SARI_ARKA, harita[y][x], BEYAZ);
            } 
            else if (harita[y][x] == '#') {
                printf("%s#%s", MAVI, BEYAZ);
            }
            else if (harita[y][x] == '@') {
                printf("%s@%s", YESIL, BEYAZ);
            }
            else {
                printf("%c", harita[y][x]);
            }
            
        }
        printf("\n");
    }

    // Editör Arayüzü (Inspector)
    printf("\n%s--- VOIDCORE HARITA EDITORU ---%s\n", SARI, BEYAZ);
    printf("Firca Hareketi: W,A,S,D | Boya: SPACE | Kaydet: K | Cikis: Q\n");
    printf("Palet: [1] Duvar(#)  [2] Silgi( )  [3] Oyuncu(@)  [4] Canavar(M)\n");
    printf("Secili Obje: '%c' | Firca Koordinati: X:%d Y:%d\n", secili_obje, firca_x, firca_y);
}

int main(void) {
    Engine_Init();
    Editor_Baslat();
    
    // Motor bu sefer oyunu değil, Editörü çalıştırıyor!
    Engine_Run(Editor_Guncelle, Editor_Ciz);
    
    Engine_Close();
    return 0;
}