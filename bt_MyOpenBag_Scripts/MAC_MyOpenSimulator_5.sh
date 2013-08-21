#!/bin/bash

##  Makes packaging folder structure
rm -fr ./VDK
mkdir VDK
mkdir VDK/bin

##  Copy QT Stuff
cp /usr/local/Trolltech/Qt-4.8.4/lib/QtXml.framework/Versions/4/QtXml VDK/bin/.
cp /usr/local/Trolltech/Qt-4.8.4/lib/QtNetwork.framework/Versions/4/QtNetwork VDK/bin/.
cp /usr/local/Trolltech/Qt-4.8.4/lib/QtCore.framework/Versions/4/QtCore VDK/bin/.
cp /usr/local/Trolltech/Qt-4.8.4/lib/QtGui.framework/Versions/4/QtGui VDK/bin/.
cp -r  /usr/local/Trolltech/Qt-4.8.4/lib/QtGui.framework/Versions/4/Resources/qt_menu.nib VDK/bin/.

##  Copy simulator stuff
cp ../BUILD_DEB/libplant/liblibplant.2.1.3.dylib VDK/bin/.
cd ./VDK/bin
ln -s liblibplant.2.1.3.dylib ./liblibplant.2.1.dylib
ln -s liblibplant.2.1.3.dylib ./liblibplant.2.dylib
ln -s liblibplant.2.1.3.dylib ./liblibplant.dylib
cd ../..
cp -r ../BUILD_DEB/myhome VDK/.
rm -fr ./VDK/myhome/Resources
mkdir ./VDK/myhome/Resources
cp ../BUILD_DEB/MyOpenBagClient/MyOpenSimulator.app/Contents/MacOS/MyOpenSimulator VDK/bin/vdk

##  Create simulator sturtup script
touch VDK/VDK.sh
echo "#!/bin/bash" >> VDK/VDK.sh
echo export DYLD_LIBRARY_PATH="$"PWD/bin >> VDK/VDK.sh
echo export DYLD_FALLBACK_LIBRARY_PATH="$"PWD/bin >> VDK/VDK.sh
echo cd bin >> VDK/VDK.sh
echo ./vdk >> VDK/VDK.sh
chmod +x VDK/VDK.sh

##  Change dyn library path
#install_name_tool -change "/Users/ciattim/Qt4.8.4/lib/QtXml.framework/Versions/4/QtXml" "@loader_path/QtXml" ./MyOpenSimulator/bin/MyOpenSimulator
#install_name_tool -change "/Users/ciattim/Qt4.8.4/lib/QtCore.framework/Versions/4/QtCore" "@loader_path/QtCore" ./MyOpenSimulator/bin/MyOpenSimulator
#install_name_tool -change "/Users/ciattim/Qt4.8.4/lib/QtGui.framework/Versions/4/QtGui" "@loader_path/QtGui" ./MyOpenSimulator/bin/MyOpenSimulator

##  Pack the installation stuff
rm ./INSTALLER_VDK_MAC.tar.gz
tar vczf INSTALLER_VDK_MAC.tar.gz VDK
