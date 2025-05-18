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

mb_mapping = modbus_mapping_new(1,0,0,0);
    if (modbus_connect(ctx) == -1) {
        std::cerr << "Connection failed: " << modbus_strerror(errno) << "\n";
        modbus_free(ctx);
        return -1;
    }

    uint16_t value130 = 0;

    while (true) {
        std::cout << "Listening ACTIVE";
        int rc = modbus_read_registers(ctx, 130, 1, &value130);
        if (rc == -1) {
            std::cerr << "Failed to read register 130: " << modbus_strerror(errno) << "\n";
            sleep(1);
            continue;
        }

        if (value130 == 1) {
            std::cout << "Register 130 is 1 — triggering action...\n";

            // Set register 130 back to 0
            if (modbus_write_register(ctx, 130, 0) == -1) {
                std::cerr << "Failed to write 0 to register 130: " << modbus_strerror(errno) << "\n";
            } else {
                std::cout << "Register 130 set to 0\n";
            }

            // Set register 131 to 1
            if (modbus_write_register(ctx, 131, 1) == -1) {
                std::cerr << "Failed to write 1 to register 131: " << modbus_strerror(errno) << "\n";
            } else {
                std::cout << "Register 131 set to 1\n";
            }
        }

        sleep(1);  // Check every 1 second
    }


    modbus_close(ctx);
    modbus_free(ctx);

    return 0;
}


