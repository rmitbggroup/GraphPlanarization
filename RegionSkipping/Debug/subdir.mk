################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../graphDFSUtils.c \
../graphDrawPlanar.c \
../graphDrawPlanar_Extensions.c \
../graphEmbed.c \
../graphExtensions.c \
../graphIO.c \
../graphIsolator.c \
../graphK23Search.c \
../graphK23Search_Extensions.c \
../graphK33Search.c \
../graphK33Search_Extensions.c \
../graphK4Search.c \
../graphK4Search_Extensions.c \
../graphNonplanar.c \
../graphOuterplanarObstruction.c \
../graphTests.c \
../graphUtils.c \
../listcoll.c \
../planarity.c \
../planarityCommandLine.c \
../planarityRandomGraphs.c \
../planaritySpecificGraph.c \
../planarityUtils.c \
../planarization.c \
../stack.c 

OBJS += \
./graphDFSUtils.o \
./graphDrawPlanar.o \
./graphDrawPlanar_Extensions.o \
./graphEmbed.o \
./graphExtensions.o \
./graphIO.o \
./graphIsolator.o \
./graphK23Search.o \
./graphK23Search_Extensions.o \
./graphK33Search.o \
./graphK33Search_Extensions.o \
./graphK4Search.o \
./graphK4Search_Extensions.o \
./graphNonplanar.o \
./graphOuterplanarObstruction.o \
./graphTests.o \
./graphUtils.o \
./listcoll.o \
./planarity.o \
./planarityCommandLine.o \
./planarityRandomGraphs.o \
./planaritySpecificGraph.o \
./planarityUtils.o \
./planarization.o \
./stack.o 

C_DEPS += \
./graphDFSUtils.d \
./graphDrawPlanar.d \
./graphDrawPlanar_Extensions.d \
./graphEmbed.d \
./graphExtensions.d \
./graphIO.d \
./graphIsolator.d \
./graphK23Search.d \
./graphK23Search_Extensions.d \
./graphK33Search.d \
./graphK33Search_Extensions.d \
./graphK4Search.d \
./graphK4Search_Extensions.d \
./graphNonplanar.d \
./graphOuterplanarObstruction.d \
./graphTests.d \
./graphUtils.d \
./listcoll.d \
./planarity.d \
./planarityCommandLine.d \
./planarityRandomGraphs.d \
./planaritySpecificGraph.d \
./planarityUtils.d \
./planarization.d \
./stack.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -DDEBUG -std=c99 -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


