/*
module sine_pwm_generator (
    input wire clk,        // 50 MHz system clock
    input wire rst_n,      // Active-low reset
    output reg pwm_out     // Modulated PWM output (D1)
);

    parameter COUNTER_MAX = 49999;
    parameter SINE_SAMPLES = 20;

    // Precomputed sine wave LUT mapped to PWM duty cycle values
    reg [15:0] SINE_LUT [0:SINE_SAMPLES-1] = 
        {24999, 32999, 39999, 44999, 47499, 47499, 44999, 39999, 
         32999, 24999, 16999, 9999, 4999, 2499, 2499, 4999, 
         9999, 16999, 24999, 32999};

    reg [15:0] counter;
    reg [4:0] sine_index;   // LUT index
    reg [15:0] duty_cycle;  // Varying duty cycle

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n)
            counter <= 0;
        else if (counter >= COUNTER_MAX)
            counter <= 0;
        else
            counter <= counter + 1;
    end

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n)
            sine_index <= 0;
        else if (counter == 0)
            sine_index <= (sine_index + 1) % SINE_SAMPLES;
    end

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n)
            duty_cycle <= SINE_LUT[0];
        else
            duty_cycle <= SINE_LUT[sine_index];
    end

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n)
            pwm_out <= 0;
        else
            pwm_out <= (counter < duty_cycle) ? 1 : 0;
    end

endmodule
*/

module sine_pwm_generator (
    input wire clk,        // 50 MHz system clock
    input wire rst_n,      // Active-low reset
    output reg pwm_out     // Modulated PWM output (D1)
);

    parameter COUNTER_MAX = 49999;
    parameter SINE_SAMPLES = 20;

    // Precomputed sine wave LUT mapped to PWM duty cycle values
    reg [15:0] SINE_LUT [0:SINE_SAMPLES-1];

    initial begin
        SINE_LUT[0]  = 24999;
        SINE_LUT[1]  = 32999;
        SINE_LUT[2]  = 39999;
        SINE_LUT[3]  = 44999;
        SINE_LUT[4]  = 47499;
        SINE_LUT[5]  = 47499;
        SINE_LUT[6]  = 44999;
        SINE_LUT[7]  = 39999;
        SINE_LUT[8]  = 32999;
        SINE_LUT[9]  = 24999;
        SINE_LUT[10] = 16999;
        SINE_LUT[11] = 9999;
        SINE_LUT[12] = 4999;
        SINE_LUT[13] = 2499;
        SINE_LUT[14] = 2499;
        SINE_LUT[15] = 4999;
        SINE_LUT[16] = 9999;
        SINE_LUT[17] = 16999;
        SINE_LUT[18] = 24999;
        SINE_LUT[19] = 32999;
    end

    reg [15:0] counter;
    reg [4:0] sine_index;   // LUT index
    reg [15:0] duty_cycle;  // Varying duty cycle

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n)
            counter <= 0;
        else if (counter >= COUNTER_MAX)
            counter <= 0;
        else
            counter <= counter + 1;
    end

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n)
            sine_index <= 0;
        else if (counter == 0) begin
            if (sine_index == SINE_SAMPLES - 1)
                sine_index <= 0;
            else
                sine_index <= sine_index + 1;
        end
    end

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n)
            duty_cycle <= SINE_LUT[0];
        else
            duty_cycle <= SINE_LUT[sine_index];
    end

    always @(posedge clk or negedge rst_n) begin
        if (!rst_n)
            pwm_out <= 0;
        else
            pwm_out <= (counter < duty_cycle) ? 1'b1 : 1'b0;
    end

endmodule
