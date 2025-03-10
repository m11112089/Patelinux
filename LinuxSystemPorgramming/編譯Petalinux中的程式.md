[TOC]

# 1. 第三方工具鍊 third party toolchain
TBD: arm-none-eabi-gcc, aarch64-linux-gnu-gcc, etc.



# 2. Petalinux app
## 2.1 產生一個可供使用者編輯的資料夾
```
$ cd <plnx-proj-root>
$ petalinux-create -t apps --template install -n <appname>  --enable
e.g. petalinux-create -t apps --template install -n app  --enable
```
該資料夾會出現在 `<proj-root>/project-spec/meta-user/recipes-apps/appname`

### 2.2 編輯編譯config
這邊需要修改`app.bb`, `Makefile`, 直接查看教學目錄下的`app-2`資料夾
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
$ petalinux-build -c <app name> -x compile
e.g. petalinux-build -c app -x compile
```
編譯完成後，執行檔會出現在 `<proj-root>/build/tmp/work/cortexa72-cortexa53-xilinx-linux/app/1.0-r0/`

### 2.4 傳輸到目標上執行
可以使用`usb` 或 `ethernet`來操作，這邊使用 `tftp`
```
tftp -g -r test 192.168.1.1
tftp -g -r test2 192.168.1.1

chmod +x test
chmod +x test2

./test
./test2
```
# 3 Daemon

# 4 Delete Petalinux App

# Reference
[AMD Creating and Adding Custom Applications](https://docs.amd.com/r/en-US/ug1144-petalinux-tools-reference-guide/Creating-and-Adding-Custom-Applications)