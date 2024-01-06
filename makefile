CC     := gcc
CFLAGS := -Wall -Oz
DFLAGS := -Wall -g
LIBS   := -lncurses
LD     := gcc

SRC     := src
INCLUDE := include
BUILD   := build
DEPSDIR := $(BUILD)

BINARY := nepTris
DBGBINARY := $(BINARY)_dbg

SOURCESPATHS := $(wildcard $(SRC)/*.c)
OBJECTS      := $(SOURCESPATHS:$(SRC)/%.c=$(BUILD)/%.o)
DEPS         := $(SOURCESPATHS:$(SRC)/%.c=$(DEPSDIR)/%.d)

$(BINARY): $(OBJECTS)
	$(LD) $(CFLAGS) -I$(INCLUDE) $(OBJECTS) -o $(BINARY) $(LIBS) 

install: $(BINARY)
	cp $(BINARY) /usr/bin/

-include $(DEPSDIR)/$(DEPS)
$(BUILD)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -I$(INCLUDE) -c $< -o $@ -MMD

clean:
	$(RM) $(BUILD)/*

fullclean:
	$(MAKE) clean
	$(RM) $(BINARY)

re:
	$(MAKE) clean
	$(MAKE) $(BINARY)

run: $(BINARY)
	./$(BINARY)

rerun:
	$(MAKE) re
	$(MAKE) run
