# cmt [![Build Status](https://api.travis-ci.org/louiscyphre/cmt.png)](https://travis-ci.org/louiscyphre/cmt)

# Introduction
**cmt** (originally CppMT) is a method for visual object tracking. It is the C++ implementation of **cmt** that was originally developed in Python by George Nebehay.
Details can be found on the [project page](http://www.gnebehay.com/cmt).
The implementation in this repository is platform-independent and runs
on Linux, Windows and OS X. You can check the original fork for more info.

# Remark
I've adapted this software to be the dependency for other project and use it as a module (cmake of parent project linking it as .so executable), if you want to use it please take a look at the class CMT which now has functions
~~~
    context_t* createContext(const Mat &im_gray, const Rect &rect);
    context_t* getContext() const { return context; }
    void switchContext(context_t *context)
    void processFrame(const Mat im_gray);// this was also before
~~~
**context_t** is a type for struct, which can be found in CMT.h

# License
**cmt** is freely available under the [Simplified BSD license][1],
meaning that you can basically do with the code whatever you want.
If you use our algorithm in scientific work, please cite this publication:
```
@inproceedings{Nebehay2015CVPR,
    author = {Nebehay, Georg and Pflugfelder, Roman},
    booktitle = {Computer Vision and Pattern Recognition},
    month = jun,
    publisher = {IEEE},
    title = {Clustering of {Static-Adaptive} Correspondences for Deformable Object Tracking},
    year = {2015}
}
```

# Dependencies
* OpenCV 3.4.2 and upper (I tend to use the latest code)
* C++17 standart compliance

# Building
**cmt** uses cmake for building.
To be able to compile it, please review the file .travis.yml, it is very intuitive and you can have a clue which commands on Ubuntu you need to execute in order to compile it. The code itself is works on Linux, Windows and OS X, as author states, but in order to build it on other systems than Ubuntu Linux, you might put some effort to fix cmake scripts, or you can add the sources to your project as is.

## Note for Windows users
These steps are necessary to get CppMT running on Windows:
* Download this repository.
* Download and install
[Visual Studio](https://www.visualstudio.com/en-us/downloads/download-visual-studio-vs.aspx).
* Download and install the latest [OpenCV 3.4.x release](http://opencv.org/downloads.html).
* Download and install the latest [CMake release](http://www.cmake.org/download/).
* Dance with a tambourine a bit, run CMake and configure the project [like so](http://www.gnebehay.com/cmt/cmake.png).
The only thing you actually have to specify yourself is the location of your OpenCV installation.
* Add the OpenCV DLLs to the PATH environment variable [like so](http://www.gnebehay.com/cmt/path.png).
The vcXX part depends on the edition of visual studio that you are using.
For example, vc12 is to be used for Visual Studio 2013.
* Open the Visual Studio solution file (CMT.sln), hit F7 to build and F5 to Run/Debug.

[1]: http://en.wikipedia.org/wiki/BSD_licenses#2-clause_license_.28.22Simplified_BSD_License.22_or_.22FreeBSD_License.22.29

