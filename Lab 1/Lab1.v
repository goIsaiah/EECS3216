/*module Lab1(
	input [0:9] SW,		// switches
	input KEY0,				// top button
	input KEY1,				// bottom (reset) button
	output [0:6] HEX0,	// seven seg
	output [0:6] HEX1,
	output [0:6] HEX2,
	output [0:6] HEX3
);

reg keyPressed = 0;
reg prevNumber;

always @(posedge KEY0) begin	// **NOTE may have to remove posedge
	if(keyPressed == 0) begin	// FIRST_NUM state
		// store numberFromSwitches in prevNumber
		//getBinaryNumber gbn1(SW, prevNumber);
		assign prevNumber = (SW[0]*1)+(SW[1]*2)+(SW[2]*4)+(SW[3]*8)+(SW[4]*16)+(SW[5]*32)+(SW[6]*64)+(SW[7]*128)+(SW[8]*256)+(SW[9]*512);
		
		// output prevNumber to seven seg
		
		keyPressed <= 1;
	end else begin	// NEXT_NUM state
		// calculate prevNumber * numberFromSwitches and store in prevNumber
		assign prevNumber = prevNumber*((SW[0]*1)+(SW[1]*2)+(SW[2]*4)+(SW[3]*8)+(SW[4]*16)+(SW[5]*32)+(SW[6]*64)+(SW[7]*128)+(SW[8]*256)+(SW[9]*512));
		// output prevNumber to seven seg
	end
end

always @(posedge KEY1) begin	// **NOTE may have to remove posedge
	// reset
	keyPressed <= 0;
end

endmodule

module getBinaryNumber(
	input [0:9] SW,
	output binaryNumber
);
	assign binaryNumber = (SW[0]*1)+(SW[1]*2)+(SW[2]*4)+(SW[3]*8)+(SW[4]*16)+(SW[5]*32)+(SW[6]*64)+(SW[7]*128)+(SW[8]*256)+(SW[9]*512);

endmodule
*/

/*

module Lab1(
    input [9:0] SW,       // switches
    input KEY0,           // top button
    input KEY1,           // bottom (reset) button
    output reg [6:0] HEX0, // seven-segment display
    output reg [6:0] HEX1,
    output reg [6:0] HEX2,
    output reg [6:0] HEX3
);

reg keyPressed = 0;
reg [13:0] prevNumber;    // reg to store the previous number, enough to hold a large value

always @(posedge KEY0 or posedge KEY1) begin
    if (KEY1) begin
        // Reset
        keyPressed <= 0;
        prevNumber <= 0;
    end else begin
        if (keyPressed == 0) begin
            // FIRST_NUM state: store number from switches in prevNumber
            prevNumber <= SW[0] + (SW[1] << 1) + (SW[2] << 2) + (SW[3] << 3) + 
                          (SW[4] << 4) + (SW[5] << 5) + (SW[6] << 6) + (SW[7] << 7) + 
                          (SW[8] << 8) + (SW[9] << 9);
            keyPressed <= 1;
        end else begin
            // NEXT_NUM state: multiply prevNumber by the new number from switches
            prevNumber <= prevNumber * (SW[0] + (SW[1] << 1) + (SW[2] << 2) + 
                                        (SW[3] << 3) + (SW[4] << 4) + (SW[5] << 5) + 
                                        (SW[6] << 6) + (SW[7] << 7) + (SW[8] << 8) + 
                                        (SW[9] << 9));
        end
    end
end

// Function to convert binary number to seven-segment display
always @* begin
    case(prevNumber % 10)
        0: HEX0 = 7'b1000000;
        1: HEX0 = 7'b1111001;
        2: HEX0 = 7'b0100100;
        3: HEX0 = 7'b0110000;
        4: HEX0 = 7'b0011001;
        5: HEX0 = 7'b0010010;
        6: HEX0 = 7'b0000010;
        7: HEX0 = 7'b1111000;
        8: HEX0 = 7'b0000000;
        9: HEX0 = 7'b0010000;
        default: HEX0 = 7'b1111111; // default to all segments off
    endcase

    case((prevNumber / 10) % 10)
        0: HEX1 = 7'b1000000;
        1: HEX1 = 7'b1111001;
        2: HEX1 = 7'b0100100;
        3: HEX1 = 7'b0110000;
        4: HEX1 = 7'b0011001;
        5: HEX1 = 7'b0010010;
        6: HEX1 = 7'b0000010;
        7: HEX1 = 7'b1111000;
        8: HEX1 = 7'b0000000;
        9: HEX1 = 7'b0010000;
        default: HEX1 = 7'b1111111; // default to all segments off
    endcase

    case((prevNumber / 100) % 10)
        0: HEX2 = 7'b1000000;
        1: HEX2 = 7'b1111001;
        2: HEX2 = 7'b0100100;
        3: HEX2 = 7'b0110000;
        4: HEX2 = 7'b0011001;
        5: HEX2 = 7'b0010010;
        6: HEX2 = 7'b0000010;
        7: HEX2 = 7'b1111000;
        8: HEX2 = 7'b0000000;
        9: HEX2 = 7'b0010000;
        default: HEX2 = 7'b1111111; // default to all segments off
    endcase

    case((prevNumber / 1000) % 10)
        0: HEX3 = 7'b1000000;
        1: HEX3 = 7'b1111001;
        2: HEX3 = 7'b0100100;
        3: HEX3 = 7'b0110000;
        4: HEX3 = 7'b0011001;
        5: HEX3 = 7'b0010010;
        6: HEX3 = 7'b0000010;
        7: HEX3 = 7'b1111000;
        8: HEX3 = 7'b0000000;
        9: HEX3 = 7'b0010000;
        default: HEX3 = 7'b1111111; // default to all segments off
    endcase
end

endmodule
*/




