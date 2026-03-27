#include "Physics.h"

bool Physics_CheckCollision(AABB a, AABB b) {
    // AABB Çarpışma Matematiği:
    // Eğer A kutusunun sağ kenarı, B'nin sol kenarını geçtiyse VE
    // A'nın sol kenarı, B'nin sağ kenarından gerideyse VE
    // A'nın alt kenarı, B'nin üst kenarını geçtiyse VE
    // A'nın üst kenarı, B'nin alt kenarından yukarıdaysa
    // ÇARPIŞMA VARDIR!
    
    return (a.x < b.x + b.width &&
            a.x + a.width > b.x &&
            a.y < b.y + b.height &&
            a.y + a.height > b.y);
}