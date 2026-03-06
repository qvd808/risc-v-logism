# Create a clock constraint for the 50MHz oscillator on PIN_AF14
# -period 20.0 means 20ns (which is 1 / 50MHz)
create_clock -name CLOCK_50 -period 20.000 [get_ports {CLOCK_50}]

# Automatically derive clock uncertainty (jitter/skew)
derive_clock_uncertainty

# 3. False paths for Human I/O (The "Pro" approach)
# Tells the Fitter not to stress over nanosecond timing for LEDs and Buttons
set_false_path -from * -to [get_ports {LEDR[*]}]
set_false_path -from [get_ports {KEY[*]}] -to *