For most up to date projects : d_tws branch

tcp_server folder : contains external python server that twsclient(C++ skeletal communcations app) connects to


twsclient folder(Skeleton Communications Application - C++) :
  -Create C++ program that connects to TCP Server in order to exchange messages (tccontrolif)
  
  -Implements a Control Interface (TCP Server) so clients can control the application w/ instructions (tcmanager)
  
  -Leverages select() system call to monitor file descriptors in non-blocking manner (tcmanager)
  
  -Provides event-logging support by prepending messages with timestamps in log file (tclogger)
  
  -Client class that connects to external Python and implements functionalities to send and receive messages asynchronously in a non-blocking fashion(tctcpClient)


  py_twsclient server(IB Trader Workstation Client - Python):
    -Create a python program that works as a client app, connects to the IB TWS, downloads contract info, and subscribes to market data(tcapi)
    
    -Implements multithreading in order to allow program to execute operations concurrently such as sending and receiving information(tcmanager)
    
    -Implements functionality for custom logging with timestamp and thread-specific logs(tcutil)

    
  
