# system libraries to include
LIBS =

# final executable name
EXEC_NAME := stuff

# where "all" target looks for source files
SRC_DIRS := src deps

# directories to pass as -I to the compiler
INC_DIRS := include $(shell find ./deps -maxdepth 1 -type d)

# where "test" target looks for source files
TEST_DIR := test

# file that defines main(), excluded from test executable
MAIN_C := src/main.c

# compiler and linker flags
CFLAGS += -Wall -Wextra -Werror -std=c99 $(addprefix -I,$(INC_DIRS))
LDFLAGS += $(addprefix -l,$(LIBS))

# configurable build profile
DEBUG ?= 1

# configure build profile
ifeq ($(DEBUG), 1)
#	CFLAGS += -g -DDEBUG
	CFLAGS += -g -fsanitize=address -fsanitize=undefined
	LDFLAGS += -fsanitize=address -fsanitize=leak -fsanitize=undefined
else
	CFLAGS += -O3
endif

# where build files live
BUILD_DIR := ./build
EXEC_FILE := $(BUILD_DIR)/$(EXEC_NAME)

# what build files are
SRCS := $(shell find $(SRC_DIRS) -type f -name '*.c' ! -wholename "$(MAIN_C)")
OBJS := $(patsubst %.c,$(BUILD_DIR)/%.o,$(SRCS))
MAIN_O := $(BUILD_DIR)/$(MAIN_C:.c=.o)

# compile targets
$(BUILD_DIR)/%.o: %.c
	@mkdir -p "$(dir $@)"
	@echo "... Compiling $<"
	$(CC) $(CFLAGS) -c $< -o $@

$(EXEC_FILE): $(OBJS) $(MAIN_O)
	@mkdir -p "$(dir $@)"
	@echo "... Linking $(EXEC_FILE)"
	$(CC) $(LDFLAGS) $^ -o $@

# test files
TEST_SRCS := $(shell find $(TEST_DIR) -type f -name '*.c')
TEST_OBJS := $(patsubst %.c,$(BUILD_DIR)/%.o,$(TEST_SRCS))

# test auto-generation
TEST_MAIN_GEN := $(TEST_DIR)/autogen.sh
TEST_MAIN_C := $(BUILD_DIR)/$(TEST_DIR)/tests.c
TEST_MAIN_O := $(TEST_MAIN_C:.c=.o)
TEST_EXEC := $(BUILD_DIR)/tests

# test compilation and linking
$(TEST_EXEC): $(TEST_MAIN_O) $(TEST_OBJS) $(OBJS)
	@echo "... Linking $(TEST_EXEC)"
	$(CC) $(LDFLAGS) $^ -o $@

# tests.c is not in sources, must redefine compile target
$(TEST_MAIN_O): $(TEST_MAIN_C)
	@mkdir -p "$(dir $@)"
	@echo "... Compiling $<"
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_MAIN_C): $(TEST_SRCS)
	@mkdir -p "$(dir $@)"
	@echo "... Generating $@"
	@$(TEST_MAIN_GEN) > "$@"

# top-level targets
.PHONY: all run test check clean

# build executable
all: $(OBJS) $(EXEC_FILE)

# run executable
run: $(EXEC_FILE)
	@$(EXEC_FILE)

# build test executable
test: $(TEST_EXEC)

# run test executable
check: $(TEST_EXEC)
	@$(TEST_EXEC)

# remove build files
clean:
	@rm -rf $(BUILD_DIR)
