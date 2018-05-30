#pragma once

#include <core/common.h>

namespace core{

    /**************/
    /* STRUCTURES */
    /**************/
    struct Module{
        friend bool operator< (const Module& left, const Module& right);
        std::set<Module> dependencies;
        std::set<Module> parent_modules;
        std::string name;
        std::string path;
        std::string url;
    };

    bool operator< (const Module& left, const Module& right){
        return left.name != right.name;
    }

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

    inline bool RemoveFolder(const std::string& path){
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

    inline bool GetProjectName(std::string* out_project_name, const std::string& path_project = "."){
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

    inline bool AddModuleHeadersToMainCMakeListsFile(const std::string& path_module, const std::string& main_path = "."){
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

    inline bool RemoveModuleHeadersFromMainCMakeListsFile(const std::string& path_module, const std::string& main_path = "."){
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

    inline bool AddModuleSourceFilesToSecondaryCMakeListsFile(
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

    inline void RemoveModuleSourceFilesToSecondaryCmakeListsFile(
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

    inline bool AddDependencyUrlToModule(const std::string& module_path, const std::string& module_url){
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

    inline bool UpdateDependenciesFile(const std::string& project_url = "", const std::string& project_path = "./"){
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

    inline bool UpdateGitUrlProject(){
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

    inline bool AddPrenameToHeaderFiles(
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

    inline bool AddPrenameToSourceFiles(
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

    inline bool ModifyIncludeHeadersSourceFiles(const std::string& module_path, const std::string& module_name){
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

    inline bool CreateSubdirectoryIncludeFolder(const std::string& module_path){
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

    inline bool MoveSubModulesToMainBSCXXDirectory(const std::string& module_path){
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

    inline bool AddZipModule(const std::string& url, const std::string& module_path, std::string* out_module_name){
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

    inline bool AddGithubModule(const std::string& github_url, const std::string& module_path, std::string* out_module_name){
        std::string final_path_module = module_path + github_url.substr(github_url.find("/")+1, github_url.length()-1);
        std::string command = "git clone http://github.com/" + github_url + " " + final_path_module + "> null && rm -r null";
        system(command.c_str());
        
        if(!std::experimental::filesystem::v1::exists(final_path_module + "/dependencies.bscxx")){
            return false;
        }
        RemoveFolder(final_path_module + "/.git");
        std::string module_name;
        GetProjectName(&module_name, final_path_module);
        std::cout << "Module name: " << module_name + "\n";
        CreateFolder("./bscxx_modules/" + module_name);
        std::experimental::filesystem::v1::copy(final_path_module, "./bscxx_modules/" + module_name, std::experimental::filesystem::v1::copy_options::recursive);
        RemoveFolder(final_path_module);
        final_path_module = module_path + "/" + module_name;
        AddDependencyUrlToModule(final_path_module, "http://github.com/" + github_url);
        CreateSubdirectoryIncludeFolder(final_path_module);
        MoveSubModulesToMainBSCXXDirectory(final_path_module);
        *out_module_name = module_name;
        return true;
    }

    inline bool AddLocalModule(
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
        AddDependencyUrlToModule(modules_folder + project_name, "local_module");
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

    inline bool DownloadModules(){
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

        for(const auto& s : modules_url){
            if(s.length()-1 > 1){
                std::string module_url_reduced = s.substr(s.find(".com")+5, s.length()-1);
                std::string module_name;
                CreateFolder("bscxx_modules");
                if(!AddGithubModule(module_url_reduced, "bscxx_modules/", &module_name)){
                    std::cout << "Not a bscxx module repository.\n";
                    return false;
                }
                std::cout << "Module correclty added, project updated.\n";
            }
        }
        return true;
    }

    inline bool ShowTreeDependenciesModule(const std::string& project_path = "."){
        std::string project_name;
        GetProjectName(&project_name, project_path);
        std::cout << "\n" << project_name << "\n";

        std::set<Module> modules;
        for(const auto& p : std::experimental::filesystem::v1::directory_iterator(project_path + "/bscxx_modules")){
            Module module;
            module.path = p.path().string();
            if(!GetProjectName(&module.name, module.path)){
                std::cerr << "\nCould not get the name of the module inside " + module.path + "\n";
                return false;
            }
            std::string line;
            std::ifstream infile(module.path + "/dependencies.bscxx", std::ios::in);
            if (!infile) {
                std::cerr << "\nCould not open the dependencies.bscxx file inside " + module.path + "/dependencies.bscxx\n";
                return false;
            }

            bool module_lines = false;
            while(!infile.eof()){
                std::getline(infile, line);
                if(module_lines){
                    size_t pos_separator = line.find("]");
                    if(pos_separator != std::string::npos){
                        Module dependency;
                        dependency.name = line.substr(2, pos_separator - 2);
                        dependency.parent_modules.insert(module);
                        module.dependencies.insert(dependency);
                        modules.insert(module);
                        modules.insert(dependency);
                    }
                }
                if(line.compare("BSCXX_DEPENDENCIES:") == 0){
                    module_lines = true;
                }
            }
            infile.close();
            modules.insert(module);
        }

        std::function<void(std::set<Module>& modules, bool show_module, int current_depth)> show_outputs_results;
        show_outputs_results = [&](std::set<Module>& modules, bool show_module, int current_depth){
            std::for_each(modules.begin(), modules.end(), [&](const Module& module){ 
                if(module.dependencies.size() == 0 && module.parent_modules.size() == 0){
                    std::cout << "|--" << module.name << "\n";
                    return;
                }
                if(module.dependencies.size() > 0 && module.parent_modules.size() == 0){
                    std::cout << "|--" << module.name << "\n";
                    std::set<Module> dependencies;
                    dependencies.insert(module.dependencies.begin(), module.dependencies.end());
                    show_outputs_results(dependencies, true, current_depth++);
                }else{
                    if(show_module){
                        std::string spaces = "   ";
                        for(int i = 0; i < current_depth; i++){
                            spaces += "   ";
                        }
                        std::cout << spaces << "|--" << module.name << "\n";
                        if(module.dependencies.size() > 0){
                            std::set<Module> dependencies;
                            dependencies.insert(module.dependencies.begin(), module.dependencies.end());
                            show_outputs_results(dependencies, true, current_depth++);
                        }
                    }
                }
            });
        };

        show_outputs_results(modules, false, 0);
        

        return true;
    }

}// namespace core