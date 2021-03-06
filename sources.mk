SRCS = ./Project2/Sources/data.c ./Project2/Sources/circbuf.c ./Project2/Sources/memory.c
TEST_SRCS = ./Project2/Sources/mock_memory.c ./Project2/Sources/mock_data.c ./Project2/Sources/mock_circbuf.c ./Project2/Sources/test.c

OBJ_NAMES  = $(SRCS:.c=.o)
TEST_OBJ_NAMES = $(TEST_SRCS:.c=.o)

HEADERS = $(SRCS:.c=.h)
TEST_HEADERS = $(TEST_SRCS:.c=.h)

TEST_SRC = ./Project2/Sources/test.c
TEST_OBJ = $(TEST_SRC:.c=.o)
TEST = test.out

EXE_SRC = ./Project2/Sources/main.c
EXE_OBJ = $(EXE_SRC:.c=.o)
EXE = main
