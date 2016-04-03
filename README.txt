Windows:

Talga Engine uses msvc2013. To build, open Engine/Engine.sln in Visual Studio 2013
and build. Then you must copy all of the dll dependencies into both build/release 
and build/debug to run each executable. Make sure the solution is x64, NOT x86. The
project WILL NOT work if you try and build it in x86.