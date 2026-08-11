# Tracking-Solar-Panel
My first engineering project. This device aims and tracks a solar panel at the sun through the sky. It may not be the best and most efficient work, with the designing and coding.

The code takes the readings from the 4 light dependent resistors (LDRs) and calculates an average over a certain period of time (to reduce noise). The LDRs work in pairs, left and right, top and bottom. If there is an imbalance in the light intensities the paired LDRs are receiving, then the code decides which way the servos should move until the light intensities are similar again.
This design also works in a dark room with a flashlight, however additional code can be added to stop the tracking at night time / in dark environments.
