# **CAN bus**

<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->

- [Commands](#commands)
- [`VCAN`](#vcan)
- [`tshark` command](#tshark-command)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

## Commands

- Check the `PCAN` drivers

```bash
mikexu@mikexu$ grep PEAK_ /boot/config-`uname -r`
CONFIG_CAN_PEAK_PCIEFD=m
CONFIG_CAN_PEAK_PCI=m
CONFIG_CAN_PEAK_PCIEC=y
CONFIG_CAN_PEAK_PCMCIA=m
CONFIG_CAN_PEAK_USB=m
```

- Check if an USB-based `PCAN` device initialized.

```bash
lsmod | grep ^peak
```

- List all the socket can device

```bash
mikexu@mikexu-VivoBook-ASUSLaptop-X513EA-K513EA:/sys/class/net$ zs
 can0      docker_gwbridge   lo            wlo1
 docker0   enx7cc2c63eef00   veth61443a2
```

- Turn on the can network

```bash
sudo ip link set can0 up
```

- Setup the loopback mode, bitrate, `fd` mode
  - [ ] loop back mode
  - [ ] `fd` mode
  - [x] baud-rate

```bash
root@mikexu# ip link set can0type can bitrate 1000000
root@mikexu# ip link set can0type can bitrate 1000000 loopback on
RTNETLINK answers: Operation not supported
root@mikexu# ip link set can0type can bitrate 1000000 fd on
RTNETLINK answers: Operation not supported
```

## `VCAN`

- Setup the virtual CAN network device

```bash
sudo modprobe vcan # Add vcan kernel module
sudo ip link add dev vcan0 type vcan
sudo ip link set vcan0 up
````

## `tshark` command

- The socket can network can be monitored by `tshark`

```bash
tshark -i vcan0
```
