#pragma once

#include <core/common.h>
#include <addons/console_color.h>

namespace core{

    /**************/
    /* STRUCTURES */
    /**************/
    struct Module{
        std::set<Module> dependencies;
        std::set<Module> parent_modules;
        std::string name;
        std::string path;
        std::string url;
        bool operator<(const Module& m1) const{
            return name < m1.name;
        }
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

    inline bool CreateMainCmakeListsFile(const std::string& path){
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
        
        if(!std::experimental::filesystem::v1::exists(path + "CMakeLists.txt")){
            return false;
        }
        return true;
    }

    inline bool CreateSecondaryCMakeListsFile(
        const std::string& path, 
        const std::string& project_name
    ){
        std::string type_project;
        std::ofstream outfile(path + "/CMakeLists.txt");
        type_project = "add_executable (" + project_name + " ${source_files})";

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

        if(!std::experimental::filesystem::v1::exists(path + "/CMakeLists.txt")){
            return false;
        }
        return true;
    }

    inline bool CreateTestCMakeListsFile(const std::string& path, const std::string& project_name){
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

        if(!std::experimental::filesystem::v1::exists(path + "/CMakeLists.txt")){
            return false;
        }
        return true;
    }

    inline bool CreateMainFile(const std::string& path){
        std::ofstream outfile(path + "/main.cc");
        outfile 
            << "#include <iostream>"
            << "\n\nint main(int argc, char** argv){"
            << "\n\tstd::cout << \"Running the app...\" << std::endl;"
            << "\n\n\treturn 0;"
            << "\n}";
        outfile.close();

        if(!std::experimental::filesystem::v1::exists(path + "/main.cc")){
            return false;
        }
        return true;
    }

    inline bool CreateTestMainFile(const std::string& path){
        std::ofstream outfile(path + "/test.cc");
        outfile 
            << "#include <iostream>"
            << "\n\nint main(int argc, char** argv){"
            << "\n\tstd::cout << \"Performing tests...\" << std::endl;"
            << "\n\n\treturn 0;"
            << "\n}";
        outfile.close();

        if(!std::experimental::filesystem::v1::exists(path + "/test.cc")){
            return false;
        }
        return true;
    }

    inline bool CreateFolder(const std::string& path){
        int error = 0;
        #if defined(_WIN32)
            error = _mkdir(path.c_str()); // can be used on Windows
        #else 
            mode_t mode = 0733;
            error = mkdir(path.c_str(), mode); // can be used on Unix
        #endif
        if(!std::experimental::filesystem::v1::exists(path)){
            return false;
        }
        return true;
    }

    bool RemoveFolder(const std::string& path){
        std::experimental::filesystem::v1::path path_to_remove = path;
        std::string command;
        if(std::experimental::filesystem::v1::exists(path_to_remove)){
            #if defined(_WIN32)
                std::string windows_path = path;
                std::string from = "/";
                std::string to = "\\";
                size_t start_pos = 0;
                while((start_pos = windows_path.find(from, start_pos)) != std::string::npos) {
                    windows_path.replace(start_pos, from.length(), to);
                    start_pos += to.length();
                }
                command = "rmdir /s /q " + windows_path + " >> NULL && del NULL";// can be used on Windows
            #else 
                command = "rm -rf " + path; // can be used on Unix
            #endif
            system(command.c_str());
            
            return true;
        }
        return false;
    }

    bool GetProjectName(std::string* out_project_name, const std::string& path_project = "."){
        std::string line;
        std::ifstream infile(path_project + "/src/CMakeLists.txt", std::ios::in);
        if (!infile) {
            std::cerr << "Could not open the secondaries CMakeLists.txt files (inside src and test folders)\n";
            return false;
        }
        std::getline(infile, line);
        std::string project_name = line.substr(line.find("(")+1, line.length()-1);
        project_name = project_name.substr(0, project_name.find(")"));
        infile.close();
        *out_project_name = project_name;
        
        if(project_name.length() > 0){
            return true;
        }
        return false;
    }

