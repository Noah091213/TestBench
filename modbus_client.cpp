#include <modbus/modbus.h>
#include <iostream>
//this is the modbus Client




modbus_t *ctx;
modbus_mapping_t *mb_mapping;
bool quit = false;

int main() {
    modbus_t *ctx = modbus_new_tcp("192.168.0.100", 502);  // IP of your UR robot
    modbus_set_debug(ctx,true);
    if (ctx == nullptr) {
        std::cerr << "Unable to allocate libmodbus context\n";
        return -1;
    }
    
    modbus_set_debug(ctx, true);
    
    if (modbus_connect(ctx) == -1) {
    std::cerr << "Connection failed: " << modbus_strerror(errno) << "\n";
    modbus_free(ctx);
    return -1;
    }

    int rc;
    uint16_t values[4];
    while (true) {
        rc = modbus_read_registers(ctx, 130, 4, values);
        if (rc == -1) {
            std::cerr << "Read failed: " << modbus_strerror(errno) << "\n";
            break;
        }

        std::cout << "Read " << rc << " registers starting at 130:\n";
        for (int i = 0; i < rc; ++i) {
            std::cout << "Register " << (130 + i) << " = " << values[i] << "\n";
        }

        // If the robot set register 130 to 1...
        if (values[0] == 1) {
            std::cout << "Massa tells us to open de grippar\n";
            // Acknowledge it by writing 1 to 131
            rc = modbus_write_register(ctx, 130, 2);
            if (rc == -1) {
                std::cerr << "Write failed: " << modbus_strerror(errno) << "\n";
            } else {
                std::cout << "Sent confirmation: 130 = 1\n";
            }
            sleep(2);
        }

        sleep(1);  // Poll once per second
    }

    modbus_close(ctx);
    modbus_free(ctx);

    return 0;
}


