#include <iostream>
#include <fcntl.h>
#include <fstream>
#include <termios.h>
#include <unistd.h>

int main() {
    const char* port_name = "/dev/..."; // Replace with your serial port name
    const speed_t baud_rate = B115200; // Adjust to match your serial device settings

    int serial_fd = open(port_name, O_RDONLY);
    if (serial_fd == -1) {
        std::cerr << "Error: Failed to open serial port." << std::endl;
        return 1;
    }

    struct termios tty;
    if (tcgetattr(serial_fd, &tty) != 0) {
        std::cerr << "Error: Failed to get serial port attributes." << std::endl;
        close(serial_fd);
        return 1;
    }

    cfsetospeed(&tty, baud_rate);
    cfsetispeed(&tty, baud_rate);

    tty.c_cflag |= (CLOCAL | CREAD);
    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;

    if (tcsetattr(serial_fd, TCSANOW, &tty) != 0) {
        std::cerr << "Error: Failed to set serial port attributes." << std::endl;
        close(serial_fd);
        return 1;
    }

    std::ofstream buffer_file("serial_data.txt");
    if (!buffer_file.is_open()) {
        std::cerr << "Error: Failed to open buffer file for writing." << std::endl;
        close(serial_fd);
        return 1;
    }

    const size_t buffer_size = 1024; // Adjust the buffer size as needed
    char buffer[buffer_size];

    while (true) {
        ssize_t bytes_read = read(serial_fd, buffer, buffer_size);
        if (bytes_read > 0) {
            buffer_file.write(buffer, bytes_read);
        }
    }

    buffer_file.close();
    close(serial_fd);

    return 0;
}
