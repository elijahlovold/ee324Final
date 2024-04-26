#pragma once

#include "presets.hpp"

namespace uart {
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

    void send_char_UART1(unsigned char message) {
        *((unsigned int *)(UART_BASE_ADDR + 0x30)) = message;
    }

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
};

/*

# This function will send a single character to the UART1 TX buffer
# Assume that the character to send is stored in r1 prior to calling the function
send_char_UART1:
	PUSH {r0}

	LDR r0, =UART1_BASEADDR
	STR r1, [r0, #0x30]

	POP {r0}
	BX lr

# This function will receive a single character from the UART1 RX buffer
# Character received is stored in r1
receive_char_UART1:
	LDR r0, =UART1_BASEADDR

	waitForRX:
		LDR r2, [r0, #0x2C]
		AND r2, r2, #0b10
		CMP r2, #0
		BNE waitForRX

	LDR r1, [r0, #0x30]

	BX lr

# This function will send a null terminated string to the UART1 TX buffer
# Assume that a pointer to the start of the string to send is stored in r1 prior to calling the function
send_string_UART1:
	PUSH {lr}

	LDR r0, =UART1_BASEADDR
	# Counter to keep track of which character I am on in the string
	MOV r2, #0

	send_string:
		# Grab our next character in the string
		LDRB r3, [r1, r2]

		# Check to see if we have hit the null character
		CMP r3, #0
		BEQ end_send_string

		waitForTX:
			LDR r4, [r0, #0x2C]
			AND r4, r4, #0b10000
			CMP r4, #0
			BNE waitForTX

		# If not at end of string, send via UART1 to PC
		PUSH {r1}
		MOV r1, r3
		BL send_char_UART1
		POP {r1}

		# Move to next character
		ADD r2, r2, #1

	B send_string

	end_send_string:

	POP {lr}
	BX lr



*/