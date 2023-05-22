# mu_http

Mu http is a tiny high performance HTTP server written in C++ using boost asio.

This primarily a educational exercise on my part to deepen my understanding of:

* The HTTP protocol
* Network Protcols more generally
* Building Asynchronus using Boost ASIO
* Using C++20 language features for writing low latancy, highly performant code

## Building the Project

To build this project on mac or linux you will need to first install git, cmake, gcc, and the boost libraries.
Assuming these requirements are met you can build the server by running the folloing commands:

 ```bash
 $ git clone https://github.com/xchg-rax-rax/mu_http.git
 $ cd mu_http
 $ ./scripts/configure.sh
 $ ./scripts/build.sh
 ```
 
 The resultant binary will be placed in the `/bin` directory
 
 ## Usage
 
 [To Do]
