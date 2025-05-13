#include <modbus/modbus.h>
#include <iostream>
#include <cerrno>
#include <cstring>
#include <unistd.h>

#define HOST "192.168.1.54"
#define PORT 502
//This is the modbus Server
int main() {
    modbus_t* ctx = modbus_new_tcp(HOST, PORT);
    if (ctx == nullptr) {
        std::cerr << "Failed to create Modbus context: " << modbus_strerror(errno) << std::endl;
        return 1;
    }

    modbus_set_debug(ctx, TRUE);

    modbus_mapping_t* mb_mapping = modbus_mapping_new(0, 0, 128, 0);
    if (mb_mapping == nullptr) {
        std::cerr << "Failed to allocate Modbus mapping: " << modbus_strerror(errno) << std::endl;
        modbus_free(ctx);
        return 1;
    }

    for (int i = 0; i < 10; ++i) {
        mb_mapping->tab_registers[i] = i * 10;
    }

    int server_socket = modbus_tcp_listen(ctx, 1);
    if (server_socket == -1) {
        std::cerr << "Failed to listen: " << modbus_strerror(errno) << std::endl;
        modbus_mapping_free(mb_mapping);
        modbus_free(ctx);
        return 1;
    }

    std::cout << "Modbus server listening on " << HOST << ":" << PORT << std::endl;
	
    while (true) {
        int client_socket = modbus_tcp_accept(ctx, &server_socket);
        if (client_socket == -1) {
            std::cerr << "Failed to accept connection: " << modbus_strerror(errno) << std::endl;
            continue;
        }

        std::cout << "Client connected" << std::endl;

        uint8_t query[MODBUS_TCP_MAX_ADU_LENGTH];
        while (true) {
            int rc = modbus_receive(ctx, query);
            if (rc > 0) {
                modbus_reply(ctx, query, rc, mb_mapping);
                std::cout << "Processed client request" << std::endl;
            } else if (rc == -1) {
                std::cerr << "Connection closed or error: " << modbus_strerror(errno) << std::endl;
                break;
            }
        }

        close(client_socket);
        std::cout << "Client disconnected" << std::endl;
    }

    modbus_mapping_free(mb_mapping);
    close(server_socket);
    modbus_free(ctx);
    return 0;
}
