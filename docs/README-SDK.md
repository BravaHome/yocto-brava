![ibeeto](artwork/ibeeto-logo.png "ibeeto")


# Middleby Yocto Project Software Development Kit (SDK)

Instructions for the Yocto Project Software Development Kit (SDK).


## History

Version     | Date       | Author             | Description
----------- | ---------- | ------------------ | -------------------------------------------
1.0         | 2021/08/17 | Rudolf J Streif    | Initial Version


## Installation Prerequisites

To use the SDK you need the following items:

* SDK installer e.g. mlui-glibc-x86_64-core-image-mlui-dev-cortexa53-crypto-toolchain-0.0.1.sh


## SDK Installation

Extract the SDK by running:

```
$ ./mlui-glibc-x86_64-core-image-mlui-dev-cortexa53-crypto-toolchain-0.0.1.sh
```

The installer extracts the SDK by default into the directory `/opt/mlui/0.0.1`. You can change the directory if you wish but for the remainder of this document we assume that you installed the SDK into the default.


## SDK Environment Initialization

Before you can use the SDK you have initialize your shell environment for use with the SDK by sourcing the SDK setup script:

```
$ source /opt/mlui_sdk/environment-setup-cortexa53-crypto-middleby-linux
```

You have to source the environment for every new shell you are using.

The script exports a series of environment variables that facilitate the use of the SDK. You can list them with `env | grep mlui`.


## Building Applications with the SDK

Once you have initialized an SDK in a command shell you can use it for software development pretty much like you would do it for building applications for your build host natively. The SDK provides all the necessary tools, also collectively referred to as *toolchain*, for building applications including the C/C++ compiler, the linker, debugger, make, cmake, autotools etc. The SDK is primarily for building C/C++ applications however other tools such as python3 are available too.

### Simple C Application

Let's consider a simple C application that calculates the terms of the Fibonacci series, `fibonacci-app.c`:

```
/* Simple application to calculate the terms of the Fibonacci series
 * (c) 2019 Rudolf J Streif, rudolf.streif@ibeeto.com
 */

#include<stdio.h>

int main()
{
   unsigned int n, first = 0, second = 1, next;

   printf("Enter the number of terms: ");
   scanf("%d",&n);

   printf("First %d terms of Fibonacci series are:\n",n);

   for (unsigned int c = 0 ; c < n ; c++)
   {
      if (c <= 1)
         next = c;
      else
      {
         next = first + second;
         first = second;
         second = next;
      }
      printf("%d ", next);
   }
   printf("\n");

   return 0;
}
```

From within your SDK shell create the directory `fibonacci` in your home directory for the application and create the file `fibonacci-app.c` inside it:

```
$ mkdir ${HOME}/fibonacci
$ cd ${HOME}/fibonacci
$ vi fibonacci-app.c
```

Use any editor you like and copy the code into the file.

Compile the application with:

```
$ ${CC} fibonacci-app.c -g -o fibonacci-app
```

The environment variable `${CC}` points to the GNU C cross compiler of the SDK. Make sure that you always invoke the compiler using `${CC}`. The variable does not just contain the path the the compiler executable but also command line options setting machine, root file system etc.:

```
$ echo ${CC}
aarch64-middleby-linux-gcc -mcpu=cortex-a53 -march=armv8-a+crc+crypto --sysroot=/opt/mlui/0.0.1/sysroots/cortexa53-crypto-middleby-linux
```

The options explained:

* `-mcpu=cortex-a53` - ARM Cortex A53 target processor
* `-march=armv8-a+crc+crypto` - ARMv8a architecture with CRC32 and crypto extensions
* `'sysroot=<sysroot>'` - target sysroot to compile and link against (containing header files and libraries for the target system)

### Using a Makefile

Typically applications consist of more than just one source file. The SDK of course lets you use *make* and *makefiles* to build your application. For the sake of simplicity of these instructions consider the following `Makefile` for the above Fibonacci application:

