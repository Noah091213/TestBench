#include <modbus/modbus.h>
#include <iostream>
#include <csignal>
#include <unistd.h>  // for close()

modbus_t *ctx = nullptr;
modbus_mapping_t *mb_mapping = nullptr;
int server_socket = -1;
volatile bool run = true;

void signal_handler(int signum) {
    run = false;
}

int main() {
    signal(SIGINT, signal_handler);

    // Listen on all interfaces so UR can connect
    ctx = modbus_new_tcp("192.168.1.54", 502);
    modbus_set_slave(ctx,1);
    if (ctx == nullptr) {
        std::cerr << "Unable to create the libmodbus context\n";
        return -1;
    }

    mb_mapping = modbus_mapping_new(500, 500, 500, 500);
    if (mb_mapping == nullptr) {
        std::cerr << "Failed to allocate Modbus mapping\n";
        modbus_free(ctx);
        return -1;
    }

    server_socket = modbus_tcp_listen(ctx, 1);
    if (server_socket == -1) {
        std::cerr << "Unable to listen TCP socket\n";
        modbus_free(ctx);
        return -1;
    }

    std::cout << "Modbus slave is running on port 502...\n";

    while (run) {
        std::cout << "Waiting for a client to connect...\n";
        if (modbus_tcp_accept(ctx, &server_socket) == -1) {
            std::cerr << "Failed to accept connection\n";
            continue;
        }

        std::cout << "Client connected\n";
        uint8_t query[MODBUS_TCP_MAX_ADU_LENGTH];

        while (run) {
            int rc = modbus_receive(ctx, query);
            if (rc > 0) {
                std::cout << "Received request (" << rc << " bytes)\n";
                std::cout << "Holding register[0] before: " << mb_mapping -> tab_registers[0] << "\n";
                modbus_reply(ctx, query, rc, mb_mapping);
                std::cout << "Holding register[0] after: " << mb_mapping -> tab_registers[0] << "\n";
                std::cout << "Replied to client\n";
            } else if (rc == -1) {
                std::cout << "Client disconnected\n";
                break;
            }
        }
    }

    std::cout << "Shutting down...\n";

    if (server_socket != -1) close(server_socket);
    if (mb_mapping) modbus_mapping_free(mb_mapping);
    if (ctx) modbus_free(ctx);

    return 0;
}


