//helloworld
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h> // syslog()

int main() {
    openlog("test2", LOG_PID | LOG_CONS, LOG_DAEMON);  // 使用 LOG_DAEMON 來確保訊息可見
    syslog(LOG_INFO, "test2 daemon started");
    while (1)
    {
        syslog(LOG_INFO, "Hello World\n");
        sleep(1);
    }
    return 0;
}