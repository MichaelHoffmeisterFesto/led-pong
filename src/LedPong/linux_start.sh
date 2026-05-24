#! /bin/bash
echo "Needs to run as sudo!"
chmod 666 /dev/ttyACM0
stty -F /dev/ttyACM0 115200
# ./program.bin --led-multiplexing=0 --led-pwm-bits=11 --led-gpio-mapping=adafruit-hat --led-pixel-mapper="PiPong" -swap
SDL_AUDIODRIVER=alsa AUDIODEV=hw:2,0 ./program.bin --led-multiplexing=0 --led-pwm-bits=11 --led-gpio-mapping=adafruit-hat --led-pixel-mapper="PiPong" -swap
