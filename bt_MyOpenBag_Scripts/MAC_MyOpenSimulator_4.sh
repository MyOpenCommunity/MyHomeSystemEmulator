#!/bin/bash

##  Makes packaging folder structure
rm -fr ./MyOpenSimulator
mkdir MyOpenSimulator
mkdir MyOpenSimulator/bin

##  Copy QT Stuff
cp /usr/local/Trolltech/Qt-4.8.4/lib/QtXml.framework/Versions/4/QtXml MyOpenSimulator/bin/.
cp /usr/local/Trolltech/Qt-4.8.4/lib/QtNetwork.framework/Versions/4/QtNetwork MyOpenSimulator/bin/.
cp /usr/local/Trolltech/Qt-4.8.4/lib/QtCore.framework/Versions/4/QtCore MyOpenSimulator/bin/.
cp /usr/local/Trolltech/Qt-4.8.4/lib/QtGui.framework/Versions/4/QtGui MyOpenSimulator/bin/.
cp -r  /usr/local/Trolltech/Qt-4.8.4/lib/QtGui.framework/Versions/4/Resources/qt_menu.nib MyOpenSimulator/bin/.

##  Copy simulator stuff
cp ../BUILD_DEB/libplant/liblibplant.2.1.3.dylib MyOpenSimulator/bin/.
cd ./MyOpenSimulator/bin
ln -s liblibplant.2.1.3.dylib ./liblibplant.2.1.dylib
ln -s liblibplant.2.1.3.dylib ./liblibplant.2.dylib
ln -s liblibplant.2.1.3.dylib ./liblibplant.dylib
cd ../..
cp -r ../BUILD_DEB/myhome MyOpenSimulator/.
rm -fr ./MyOpenSimulator/myhome/Resources
mkdir .//MyOpenSimulator/myhome/Resources
cp ../BUILD_DEB/MyOpenBagClient/MyOpenSimulator.app/Contents/MacOS/MyOpenSimulator MyOpenSimulator/bin/.

##  Create simulator sturtup script
touch MyOpenSimulator/MyOpenSimulator.sh
echo "#!/bin/bash" >> MyOpenSimulator/MyOpenSimulator.sh
echo export DYLD_LIBRARY_PATH="$"PWD/bin >> MyOpenSimulator/MyOpenSimulator.sh
echo export DYLD_FALLBACK_LIBRARY_PATH="$"PWD/bin >> MyOpenSimulator/MyOpenSimulator.sh
echo cd bin >> MyOpenSimulator/MyOpenSimulator.sh
echo ./MyOpenSimulator >> MyOpenSimulator/MyOpenSimulator.sh
chmod +x MyOpenSimulator/MyOpenSimulator.sh

##  Change dyn library path
#install_name_tool -change "/Users/ciattim/Qt4.8.4/lib/QtXml.framework/Versions/4/QtXml" "@loader_path/QtXml" ./MyOpenSimulator/bin/MyOpenSimulator
#install_name_tool -change "/Users/ciattim/Qt4.8.4/lib/QtCore.framework/Versions/4/QtCore" "@loader_path/QtCore" ./MyOpenSimulator/bin/MyOpenSimulator
#install_name_tool -change "/Users/ciattim/Qt4.8.4/lib/QtGui.framework/Versions/4/QtGui" "@loader_path/QtGui" ./MyOpenSimulator/bin/MyOpenSimulator

##  Pack the installation stuff
rm ./INSTALLER_MyOpenSimulator_MAC_4.tar.gz
tar vczf INSTALLER_MyOpenSimulator_MAC_4.tar.gz MyOpenSimulator
