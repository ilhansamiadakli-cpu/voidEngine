// Game/main.c dosyasının içi
#include "../Engine/core.h"
#include "game.h"

int main(void) {
    Engine_Init(); // Motoru, terminali ve donanımı hazırla
    Oyun_Baslat(); // Oyunun kendi iç ayarlarını (varsa) yap
    
    // Motoru çalıştır ve "Şu fonksiyonları kullanarak dön" de
    Engine_Run(Oyun_Guncelle, Oyun_Ekrana_Ciz); 
    
    Engine_Close(); // Program kapanırken terminali eski haline getir
    return 0;
}