#!/bin/bash
echo "This would lower the volume!"
# amixer -c 2 set PCM 10%-
amixer -c UACDemoV10 set PCM 10%-
echo "Continue app!"