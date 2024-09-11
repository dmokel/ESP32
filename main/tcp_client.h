#ifndef DB_ESP32_TCP_CLIENT_H
#define DB_ESP32_TCP_CLIENT_H

int open_tcp_client(void);
void send_to_all_tcp_servers(const int tcp_servers[], uint8_t data[], uint data_length);

#endif //DB_ESP32_TCP_SERVER_H