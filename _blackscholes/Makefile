#makefile
GCC_TOOLCHAIN_DIR := /home/chenying/compiler/riscv-gnu-toolchain/install14
SYSROOT_DIR := $(GCC_TOOLCHAIN_DIR)/riscv64-unknown-elf

LLVM := /home/chenying/compiler/llvm-project/install
SPIKE := /home/chenying/spike
PK := /home/chenying/pk

target = bin/blackscholes_vector.exe

serial:
	${LLVM}/bin/clang++ --target=riscv64-unknown-elf -march=rv64gcv -menable-experimental-extensions --sysroot=$(SYSROOT_DIR) --gcc-toolchain=$(GCC_TOOLCHAIN_DIR) -O2 -o $(target) src/blackscholes.cpp -lm

vector:
	${LLVM}/bin/clang++ --target=riscv64-unknown-elf -DUSE_RISCV_VECTOR   -march=rv64gcv -menable-experimental-extensions --sysroot=$(SYSROOT_DIR) --gcc-toolchain=$(GCC_TOOLCHAIN_DIR)  -O2 -o $(target)  src/blackscholes.cpp -lm

dump:
	${LLVM}/bin/llvm-objdump -d  bin/rvv-test-vector > bin/rvv-test-vector.dump

runspike :
	$(SPIKE) --isa=rv64gcv $(PK) $(target) 1 ./input/in_64K.input ./prices.txt