```
# Makefile for the Fibonacci Application
# (c) 2019 Rudolf J Streif, rudolf.streif@ibeeto.com

# Files
SOURCES = fibonacci-app.c
OBJECTS = $(SOURCES:.c=.o)
EXEC = fibonacci-app

all: $(SOURCES) $(EXEC)

# Define extra flags here
debug: EXTRACFLAGS += -DDEBUG -g
debug: ${SOURCES} ${EXEC}


$(EXEC): $(OBJECTS)
	$(CC) $(CFLAGS) $(EXTRACFLAGS) $^ -o $@

%.o : %.c
	$(CC) $(CFLAGS) $(EXTRACFLAGS) -c $<

install:
	install $(EXEC) $(DESTDIR)/usr/bin

clean:
	rm -rf $(OBJECTS) $(EXEC)
```

If you create `Makefile` in the same directory of your Fibonacci application you can run:

```
$ fibonacci-app]$ make
aarch64-middleby-linux-gcc  -mcpu=cortex-a53 -march=armv8-a+crc+crypto --sysroot=/opt/mlui/0.0.1/sysroots/cortexa53-crypto-middleby-linux  -O2 -pipe -g -feliminate-unused-debug-types   -c fibonacci-app.c
aarch64-middleby-linux-gcc  -mcpu=cortex-a53 -march=armv8-a+crc+crypto --sysroot=/opt/mlui/0.0.1/sysroots/cortexa53-crypto-middleby-linux  -O2 -pipe -g -feliminate-unused-debug-types   fibonacci-app.o -o fibonacci-app
```

It is of course entirely up to you hwo you write your Makefile meeeting the requirements of your application. However, you always need to use the proper environment variables of the SDK to invoke the tools:

* `${CC}` - C compiler
* `${CXX}` - C++ compiler
* `${CPP}` - C preprocessor
* `${AR}` - archiver (for creating libraries)
* `${LD}` - linker
* `${SDKTARGETSYSROOT}` - directory containing the target sysroot
* `${GDB}` - GNU debugger

### Using CMake

Similar to using make and a Makefile using CMake only requires adding a `CMakeLists.txt` file to your source directory:

```
# Makefile for the Fibonacci Application
# (c) 2019 Rudolf J Streif, rudolf.streif@ibeeto.com

cmake_minimum_required(VERSION 3.10)

# set the project name
project(fibonacci-app)

# add the executable
add_executable(fibonacci-app fibonacci-app.c)

# add install
install(TARGETS fibonacci-app DESTINATION /usr/bin)
```

Build the application with the SDK:

```
$ cmake .
$ make
```

CMake automatically uses the environment set up by the SDK.


## Target Deployment

This section only applies to the Extended SDK which includes the SDK tool `devtool`. For the standard SDK use either `sftp` to transfer the application files to the target or copy them to an SD card.

The first step is to add our Fibonacci application to the SDK for deployment:

```
$ devtool add ${HOME}/fibonacci
```

This creates a recipe for the Fibonacci application using CMake based on the CMakeLists.txt file. You can edit the recipe with:

```
$ devtool edit-recipe fibonacci-app
```

>>>
The name of the recipe is not the name of the source directory but the name that *devtool* determines from the CMakeLists.txt file.
>>>

You can now build the application:

```
$ devtool build fibonacci-app
```

>>>
If you have previously built the application in the source directory you need to remove the CMake artifacts otherwise the build will fail.
>>>

To deploy the application to your target the target has to be connected to the network and you need to know its IP address. Run the following command but replace `<target-ip>` with the IP address of your target:

```
$ devtool deploy-target fibonacci-app root@<target-ip>
```

Devtool copies the application package to the target system and installs it on the target system. Log into your target via SSH and run the application on the target:

```
$ ssh root@<target-ip>
root@mlui:~# fibonacci-app
Enter the number of terms: 5
First 5 terms of Fibonacci series are:
0 1 1 2 3
root@mlui:~#
```

To remove the application from the target:

```
$ devtool undeploy-target fibonacci-app root@<target-ip>
```


## Remote ON-Target Debugging

You can debug applications running on the target device remotely with the GNU debugger *gdb*. For these instructions we are using the following components:

* `gdbserver` - server component of gdb to control the application to be debugged also referred to as *the inferior*; gdbserver is already installed on the target system.
* `gdb` - debugger running on the development host system for the target architecture; gdb is part of the SDK.
* `gdbgui` - a graphical user interface frontend for gdb running on the development host; gdbgui uses a web browser for its user interface

