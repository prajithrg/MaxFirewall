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

	uint16_t inDstPort = 7653;
	uint8_t inIPProtocol = 17;

	uint16_t outDstPort = 7653;
	uint8_t outIPProtocol = 17;

	printf("Rules: INPUT--IPProtocol=%u, DstPort=%u :: OUTPUT-- IPProtocol=%u, DstPort=%u\n", inIPProtocol, inDstPort, outIPProtocol, outDstPort);
	max_file_t *maxfile = Firewall_init();
	max_engine_t * engine = max_load(maxfile, "*");	

//	max_ip_config(engine, MAX_NET_CONNECTION_QSFP_TOP_10G_PORT1, &top_ip, &netmask);
//	max_ip_config(engine, MAX_NET_CONNECTION_QSFP_BOT_10G_PORT1, &bot_ip, &netmask);

//	struct ether_addr local_mac2, remote_mac2;
//	max_arp_lookup_entry(engine, MAX_NET_CONNECTION_QSFP_BOT_10G_PORT1, &fwd_ip, &remote_mac2);
//	max_eth_get_default_mac_address(engine, MAX_NET_CONNECTION_QSFP_BOT_10G_PORT1, &local_mac2);

//	uint64_t localMac = 0, forwardMac = 0;
//	memcpy(&localMac, &local_mac2, 6);
//	memcpy(&forwardMac, &remote_mac2, 6);

//	printf("local MAC = %012llx\n" , (long long)localMac);
//	printf("forward MAC = %012llx\n" , (long long)forwardMac);

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

