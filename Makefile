TARGET_EXEC := tinyrenderer

BUILD_DIR := ./bin
SRC_DIRS := ./src
ASSETS_DIR := ./assets
INCLUDE_DIR := ./includes

SRCS := src/main.c

OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

CFLAGS  := -Wall -O3 -g
LDFLAGS := -lraylib -framework IOKit -framework Cocoa -framework OpenGL

# The final build step.
$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# Build step for C source
$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)
