$(CC) := dcc
$(TARGET) := cs_bookshelf

build:
	$(CC) $(TARGET).c -o $(TARGET)

run: build
	./$(TARGET)
