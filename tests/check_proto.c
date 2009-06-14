
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <check.h>

#include "../src/common.h"
#include "../src/util.h"
#include "../src/proto/protos.h"

#include "check_wrap.h"

uint32_t options = OPT_DAEMON | OPT_CHECK;
extern uint8_t loglevel;

extern char check_wrap_errstr[];

START_TEST(test_proto_packet) {
    struct master_msg msg;
    struct netif master, netif;
    struct sysinfo sysinfo;

    memset(&sysinfo, 0, sizeof(struct sysinfo));
    sysinfo_fetch(&sysinfo);
    sysinfo.cap_active = CAP_ROUTER;
    sysinfo.country[0] = 'Z';
    sysinfo.country[1] = 'Z';
    sysinfo.uts_rel[0] = 12;
    sysinfo.uts_rel[1] = 34;
    sysinfo.uts_rel[2] = 56;
    memset(sysinfo.hwaddr, 77, ETHER_ADDR_LEN);
    strlcpy(sysinfo.uts_str, "Testing", sizeof(sysinfo.uts_str));
    strlcpy(sysinfo.uts.sysname, "Testing", sizeof(sysinfo.uts.sysname));
    strlcpy(sysinfo.hostname, "Blanket", sizeof(sysinfo.hostname));
    strlcpy(sysinfo.location, "Towel", sizeof(sysinfo.location));
    strlcpy(sysinfo.hw_revision, "lala", LLDP_INVENTORY_SIZE);  
    strlcpy(sysinfo.fw_revision, "lala", LLDP_INVENTORY_SIZE);  
    strlcpy(sysinfo.sw_revision, "lala", LLDP_INVENTORY_SIZE);  
    strlcpy(sysinfo.serial_number, "lala", LLDP_INVENTORY_SIZE);  
    strlcpy(sysinfo.manufacturer, "lala", LLDP_INVENTORY_SIZE);  
    strlcpy(sysinfo.model_name, "lala", LLDP_INVENTORY_SIZE);  
    strlcpy(sysinfo.asset_id, "lala", LLDP_INVENTORY_SIZE);  

    memset(&master, 0, sizeof(struct netif));
    master.index = 3;
    master.argv = 1;
    master.slave = 0;
    master.lacp = 1;
    master.type = NETIF_BONDING;
    netif.mtu = 1500;
    master.subif = &netif;
    master.master = NULL;
    master.ipaddr4 = htonl(0x7f000001);
    memset(master.ipaddr6, 'a', sizeof(master.ipaddr6));
    strlcpy(master.name, "bond0", IFNAMSIZ);

    memset(&netif, 0, sizeof(struct netif));
    netif.index = 1;
    netif.argv = 0;
    netif.slave = 1;
    netif.type = NETIF_REGULAR;
    netif.mtu = 9000;
    netif.duplex = 1;
    netif.subif = NULL;
    netif.master = &master;
    master.ipaddr4 = htonl(0xa0000001);
    memset(master.ipaddr6, 'b', sizeof(master.ipaddr6));
    strlcpy(netif.name, "eth0", IFNAMSIZ);
    strlcpy(netif.description, "utp naar de buren", IFNAMSIZ);

    memset(msg.msg, 0, ETHER_MAX_LEN);
    msg.len = lldp_packet(msg.msg, &netif, &sysinfo);
    fail_unless(msg.len == 235, "length should not be %d", msg.len);
    msg.len = cdp_packet(msg.msg, &netif, &sysinfo);
    fail_unless(msg.len == 154, "length should not be %d", msg.len);
    msg.len = edp_packet(msg.msg, &netif, &sysinfo);
    fail_unless(msg.len == 109, "length should not be %d", msg.len);
    msg.len = fdp_packet(msg.msg, &netif, &sysinfo);
    fail_unless(msg.len == 122, "length should not be %d", msg.len);
    msg.len = ndp_packet(msg.msg, &netif, &sysinfo);
    fail_unless(msg.len == 33, "length should not be %d", msg.len);

    sysinfo.cap = CAP_HOST;
    sysinfo.cap_active = CAP_HOST;
    netif.master = NULL;
    msg.len = lldp_packet(msg.msg, &netif, &sysinfo);
    fail_unless(msg.len == 184, "length should not be %d", msg.len);
    msg.len = cdp_packet(msg.msg, &netif, &sysinfo);
    fail_unless(msg.len == 109, "length should not be %d", msg.len);
    msg.len = edp_packet(msg.msg, &netif, &sysinfo);
    fail_unless(msg.len == 89, "length should not be %d", msg.len);
    msg.len = fdp_packet(msg.msg, &netif, &sysinfo);
    fail_unless(msg.len == 75, "length should not be %d", msg.len);
    msg.len = ndp_packet(msg.msg, &netif, &sysinfo);
    fail_unless(msg.len == 33, "length should not be %d", msg.len);

    sysinfo.cap_active = CAP_BRIDGE;
    msg.len = lldp_packet(msg.msg, &netif, &sysinfo);
    fail_unless(msg.len == 184, "length should not be %d", msg.len);
    msg.len = cdp_packet(msg.msg, &netif, &sysinfo);
    fail_unless(msg.len == 109, "length should not be %d", msg.len);
    msg.len = edp_packet(msg.msg, &netif, &sysinfo);
    fail_unless(msg.len == 89, "length should not be %d", msg.len);
    msg.len = fdp_packet(msg.msg, &netif, &sysinfo);
    fail_unless(msg.len == 77, "length should not be %d", msg.len);
    msg.len = ndp_packet(msg.msg, &netif, &sysinfo);
    fail_unless(msg.len == 33, "length should not be %d", msg.len);

    sysinfo.cap_active = CAP_SWITCH;
    msg.len = lldp_packet(msg.msg, &netif, &sysinfo);
    fail_unless(msg.len == 184, "length should not be %d", msg.len);
    msg.len = cdp_packet(msg.msg, &netif, &sysinfo);
    fail_unless(msg.len == 109, "length should not be %d", msg.len);
    msg.len = edp_packet(msg.msg, &netif, &sysinfo);
    fail_unless(msg.len == 89, "length should not be %d", msg.len);
    msg.len = fdp_packet(msg.msg, &netif, &sysinfo);
    fail_unless(msg.len == 77, "length should not be %d", msg.len);
    msg.len = ndp_packet(msg.msg, &netif, &sysinfo);
    fail_unless(msg.len == 33, "length should not be %d", msg.len);
}
END_TEST

