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
                core::CreateTestMainFile("./test");
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
        else if(strcmp(argv[1], "add") == 0 
            || strcmp(argv[1], "-am") == 0){
            if(argv[2] != nullptr){
                std::string module_name;
                if(argv[3] != nullptr){
                    module_name = argv[3];
                }

                /*************************/
                /* Show the help command */
                /*************************/
                if(strcmp(argv[2], "-h") == 0 || strcmp(argv[2], "--help") == 0){
                    std::cout 
                        << "\nAdd a C++ module in the project's dependencies (located in ./bscxx_modules)"
                        << "\n\nUsage: bscxx add [OPTION] [module_name]"
                        << "\n\nOptions:"
                        << "\n\t--new\t Create a new module skeleton"
                        << "\n";
                }

                /****************************/
                /* If creating a new module */
                /****************************/
                else if(strcmp(argv[2], "--new") == 0 || strcmp(argv[2], "-n") == 0){
                    core::CreateFolder("bscxx_modules");
                    core::CreateFolder("bscxx_modules/" + module_name);
                    core::CreateMainCmakeListsFile("./bscxx_modules/" + module_name + "/");
                    core::CreateFolder("bscxx_modules/" + module_name + "/src");
                    core::CreateFolder("bscxx_modules/" + module_name + "/include");
                    core::CreateFolder("bscxx_modules/" + module_name + "/lib");
                    core::CreateFolder("bscxx_modules/" + module_name + "/test");
                    core::CreateMainFile("bscxx_modules/" + module_name + "/src");
                    core::CreateTestMainFile("bscxx_modules/" + module_name + "/test");
                    core::CreateSecondaryCMakeListsFile("bscxx_modules/" + module_name + "/src", module_name);
                    core::CreateSecondaryCMakeListsFile("bscxx_modules/" + module_name + "/test", module_name);
                    core::AddModuleHeadersToMainCMakeListsFile("bscxx_modules/" + module_name);
                    core::AddModuleSourceFilesToSecondaryCMakeListsFile(module_name, "src");
                    core::AddModuleSourceFilesToSecondaryCMakeListsFile(module_name, "test");
                }
            }
        }

        /**************************************************/
        /* Remove a package from the project dependencies */
        /**************************************************/
        else if(strcmp(argv[1], "remove") == 0 
            || strcmp(argv[1], "-rmm") == 0){
            if(argv[2] != nullptr){
                std::string module_name;
                module_name = argv[2];

                /*************************/
                /* Show the help command */
                /*************************/
                if(strcmp(argv[2], "-h") == 0 || strcmp(argv[2], "--help") == 0){
                    std::cout 
                        << "\nRemove a C++ module from the project's dependencies (located in ./bscxx_modules)"
                        << "\n\nUsage: bscxx remove [OPTION] [module_name]"
                        << "\n";
                }

                /*******************/
                /* Remove a module */
                /*******************/
                else{
                    core::RemoveFolder("bscxx_modules/" + module_name);
                    core::RemoveModuleHeadersFromMainCMakeListsFile("bscxx_modules/" + module_name);
                    core::RemoveModuleSourceFilesToSecondaryCmakeListsFile(module_name, "src");
                }
            }
        }

        /************************/
        /* Show the app Version */
        /************************/
        else if(strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0){
            std::cout << "\nBSCXX version 1.0.0\n";
        }

        /*****************/
        /* Show the help */
        /*****************/
        else if(strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0){
            std::cout << "\n\nCommands:";
            std::cout << "\n\ncreate\t\tCreate a new C++ project";
            std::cout << "\nadd\tAdd a C++ module in the project dependencies";
            std::cout << "\n";
        }

        /***********/
        /* Default */
        /***********/ 
        else {
            std::cout << "\n\t# [ERROR] Command not valid.";
            std::cout << "\n\nHere is the list of all the available commands:";
            std::cout << "\n\ncreate\t\tCreate a new C++ project";
            std::cout << "\nadd\tAdd a C++ module in the project dependencies";
            std::cout << "\n";
        }
    }

    return 0;
};