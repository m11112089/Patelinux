<font color=#008000>Linux 自帶的工具</font>

<font color=#FF6600>Yocto開源第三方程式</font>

<font color=#0000FF>Bruce的小程式 </font>

# DISK
影響儲存檔案速度的因素:

    - 檔案系統之間的差異`xfs`, `ext4`, `journal`(差了20%以上，超誇張)
        - fdisk
        - xfs
        - dd
        - time
        - df
    - FSYNC/OSYNC

Sync-資料流中的檔案同步問題
要用Ram Disk嗎？

# Power
- hellopm

# Memory
與R5共用記憶體，reserved memory
dump/write memory data
- mmap()

# MailBox
與R5溝通

# Ethernet
socket INADDR_ANY
網路不通怎麼辦？網路測試工具
- PC (Windows, Linux) 
    - Wireshark
- Petalinux
    - iperf
    - ethertool
    - phytool
    - eth-loopback

裝置之間的網路傳輸
- tftp
    - [tftpd64](https://www.cnblogs.com/coolYuan/p/9035500.html)
- tcp
- socat
- mysocat (pipline transmition) 

usb hub怎麼一插上去就自動設定ip
socket绑定的ip為INADDR_ANY 的意義
# Device Tree
???
# Linux System programming(系統程式設計)
[編譯自己的 Hello world!](./LinuxSystemPorgramming/編譯Petalinux中的程式.md)

硬體儲存空間不夠大?跑不動Python? 在外掛裝置中製作自己的檔案系統巴!

怎麼讓R5知道你還活著? Heartbeat實現
- linux singnal
- mailbox

Daemon介紹
- 如何log?如何printf? 

process之間的通訊message queue

版本號

fopen() vs open()