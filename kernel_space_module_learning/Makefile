obj-m += file01.o 

all: 
	make -C /lib/modules/6.19.14+kali-amd64/build M=$(PWD) modules

clean: 
	make -c /lib/modules/6.19.14+kali-amd64/build M=$(PWD) clean