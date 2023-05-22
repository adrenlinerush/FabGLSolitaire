LIBS = /home/austin/src/FabGL/src
SKETCH = /home/austin/src/FabGLSolitaire/solitaire.ino
UPLOAD_PORT = /dev/ttyACM0
CHIP = esp32
ESP_ROOT=/home/austin/src/esp32
#BUILD_EXTRA_FLAGS = -DBOARD_HAS_PSRAM
include /home/austin/src/makeEspArduino/makeEspArduino.mk
