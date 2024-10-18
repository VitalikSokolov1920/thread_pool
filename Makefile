TARGET_PREFIX=threadpool
TEST_TARGET_NAME=test.c

ifeq ($(OS), Windows_NT)
	TARGET=$(TARGET_PREFIX).dll
	DEL_CMD=del
	TEST_TAGET=test.exe
	SOURCES=src\thread_pool.o \
			src\thread_task.o \
			src\thread_task_list.o \
			src\list.o
else
	TARGET=lib$(TARGET_PREFIX).so
	DEL_CMD=rm -f
	TEST_TAGET=test
	SOURCES=src/thread_pool.o \
			src/thread_task.o \
			src/thread_task_list.o \
			src/list.o
endif

LDFLAGS=-shared -ggdb -g -O0 -Werror -lpthread
CPPFLAGS=-Iinclude -DDEBUG -ggdb -g -fPIC -O0 -Werror
CC=gcc

.PHONY: all clean clean_all

all: clean_all $(SOURCES)
	$(CC) $(LDFLAGS) $(SOURCES) -o $(TARGET)

test: all
	$(CC) $(TEST_TARGET_NAME) -o $(TEST_TAGET) -L. -l$(TARGET_PREFIX) -Iinclude -lpthread

test_only:
	$(CC) $(TEST_TARGET_NAME) -o $(TEST_TAGET) -L. -l$(TARGET_PREFIX) -Iinclude -lpthread

%.o: %.c
	$(CC) $(CPPFLAGS) -c $< -o $@

distclean: clean
	$(DEL_CMD) $(TARGET) $(TEST_TAGET)

clean:
	$(DEL_CMD) $(SOURCES)
