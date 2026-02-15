# stdlibx Library Makefile
# 
# This Makefile provides convenient targets for building, testing, and managing
# the stdlibx C++ module library using CMake and Ninja.

# Project configuration
PROJECT_NAME := stdlibx
BUILD_DIR := build
CMAKE_GENERATOR := Ninja
CMAKE_BUILD_TYPE := Release

# Build options
BUILD_TESTS ?= OFF
CMAKE_BUILD_FLAGS :=

# Process build options
ifneq ($(BUILD_TESTS),OFF)
	CMAKE_BUILD_FLAGS += -DSTDLIBX_BUILD_TESTS=ON
endif

# Sanitiser configuration (can be overridden with make SANITIZERS="address undefined")
SANITIZERS ?=
ENABLE_SANITIZERS := OFF
CMAKE_SANITIZER_FLAGS :=

# Process sanitiser flags
ifneq ($(SANITIZERS),)
	ENABLE_SANITIZERS := ON
	CMAKE_BUILD_TYPE := Debug
	
	ifeq ($(findstring address,$(SANITIZERS)),address)
		CMAKE_SANITIZER_FLAGS += -DUSE_SANITIZER_ADDRESS=ON -DUSE_SANITIZER_LEAK=ON
	endif
	
	ifeq ($(findstring undefined,$(SANITIZERS)),undefined)
		CMAKE_SANITIZER_FLAGS += -DUSE_SANITIZER_UNDEFINED=ON
	endif
	
	ifeq ($(findstring thread,$(SANITIZERS)),thread)
		CMAKE_SANITIZER_FLAGS += -DUSE_SANITIZER_THREAD=ON
	endif
	
	ifeq ($(findstring memory,$(SANITIZERS)),memory)
		CMAKE_SANITIZER_FLAGS += -DUSE_SANITIZER_MEMORY=ON
	endif
	
	ifeq ($(SANITIZERS),all)
		CMAKE_SANITIZER_FLAGS := -DUSE_SANITIZER_ADDRESS=ON -DUSE_SANITIZER_UNDEFINED=ON -DUSE_SANITIZER_LEAK=ON
	endif
endif

