# About
MGF Explorer is a C++/Qt GUI tool for opening and viewing assets contained within Meyer/Glass Interactive archive files used in MechAssault and MechAssault 2: Lone Wolf. This tool is heavily inspired by Adjutant, a similar tool for browsing .map files used in the Halo games. Please note that this application is early in development and a lot of features are not complete, especially the model viewer. MGF Explorer can open textures (.tif), models (.node and .mgmodel), and localised string tables (.mgtext) from both games. You can also extract files as they are - conversion to common formats isn't implemented yet.

## Screenshots

# Instructions for use
The MGF files that come with MechAssault and MechAssault 2 are initially compressed, and must be decompressed before MGF-Explorer can open them.
These files can be decompressed using [offzip](http://aluigi.altervista.org/mytoolz/offzip.zip "http://aluigi.altervista.org/mytoolz/offzip.zip"). Once offzip is downloaded, run command prompt and navigate to the folder where offzip.exe is stored.
Use the following command to decompress an MGF archive:
```
offzip.exe -a -1 <mgf file dir> <output dir>
```
The result should be a decompressed version of the same file which can now be opened by MGF-Explorer.
I have included offzip and a batch script which automates this for you. Simply place the two files in the root of your MechAssault and/or MechAssault 2 root directory (where all the mgf files are) and run the .bat file. This will decompress all the MGF files and place them in ./decompressed. Note that some files will fail - this is because they are already decompressed.

MGF Explorer can have multiple MGF files opened, separated by tabs. The tree view on the left displays the entire contents of the archive, preserving the actual directory structure. If the "Type" column for a particular file says something (such as Strings or Texture), you can click on it and review the asset on the right.

# Current features
* Open MGF archive files and display the contents, preserving the directory tree structure
* Model viewer
  * Can open both .mgmodel files (MA2) and .node files (MA1 and MA2)
  * Displays the node hierarchy, animations, meshes and materials
  * Nodes can be toggled on and off to hide/view meshes
* Texture viewer that can open and preview almost all textures found in an MGF file
* File extraction
  * Only raw file extraction for now
	
# Planned features
* Converting assets to common 3rd party formats (i.e. textures to PNG, models to FBX, etc)
* File injection
* Animations in the model viewer

# Credits
* https://www.qt.io/
* https://pugixml.org/