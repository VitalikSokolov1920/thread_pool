TARGET_PREFIX=threadpool

ifeq ($(OS), Windows_NT)
	TARGET=$(TARGET_PREFIX).dll
	DEL_CMD=del
	SOURCES=src\thread_pool.o \
			src\thread_task.o \
			src\thread_task_list.o \
			src\containers\list.o
else
	TARGET=lib$(TARGET_PREFIX).so
	DEL_CMD=rm -f
	SOURCES=src/thread_pool.o \
			src/thread_task.o \
			src/thread_task_list.o \
			src/containers/list.o
endif

LDFLAGS=-shared -ggdb -g -O0 -Werror -lpthread
CPPFLAGS=-Iinclude -DDEBUG -ggdb -g -fPIC -O0 -Werror

ifneq (,$(shell which clang))
CC=clang
else ifneq (,$(shell which gcc))
CC=gcc
else
$(error "C compiler not found")
endif

.PHONY: all clean clean_all

all: clean_all $(SOURCES)
	$(CC) $(LDFLAGS) $(SOURCES) -o $(TARGET)

test: all
	make -C ./test/unix_socket

test_only:
	make -C ./test/unix_socket

%.o: %.c
	$(CC) $(CPPFLAGS) -c $< -o $@

distclean: clean
	$(DEL_CMD) $(TARGET) $(TEST_TAGET)
	$(DEL_CMD) client
	make -C ./test/unix_socket clean

clean:
	$(DEL_CMD) $(SOURCES)
