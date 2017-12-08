#!/bin/sh

VERSION_STRING="0.1.0"

echo "Copying 6 macOS apps to ./Applications [MIEM project version $VERSION_STRING]..."

# D'abord, création du dossier s'il n'existe pas. S'il existe : on n'écrase pas !
if [ -d "./Applications/v$VERSION_STRING" ]; then
	echo "v$VERSION_STRING folder already exists! Increase version number."
	
else
	# Création du dossier
	mkdir ./Applications/v$VERSION_STRING
	
	# Pour chaque app : création de l'archive .dmg puis déplacement de l'archive

	echo "Spat Player..."
	cp -R ../MiamSpat/Builds/MacOSX/build/Release/MiemSpatPlayer.app/* ./Applications/v$VERSION_STRING/MiemSpatPlayer.app/
	#hdiutil create -format UDZO -srcfolder ../MiamSpat/Builds/MacOSX/build/Release/MiemSpatPlayer.app ./Applications/v$VERSION_STRING/MiemSpatPlayer.dmg
	#cp -R ../MiamSpat/Builds/MacOSX/build/Release/MiemSpatPlayer.app ./Applications/v$VERSION_STRING/MiemSpatPlayer.app

	echo "Editor..."
fi