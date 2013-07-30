#!/bin/bash
mkdir MyOpenSimulator
mkdir MyOpenSimulator/bin
cp /Users/ciattim/Qt4.8.4/lib/QtXml.la MyOpenSimulator/bin/.
cp /Users/ciattim/Qt4.8.4/lib/QtNetwork.la MyOpenSimulator/bin/.
cp /Users/ciattim/Qt4.8.4/lib/QtCore.la MyOpenSimulator/bin/.
cp /Users/ciattim/Qt4.8.4/lib/QtGui.la MyOpenSimulator/bin/.
cp ../BIN_libplant/liblibplant.2.1.1.dylib MyOpenSimulator/bin/.
ln -s liblibplant.2.1.1.dylib MyOpenSimulator/bin/liblibplant.2.1.dylib
ln -s liblibplant.2.1.1.dylib MyOpenSimulator/bin/liblibplant.2.dylib
ln -s liblibplant.2.1.1.dylib MyOpenSimulator/bin/liblibplant.dylib
cp -r ../myhome MyOpenSimulator/.
cp ../BIN_MyOpenBagClient/MyOpenSimulator.app/Contents/MacOS/MyOpenSimulator MyOpenSimulator/bin/.
touch MyOpenSimulator/MyOpenSimulator.sh
echo "#!/bin/bash" >> MyOpenSimulator/MyOpenSimulator.sh
echo export DYLD_LIBRARY_PATH="$"PWD/bin >> MyOpenSimulator/MyOpenSimulator.sh
echo cd bin >> MyOpenSimulator/MyOpenSimulator.sh
echo ./MyOpenSimulator >> MyOpenSimulator/MyOpenSimulator.sh
chmod +x MyOpenSimulator/MyOpenSimulator.sh
tar vczf INSTALLER_MyOpenSimulator_MAC_1.tar.gz MyOpenSimulator
