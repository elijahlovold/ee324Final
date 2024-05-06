#pragma once

#include "presets.hpp"

// functions for controlling uart1 -> USB port
namespace uart {
	// set the uart
    void setup_UART1() {
        // reset the UART controller   
        *((unsigned int *)(UART_BASE_ADDR)) = 0b1101011;       

        // Set the BAUDGEN clock divider (115200 baud rate, 124 divider value)
        // # 100MHz / 124 = 806.451kHz
        *((unsigned int *)(UART_BASE_ADDR + 0x18)) = 124;       

        // # Set the Baud_Rate_Divider for bit-clock (115200 baud rate, 6 divider value)
        // # 806.451kHz / (6 + 1) = 115.207kHz, 115207 baud rate
        *((unsigned int *)(UART_BASE_ADDR + 0x34)) = 6;       

        // # Enable software resets, transmitter & receiver, stop transmitter break
        *((unsigned int *)(UART_BASE_ADDR)) = 0b100010111;

        // # Channel mode to normal, 1 stop bit, no parity, character length 8-bit, clock source ref. clk
        *((unsigned int *)(UART_BASE_ADDR + 0x4)) = 0b0000100000;

        // # Set timeout to 1 (RXTOUT register)
        *((unsigned int *)(UART_BASE_ADDR + 0x1C)) = 1;

        // # Disable all interrupts (IDR register)
        *((unsigned int *)(UART_BASE_ADDR + 0xC)) = 0xFFF;
    }

	// send a char
    void send_char_UART1(unsigned char message) {
        *((unsigned int *)(UART_BASE_ADDR + 0x30)) = message;
    }

	// blocking char read
	unsigned char read_char_UART1() {
		// poll the FIFO until data arrives	
		unsigned int data_check;
		do {
			// FIFO recieve buffer empty status, bit 1 equals 1 when empty
			data_check = *((unsigned int *)(UART_BASE_ADDR + 0x2C));
			data_check &= 0b10;
			usleep(100);
		} while (data_check != 0);

		return *((unsigned int *)(UART_BASE_ADDR + 0x30));
	}

	// ps4 formatted cmd to write
	void ps4_write(RGB color, bool game_state = true) {
		// send color 1
		send_char_UART1(color.R);
		send_char_UART1(color.G);
		send_char_UART1(color.B);

		// send over game state
		send_char_UART1(static_cast<unsigned int>(game_state));

		// send zeros
		send_char_UART1(0);
		send_char_UART1(0);
		send_char_UART1(0);
		send_char_UART1(0);
	}
	
	// read ps4 command
	unsigned char ps4_read() {
		return read_char_UART1(); 
	}
	
	// old protocol for transferring data in one go...
	unsigned char ps4_transfer(RGB color, bool game_state = true) {
		// first, send over colors
		send_char_UART1(color.R);
		send_char_UART1(color.G);
		send_char_UART1(color.B);

		// send over game state
		send_char_UART1(static_cast<unsigned int>(game_state));

		// zero fill the rest
		for (int i = 0; i < 4; i++) {
			send_char_UART1(56);
		}

		// next, read back controller info byte
		unsigned char response1 = read_char_UART1(); 
		unsigned char response2 = read_char_UART1(); 
		
		return response1;
	}
};