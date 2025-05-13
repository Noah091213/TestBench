#include <modbus/modbus.h>
#include <iostream>
#include <csignal>

modbus_t *ctx = nullptr;
modbus_mapping_t *mb_mapping = nullptr;
int server_socket = -1;
bool run = true;

void signal_handler(int signum) {
    run = false;
}

int main() {
    signal(SIGINT, signal_handler);

    // Create a new TCP Modbus context
    ctx = modbus_new_tcp("192.168.1.100", 502);
    if (ctx == nullptr) {
        std::cerr << "Unable to create the libmodbus context\n";
        return -1;
    }

    // Allocate memory for holding Modbus data
    mb_mapping = modbus_mapping_new(500, 500, 500, 500);
    if (mb_mapping == nullptr) {
        std::cerr << "Failed to allocate Modbus mapping\n";
        modbus_free(ctx);
        return -1;
    }

    // Create a socket to listen for incoming connections
    server_socket = modbus_tcp_listen(ctx, 1);
    if (server_socket == -1) {
        std::cerr << "Unable to listen TCP socket\n";
        modbus_free(ctx);
        return -1;
    }

    std::cout << "Modbus slave is running...\n";

    while (run) {
        int rc = modbus_tcp_accept(ctx, &server_socket);
       	if (rc == -1) {
       		std::cerr << "Failed to accept client connection\n";
       		break;
       	}
       	uint8_t query[MODBUS_TCP_MAX_ADU_LENGTH];
       	while (run) {
       		int rc_recv = modbus_receive(ctx, query);
       		if (rc_recv > 0){
       			modbus_reply(ctx, query, rc_recv, mb_mapping);
       		} else if (rc_recv == -1) {
       			break;
       		}
       	}
    }

    std::cout << "Shutting down...\n";

    // Cleanup
    if (server_socket != -1) close(server_socket);
    if (mb_mapping) modbus_mapping_free(mb_mapping);
    if (ctx) modbus_free(ctx);

    return 0;
}


