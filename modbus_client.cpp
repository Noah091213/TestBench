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

    
    while(true){
    int rc = modbus_read_registers(ctx, 130, 4, values);
    
    if (rc == -1) {
        std::cerr << "Read failed: " << modbus_strerror(errno) << "\n";
    } else {
        std::cout << "Read " << rc << " registers starting at 130:\n";
        for (int i = 0; i < rc; ++i) {
            std::cout << "Register " << (130 + i) << " = " << values[i] << "\n";
        }
    }
        
    if (values[0] == 1) {
        std::cout << "Massa tells us to open de grippar";
            rc = modbus_write_register(ctx, 131, 1);
        if (rc == -1) {
            std::cerr << "Write failed: " << modbus_strerror(errno) << "\n";
        } else {
            sleep(2);
            while(values[0] == 0){
                if (values[1] == 1) {
                    rc = modbus_write_register(ctx, 131, 0);
                    if (rc == -1) {
                    std::cerr << "Write failed: " << modbus_strerror(errno) << "\n";
                    } else {
                    std::cout << "Successfully told Massa Yes VALUE:" << values[0] << "\n"; 
                }
                }
                sleep(1);
            }
                if (rc == -1) {
                std::cerr << "Write failed: " << modbus_strerror(errno) << "\n";
                } else {
                    std::cout << "reset confirmation (set 131 = 0)";
                }
        }
    }


    sleep(1);
        
        
    }


    modbus_close(ctx);
    modbus_free(ctx);

    return 0;
}


