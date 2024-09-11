#include "sdkconfig.h"
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <errno.h>
#include <netdb.h> // struct addrinfo
#include <arpa/inet.h>
#include "esp_netif.h"
#include "esp_log.h"

#define HOST_IP_ADDR "192.168.31.63"
#define PORT 5600

#define TCP_CLINET_TAG "TCP_CLIENT_SETUP"

int open_tcp_client(void)
{
    char host_ip[] = HOST_IP_ADDR;

    struct sockaddr_in dest_addr;
    inet_pton(AF_INET, host_ip, &dest_addr.sin_addr);
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(PORT);
    int addr_family = AF_INET;
    int ip_protocol = IPPROTO_IP;

    int sock = socket(addr_family, SOCK_STREAM, ip_protocol);
    if (sock < 0)
    {
        // ESP_LOGE(TCP_CLINET_TAG, "Unable to create socket: %S", esp_err_to_name(errno));
        return ESP_FAIL;
    }

    int err = connect(sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    if (err != 0)
    {
        // ESP_LOGE(TCP_CLINET_TAG, "Socket unable to connect: %s", esp_err_to_name(errno));
        return ESP_FAIL;
    }

    return sock;
}

void send_to_all_tcp_servers(const int tcp_servers[], uint8_t data[], uint data_length)
{
    for (int i = 0; i < CONFIG_LWIP_MAX_ACTIVE_TCP; i++)
    {
        if (tcp_servers[i] > 0)
        {
            ESP_LOGD(TCP_CLINET_TAG, "Sending %i bytes", data_length);
            int err = write(tcp_servers[i], data, data_length);
            if (err < 0)
            {
                ESP_LOGE(TCP_CLINET_TAG, "Error occurred during sending: %d", errno);
            }
        }
    }
}