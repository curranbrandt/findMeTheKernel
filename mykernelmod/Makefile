obj-m += hello.o

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

