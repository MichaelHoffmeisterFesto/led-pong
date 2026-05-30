# led-pong

## About

LedPong is an educational arcade game for LED walls

## Compiling

### Compiling in Windows

There is a solution file provided for Visual Studio.

When compiling, the "x86" platform needs to be selected

In project properties/ debugging / environment
* PATH=%PATH%;\$(ProjectDir)..\\SDL2\\lib\\x86$(LocalDebuggerEnvironment)

In project properties/ configuration/ c/c++ / pre-processor
* WIN32;_DEBUG;_CONSOLE;%(PreprocessorDefinitions);Use_SDL_Grafix

In project properties/ configuration/ linker / input
* SDL2.lib;SDL2main.lib;SDL2_ttf.lib;SDL2_mixer.lib;%(AdditionalDependencies)

### Compiling on RPi

There is a different main file for linux (linux_main.cpp). 
SDL is used for sound, but not for graphics at all.
There is a makefile.

```
make
```

No make install used.

This is the typical commandline to execute:

```
sudo ./program.bin --led-multiplexing=0 --led-pwm-bits=11 --led-gpio-mapping=adafruit-hat --led-pixel-mapper="PiPong"
```

This is the typical commandline to debug:

```
sudo gdb --args ./program.bin --led-multiplexing=0 --led-pwm-bits=11 --led-gpio-mapping=adafruit-hat --led-pixel-mapper="PiPong"
```

### Installing on RPi as a service

Create a service file

```
sudo vim /etc/systemd/system/led-pong.service
```

INI contents:

```
[Unit]
Description=LED Pong Game
After=network-online.target
Wants=network-online.target

[Service]
Type=simple
ExecStart=/home/pi/led-pong/led-pong/src/LedPong/linux_start.sh
WorkingDirectory=/home/pi/led-pong/led-pong/src/LedPong
Restart=always
RestartSec=5

# Run as root
User=root
Group=root

[Install]
WantedBy=multi-user.target
```

Prepare the (local) startup script:

```
sudo chmod +x /home/pi/led-pong/led-pong/src/LedPong/linux_start.sh
```

Fill it with contents (the script enables the serial port to be read the hard way). It would also be the right place to set some configuration options.

```
#! /bin/bash
echo "Needs to run as sudo!"
chmod 666 /dev/ttyACM0
stty -F /dev/ttyACM0 115200
SDL_AUDIODRIVER=alsa AUDIODEV=hw:2,0 ./program.bin --led-multiplexing=0 --led-pwm-bits=11 --led-gpio-mapping=adafruit-hat --led-pixel-mapper="PiPong"
```

Enable the service (once?):

```
sudo systemctl daemon-reload
sudo systemctl enable led-pong.service
```

Start/ Stop:

```
sudo systemctl start led-pong.service
sudo systemctl stop led-pong.service
```

Check status:

```
sudo systemctl status led-pong.service
```

View logs:

```
sudo journalctl -u led-pong.service -f
```


### History

For documentation reasons, see here a history of the commands issued on Rasberry Pi

