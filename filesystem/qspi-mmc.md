
## Steps
### 0. Build the petalinux project with btrfs file system
> $ petalinux-config -c kernel
```
  File systems ->
    Btrfs filesystem support
```

> $  gedit project-spec/meta-user/conf/petalinuxbsp.conf
```
#User Configuration

#OE_TERMINAL = "tmux"
IMAGE_INSTALL:append = " docker docker-ce-contrib btrfs-tools"
```

### 1. Partition the eMMC

- 1G for system
- rest for `rootfs`

> ~# fdisk /dev/mmcblk0
```
# p (print), d (delete any old), then:
# n, p, 1, <Enter>, +1G
# n, p, 2, <Enter>, <Enter>
# w
```
You should see something like:
```bash
Device         Boot   Start       End   Sectors   Size Id Type
/dev/mmcblk0p1           16   2097167   2097152     1G 83 Linux
/dev/mmcblk0p2      2097168 244277247 242180080 115.5G 83 Linux
```
### 2. Format the partitions
> ~# mkfs.vfat /dev/mmcblk0p1

> ~# mkfs.btrfs /dev/mmcblk0p2
### 3. Mount the partitions

- If the device is not mounted, please mount using the following command.

```
~# mkdir -p /run/media/mmcblk0p1
~# mkdir -p /run/media/mmcblk0p2

~# mount /dev/mmcblk0p1 /run/media/mmcblk0p1
~# mount /dev/mmcblk0p2 /run/media/mmcblk0p2
```

### 4. Extract the rootfs to p2 (btrfs)

Under the mount point `/run/media/mmcblk0p2`, `untar` the `rootfs`


> ~# tar -xvf rootfs.tar.gz -C /run/media/mmcblk0p2


### 5. Copy `Image` and `system.dtb` to p1 (FAT)


> ~# cp system.dtb Image /run/media/mmcblk0p1/


### 6. Patch boot.scr (U-Boot script) to add spi-mmc target
> $ tail -c+73 < boot.scr > out

[Using Distro Boot With Xilinx U-Boot](https://xilinx-wiki.atlassian.net/wiki/spaces/A/pages/749142017/Using+Distro+Boot+With+Xilinx+U-Boot#Modifying-an-Existing-boot.scr.uimg/boot.scr-File)
```
 P.S. `boot.scr` is a byte file; use `tail -c+73 < boot.scr > out` to extract the source.

 '^E^YV<81>U^UüM<9b><9e>ð^@^@
 <99>^@^@^@^@^@^@^@^@=?õg^E^B^F^@Boot script^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@^@
 <91>^@^@^@^@# This is a boot script for U-Boot
 # Generate boot.scr:
 # mkimage -c none -A arm -T script -d boot.cmd.default boot.scr
 #
 ################
 fitimage_name=image.ub
 kernel_name=Image
 ramdisk_name=ramdisk.cpio.gz.

```
Edit: in the loop over boot_targets, add a branch for spi-mmc.


```uboot
for boot_target in ${boot_targets};
```

```diff
diff --git a/out b/out
index 4499346..96d5b26 100644
--- a/out
+++ b/out
@@ -67,4 +67,12 @@ do
                booti 0x00200000 0x04000000 0x00100000;
                echo "Booting using Separate images failed"
        fi
+       if test "${boot_target}" = "spi-mmc"; then
+               setenv bootargs = "root=/dev/mmcblk0p2 rootwait rw";
+
+               fatload mmc 0:1 0x00200000 Image;
+               fatload mmc 0:1 0x00100000 system.dtb;
+               booti 0x00200000 - 0x00100000
+               echo "Booting using Separate images failed"
+       fi
 done
```

Now (re)generate boot.scr. Use arm64 (ZynqMP is AArch64):

> $ mkimage -C none -A arm -T script -d out boot.scr


### 7. Program the new boot.scr into QSPI (from U-Boot)

> ZynqMP> sf probe;fatload usb 0:1 0x80000 pc100/boot_2.scr;echo "erase ...";sf erase 1000000 3000000;echo "write src...";sf write 0x80000 0x3E80000 $filesize;


### 8. Select the new boot target and save

> ZynqMP> setenv boot_target spi-mmc

> ZynqMP> saveenv

```

### Result
After booting via spi-mmc, root (/) should be btrfs on /dev/mmcblk0p2:

```bash
root@pc100:~# df -Th
Filesystem           Type            Size      Used Available Use% Mounted on
/dev/root            ext4          113.1G    555.4M    106.8G   1% /
devtmpfs             devtmpfs        1.8G      4.0K      1.8G   0% /dev
tmpfs                tmpfs           1.9G         0      1.9G   0% /dev/shm
tmpfs                tmpfs         786.5M      9.8M    776.7M   1% /run
tmpfs                tmpfs           4.0M         0      4.0M   0% /sys/fs/cgroup
tmpfs                tmpfs           1.9G         0      1.9G   0% /tmp
tmpfs                tmpfs           1.9G     32.0K      1.9G   0% /var/volatile
/dev/mmcblk0p1       vfat         1022.0M     20.8M   1001.2M   2% /run/media/mmcblk0p1
/dev/sda1            vfat          115.2G    294.8M    114.9G   0% /run/media/sda1
tmpfs                tmpfs         393.3M         0    393.3M   0% /run/user/0
```
