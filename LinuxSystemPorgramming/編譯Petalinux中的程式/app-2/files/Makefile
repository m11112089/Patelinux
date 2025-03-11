TEST = test
TEST2 = test2
# Add any other object files to this list below
TEST_OBJS = test.o
TEST2_OBJS = test2.o
all: build

build: $(TEST) $(TEST2)

$(TEST): $(TEST_OBJS)
	$(CC) -o $@ $(TEST_OBJS) $(LDFLAGS) $(LDLIBS)
$(TEST2): $(TEST2_OBJS)
	$(CC) -o $@ $(TEST2_OBJS) $(LDFLAGS) $(LDLIBS)

clean:
	rm -f $(TEST) $(TEST2) *.o


