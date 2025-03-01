module pwm_generator (
    input wire clk,       // 50 MHz system clock
    input wire rst_n,     // Active-low reset
    output reg pwm_out    // PWM output (D0)
);

    parameter COUNTER_MAX = 49999; // 1 kHz PWM period
    parameter DUTY_CYCLE = COUNTER_MAX / 2; // 50% duty cycle

    reg [15:0] counter;

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
            pwm_out <= 0;
        else
            pwm_out <= (counter < DUTY_CYCLE) ? 1 : 0;
    end

endmodule