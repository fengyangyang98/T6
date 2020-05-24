# Distributed-Key-Value-Database

![](./asset/logo.jpg)

## How to use

Compile the T6DB.

``` shell
cmake CmakeLists.txt
make
```

Configure node.
```
! The argument name and value are separated by whitespace in the configuration file.
!
! Mode of process, coordinator OR participant
mode coordinator
!
! The address and port the coordinator process is listening on.
! Note that the address and port are separated by character ':'. 
coordinator_info 127.0.0.1:8001
!
! Address and port information of all participants. 
! Three lines specifies three participants' addresses.
participant_info 127.0.0.1:8002 
participant_info 127.0.0.1:8003
participant_info 127.0.0.1:8004
```

Run the program.
```shell
./kvstore2pcsystem --c <filename>.config
```

Run the clients.
```shell
./kvstore2pcsystem
```

You can send the command:
```
SET key value
GET key
DEL key1 key2 ... keyn
```