#pragma once

#include <core/common.h>
#include <core/generator.h>
#include <stdexcept>
#include <create_project.h>
#include <addons/console_color.h>

namespace test {

    bool CreateDefaultProject(const std::string& project_name, int *out_test_passed){
        bool return_state = true;
        
        if(!core::CreateFolder(project_name)){
            std::cout << "- Create the " << project_name << " folder." << red << " error" << white << "\n";
            return_state = false;
        }else{
            std::cout << "- Create the " << project_name << " folder." << green << " done" << white << "\n";
            (*out_test_passed)++;
        }

        if(!core::InitializeGit()){
            std::cout << "- Initialize the git repository inside the " << project_name << " folder."
                << red << " error" << white << "\n";
            return_state = false;
        }else{
            std::cout << "- Initialize the git repository inside the " << project_name << " folder."
                << green << " done" << white << "\n";
            (*out_test_passed)++;
        }
        
        if(!core::CreateMainCmakeListsFile("./" + project_name + "/")){
            std::cout << "- Create the main CMakeLists.txt file." << red << " error" << white << "\n";
            return_state = false;
        }else{
            std::cout << "- Create the main CMakeLists.txt file." << green << " done" << white << "\n";
            (*out_test_passed)++;
        }
        
        if(!core::CreateFolder(project_name + "/src")){
            std::cout << "- Create the src folder." << red << " error" << white << "\n";
            return_state = false;
        }else{
            std::cout << "- Create the src folder." << green << " done" << white << "\n";
            (*out_test_passed)++;
        }
        
        if(!core::CreateFolder(project_name + "/include")){
            std::cout << "- Create the include folder." << red << " error" << white << "\n";
            return_state = false;
        }else{
            std::cout << "- Create the include folder." << green << " done" << white << "\n";
            (*out_test_passed)++;
        }

        if(!core::CreateFolder(project_name + "/test")){
            std::cout << "- Create the test folder." << red << " error" << white << "\n";
            return_state = false;
        }else{
            std::cout << "- Create the test folder." << green << " done" << white << "\n";
            (*out_test_passed)++;
        }

        if(!core::CreateMainFile(project_name + "/src")){
            std::cout << "- Create the main.cc file inside the src folder." << red << " error" << white << "\n";
            return_state = false;
        }else{
            std::cout << "- Create the main.cc file inside the src folder." << green << " done" << white << "\n";
            (*out_test_passed)++;
        }
        
        if(!core::CreateTestMainFile(project_name + "/test")){
            std::cout << "- Create the test.cc file inside the test folder." << red << " error" << white << "\n";
            return_state = false;
        }else{
            std::cout << "- Create the test.cc file inside the test folder." << green << " done" << white << "\n";
            (*out_test_passed)++;
        }

        if(!core::CreateSecondaryCMakeListsFile(project_name + "/src", project_name)){
            std::cout << "- Create the CMakeLists.txt file inside the src folder." << red << " error" << white << "\n";
            return_state = false;
        }else{
            std::cout << "- Create the CMakeLists.txt file inside the src folder." << green << " done" << white << "\n";
            (*out_test_passed)++;
        }

        if(!core::CreateTestCMakeListsFile(project_name + "/test", project_name)){
            std::cout << "- Create the CMakeLists.txt file inside the test folder." << red << " error" << white << "\n";
            return_state = false;
        }else{
            std::cout << "- Create the CMakeLists.txt file inside the test folder." << green << " done" << white << "\n";
            (*out_test_passed)++;
        }

        if(!core::CreateFolder(project_name + "/bscxx_modules")){
            std::cout << "- Create the bscxx_modules folder." << red << " error" << white << "\n";
            return_state = false;
        }else{
            std::cout << "- Create the bscxx_modules folder." << green << " done" << white << "\n";
            (*out_test_passed)++;
        }

        if(!core::UpdateDependenciesFile("", project_name + "/")){
            std::cout << "- Update the dependencies in the dependencies.bscxx file." << red << " error" << white << "\n";
            return_state = false;
        }else{
            std::cout << "- Update the dependencies in the dependencies.bscxx file." << green << " done" << white << "\n";
            (*out_test_passed)++;
        }
        
        return return_state;
    }

    bool BuildAndRunDefaultProject(const std::string& project_name, int *out_test_passed){
        bool return_state = true;

        std::string command;
        command = "cd " + project_name + " && mkdir build && cd build ";
        command += " && cmake ..";
        command += " && cmake --build . --config Release";
        command += " && \"src/bin/release/" + project_name + ".exe\" > log_project.txt";
        command += " && cd ../..";
        system(command.c_str());

        std::string line;
        std::ifstream infile(project_name + "/build/log_project.txt", std::ios::in);
        if (!infile) {
            std::cerr << red << "Error when trying to open the log_project.txt file.\n";
            return_state = false;
        }
        std::getline(infile, line);
        infile.close();
        if(line.compare("Running the app...") != 0){
            std::cout << "- - Build & run the default abc project." << red << " error" << white << "\n";
            return_state = false;
        }else{
            std::cout << "- Build & run the default abc project." << green << " done" << white << "\n";
            (*out_test_passed)++;
        }
        
        return return_state;
    }

} // end namespace test