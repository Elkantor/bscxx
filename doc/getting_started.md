# Getting Started
> Here are the first steps to begin to use bscxx as a C++ package manager

## Create a new project

The first thing you want to do, when creating a new project, is actually to create the structure of your new C++ project.
If you often create new projects, you know that can be redundant and a bit borring. So why not generate it automaticly, just by using one command ?

First, go where you want to create your project, and create a new folder for it.

*For example, for me, it's inside my C:\Users\V_GALLET\Documents\ folder*
```shell
cd C:\Users\V_GALLET\Documents && mkdir new_project
```

Then, create the structure of your new project, with everything you need to begin.
In my case, my project is named "new_project"
```shell
bscxx create new_project
```

By typing the "ls" command (on Unix) or the "dir" command (on Windows), you should see a structure which looks like this:
<div style="position:absolute; width:100%; height:100%">
  <img src="http://gamedevpex.com/bscxx/bscxx_structure.png"/>
</div>

Here's a quick explaination of all of these folders and files :
* The main CMakeLists.txt file, to generate your project with CMake, and include all the headers/source files your project will need...
* The "bscxx_module" folder, which will contain all your modules dependencies (just like node_modules with npm for instance).
* The "dependencies.bscxx" file, which contains the list of the project's dependencies. You just need this file to download and install your modules. It's really similar to the "package.json" or to the "composer.json" files for npm or composer package managers.
* And finally, the default "classic" structure of your C++ project, with the "include" folder, which contains all your header files, the "src" folder, which contains your source files, and the "test" folder, which will contain all your C++ files needed for your unit tests.

Actually, there is a default "main.cc" file inside the src folder, created by default (just like the "test.cc" file inside the "test" folder).
So your project is already ready to be built.

<blockquote>
<p><g-emoji class="g-emoji" alias="bulb" fallback-src="https://assets-cdn.github.com/images/icons/emoji/unicode/1f4a1.png" ios-version="6.0"><img class="emoji" alt="bulb" height="20" width="20" src="https://assets-cdn.github.com/images/icons/emoji/unicode/1f4a1.png"></g-emoji> [Note] bscxx automaticly initializes a git repository when creating a new project, and add a .gitignore file with the "build" folder already listed in. In the next step, if you choose to rename the "build" folder whatever you want, don't forget to add it in the gitignore.
</p>
</blockquote>

Create a new folder named "build" and go inside.
```shell
mkdir build && cd build
```

With the help of CMake, generate the project files (with whatever generator you want, here, I'll keep it simple, by using the default one, which is Visual Studio 2017 for example, for me on Windows).
```shell
cmake ..
```

Everything should be fine, and you can now build this default project, with the help of CMake.
```shell
cmake --build . --config Release
```

Once again, everything should compile just fine. Which means you can actually start to launch the executable file (.exe on windows, without ".exe" part on linux).
```shell
"src/bin/release/new_project.exe"
```

And you should see this output:
```shell
Running the app...
```

Well done ! You created, built and ran your first C++ project generated with bscxx !
Now comes the real exciting part: take the advantage of its package manager features !
