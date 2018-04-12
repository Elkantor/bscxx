#include <core/common.h>
#include <core/generater.h>

int main(int argc, char* argv[]){
     
    if(argv[1] != nullptr){
        
        /************************/
        /* Create a new project */
        /************************/ 
        if(strcmp(argv[1], "create") == 0){
            std::cout << "\n\t[Create a new project]\n\n";
            if(argv[2] != nullptr){
                core::CreateMainCmakeListsFile("./");
                core::CreateFolder("src");
                core::CreateFolder("include");
                core::CreateFolder("lib");
                core::CreateFolder("test");
                core::CreateMainFile("./src");
                if(argv[3] != nullptr){
                    if(strcmp(argv[3], "--dynamic_lib") == 0 || strcmp(argv[3], "-dl") == 0){
                        core::CreateSecondaryCMakeListsFile("./src", argv[2], core::ProjectType::DYNAMIC_LIBRARY);
                    }else if(strcmp(argv[3], "--static_lib") == 0 || strcmp(argv[3], "-sl") == 0){
                        core::CreateSecondaryCMakeListsFile("./src", argv[2], core::ProjectType::STATIC_LIBRARY);
                    }else{
                        core::CreateSecondaryCMakeListsFile("./src", argv[2]);
                    }
                }else{
                    core::CreateSecondaryCMakeListsFile("./src", argv[2]);
                }
                core::CreateTestCMakeListsFile("./test", argv[2]);
            }
        }

        /*********************************************/
        /* Add a package to the project dependencies */
        /*********************************************/ 
        else if(strcmp(argv[1], "addmodule") == 0 
            || strcmp(argv[1], "-am") == 0){
            std::cout << "\n\t[Add a new module (" << argv[2] << ") dependency to the project]\n\n";
            if(argv[2] != nullptr){
                std::string module_name;
                if(argv[3] != nullptr){
                    module_name = argv[3];
                }

                /****************************/
                /* If creating a new module */
                /****************************/
                if(strcmp(argv[2], "--new") == 0 || strcmp(argv[2], "-n") == 0){
                    core::CreateFolder("bscxx_modules");
                    core::CreateFolder("bscxx_modules/" + module_name);
                    core::CreateMainCmakeListsFile("./bscxx_modules/" + module_name + "/");
                    core::CreateFolder("bscxx_modules/" + module_name + "/src");
                    core::CreateFolder("bscxx_modules/" + module_name + "/include");
                    core::CreateFolder("bscxx_modules/" + module_name + "/lib");
                    core::CreateFolder("bscxx_modules/" + module_name + "/test");
                    core::CreateMainFile("bscxx_modules/" + module_name + "/src");
                    core::CreateSecondaryCMakeListsFile("bscxx_modules/" + module_name + "/src", module_name);
                    core::AddModuleHeadersToMainCMakeListsFile("bscxx_modules/" + module_name);
                    core::AddModuleSourceFilesToSecondaryCmakeListsFile(module_name, "src");
                }
            }
        }

        /********************/
        /* To test features */
        /********************/ 
        else if(strcmp(argv[1], "test") == 0){
            std::cout << "\n\t [Test]";
        }

        /************************/
        /* Show the app Version */
        /************************/
        else if(strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0){
            std::cout << "\nBSCXX version 1.0.0";
        }

        /*****************/
        /* Show the help */
        /*****************/
        else if(strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0){
            std::cout << "\n\nCommands:";
            std::cout << "\n\ncreate\t\tCreate a new C++ project";
            std::cout << "\naddmodule\tAdd a C++ module in the project dependencies";
            std::cout << "\n";
        }

        /***********/
        /* Default */
        /***********/ 
        else {
            std::cout << "\n\t# [ERROR] Command not valid.";
            std::cout << "\n\nHere is the list of all the available commands:";
            std::cout << "\n\tcreate : to create a new C++ project";
            std::cout << "\n\t=> app create [project_name] [--exe (by default) | --static_lib | --dynamic_lib]";
            std::cout << "\n\n\taddmodule : to add a C++ module in the project dependencies";
            std::cout << "\n\t=> app addmodule [--new] [module_name]";
        }
    }

    return 0;
};