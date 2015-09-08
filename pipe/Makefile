TARGET = pipe pipe_sleep
ALL: $(TARGET)

CFLAGS = -g -Wall

clean:
	$(RM) *.o $(TARGET)

pipe: pipe.o

pipe_sleep.o: pipe.c
	$(CC) -o $@ -c $< -DDO_SLEEP=1

pipe_sleep: pipe_sleep.o
