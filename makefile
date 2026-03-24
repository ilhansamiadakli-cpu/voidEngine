# VoidCore & VoidEditor Çift Derleme Sistemi (Düzenli Klasör Yapısı)

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2 -g
INCLUDES = -I./Engine

# --- YENİ: Obje Klasörü Tanımı ---
OBJ_DIR = obj

# 1. Kaynak Dosyaları Bul
ENGINE_SRC = $(wildcard Engine/*.c)
GAME_SRC = $(wildcard Game/*.c)
EDITOR_SRC = $(wildcard Editor/*.c)

# 2. Obje Dosyalarının Yeni Adreslerini Belirle (Örn: Engine/core.c -> obj/Engine/core.o)
ENGINE_OBJ = $(patsubst %.c, $(OBJ_DIR)/%.o, $(ENGINE_SRC))
GAME_OBJ = $(patsubst %.c, $(OBJ_DIR)/%.o, $(GAME_SRC))
EDITOR_OBJ = $(patsubst %.c, $(OBJ_DIR)/%.o, $(EDITOR_SRC))

# Çıkacak Uygulamalar
GAME_TARGET = voidcore
EDITOR_TARGET = voideditor

.PHONY: all clean run game editor

# "make" yazıldığında ikisini birden üret
all: $(GAME_TARGET) $(EDITOR_TARGET)

# Oyunu Derle
$(GAME_TARGET): $(ENGINE_OBJ) $(GAME_OBJ)
	$(CC) $(CFLAGS) $^ -o $@

# Editörü Derle
$(EDITOR_TARGET): $(ENGINE_OBJ) $(EDITOR_OBJ)
	$(CC) $(CFLAGS) $^ -o $@

# --- YENİ: Sihrin Gerçekleştiği Kural ---
# C dosyalarını okuyup, obj/ klasörünün içine .o olarak kaydeder
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# --- YENİ: Daha Güvenli Temizlik ---
clean:
	rm -rf $(OBJ_DIR) $(GAME_TARGET) $(EDITOR_TARGET)

# Oyunu çalıştır
game: $(GAME_TARGET)
	./$(GAME_TARGET)

# Editörü çalıştır
editor: $(EDITOR_TARGET)
	./$(EDITOR_TARGET)

# Varsayılan run: Oyunu çalıştır
run: game