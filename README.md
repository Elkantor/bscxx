[![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg?style=flat-square)](http://makeapullrequest.com) 
[![Language](https://img.shields.io/badge/language-C++-blue.svg)](https://isocpp.org/)
[![Standard](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B17)
[![Build status](https://ci.appveyor.com/api/projects/status/79o8ckxl0vnq2b85?svg=true)](https://ci.appveyor.com/project/Elkantor/bscxx)


# bscxx
> bscxx (for bootstrap CXX project) is a very light utility software, written in C++ 17, to help you starting quickly a new C++ project using cmake. 
Its end goal is to be a package manager, to easily include C++ modules to your project, deeply inspired by npm (node.js), composer (php) and cargo (rust).

## Prerequisite

To be able to build this project, you need a compiler which supports the last C++ 17 features, such as "filesystem". 
So you'll need Visual Studio 17 or GCC 8 for example, at least, to be able to build this project from source.

## Installing / Getting started

<blockquote>
<p><g-emoji class="g-emoji" alias="bulb" fallback-src="https://assets-cdn.github.com/images/icons/emoji/unicode/1f4a1.png" ios-version="6.0"><img class="emoji" alt="bulb" height="20" width="20" src="https://assets-cdn.github.com/images/icons/emoji/unicode/1f4a1.png"></g-emoji> [Warning] bscxx generates CMake build files, so you need to install CMake to be able to compile your C++ code with whatever compiler you want.
</p>
</blockquote>

You have three choices to install bscxx. 

1. You can download the [released version](https://github.com/Elkantor/bscxx/releases) (for now, it's just available on Windows 64 bits system), and add it to your environment path.
---

2. You can easily install bscxx with the help of [*__scoop__*,](https://github.com/lukesampson/scoop) a windows package manager for portable apps.
Start by installing *__scoop__* (on Windows only) with this PowerShell command (you need PowerShell 3 at least):
```shell
iex (new-object net.webclient).downloadstring('https://get.scoop.sh')
```

Now, open a new *__cmd__* and type this command to add this git repository as a scoop bucket:
```shell
scoop bucket add bscxx https://github.com/Elkantor/bscxx.git
```

Finally, install bscxx with this command: 
```shell
scoop install bscxx
```

Now, you should be ready to use bscxx. You can test if it has correctly been installed by typing the following command for example:
```shell
bscxx -h
```
---

3. Or you can build bscxx yourself by cloning this repository:
```shell
git clone https://github.com/Elkantor/bscxx
cd bscxx
```

Create a build folder:
```shell
mkdir build
cd build
```

Run this cmake command to create the build system files.
You can use whatever build system you want like Make, MSVS/Cl... I personnaly prefer using [Ninja](https://ninja-build.org/) when I can. 
If you don't have Ninja and want to use it, install it first (you can easily install it with *__scoop__* or *__chocolatey__*).
```shell
cmake .. -G "Ninja" -DCMAKE_BUILD_TYPE:STRING=Release && cmake --build .
```

And finally, launch the app to test it if everything compiled fine:
```shell
../build/src/bin/Release/bscxx -h
```

## Documentation 

<blockquote>
<p><g-emoji class="g-emoji" alias="bulb" fallback-src="https://assets-cdn.github.com/images/icons/emoji/unicode/1f4a1.png" ios-version="6.0"><img class="emoji" alt="bulb" height="20" width="20" src="https://assets-cdn.github.com/images/icons/emoji/unicode/1f4a1.png"></g-emoji> [Warning] bscxx is a young WIP project, so the documentation is not fully available for now, but will be released as soon as possible.
</p>
</blockquote>

* [Getting started](./doc/getting_started.md)

## Developing

### Built and developed with

* [CMake](https://cmake.org/)
* C++ 17
* Lots of coffee

### Rules / Coding Standard

This project follow some rules, to be as consistent and comprehensible as possible.

* [semver](https://semver.org/), for the version of the package.
* [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html) : I do my best to try to conform to the Google C++ Style Guide, with only one exception: I use 4 spaces for indentations instead of 2.

## Licencing

MIT License

Copyright (c) 2018 Victor Gallet

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
