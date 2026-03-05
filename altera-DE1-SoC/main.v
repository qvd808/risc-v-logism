module main (
    input  wire       CLOCK_50, // 50MHz Clock
    input  wire [0:0] KEY,      // Use KEY[0] on DE1-SoC as a Reset
    output reg  [9:0] LEDR      // All 10 Red LEDs
);
    // 1. Counter to reach 1 second (50,000,000 cycles)
    reg [25:0] tick_counter;
    
    // 2. Logic for the 1-second "Tick"
    always @(posedge CLOCK_50) begin
        if (tick_counter == 26'd49_999_999) begin
            tick_counter <= 26'b0;
        end else begin
            tick_counter <= tick_counter + 1'b1;
        end
    end

    // 3. Shifting Logic
    always @(posedge CLOCK_50) begin
        // Use the KEY[0] button to initialize the pattern
        if (!KEY[0]) begin 
            LEDR <= 10'b0000000001; // Start with the first LED on
        end 
        else if (tick_counter == 26'd49_999_999) begin
            // Shift left: the 0 at the end fills the gap
            // We loop it by taking the highest bit and putting it at the bottom
            LEDR <= {LEDR[8:0], LEDR[9]}; 
        end
    end
endmodule