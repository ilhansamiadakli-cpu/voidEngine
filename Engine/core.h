#ifndef CORE_H
#define CORE_H

#ifdef __cplusplus
extern "C" {
#endif
#include <stdbool.h>

// --- FPS AYARLARI ---
extern int target_ms; // Varsayılan: 60 FPS

void Engine_Init(void);
void Engine_Stop(void);
void Engine_Close(void); // Terminali eski haline getir
void setTargetFPS(int fps);
double GetCurrentTimeInSeconds(void); // Zaman ölçümü için


// Motorun ana döngüsü. Dışarıdan oyunun kurallarını parametre olarak alır.
void Engine_Run(void (*UpdateFunc)(int, float), void (*RenderFunc)(float));

#ifdef __cplusplus
}
#endif

#endif // CORE_H