START_TEST(test_lldp_check) {
    struct master_msg msg;
    struct ether_hdr ether;
    static uint8_t lldp_dst[] = LLDP_MULTICAST_ADDR;

    msg.len = ETHER_MIN_LEN;
    fail_unless (lldp_check(msg.msg, msg.len) == NULL,
	    "empty packets should generate a NULL");

    memcpy(ether.dst, lldp_dst, ETHER_ADDR_LEN);
    memcpy(msg.msg, &ether, sizeof(ether));
    fail_unless (lldp_check(msg.msg, msg.len) == NULL,
	    "packets without an ethertype should generate a NULL");

    ether.type = htons(ETHERTYPE_LLDP);
    memcpy(msg.msg, &ether, sizeof(ether));
    fail_unless (lldp_check(msg.msg, msg.len) == msg.msg + sizeof(ether),
	    "valid packets should return a correct ptr");
}
END_TEST

START_TEST(test_cdp_check) {
    struct master_msg msg;
    struct ether_hdr ether;
    struct ether_llc llc;
    static uint8_t cdp_dst[] = CDP_MULTICAST_ADDR;
    static uint8_t cdp_org[] = LLC_ORG_CISCO;

    msg.len = ETHER_MIN_LEN;
    fail_unless (cdp_check(msg.msg, msg.len) == NULL,
	    "empty packets should generate a NULL");

    memcpy(ether.dst, cdp_dst, ETHER_ADDR_LEN);
    memcpy(msg.msg, &ether, sizeof(ether));
    fail_unless (cdp_check(msg.msg, msg.len) == NULL,
	    "packets without an llc header should generate a NULL");

    memcpy(llc.org, cdp_org, sizeof(llc.org));
    memcpy(msg.msg + sizeof(ether), &llc, sizeof(llc));
    fail_unless (cdp_check(msg.msg, msg.len) == NULL,
	    "packets with an invalid llc header should generate a NULL");

    llc.protoid = htons(LLC_PID_CDP);
    memcpy(msg.msg + sizeof(ether), &llc, sizeof(llc));
    fail_unless (cdp_check(msg.msg, msg.len) == 
		 msg.msg + sizeof(ether) + sizeof(llc),
	    "valid packets should return a correct ptr");
}
END_TEST

