#include <modbus/modbus.h>
#include <iostream>
//this is the modbus Client




modbus_t *ctx;
modbus_mapping_t *mb_mapping;
bool quit = false;

int main() {
    modbus_t *ctx = modbus_new_tcp("0.0.0.0", 502);  // IP of your UR robot
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

    bool quit = false;
    const int start_address = 130;
    const int num_registers = 4;
    uint16_t tab_reg[num_registers];
    uint16_t previous_values[num_registers] = {0};

    while (!quit) {
        sleep(1);
        int rc = modbus_read_registers(ctx, start_address, num_registers, tab_reg);
        if (rc == -1) {
            std::cerr << "Read failed: " << modbus_strerror(errno) << "\n";
            continue;
        }

        for (int i = 0; i < num_registers; ++i) {
            if (tab_reg[i] != previous_values[i]) {
                std::cout << "Register[" << start_address + i << "] changed: " 
                          << previous_values[i] << " -> " << tab_reg[i] << "\n";
                previous_values[i] = tab_reg[i];
            }
        }
    }


    modbus_close(ctx);
    modbus_free(ctx);

    return 0;
}


