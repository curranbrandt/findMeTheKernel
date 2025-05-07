# Hello

I'm beginning to learn about kernel modules. I have written a simple module that prints "hello world" and "goodbye" when loaded/unloaded.

### Specs
```
DISTRIB_ID=Ubuntu
DISTRIB_RELEASE=24.04
DISTRIB_CODENAME=noble
DISTRIB_DESCRIPTION="Ubuntu 24.04.2 LTS"
PRETTY_NAME="Ubuntu 24.04.2 LTS"
NAME="Ubuntu"
VERSION_ID="24.04"
VERSION="24.04.2 LTS (Noble Numbat)"
VERSION_CODENAME=noble
ID=ubuntu
ID_LIKE=debian
...
UBUNTU_CODENAME=noble
LOGO=ubuntu-logo
...
6.11.0-25-generic
...

```

### 1. Set up the kernel source

I have to ensure that I have the proper kernel header files and build components...
Packages I need:
 - linux-headers-$(uname -r)
 - linux-kbuild-$(uname -r)
 - build-essential
 - linux-source
Check for correct installation by running:
```
ls /lib/modules/$(uname -r)/build
```
Ensure that it points to `/usr/src/linux-headers-$(uname-r)`

### 2. Start building makefile...
Referencing `hello.c`, here are some important sections and what they mean:
**obj-m:**
```
# obj-m = what obj files to build as *modules* (m = modules)...
# add hello.o to said list, telling the kernel to track it as one of the files to build...
obj-m += hello.o
```

**make and clean:**
```
# Change cwd to kernel source's folder...
#   From there, set M to cwd... M is used in kernel source's makefile
#    to locate the kernel module to compile
#        Compile using the modules target... so kernel source's makefile
#         will locate modules target section, and compile using command/steps
#         specified there
all:
        make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
        make -C /lib/modules/$(shell uname -r)/build M=$(PWD) cleanup
```

**Note**: a lot of how the kernel handles targets, env variables (such as M), and such, is black magic to me atm, but I am planning on diving into it moving forward as I get more comfortable
### 3. Kernel module
We want:
1. initialization function -- this will run every time that the kernel module is loaded into memory
2. exit function -- this will run whenever the kernel module exits
3. module_init(<initialization fxn) -- tells the kernel to save a ptr to the initialization function in `.init.text` section, when it compiles said kernel module. This symbol is resolved when  the module is loaded. Code at the ptr/address is then ran, and freed afterwards. BLUF: saves address, which tells kernel where to locate initialization code for said module
4. module_exit(<exit fxn>) -- tells kernel to save a ptr to the module's exit function in `.exit.text` when it compiles said module. When the module is unloaded, it runs code at stored address.
5. GPL -- tells the kernel to use GPL license -- this gives it access to all kernel facilities under the General Public License. Ensures that the kernel module has the freedom that it needs & is (more or less) unrestricted. 
6. printk -- used to log messages to kernel log buffer, `/dev/kmsg`. This can be read from using dmesg, but we want the priority of the message that we write to be high enough, such that it is written to the console
***NOTE***: if you're not seeing your "printk" messages in the console, ensure that you are using a **tty**, *not a pseudo-terminal*... switch to one in Ubuntu using `Ctrl-Alt-F3`
***NOTE***: printk sends the message to the kernel ring buffer. It doesn't actually interpret & process it properly until it receives a newline. So, if you don't use a `\n` with `printk()`, your messages/strings will still go to the kernel ring buffer, but they will not be processed correctly (to get sent to stdout, or read from dmesg), until a newline appears and they are seen as one complete message... This is a weird kernel thing in which the kernel is receiving data, but doesn't make it available to the user/system until it is able to correctly process it. So when you omit `\n`, you will notice that both strings are printed to the console when you remove the module, but not when you load it. That is because the kernel doesn't process the strings for user-space until it sees a newline in its buffer, or it unloads a module and flushes the buffer as a result. Since the latter occurs when you `rmmod`, the buffer is flushed and processed correctly within the kernel ring buffer, made available to dmesg, and (in our case) written to stdout due to low loglevel (`KERN_EMERG`).

### 3.5 -- Makefile errors
```
       make executes commands in the makefile to update  one  or  more  target
       names,  where name is typically a program.  If no -f option is present,
       make will look for the makefiles GNUmakefile, makefile,  and  Makefile,
       in that order.
```
- This makes me think I can call my makefile "Makefile", "makefile", or "GNUmakefile"...
- however, when I use "makefile", I get this error:
```
make -C /lib/modules/6.11.0-25-generic/build M=/home/<username>/mykernelmod modules
make[1]: Entering directory '/usr/src/linux-headers-6.11.0-25-generic'
warning: the compiler differs from the one used to build the kernel
  The kernel was built by: x86_64-linux-gnu-gcc-13 (Ubuntu 13.3.0-6ubuntu2~24.04) 13.3.0
  You are using:           gcc-13 (Ubuntu 13.3.0-6ubuntu2~24.04) 13.3.0
scripts/Makefile.build:41: /home/<username>/mykernelmod/Makefile: No such file or directory
make[3]: *** No rule to make target '/home/<username>/mykernelmod/Makefile'.  Stop.
make[2]: *** [/usr/src/linux-headers-6.11.0-25-generic/Makefile:1932: /home/<username>/mykernelmod] Error 2
make[1]: *** [Makefile:224: __sub-make] Error 2
make[1]: Leaving directory '/usr/src/linux-headers-6.11.0-25-generic'
make: *** [makefile:10: all] Error 2
```

- When we specify `M=$(PWD)` within our makefile, we are telling the kernel to use `/home/<username>/mykernelmod/` in order to search for a "makefile"
- Looks like the kernel looks specifically for *Makefile* in order to determine how to build the module...
- We could change how the kernel builds modules, and allow it to handle files without the uppercase, or with GNU in front (`makefile` and `GNUmakefile`), but for simplicity's sake, we can just make sure that our makefile in our kernel mod's source directory has a capital 'M'

### 4. Make & run
```
# build 
make
ls -altr

# insert module
sudo insmod hello.ko

# for debugging/checking if message shows up
sudo lsmod | grep hello
dmesg -T | tail -10

# remove module
sudo rmmod hello.ko

```

:)


### Resources:
[Hello World](https://tldp.org/LDP/lkmpg/2.6/html/x121.html)

[printk](https://www.kernel.org/doc/html/next/core-api/printk-basics.html)

