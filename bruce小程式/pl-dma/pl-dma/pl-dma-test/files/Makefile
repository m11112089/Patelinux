APP = pl-dma-test

# Add any other object files to this list below
APP_OBJS = pl-dma-test.o os_wrap.c AF_AXI_DMA_ctrl.c

all: build

build: $(APP)

$(APP): $(APP_OBJS)
	$(CC) -o $@ $(APP_OBJS) $(LDFLAGS) $(LDLIBS)
clean:
	rm -f $(APP) *.o

