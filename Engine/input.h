// engine/input.h dosyasının içi
#ifndef INPUT_H
#define INPUT_H

#ifdef __cplusplus
extern "C" {
#endif

#define KEY_UP    1000
#define KEY_DOWN  1001
#define KEY_LEFT  1002
#define KEY_RIGHT 1003

// Ana Oyun Döngüsü için (Sistemi durdurmaz, tuş yoksa hemen 0 döner)
int Input_GetRawKey(void);

// Menüler için (Kullanıcı bir tuşa basana kadar bekler)
int Input_GetPressed(void);
#ifdef __cplusplus
}
#endif
#endif // INPUT_H