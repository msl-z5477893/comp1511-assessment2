CC := dcc
FLAGS := --leak-check
TARGET := cs_bookshelf

build:
	$(CC) $(FLAGS) $(TARGET).c -o $(TARGET)

run: $(TARGET)
	./$(TARGET)
