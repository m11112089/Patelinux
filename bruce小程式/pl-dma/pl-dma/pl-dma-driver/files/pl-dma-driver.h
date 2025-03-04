// --------------------- IOCTL commands ------------------------ //
#define IOC_MAGIC 'r'
#define IOCTL_LEDS_ON            _IOWR (IOC_MAGIC, 0, int)
#define IOCTL_LEDS_OFF           _IOWR (IOC_MAGIC, 1, int)