    bool AddModuleHeadersToMainCMakeListsFile(const std::string& path_module, const std::string& main_path = "."){
        std::vector<std::string> lines;
        std::string line;
        std::ifstream infile(main_path + "/CMakeLists.txt", std::ios::in);
        
        if (!infile) {
            std::cerr << "Could not open the main CMakeLists.txt file\n";
            return false;
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

        if(lines.size() == 0){
            return false;
        }

        std::ofstream outfile(main_path + "/CMakeLists.txt");
        for(size_t i = 0; i < lines.size(); i++){
            outfile << lines.at(i) << ((i == lines.size()-1) ? "" : "\n");
        }
        outfile.close();

        if(!std::experimental::filesystem::v1::exists(main_path + "/CMakeLists.txt")){
            return false;
        }
        return true;
    }

    bool RemoveModuleHeadersFromMainCMakeListsFile(const std::string& path_module, const std::string& main_path = "."){
        std::vector<std::string> lines;
        std::string line;
        std::ifstream infile(main_path + "/CMakeLists.txt", std::ios::in);
        
        if (!infile) {
            std::cerr << "Could not open the main CMakeLists.txt file\n";
            return false;
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

        if(lines.size() == 0){
            return false;
        }

        std::ofstream outfile(main_path + "/CMakeLists.txt");
        for(size_t i = 0; i < lines.size(); i++){
            outfile << lines.at(i) << ((i == lines.size()-1) ? "" : "\n");
        }
        outfile.close();

        if(!std::experimental::filesystem::v1::exists(main_path + "/CMakeLists.txt")){
            return false;
        }
        return true;
    }

    bool AddModuleSourceFilesToSecondaryCMakeListsFile(
        const std::string& module_name, 
        const std::string& source_folder
    ){
        std::vector<std::string> lines;
        std::string line;
        
        std::ifstream infile(source_folder + "/CMakeLists.txt", std::ios::in);
        if (!infile) {
            std::cerr << "Could not open the secondaries (inside src and test folders) CMakeLists.txt files\n";
            return false;
        }
        
        while(!infile.eof()){
            std::getline(infile, line);
            if(line.compare("## End of adding source files ##") == 0){
                if(lines.at(lines.size() - 1).compare("\tfile (GLOB_RECURSE " + module_name + "_source_files " + "../bscxx_modules/" + module_name + "/src/*)") != 0){
                    lines.emplace_back("\tfile (GLOB_RECURSE " + module_name + "_source_files " + "../bscxx_modules/" + module_name + "/src/*)");
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

        if(lines.size() == 0){
            return false;
        }

        std::ofstream outfile("./" + source_folder + "/CMakeLists.txt");
        for(size_t i = 0; i < lines.size(); i++){
            outfile << lines.at(i) << ((i == (int)lines.size()-1) ? "" : "\n");
        }
        outfile.close();

        if(!std::experimental::filesystem::v1::exists("./" + source_folder + "/CMakeLists.txt")){
            return false;
        }
        return true;
    }

    void RemoveModuleSourceFilesToSecondaryCmakeListsFile(
        const std::string& module_name, 
        const std::string& source_folder
    ){
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
                if(line.compare("\tfile (GLOB_RECURSE " + module_name + "_source_files " + "../bscxx_modules/" + module_name + "/src/*)") == 0){
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
        for(size_t i = 0; i < lines.size(); i++){
            outfile << lines.at(i) << ((i == lines.size()-1) ? "" : "\n");
        }
        outfile.close();
    }

    bool AddDependencyUrlToModule(const std::string& module_path, const std::string& module_url){
        std::vector<std::string> lines;
        std::string line;
        std::ifstream infile("./" + module_path + "/dependencies.bscxx", std::ios::in);
        if (!infile) {
            std::cerr << "Could not open the dependencies.bscxx file (inside the " + module_path + " folder)\n";
            return false;
        }

        std::getline(infile, line);
        lines.emplace_back(line);

        std::getline(infile, line);
        if(line.substr(0, line.find("|\t")).length() + 2 < line.length()){
            return true;
        }else{
            if(line.find("|") != std::string::npos){
                line = line.substr(0, line.find("|")+1) + "\t" + module_url;
            }else{
                line += "\t|\t" + module_url;
            }
        }
        lines.emplace_back(line);

        while(!infile.eof()){
            std::getline(infile, line);
            lines.emplace_back(line);
        }
        infile.close();

        std::ofstream outfile("./" + module_path + "/dependencies.bscxx");
        std::string body;
        for(const auto& line_body : lines){
            body += line_body + "\n";
        }
        outfile << body;
        outfile.close();
        return true;
    }

    bool UpdateDependenciesFile(const std::string& project_url = "", const std::string& project_path = "./"){
        std::string line;
        std::ifstream infile(project_path + "src/CMakeLists.txt", std::ios::in);
        if (!infile) {
            std::cerr << "Could not open the secondaries CMakeLists.txt files (inside src and test folders)\n";
            return false;
        }
        std::getline(infile, line);
        std::string project_name = line.substr(line.find("(")+1, line.length()-1);
        project_name = project_name.substr(0, project_name.find(")"));
        infile.close();

        if(project_name.length() == 0){
            return false;
        }

        std::ofstream outfile(project_path + "dependencies.bscxx");
        std::string body;
        body = "BSCXX_PROJECT:\n";
        body += "\t[" + project_name + "]:^1.0.0\t|\t" + project_url + "\n\n";
        body += "BSCXX_DEPENDENCIES:\n";
        for(const auto& p : std::experimental::filesystem::v1::directory_iterator(project_path + "bscxx_modules")){
            std::string module_path = p.path().string();
            std::string line_module;
            std::ifstream infile_module(module_path + "/dependencies.bscxx", std::ios::in);
            if (!infile_module) {
                std::cerr << "Could not open the dependencies.bscxx file (inside the " + module_path + " folder)\n";
                break;
            }
            std::getline(infile_module, line_module);
            std::getline(infile_module, line_module);
            body += line_module + "\n";
            infile_module.close();
        }
        outfile << body;
        outfile.close();

        if(!std::experimental::filesystem::v1::exists(project_path + "dependencies.bscxx")){
            return false;
        }
        return true;      
    }

    bool UpdateGitUrlProject(){
        std::string git_url_project;
        std::experimental::filesystem::v1::path path_git_directory = ".git";
        if(std::experimental::filesystem::v1::exists(path_git_directory)){
            std::string line;
            std::ifstream infile(".git/config", std::ios::in);
            if (!infile) {
                std::cerr << "Could not open the secondaries CMakeLists.txt files (inside src and test folders)\n";
                return false;
            }
            std::size_t found_url;
            while(!infile.eof()){
                std::getline(infile, line);
                found_url = line.find("url = ");
                if(found_url != std::string::npos){ 
                    git_url_project = line.substr(found_url + 6, line.length()-1);
                }
            }
            infile.close();
            UpdateDependenciesFile(git_url_project);
            return true;
        }
        return false;
    }

    bool AddPrenameToHeaderFiles(
        const std::vector<std::string>& include_files,
        const std::string& module_path,
        const std::string& module_name
    ){
        for(auto& p : std::experimental::filesystem::v1::recursive_directory_iterator(module_path + "/include/" + module_name)){
            std::string body;
            std::string file_name = p.path().string();
            std::string extension_file = p.path().string().substr(file_name.find_last_of(".") + 1);
            if(extension_file == "h" || extension_file == "hpp"){
                std::string line;
                std::ifstream infile(file_name, std::ios::in);
                if (!infile) {
                    std::cerr << "Could not open the include files of the " + module_name + " module.\n";
                    return false;
                }
                while(!infile.eof()){
                    std::getline(infile, line);
                    std::size_t found_header;
                    found_header = line.find("#include");
                    if(found_header != std::string::npos){
                        std::for_each(include_files.begin(), include_files.end(), [&](const std::string& name){
                            std::size_t found_include_file;
                            found_include_file = line.find(name);
                            if(found_include_file != std::string::npos){
                                std::string from = name;
                                std::string to = module_name + "/" + name;
                                size_t start_pos = 0;
                                while((start_pos = line.find(from, start_pos)) != std::string::npos) {
                                    line.replace(start_pos, from.length(), to);
                                    start_pos += to.length();
                                }
                            }
                        });
                    }
                    body += line + "\n";
                }
                infile.close();
                std::ofstream outfile(file_name);
                outfile << body;
                outfile.close();
            }
        }
        return true;
    }

    bool AddPrenameToSourceFiles(
        const std::vector<std::string>& include_files,
        const std::string& module_path,
        const std::string& module_name
    ){
        for(auto& p : std::experimental::filesystem::v1::recursive_directory_iterator(module_path + "/src")){
            std::string body;
            std::string file_name = p.path().string();
            std::string extension_file = p.path().string().substr(file_name.find_last_of(".") + 1);
            if(extension_file == "cc" || extension_file == "cpp" || extension_file == "cxx" || extension_file == "c"){
                std::string line;
                std::ifstream infile(file_name, std::ios::in);
                if (!infile) {
                    std::cerr << "Could not open the source files of the " + module_name + " module.\n";
                    return false;
                }
                while(!infile.eof()){
                    std::getline(infile, line);
                    std::size_t found_header;
                    found_header = line.find("#include");
                    if(found_header != std::string::npos){
                        std::for_each(include_files.begin(), include_files.end(), [&](const std::string& name){
                            std::size_t found_include_file;
                            found_include_file = line.find(name);
                            if(found_include_file != std::string::npos){
                                std::string from = name;
                                std::string to = module_name + "/" + name;
                                size_t start_pos = 0;
                                while((start_pos = line.find(from, start_pos)) != std::string::npos) {
                                    line.replace(start_pos, from.length(), to);
                                    start_pos += to.length();
                                }
                            }
                        });
                    }
                    body += line + "\n";
                }
                infile.close();
                std::ofstream outfile(file_name);
                outfile << body;
                outfile.close();
            }
        }
        return true;
    }

    bool ModifyIncludeHeadersSourceFiles(const std::string& module_path, const std::string& module_name){
        std::vector<std::string> include_files;
        for(const auto& p : std::experimental::filesystem::v1::recursive_directory_iterator(module_path + "/include/" + module_name)){
            std::string file_name = p.path().string();
            file_name = file_name.substr(file_name.find("include") + 8 + module_name.length()+1, file_name.length()-1);
            std::string from = "\\";
            std::string to = "/";
            size_t start_pos = 0;
            while((start_pos = file_name.find(from, start_pos)) != std::string::npos) {
                file_name.replace(start_pos, from.length(), to);
                start_pos += to.length();
            }
            std::string file_extension = file_name.substr(file_name.find_last_of(".") + 1);
            if(file_extension == "h" || file_extension == "hpp"){
                include_files.emplace_back(file_name);
            }
        }

        std::thread replace_headers(AddPrenameToHeaderFiles, include_files, module_path, module_name);
        std::thread replace_sources(AddPrenameToSourceFiles, include_files, module_path, module_name);
        replace_headers.join();
        replace_sources.join();
        
        return true;
    }

    bool CreateSubdirectoryIncludeFolder(const std::string& module_path){
        std::string module_name = module_path.substr(module_path.find("/", 14)+1, module_path.length()-1);
        std::string new_include_path = module_path + "/include/" + module_name;
        CreateFolder(module_path + '/' + module_name);
        if(std::experimental::filesystem::v1::exists(module_path + "/include")){
            std::experimental::filesystem::v1::copy(module_path + "/include/", module_path + '/' + module_name, std::experimental::filesystem::v1::copy_options::recursive);
            if(!RemoveFolder(module_path + "/include")){
                return false;
            }
            CreateFolder(module_path + "/include");
            std::experimental::filesystem::v1::copy(module_path + '/' + module_name, module_path + "/include/" + module_name, std::experimental::filesystem::v1::copy_options::recursive);
            if(!RemoveFolder(module_path + '/' + module_name)){
                return false;
            }
            ModifyIncludeHeadersSourceFiles(module_path, module_name);
            return true;
        }
        return false;
    }

    bool MoveSubModulesToMainBSCXXDirectory(const std::string& module_path){
        for(const auto& module : std::experimental::filesystem::v1::directory_iterator(module_path + "/bscxx_modules")){
            std::string submodule_name;
            GetProjectName(&submodule_name, module.path().string());
            if(!std::experimental::filesystem::v1::exists("./bscxx_modules/" + submodule_name)){
                CreateFolder("./bscxx_modules/" + submodule_name);
                std::experimental::filesystem::v1::copy(module_path + "/bscxx_modules/" + submodule_name, "./bscxx_modules/" + submodule_name, std::experimental::filesystem::v1::copy_options::recursive);
            }
            RemoveModuleHeadersFromMainCMakeListsFile("bscxx_modules/" + submodule_name, module_path);
            RemoveModuleSourceFilesToSecondaryCmakeListsFile(submodule_name, module_path + "/src");
            RemoveModuleSourceFilesToSecondaryCmakeListsFile(submodule_name, module_path + "/test");
            AddModuleHeadersToMainCMakeListsFile("bscxx_modules/" + submodule_name);
            AddModuleSourceFilesToSecondaryCMakeListsFile(submodule_name, "src");
            AddModuleSourceFilesToSecondaryCMakeListsFile(submodule_name, "test");
            if(!RemoveFolder(module_path + "/bscxx_modules/" + submodule_name)){
                return false;
            }
        }
        if(!RemoveFolder(module_path + "/bscxx_modules")){
            return false;
        }
        return true;
    }

    bool AddZipModule(const std::string& url, const std::string& module_path, std::string* out_module_name){
        std::string command;
        #if defined(_WIN32)
            command = "(for /f \"delims=\" %a in ('where git') do echo %~dpa > git_location.txt) > nul";
            system(command.c_str());
            std::string line;
            std::ifstream infile("git_location.txt", std::ios::in);
            if (!infile) {
                std::cerr << "Could not open the git_location.txt file\n";
                return false;
            }
            std::getline(infile, line);
            infile.close();
            line.erase(line.end()-1, line.end());
            
            std::string full_url = url;
            if(full_url.at(4) != 's'){
                full_url.insert(4, "s");
            }

            CreateFolder("./new_module");
            command = "echo. && cd new_module && \"" + line + "..\\mingw64\\bin\\curl.exe\" -# -k -O -J " + full_url + " && cd ..";
            system(command.c_str());
            std::string module_name;
            std::string module_path_zip;
            for(const auto& p : std::experimental::filesystem::v1::directory_iterator("new_module")){
                module_path_zip = p.path().string();
                if(!GetProjectName(&module_name, module_path)){
                    RemoveFolder("./new_module");
                    command = "del git_location.txt /s /q > nul";
                    system(command.c_str());
                    return false;
                }
            }
            std::cout << "Module name: " << module_name << "\n";
            command = "\"" + line + "..\\usr\\bin\\unzip.exe\" -q " + module_path_zip + " -d bscxx_modules";
            command += " && del git_location.txt /s /q > nul && del new_module /s /q > nul";
            system(command.c_str());
        #else 
            command = "curl --v"; 
        #endif
        return true;
    }

    bool AddGithubModule(const std::string& github_url, const std::string& module_path, std::string* out_module_name){
        std::string final_path_module = module_path + "/tmp_bscxx/" + github_url.substr(github_url.find("/")+1, github_url.length()-1);
        RemoveFolder(final_path_module);
        
        std::string command = "git clone http://github.com/" + github_url + " " + final_path_module + " >nul";
        system(command.c_str());

        if(!std::experimental::filesystem::v1::exists(final_path_module + "/dependencies.bscxx")){
            std::cout << "\nError: not a bscxx module.\n";
            RemoveFolder(module_path + "/tmp_bscxx");
            return false;
        }

        RemoveFolder(final_path_module + "/.git");
        std::string module_name;
        GetProjectName(&module_name, final_path_module);

        if(std::experimental::filesystem::v1::exists(module_path + "/" + module_name)){
            std::cout << "\nError: " << module_name << " module already exists.\n"
                << "Do you want to remove the folder ? [Y / N]\n";
            std::string answer;
            std::cin >> answer;
            std::transform(answer.begin(), answer.end(), answer.begin(), ::toupper);
            while(answer.compare("Y") != 0 && answer.compare("N") != 0){
                std::cout << "\nPlease, answer by Y (yes) or N (no)\n";
                std::cin >> answer;
                std::transform(answer.begin(), answer.end(), answer.begin(), ::toupper);
            }
            if(answer.compare("Y") == 0){
                RemoveFolder(module_path + "/" + module_name);
            }else{
                return false;
            }
        }

        std::experimental::filesystem::v1::copy(final_path_module, "./bscxx_modules/" + module_name, std::experimental::filesystem::v1::copy_options::recursive);
        RemoveFolder(module_path + "/tmp_bscxx");
        final_path_module = module_path + "/" + module_name;
        AddDependencyUrlToModule(final_path_module, "http://github.com/" + github_url);
        CreateSubdirectoryIncludeFolder(final_path_module);
        MoveSubModulesToMainBSCXXDirectory(final_path_module);
        *out_module_name = module_name;
        return true;
    }

    bool AddLocalModule(
        const std::string& module_path, 
        const std::string& modules_folder,
        std::string* out_module_name
    ){
        if(!std::experimental::filesystem::v1::exists(module_path + "/dependencies.bscxx")){
            return false;
        }
        std::string line;
        std::ifstream infile(module_path + "/src/CMakeLists.txt", std::ios::in);
        if (!infile) {
            std::cerr << "Could not open the CMakeLists.txt file (inside the " + module_path + "/src folder)\n";
            return false;
        }
        std::getline(infile, line);
        std::string project_name = line.substr(line.find("(")+1, line.length()-1);
        project_name = project_name.substr(0, project_name.find(")"));
        *out_module_name = project_name;
        infile.close();
        std::cout << "Adding " + project_name + " module..." << std::endl;
        CreateFolder(modules_folder + project_name);
        std::experimental::filesystem::v1::copy(module_path, modules_folder + project_name, std::experimental::filesystem::v1::copy_options::recursive);
        AddDependencyUrlToModule(modules_folder + project_name, module_path);
        CreateSubdirectoryIncludeFolder(modules_folder + project_name);
        MoveSubModulesToMainBSCXXDirectory(modules_folder + project_name);
        return true;
    }

    inline bool InitializeGit(){
        std::string command = "git init > nul";
        system(command.c_str());

        // Create the gitignore file
        std::ofstream outfile(".gitignore");
        std::string body;
        body += "# BSCXX modules folder\nbscxx_modules\n\n";
        body += "# Build folder\nbuild\n\n";
        outfile << body;
        outfile.close();

        if(!std::experimental::filesystem::v1::exists(".git")){
            return false;
        }

        return true;
    }

    bool DownloadModules(){
        std::string project_name;
        GetProjectName(&project_name);

        std::vector<std::string> modules_url;
        bool module_lines = false;
        std::string line;
        std::ifstream infile("dependencies.bscxx", std::ios::in);
        if (!infile) {
            std::cerr << "Could not open the dependencies.bscxx file\n";
            return false;
        }
        while(!infile.eof()){
            std::getline(infile, line);
            if(module_lines){
                size_t pos_separator = line.find("|");
                if(pos_separator != std::string::npos){
                    std::string module_url = line.substr(pos_separator + 2, line.length()-1);
                    modules_url.emplace_back(module_url);
                }
            }
            if(line.compare("BSCXX_DEPENDENCIES:") == 0){
                module_lines = true;
            }
        }
        infile.close();

        CreateFolder("bscxx_modules");

        for(const auto& s : modules_url){
            if(s.length()-1 > 1){
                size_t pos_website = s.find("http");
                std::string module_name;
                // if it's a module from internet
                if(pos_website != std::string::npos){
                    // if it's a github link
                    if(s.find("github") != std::string::npos){
                        std::string module_url_reduced = s.substr(s.find(".com")+5, s.length()-1);
                        if(!AddGithubModule(module_url_reduced, "bscxx_modules/", &module_name)){
                            std::cout << "Error: the module from " + s + " is not a bscxx module.\n";
                            continue;
                        }
                    }
                    // if it's a url to a zip folder
                    else if(s.find(".zip") != std::string::npos){
                        if(!core::AddZipModule(s, "bscxx_modules/", &module_name)){
                            std::cout << "Error: the module from " + s + " is not a bscxx module.\n";
                            continue;
                        }
                    }
                    
                }
                // if it's a local module
                else{
                     if(!core::AddLocalModule(s, "./bscxx_modules/", &module_name)){
                        std::cout << "Error: the module from " + s + " is not a bscxx module.\n";
                        continue;
                    }
                }
                core::AddModuleHeadersToMainCMakeListsFile("bscxx_modules/" + module_name);
                core::AddModuleSourceFilesToSecondaryCMakeListsFile(module_name, "src");
                core::AddModuleSourceFilesToSecondaryCMakeListsFile(module_name, "test");
                std::cout << "\nModule " + module_name + " correclty installed.\n";
            }
        }
        return true;
    }

    bool GetIncludeModulesInHeaders(std::set<Module>& modules, Module* out_module){
        for(auto& p : std::experimental::filesystem::v1::recursive_directory_iterator(out_module->path + "/include")){
            std::string file_name = p.path().string();
            std::string extension_file = p.path().string().substr(file_name.find_last_of(".") + 1);
            if(extension_file == "h" || extension_file == "hpp"){
                std::string line;
                std::ifstream infile(file_name, std::ios::in);
                if (!infile) {
                    std::cerr << "Could not open the header files of the " + out_module->name + " module.\n";
                    return false;
                }
                while(!infile.eof()){
                    std::getline(infile, line);
                    std::size_t found_header;
                    found_header = line.find("#include");
                    if(found_header != std::string::npos){
                        std::string begining_line = line.substr(0, found_header + 1);
                        // if the include line is not commented
                        if(begining_line.find("//") != std::string::npos
                            || begining_line.find("/*") != std::string::npos){
                                continue;
                        }
                        std::for_each(modules.begin(), modules.end(), [&](const Module& m){
                            std::size_t found_include_file;
                            std::string include_name = "<" + m.name + "/";
                            found_include_file = line.find(include_name);
                            if(found_include_file != std::string::npos){
                                if(out_module->name.compare(m.name) != 0){
                                    out_module->dependencies.insert(m);
                                }
                            }else{
                                include_name = "\"" + m.name + "/";
                                found_include_file = line.find(include_name);
                                if(found_include_file != std::string::npos){
                                    if(out_module->name.compare(m.name) != 0){
                                        out_module->dependencies.insert(m);
                                    }
                                }
                            }
                        });
                    }
                }
                infile.close();
            }
        }
        return true;
    }

    bool GetIncludeModulesInSourceFiles(std::set<Module>& modules, Module* out_module){ 
        for(auto& p : std::experimental::filesystem::v1::recursive_directory_iterator(out_module->path + "/src")){
            std::string file_name = p.path().string();
            std::string extension_file = p.path().string().substr(file_name.find_last_of(".") + 1);
            if(extension_file == "cc" || extension_file == "cpp" || extension_file == "cxx" || extension_file == "c"){
                std::string line;
                std::ifstream infile(file_name, std::ios::in);
                if (!infile) {
                    std::cerr << "Could not open the source files of the " + out_module->name + " module.\n";
                    return false;
                }
                while(!infile.eof()){
                    std::getline(infile, line);
                    std::size_t found_header;
                    found_header = line.find("#include");
                    if(found_header != std::string::npos){
                        std::string begining_line = line.substr(0, found_header + 1);
                        // if the include line is not commented
                        if(begining_line.find("//") != std::string::npos
                            || begining_line.find("/*") != std::string::npos){
                                continue;
                        }
                        std::for_each(modules.begin(), modules.end(), [&](const Module& m){
                            std::size_t found_include_file;
                            std::string include_name = "<" + m.name + "/";
                            found_include_file = line.find(include_name);
                            if(found_include_file != std::string::npos){
                                if(out_module->name.compare(m.name) != 0){
                                    out_module->dependencies.insert(m);
                                }
                            }else{
                                include_name = "\"" + m.name + "/";
                                found_include_file = line.find(include_name);
                                if(found_include_file != std::string::npos){
                                    if(out_module->name.compare(m.name) != 0){
                                        out_module->dependencies.insert(m);
                                    }
                                }
                            }
                        });
                    }
                }
                infile.close();
            }
        }
        return true;
    }

    bool ShowTreeDependenciesModule(
        const std::string& project_path = ".",
        const bool module_dependency = false
    ){
        std::set<Module> all_modules;

        Module main_module;
        main_module.path = project_path;
        GetProjectName(&main_module.name, project_path);
        all_modules.insert(main_module);

        for(const auto& p : std::experimental::filesystem::v1::directory_iterator(project_path + "/bscxx_modules")){
            Module module;
            module.path = p.path().string();
            GetProjectName(&module.name, module.path);
            all_modules.insert(module);
        }

        for(const auto& m : all_modules){
            const Module& const_m = m;
            Module& m2 = const_cast<Module&>(m);
            std::thread dependencies_headers(GetIncludeModulesInHeaders, all_modules, &m2);
            std::thread dependencies_source_files(GetIncludeModulesInSourceFiles, all_modules, &m2);
            dependencies_headers.join();
            dependencies_source_files.join();
        }

        std::function<void(const std::set<Module>& modules, int current_depth)> show_outputs_results;
        show_outputs_results = [&](const std::set<Module>& modules, int current_depth){
            std::for_each(modules.begin(), modules.end(), [&](const Module& module){
                std::string spaces = "   ";
                for(int i = 0; i < current_depth; i++){
                    spaces += "   ";
                }
                std::cout << spaces << "|--" << module.name << "\n";
                if(module.dependencies.size() > 0){
                    std::set<Module>::iterator it = all_modules.find(module);
                    if(it != all_modules.end()){
                        int new_depth = current_depth;
                        new_depth++;
                        show_outputs_results(it->dependencies, new_depth);
                    }
                }
            });
        };
        std::cout << "\n" << main_module.name << "\n";
        std::set<Module>::iterator it = all_modules.find(main_module);
        if(it->dependencies.size() > 0){
            show_outputs_results(it->dependencies, 0);
        }
        return true;
    }

    bool ShowListDependenciesModules(const std::string& project_path = "."){
        std::set<Module> all_modules;

        Module main_module;
        main_module.path = project_path;
        GetProjectName(&main_module.name, project_path);
        std::cout << "\n[Help]\tIn" << green << " green" << white << " (or +): the dependencies used in this project." 
            << "\n\tIn" << red << " red" << white << " (or -): the dependencies unused in this project.\n\n"
            << "List of modules as dependencies for the " << main_module.name << " project:\n"; 
        all_modules.insert(main_module);

        for(const auto& p : std::experimental::filesystem::v1::directory_iterator(project_path + "/bscxx_modules")){
            Module module;
            module.path = p.path().string();
            GetProjectName(&module.name, module.path);
            all_modules.insert(module);
        }

        for(const auto& m : all_modules){
            const Module& const_m = m;
            Module& m2 = const_cast<Module&>(m);
            std::thread dependencies_headers(GetIncludeModulesInHeaders, all_modules, &m2);
            std::thread dependencies_source_files(GetIncludeModulesInSourceFiles, all_modules, &m2);
            dependencies_headers.join();
            dependencies_source_files.join();
        }

        std::set<Module>::iterator it = all_modules.find(main_module);
        std::set<Module> dependencies_used;
        if(it->dependencies.size() > 0){
            std::function<void(const std::set<Module>& modules)> store_dependencies_used;

            store_dependencies_used = [&](const std::set<Module>& modules){
                std::for_each(modules.begin(), modules.end(), [&](const Module& module){
                    dependencies_used.insert(module);
                    std::set<Module>::iterator it_module = all_modules.find(module);
                    if(it_module != all_modules.end()){
                        store_dependencies_used(it_module->dependencies);
                    }
                });
            };
            store_dependencies_used(it->dependencies);

            for(const auto& m1 : all_modules){
                bool module_founded = false;
                if(m1.name == main_module.name){
                    continue;
                }
                for(const auto& m2 : dependencies_used){
                    if(m1.name == m2.name){
                        std::cout << green << "  + " << m2.name << "\n";
                        module_founded = true;
                        break;
                    }
                }
                if(!module_founded){
                    std::cout << red << "  - " << m1.name << "\n";
                }
            }
            std::cout << white << "\n" << all_modules.size()-1 << " modules as dependencies in this project.\n"
                << dependencies_used.size() << " modules as dependencies actually used in this project.\n";
        }else{
            for(const auto& m1 : all_modules){
                if(m1.name == main_module.name){
                    continue;
                }
                std::cout << red << "  - " << m1.name << "\n";
            }
        }
        return true;
    }

}// namespace core