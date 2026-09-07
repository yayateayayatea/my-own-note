hardware adaptor (the hardware itself)
-> hci (kernel)
-> bluez (kernel driver provide syscall for bluetooth)
-> bluetoothd (bluez daemon)
-> bluetoothctl / our program


1, to see what bluetooth adaptor we have 
hciconfig 


2, to scan bluetooth device around 
hcitool scan 

3, inspect a bluetooth device found 
hcitool info "mac add of device" 

4, pairing and connect
4.1, start bluetoothd (no hci here as doing pin, auth, key gent is not our level now)
systemctl start bluetooth (no d behind)

4.2 pair
bluetoothctl 
power on 
agent on 
pair mac_addr_of_device
connect mac_addr_of_device 

