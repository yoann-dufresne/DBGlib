# De Bruijn Graph library

## Compilation

```bash
	mkdir -p build && cd build
	cmake .. && make
```

## Tests

```bash
	mkdir -p build && cd build
	cmake -DCMAKE_BUILD_TYPE=DEBUG .. && make && ./tests/dbglib-tests
```