module main (
    input  wire        CLOCK_50, // 50MHz Clock
    output wire [9:0]  LEDR      // All 10 Red LEDs
);
    reg [25:0] counter;

    always @(posedge CLOCK_50) begin
        counter <= counter + 1'b1;
    end

    // Assign the blinky bit to the first LED
    assign LEDR[0] = counter[25];

    // Assign exactly 0 to the remaining 9 LEDs to keep them off
    assign LEDR[9:1] = 9'b000000000; 
endmodule