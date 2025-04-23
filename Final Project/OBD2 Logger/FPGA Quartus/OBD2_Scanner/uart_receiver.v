/*
Intermediate function to receive data via UART.
*/

module uart_receiver (
    input wire clk,       // System clock
    input wire rx,        // UART RX pin
    output reg [7:0] received_data, // Received 8-bit data
	 output reg DTR,		// data terminal ready
	 output wire [9:0] byte_count // number of bytes received
);

	wire data_valid; // data terminal ready
	wire [7:0] uart_data;
	//reg [7:0] previous_data;
	//wire [9:0] index;
    // Instantiate the UART RX module
	uart_rx rx_byte(.clk(clk), .rx(rx), .data(uart_data), .data_ready(data_valid),.byte_count(byte_count));

	always @(posedge clk) begin
        if (data_valid) begin
		  DTR <= data_valid;
				//DTR <= (previous_data <= uart_data) ? 0 : 1;
				//previous_data <= uart_data;
            received_data <= uart_data; // Capture received UART data
        end
    end
endmodule
