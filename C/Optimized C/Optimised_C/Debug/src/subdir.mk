################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Optimised_C.c \
../src/bmp_operations.c \
../src/stack.c 

SRC_OBJS += \
./src/Optimised_C.doj \
./src/bmp_operations.doj \
./src/stack.doj 

C_DEPS += \
./src/Optimised_C.d \
./src/bmp_operations.d \
./src/stack.d 


# Each subdirectory must supply rules for building sources it contributes
src/Optimised_C.doj: ../src/Optimised_C.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore SHARC C/C++ Compiler'
	cc21k -c -file-attr ProjectName="Optimised_C" -proc ADSP-21489 -flags-compiler --no_wrap_diagnostics -si-revision 0.2 -O -Ov100 -ipa -g -DCORE0 -DDO_CYCLE_COUNTS -D_DEBUG @includes-088a265c9bddd25b263aa692857d5567.txt -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -swc -gnu-style-dependencies -MD -Mo "src/Optimised_C.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/bmp_operations.doj: ../src/bmp_operations.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore SHARC C/C++ Compiler'
	cc21k -c -file-attr ProjectName="Optimised_C" -proc ADSP-21489 -flags-compiler --no_wrap_diagnostics -si-revision 0.2 -O -Ov100 -ipa -g -DCORE0 -DDO_CYCLE_COUNTS -D_DEBUG @includes-088a265c9bddd25b263aa692857d5567.txt -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -swc -gnu-style-dependencies -MD -Mo "src/bmp_operations.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/stack.doj: ../src/stack.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore SHARC C/C++ Compiler'
	cc21k -c -file-attr ProjectName="Optimised_C" -proc ADSP-21489 -flags-compiler --no_wrap_diagnostics -si-revision 0.2 -O -Ov100 -ipa -g -DCORE0 -DDO_CYCLE_COUNTS -D_DEBUG @includes-088a265c9bddd25b263aa692857d5567.txt -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -swc -gnu-style-dependencies -MD -Mo "src/stack.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


