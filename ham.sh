rm -rf /mnt/share/multirom.zip
cd ../../..
. build/envsetup.sh
lunch 9
make -j6 multirom trampoline
make -j6 multirom_zip
cd out/target/product/hammerhead/
cp ./multirom.zip /mnt/share/
~                               
