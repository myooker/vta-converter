BUILD_DIR := build

.PHONY: all build install clean

all: build

build:
	@echo "Configuring project..."
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake ..

make_build:
	@echo "Building..."
	@$(MAKE) -C $(BUILD_DIR)

install: build make_build
	@echo "Installing..."
	@$(MAKE) -C $(BUILD_DIR) install

clean:
	@echo "Cleaning..."
	@rm -rf $(BUILD_DIR)
