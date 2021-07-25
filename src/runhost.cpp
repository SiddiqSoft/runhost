/*
	runhost
	Host application configured via command line to handle signals and return code to host.
	https://github.com/siddiqsoft/runhost

	BSD 3-Clause License

	Copyright (c) 2021, Siddiq Software LLC
	All rights reserved.

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions are met:

	1. Redistributions of source code must retain the above copyright notice, this
	   list of conditions and the following disclaimer.

	2. Redistributions in binary form must reproduce the above copyright notice,
	   this list of conditions and the following disclaimer in the documentation
	   and/or other materials provided with the distribution.

	3. Neither the name of the copyright holder nor the names of its
	   contributors may be used to endorse or promote products derived from
	   this software without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
	AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
	IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
	DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
	FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
	DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
	SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
	CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
	OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
	OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 */

#include <signal.h>

#include <iostream>
#include <string>

#include <Windows.h>
#include <winsvc.h>


void SignalHandler(int signalId)
{
	std::cout << " - Signal [" << signalId << "]" << std::endl;
	exit(signalId);
}


BOOL WINAPI OnConsoleHandler(DWORD dwCtrlType)
{
	std::cout << " - OnConsoleHandler:Killed " << dwCtrlType << std::endl;
	exit(SIGTERM);
	return SIGTERM;
}


void ServiceControlShutdown(const char* arg_ServiceName)
{
	SC_HANDLE	  myService = NULL;
	SC_HANDLE	  mySCM = NULL;
	int			   rc = 0;
	SERVICE_STATUS myServiceStatus;

	mySCM = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
	if (mySCM != NULL)
	{
		//	Open the service to control..
		myService = OpenServiceA(mySCM, arg_ServiceName, SC_MANAGER_ALL_ACCESS);
		if (myService != NULL)
		{
			//	Send the service a shutdown event.
			if (ControlService(myService, SERVICE_CONTROL_STOP, &myServiceStatus))
			{
			}
			else
			{
				rc = GetLastError();
			}

			CloseServiceHandle(myService);
		}
		else
		{
			rc = GetLastError();
		}

		CloseServiceHandle(mySCM);
	}
}


void ServiceControlStart(const char* arg_ServiceName)
{
	SC_HANDLE myService = NULL;
	SC_HANDLE mySCM = NULL;
	int		  rc = 0;

	mySCM = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
	if (mySCM != NULL)
	{
		//	Open the service to control..
		myService = OpenServiceA(mySCM, arg_ServiceName, SC_MANAGER_ALL_ACCESS);
		if (myService != NULL)
		{
			//	Send the service a start event.
			if (StartService(myService, NULL, NULL))
			{
			}
			else
			{
				rc = GetLastError();
			}

			CloseServiceHandle(myService);
		}
		else
		{
			rc = GetLastError();
		}

		CloseServiceHandle(mySCM);
	}
}


int main(int argc, char** argv)
{
	// The following should fire under unix/linux.
	signal(SIGABRT, SignalHandler);
	signal(SIGTERM, SignalHandler);
	signal(SIGBREAK, SignalHandler);
	signal(SIGABRT_COMPAT, SignalHandler);
	signal(SIGINT, SignalHandler);
	// Specifically for commanline applications; docker engine on windows should fire here.
	SetConsoleCtrlHandler(OnConsoleHandler, TRUE);

#ifdef _DEBUG
	cout << "argc: " << argc << endl;
	for (int i = 0; i < argc; i++)
	{
		cout << "[" << i << "] - " << argv[i] << endl;
	}
#endif

	if (argc > 1)
	{
		// The first parameter should be the name of the service.
		// It should be provided as "name of service" in quotes in case you have space in the name.
		std::cout << "Attempting to start `" << argv[1] << "`..";
		ServiceControlStart(argv[1]);
		std::cout << endl;
	}

	char x{};

	do
	{
		std::cout << "Waiting for signal (or hit `q` to quit)..";
		x = getchar();
	} while (x != 'q');

	std::cout << endl
		<< "Bye, bye." << std::endl;

	return 0;
}
