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

//mb_mapping = modbus_mapping_new(0,0,0,0);
//    if (modbus_connect(ctx) == -1) {
//        std::cerr << "Connection failed: " << modbus_strerror(errno) << "\n";
//        modbus_free(ctx);
//        return -1;
//    }
modbus_mapping_t* modbus_mapping_new_start_address(
    0,0    //Coils
    0,0    // Diskret input
    130, 4,    //Holding register, som defineret på UR robotten
    0,0    //Input register
);
    if (mb_mapping == nullptr) {
        std::cerr << "Failed to allocate Modbus mapping: " << modbus_strerror(errno) << "\n";
        modbus_close(ctx);
        modbus_free(ctx);
        return -1;
    }
    
    for (int i = 0; i < 3; ++i) {
        mb_mapping->tab_registers[130 + i] = i;    //Vi laver de tab_registers, som skal give os data frem og tilbage
    }

    

    
    while (true) {
        uint8_t query[MODBUS_TCP_MAX_ADU_LENGTH];
        int rc = modbus_receive(ctx, query);
        if (rc > 0) {
            uint8_t function = query[7];
            uint16_t addr = (query[8] << 8) | query[9];

            if (addr == REG_ADDR) {
                if (function == 0x03) {
                    std::cout << "[READ] Register 130 requested\n";
                } else if (function == 0x06 || function == 0x10) {
                    std::cout << "[WRITE] Register 130 being written\n";
                }
            }

            modbus_reply(ctx, query, rc, mb_mapping);
        } else {
            break;
        }
        sleep(1);  // Check hver 1 sekund
    }


    modbus_close(ctx);
    modbus_free(ctx);

    return 0;
}


