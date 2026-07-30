#!/bin/sh
#
# Build G3D libraries in Development configuration 
#
# Script maintained by Casey O'Donnell (ckodonnell at users dot sourceforge dot net)
#

XCODE_PROJECT=G3DOSX.xcodeproj

# Build the Development (aka debug) build style
xcodebuild -project $XCODE_PROJECT -target G3DTest -configuration Deployment

