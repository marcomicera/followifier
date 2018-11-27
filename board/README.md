# Board section

Software that will be running on ESP32 boards (constantly receiving Probe Request frames).

## Setup

##### [Manufacturer documentation](https://esp-idf.readthedocs.io/en/latest/get-started/index.html)
1. [Install a C/C++ toolchain](https://esp-idf.readthedocs.io/en/latest/get-started/index.html#setup-toolchain)
2. [Clone the ESP-IDF repo](https://github.com/espressif/esp-idf)
	*  `git clone https://github.com/espressif/esp-idf.git`
3.  New Windows environment variable `IDF_PATH` pointing at the ESP-IDF directory
4. `python -m pip install --user -r $IDF_PATH/requirements.txt` to install the Python packages required by ESP-IDF

##### Windows driver installation
1. Install [this zip file](https://www.silabs.com/documents/public/software/CP210x_Universal_Windows_Driver.zip) 
2. Connect the ESP32 to the computer
3. `Win + X` → Device Manager → Ports → Ports (COM & LPT) → take note of the port to which the board is connected (e.g., "`COM3`")

##### Eclipse setup
1. Install the Eclipse IDE
	*  First installation: _Eclipse CDT for C/C++_
	*  Eclipse already installed: install the C/C++ language support extension via `Help` → `Install New Software...` → Type the Eclipse version in the `Work with:` field and install the needed toolchain and autotools
2. Follow this [step-by-step guide](https://esp-idf.readthedocs.io/en/latest/get-started/eclipse-setup-windows.html#eclipse-ide-on-windows)
3. Follow this other [step-by-step guide](https://esp-idf.readthedocs.io/en/latest/get-started/eclipse-setup.html#eclipse-build-project)

## Compile and run an hello world example
1. `Win + R` → `mingw32` to run the shell
2. `cd $IDF_PATH` to navigate to the ESP-IDF project root
3. `cd examples/get-started/hello_world/` to navigate to the hello world example
4. `make menuconfig` to launch the ESP32 board's configuration UI.
![ESP32 board's configuration UI](https://docs.espressif.com/projects/esp-idf/en/latest/_images/project-configuration.png)
5. Select `Serial flasher config` → `Default serial port`
6. Configure the serial port to which the ESP32 board is connected to (e.g., "`COM3`")
7. Save and exit
8. `make flash -j 4` to compile
9. `make monitor` to launch the application

###### Flash and launch the app
1. `make app-flash monitor -j 4`
	*  It will automatically rebuild the app if it needs to
