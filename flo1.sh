cd ../../..
. build/envsetup.sh
lunch 13
make -j6 multirom_zip
cd out/target/product/flo/
mv ./multirom.zip argos.zip
cp ./argos.zip /home/jae/
cp ./argos.zip /mnt/share/

