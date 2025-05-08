obj-m += hello.o
obj-m += hello_params.o
obj-m += start_stop.o
# composite module... meaning "build start_stop.o (object file) from start.o and stop.o"
#   ':=' is used to define composite relationship
#   '*-objs' is recognized by kernel build system --> means, "treat start.o and stop.o as parts of start_stop"
start_stop-objs := start.o stop.o


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