START_TEST(test_edp_check) {
    struct master_msg msg;
    struct ether_hdr ether;
    struct ether_llc llc;
    static uint8_t edp_dst[] = EDP_MULTICAST_ADDR;
    static uint8_t edp_org[] = LLC_ORG_EXTREME;

    msg.len = ETHER_MIN_LEN;
    fail_unless (edp_check(msg.msg, msg.len) == NULL,
	    "empty packets should generate a NULL");

    memcpy(ether.dst, edp_dst, ETHER_ADDR_LEN);
    memcpy(msg.msg, &ether, sizeof(ether));
    fail_unless (edp_check(msg.msg, msg.len) == NULL,
	    "packets without an llc header should generate a NULL");

    memcpy(llc.org, edp_org, sizeof(llc.org));
    memcpy(msg.msg + sizeof(ether), &llc, sizeof(llc));
    fail_unless (edp_check(msg.msg, msg.len) == NULL,
	    "packets with an invalid llc header should generate a NULL");

    llc.protoid = htons(LLC_PID_EDP);
    memcpy(msg.msg + sizeof(ether), &llc, sizeof(llc));
    fail_unless (edp_check(msg.msg, msg.len) == 
		 msg.msg + sizeof(ether) + sizeof(llc),
	    "valid packets should return a correct ptr");
}
END_TEST

START_TEST(test_fdp_check) {
    struct master_msg msg;
    struct ether_hdr ether;
    struct ether_llc llc;
    static uint8_t fdp_dst[] = FDP_MULTICAST_ADDR;
    static uint8_t fdp_org[] = LLC_ORG_FOUNDRY;

    msg.len = ETHER_MIN_LEN;
    fail_unless (fdp_check(msg.msg, msg.len) == NULL,
	    "empty packets should generate a NULL");

    memcpy(ether.dst, fdp_dst, ETHER_ADDR_LEN);
    memcpy(msg.msg, &ether, sizeof(ether));
    fail_unless (fdp_check(msg.msg, msg.len) == NULL,
	    "packets without an llc header should generate a NULL");

    memcpy(llc.org, fdp_org, sizeof(llc.org));
    memcpy(msg.msg + sizeof(ether), &llc, sizeof(llc));
    fail_unless (fdp_check(msg.msg, msg.len) == NULL,
	    "packets with an invalid llc header should generate a NULL");

    llc.protoid = htons(LLC_PID_FDP);
    memcpy(msg.msg + sizeof(ether), &llc, sizeof(llc));
    fail_unless (fdp_check(msg.msg, msg.len) == 
		 msg.msg + sizeof(ether) + sizeof(llc),
	    "valid packets should return a correct ptr");
}
END_TEST

START_TEST(test_ndp_check) {
    struct master_msg msg;
    struct ether_hdr ether;
    struct ether_llc llc;
    static uint8_t ndp_dst[] = NDP_MULTICAST_ADDR;
    static uint8_t ndp_org[] = LLC_ORG_NORTEL;

    msg.len = ETHER_MIN_LEN;
    fail_unless (ndp_check(msg.msg, msg.len) == NULL,
	    "empty packets should generate a NULL");

    memcpy(ether.dst, ndp_dst, ETHER_ADDR_LEN);
    memcpy(msg.msg, &ether, sizeof(ether));
    fail_unless (ndp_check(msg.msg, msg.len) == NULL,
	    "packets without an llc header should generate a NULL");

    memcpy(llc.org, ndp_org, sizeof(llc.org));
    memcpy(msg.msg + sizeof(ether), &llc, sizeof(llc));
    fail_unless (ndp_check(msg.msg, msg.len) == NULL,
	    "packets with an invalid llc header should generate a NULL");

    llc.protoid = htons(LLC_PID_NDP_HELLO);
    memcpy(msg.msg + sizeof(ether), &llc, sizeof(llc));
    fail_unless (ndp_check(msg.msg, msg.len) == 
		 msg.msg + sizeof(ether) + sizeof(llc),
	    "valid packets should return a correct ptr");
}
END_TEST

void read_packet(struct master_msg *msg, const char *path) {
    int fd;

    memset(msg->msg, 0, ETHER_MAX_LEN);
    msg->len = 0;
    msg->ttl = 0;
    memset(msg->peer, 0, IFDESCRSIZE);

    if ((fd = open(path, O_RDONLY)) == -1)
	my_fatal("failed to open %s", path);
    msg->len = read(fd, msg->msg, ETHER_MAX_LEN);
}

