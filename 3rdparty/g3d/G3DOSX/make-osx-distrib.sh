#!/bin/sh
#
# Build G3D libraries and copy them to the current directory,
# then build a tarfile. 
#
# Script maintained by Casey O'Donnell (ckodonnell at users dot sourceforge dot net)
#

XCODE_PROJECT=G3DOSX.xcodeproj

# SDLMain.h and SDLMain.m are required to built SDL applications on 
# OS X.  Authorship information for these two files is in the text
# of each file. 

FILES="SDLMain.h SDLMain.m libG3D.a libG3D_debug.a libGLG3D.a libGLG3D_debug.a OSX_README.txt" 

# Remove any libraries that are hanging around this directory.
rm *.a 

xcodebuild -project $XCODE_PROJECT clean
rm ./build/Development/*.a

# Build the Development (aka debug) build style
xcodebuild -project $XCODE_PROJECT -target GLG3D -configuration Development
cp ./build/Development/libG3D.a libG3D_debug.a
cp ./build/Development/libGLG3D.a libGLG3D_debug.a

# We have to clean what we just built 
xcodebuild -project $XCODE_PROJECT clean
rm ./build/Deployment/*.a

# Build the Deployement (aka Release) build style
xcodebuild -project $XCODE_PROJECT -target GLG3D -configuration Deployment
cp ./build/Deployment/libG3D.a libG3D.a
cp ./build/Deployment/libGLG3D.a libGLG3D.a

# Delete the osx-lib.tar.gz file if it exists
rm osx-lib.tar.gz

# Now tar and zip up the files that we need as part of the distribution. 
tar cf osx-lib.tar $FILES 
gzip osx-lib.tar


