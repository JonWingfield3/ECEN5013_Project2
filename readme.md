# ECEN 5013 Project 2: Binary Logger

This project implements a binary logger that uses uart and interrupts to log data to the terminal. It also contains unit testing code to test the implementations of our data.c, circbuf,c and memory.c files.

## Compiling Unit Tests

To compile unit tests, navigate to the top level of the repo, ECEN5013_Project2, and type:
    
    make test TEST_VER=X
    
replacing X with MEM for memory.c testing, DATA for data.c testing, or CIRCBUF for circbuf.c testing.

## Other Info

Code was compiled using Kinetis Design Studio and our microcontroller was flashed using it as well. Compile time switches are used to select architecture you are compiling for. For this project, FRDM is used to indicate the Freescale Freedom board, and the lack thereof means anything else.

There are also compile time switches to enable logging, DEBUG, specifically the binary logger, B_LOGGER, and interrupts, INTERRUPTS.

## File Descriptions

* **binary_logger.c**
  * Implementation of the binary logger. Contains functions to write logs and send data to terminal via UART.
* **circbuf.c**
  * Implementation of the circular buffer. Generalized for easy translation to different data types.
* **data.c**
  * Implementation of data manipulation functions. More optimized versions of standard data manipulation functions specifically for architectures we are using in this project.
* **defines.h**
  * Contains globally used defines. This was used to make changing of global constants or globally used functions easier to handle.
* **log.c**
  * Implementation of basic log functions. Simpler than binary_log.c, but still logs to the terminal via UART.
* **main.c**
  * Execution varies depending on compile time switches.
* **memory.c**
  * Implementation of memory manipulation functions. More optimized versions of standard memory manipulation functions designed specifically for architectures we are using in this project.
* **uart.c**
  * Implementation of UART configuration and exectution.
* **uartbuf.c**
  * Implements UART specific version of circular buffers.
