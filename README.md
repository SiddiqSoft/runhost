# runhost

Copyright &copy; 2021, Siddiq Software LLC

[![CodeQL](https://github.com/SiddiqSoft/runhost/actions/workflows/codeql-analysis.yml/badge.svg)](https://github.com/SiddiqSoft/runhost/actions/workflows/codeql-analysis.yml)
[![Build Status](https://dev.azure.com/siddiqsoft/siddiqsoft/_apis/build/status/SiddiqSoft.runhost?branchName=release%2F1.1.0)](https://dev.azure.com/siddiqsoft/siddiqsoft/_build/latest?definitionId=9&branchName=main)
![](https://img.shields.io/github/v/tag/SiddiqSoft/runhost)
![](https://img.shields.io/nuget/v/SiddiqSoft.runhost)



## Objective
Host for launching windows service for docker on windows server


## Usage

`runhost.exe "name of service"`

```
Attempting to start `name of service`..
Waiting for signal (or hit `q` to quit)..
```

- There is no check for service name validity.
- There is no check for service startup status.
- There is no error checking or recovery.
- The purpose is to allow for handling/intercept of `SIGINT`, `SIGTERM`, `SIGKILL` signals and return code accordingly.

### In `Dockerfile`

`ENTRYPOINT ["runhost.exe", "name of service"]`

<p width="40%" align="right"><b>NOTE</b>
</br>
Windows Server Containers do not support interception of SIGTERM or SIGKILL as of this version.
</p>

## Where to get it

- Build it yourself (current support requires Visual Studio 2019)
- Get the [nuget packages](https://www.nuget.org/packages/SiddiqSoft.runhost/); it will install to your project directory under `runhost` with `x64` and `x86`/`Win32` binaries.
