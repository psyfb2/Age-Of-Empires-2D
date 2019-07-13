Changed for version 30:

Commented out most of the output code. Left output to do with file loading and errors so that file location issues can be diagnosed.

Fixed potential issue with modifying the objects array from within draw() or doUpdate() since a number of students do this, and it's fairly logical to do so.

Added memory leak destection code for windows when built in debug (not release). If you don't want it, comment out:
	_CrtDumpMemoryLeaks();  	in mainfunction.cpp
	#include <crtdbg.h>		in header.h
Note: changed various header includes to the c versions rather than c++ versions to avoid conflicts with this header.

Added code to delete the framework objects - 
	ImageManager::destroyImageManager();
The image manager is a singleton and designed to never die. However this appears as a memory leak so added this code to destroy it. Only do this after anything which may use an image has been destroyed. For this reason the creation of the objects was moved into a sub-block, so that the stack object for the BaseEngine subclass is destroyed before the image manager is destroyed.

Moved destruction of object array from BaseEngine destructor to base class destructor: this object array code was moved into a base class from BaseEngine (where it was previously) so it could be reused in card demo, but the destruction wasn't so card demo had a memory leak when it used the code.

Labelled a number of extra functions const because they could be, and not doing caused issues for a student using a const reference.