START_TEST(test_lldp_peer) {
    struct master_msg msg;
    const char *errstr = NULL;

    loglevel = INFO;

    errstr = "Invalid LLDP packet: missing Chassis ID TLV";
    read_packet(&msg, "proto/lldp/00.empty");
    fail_unless (lldp_peer(&msg) == 0, "empty packets should return 0");
    fail_unless (strcmp(check_wrap_errstr, errstr) == 0,
	"incorrect message logged: %s", check_wrap_errstr);

    errstr = "check";
    my_log(CRIT, errstr);
    read_packet(&msg, "proto/lldp/01.chassis_id.broken");
    fail_unless (lldp_peer(&msg) == 0, "broken packets should return 0");
    fail_unless (strcmp(check_wrap_errstr, errstr) == 0,
	"incorrect message logged: %s", check_wrap_errstr);
    errstr = "Invalid LLDP packet: missing Port ID TLV";
    read_packet(&msg, "proto/lldp/02.chassis_id.only");
    fail_unless (lldp_peer(&msg) == 0, "incomplete packets should return 0");
    fail_unless (strcmp(check_wrap_errstr, errstr) == 0,
	"incorrect message logged: %s", check_wrap_errstr);
    errstr = "Invalid LLDP packet: missing Chassis ID TLV";
    read_packet(&msg, "proto/lldp/03.chassis_id.missing");
    fail_unless (lldp_peer(&msg) == 0, "incomplete packets should return 0");
    fail_unless (strcmp(check_wrap_errstr, errstr) == 0,
	"incorrect message logged: %s", check_wrap_errstr);

    errstr = "check";
    my_log(CRIT, errstr);
    read_packet(&msg, "proto/lldp/11.port_id.broken");
    fail_unless (lldp_peer(&msg) == 0, "broken packets should return 0");
    fail_unless (strcmp(check_wrap_errstr, errstr) == 0,
	"incorrect message logged: %s", check_wrap_errstr);
    errstr = "Invalid LLDP packet: missing TTL TLV";
    read_packet(&msg, "proto/lldp/12.port_id.only");
    fail_unless (lldp_peer(&msg) == 0, "incomplete packets should return 0");
    fail_unless (strcmp(check_wrap_errstr, errstr) == 0,
	"incorrect message logged: %s", check_wrap_errstr);
    errstr = "Invalid LLDP packet: missing Port ID TLV";
    read_packet(&msg, "proto/lldp/13.port_id.missing");
    fail_unless (lldp_peer(&msg) == 0, "incomplete packets should return 0");
    fail_unless (strcmp(check_wrap_errstr, errstr) == 0,
	"incorrect message logged: %s", check_wrap_errstr);

    errstr = "Invalid LLDP packet: invalid TTL TLV";
    read_packet(&msg, "proto/lldp/21.ttl.broken");
    fail_unless (lldp_peer(&msg) == 0, "broken packets should return 0");
    fail_unless (strcmp(check_wrap_errstr, errstr) == 0,
	"incorrect message logged: %s", check_wrap_errstr);
    errstr = "Corrupt LLDP packet: missing END TLV";
    read_packet(&msg, "proto/lldp/22.ttl.only");
    fail_unless (lldp_peer(&msg) == 0, "incomplete packets should return 0");
    fail_unless (strcmp(check_wrap_errstr, errstr) == 0,
	"incorrect message logged: %s", check_wrap_errstr);
    errstr = "Invalid LLDP packet: missing TTL TLV";
    read_packet(&msg, "proto/lldp/23.ttl.missing");
    fail_unless (lldp_peer(&msg) == 0, "incomplete packets should return 0");
    fail_unless (strcmp(check_wrap_errstr, errstr) == 0,
	"incorrect message logged: %s", check_wrap_errstr);

    errstr = "Corrupt LLDP packet: duplicate System Name TLV";
    read_packet(&msg, "proto/lldp/31.system_name.dup");
    fail_unless (lldp_peer(&msg) == 0, "broken packets should return 0");
    fail_unless (strcmp(check_wrap_errstr, errstr) == 0,
	"incorrect message logged: %s", check_wrap_errstr);
    errstr = "Corrupt LLDP packet: invalid System Name TLV";
    read_packet(&msg, "proto/lldp/32.system_name.broken");
    fail_unless (lldp_peer(&msg) == 0, "broken packets should return 0");
    fail_unless (strcmp(check_wrap_errstr, errstr) == 0,
	"incorrect message logged: %s", check_wrap_errstr);

    errstr = "Corrupt LLDP packet: invalid TLV Type";
    read_packet(&msg, "proto/lldp/91.tlv.unknown");
    fail_unless (lldp_peer(&msg) == 0, "unknown tlv's should return 0");
    fail_unless (strcmp(check_wrap_errstr, errstr) == 0,
	"incorrect message logged: %s", check_wrap_errstr);
    errstr = "Corrupt LLDP packet: invalid TLV";
    read_packet(&msg, "proto/lldp/92.tlv.broken");
    fail_unless (lldp_peer(&msg) == 0, "broken packets should return 0");
    fail_unless (strcmp(check_wrap_errstr, errstr) == 0,
	"incorrect message logged: %s", check_wrap_errstr);
    errstr = "Corrupt LLDP packet: invalid TLV Length";
    read_packet(&msg, "proto/lldp/93.tlv.long");
    fail_unless (lldp_peer(&msg) == 0, "broken packets should return 0");
    fail_unless (strcmp(check_wrap_errstr, errstr) == 0,
	"incorrect message logged: %s", check_wrap_errstr);

    errstr = "Corrupt LLDP packet: invalid END TLV";
    read_packet(&msg, "proto/lldp/97.end.invalid");
    fail_unless (lldp_peer(&msg) == 0, "invalid packets should return 0");
    fail_unless (strcmp(check_wrap_errstr, errstr) == 0,
	"incorrect message logged: %s", check_wrap_errstr);
    errstr = "Corrupt LLDP packet: invalid TLV";
    read_packet(&msg, "proto/lldp/98.end.broken");
    fail_unless (lldp_peer(&msg) == 0, "broken packets should return 0");
    fail_unless (strcmp(check_wrap_errstr, errstr) == 0,
	"incorrect message logged: %s", check_wrap_errstr);
    errstr = "Corrupt LLDP packet: missing END TLV";
    read_packet(&msg, "proto/lldp/99.end.missing");
    fail_unless (lldp_peer(&msg) == 0, "incomplete packets should return 0");
    fail_unless (strcmp(check_wrap_errstr, errstr) == 0,
	"incorrect message logged: %s", check_wrap_errstr);

    errstr = "check";
    my_log(CRIT, errstr);
    read_packet(&msg, "proto/lldp/41.good.small");
    fail_unless (lldp_peer(&msg) == msg.len, "packet length incorrect");
    fail_unless (strcmp(check_wrap_errstr, errstr) == 0,
	"incorrect message logged: %s", check_wrap_errstr);
    fail_unless (msg.ttl == 120, "ttl should be 120");
    fail_unless (strcmp(msg.peer, "test") == 0, "system name should be 'test'");
    read_packet(&msg, "proto/lldp/42.good.big");
    fail_unless (strcmp(check_wrap_errstr, errstr) == 0,
	"incorrect message logged: %s", check_wrap_errstr);
    fail_unless (lldp_peer(&msg) == msg.len, "packet length incorrect");
    fail_unless (msg.ttl == 120, "ttl should be 120");
    fail_unless (strcmp(msg.peer, "Summit300-48") == 0,
		"system name should be 'Summit300-48'");
    read_packet(&msg, "proto/lldp/43.good.lldpmed");
    fail_unless (strcmp(check_wrap_errstr, errstr) == 0,
	"incorrect message logged: %s", check_wrap_errstr);
    fail_unless (lldp_peer(&msg) == msg.len, "packet length incorrect");
    fail_unless (msg.ttl == 120, "ttl should be 120");
    fail_unless (strcmp(msg.peer, "ProCurve Switch 2600-8-PWR") == 0,
		"system name should be 'ProCurve Switch 2600-8-PWR'");
}
END_TEST

