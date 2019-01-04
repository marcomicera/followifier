# Board section

Software that will be running on ESP32 boards (constantly receiving Probe Request frames).

## Setup

### [Manufacturer documentation](https://esp-idf.readthedocs.io/en/latest/get-started/index.html)
1. [Install a C/C++ toolchain](https://esp-idf.readthedocs.io/en/latest/get-started/index.html#setup-toolchain)
2. [Clone the ESP-IDF repo](https://github.com/espressif/esp-idf)
	*  `git clone https://github.com/espressif/esp-idf.git`
3.  New environment variable `IDF_PATH` pointing at the ESP-IDF directory
4. `python -m pip install --user -r $IDF_PATH/requirements.txt` to install the Python packages required by ESP-IDF

### Connect the board
##### Linux
1. If the board is not recognized by the O.S., install [these drivers](https://www.silabs.com/documents/login/software/Linux_3.x.x_4.x.x_VCP_Driver_Source.zip) following the instructions in `CP210x_VCP_Linux_4.x_Release_Notes.txt`
2. Give the current user read and write access the serial port over USB with `sudo usermod -a -G dialout $USER`
3. Run `/dev/tty*` twice, first with the board/dongle unplugged, then with plugged in. The port which appears the second time is the one you need

##### Windows
1. Install [this zip file](https://www.silabs.com/documents/public/software/CP210x_Universal_Windows_Driver.zip)
2. Connect the ESP32 to the computer
3. `Win + X` → Device Manager → Ports → Ports (COM & LPT) → take note of the port to which the board is connected (e.g., "`COM3`")

### Eclipse setup
1. Install the Eclipse IDE
	* First installation: _Eclipse CDT for C/C++_
	* Eclipse already installed: install the C/C++ language support extension via `Help` → `Install New Software...` → Type the Eclipse version in the `Work with:` field and install the needed toolchain and autotools
2. Follow this [step-by-step guide](https://docs.espressif.com/projects/esp-idf/en/latest/get-started/eclipse-setup.html)

## Compile and run an hello world example
1. Open a Unix shell (`Win + R` → `mingw32` on Windows)
2. `cd $IDF_PATH` to navigate to the ESP-IDF project root
3. `cd examples/get-started/hello_world/` to navigate to the hello world example
4. `make menuconfig` to launch the ESP32 board's configuration UI.
![ESP32 board's configuration UI](https://docs.espressif.com/projects/esp-idf/en/latest/_images/project-configuration.png)
5. Select `Serial flasher config` → `Default serial port`
6. Configure the serial port to which the ESP32 board is connected to (e.g., "`COM3`" on Windows or "`/dev/ttyUSB0`" on Linux)
7. Save and exit
8. `make flash -j 4` to compile
9. `make monitor` to launch the application

###### Flash and launch the app
1. `make app-flash monitor -j 4`
	* It will automatically rebuild the app if it needs to
