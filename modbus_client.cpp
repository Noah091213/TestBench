#include <modbus/modbus.h>
#include <iostream>
//this is the modbus Client




modbus_t *ctx;
modbus_mapping_t *mb_mapping;
bool quit = false;

int main() {
    modbus_t *ctx = modbus_new_tcp("192.168.0.100", 502);  // IP of your UR robot
    if (ctx == nullptr) {
        std::cerr << "Unable to allocate libmodbus context\n";
        return -1;
    }

    // Terminal text, som giver os rå Modbus Data
    modbus_set_debug(ctx, false);
    Bool addDebugging = false;
    
    if (modbus_connect(ctx) == -1) {
    std::cerr << "Connection failed: " << modbus_strerror(errno) << "\n";
    modbus_free(ctx);
    return -1;
    }

    int rc;
    uint16_t values[5];

    while (true) {
        rc = modbus_read_registers(ctx, 130, 5, values); // only need register 130
        if (rc == -1) {
            std::cerr << "Read failed: " << modbus_strerror(errno) << "\n";
            break;
        }
        if(addDebugging){
            std::cout << "-------------------------------------"<< "\n";
            std::cout << "Register 130 = " << values[0] << "\n";
            std::cout << "Register 131 = " << values[1] << "\n";
            std::cout << "Register 132 = " << values[2] << "\n";
            std::cout << "Register 133 = " << values[3] << "\n";
            std::cout << "Register 134 = " << values[4] << "\n";
        }
        
        
        
        if ((values[1] == 1 && values[4] == 1) || (values[3] == 1 && values[4] == 1)) {
            rc = modbus_write_register(ctx, 131, 0); //Reset open gripper
            rc = modbus_write_register(ctx, 133, 0); //reset close gripper
            rc = modbus_write_register(ctx, 134, 0); //reset resetvalue
            
        }
        //Close Gripper code:
        if (values[2] == 1) {
            std::cout << "-------------------------------------"<< "\n";
            std::cout << "Massa tells us to close da grippar\n";
            if(addDebugging){
            std::cout << "Register 130 = " << values[0] << "\n";
            std::cout << "Register 131 = " << values[1] << "\n";
            std::cout << "Register 132 = " << values[2] << "\n";
            std::cout << "Register 133 = " << values[3] << "\n";
            std::cout << "Register 134 = " << values[4] << "\n";
            }
            // Simulate gripper operation
            sleep(2);  // Gripper action...

            // Acknowledge by setting register 133 = 1
            rc = modbus_write_register(ctx, 133, 1);
            if (rc == -1) {
                std::cerr << "Failed to write 1 to register 133: " << modbus_strerror(errno) << "\n";
            } else {
                std::cout << "-------------------------------------"<< "\n";
                std::cout << "Successfully Close for Massa and asks for more work\n";
            if(addDebugging){
                std::cout << "Register 130 = " << values[0] << "\n";
                std::cout << "Register 131 = " << values[1] << "\n";
                std::cout << "Register 132 = " << values[2] << "\n";
                std::cout << "Register 133 = " << values[3] << "\n";
                std::cout << "Register 134 = " << values[4] << "\n";
            }
            }
        }



        
        //open gripper code
        if (values[0] == 1) {
            std::cout << "-------------------------------------"<< "\n";
            std::cout << "Massa tells us to open da grippar\n";
        if(addDebugging){
            std::cout << "Register 130 = " << values[0] << "\n";
            std::cout << "Register 131 = " << values[1] << "\n";
            std::cout << "Register 132 = " << values[2] << "\n";
            std::cout << "Register 133 = " << values[3] << "\n";
            std::cout << "Register 134 = " << values[4] << "\n";
        }
            // Simulate gripper operation
            sleep(2);  // Gripper action...

            // Acknowledge by setting register 131 = 1
            rc = modbus_write_register(ctx, 131, 1);
            if (rc == -1) {
                std::cerr << "Failed to write 1 to register 131: " << modbus_strerror(errno) << "\n";
            } else {
                std::cout << "-------------------------------------"<< "\n";
                std::cout << "Successfully Open for Massa and asks for more work\n";
            if(addDebugging){
                std::cout << "Register 130 = " << values[0] << "\n";
                std::cout << "Register 131 = " << values[1] << "\n";
                std::cout << "Register 132 = " << values[2] << "\n";
                std::cout << "Register 133 = " << values[3] << "\n";
                std::cout << "Register 134 = " << values[4] << "\n";
            }
            }
        }

        sleep(2); // polling delay
    }

    modbus_close(ctx);
    modbus_free(ctx);

    return 0;
}


