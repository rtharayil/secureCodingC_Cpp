(gdb) layout src     # Optional: open source view
(gdb) print &turbine
$2 = (WindTurbineController *) 0x7fffffffe2a0  # example address

(gdb) x/80xb &turbine

g++ -g -fno-stack-protector -no-pie -o turbine turbine