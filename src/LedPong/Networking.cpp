#include "Networking.h"
#include <stdio.h>

#if WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <netioapi.h>
#include <stdio.h>
#include <vector>

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")
#else
#endif

void Networking::ReadAdapterInfo()
{
#if WIN32
    IP_ADAPTER_ADDRESSES* pAddresses = nullptr;
    ULONG outBufLen = 15000;

    pAddresses = (IP_ADAPTER_ADDRESSES*)malloc(outBufLen);

    if (GetAdaptersAddresses(AF_UNSPEC, 0, nullptr,
        pAddresses, &outBufLen) == NO_ERROR)
    {
        for (IP_ADAPTER_ADDRESSES* p = pAddresses;
            p != nullptr;
            p = p->Next)
        {
            printf("Name: %ws\n", p->FriendlyName);

            switch (p->IfType)
            {
                case IF_TYPE_ETHERNET_CSMACD:
                    printf("Type: Ethernet\n");
                    break;

                case IF_TYPE_IEEE80211:
                    printf("Type: Wi-Fi\n");
                    break;

                default:
                    printf("Type: Other (%u)\n", p->IfType);
                    break;
            }

            // IPs

            for (auto ua = p->FirstUnicastAddress;
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

                    cout << ip << endl;
                }
            }

            // physical?
            MIB_IF_ROW2 row = {};

            row.InterfaceIndex = p->IfIndex;

            if (GetIfEntry2(&row) == NO_ERROR)
            {
                if (row.InterfaceAndOperStatusFlags.HardwareInterface)
                {
                    printf("Physical interface\n");
                }
            }

            // currently active?
            if (row.InterfaceAndOperStatusFlags.HardwareInterface &&
                p->IfType == IF_TYPE_ETHERNET_CSMACD &&
                row.OperStatus == IfOperStatusUp)
            {
                // Physical Ethernet port with cable connected
                printf("active\n");
            }

            // combined?

            if (row.InterfaceAndOperStatusFlags.HardwareInterface &&
                row.OperStatus == IfOperStatusUp)
            {
                if (p->IfType == IF_TYPE_ETHERNET_CSMACD)
                    printf("Connected Ethernet\n");

                else if (p->IfType == IF_TYPE_IEEE80211)
                    printf("Connected WiFi\n");
            }

        }
    }

    free(pAddresses);
#else
#endif

}
