BUILD_DIR = build
EXEC      = bot

.PHONY: run build clean plot all

run: build
	./$(BUILD_DIR)/$(EXEC)

build:
	@if [ "$$(basename $$PWD)" = "$(BUILD_DIR)" ]; then \
		echo "❌ ERROR: Don't run make inside build/. Run it from project root."; \
		exit 1; \
	fi
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && cmake .. && make

plot:
	python3 plot_graph.py ../output/equity.csv

all: run plot

clean:
	rm -rf $(BUILD_DIR)
	echo "Build directory removed."
