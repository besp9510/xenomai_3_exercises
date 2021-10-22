///////////////////////////////////////////////////////////////////////////////
//
// Intertask Communication Example
//
// -------------------------------------------------------------------------- /
//
// Dependencies:
// - N/A
//
// Input Arguments:
// - N/A
//
// Output Arguments:
// - N/A
// 
// -------------------------------------------------------------------------- /
//
// Benjamin Spencer
// ASEN 4018
// Project HEPCATS
// Subsystem: C&DH
// Created: December 20, 2018
// 
///////////////////////////////////////////////////////////////////////////////\

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>

int main(int argc, char* argv[]) {
    // Initialize:
    char buffer[28];
    int fd;

    // Open real-time pipe:
    fd = open("/dev/rtp0",O_RDWR);

    // Check success:
    if (fd < 0 ) {
        printf("ERROR %d",errno);
    }

    // Read pipe:
    if (0< read(fd, buffer, sizeof(buffer))) {
        printf("Message received: %s\n",buffer);
    }

    return 0;
}