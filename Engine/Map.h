#ifndef MAP_H
#define MAP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "Physics.h"
#include <stdbool.h>

#define MAX_WALLS 2000 // Haritada en fazla 2000 adet duvar bloğu olabilir

// Haritayı yükler, ekrana çizmek için kaydeder ve duvarları fiziksel kutulara dönüştürür
void Map_Load(const char* dosya_yolu);

// Verilen kutu (oyuncu/canavar) haritadaki herhangi bir duvara çarpıyor mu?
bool Map_CheckCollision(AABB hedefKutu);

// Haritayı ekrana çizmek için
void Map_Render(char *color);

#ifdef __cplusplus
}
#endif

#endif // MAP_H