START_TEST(test_cdp_peer) {
    struct master_msg msg;
    const char *errstr = NULL;

    loglevel = INFO;

    errstr = "missing CDP header";
    read_packet(&msg, "proto/cdp/00.empty");
    fail_unless (cdp_peer(&msg) == 0, "empty packets should return 0");
    fail_unless (strcmp(check_wrap_errstr, errstr) == 0,
	"incorrect message logged: %s", check_wrap_errstr);

    errstr = "missing CDP header";
    read_packet(&msg, "proto/cdp/01.header.broken");
    fail_unless (cdp_peer(&msg) == 0, "broken packets should return 0");
    fail_unless (strcmp(check_wrap_errstr, errstr) == 0,
	"incorrect message logged: %s", check_wrap_errstr);

    errstr = "invalid CDP version";
    read_packet(&msg, "proto/cdp/02.header.invalid");
    fail_unless (cdp_peer(&msg) == 0, "broken packets should return 0");
    fail_unless (strcmp(check_wrap_errstr, errstr) == 0,
	"incorrect message logged: %s", check_wrap_errstr);
    read_packet(&msg, "proto/cdp/03.header.only");
    fail_unless (cdp_peer(&msg) == msg.len, "packet length incorrect");

    errstr = "Corrupt CDP packet: invalid System Name TLV";
    read_packet(&msg, "proto/cdp/21.device_id.broken");
    fail_unless (cdp_peer(&msg) == 0, "broken packets should return 0");
    fail_unless (strcmp(check_wrap_errstr, errstr) == 0,
	"incorrect message logged: %s", check_wrap_errstr);

    read_packet(&msg, "proto/cdp/91.tlv.unknown");
    fail_unless (cdp_peer(&msg) == msg.len, "packet length incorrect");
    errstr = "Corrupt CDP packet: invalid TLV";
    read_packet(&msg, "proto/cdp/92.tlv.broken");
    fail_unless (cdp_peer(&msg) == 0, "broken packets should return 0");
    fail_unless (strcmp(check_wrap_errstr, errstr) == 0,
	"incorrect message logged: %s", check_wrap_errstr);
    errstr = "Corrupt CDP packet: invalid TLV length";
    read_packet(&msg, "proto/cdp/93.tlv.long");
    fail_unless (cdp_peer(&msg) == 0, "broken packets should return 0");
    fail_unless (strcmp(check_wrap_errstr, errstr) == 0,
	"incorrect message logged: %s", check_wrap_errstr);

    errstr = "check";
    my_log(CRIT, errstr);
    read_packet(&msg, "proto/cdp/41.good.small");
    fail_unless (strcmp(check_wrap_errstr, errstr) == 0,
	"incorrect message logged: %s", check_wrap_errstr);
    fail_unless (cdp_peer(&msg) == msg.len, "packet length incorrect");
    fail_unless (msg.ttl == 180, "ttl should be 180");
    fail_unless (strcmp(msg.peer, "R1") == 0, "system name should be 'R1'");
    read_packet(&msg, "proto/cdp/42.good.medium");
    fail_unless (strcmp(check_wrap_errstr, errstr) == 0,
	"incorrect message logged: %s", check_wrap_errstr);
    fail_unless (cdp_peer(&msg) == msg.len, "packet length incorrect");
    fail_unless (msg.ttl == 180, "ttl should be 180");
    fail_unless (strcmp(msg.peer, "R2D2") == 0,
		"system name should be 'R2D2'");
    read_packet(&msg, "proto/cdp/43.good.big");
    fail_unless (strcmp(check_wrap_errstr, errstr) == 0,
	"incorrect message logged: %s", check_wrap_errstr);
    fail_unless (cdp_peer(&msg) == msg.len, "packet length incorrect");
    fail_unless (msg.ttl == 180, "ttl should be 180");
    fail_unless (strcmp(msg.peer, "xpfs1.yapkjn.network.bla.nl") == 0,
		"system name should be 'xpfs1.yapkjn.network.bla.nl'");
    read_packet(&msg, "proto/cdp/44.good.bcm");
    fail_unless (strcmp(check_wrap_errstr, errstr) == 0,
	"incorrect message logged: %s", check_wrap_errstr);
    fail_unless (cdp_peer(&msg) == msg.len, "packet length incorrect");
    fail_unless (msg.ttl == 180, "ttl should be 180");
    fail_unless (strcmp(msg.peer, "0060B9C14027") == 0,
		"system name should be '0060B9C14027'");
}
END_TEST

