#include <iostream>
#include <fstream>
#include "hidapi.h"

int main() {
    
    if (hid_init() != 0) {
        std::cerr << "Error: Failed to initialize hidapi." << std::endl;
        return 1;
    }

    const unsigned short targetVendorID = 0x8564; // Replace with VID
    const unsigned short targetProductID = 0x1000; // Replace with PID

    hid_device* device = hid_open(targetVendorID, targetProductID, nullptr);
    if (!device) {
        std::cerr << "Error: Unable to open USB device." << std::endl;
        hid_exit();
        return 1;
    }

    const size_t bufferSize = 64; // Adjust this based on report size
    unsigned char buffer[bufferSize];
    int bytesRead = hid_read(device, buffer, bufferSize);
    if (bytesRead < 0) {
        std::cerr << "Error: Failed to read data from USB device." << std::endl;
    } else {
        // Create and open a buffer file for writing
        std::ofstream outputFile("usb_data.bin", std::ios::binary);
        if (!outputFile.is_open()) {
            std::cerr << "Error: Failed to open buffer file for writing." << std::endl;
        } else {
            // Write the read data to the buffer file
            outputFile.write(reinterpret_cast<const char*>(buffer), bytesRead);
            outputFile.close();
            std::cout << "Data written to buffer file: usb_data.bin" << std::endl;
        }
    }

    hid_close(device);
    hid_exit();

    return 0;
}
