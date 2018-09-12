# MIEM Controller


### Touch app for controlling OSC devices

MIEM Controller is multitouch app, based on geometric shapes linked to presets (specific configurations) of the controlled devices. Touch gestures trigger interpolations between presets and allows you to easily create new presets, even if the device offers many control parameters. A second app, called MIEM Editor, can create the session files (.mcs) that MIEM Controller uses.

These apps are parts of the MIEM - Multitouch Interfaces for Electroacoustic Music - project from the [LARAS](http://laras.be) (Brussels, Belgium) and [NUMEDIART](https://numediart.org) (University of Mons, Belgium) laboratories.

A [presentation video](https://youtu.be/87nqCIUmVOA) is available on Youtube, and shows usage examples. Here is a short gif preview of the video:

![(Video preview, original full video at https://youtu.be/87nqCIUmVOA)](https://j.gifs.com/voAOAm.gif)


### How to build yourself the last version

This project relies on the [Juce C++ framework](https://juce.com), which allows to compile applications for macOS, Windows, Android and iOS.

The sources can be built on macOS or Windows using Juce's [Projucer app](https://juce.com/discover/projucer). MIEM Controller Projucer's project can be found in the MiemGenericController folder, and the MIEM Editor Projucer's project is in the MiamEdit folder.

### How to get the applications

To get the ready-to-use MIEM Controller and Editor applications, you can currently contact us directly at [glevaillant@he2b.be](mailto:glevaillant@he2b.be)

### Features coming soon

Some important features are currently being developed and tested:
- Session files upload via iCloud and Google Drive, for a simplified workflow with the iOS and Android app
- Apps published on the tester's programs of Google Play Store and Apple App Store
- Custom range for interpolated parameters' values

### Authors and contact

- Gwendal Le Vaillant, [glevaillant@he2b.be](mailto:glevaillant@he2b.be)
- Guillaume Villée, [guillaume.villee@irisib.be](guillaume.villee@irisib.be)
- Rudi Giot, [rgiot@he2b.be](rgiot@he2b.be)
