# Pre-compile binaries #

Pre-compiled binaries are only available for the Windows platform currently. However, use at your own risk! Depending on your version of Matlab, you may have problems using these binaries. You can obtain the easy-to-use installer in the downloads area.

Once you have run the installer, in Matlab set your working directory to the SceneML installation directory. Once there run the startup.m script which will set the paths for SceneML.

# Compiling from source #

## Prerequisites ##

Like all good open source projects, SceneML stands on the shoulders of giants. It depends on a few other open-source libraries so before installing SceneML go install the following. Installation of these will obviously depend on your platform but all are available for Windows, Mac, and Linux.

  * [ODE](http://www.ode.org/) - Physics engine
  * [muParser](http://muparser.sourceforge.net/) - Mathematical expression library
  * [Xerces-C](http://xerces.apache.org/xerces-c/) - XML parsing library
  * [CMake](http://www.cmake.org/HTML/Index.html) - Cross-platform build utility

## On with the compiling! ##

While some binaries of SceneML, compiling from source is always a good choice if you want to make absolutely sure the library will run on your machine/OS with no problems.

### Windows ###

1. Install the prerequisites listed above.

2. Set the following environmental variables:
  * ODE\_HOME - Point to root directory of your ODE installation
  * MUPARSER\_HOME - Point to root directory of your muParser installation
  * XERCES\_HOME - Points to the root directory of your Xerces-C installation

3. Point CMake to the lib directory. Hit Configure then OK.

4. Assuming you use Visual Studio on Windows, CMake will have generated a VS solution file. Open it up, set build type to Release and build the library project. Once this completes select the "INSTALL" project and build it.

5. Close VS, and open Matlab. Point your working directory to the base scememl directory. Then in Matlab type:

```
cd mex
compile
cd ..
```

6. Assuming you had no errors, in Matlab set your working directory to the SceneML installation directory. Once there run the startup.m script which will set the paths for SceneML.the sceneml library is ready to use in Matlab.

### Mac and Linux ###

1. Install the prerequisites listed above.

2. In a terminal window, navigate to the sceneml/libsceneml directory. Issue the following commands:

```
cmake .
make
make install
```

3. Now open Matlab and point your working directory to the base sceneml directory. Then in Matlab type:

```
cd mex
compile
cd ..
```

4. Assuming you had no errors, in Matlab set your working directory to the SceneML installation directory. Once there run the startup.m script which will set the paths for SceneML.the sceneml library is ready to use in Matlab.