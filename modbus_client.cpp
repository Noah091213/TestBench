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

    modbus_add_signal("192.168.0.100", 255, 130, 1, "openGripper");
    
    while (true) {
        modbus_get_signal_status("openGripper", False);

        sleep(1);  // Check every 1 second
    }


    modbus_close(ctx);
    modbus_free(ctx);

    return 0;
}


