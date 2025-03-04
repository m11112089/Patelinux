<font color=#008000>Linux 自帶的工具</font>

<font color=#FF6600>Yocto開源第三方程式</font>

<font color=#0000FF>Bruce的小程式 </font>


如何使用Patelinux編譯程式？ [Tutorial](link)
- Yocto
為什麼有bb檔，但是沒有在rootfs裡面？
# DISK
影響儲存檔案速度的因素:

    - 檔案系統之間的差別(差了20%以上，超誇張)
        - fdisk
        - xfs
        - dd
        - df
    - FSYNC/OSYNC

Sync-資料流中的檔案同步問題
要用Ram Disk嗎？


# Memory
file:///home/kai/tmp/tmp2.c

與R5共用記憶體，reserved memory
dump/write memory data

# MailBox
與R5溝通

# Ethernet
網路不通怎麼辦？網路測試工具
- PC (Windows, Linux) 
    - Wireshark
- Petalinux
    - iperf 測速度
    - ethertool
    - phytool
    - eth-loopback

裝置之間的傳輸
- tftp
- tcp

usb hub怎麼一插上去就自動設定ip
# Device Tree
???
# Linux System programming(系統程式設計)
IO, Daemon如何log?如何printf? 

fopen() vs open()