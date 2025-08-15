SRC_DIR = src
HDR_DIR = include
BUILD_DIR = build

CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -I$(HDR_DIR)
LIB = -lpthread

# Find all .c files in SRC_DIR
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
# Convert .c file paths in SRC_DIR to .o file paths in BUILD_DIR
OBJ = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_FILES))

NAME = cfp

$(NAME): $(OBJ)
	$(CC) -o $@ $(OBJ) $(CFLAGS) $(LIB)

# Rule to compile .c in src/ to .o in build/
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(NAME) $(BUILD_DIR)
