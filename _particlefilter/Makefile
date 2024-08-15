#makefile
GCC_TOOLCHAIN_DIR := /home/chenying/compiler/riscv-gnu-toolchain/install14
SYSROOT_DIR := $(GCC_TOOLCHAIN_DIR)/riscv64-unknown-elf

LLVM := /home/chenying/compiler/llvm-project/install
SPIKE := /home/chenying/spike
PK := /home/chenying/pk

target = bin/rvv-test

start:
	mkdir bin; \
	rm bin/*.*

serial:
	${LLVM}/bin/clang -Wall --target=riscv64-unknown-elf   -march=rv64gcv1p0 -menable-experimental-extensions --sysroot=$(SYSROOT_DIR) --gcc-toolchain=$(GCC_TOOLCHAIN_DIR) -O2 -c -o src/particlefilter_serial.o src/particlefilter.c
	${LLVM}/bin/clang -Wall --target=riscv64-unknown-elf  -march=rv64gcv1p0 -menable-experimental-extensions --sysroot=$(SYSROOT_DIR) --gcc-toolchain=$(GCC_TOOLCHAIN_DIR) -O2 -o $(target) src/particlefilter_serial.o -lm
	

vector:
	${LLVM}/bin/clang -Wall --target=riscv64-unknown-elf -DUSE_RISCV_VECTOR -march=rv64gcv1p0 -menable-experimental-extensions --sysroot=$(SYSROOT_DIR) --gcc-toolchain=$(GCC_TOOLCHAIN_DIR)  -O2 -c -o src/particlefilter.o src/particlefilter.c
	${LLVM}/bin/clang -Wall --target=riscv64-unknown-elf -DUSE_RISCV_VECTOR -march=rv64gcv1p0 -menable-experimental-extensions --sysroot=$(SYSROOT_DIR) --gcc-toolchain=$(GCC_TOOLCHAIN_DIR)  -O2 -o $(target) src/particlefilter.o       -lm


runspike :
	$(SPIKE) --isa=rv64gcv $(PK) $(target) -x 128 -y 128 -z 2 -np 256


clean:
	rm src/*.o


	
