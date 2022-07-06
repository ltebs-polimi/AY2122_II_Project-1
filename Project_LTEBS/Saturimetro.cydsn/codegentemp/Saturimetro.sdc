# THIS FILE IS AUTOMATICALLY GENERATED
# Project: C:\Users\Perro\Desktop\AY2122_II_Project-1\Project_LTEBS\Saturimetro.cydsn\Saturimetro.cyprj
# Date: Wed, 06 Jul 2022 16:02:30 GMT
#set_units -time ns
create_clock -name {Clock(routed)} -period 1000000000 -waveform {0 500000000} [list [get_pins {ClockBlock/dclk_1}]]
create_clock -name {CyILO} -period 1000000 -waveform {0 500000} [list [get_pins {ClockBlock/ilo}] [get_pins {ClockBlock/clk_100k}] [get_pins {ClockBlock/clk_1k}] [get_pins {ClockBlock/clk_32k}]]
create_clock -name {CyIMO} -period 333.33333333333331 -waveform {0 166.666666666667} [list [get_pins {ClockBlock/imo}]]
create_clock -name {CyPLL_OUT} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/pllout}]]
create_clock -name {CyMASTER_CLK} -period 41.666666666666664 -waveform {0 20.8333333333333} [list [get_pins {ClockBlock/clk_sync}]]
create_generated_clock -name {CyBUS_CLK} -source [get_pins {ClockBlock/clk_sync}] -edges {1 2 3} [list [get_pins {ClockBlock/clk_bus_glb}]]
create_generated_clock -name {UART_Debug_IntClock} -source [get_pins {ClockBlock/clk_sync}] -edges {1 27 53} -nominal_period 1083.3333333333333 [list [get_pins {ClockBlock/dclk_glb_0}]]
create_generated_clock -name {Clock} -source [get_pins {ClockBlock/clk_sync}] -edges {1 24000001 48000001} [list [get_pins {ClockBlock/dclk_glb_1}]]


# Component constraints for C:\Users\Perro\Desktop\AY2122_II_Project-1\Project_LTEBS\Saturimetro.cydsn\TopDesign\TopDesign.cysch
# Project: C:\Users\Perro\Desktop\AY2122_II_Project-1\Project_LTEBS\Saturimetro.cydsn\Saturimetro.cyprj
# Date: Wed, 06 Jul 2022 16:02:26 GMT