/*
module Lab1(
    input [9:0] SW,       // switches
    input KEY0,           // top button
    input KEY1,           // bottom (reset) button
    output reg [6:0] HEX0, // seven-segment display
    output reg [6:0] HEX1,
    output reg [6:0] HEX2,
    output reg [6:0] HEX3
);

reg keyPressed;
reg [13:0] prevNumber;    // reg to store the previous number, enough to hold a large value

always @(posedge KEY0) begin
    if (KEY1) begin
        // Reset when KEY1 is pressed
        keyPressed <= 0;
        prevNumber <= 0;
    end else if (KEY0) begin
        if (keyPressed == 0) begin
            // FIRST_NUM state: store number from switches in prevNumber
            prevNumber <= SW[0] + (SW[1] << 1) + (SW[2] << 2) + (SW[3] << 3) + 
                          (SW[4] << 4) + (SW[5] << 5) + (SW[6] << 6) + (SW[7] << 7) + 
                          (SW[8] << 8) + (SW[9] << 9);
            keyPressed <= 1;
        end else begin
            // NEXT_NUM state: multiply prevNumber by the new number from switches
            prevNumber <= prevNumber * (SW[0] + (SW[1] << 1) + (SW[2] << 2) + 
                                        (SW[3] << 3) + (SW[4] << 4) + (SW[5] << 5) + 
                                        (SW[6] << 6) + (SW[7] << 7) + (SW[8] << 8) + 
                                        (SW[9] << 9));
        end
    end
end

// Function to convert binary number to seven-segment display
always @* begin
    // HEX0 for ones place
    case(prevNumber % 10)
        0: HEX0 = 7'b1000000;
        1: HEX0 = 7'b1111001;
        2: HEX0 = 7'b0100100;
        3: HEX0 = 7'b0110000;
        4: HEX0 = 7'b0011001;
        5: HEX0 = 7'b0010010;
        6: HEX0 = 7'b0000010;
        7: HEX0 = 7'b1111000;
        8: HEX0 = 7'b0000000;
        9: HEX0 = 7'b0010000;
        default: HEX0 = 7'b1111111; // default to all segments off
    endcase

    // HEX1 for tens place
    case((prevNumber / 10) % 10)
        0: HEX1 = 7'b1000000;
        1: HEX1 = 7'b1111001;
        2: HEX1 = 7'b0100100;
        3: HEX1 = 7'b0110000;
        4: HEX1 = 7'b0011001;
        5: HEX1 = 7'b0010010;
        6: HEX1 = 7'b0000010;
        7: HEX1 = 7'b1111000;
        8: HEX1 = 7'b0000000;
        9: HEX1 = 7'b0010000;
        default: HEX1 = 7'b1111111; // default to all segments off
    endcase

    // HEX2 for hundreds place
    case((prevNumber / 100) % 10)
        0: HEX2 = 7'b1000000;
        1: HEX2 = 7'b1111001;
        2: HEX2 = 7'b0100100;
        3: HEX2 = 7'b0110000;
        4: HEX2 = 7'b0011001;
        5: HEX2 = 7'b0010010;
        6: HEX2 = 7'b0000010;
        7: HEX2 = 7'b1111000;
        8: HEX2 = 7'b0000000;
        9: HEX2 = 7'b0010000;
        default: HEX2 = 7'b1111111; // default to all segments off
    endcase

    // HEX3 for thousands place
    case((prevNumber / 1000) % 10)
        0: HEX3 = 7'b1000000;
        1: HEX3 = 7'b1111001;
        2: HEX3 = 7'b0100100;
        3: HEX3 = 7'b0110000;
        4: HEX3 = 7'b0011001;
        5: HEX3 = 7'b0010010;
        6: HEX3 = 7'b0000010;
        7: HEX3 = 7'b1111000;
        8: HEX3 = 7'b0000000;
        9: HEX3 = 7'b0010000;
        default: HEX3 = 7'b1111111; // default to all segments off
    endcase
end

endmodule
*/



