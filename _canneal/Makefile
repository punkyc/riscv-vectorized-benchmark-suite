#makefile
GCC_TOOLCHAIN_DIR := /home/chenying/compiler/riscv-gnu-toolchain/install14
SYSROOT_DIR := $(GCC_TOOLCHAIN_DIR)/riscv64-unknown-elf

LLVM := /home/chenying/compiler/llvm-project/install
SPIKE := /home/chenying/spike
PK := /home/chenying/pk

target = bin/rvv-test-serial

#start:
#	mkdir bin ; \
#	rm bin/*.*;

serial:
	for entry in src/*.cpp ; do \
		${LLVM}/bin/clang++  -Wall --target=riscv64-unknown-elf  -march=rv64gcv -menable-experimental-extensions --sysroot=$(SYSROOT_DIR) --gcc-toolchain=$(GCC_TOOLCHAIN_DIR)  -O2 -c -o  $$entry.o  $$entry ; \
	done
	${LLVM}/bin/clang++ -Wall --target=riscv64-unknown-elf  -march=rv64gcv -menable-experimental-extensions --sysroot=$(SYSROOT_DIR) --gcc-toolchain=$(GCC_TOOLCHAIN_DIR) -O2  -o $(target) src/*.cpp.o -lm

vector:
	${LLVM}/bin/clang++ -Wall --target=riscv64-unknown-elf -DUSE_RISCV_VECTOR  -march=rv64gcv -menable-experimental-extensions --sysroot=$(SYSROOT_DIR) --gcc-toolchain=$(GCC_TOOLCHAIN_DIR) -O2 -c -o src/annealer_thread.o src/annealer_thread.cpp
	${LLVM}/bin/clang++ -Wall --target=riscv64-unknown-elf -DUSE_RISCV_VECTOR -march=rv64gcv -menable-experimental-extensions --sysroot=$(SYSROOT_DIR) --gcc-toolchain=$(GCC_TOOLCHAIN_DIR) -O2 -c -o src/main.o src/main.cpp
	${LLVM}/bin/clang++ -Wall --target=riscv64-unknown-elf -DUSE_RISCV_VECTOR -march=rv64gcv -menable-experimental-extensions --sysroot=$(SYSROOT_DIR) --gcc-toolchain=$(GCC_TOOLCHAIN_DIR) -O2 -c -o src/netlist.o src/netlist.cpp
	${LLVM}/bin/clang++ -Wall --target=riscv64-unknown-elf -DUSE_RISCV_VECTOR -march=rv64gcv -menable-experimental-extensions --sysroot=$(SYSROOT_DIR) --gcc-toolchain=$(GCC_TOOLCHAIN_DIR) -O2 -c -o src/netlist_elem.o src/netlist_elem.cpp
	${LLVM}/bin/clang++ -Wall --target=riscv64-unknown-elf -DUSE_RISCV_VECTOR -march=rv64gcv -menable-experimental-extensions --sysroot=$(SYSROOT_DIR) --gcc-toolchain=$(GCC_TOOLCHAIN_DIR) -O2 -c -o src/rng.o src/rng.cpp
	${LLVM}/bin/clang++ -Wall --target=riscv64-unknown-elf -DUSE_RISCV_VECTOR -march=rv64gcv -menable-experimental-extensions --sysroot=$(SYSROOT_DIR) --gcc-toolchain=$(GCC_TOOLCHAIN_DIR) -O2  -o $(target) src/*.o -lm
	

runspike :
	$(SPIKE) --isa=rv64gcv $(PK) $(target) 1 100 300 input/100.nets 8


clean:
	rm src/*.o
