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

    while(quit == false){
        time_sleep(1);
        uint16_t tab_reg[32];
        int rc1 = modbus_read_registers(ctx, 130, 1, tab_reg);  // Read Holding Register 128
        int rc2 = modbus_read_registers(ctx, 131, 1, tab_reg);
        int rc3 = modbus_read_registers(ctx, 132, 1, tab_reg);
        int rc4 = modbus_read_registers(ctx, 133, 1, tab_reg);

        if (rc1 == -1) {
            std::cerr << "Read failed: " << modbus_strerror(errno) << "\n";
        } else {
            std::cout << "Register[128] = " << tab_reg[0] << "\n";
        }
    }


    modbus_close(ctx);
    modbus_free(ctx);

    return 0;
}