START_TEST(test_edp_peer) {
    struct master_msg msg;
    const char *errstr = NULL;

    loglevel = INFO;

    errstr = "missing EDP header";
    read_packet(&msg, "proto/edp/00.empty");
    fail_unless (edp_peer(&msg) == 0, "empty packets should return 0");
    fail_unless (strcmp(check_wrap_errstr, errstr) == 0,
	"incorrect message logged: %s", check_wrap_errstr);

    my_log(CRIT, "check");
    read_packet(&msg, "proto/edp/01.header.broken");
    fail_unless (edp_peer(&msg) == 0, "broken packets should return 0");
    fail_unless (strcmp(check_wrap_errstr, errstr) == 0,
	"incorrect message logged: %s", check_wrap_errstr);
    errstr = "unsupported EDP version";
    read_packet(&msg, "proto/edp/02.header.invalid");
    fail_unless (edp_peer(&msg) == 0, "broken packets should return 0");
    fail_unless (strcmp(check_wrap_errstr, errstr) == 0,
	"incorrect message logged: %s", check_wrap_errstr);
    errstr = "check";
    my_log(CRIT, errstr);
    read_packet(&msg, "proto/edp/03.header.only");
    fail_unless (edp_peer(&msg) == msg.len, "packet length incorrect");
    fail_unless (strcmp(check_wrap_errstr, errstr) == 0,
	"incorrect message logged: %s", check_wrap_errstr);

    errstr = "Corrupt EDP packet: invalid Display TLV";
    read_packet(&msg, "proto/edp/21.display.broken");
    fail_unless (edp_peer(&msg) == 0, "broken packets should return 0");
    fail_unless (strcmp(check_wrap_errstr, errstr) == 0,
	"incorrect message logged: %s", check_wrap_errstr);

    errstr = "check";
    my_log(CRIT, errstr);
    read_packet(&msg, "proto/edp/91.tlv.unknown");
    fail_unless (edp_peer(&msg) == msg.len, "packet length incorrect");
    fail_unless (strcmp(check_wrap_errstr, errstr) == 0,
	"incorrect message logged: %s", check_wrap_errstr);
    errstr = "Corrupt EDP packet: invalid TLV";
    read_packet(&msg, "proto/edp/92.tlv.invalid");
    fail_unless (edp_peer(&msg) == 0, "broken packets should return 0");
    fail_unless (strcmp(check_wrap_errstr, errstr) == 0,
	"incorrect message logged: %s", check_wrap_errstr);
    errstr = "Corrupt EDP packet: invalid TLV length";
    read_packet(&msg, "proto/edp/93.tlv.long");
    fail_unless (edp_peer(&msg) == 0, "broken packets should return 0");
    fail_unless (strcmp(check_wrap_errstr, errstr) == 0,
	"incorrect message logged: %s", check_wrap_errstr);

    errstr = "check";
    my_log(CRIT, errstr);
    read_packet(&msg, "proto/edp/41.good.small");
    fail_unless (edp_peer(&msg) == msg.len, "packet length incorrect");
    fail_unless (strcmp(check_wrap_errstr, errstr) == 0,
	"incorrect message logged: %s", check_wrap_errstr);
    fail_unless (msg.ttl == 180, "ttl should be 180");
    fail_unless (strcmp(msg.peer, "HD000002") == 0,
		"system name should be 'HD000002'");
    read_packet(&msg, "proto/edp/42.good.medium");
    fail_unless (edp_peer(&msg) == msg.len, "packet length incorrect");
    fail_unless (strcmp(check_wrap_errstr, errstr) == 0,
	"incorrect message logged: %s", check_wrap_errstr);
    fail_unless (msg.ttl == 180, "ttl should be 180");
    fail_unless (strcmp(msg.peer, "SW1") == 0, "system name should be 'SW1'");
}
END_TEST

