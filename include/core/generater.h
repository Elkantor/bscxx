#pragma once

#include <core/common.h>

namespace core{

    /*********/
    /* ENUMS */
    /*********/

    enum ProjectType{
        EXECUTABLE,
        DYNAMIC_LIBRARY,
        STATIC_LIBRARY
    };


    /*************/
    /* FUNCTIONS */
    /*************/

    inline std::string GetCurrentWorkingDir() {
        char buff[FILENAME_MAX];
        GetCurrentDir(buff, FILENAME_MAX );
        std::string current_working_dir(buff);
        return current_working_dir;
    }

    inline void CreateMainCmakeListsFile(){
        std::ofstream outfile("CMakeLists.txt");
        outfile 
            << "cmake_minimum_required (VERSION 3.9)"
            << "\n\n## Include libraries ##"
            << "\ninclude_directories (\"${PROJECT_BINARY_DIR}/../include\")"
            << "\nset(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})"
            << "\n\nif (MSVC)"
            << "\n\tset(EXECUTABLE_OUTPUT_PATH bin/)"
            << "\nendif (MSVC)";
        outfile.close();
    }

    inline void CreateSecondaryCMakeListsFile(const std::string& path, const std::string& project_name){
        std::ofstream outfile(path + "/CMakeLists.txt");
        outfile
            << "project(" << project_name << ")"
            << "\n\nset(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})"
            << "\n\nfile (GLOB_RECURSE source_files ./*)"
            << "\n\nadd_executable (" << project_name << " ${source_files})";
        outfile.close();
    }

    inline void CreateTestCMakeListsFile(const std::string& path, const std::string& project_name){
        std::ofstream outfile(path + "/CMakeLists.txt");
        outfile
            << "project(" << "test_" << project_name << ")"
            << "\n\nset(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})"
            << "\n\nfile (GLOB_RECURSE testing_files ./*)"
            << "\nfile (GLOB_RECURSE testing_source_files ../src/*)"
            << "\n\nFOREACH(item ${testing_source_files})"
            << "\n\tIF(${item} MATCHES \"main.cc\")"
            << "\n\t\tLIST(REMOVE_ITEM testing_source_files ${item})"
            << "\n\tENDIF(${item} MATCHES \"main.cc\")"
            << "\nENDFOREACH(item)"
            << "\n\nadd_executable (test_" << project_name << " ${testing_files} ${testing_source_files})";
        outfile.close();
    }

    inline void CreateMainFile(){
        std::ofstream outfile("src/main.cc");
        outfile 
            << "#pragma once"
            << "\n\n#include <iostream>"
            << "\n\nint main(int argc, char** argv){"
            << "\n\tstd::cout << \"test\" << std::endl;"
            << "\n\n\treturn 0;"
            << "\n}";

        outfile.close();
    }

    inline void CreateFolder(const char* folder_name){
        int error = 0;
        #if defined(_WIN32)
            error = _mkdir(folder_name); // can be used on Windows
        #else 
            mode_t mode = 0733;
            error = mkdir(folder_name, mode); // can be used on Unix
        #endif
        if (error != 0) {
            std::cout << "\t# [ERROR] Can't create the " << folder_name << " folder. (already exists)\n";
        }else{
            std::cout << "\t# [SUCCESS] The " << folder_name << " folder is created.\n";
        }
    }

}// namespace core