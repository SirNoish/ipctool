#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "backup.h"
#include "dns.h"
#include "http.h"
#include "network.h"

void do_backup(const char *yaml, size_t yaml_len) {
    printf("do_backup()\n");

    nservers_t ns;
    ns.len = 0;

    if (!parse_resolv_conf(&ns)) {
#if 0
        fprintf(stderr, "parse_resolv_conf failed\n");
#endif
        return;
    }
    add_predefined_ns(&ns, 0xd043dede /* 208.67.222.222 of OpenDNS */,
                      0x01010101 /* 1.1.1.1 of Cloudflare */, 0);

    char mac[32];
    if (!get_mac_address(mac, sizeof mac)) {
        return;
    };

    printf("%d\n", upload("camware.s3.eu-north-1.amazonaws.com", mac, &ns, yaml,
                          yaml_len));
    printf("~do_backup()\n");
}