START_TEST(test_fdp_peer) {
    struct master_msg msg;
    const char *errstr = NULL;

    loglevel = INFO;

    errstr = "missing FDP header";
    read_packet(&msg, "proto/fdp/00.empty");
    fail_unless (fdp_peer(&msg) == 0, "empty packets should return 0");
    fail_unless (strcmp(check_wrap_errstr, errstr) == 0,
	"incorrect message logged: %s", check_wrap_errstr);

    my_log(CRIT, "check");
    read_packet(&msg, "proto/fdp/01.header.broken");
    fail_unless (fdp_peer(&msg) == 0, "broken packets should return 0");
    fail_unless (strcmp(check_wrap_errstr, errstr) == 0,
	"incorrect message logged: %s", check_wrap_errstr);
    errstr = "unsupported FDP version";
    read_packet(&msg, "proto/fdp/02.header.invalid");
    fail_unless (fdp_peer(&msg) == 0, "broken packets should return 0");
    fail_unless (strcmp(check_wrap_errstr, errstr) == 0,
	"incorrect message logged: %s", check_wrap_errstr);
    errstr = "check";
    my_log(CRIT, errstr);
    read_packet(&msg, "proto/fdp/03.header.only");
    fail_unless (fdp_peer(&msg) == msg.len, "packet length incorrect");
    fail_unless (strcmp(check_wrap_errstr, errstr) == 0,
	"incorrect message logged: %s", check_wrap_errstr);

    errstr = "Corrupt FDP packet: invalid Device ID TLV";
    read_packet(&msg, "proto/fdp/21.device_id.broken");
    fail_unless (fdp_peer(&msg) == 0, "broken packets should return 0");
    fail_unless (strcmp(check_wrap_errstr, errstr) == 0,
	"incorrect message logged: %s", check_wrap_errstr);

    errstr = "check";
    my_log(CRIT, errstr);
    read_packet(&msg, "proto/fdp/91.tlv.unknown");
    fail_unless (fdp_peer(&msg) == msg.len, "packet length incorrect");
    fail_unless (strcmp(check_wrap_errstr, errstr) == 0,
	"incorrect message logged: %s", check_wrap_errstr);
    errstr = "Corrupt FDP packet: invalid TLV";
    read_packet(&msg, "proto/fdp/92.tlv.invalid");
    fail_unless (fdp_peer(&msg) == 0, "broken packets should return 0");
    fail_unless (strcmp(check_wrap_errstr, errstr) == 0,
	"incorrect message logged: %s", check_wrap_errstr);
    errstr = "Corrupt FDP packet: invalid TLV length";
    read_packet(&msg, "proto/fdp/93.tlv.long");
    fail_unless (fdp_peer(&msg) == 0, "broken packets should return 0");
    fail_unless (strcmp(check_wrap_errstr, errstr) == 0,
	"incorrect message logged: %s", check_wrap_errstr);

    errstr = "check";
    my_log(CRIT, errstr);
    read_packet(&msg, "proto/fdp/41.good.bi");
    fail_unless (fdp_peer(&msg) == msg.len, "packet length incorrect");
    fail_unless (strcmp(check_wrap_errstr, errstr) == 0,
	"incorrect message logged: %s", check_wrap_errstr);
    fail_unless (msg.ttl == 10, "ttl should be 10");
    fail_unless (strcmp(msg.peer, "doetnix") == 0,
		"system name should be 'doetnix'");
    read_packet(&msg, "proto/fdp/42.good.rx");
    fail_unless (fdp_peer(&msg) == msg.len, "packet length incorrect");
    fail_unless (strcmp(check_wrap_errstr, errstr) == 0,
	"incorrect message logged: %s", check_wrap_errstr);
    fail_unless (msg.ttl == 10, "ttl should be 10");
    fail_unless (strcmp(msg.peer, "erix") == 0,
		"system name should be 'erix'");
    read_packet(&msg, "proto/fdp/43.good.mlx");
    fail_unless (fdp_peer(&msg) == msg.len, "packet length incorrect");
    fail_unless (strcmp(check_wrap_errstr, errstr) == 0,
	"incorrect message logged: %s", check_wrap_errstr);
    fail_unless (msg.ttl == 10, "ttl should be 10");
    fail_unless (strcmp(msg.peer, "emmerix") == 0,
		"system name should be 'emmerix'");
}
END_TEST