```
history
    1  history
    2  ls -la
    3  cd binpack/
    4  ls -la
    5  rm -Rfv SDL3*
    6  ls -la
    7  cd SDL2-2.32.6/
    8  ls -la
    9  make
   10  make clean
   11  make
   12  make install
   13  sudo make install
   14  cd ..
   15  wget https://github.com/libsdl-org/SDL_mixer/releases/download/release-2.8.1/SDL2_mixer-2.8.1.tar.gz
   16  tar -xvzf SDL2_mixer-2.8.1.tar.gz
   17  cd SDL2_mixer-2.8.1/
   18  ./configure
   19  make
   20  make install
   21  sudo make install
   22  sudo shutdown -r now
   23  sudo vim /boot/config
   24  sudo vim /boot/config.txt
   25  sudo vim /boot/firmware/config.txt
   26  sudo vim /etc/modprobe.d/blacklist-8192cu.conf
   27  sudo vim /usr/share/alsa/alsa.conf
   28  ssh-keygen -t ed25519 -C "michahofft2@googlemail.com"
   29  eval "$(ssh-agent -s)"
   30  ssh-add ~/.ssh/id_ed25519
   31  cta ~/.ssh/id_ed25519
   32  cat ~/.ssh/id_ed25519
   33  cat ~/.ssh/id_ed25519.pub
   34  pwd
   35  mkdir led-pong
   36  cd led-pong/
   37  git clone git@github.com:MichaelHoffmeisterFesto/rpi-rgb-led-matrix.git
   38  git clone git@github.com:MichaelHoffmeisterFesto/led-pong.git
   39  cd rpi-rgb-led-matrix/
   40  make
   41  dmesg
   42  cd ~/binpack/
   43  wget https://file-examples.com/storage/fe32c8d6966839f839df247/2017/11/file_example_MP3_700KB.mp3
   44  ls -la
   45  aplay file_example_MP3_700KB.mp3
   46  apt-get install mpg123
   47  apt get install mpg123
   48  sudo apt install mpg123
   49  mpg123 file_example_MP3_700KB.mp3
   50  cd ~/led-pong/
   51  ls -la
   52  sudo apt-get remove bluez bluez-firmware pi-bluetooth triggerhappy pigpio
   53  sudo update-initramfs -u
   54  history
   55  sudo shutdown -r now
   56  cd led-pong/rpi-rgb-led-matrix/examples-api-use/
   57  ls
   58  udo ./demo -D 0 --led-rows=32 --led-cols=64 --led-slowdown-gpio=4 --led-show-refresh --led-chain=6 --led-multiplexing=0 --led-pwm-bits=11 --led-gpio-mapping=adafruit-hat --led-pixel-mapper="PiPong"
   59  sudo ./demo -D 0 --led-rows=32 --led-cols=64 --led-slowdown-gpio=4 --led-show-refresh --led-chain=6 --led-multiplexing=0 --led-pwm-bits=11 --led-gpio-mapping=adafruit-hat --led-pixel-mapper="PiPong"
   60  cd ~/led-pong/led-pong/src/
   61  ls -la
   62  cd LedPong/
   63  ls -la
   64  ./linux_start.sh
   65  sudo ./linux_start.sh
   66  make
   67  sudo ./linux_start.sh
   68  sudo vinm /boot/firmware/cmdline.txt
   69  sudo vim /boot/firmware/cmdline.txt
   70  history
   71  sudo shutdown -r now
   72  cd ~/led-pong/led-pong/src/LedPong/
   73  sudo ./linux_start.sh
   74  ls -la media/
   75  ls -la media/arcade-arped-145549-shortened.wav
   76  mpg123 media/arcade-arped-145549-shortened.wav
   77  aplay media/arcade-arped-145549-shortened.wav
   78  sudo apt-get install libasound2-dev libpulse-dev
   79  cd ~/binpack/
   80  cd SDL2-2.32.6/
   81  make clean
   82  ./config
   83  ./configure
   84  make
   85  pwd
   86  cd binpack/
   87  cd SDL2-2.32.6/
   88  make
   89  sudo make install
   90  cd ..
   91  cd SDL2_mixer-2.8.1/
   92  make clean
   93  ./configure
   94  make
   95  make install
   96  sudo make install
   97  cd ~/led-pong/led-pong/src/LedPong/
   98  make clean
   99  make
  100  history
  101  sudo ./linux_start.sh
  102  vim linux_main.cpp
  103  ls media/pop-268648-shortened-fade-out.wav
  104  ls -la media/pop-268648-shortened-fade-out.wav
  105  aplay media/pop-268648-shortened-fade-out.wav
  106  sudo ./linux_start.sh
  107  cat /dev/ttyACM0
  108  stty -F /dev/ttyACM0
  109  cat /dev/ttyACM0
  110  dmesg
  111  cat /dev/ttyACM0
  112  sudo ./linux_start.sh
  113  cat /dev/ttyACM0
  114  ps -aux
  115  cat ./linux_start.sh
  116  screen /dev/ttyACM0
  117  sudo apt get install screen
  118  sudo apt install screen
  119  screen /dev/ttyACM0
  120  killall screen
  121  screen /dev/ttyACM0 115200
  122  killall screen
  123  vim linux_main.cpp
  124  vim ~/.vimrc
  125  vim linux_main.cpp
  126  make
  127  ./linux_start.sh
  128  sudo ./linux_start.sh
  129  vim linux_main.cpp
  130  history
  131  cd led-pong/led-pong/src/LedPong/
  132  make
  133  sudo ./linux_start.sh
  134  gdb
  135  gdb --args ./program.bin --led-multiplexing=0 --led-pwm-bits=11 --led-gpio-mapping=adafruit-hat --led-pixel-mapper="PiPong"
  136  make
  137  sudo ./linux_start.sh
  138  gdb --args ./program.bin --led-multiplexing=0 --led-pwm-bits=11 --led-gpio-mapping=adafruit-hat --led-pixel-mapper="PiPong"
  139  sudo gdb --args ./program.bin --led-multiplexing=0 --led-pwm-bits=11 --led-gpio-mapping=adafruit-hat --led-pixel-mapper="PiPong"
  140  make
  141  make clean
  142  make
  143  sudo ./linux_start.sh
  144  sudo shutdown -h now
  145  cd led-pong/led-pong/src/LedPong/
  146  history
  147  sudo gdb --args ./program.bin --led-multiplexing=0 --led-pwm-bits=11 --led-gpio-mapping=adafruit-hat --led-pixel-mapper="PiPong"
  148  cd led-pong/led-pong/src/LedPong/
  149  sudo ./linux_start.sh
  150  vim linux_main.cpp
  151  fg
  152  git status
  153  git add --all
  154  git commit -m "Linux improve"
  155  git config --global user.email "michahofft2@googlemail.com"
  156  git config --global user.name "Michael Hoffmeister"
  157  git commit -m "Linux improve"
  158  git push
  159  git pull
  160  make
  161  git pull
  162  make
  163  sudo ./linux_start.sh
  164  make
  165  sudo ./linux_start.sh
  166  make
  167  history
  168  sudo gdb --args ./program.bin --led-multiplexing=0 --led-pwm-bits=11 --led-gpio-mapping=adafruit-hat --led-pixel-mapper="PiPong"
  169  make
  170  sudo ./linux_start.sh
  171  make
  172  sudo gdb --args ./program.bin --led-multiplexing=0 --led-pwm-bits=11 --led-gpio-mapping=adafruit-hat --led-pixel-mapper="PiPong"
  173  pit pull
  174  git pull
  175  git stash
  176  git pull
  177  make
  178  sudo gdb --args ./program.bin --led-multiplexing=0 --led-pwm-bits=11 --led-gpio-mapping=adafruit-hat --led-pixel-mapper="PiPong"
  179  make clean
  180  make
  181  git stash drop
  182  ls *.cpp
  183  ls T*.cpp
  184  make
  185  make clean
  186  make
  187  rm PacManGame.o
  188  make
  189  make clean
  190  make
  191  sudo ./linux_start.sh
  192  git status
  193  git add --all
  194  git commit -m "Update"
  195  git push
  196  sudo shutdown -h now
  197  history
  198  cd led-pong/led-pong/src/LedPong/
  199  ps -aux
  200  make
  201  sudo ./linux_start.sh
  202  make
  203  sudo ./linux_start.sh
  204  make
  205  sudo ./linux_start.sh
  206  make
  207  sudo ./linux_start.sh
  208  make
  209  sudo ./linux_start.sh
  210  grep -rn *.c* -e "\*1\*"
  211  grep -rn *.c* -e "*1*"
  212  grep -rn *.c* -e "\*1*"
  213  grep -rn *.c* -e "\*1\*"
  214  grep -rn *.c* -e "\*1"
  215  grep -rn *.c* -e "*1"
  216  grep -rn *.c* -e "*"
  217  grep -rn *.c* -e "*1"
  218  grep -rn *.c* -e "*\1"
  219  grep -rn *.c* -e "*1"
  220  make
  221  sudo ./linux_start.sh
  222  make
  223  sudo ./linux_start.sh
  224  ls *.cpp
  225  make
  226  sudo ./linux_start.sh
  227  make
  228  sudo ./linux_start.sh
  229  make
  230  sudo ./linux_start.sh
  231  vim MenuGameBase.cpp
  232  dmesg
  233  history
  234  cd binpack/
  235  ll
  236  ls -la
  237  history
  238  mpg123 file_example_MP3_700KB.mp3
  239  cd ~/led-pong/led-pong/src/LedPong/
  240  ./linux_start.sh
  241  sudo ./linux_start.sh
  242  dmesg
  243  mpg123 ~/binpack/file_example_MP3_700KB.mp3
  244  aplay media/arcade-ui-26-229495-shortened.wav
  245  aplay -l
  246  ls /dev/snd/
  247  aplay --help
  248  aplay -D /dev/snd/by-id/usb-Jieli_Technology_UACDemoV1.0_4150344C3631390E-00  media/arcade-ui-26-229495-shortened.wav
  249  ls /dev/snd/by-id/usb-Jieli_Technology_UACDemoV1.0_4150344C3631390E-00
  250  cat /dev/snd/by-id/usb-Jieli_Technology_UACDemoV1.0_4150344C3631390E-00
  251  aplay -D=/dev/snd/by-id/usb-Jieli_Technology_UACDemoV1.0_4150344C3631390E-00  media/arcade-ui-26-229495-shortened.wav
  252  aplay -D /dev/snd/pcmC2D0p  media/arcade-ui-26-229495-shortened.wav
  253  aplay --list-pcms
  254  aplay -l
  255  aplay -D hw:2,0  media/arcade-ui-26-229495-shortened.wav
  256  ls /dev/
  257  ls /dev/snd/
  258  vim linux_main.cpp
  259  SDL_AUDIODRIVER=alsa AUDIODEV=hw:2,0 sudo ./linux_start.sh
  260  less ./linux_start.sh
  261  SDL_AUDIODRIVER=alsa AUDIODEV=hw:2,0 sudo ./program.bin --led-multiplexing=0 --led-pwm-bits=11 --led-gpio-mapping=adafruit-hat --led-pixel-mapper="PiPong" -swap
  262  SDL_AUDIODRIVER=alsa AUDIODEV=hw:2,0 ./program.bin --led-multiplexing=0 --led-pwm-bits=11 --led-gpio-mapping=adafruit-hat --led-pixel-mapper="PiPong" -swap
  263  sudo SDL_AUDIODRIVER=alsa AUDIODEV=hw:2,0 ./program.bin --led-multiplexing=0 --led-pwm-bits=11 --led-gpio-mapping=adafruit-hat --led-pixel-mapper="PiPong" -swap
  264  amixer
  265  amixer --help
  266  amixer -D hw:2,0 controls
  267  sudo SDL_AUDIODRIVER=alsa AUDIODEV=hw:2,0 ./program.bin --led-multiplexing=0 --led-pwm-bits=11 --led-gpio-mapping=adafruit-hat --led-pixel-mapper="PiPong" -swap
  268  amixer -c2 set Master 100%
  269  amixer -c2 scontrols
  270  amixer -c2 controls
  271  amixer -c2 set "PCM Playback Volume" 100%
  272  amixer -c2 contents
  273  amixer -c2 set 'PCM Playback Volume' 100%
  274  amixer -c2 sget
  275  amixer -c2 sget 'PCM Playback Volume'
  276  amixer -c2 sget 3
  277  amixer -c2 sget 'PCM Playback Volume'
  278  amixer scontrols
  279  amixer sset 'Master' 50%
  280  amixer sset 'PCM Playback Volume' 50%
  281  amixer -c 2 cset 'numid=3' 20
  282  amixer -c 2 cset 'numid=3' 100
  283  amixer -c 2 cset 'numid=3' 1100
  284  amixer -c 2 cset 'numid=3' 100
  285  sudo SDL_AUDIODRIVER=alsa AUDIODEV=hw:2,0 ./program.bin --led-multiplexing=0 --led-pwm-bits=11 --led-gpio-mapping=adafruit-hat --led-pixel-mapper="PiPong" -swap
  286  amixer -c 2 cset 'numid=3' 147
  287  sudo SDL_AUDIODRIVER=alsa AUDIODEV=hw:2,0 ./program.bin --led-multiplexing=0 --led-pwm-bits=11 --led-gpio-mapping=adafruit-hat --led-pixel-mapper="PiPong" -swap
  288  pacmd list-sinks | grep 'latency: [1-9]'
  289  history
  290  aplay -D hw:2,0  media/arcade-ui-26-229495-shortened.wav
  291  aplay -D hw:2,0  media/arcade-ui-26-229495-shortened.wav -vv
  292  cd led-pong/led-pong/src/LedPong/
  293  vim linux_main.cpp
  294  sudo shutdown -h now
  295  history
  296  cd led-pong/led-pong/src/LedPong/
  297  ls -la
  298  ./linux_start.sh
  299  sudo ./linux_start.sh
  300  dmesg
  301  history
  302  sudo SDL_AUDIODRIVER=alsa AUDIODEV=hw:2,0 ./program.bin --led-multiplexing=0 --led-pwm-bits=11 --led-gpio-mapping=adafruit-hat --led-pixel-mapper="PiPong" -swap
  303  vim linux_start.sh
  304  sudo ./linux_start.sh
  305  vim linux_start.sh
  306  sudo ./linux_start.sh
  307  vim linux_start.sh
  308  sudo ./linux_start.sh
  309  vim linux_start.sh
  310  sudo ./linux_start.sh
  311  vim linux_start.sh
  312  sudo ./linux_start.sh
  313  vim linux_start.sh
  314  sudo ./linux_start.sh
  315  vim linux_start.sh
  316  sudo ./linux_start.sh
  317  exit
  318  shutdown -h now
  319  sudo shutdown -h now
  320  history
  321  cd led-pong/led-pong/src/LedPong/
  322  sudo ./linux_start.sh
  323  sudo shutdown -h now
  324  cd led-pong/led-pong/src/LedPong/
  325  sudo ./linux_start.sh
  326  sudo shutdown -h now
```