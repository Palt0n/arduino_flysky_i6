# arduino_flysky_i6
Arduino code used together with the Flysky i6X Transmitter and Flysky iA6B Receiver



# Joystick to Differential Mapping

# Stop
x=   0, y=   0,l=0,r=0
# Forward
x=   0, y= 255, l= 255, r= 255
# Backward
x=   0, y=-255, l=-255, r=-255
# Rotate Right
x= 255, y=   0, l= 255, r=-255
# Rotate Left
x=-255, y=   0, l=-255, r= 255

# Turn Forward Right
x= 255, y= 255, l= 255, r= 0
# Turn Forward Left
x=-255, y= 255, l= 0, r= 255

# This give the formula
x + y = l
y - x = r

# Not sure if want to include these
# Turn Backward Left
x= 255, y=-255, l=0, r=-255
# Turn Backward Right
x=-255, y=-255, l=-255, r=0


