#makefile
GCC_TOOLCHAIN_DIR := /home/chenying/compiler/riscv-gnu-toolchain/install14
SYSROOT_DIR := $(GCC_TOOLCHAIN_DIR)/riscv64-unknown-elf

LLVM := /home/chenying/compiler/llvm-project/install
SPIKE := /home/chenying/spike
PK := /home/chenying/pk

target = bin/rvv-test-vector

serial:
	${LLVM}/bin/clang++ --target=riscv64-unknown-elf -march=rv64gcv1p0 -menable-experimental-extensions --sysroot=$(SYSROOT_DIR) --gcc-toolchain=$(GCC_TOOLCHAIN_DIR) -O2 -o $(target) src/pathfinder.cpp

vector:
	${LLVM}/bin/clang++ --target=riscv64-unknown-elf -DUSE_RISCV_VECTOR   -march=rv64gcv1p0 -menable-experimental-extensions --sysroot=$(SYSROOT_DIR) --gcc-toolchain=$(GCC_TOOLCHAIN_DIR)  -O2 -o $(target)  src/pathfinder.cpp


runspike :
	$(SPIKE) --isa=rv64gcv $(PK) $(target) 32 32 out
