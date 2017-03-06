CC = cc
CFLAGS = -std=c99 -Wall -Werror -g -O0

SUBDIRS = 3rd-party

ifdef SUBDIRS
.PHONY : $(SUBDIRS)
$(SUBDIRS) :
	@if [ -d $@ ]; then \
		$(MAKE) --no-print-directory --directory=$@ \
			CC=$(CC) CFLAGS="$(CFLAGS)" $(MAKECMDGOALS); \
	fi
endif

THIRD_PARTY_DIR = 3rd-party
CMOCKA_INCLUDE_DIR = $(THIRD_PARTY_DIR)/build-Debug/include
CMOCKA_LIBRARY = $(THIRD_PARTY_DIR)/build-Debug/lib/libcmocka.a

cmocka : $(SUBDIRS)

CFLAGS += -I $(CMOCKA_INCLUDE_DIR)

LIB = libutils.a

include sources.mk

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(LIB) : $(OBJ_NAMES)
	$(AR) $(ARFLAGS) $@ $^

all : $(LIB) $(EXE)

$(EXE) : $(EXE_OBJ) $(LIB)
	$(CC) $(CFLAGS) -o $@ $^ -lm

test : $(TEST)
	./$(TEST)

$(TEST) : $(TEST_SRC) $(LIB)
	$(CC) $(CFLAGS) -o $@ $^ $(CMOCKA_LIBRARY) -lm

.PHONY : clean
clean : FORCE
	-rm -rf *~ *.o $(LIB) $(EXE) $(TEST) *.dSYM/

.PHONY : clobber
clobber : $(SUBDIRS) clean

FORCE:
