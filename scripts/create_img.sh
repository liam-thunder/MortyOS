# ALL CREDITS GOES TO ...
# roscopeco https://roscopeco.com/2013/08/12/creating-a-bootable-hard-disk-image-with-grub2/
# viridis   https://codezen.org/viridis-ng/2012/08/31/preliminary-work-getting-a-grub2-on-a-qemu-disk/

# Create the actual disk image - 20MB
sudo dd if=/dev/zero of=v_disk.img count=20 bs=1048576
 
# Make the partition table, partition and set it bootable.
sudo parted --script v_disk.img mklabel msdos mkpart p ext2 1 20 set 1 boot on
 
# Map the partitions from the image file
sudo kpartx -a v_disk.img
 
# sleep a sec, wait for kpartx to create the device nodes
sudo sleep 1
 
# Make an ext2 filesystem on the first partition.
sudo mkfs.ext2 /dev/mapper/loop0p1
 
# Make the mount-point
#sudo mkdir -p build/tmp/p1
 
# Mount the filesystem via loopback
#sudo mount /dev/mapper/loop0p1 build/tmp/p1
sudo mount /dev/mapper/loop0p1 /mnt/kernel

# Create image file structure
sudo mkdir -p /mnt/kernel/boot/grub

# Copy in the files from the staging directory
#sudo cp -r build/img/* build/tmp/p1
 
# Create a device map for grub
sudo echo "(hd0) /dev/loop0" > tmp_device.map
 
# Use grub2-install to actually install Grub. The options are:
#   * No floppy polling.
#   * Use the device map we generated in the previous step.
#   * Include the basic set of modules we need in the Grub image.
#   * Install grub into the filesystem at our loopback mountpoint.
#     ** change to --boot-directory since --root-directory is not working for mycase 
#   * Install the MBR to the loopback device itself.
sudo grub-install --no-floppy                                                        \
                  --grub-mkdevicemap=tmp_device.map                            \
                  --modules="biosdisk part_msdos ext2 configfile normal multiboot"   \
                  --boot-directory=/mnt/kernel/boot                                 \
                  /dev/loop0                                                         \

# Delete tmp_device.map
sudo rm tmp_device.map

# Unmount the loopback
sudo umount /mnt/kernel
 
# Unmap the image
sudo kpartx -d v_disk.img

# Sleep in case..
sudo sleep 1
 
# hack to make everything owned by the original user, since it will currently be
# owned by root...
# ** whoami instead of who am i
# ** no sudo here
LOGNAME=`whoami | awk '{print $1}'`
LOGGROUP=`groups $LOGNAME | awk '{print $3}'`

sudo chown $LOGNAME:$LOGGROUP -R v_disk.img