# 使用するCコンパイラ
CC = gcc
# コンパイルオプション（例: 警告をすべて表示）
CFLAGS = -Wall

# コマンドAで生成・実行するプログラム名
TARGET_A = runGA
# コマンドAに必要なソースファイル
SRCS_A = fitness.c initialize.c main.c readdata.c

# コマンドBで生成・実行するプログラム名
TARGET_B = runMakeData
# コマンドBに必要なソースファイル
SRCS_B = makedata.c

# --- ルールの記述 ---

# `make A`と入力した時のルール
RunGA: $(TARGET_A)
	@echo "--- プログラムAを実行します ---"
	./$(TARGET_A)

# `make B`と入力した時のルール
MakeData: $(TARGET_B)
	@echo "--- プログラムBを実行します ---"
	./$(TARGET_B)

# プログラムA (run_a) をソースファイルから生成するためのルール
$(TARGET_A): $(SRCS_A)
	$(CC) $(CFLAGS) -o $@ $^ -lm

# プログラムB (run_b) をソースファイルから生成するためのルール
$(TARGET_B): $(SRCS_B)
	$(CC) $(CFLAGS) -o $@ $^

# `make clean` で生成されたファイルを削除するルール
clean:
	rm -f $(TARGET_A) $(TARGET_B)

# ターゲット名がファイル名と重複しても正しく動作させるおまじない
.PHONY: A B clean