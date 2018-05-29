# Getting Started
> Here are the first steps to begin to use bscxx as a C++ package manager

## Create a new project

The first thing you want to do, when creating a new project, is actually to create the structure of your new C++ project.
If you often create new projects, you know that can be redundant and a bit borring. So why not generate it automaticly, just by using one command ?

First, go where you want to create your project, and create a new folder for it.

*For example, for me, it's inside my C:\Users\V_Gallet\Documents\ folder*
```shell
cd path_to_projects && mkdir new_project
```

Then, create the structure of your new project, with everything you need to begin.
In my case, my project is named "new_project"
```shell
bscxx create new_project
```

You should see a structure which looks like this:
<div style="position:absolute; width:100%; height:100%">
  <img src="http://gamedevpex.com/bscxx/bscxx_structure.png"/>
</div>

So, you have the main CMakeLists.txt file, to compile your project with CMake, and include all the headers/source files your project will need...
The "bscxx_module" folder, which will contains all your modules dependencies (just like node_modules with npm for instance).
There is the dependencies.bscxx file, which contains the list of the project's dependencies. You just need this file to install your module. It's really similar to the "package.json" or to the "composer.json" files for npm or composer packages managers.
And finally you have the default "classic" structure of your C++ project, with the "include" folder, which contains all your header files, the "src" folder, which contains your source files, and the "test" folder, which will contain all your C++ files needed for your unit tests.

Actually, there is a default "main.cc" file inside the src folder, created by default (just like the "test.cc" file inside the "test" folder).
So your project is already ready to be built.

Create a new folder named "build" and go inside.
```shell
mkdir build && cd build
```

With the help of CMake, generate the project file (with whatever generator you want, here, I'll keep it simple, by using the default one, which is Visual Studio 2017 for example, for me).
```shell
cmake ..
```

Everything should be fine, and you can now build this default project, with the help of CMake.
```shell
cmake --build . --config Release
```

Once again, everything should compile just fine. Which means you can actuelly start launch the executable file (.exe on windows, without ".exe" part on linux).
```shell
"src/bin/release/new_project.exe"
```

And you should see this output:
```shell
Running the app...
```

Well done ! You created, built and ran your first C++ project generated with bscxx !
Now comes the real exciting part: take the advantage of its package manager features !
