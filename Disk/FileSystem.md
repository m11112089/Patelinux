# 使用不同檔案系統格式化
## 







存取資料過程： `read()` request 10bytes -> `Linux Kernel` request 10bytes localization **block(1kbytes)** -> `Disk`

影響io的效能
- system call (開根號 vs getuid())
- sync/nonsync