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

    inline void CreateMainCmakeListsFile(const std::string& path){
        std::ofstream outfile(path + "CMakeLists.txt");
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
            << "\n\tadd_subdirectory (test)"
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
            << "\n\n## Remove main.cc files of modules ##"
            << "\n## End of removing main.cc files of modules ##"
            << "\n\n## Add executables ##"
            << "\n\t" << type_project
            << "\n## End of adding executables ##";
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
            << "\n## End of adding source files ##"
            << "\n\n## Remove main.cc files of modules ##"
            << "\n\tFOREACH(item ${testing_source_files})"
            << "\n\t\tIF(${item} MATCHES \"main.cc\")"
            << "\n\t\t\tLIST(REMOVE_ITEM testing_source_files ${item})"
            << "\n\t\tENDIF(${item} MATCHES \"main.cc\")"
            << "\n\tENDFOREACH(item)"
            << "\n## End of removing main.cc files of modules ##"
            << "\n\n## Add executables ##"
            << "\n\tadd_executable (test_" << project_name << " ${testing_files} ${testing_source_files})"
            << "\n## End of adding executables ##";
        outfile.close();
    }

    inline void CreateMainFile(const std::string& path){
        std::ofstream outfile(path + "/main.cc");
        outfile 
            << "\n\n#include <iostream>"
            << "\n\nint main(int argc, char** argv){"
            << "\n\tstd::cout << \t\"Running the app...\" << std::endl;"
            << "\n\n\treturn 0;"
            << "\n}";
        outfile.close();
    }

    inline void CreateTestMainFile(const std::string& path){
        std::ofstream outfile(path + "/test.cc");
        outfile 
            << "\n\n#include <iostream>"
            << "\n\nint main(int argc, char** argv){"
            << "\n\tstd::cout << \t\"Performing tests...\" << std::endl;"
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
    }

    inline void RemoveFolder(const std::string& path){
        std::string command = "rm -r " + path + "> null && rm -r null";
        const char* command_cstr = command.c_str();
        system(command_cstr);
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
                if(lines.at(lines.size() - 1).compare("\tinclude_directories (\"${PROJECT_BINARY_DIR}/../" + path_module + "/include\")") != 0){
                    lines.emplace_back("\tinclude_directories (\"${PROJECT_BINARY_DIR}/../" + path_module + "/include\")");
                }
            }
            lines.emplace_back(line);
        }
        infile.close();

        std::ofstream outfile("./CMakeLists.txt");
        for(int i = 0; i < lines.size(); i++){
            outfile << lines.at(i) << ((i == lines.size()-1) ? "" : "\n");
        }
        outfile.close();
    }

    inline void RemoveModuleHeadersFromMainCMakeListsFile(const std::string& path_module){
        std::vector<std::string> lines;
        std::string line;
        std::ifstream infile("./CMakeLists.txt", std::ios::in);
        
        if (!infile) {
            std::cerr << "Could not open the main CMakeLists.txt file\n";
            return;
        }

        while(!infile.eof()){
            std::getline(infile, line);
            if(line.compare("\tinclude_directories (\"${PROJECT_BINARY_DIR}/../" + path_module + "/include\")") == 0){
                continue;
            }else{
                lines.emplace_back(line);
            }
        }
        infile.close();

        std::ofstream outfile("./CMakeLists.txt");
        for(int i = 0; i < lines.size(); i++){
            outfile << lines.at(i) << ((i == lines.size()-1) ? "" : "\n");
        }
        outfile.close();
    }

    inline void AddModuleSourceFilesToSecondaryCMakeListsFile(const std::string& module_name, const std::string& source_folder){
        std::vector<std::string> lines;
        std::string line;
        
        std::ifstream infile("./" + source_folder + "/CMakeLists.txt", std::ios::in);
        if (!infile) {
            std::cerr << "Could not open the secondaries (inside src and test folders) CMakeLists.txt files\n";
            return;
        }
        
        while(!infile.eof()){
            std::getline(infile, line);
            if(line.compare("## End of adding source files ##") == 0){
                if(lines.at(lines.size() - 1).compare("\tfile (GLOB_RECURSE " + module_name + "_source_files " + "./bscxx_modules/" + module_name + "/src/*)") != 0){
                    lines.emplace_back("\tfile (GLOB_RECURSE " + module_name + "_source_files " + "./bscxx_modules/" + module_name + "/src/*)");
                }
                lines.emplace_back("## End of adding source files ##");
            }else if(line.compare("## End of removing main.cc files of modules ##") == 0){
                if(lines.size() > 5){ 
                    if(lines.at(lines.size() - 5).compare("\tFOREACH(item ${" + module_name + "_source_files})") != 0){
                        lines.emplace_back("\tFOREACH(item ${" + module_name + "_source_files})");
                        lines.emplace_back("\t\tIF(${item} MATCHES \"main.cc\")");
                        lines.emplace_back("\t\t\tLIST(REMOVE_ITEM " + module_name + "_source_files ${item})");
                        lines.emplace_back("\t\tENDIF(${item} MATCHES \"main.cc\")");
                        lines.emplace_back("\tENDFOREACH(item)");
                    }
                }
                lines.emplace_back("## End of removing main.cc files of modules ##");
            }else if(line.compare("## End of adding executables ##") == 0){
                std::string previous_line = lines.at(lines.size() -1);
                std::size_t found_module = previous_line.find("${" + module_name + "_source_files}");
                if(found_module == std::string::npos){ 
                    std::size_t project_source_found = previous_line.find("})");
                    std::string new_line = previous_line.substr(0, project_source_found) + "} " + "${" + module_name + "_source_files})";
                    lines.at(lines.size() - 1) = new_line;
                }
                lines.emplace_back("## End of adding executables ##");
            }else{
                lines.emplace_back(line);
            }
        }
        infile.close();

        std::ofstream outfile("./" + source_folder + "/CMakeLists.txt");
        for(int i = 0; i < lines.size(); i++){
            outfile << lines.at(i) << ((i == lines.size()-1) ? "" : "\n");
        }
        outfile.close();
    }

    inline void RemoveModuleSourceFilesToSecondaryCmakeListsFile(const std::string& module_name, const std::string& source_folder){
        std::vector<std::string> lines;
        std::string line;
        
        std::ifstream infile("./" + source_folder + "/CMakeLists.txt", std::ios::in);
        if (!infile) {
            std::cerr << "Could not open the secondaries CMakeLists.txt files (inside src and test folders)\n";
            return;
        }

        int count_lines_removing_main_file = 5;
        while(!infile.eof()){
            std::getline(infile, line);
            if(lines.size() > 0){
                if(line.compare("\tfile (GLOB_RECURSE " + module_name + "_source_files " + "./bscxx_modules/" + module_name + "/src/*)") == 0){
                    continue;
                }
                if(count_lines_removing_main_file > 0){
                    if(line.compare("\tFOREACH(item ${" + module_name + "_source_files})") == 0 
                        || count_lines_removing_main_file < 5){
                        count_lines_removing_main_file--;
                        continue;
                    }
                }
                if(line.compare("## End of adding executables ##") == 0){
                    std::string previous_line = lines.at(lines.size() -1);
                    std::size_t size_module_name = module_name.length();
                    std::size_t found_module = previous_line.find("${" + module_name + "_source_files}");
                    if(found_module != std::string::npos){ 
                        std::size_t found_end = previous_line.find(")", found_module);
                        std::string new_line_1 = previous_line.substr(0, found_module - 1);
                        std::string new_line_2 = previous_line.substr(found_module + size_module_name + 16, found_end);
                        std::string new_line = new_line_1 + new_line_2;
                        lines.at(lines.size() - 1) = new_line;
                    }
                    lines.emplace_back("## End of adding executables ##");
                    continue;
                }
            }
            lines.emplace_back(line);
        }
        infile.close();

        std::ofstream outfile("./" + source_folder + "/CMakeLists.txt");
        for(int i = 0; i < lines.size(); i++){
            outfile << lines.at(i) << ((i == lines.size()-1) ? "" : "\n");
        }
        outfile.close();
    }

    inline void AddGithubModule(const std::string& github_url, const std::string& module_path){
        std::string final_path_module = module_path + github_url.substr(github_url.find("/")+1, github_url.length()-1);
        std::string command = "git clone http://github.com/" + github_url + " " + final_path_module + "> null && rm -r null";
        const char* command_cstr = command.c_str();
        system(command_cstr);

        std::string command_rm = "rm -rf " + final_path_module + "/.git > null && rm -r null";
        const char* command_rm_cstr = command_rm.c_str();
        system(command_rm_cstr);
    }

    inline void UpdateDependenciesFile(const std::string& module_name, const std::string& module_version){
        std::string line;
        std::ifstream infile("./src/CMakeLists.txt", std::ios::in);
        if (!infile) {
            std::cerr << "Could not open the secondaries CMakeLists.txt files (inside src and test folders)\n";
            return;
        }
        std::getline(infile, line);
        std::string project_name = line.substr(line.find("(")+1, line.length()-1);
        project_name = project_name.substr(0, project_name.find(")"));
        infile.close();
        std::ofstream outfile("./bscxx_dependencies.txt");
        std::string body;
        body = "\tBSCXX_PROJECT:\n\n";
        body += "[" + project_name + "]:^1.0.0\n\n\n";
        body += "\tBSCXX_DEPENDENCIES:\n\n";
        for(const auto& p : std::experimental::filesystem::v1::directory_iterator("bscxx_modules")){
            std::string module_path = p.path().string();
            body += "[" + module_path.substr(14, module_path.length()-1) + "]: ";
        }
        outfile << body;
        outfile.close();      
    }

}// namespace core