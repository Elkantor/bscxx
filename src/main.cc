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
                core::CreateMainCmakeListsFile();
                core::CreateFolder("src");
                core::CreateFolder("include");
                core::CreateFolder("lib");
                core::CreateFolder("test");
                core::CreateMainFile();
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
            std::cout << "\n\t[Create a new project]\n\n";
            if(argv[2] != nullptr){
                // core::CreateSecondaryCMakeListsFile("./src", argv[2]);
                // core::CreateFolder("src");
                // core::CreateFolder("include");
                // core::CreateFolder("lib");
                // core::CreateFolder("test");
                // core::CreateMainFile();
            }
        }

        /***********/
        /* Default */
        /***********/ 
        else {
            std::cout << "\n\t# [ERROR] Command not valid.";
            std::cout << "\n\nHere is the list of all the commands available :";
            std::cout << "\n\tcreate : to create a new C++ project";
            std::cout << "\n\t=> app create [project_name] [executable | staticlib | dynamiclib]";
            std::cout << "\n";
            std::cout << "\n\taddmodule : to add a C++ module in the project dependencies";
        }
    }

    return 0;
};