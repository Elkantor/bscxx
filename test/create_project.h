#pragma once

#include <core/common.h>
#include <core/generator.h>
#include <stdexcept>
#include <create_project.h>

namespace test {

    inline bool CreateDefaultProject(const std::string& project_name, int *out_test_passed){
        if(!core::CreateFolder(project_name)){
            throw std::runtime_error("Can't create the " + project_name + " folder.");
            return EXIT_FAILURE;
        }
        std::cout << "- Create the " << project_name << " folder.\n";
        (*out_test_passed)++;

        if(!core::InitializeGit()){
            throw std::runtime_error("Can't initialize git repository for " + project_name + " project.");
            return EXIT_FAILURE;
        }
        std::cout << "- Initialize the git repository inside the " << project_name << " folder.\n";
        (*out_test_passed)++;
        
        if(!core::CreateMainCmakeListsFile("./" + project_name + "/")){
            throw std::runtime_error("Can't create the main CMakeLists.txt file.");
            return EXIT_FAILURE;
        }
        std::cout << "- Create the main CMakeLists.txt file.\n";
        (*out_test_passed)++;
        
        if(!core::CreateFolder(project_name + "/src")){
            throw std::runtime_error("Can't create the " + project_name + "/src folder.");
            return EXIT_FAILURE;
        }
        std::cout << "- Create the src folder.\n";
        (*out_test_passed)++;
        
        if(!core::CreateFolder(project_name + "/include")){
            throw std::runtime_error("Can't create the " + project_name + "/include folder.");
            return EXIT_FAILURE;
        }
        std::cout << "- Create the include folder.\n";
        (*out_test_passed)++;

        if(!core::CreateFolder(project_name + "/test")){
            throw std::runtime_error("Can't create the " + project_name + "/test folder.");
            return EXIT_FAILURE;
        }
        std::cout << "- Create the test folder.\n";
        (*out_test_passed)++;

        if(!core::CreateMainFile(project_name + "/src")){
            throw std::runtime_error("Can't create the main.cc file inside " + project_name + "/src folder.");
            return EXIT_FAILURE;
        }
        std::cout << "- Create the main.cc file inside the src folder.\n";
        (*out_test_passed)++;
        
        if(!core::CreateTestMainFile(project_name + "/test")){
            throw std::runtime_error("Can't create the test.cc file inside " + project_name + "/test folder.");
            return EXIT_FAILURE;
        }
        std::cout << "- Create the test.cc file inside the test folder.\n";
        (*out_test_passed)++;

        if(!core::CreateSecondaryCMakeListsFile(project_name + "/src", project_name)){
            throw std::runtime_error("Can't create the CMakeLists.txt file inside " + project_name + "/src folder.");
            return EXIT_FAILURE;
        }
        std::cout << "- Create the CMakeLists.txt file inside the src folder.\n";
        (*out_test_passed)++;

        if(!core::CreateTestCMakeListsFile(project_name + "/test", project_name)){
            throw std::runtime_error("Can't create the CMakeLists.txt file inside " + project_name + "/test folder.");
            return EXIT_FAILURE;
        }
        std::cout << "- Create the CMakeLists.txt file inside the test folder.\n";
        (*out_test_passed)++;

        if(!core::CreateFolder(project_name + "/bscxx_modules")){
            throw std::runtime_error("Can't create the bscxx_modules folder.");
            return EXIT_FAILURE;
        }
        std::cout << "- Create the bscxx_modules folder.\n";
        (*out_test_passed)++;

        if(!core::UpdateDependenciesFile("", project_name + "/")){
            throw std::runtime_error("Can't update the dependencies in the dependencies.bscxx file.");
            return EXIT_FAILURE;
        }
        std::cout << "- Update the dependencies in the dependencies.bscxx file.\n";
        (*out_test_passed)++;
        
        return true;
    }

} // end namespace test