[TOC]

# 1. 交叉編譯器
## 1.1 安裝GCC
檢查是否有安裝過針對ARM AA64 的編譯器GCC
```
// Any Linux, windows wsl, ubuntu .etc
$ which aarch64-linux-gnu-gcc
```
如果沒有安裝
```
sudo apt-get install gcc-aarch64-linux-gnu
```

## 1.2 編譯程式

```
$ gcc <test.c> -o test
```

## 1.3 傳輸到目標上執行
可以使用`usb` 或 `ethernet`來操作，這邊使用 `tftp`下載tftp server file中的執行檔
```
// petalinux
# tftp -g -r test 192.168.1.1
# tftp -g -r test2 192.168.1.1

# chmod +x test
# chmod +x test2

# ./test
# ./test2
```



# 2. Petalinux app
## 2.1 產生一個可供使用者編輯的資料夾
```
// build server
$ cd <plnx-proj-root>
$ petalinux-create -t apps --template install -n <appname>  --enable
e.g. petalinux-create -t apps --template install -n app  --enable
```
該資料夾會出現在 `<proj-root>/project-spec/meta-user/recipes-apps/appname`

### 2.2 編輯編譯config
這邊需要修改`app.bb`, `Makefile`, 直接查看本目錄下的`app-2`資料夾
```
.
├── README
├── app.bb        <---
└── files
    ├── Makefile  <---
    ├── test.c
    └── test2.c

```
P.S. `.bb`(BitBake recipe) 是指示Yocto BitBake 如何取得、設定、編譯和安裝軟體的腳本

TBD: compile toolchain 之間的關係 `Yocto`, `BitBake`, `.bb`, `Makefile`, `xxx-gcc`

### 2.3 編譯Petalinux APP

編譯指定的Petalinux APP
```
// build server
$ petalinux-build -c <app name> -x compile
e.g. petalinux-build -c app -x compile
```
編譯完成後，執行檔會出現在 `<proj-root>/build/tmp/work/cortexa72-cortexa53-xilinx-linux/app/1.0-r0/`

### 2.4 傳輸到目標上執行
可以使用`usb` 或 `ethernet`來操作，這邊使用 `tftp`下載tftp server file中的執行檔
```
// petalinux
# tftp -g -r test 192.168.1.1
# tftp -g -r test2 192.168.1.1

# chmod +x test
# chmod +x test2

# ./test
# ./test2
```

### 2.5 將其編譯到image.ub中並載入

```
// build server
$ petalinux-build
```
```
// uboot
tftpboot 0x10000000 image.ub;bootm 0x10000000;
```

執行檔會出現在 `/usr/bin/test`, `/usr/bin/test2`中，因為在`.bb`檔中有以下設定:
```
install -m 0755 test ${D}/${bindir}
install -m 0755 test2 ${D}/${bindir}
```


# 3 Daemon
在 Petalinux 中，守護程序（常稱為「守護程序」）是一種在背景運行並提供特定服務或功能的進程。它包括系統啟動時 **<font color="red">自動啟動</font>** ，並持續運行以處理系統任務、監控事件或提供遠端服務。

### 3.1 .service檔案
- 新增 `.service` 檔案，用來設定`Daemon`的啟動參數
- 修改`.bb`檔，將.service檔案安裝到指定目錄下
- 修改`test2`中的`printf`，改為`syslog`

請查看本資料夾中的`app-3`資料夾
```
.
├── README
├── app.bb  <---
└── files
    ├── Makefile
    ├── app
    ├── test.c
    ├── test2.c
    └── testd.service   <---
```
### 3.2 編譯 `image.ub` 並載入

```
// build server
$ petalinux-build
```
```
// uboot
tftpboot 0x10000000 image.ub;bootm 0x10000000;
```
## Daemon 狀態確認
```
// petalinux

//查看 test2 是否在執行
# ps aux | grep test2

// 查看 Systemd 服務狀態
# systemctl status testd

//  查看 log 輸出
# journalctl -u testd -f
# cat /var/log/messages | grep test2

```

## 強制停止Daemon

```
// petalinux

# killall test2
```
# 4 Delete Petalinux App
1. 編輯 `project-spec/meta-user/conf/user-rootfsconfig`，刪除`app`名稱
2. 使用指令`# petalinux-config -c rootfs` -\> `user apps` -\> `uncheck <appname>`
3. 刪除路徑中的資料夾 `project-spec/meta-user/repicpes-apps/<appname>`

# Reference
[AMD Creating and Adding Custom Applications](https://docs.amd.com/r/en-US/ug1144-petalinux-tools-reference-guide/Creating-and-Adding-Custom-Applications)