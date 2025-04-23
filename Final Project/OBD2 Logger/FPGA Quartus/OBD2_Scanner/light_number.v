/*
Converts a 16 bit number into a line on the LEDR
This is used to qualitatively display the tachometer reading.
At a glance the length of the line is proportional to the tachometer reading.
Length of the line increases from left to right.
*/

module light_number(
    input wire [15:0] number,  // Tachometer reading
    input wire clk,            // System clock
    output reg [9:0] LEDR      // LEDR
);

    localparam MAX_TACH = 15'd4000;  // Maximum tachometer speed
    reg [15:0] temp_number;
    integer i;

    always @(posedge clk) begin
        temp_number <= (number > MAX_TACH) ? MAX_TACH : number; // Prevent overflow
        for (i = 0; i < 10; i = i + 1) begin
            LEDR[9-i] <= (temp_number > (i * MAX_TACH / 10)) ? 1 : 0;  
        end
    end
endmodule
