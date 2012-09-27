#!/tmp/busybox sh

cd /tmp

# remove old log
rm -rf /sdcard/kernel_update.log
# everything is logged into /sdcard/bml_over_mtd.log
exec >> /sdcard/kernel_update.log 2>&1
set -x

export PATH=/tmp:/:/sbin:/system/xbin:/system/bin:$PATH

# Set up busybox symlinks
for i in $(busybox --list)
do
	ln -s busybox /tmp/$i
done

bml_over_mtd dump boot 72 reservoir 4012 old.img
eval $(busybox grep -m 1 -A 1 BOOT_IMAGE_OFFSETS old.img | busybox tail -n 1)
replace_kernel old.img $boot_offset $boot_len $recovery_len zImage new.img
if [ "$?" -eq '0' ]; then
erase_image boot
bml_over_mtd flash boot 72 reservoir 4012 new.img
eval $(grep -m 1 -A 1 BOOT_IMAGE_OFFSETS new.img | tail -n 1)
else
exit 1
fi
