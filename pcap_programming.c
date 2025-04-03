#include <stdlib.h>
#include <stdio.h>
#include <pcap.h>
#include <arpa/inet.h>


/* Ethernet header */
typedef struct
{
    u_char ether_dhost[6]; /* destination host address */
    u_char ether_shost[6]; /* source host address */
    u_short ether_type; /* protocol type (IP, ARP, RARP, etc) */
} ethheader;

/* IP Header */
typedef struct
{
    unsigned char iph_ihl : 4, //IP header length
        iph_ver : 4; //IP version
    unsigned char iph_tos; //Type of service
    unsigned short int iph_len; //IP Packet length (data + header)
    unsigned short int iph_ident; //Identification
    unsigned short int iph_flag : 3, //Fragmentation flags
        iph_offset : 13; //Flags offset
    unsigned char iph_ttl; //Time to Live
    unsigned char iph_protocol; //Protocol type
    unsigned short int iph_chksum; //IP datagram checksum
    struct in_addr iph_sourceip; //Source IP address
    struct in_addr iph_destip; //Destination IP address
} ipheader;

/* TCP Header */
typedef struct
{
    u_short tcp_sport; /* source port */
    u_short tcp_dport; /* destination port */
    u_int tcp_seq; /* sequence number */
    u_int tcp_ack; /* acknowledgement number */
    u_char tcp_offx2; /* data offset, rsvd */
#define TH_OFF(th)      (((th)->tcp_offx2 & 0xf0) >> 4)
    u_char tcp_flags;
#define TH_FIN  0x01
#define TH_SYN  0x02
#define TH_RST  0x04
#define TH_PUSH 0x08
#define TH_ACK  0x10
#define TH_URG  0x20
#define TH_ECE  0x40
#define TH_CWR  0x80
#define TH_FLAGS        (TH_FIN|TH_SYN|TH_RST|TH_ACK|TH_URG|TH_ECE|TH_CWR)
    u_short tcp_win; /* window */
    u_short tcp_sum; /* checksum */
    u_short tcp_urp; /* urgent pointer */
} tcpheader;


// int pk_count = 0;
void got_packet(u_char* args, const struct pcap_pkthdr* header,
                const u_char* packet)
{
    ethheader* eth = (ethheader*)packet;
    // printf("%d", pk_count++);

    // Ethernet Header: src mac / dst mac
    // 맥주소가 raw 값이라서 unit을 1byte로 잡고 인덱스를 옮기면서 16진수로 출력하면 나옴 
    printf("Ethernet Header : %02x:%02x:%02x:%02x:%02x:%02x / %02x:%02x:%02x:%02x:%02x:%02x\n",
           eth->ether_shost[0], eth->ether_shost[1], eth->ether_shost[2], eth->ether_shost[3],
           eth->ether_shost[4], eth->ether_shost[5], eth->ether_dhost[0], eth->ether_dhost[1],
           eth->ether_dhost[2], eth->ether_dhost[3], eth->ether_dhost[4], eth->ether_dhost[5]);

    // IP Header: src ip / dst ip
    if (ntohs(eth->ether_type) == 0x0800)
    {
        // 0x0800 is IP type
        // 이더넷 헤더의 위치 + 이더넷 헤더의 길이 = IP헤더의 위치
        ipheader* ip = (ipheader*)(packet + sizeof(ethheader));

        // printf("IP Header : %s / %s\n", inet_ntoa(ip->iph_sourceip), inet_ntoa(ip->iph_destip));
        // 알게된 사실: inet_ntoa 은 내부적으로 static 변수를 사용중이라 
        // 한줄에서 두 번 호출하면 값이 덮어 씌워져서 똑같은 값이 두 번 나옴.
        printf("IP Header : %s", inet_ntoa(ip->iph_sourceip));
        printf(" / %s\n", inet_ntoa(ip->iph_destip));

        // TCP Header: src port / dst port
        // 4를 곱하는 이유는 한 줄이 4바이트이기 때문에.
        // iph_ihl는 IP 헤더 줄의 개수라고 생각하면 됨
        int ip_header_len = ip->iph_ihl * 4;
        tcpheader* tcp = (tcpheader*)((u_char*)ip + ip_header_len);
        printf("TCP Header : %hu / %hu\n", ntohs(tcp->tcp_sport), ntohs(tcp->tcp_dport));

        // Message도 출력하면 좋음.
        // 를 위해. 우선 가변길이인 tcp 전체의 길이를 알아야함.
        // data offset 은 tcp 세그먼트의 첫 줄부터 옵션까지의 줄의 개수라고 생각하면됨.
        int tcp_header_len = ((tcp->tcp_offx2 & 0xF0) >> 4) * 4;
        // IP랑 똑같이 한줄에 4바이트(32비트)라서 4를 곱해줌. 그럼 길이가 됨

        // 겸사 겸사 페이로드(데이터) = 메시지 의 길이도 파악
        int payload_len = ntohs(ip->iph_len) - ip_header_len - tcp_header_len;
        //  IP 패킷의 길이 - IP 헤더의 길이 - tcp 헤더의 길이 = 페이로드 길이
        u_char* message = (u_char*)tcp + tcp_header_len;
        // tcp 헤더 위치에서 tcp 헤더만큼 이동 = payload 시작 위치
        printf("Message : ");
        for (int i = 0; i < payload_len; i++) {
            printf("%c", message[i]);
        }
        printf("\n");
    }
}

int main(int argc, char* argv[])
{
    char errbuf[PCAP_ERRBUF_SIZE];
    struct bpf_program fp;
    char filter_exp[] = "tcp";
    // TCP 만 필터링 하여 캡쳐
    bpf_u_int32 net;
    // 네트워크 인터페이스 이름을 프로그램 실행 인자로 부터 받음
    char** iface = &argv[1];

    // Step 1: Open live pcap session on NIC with name enp0s3
    pcap_t* handle = pcap_open_live(*iface, BUFSIZ, 1, 1000, errbuf);

    // Step 2: Compile filter_exp into BPF psuedo-code
    pcap_compile(handle, &fp, filter_exp, 0, net);
    if (pcap_setfilter(handle, &fp) != 0)
    {
        pcap_perror(handle, "Error:");
        exit(EXIT_FAILURE);
    }

    // Step 3: Capture packets
    pcap_loop(handle, -1, got_packet, NULL);

    pcap_close(handle); //Close the handle
    return 0;
}
