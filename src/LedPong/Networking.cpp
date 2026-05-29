#include "Networking.h"
#include <stdio.h>

#if WIN32
// Windows
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <netioapi.h>
#include <stdio.h>
#include <vector>

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")
#else
// Linux
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>
#endif

void Networking::ReadAdapterInfo()
{
#if WIN32

    //
    // Windows
    //

    IP_ADAPTER_ADDRESSES* pAddresses = nullptr;
    ULONG outBufLen = 15000;

    pAddresses = (IP_ADAPTER_ADDRESSES*)malloc(outBufLen);

    if (GetAdaptersAddresses(AF_UNSPEC, 0, nullptr,
        pAddresses, &outBufLen) == NO_ERROR)
    {
        for (IP_ADAPTER_ADDRESSES* adapter = pAddresses;
             adapter != nullptr;
             adapter = adapter->Next)
        {
            printf("Name: %ws\n", adapter->FriendlyName);

            // find successful IPv4 adress?
            string foundIPv4 = "";

            for (auto ua = adapter->FirstUnicastAddress;
                ua;
                ua = ua->Next)
            {
                SOCKADDR* sa = ua->Address.lpSockaddr;

                if (sa->sa_family == AF_INET)
                {
                    char ip[INET_ADDRSTRLEN];

                    auto* ipv4 =
                        reinterpret_cast<sockaddr_in*>(sa);

                    InetNtopA(AF_INET,
                        &ipv4->sin_addr,
                        ip,
                        sizeof(ip));

                    cout << "IP:" << ip << endl;
                    foundIPv4 = ip;
                }
            }

            if (foundIPv4.length() < 7)
                continue;

            // find physical, up interfaces and differentiate for type
            MIB_IF_ROW2 row = {};
            row.InterfaceIndex = adapter->IfIndex;

            if (GetIfEntry2(&row) == NO_ERROR)
            {
                if (row.InterfaceAndOperStatusFlags.HardwareInterface &&
                    row.OperStatus == IfOperStatusUp)
                {
                    if (adapter->IfType == IF_TYPE_ETHERNET_CSMACD)
                    {
                        printf("Connected Ethernet\n");
                        EthIp = foundIPv4;
                    }

                    else if (adapter->IfType == IF_TYPE_IEEE80211)
                    {
                        printf("Connected WiFi\n");
                        WifiIp = foundIPv4;
                    }
                }
            }

        }
    }

    free(pAddresses);
#else

    //
    // Linux
    //

    int fd;
    struct ifreq ifr;

    fd = socket(AF_INET, SOCK_DGRAM, 0);

    /* I want to get an IPv4 IP address */
    ifr.ifr_addr.sa_family = AF_INET;

    /* I want IP address attached to "eth0" */
    strncpy(ifr.ifr_name, "eth0", IFNAMSIZ - 1);

    ioctl(fd, SIOCGIFADDR, &ifr);

    close(fd);

    /* display result */
    printf("%s\n", inet_ntoa(((struct sockaddr_in*)&ifr.ifr_addr)->sin_addr));

#endif

}
