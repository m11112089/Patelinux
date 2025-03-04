#define MAP_SIZE 0x10000

#ifdef __linux__
void *prepare_memory(void * base_addr);
void detroy_memory(void *base_addr);
int get_devpath(void *match, char *store, int mode);

inline uint32_t bus_rd32(void *BaseAddress)
{
    return *((volatile uint32_t *)(BaseAddress));
}

inline void bus_wr32(void *BaseAddress, uint32_t value)
{
    *((volatile uint32_t *)(BaseAddress)) = value;
}
#else
#define prepare_memory(x) (x)
#define detroy_memory(x)
#endif
