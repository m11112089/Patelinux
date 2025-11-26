APP = eth-loopback

# Add any other object files to this list below
APP_OBJS = eth-loopback.o funcs.o

all: build

build: $(APP)

$(APP): $(APP_OBJS)
	$(CC) -o $@ $(APP_OBJS) $(LDFLAGS) $(LDLIBS)
clean:
	rm -f $(APP) *.o

