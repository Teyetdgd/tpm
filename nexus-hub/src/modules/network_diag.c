#include "network_diag.h"
#include <lumia.h>

int network_diag_run(void) {
    int sock = net_socket(2, 1, 0); // AF_INET, SOCK_STREAM
    if (sock >= 0) {
        // dummy check
        return 1;
    }
    return 0;
}