`gdbgui` provides an easy-to-use yet powerful graphical frontend for gdb. It can be downloaded and installed from https://www.gdbgui.com.

Follow these step to debug the Fibonacci application:

1. Log into the target system via SSH and start gdbserver with the application:

```
$ $ ssh root@<target-ip>
root@mlui:~# gdbserver localhost:2345 /usr/bin/fibonacci-app
```

The two parameters are:

* `interface` - the interface on which gdpserver expects connections; in this example it is a TCP port specified through `<host>:<port>`; gdpserver ignores `<host>` part; only `<port>` is signficant.
* `program` - the full path to the program to debug

2. From within a shell with the sourced SDK environment start gdbgui:

```
$ gdbgui -g ${GDB}
```

The parameter `-g` tells gdbgui which gdb to use. We are using the gdb of the SDK from the `${GDB}` environment variable. gdbgui launches, starts the debugger and opens its user interface in your default browser.

![alt text](artwork/gdbgui.png "GDBGUI User Interface")

3. Load the source file and connect to the GDB server on the target:

   * In the field under the blue button *Fetch source files* enter the path to the `fibonacci-app.c` file e.g. `/home/<user>/fibonacci-app/fibonacci-app.c` (replace `<user>` with your user name) and press *Enter>* to load the file.

   * Using the down arrow next to the blue button *Load Binary* select *Connect to gdbserver* and enter the target IP address and gdbserver port number in the form of `<target_ip>:<port>` e.g. `192.168.4.175:2345`. The port number must match the port number you used for starting gdbserver on the target in step 1. Press *Enter* to establish the connection. You can now step through the program, set breakpoints, inspect variables etc. from the gdbgui user interface.

>>>
Since Fibonacci is an interactive application expecting the number of terms as a user input the program will block at the `scanf("5d", &n)` instruction until you enter a number in the SSH shell from which you started gdbserver.
>>>

![alt text](artwork/gdbgui-active.png "GDBGUI User Interface")

You can set and clear breakpoints by clicking on the line numbers in the code pane of the user interface.


## Building and Running an Emulated Target

In addition to building images for an actual hardware target the eSDK can also be used to build images for an emulated target. Emulated targets are executed by the Quick Emulator (QEMU). QEMU is a machine emulator that translates the instruction set of the target CPU architecture into the instruction set of the development host architecture, which for the eSDK is x86_64. Potentially, an emulated target has significantly less execution performance than an actual target unless the target architecture is the same as the one QEMU is running on.

Emulation targets can be built directly with *devtool*:

```
$ MACHINE="<machine>" devtool build-image <image>
```

with

* `<machine>` - one of *qemuarm*, *qemuarm64*, *qemumips*, *qemumips64*, *qemuppc*, *qemux86* or *qemux86-64*
* `<image>` - the target image to build, for instance, *core-image-mlui-dev*

While you can build the emulation target for any of the machines listed above it makes most sense to build it either for the same architecture as your hardware target (e.g. *qemuarm64* if your hardware target uses ARMv8) or the architecture of your development host (*qemux86-64* as all Yocto Project development hosts must be x86-64 architecture). QEMU uses KVM when running on its native architecture meaning that a *qemux86-64* target will run at near native speeds.

The emulation target build creates a QEMU configuration file in `tmp/deploy/images/<machine>` called `<image>-<machine>-<timestamp>.qemuboot.conf`, for instance, `tmp/deploy/images/qemuarm64/core-image-mlui-dev-qemuarm64-20210521233139.qemuboot.conf`. The file contains the necessary setting to tell QEMU what kernel image to boot and what root file system to use.

To run you emulation target use

```
$ runqemu tmp/deploy/images/qemuarm64/core-image-mlui-dev-qemuarm64-*.qemuboot.conf
```

The command starts the emulator in a separate window on your development host. You have to enter the root password for your development system as QEMU sets up a TUN/TAP network device that lets you connect from your development system to the emulated system. The TUN/TAP device *tap0* on your development system is assigned the IP address *192.168.7.1*. On your emulated system the IP address *192.168.7.2* is assigned to interface *eth0*.

You can then ssh into your emulated target system as you would into the actual hardware target.