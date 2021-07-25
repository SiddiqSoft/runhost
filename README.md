# runhost

Copyright &copy; 2021, Siddiq Software LLC

[![CodeQL](https://github.com/SiddiqSoft/runhost/actions/workflows/codeql-analysis.yml/badge.svg)](https://github.com/SiddiqSoft/runhost/actions/workflows/codeql-analysis.yml)
![](https://img.shields.io/github/v/tag/SiddiqSoft/runhost)


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

> NOTE
>
> Windows Server Containers do not support interception of SIGTERM or SIGKILL as of this version.
