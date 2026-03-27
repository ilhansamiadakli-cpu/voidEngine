
#ifndef PHYSICS_H
#define PHYSICS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

// Her fiziksel objenin sahip olacağı görünmez sınır kutusu (AABB)
typedef struct {
    float x;      // Kutunun sol üst köşesinin X koordinatı
    float y;      // Kutunun sol üst köşesinin Y koordinatı
    float width;  // Kutunun genişliği
    float height; // Kutunun yüksekliği
} AABB;

// İki AABB kutusu birbirine temas ediyor mu?
bool Physics_CheckCollision(AABB a, AABB b);

#ifdef __cplusplus
}
#endif

#endif // PHYSICS_H