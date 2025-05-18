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
        rc = modbus_read_registers(ctx, 130, 1, values); // only need register 130
        if (rc == -1) {
            std::cerr << "Read failed: " << modbus_strerror(errno) << "\n";
            break;
        }

        uint16_t request = values[0];
        std::cout << "Register 130 = " << request << "\n";

        if (request == 1) {
            std::cout << "UR robot requested gripper action\n";

            // Simulate gripper operation
            sleep(1);  // Gripper action...

            // Acknowledge by setting register 130 = 2
            rc = modbus_write_register(ctx, 130, 2);
            if (rc == -1) {
                std::cerr << "Failed to write 2 to register 130: " << modbus_strerror(errno) << "\n";
            } else {
                std::cout << "Acknowledged gripper use with 130 = 2\n";
            }
        }

        sleep(1); // polling delay
    }

    modbus_close(ctx);
    modbus_free(ctx);

    return 0;
}


