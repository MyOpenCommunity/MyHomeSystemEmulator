mkdir VDK
mkdir VDK/bin
cp /usr/local/Trolltech/Qt-4.8.4/lib/libQtXml.so VDK/bin/.
ln -s libQtXml.so VDK/bin/libQtXml.so.4
cp /usr/local/Trolltech/Qt-4.8.4/lib/libQtNetwork.so VDK/bin/.
ln -s libQtNetwork.so VDK/bin/libQtNetwork.so.4
cp /usr/local/Trolltech/Qt-4.8.4/lib/libQtCore.so VDK/bin/.
ln -s libQtCore.so VDK/bin/libQtCore.so.4
cp /usr/local/Trolltech/Qt-4.8.4/lib/libQtXmlPatterns.so VDK/bin/.
ln -s libQtXmlPatterns.so VDK/bin/libQtXmlPatterns.so.4
cp /usr/local/Trolltech/Qt-4.8.4/lib/libQtGui.so VDK/bin/.
ln -s libQtGui.so VDK/bin/libQtGui.so.4
cp ../BUILD_REL/libplant/liblibplant.so.2.1.3 VDK/bin/.
ln -s liblibplant.so.2.1.3 VDK/bin/liblibplant.so.2.1
ln -s liblibplant.so.2.1 VDK/bin/liblibplant.so.2
ln -s liblibplant.so.2 VDK/bin/liblibplant.so
cp -r ../BUILD_REL/myhome VDK/.
cp ../BUILD_REL/MyOpenBagClient/MyOpenSimulator VDK/bin/VirtualDevelopmentKit
touch VDK/VirtualDevelopmentKit.sh
echo "#!/bin/bash" >> VDK/VirtualDevelopmentKit.sh
echo export LD_LIBRARY_PATH="$"PWD/bin >> VDK/VirtualDevelopmentKit.sh
echo cd bin >> VDK/VirtualDevelopmentKit.sh
echo ./VirtualDevelopmentKit >> VDK/VirtualDevelopmentKit.sh
chmod +x VDK/VirtualDevelopmentKit.sh
tar vczf INSTALLER_VDK_Linux.tar.gz VDK
