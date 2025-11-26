# **CAN BUS**

<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->

- [`RSI3000E`](#rsi3000e)
  - [Trace the can driver messages](#trace-the-can-driver-messages)
  - [`SocketCAN` interface for `RSI3000E`](#socketcan-interface-for-rsi3000e)
    - [List all available CAN devices](#list-all-available-can-devices)
    - [Check `ip` status](#check-ip-status)
    - [Bringing the `SocketCAN` interface up](#bringing-the-socketcan-interface-up)
    - [Check the CAN network details](#check-the-can-network-details)
    - [Setup the FD mode](#setup-the-fd-mode)
    - [Setup loopback mode](#setup-loopback-mode)
    - [Set `bitrate`](#set-bitrate)
    - [Summary of the `RSI3000E` CAN bus device](#summary-of-the-rsi3000e-can-bus-device)
- [`AM625`](#am625)
  - [Overlay the device tree](#overlay-the-device-tree)
  - [Trace the M_CAN driver messages](#trace-the-m_can-driver-messages)
  - [`SocketCAN` interface for `AM625`](#socketcan-interface-for-am625)
- [Testing between the Ubuntu and `RSI3000E`](#testing-between-the-ubuntu-and-rsi3000e)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

## `RSI3000E`

### Trace the can driver messages

```bash
root@pc100:~# dmesg | grep can
[    2.431149] can: controller area network core
[    2.431193] can: raw protocol
[    2.431203] can: broadcast manager protocol
[    2.431214] can: netlink gateway - max_hops=1
[    4.117483] xilinx_can ff060000.can can0: TDC Offset value not in range
```

### `SocketCAN` interface for `RSI3000E`

#### CAN network status when boot up

- Default CAN network is **down** when boot up

```bash
root@pc100:~# ip link show can0
3: can0: <NOARP,ECHO> mtu 16 qdisc noop state DOWN mode DEFAULT group default qlen 10
    link/can
```

- Set the bitrate would not change the CAN network status

```bash
root@pc100:~# ip link set can0 type can bitrate 125000
[  890.676036] xilinx_can ff060000.can can0: bitrate error 0.0%
root@pc100:~# ip link show can0
3: can0: <NOARP,ECHO> mtu 16 qdisc noop state DOWN mode DEFAULT group default qlen 10
    link/can
```

#### Commands

##### List all available CAN devices

```bash
root@pc100:~# ls /sys/class/net
can0     docker0  eth0     lo       sit0
```

##### Check `ip` status

```bash
root@pc100:~# ifconfig can0
can0      Link encap:UNSPEC  HWaddr 00-00-00-00-00-00-00-00-00-00-00-00-00-00-00-00
          NOARP  MTU:16  Metric:1
          RX packets:0 errors:0 dropped:0 overruns:0 frame:0
          TX packets:0 errors:0 dropped:0 overruns:0 carrier:0
          collisions:0 txqueuelen:10
          RX bytes:0 (0.0 B)  TX bytes:0 (0.0 B)
          Interrupt:19
```

##### Bringing the `SocketCAN` interface up

```bash
ip link set can0 up
```

##### Check the CAN network details

```bash
root@pc100:~# ip -details link show can0
3: can0: <NOARP,UP,LOWER_UP,ECHO> mtu 16 qdisc pfifo_fast state UP mode DEFAULT group default qlen 10
    link/can  promiscuity 0 minmtu 0 maxmtu 0
    can state ERROR-ACTIVE (berr-counter tx 0 rx 0) restart-ms 0
          bitrate 999999 sample-point 0.750
          tq 250 prop-seg 1 phase-seg1 1 phase-seg2 1 sjw 1
          xilinx_can: tseg1 1..16 tseg2 1..8 sjw 1..4 brp 1..256 brp-inc 1
          clock 99999999 numtxqueues 1 numrxqueues 1 gso_max_size 65536 gso_max_segs 65535 parentbus platform parentdev ff060000.can
```

##### Setup the FD mode

```bash
root@pc100:~# ip link set can0 up type can fd on
RTNETLINK answers: Operation not supported
```

##### Setup loopback mode

```bash
root@pc100:~# ip link set can0 type can loopback on
RTNETLINK answers: Device or resource busy
```

##### Set `bitrate`

- Syntax:

```bash
sudo ip link set canX type can bitrate <arbitration_bitrate>
```

- Test:

```bash
root@pc100:~# ip link set can0 type can bitrate 1000000 dbitrate 2000000
RTNETLINK answers: Operation not supported
root@pc100:~# ip link set can0 type can bitrate 12500
[  247.195937] xilinx_can ff060000.can can0: bitrate error 34359738.3% too high
RTNETLINK answers: Numerical argument out of domain
root@pc100:~# ip link set can0 type can bitrate 1000000
[  271.407989] xilinx_can ff060000.can can0: bitrate error 0.0%
```

##### Summary of the `RSI3000E` CAN bus device

- [ ] loop back mode
- [ ] `fd` mode
- [x] baud-rate

## `AM625`

### Overlay the device tree

```bash
=> setenv name_overlays ti/k3-am62x-sk-mcan.dtbo
```

### Trace the M_CAN driver messages

```bash
root@am62xxsip-evm:~# dmesg | grep m_can
[    8.377550] m_can_platform 4e08000.can: m_can device registered (irq=0, version=32)
[    8.397300] m_can_platform 4e18000.can: m_can device registered (irq=0, version=32)
[    8.411695] m_can_platform 20701000.can: m_can device registered (irq=421, version=32)
[    8.804922] m_can_platform 4e18000.can mcu_mcan1: renamed from can1
[    8.825474] m_can_platform 20701000.can main_mcan0: renamed from can2
[    8.866484] m_can_platform 4e08000.can mcu_mcan0: renamed from can0
```

### `SocketCAN` interface for `AM625`

- List all available CAN devices

```bash
root@am62xxsip-evm:~# ls /sys/class/net
eth0  eth1  lo  main_mcan0  mcu_mcan0  mcu_mcan1
```

- Check `ip` status

```bash
root@am62xxsip-evm:~# ifconfig main_mcan0
main_mcan0: flags=128<NOARP>  mtu 16
        unspec 00-00-00-00-00-00-00-00-00-00-00-00-00-00-00-00  txqueuelen 10  (UNSPEC)
        RX packets 0  bytes 0 (0.0 B)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 0  bytes 0 (0.0 B)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0
        device interrupt 165
```

- Setup loopback mode

```bash
ip link set main_mcan0 type can bitrate 1000000 dbitrate 2000000 fd on loopback on
```

- Set one session to listen

```bash
root@am62xxsip-evm:~# candump main_mcan0
  main_mcan0  123   [8]  11 22 33 44 55 66 77 88
  main_mcan0  123   [8]  11 22 33 44 55 66 77 88
```

- Send the CAN message

```bash
root@am62xxsip-evm:~# cansend main_mcan0 123#1122334455667788
```

## Testing between the Ubuntu and `RSI3000E`

```bash
root@mikexu-VivoBook-ASUSLaptop-X513EA-K513EA:/sys/class/net# candump can0
  can0  123   [8]  11 22 33 44 55 66 77 88
```

```bash
root@pc100:~# candump can0
  can0  123   [8]  11 22 33 44 55 66 77 88
```
