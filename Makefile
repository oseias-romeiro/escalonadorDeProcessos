CC = gcc
RMDIR = rm -rf
RM = rm -f

RUN = src/main.c

DEP_FLAGS = -MT $@ -MMD -MP -MF $(DEP_PATH)/$*.d

DIRECTIVES = -Wall -Wextra -c -I $(HEADER_PATH)

HEADER_PATH = include
SRC_PATH = src
BIN_PATH = bin
DEP_PATH = dep

C_FILES = $(filter-out $(SRC_PATH)/teste15.c $(SRC_PATH)/teste30.c $(SRC_PATH)/teste.c, $(wildcard $(SRC_PATH)/*.c))
OBJ_FILES = $(addprefix $(BIN_PATH)/,$(notdir $(C_FILES:.c=.o)))
DEP_FILES = $(wildcard $(DEP_PATH)/*.d)

EXEC = ./escalona

ifeq ($(OS),Windows_NT)

RMDIR = rd /s /q
RM = del

RUN =

EXEC := $(EXEC).exe

else
UNAME_S := $(shell uname -s)

endif

all: $(EXEC)

$(EXEC): $(OBJ_FILES)
	$(CC) -o $@ $^

$(BIN_PATH)/%.o: $(SRC_PATH)/%.c

ifeq ($(OS), Windows_NT)
	@if not exist $(DEP_PATH) @mkdir $(DEP_PATH)
	@if not exist $(BIN_PATH) @mkdir $(BIN_PATH)
else
	@mkdir -p $(DEP_PATH) $(BIN_PATH)
endif

	$(CC) $(DEP_FLAGS) -c -o $@ $< $(DIRECTIVES)

print-% : ; @echo $* = $($*)

debug: DIRECTIVES += -ggdb -O0 -DDEBUG
debug: all

dev: debug run

gdb: RUN := gdb $(RUN)
gdb: dev

release: DIRECTIVES += -Ofast -mtune=native
release: all

run:
	$(RUN)$(EXEC)

clean:
	$(RMDIR) $(BIN_PATH) $(DEP_PATH)
	$(RM) $(EXEC)

.PRECIOUS: $(DEP_PATH)/%.D

.PHONY: debug clean release run

-include $(DEP_FILES)