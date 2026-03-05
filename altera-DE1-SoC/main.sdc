# Create a clock constraint for the 50MHz oscillator on PIN_AF14
# -period 20.0 means 20ns (which is 1 / 50MHz)
create_clock -name CLOCK_50 -period 20.000 [get_ports {CLOCK_50}]

# Automatically derive clock uncertainty (jitter/skew)
derive_clock_uncertainty

# 2. Set the Output Delay for the LEDR bus
# -max: The maximum time allowed for the signal to reach the LED
# -min: The minimum time (to prevent "hold time" violations)
set_output_delay -clock { CLOCK_50 } -max 10.0 [get_ports {LEDR[*]}]
set_output_delay -clock { CLOCK_50 } -min 0.0  [get_ports {LEDR[*]}]