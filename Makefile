CC=g++

MAGMA_DIR=third-party/magma
INCLUDE_DIR=-I$(VULKAN_SDK)/include
LIBRARY_DIR=-L$(VULKAN_SDK)/lib -L$(MAGMA_DIR)
PLATFORM=VK_USE_PLATFORM_XCB_KHR

BASE_CFLAGS=-std=c++14 -m64 -msse4 -MD -D$(PLATFORM) $(INCLUDE_DIR)
DEBUG ?= 1
ifeq ($(DEBUG), 1)
	CFLAGS=$(BASE_CFLAGS) -O0 -g -D_DEBUG
	MAGMA_LIB=magmad
else
	CFLAGS=$(BASE_CFLAGS) -O3 -DNDEBUG
	MAGMA_LIB=magma
endif
LDFLAGS=$(LIBRARY_DIR) -l$(MAGMA_LIB) -lvulkan -lxcb -lpthread

DEPS := $(OBJS:.o=.d)

-include $(DEPS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

magma:
	$(MAKE) -C $(MAGMA_DIR) magma

gpucaps: gpucaps.o
	$(CC) -o $@ $^ $(LDFLAGS)

clean:
	$(MAKE) -C $(MAGMA_DIR) clean
	@find . -name '*.o' -delete
	@rm -rf $(DEPS) gpucaps
