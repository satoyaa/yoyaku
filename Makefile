CC = gcc
CFLAGS = -Wall -g
TARGET = hello

# コンパイル対象のソースファイルのみを記述
SRCS = hello.c goodbye.c
# ソースファイルからオブジェクトファイル名リストを自動生成
OBJS = $(SRCS:.c=.o)

# デフォルトのターゲット (makeとだけ打った時に実行される)
all: $(TARGET)

# 実行ファイル(TARGET)は、オブジェクトファイル(OBJS)に依存する
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# 全てのオブジェクトファイルは、extern.hに依存する
$(OBJS): extern.h

# cleanルールを追加しておくと便利
clean:
	rm -f $(TARGET) $(OBJS)

.PHONY: all clean