#define _GNU_SOURCE

#include <arpa/inet.h>
#include <linux/if.h>
#include <linux/if_ether.h>
#include <netinet/ether.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "MaxSLiCInterface.h"

int main(int argc, char *argv[]) {
	if (argc < 4) {
		printf("Syntax: %s <TOP local IP> <BOT local IP> <forward IP>\n", argv[0]);
		return 1;
	}

	struct in_addr top_ip;
	struct in_addr bot_ip;
	struct in_addr fwd_ip;
	struct in_addr netmask;

	inet_aton(argv[1], &top_ip);
	inet_aton(argv[2], &bot_ip);
	inet_aton(argv[3], &fwd_ip);
	inet_aton("255.255.255.0", &netmask);

	// INPUT/OUTPUT Rule Configuration : IP Protocol (UDP=17, TCP=6, etc.) & Destination Port
	uint16_t inDstPort = 7653;
	uint8_t inIPProtocol = 17;

	uint16_t outDstPort = 1534;
	uint8_t outIPProtocol = 17;

	printf("Rules: INPUT--IPProtocol=%u, DstPort=%u :: OUTPUT-- IPProtocol=%u, DstPort=%u\n", inIPProtocol, inDstPort, outIPProtocol, outDstPort);
	max_file_t *maxfile = Firewall_init();
	max_engine_t * engine = max_load(maxfile, "*");	

	max_config_set_bool(MAX_CONFIG_PRINTF_TO_STDOUT, true);

	max_actions_t *action = max_actions_init(maxfile, NULL);
	max_set_uint64t(action, "inFirewallKernel", "inIPProtocol", inIPProtocol);
	max_set_uint64t(action, "inFirewallKernel", "inDstPort", inDstPort);

	max_set_uint64t(action, "outFirewallKernel", "outIPProtocol", outIPProtocol);
	max_set_uint64t(action, "outFirewallKernel", "outDstPort", outDstPort);

	max_run(engine, action);

	printf("JDFE Running.\n");
	getchar();

	max_unload(engine);
	max_file_free(maxfile);

	printf("Done.\n");
	return 0;
}

