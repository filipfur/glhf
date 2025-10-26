.PHONY: glhf
glhf:
	@cmake -GNinja -DCMAKE_BUILD_TYPE=Release -B build-glhf && \
		cmake --build build-glhf --target glhf

.PHONY: examples
examples: glhf
	@cmake --build build-glhf --target hello_triangle --target hello_cube --target hello_dancer && \
		cmake --install build-glhf --component examples --prefix .

.PHONY: tests
tests: glhf
	@cmake --build build-glhf --target glhf_test
	@cd build-glhf/tests && ctest --output-on-failure

.PHONY: tools
tools:
	@$(MAKE) -C tools all

.PHONY: wasm
wasm: tools
	@emcmake cmake -GNinja -DCMAKE_BUILD_TYPE=Release -B build-wasm && \
		cmake --build build-wasm

.PHONY: clean
clean:
	@rm -rf build build-glhf build-wasm
	@$(MAKE) -C tools clean