module Lab1(
    input [9:0] SW, // switches
    input KEY0,     // top button
    input KEY1,     // bottom (reset) button
    output reg [6:0] HEX0, // seven-segment display
    output reg [6:0] HEX1,
    output reg [6:0] HEX2,
    output reg [6:0] HEX3,
	 output reg [6:0] HEX5
);
    reg keyPressed;
    reg [19:0] prevNumber; // reg to store the previous number, enough to hold a large value

    // Initialize registers
    initial begin
        keyPressed = 0;
        prevNumber = 0;
    end

    always @(negedge KEY1 or negedge KEY0) begin
        if (!KEY1) begin
            // Reset when KEY1 is pressed (active-high)
            keyPressed <= 0;
            prevNumber <= 0;
        end else if (!KEY0) begin
            // When KEY0 is pressed (active-high)
            if (keyPressed == 0) begin
                // FIRST_NUM state: store number from switches in prevNumber
                prevNumber <= SW[0] + (SW[1] << 1) + (SW[2] << 2) + (SW[3] << 3) + (SW[4] << 4) + (SW[5] << 5) + (SW[6] << 6) + (SW[7] << 7) + (SW[8] << 8) + (SW[9] << 9);
                keyPressed <= 1;
            end else begin
                // NEXT_NUM state: multiply prevNumber by the new number from switches
                prevNumber <= prevNumber * (SW[0] + (SW[1] << 1) + (SW[2] << 2) + (SW[3] << 3) + (SW[4] << 4) + (SW[5] << 5) + (SW[6] << 6) + (SW[7] << 7) + (SW[8] << 8) + (SW[9] << 9));
            end
        end
    end

    // Function to convert binary number to seven-segment display
    always @* begin
    if(prevNumber > 9999) begin
        HEX5 = 7'b0000110; // Assign the segments for "E" shape
        HEX0 = 7'b1000000; // Display 0 on HEX0
        HEX1 = 7'b1000000; // Display 0 on HEX1
        HEX2 = 7'b1000000; // Display 0 on HEX2
        HEX3 = 7'b1000000; // Display 0 on HEX3
    end else begin
        HEX5 = 7'b1111111; // Turn off HEX5
        // HEX0 for ones place
        case(prevNumber % 10)
            0: HEX0 = 7'b1000000;
            1: HEX0 = 7'b1111001;
            2: HEX0 = 7'b0100100;
            3: HEX0 = 7'b0110000;
            4: HEX0 = 7'b0011001;
            5: HEX0 = 7'b0010010;
            6: HEX0 = 7'b0000010;
            7: HEX0 = 7'b1111000;
            8: HEX0 = 7'b0000000;
            9: HEX0 = 7'b0010000;
            default: HEX0 = 7'b1111111; // default to all segments off
        endcase
        
        // HEX1 for tens place
        case((prevNumber / 10) % 10)
            0: HEX1 = 7'b1000000;
            1: HEX1 = 7'b1111001;
            2: HEX1 = 7'b0100100;
            3: HEX1 = 7'b0110000;
            4: HEX1 = 7'b0011001;
            5: HEX1 = 7'b0010010;
            6: HEX1 = 7'b0000010;
            7: HEX1 = 7'b1111000;
            8: HEX1 = 7'b0000000;
            9: HEX1 = 7'b0010000;
            default: HEX1 = 7'b1111111; // default to all segments off
        endcase
        
        // HEX2 for hundreds place
        case((prevNumber / 100) % 10)
            0: HEX2 = 7'b1000000;
            1: HEX2 = 7'b1111001;
            2: HEX2 = 7'b0100100;
            3: HEX2 = 7'b0110000;
            4: HEX2 = 7'b0011001;
            5: HEX2 = 7'b0010010;
            6: HEX2 = 7'b0000010;
            7: HEX2 = 7'b1111000;
            8: HEX2 = 7'b0000000;
            9: HEX2 = 7'b0010000;
            default: HEX2 = 7'b1111111; // default to all segments off
        endcase
        
        // HEX3 for thousands place
        case((prevNumber / 1000) % 10)
            0: HEX3 = 7'b1000000;
            1: HEX3 = 7'b1111001;
            2: HEX3 = 7'b0100100;
            3: HEX3 = 7'b0110000;
            4: HEX3 = 7'b0011001;
            5: HEX3 = 7'b0010010;
            6: HEX3 = 7'b0000010;
            7: HEX3 = 7'b1111000;
            8: HEX3 = 7'b0000000;
            9: HEX3 = 7'b0010000;
            default: HEX3 = 7'b1111111; // default to all segments off
        endcase
    end
end

endmodule


