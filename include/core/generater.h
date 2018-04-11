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
            << "\n\tinclude_directories (\"${PROJECT_BINARY_DIR}/../include\")"
            << "\n## End of include libraries ##"
            << "\n\nset(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})"
            << "\n\nif (MSVC)"
            << "\n\tset(EXECUTABLE_OUTPUT_PATH bin/)"
            << "\nendif (MSVC)"
            << "\n\n## Add executables ##"
            << "\n\tadd_subdirectory (src)"
            << "\n## End of adding executables ##";
        outfile.close();
    }

    inline void CreateSecondaryCMakeListsFile(
        const std::string& path, 
        const std::string& project_name, 
        const ProjectType project_type = ProjectType::EXECUTABLE
    ){
        std::string type_project;
        std::ofstream outfile(path + "/CMakeLists.txt");

        switch(project_type){
            case DYNAMIC_LIBRARY:
                type_project = "add_library (" + project_name + " SHARED ${source_files})";
                break;
            case STATIC_LIBRARY:
                type_project = "add_library (" + project_name + " STATIC ${source_files})";
                break;
            default:
                type_project = "add_executable (" + project_name + " ${source_files})";
                break;
        }
        outfile
            << "project(" << project_name << ")"
            << "\n\nset(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})"
            << "\n\n## Add source files ##"
            << "\n\tfile (GLOB_RECURSE source_files ./*)"
            << "\n## End of adding source files ##"
            << "\n\n" << type_project;
        outfile.close();
    }

    inline void CreateTestCMakeListsFile(const std::string& path, const std::string& project_name){
        std::ofstream outfile(path + "/CMakeLists.txt");
        outfile
            << "project(" << "test_" << project_name << ")"
            << "\n\nset(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})"
            << "\n\n## Add source files ##"
            << "\n\tfile (GLOB_RECURSE testing_files ./*)"
            << "\n\tfile (GLOB_RECURSE testing_source_files ../src/*)"
            << "\n## End of adding executables ##"
            << "\n\nFOREACH(item ${testing_source_files})"
            << "\n\tIF(${item} MATCHES \"main.cc\")"
            << "\n\t\tLIST(REMOVE_ITEM testing_source_files ${item})"
            << "\n\tENDIF(${item} MATCHES \"main.cc\")"
            << "\nENDFOREACH(item)"
            << "\n\n## Add executables ##"
            << "\n\tadd_executable (test_" << project_name << " ${testing_files} ${testing_source_files})"
            << "\n\n## End of adding executables ##";
        outfile.close();
    }

    inline void CreateMainFile(const std::string& path){
        std::ofstream outfile(path + "/main.cc");
        outfile 
            << "#pragma once"
            << "\n\n#include <iostream>"
            << "\n\nint main(int argc, char** argv){"
            << "\n\tstd::cout << \"test\" << std::endl;"
            << "\n\n\treturn 0;"
            << "\n}";
        outfile.close();
    }

    inline void CreateFolder(const std::string& path){
        int error = 0;
        #if defined(_WIN32)
            error = _mkdir(path.c_str()); // can be used on Windows
        #else 
            mode_t mode = 0733;
            error = mkdir(path.c_str(), mode); // can be used on Unix
        #endif
        if (error != 0) {
            std::cout << "\t# [ERROR] Can't create the " << path << " folder. (already exists)\n";
        }else{
            std::cout << "\t# [SUCCESS] The " << path << " folder is created.\n";
        }
    }

    inline void AddModuleHeadersToMainCMakeListsFile(const std::string& path_module){
        std::vector<std::string> lines;
        std::string line;
        std::ifstream infile("./CMakeLists.txt", std::ios::in);
        if (!infile) {
            std::cerr << "Could not open the main CMakeLists.txt file\n";
            return;
        }

        while(!infile.eof()){
            std::getline(infile, line);
            if(line.compare("## End of include libraries ##") == 0){
                lines.emplace_back("\tinclude_directories (\"${PROJECT_BINARY_DIR}/../" + path_module + "/include\")");
            }
            lines.emplace_back(line);
        }

        std::ofstream outfile("./CMakeLists.txt");
        for(const auto& line : lines){
            outfile << "\n" << line;
        }
        outfile.close();
    }

    inline void AddModuleSourceFilesToSecondariesCmakeListsFiles(const std::string& path_module){
        std::vector<std::string> lines;
        std::string line;
        std::ifstream infile("./CMakeLists.txt", std::ios::in);
        if (!infile) {
            std::cerr << "Could not open the main CMakeLists.txt file\n";
            return;
        }
    }

}// namespace core