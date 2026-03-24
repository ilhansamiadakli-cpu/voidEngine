// engine/input.h dosyasının içi
#ifndef INPUT_H
#define INPUT_H

// Ana Oyun Döngüsü için (Sistemi durdurmaz, tuş yoksa hemen 0 döner)
char Input_GetRawKey(void);

// Menüler için (Kullanıcı bir tuşa basana kadar bekler)
char Input_GetPressed(void);

#endif // INPUT_H