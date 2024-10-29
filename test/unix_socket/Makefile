ifeq ($(OS), Windows_NT)
	DEL_CMD=del
else
	DEL_CMD=rm -f
endif

all:
	$(CC) server.c -L../../ -lpthread -lthreadpool -o server -g
	$(CC) client.c -L../../ -lpthread -lthreadpool -o client -g

clean:
	$(DEL_CMD) ./client ./server