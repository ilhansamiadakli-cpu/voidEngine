# VoidCore & VoidEditor Çift Derleme Sistemi

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2 -g
INCLUDES = -I./Engine

# Motor Dosyaları (Her iki program da bunu kullanır)
ENGINE_SRC = $(wildcard Engine/*.c)
ENGINE_OBJ = $(ENGINE_SRC:.c=.o)

# Oyun Dosyaları
GAME_SRC = $(wildcard Game/*.c)
GAME_OBJ = $(GAME_SRC:.c=.o)

# Editör Dosyaları
EDITOR_SRC = $(wildcard Editor/*.c)
EDITOR_OBJ = $(EDITOR_SRC:.c=.o)

# Çıkacak Uygulamalar
GAME_TARGET = voidcore
EDITOR_TARGET = voideditor

.PHONY: all clean run game editor

# "make" yazıldığında ikisini birden üret!
all: $(GAME_TARGET) $(EDITOR_TARGET)

# Oyunu Derle
$(GAME_TARGET): $(ENGINE_OBJ) $(GAME_OBJ)
	$(CC) $(CFLAGS) $^ -o $@

# Editörü Derle
$(EDITOR_TARGET): $(ENGINE_OBJ) $(EDITOR_OBJ)
	$(CC) $(CFLAGS) $^ -o $@

# .c dosyalarını .o dosyalarına çevirme kuralı
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Temizlik (o .o dosyalarını silmek için)
clean:
	rm -f $(GAME_TARGET) $(EDITOR_TARGET) $(ENGINE_OBJ) $(GAME_OBJ) $(EDITOR_OBJ)

# Oyunu çalıştır
game: $(GAME_TARGET)
	./$(GAME_TARGET)

# Editörü çalıştır
editor: $(EDITOR_TARGET)
	./$(EDITOR_TARGET)

# Varsayılan run: Oyunu çalıştır
run: game