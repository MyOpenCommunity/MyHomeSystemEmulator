#!/bin/bash

##  Makes packaging folder structure
rm -fr ./MyOpenSimulator
mkdir MyOpenSimulator
mkdir MyOpenSimulator/bin

##  Copy QT Stuff
cp /Users/ciattim/Qt4.8.4/lib/QtXml.framework/Versions/4/QtXml MyOpenSimulator/bin/.
cp /Users/ciattim/Qt4.8.4/lib/QtNetwork.framework/Versions/4/QtNetwork MyOpenSimulator/bin/.
cp /Users/ciattim/Qt4.8.4/lib/QtCore.framework/Versions/4/QtCore MyOpenSimulator/bin/.
cp /Users/ciattim/Qt4.8.4/lib/QtGui.framework/Versions/4/QtGui MyOpenSimulator/bin/.
cp -r  /Users/ciattim/Qt4.8.4/lib/QtGui.framework/Versions/4/Resources/qt_menu.nib MyOpenSimulator/bin/.

##  Copy simulator stuff
cp ../BIN_libplant/liblibplant.2.1.1.dylib MyOpenSimulator/bin/.
ln -s liblibplant.2.1.1.dylib MyOpenSimulator/bin/liblibplant.2.1.dylib
ln -s liblibplant.2.1.1.dylib MyOpenSimulator/bin/liblibplant.2.dylib
ln -s liblibplant.2.1.1.dylib MyOpenSimulator/bin/liblibplant.dylib
cp -r ../myhome MyOpenSimulator/.
cp ../BIN_MyOpenBagClient/MyOpenSimulator.app/Contents/MacOS/MyOpenSimulator MyOpenSimulator/bin/.

##  Create simulator sturtup script
touch MyOpenSimulator/MyOpenSimulator.sh
echo "#!/bin/bash" >> MyOpenSimulator/MyOpenSimulator.sh
echo export DYLD_LIBRARY_PATH="$"PWD/bin >> MyOpenSimulator/MyOpenSimulator.sh
echo cd bin >> MyOpenSimulator/MyOpenSimulator.sh
echo ./MyOpenSimulator >> MyOpenSimulator/MyOpenSimulator.sh
chmod +x MyOpenSimulator/MyOpenSimulator.sh

##  Change dyn library path
install_name_tool -change "/Users/ciattim/Qt4.8.4/lib/QtXml.framework/Versions/4/QtXml" "@loader_path/QtXml" ./MyOpenSimulator/bin/MyOpenSimulator
install_name_tool -change "/Users/ciattim/Qt4.8.4/lib/QtCore.framework/Versions/4/QtCore" "@loader_path/QtCore" ./MyOpenSimulator/bin/MyOpenSimulator
install_name_tool -change "/Users/ciattim/Qt4.8.4/lib/QtGui.framework/Versions/4/QtGui" "@loader_path/QtGui" ./MyOpenSimulator/bin/MyOpenSimulator

##  Pack the installation stuff
rm ./INSTALLER_MyOpenSimulator_MAC_2.tar.gz
tar vczf INSTALLER_MyOpenSimulator_MAC_2.tar.gz MyOpenSimulator
