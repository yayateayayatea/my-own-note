# linux boot process 

## firmware phase 
1) UEFI firmware starts 
2) firmware reads NVRAM boot entries
3) entry points to an `.efi` executable inside the EFI System Partition `(ESP)`  
**The ESP is:**  
a real disk partition  
fromatted as FAT32  
usually 512MB or 1GB  
flag of bootable 

## UEFI loads EFT executable 
EFI/  
 ├── BOOT/  
 │    └── BOOTX64.EFI  
 ├── arch/  
 │    └── grubx64.efi (just a PE executable file actually)
 └── Microsoft/  
    └──---- Boot/  

### *delusion of /boot/efi*
this is not a real partition of the main disk

it is just an mount point that point to the above **ESP** partition


## GRUB phase 
1) reads its config `/boot/grub/grub.cfg`
2) locates kernel `/boot/vmlinuz-linux`
3) locates initramfs `/boot/initramfs-linux.img`

## kernel phase 
1) kernel loads into memory
2) initramfs is unpacked into temporary rootfs 
3) kernel executes `/init` inside initramfs  
initramfs handles early boot taks: loading drivers, raid/lvm, decrypting disks, finding real root filesystem, switch root
4) initram mount the real `/` 
5) run `switch_root` or `pivot_root`

## userspace start 
1) kernel execute /sbin/init, which run `systemd` 
2) `/home`, `/var`, `/tmp` mount