Suite * proto_suite (void) {
    Suite *s = suite_create("libproto");
    loglevel = DEBUG;

    // proto_packet test cases
    TCase *tc_packet = tcase_create("proto_packet");
    tcase_add_test(tc_packet, test_proto_packet);
    suite_add_tcase(s, tc_packet);

    // proto_check test cases
    TCase *tc_check = tcase_create("proto_check");
    tcase_add_test(tc_check, test_lldp_check);
    tcase_add_test(tc_check, test_cdp_check);
    tcase_add_test(tc_check, test_edp_check);
    tcase_add_test(tc_check, test_fdp_check);
    tcase_add_test(tc_check, test_ndp_check);
    suite_add_tcase(s, tc_check);

    // proto_peer test cases
    TCase *tc_peer = tcase_create("proto_peer");
    tcase_add_test(tc_peer, test_lldp_peer);
    tcase_add_test(tc_peer, test_cdp_peer);
    tcase_add_test(tc_peer, test_edp_peer);
    tcase_add_test(tc_peer, test_fdp_peer);
    suite_add_tcase(s, tc_peer);

    return s;
}

int main (void) {
    int number_failed;
    Suite *s = proto_suite ();
    SRunner *sr = srunner_create (s);
    srunner_run_all (sr, CK_NORMAL);
    number_failed = srunner_ntests_failed (sr);
    srunner_free (sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
