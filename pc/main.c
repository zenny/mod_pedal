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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <unistd.h>
#include "serial.h"
#include "x11.h"

unsigned char states[2] = { 0, 0 };
unsigned char data = 0;
volatile int int_flag = 0;

void sig_handler(int signal)
{
    int_flag = signal;
}

int main(int argc, char *argv[])
{
    int lock_fd, lock;
    pid_t pid, sid;

    if (argc != 2)
    {
	printf("%s: Incorrect number of arguments provided.\n\n", argv[0]);
	printf("Usage: %s device\n", argv[0]);

	return 1;
    }

    /* Make sure only one instance of the program is running */
    lock_fd = open("/var/run/pedald.pid", O_CREAT | O_RDWR, 0666);
    lock = flock(lock_fd, LOCK_EX | LOCK_NB);

    /* Is an instance already running? if so, quit */
    if (lock != 0 && errno == EWOULDBLOCK)
    {
	perror("Could not init");
	return EXIT_FAILURE;
    }

    pid = fork();
    if (pid < 0)
    {
	perror("fork()");
	return EXIT_FAILURE;
	
    } else if (pid > 0) {
	return EXIT_SUCCESS;
    }

    umask(0);
    openlog(argv[0], LOG_NOWAIT | LOG_PID, LOG_USER);

    /* Create a session ID */
    sid = setsid();
    if (sid < 0)
    {
	syslog(LOG_ERR, "Could not set a session ID: %s\n", strerror(errno));
	return EXIT_FAILURE;
    }

    if ((chdir("/")) < 0)
    {
	syslog(LOG_ERR, "Could not chdir to '/': %s\n", strerror(errno));
	return EXIT_FAILURE;
    }

    /* Close std file descriptors */
    syslog(LOG_NOTICE, "Closing standard file descriptors...");
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    close(STDIN_FILENO);

    if (!init_serial(argv[1]))
    {
	syslog(LOG_ERR, "Could not init serial I/O: %s\n", strerror(errno));
	return EXIT_FAILURE;
    }

    if (!init_x11())
    {
	syslog(LOG_ERR, "Could not open a connection to the X server: %s\n", strerror(errno));
	return EXIT_FAILURE;
    }

    syslog(LOG_NOTICE, "Successfully initialised\n");

    while (int_flag != SIGTERM)
    {
	/* Read a byte from the serial port */
	data = read_serial();

	/* Has there been a change in state? If so act upon it */
	if (states[0] != (data & 0x01))
	{
	    /* Update the change */
	    states[0] = (data & 0x01);
	    
	    /* Simulate a keypress */
	    press_key(XK_Control_L, states[0]);
	}

	if (states[1] != (data & 0x02))
	{
	    states[1] = (data & 0x02);
	    press_key(XK_Alt_L, data & 0x02);
	}
    }

    syslog(LOG_NOTICE, "Shutdown...\n");

    /* Unpress both modifiers */
    press_key(XK_Control_L, 0);
    press_key(XK_Alt_L, 0);

    closelog();
    
    return EXIT_SUCCESS;
}
