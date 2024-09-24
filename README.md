Skeleton Communications Application : twsclient, tcp_server
- Connects to TCP Server in order to exchange messages
- Implements a Control Interface (TCP Server) so clients can control the application w/ instructions
- Leverages select() system call to monitor file descriptors in non-blocking manner
- Provides event-logging support by prepending messages with timestamp in log file
- Implements Singleton design pattern for centralized configuration
   
   
   
   
IB Trader Workstation Client : py_twsclient
- Connects to the IB TWS, downloads contract info and subscribes to market data.
- Implements multithreading in order to allow program to execute operations concurrently such as sending and receiving information
- Implements functionality for custom logging with timestamp and thread-specific logs


