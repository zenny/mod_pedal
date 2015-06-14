/*
 * Copyright (C) 2015  Alister Sanders
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#define _GNU_SOURCE

#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include "serial.h"

static int device_fd;
static struct termios term_new;
static struct termios term_old;

int init_serial(const char* device)
{
    /* Open the device for reading */
    device_fd = open(device, O_RDWR | O_NOCTTY);
    if (device_fd < 0) return 0;

    /* Save the state of the old termios */
    tcgetattr(device_fd, &term_old);

    /* Init the new termios */
    term_new.c_cflag = B9600 | CS8 | CLOCAL | CREAD; /* BAUD: 9600, Char size: 8, Ignore control lines, Read */
    term_new.c_iflag = IGNPAR | IGNCR;               /* Ignore parity, ignore carriage return */
    term_new.c_oflag = 0;                            /* Raw output */
    term_new.c_lflag = ICANON;                       /* Canonical mode */
    cfmakeraw(&term_new);                            /* Raw mode */

    /* Apply the changes */
    tcflush(device_fd, TCIFLUSH);
    tcsetattr(device_fd, TCSANOW, &term_new);

    return 1;
}

unsigned char read_serial()
{
    unsigned char data = 0;
    read(device_fd, &data, 1); /* `read` blocks until input is recieved */

    return data;
}

void end_serial()
{
    /* Restore the termios structure */
    tcsetattr(device_fd, TCSANOW, &term_new);

    close(device_fd);
}
