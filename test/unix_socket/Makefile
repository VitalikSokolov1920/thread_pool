ifeq ($(OS), Windows_NT)
	DEL_CMD=del
else
	DEL_CMD=rm -f
endif

all:
	$(CC) server.c -L../../ -lthreadpool -o server
	$(CC) client.c -L../../ -lthreadpool -o client

clean:
	$(DEL_CMD) ./client ./server