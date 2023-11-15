/*
 * Copyright (C) 2015 Niek Linnenbank
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

#include <Types.h>
#include <Macros.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ProcessClient.h>
#include "Renice.h"

Renice::Renice(int argc, char **argv)
    : POSIXApplication(argc, argv)
{
    parser().setDescription("Changes the priority of running processes");
	parser().registerPositional("PRIORITY", "the level that the processes' scheduling priority will be changed to");
    parser().registerPositional("PROCESS_ID", "this processes' scheduling priority will change");
    parser().registerFlag('n', "priority", "alter current priority level");
}

Renice::Result Renice::exec()
{
    if (arguments().get("priority")) {
        pid_t pid = atoi(arguments().get("PROCESS_ID"));
        int priority = atoi(arguments().get("PRIORITY"));

        ProcessClient process;
        ProcessClient::Info info;

        if (process.processInfo(pid, info) != ProcessClient::Success) {
            ERROR("No process of ID '" << pid << "' is found");
            return InvalidArgument;
        }

        if (priority > 5 || priority < 1) {
            ERROR("Failed to set priority for process " << pid);
            return InvalidArgument;
        }

        ProcessCtl(pid, SetPriority, priority);

        printf("process %d set to priority %d, from priority %d\n", pid, priority, info.kernelState.priority);
    }

    return Success;
}

