#!/tmp/busybox sh

export PATH=/tmp:$PATH
# Set up busybox symlinks
for i in $(busybox --list)
do
	ln -s busybox /tmp/$i
done

./dump_image boot boot.img
eval $(grep -m 1 -A 1 BOOT_IMAGE_OFFSETS old.img | tail -n 1)
./replace_kernel old.img $boot_offset $boot_len $recovery_len zImage new.img
if [ "$?" -eq '0' ]; then
./erase_image boot
./bml_over_mtd.sh boot 72 reservoir 4012 ./new.img
else
exit 1
fi
