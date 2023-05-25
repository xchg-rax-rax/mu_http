# Mu HTTP 

Mu http is a tiny high performance HTTP server written in C++ using boost aiso.

This primarily an educational exercise on my part to deepen my understanding of:

* The HTTP protocol
* Network Protcols more generally
* Building Asynchronous using Boost ASIO
* Using C++20 language features for writing low latency, highly performant code

## Building the Project

To build this project on mac or linux you will need to first install git, cmake, gcc, and the boost libraries.
Assuming these requirements are met, you can build the server by running the folloing commands:

 ```bash
 $ git clone https://github.com/xchg-rax-rax/mu_http.git
 $ cd mu_http
 $ ./scripts/configure.sh
 $ ./scripts/build.sh
 ```
 
 The resultant binary will be placed in the `/bin` directory
 
 ## Usage

```bash
Mu HTTP - A tiny high performance HTTP server
Help:
  --help                  Display this Message
  --web-root arg (=/opt/) The web root that the server should use.
  --ip arg (=0.0.0.0)     The IP address on which to listen for connections.
  --port arg (=8080)      The port on which to listen for connections.
  --threads arg (=1)      The number of threads to be used to handle 
                          connections.
```

 ## To Do

* Mime types
* More graceful connection termination (mostly a logging thing)
* Handle inbound message bodies correctly
* Refactor and Optimize
* Testing 
    - Unit Tests
    - Fuzzing
* Encoding(?)

