rm -rf /mnt/share/multirom.zip
cd ../../..
. build/envsetup.sh
lunch 13
make -j6 multirom trampoline