# Colors for output
BOLD := \033[1m
RED := \033[31m
GREEN := \033[32m
YELLOW := \033[33m
BLUE := \033[34m
MAGENTA := \033[35m
CYAN := \033[36m
RESET := \033[0m

# Check if required tools are available
REQUIRED_TOOLS := cmake ninja
$(foreach tool,$(REQUIRED_TOOLS),\
	$(if $(shell command -v $(tool) 2> /dev/null),,\
		$(error $(tool) is required but not found in PATH)))

# Default target
.PHONY: all
all: build

# Help target - shows available commands
.PHONY: help
help:
	@printf "$(BOLD)$(CYAN)stdlibx Library Makefile$(RESET)\n"
	@printf "\n"
	@printf "$(BOLD)Build Targets:$(RESET)\n"
	@printf "  $(YELLOW)build$(RESET)         - Build the library (default)\n"
	@printf "  $(YELLOW)test-build$(RESET)    - Build library with tests\n"
	@printf "  $(YELLOW)clean$(RESET)         - Clean build directory\n"
	@printf "  $(YELLOW)rebuild$(RESET)       - Clean and build\n"
	@printf "  $(YELLOW)configure$(RESET)     - Configure CMake build system\n"
	@printf "\n"
	@printf "$(BOLD)Test Targets:$(RESET)\n"
	@printf "  $(YELLOW)test$(RESET)          - Build and run tests\n"
	@printf "\n"
	@printf "$(BOLD)Build Options:$(RESET)\n"
	@printf "  $(YELLOW)BUILD_TESTS$(RESET)   - Enable test building (OFF/ON)\n"
	@printf "    Example: make build BUILD_TESTS=ON\n"
	@printf "\n"
	@printf "$(BOLD)Sanitiser Options:$(RESET)\n"
	@printf "  $(YELLOW)SANITIZERS$(RESET)    - Enable sanitisers (builds in Debug mode)\n"
	@printf "    Values: address, undefined, thread, memory, all\n"
	@printf "    Example: make build SANITIZERS=\"address undefined\"\n"
	@printf "    Example: make test SANITIZERS=all\n"
	@printf "\n"
	@printf "$(BOLD)Utility Targets:$(RESET)\n"
	@printf "  $(YELLOW)format$(RESET)        - Format code using clang-format\n"
	@printf "  $(YELLOW)deps$(RESET)          - Check dependencies\n"
	@printf "  $(YELLOW)info$(RESET)          - Show project information\n"
	@printf "  $(YELLOW)help$(RESET)          - Show this help message\n"

# Configure CMake build system
.PHONY: configure
configure:
	@START_TIME=$$(date +%s); \
	printf "$(BOLD)$(BLUE)Configuring CMake build system...$(RESET)\n"; \
	if [ "$(ENABLE_SANITIZERS)" = "ON" ]; then \
		printf "$(BOLD)$(MAGENTA)Sanitisers enabled:$(RESET) $(SANITIZERS)\n"; \
		printf "$(YELLOW)Building in Debug mode for sanitiser support$(RESET)\n"; \
	fi; \
	if [ "$(BUILD_TESTS)" = "ON" ]; then \
		printf "$(BOLD)$(CYAN)Tests enabled$(RESET)\n"; \
	fi; \
	cmake -S . -B $(BUILD_DIR) -G $(CMAKE_GENERATOR) \
		-DCMAKE_BUILD_TYPE=$(CMAKE_BUILD_TYPE) \
		-DENABLE_SANITIZERS=$(ENABLE_SANITIZERS) \
		$(CMAKE_BUILD_FLAGS) \
		$(CMAKE_SANITIZER_FLAGS); \
	END_TIME=$$(date +%s); ELAPSED=$$(($$END_TIME - $$START_TIME)); \
	printf "$(GREEN)✓ Configuration complete in $${ELAPSED}s$(RESET)\n"

# Build the library
.PHONY: build
build: configure
	@START_TIME=$$(date +%s); \
	printf "$(BOLD)$(BLUE)Building $(PROJECT_NAME) library...$(RESET)\n"; \
	cmake --build $(BUILD_DIR); \
	END_TIME=$$(date +%s); ELAPSED=$$(($$END_TIME - $$START_TIME)); \
	if [ $$ELAPSED -ge 60 ]; then \
		MINUTES=$$(($$ELAPSED / 60)); SECONDS=$$(($$ELAPSED % 60)); \
		printf "$(GREEN)✓ Build complete in $${MINUTES}m $${SECONDS}s$(RESET)\n"; \
	else \
		printf "$(GREEN)✓ Build complete in $${ELAPSED}s$(RESET)\n"; \
	fi

# Build with tests enabled
.PHONY: test-build
test-build:
	@$(MAKE) build BUILD_TESTS=ON

# Clean build directory
.PHONY: clean
clean:
	@START_TIME=$$(date +%s); \
	printf "$(BOLD)$(YELLOW)Cleaning build directory...$(RESET)\n"; \
	if [ -d "$(BUILD_DIR)" ]; then \
		rm -rf $(BUILD_DIR); \
		END_TIME=$$(date +%s); ELAPSED=$$(($$END_TIME - $$START_TIME)); \
		printf "$(GREEN)✓ Build directory cleaned in $${ELAPSED}s$(RESET)\n"; \
	else \
		printf "$(YELLOW)Build directory doesn't exist$(RESET)\n"; \
	fi

# Clean and rebuild
.PHONY: rebuild
rebuild:
	@START_TIME=$$(date +%s); \
	$(MAKE) clean; \
	$(MAKE) build; \
	END_TIME=$$(date +%s); ELAPSED=$$(($$END_TIME - $$START_TIME)); \
	if [ $$ELAPSED -ge 60 ]; then \
		MINUTES=$$(($$ELAPSED / 60)); SECONDS=$$(($$ELAPSED % 60)); \
		printf "$(GREEN)✓ Rebuild complete in $${MINUTES}m $${SECONDS}s$(RESET)\n"; \
	else \
		printf "$(GREEN)✓ Rebuild complete in $${ELAPSED}s$(RESET)\n"; \
	fi

# Build and run tests
.PHONY: test
test:
	@$(MAKE) build BUILD_TESTS=ON
	@START_TIME=$$(date +%s); \
	printf "$(BOLD)$(BLUE)Running tests...$(RESET)\n"; \
	cd $(BUILD_DIR) && ctest --output-on-failure; \
	END_TIME=$$(date +%s); ELAPSED=$$(($$END_TIME - $$START_TIME)); \
	printf "$(GREEN)✓ Tests complete in $${ELAPSED}s$(RESET)\n"

# Format code
.PHONY: format
format:
	@printf "$(BOLD)$(BLUE)Formatting code...$(RESET)\n"
	@if [ -f ".clang-format" ]; then \
		find src include -name "*.cpp" -o -name "*.cppm" -o -name "*.hpp" 2>/dev/null | xargs clang-format -i; \
		printf "$(GREEN)✓ Code formatted$(RESET)\n"; \
	else \
		printf "$(YELLOW).clang-format not found, skipping formatting$(RESET)\n"; \
	fi

# Check dependencies
.PHONY: deps
deps:
	@printf "$(BOLD)$(BLUE)Checking dependencies...$(RESET)\n"
	@printf "$(CYAN)Required tools:$(RESET)\n"
	@for tool in $(REQUIRED_TOOLS); do \
		if command -v $$tool > /dev/null 2>&1; then \
			printf "  $(GREEN)✓$(RESET) $$tool\n"; \
		else \
			printf "  $(RED)✗$(RESET) $$tool (missing)\n"; \
		fi; \
	done
	@printf "\n"
	@printf "$(CYAN)Optional tools:$(RESET)\n"
	@for tool in clang-format clang-tidy; do \
		if command -v $$tool > /dev/null 2>&1; then \
			printf "  $(GREEN)✓$(RESET) $$tool\n"; \
		else \
			printf "  $(YELLOW)○$(RESET) $$tool (optional)\n"; \
		fi; \
	done

# Show project information
.PHONY: info
info:
	@printf "$(BOLD)$(CYAN)Project Information$(RESET)\n"
	@printf "\n"
	@printf "$(BOLD)Project:$(RESET)      $(PROJECT_NAME) (C++ Module Library)\n"
	@printf "$(BOLD)Build Dir:$(RESET)    $(BUILD_DIR)\n"
	@printf "$(BOLD)Generator:$(RESET)    $(CMAKE_GENERATOR)\n"
	@printf "$(BOLD)Build Type:$(RESET)   $(CMAKE_BUILD_TYPE)\n"
	@printf "$(BOLD)Tests:$(RESET)        $(BUILD_TESTS)\n"
	@printf "$(BOLD)Sanitisers:$(RESET)   $(if $(SANITIZERS),$(SANITIZERS),None)\n"
	@printf "\n"
	@printf "$(BOLD)Status:$(RESET)\n"
	@if [ -d "$(BUILD_DIR)" ]; then \
		printf "  $(GREEN)✓$(RESET) Build directory exists\n"; \
	else \
		printf "  $(YELLOW)○$(RESET) Build directory needs creation\n"; \
	fi
	@if [ -f "$(BUILD_DIR)/CMakeCache.txt" ]; then \
		printf "  $(GREEN)✓$(RESET) CMake configured\n"; \
	else \
		printf "  $(YELLOW)○$(RESET) CMake needs configuration\n"; \
	fi

# Prevent make from treating file names as targets
.PHONY: $(BUILD_DIR) src include
