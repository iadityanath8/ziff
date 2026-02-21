CC = cc
OPT = -O3
WARN = -Wall -Werror -Wextra
TARGET = ziff

run:
	$(CC) -o $(TARGET) main.c $(OPT) $(WARN) && ./$(TARGET)

compile:
	$(CC) -o $(TARGET) main.c $(OPT) $(WARN)


clean:
	rm -rf ./$(TARGET)
