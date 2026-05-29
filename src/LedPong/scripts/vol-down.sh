#!/bin/bash
echo "This would lower the volume!"
amixer -c 2 set PCM 10%-
echo "Continue app!"