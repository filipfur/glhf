.PHONY: all
all:
	@cmake -GNinja -DCMAKE_BUILD_TYPE=Release -Wno-dev -B build-tools && \
		cmake --build build-tools && \
		cmake --install build-tools --prefix .

.PHONY: clean
clean:
	@rm -rf build-tools