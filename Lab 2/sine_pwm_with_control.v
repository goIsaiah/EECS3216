 module sine_pwm_with_control (
    input wire clk,        // 50 MHz system clock
    input wire rst_n,      // Active-low reset
    input wire btn_up,     // Increase frequency
    input wire btn_down,   // Decrease frequency
    output reg pwm_out     // PWM output
);

    parameter COUNTER_MAX = 49999;
    parameter SINE_SAMPLES = 20;

    reg [15:0] SINE_LUT [0:SINE_SAMPLES-1] = 
        {24999, 32999, 39999, 44999, 47499, 47499, 44999, 39999, 
         32999, 24999, 16999, 9999, 4999, 2499, 2499, 4999, 
         9999, 16999, 24999, 32999};

    reg [15:0] counter;
    reg [4:0] sine_index;
    reg [15:0] duty_cycle;
    reg [3:0] frequency_step; // LUT step size for frequency control

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
            sine_index <= (sine_index + frequency_step) % SINE_SAMPLES;
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

    // Push button controls for frequency adjustment
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n)
            frequency_step <= 1;
        else begin
            if (btn_up)   frequency_step <= (frequency_step < 5) ? frequency_step + 1 : 5;
            if (btn_down) frequency_step <= (frequency_step > 1) ? frequency_step - 1 : 1;
        end
    end

endmodule