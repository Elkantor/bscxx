#include <core/common.h>
#include <core/generator.h>

int main(int argc, char* argv[]){
     
    if(argv[1] != nullptr){
        
        /************************/
        /* Create a new project */
        /************************/ 
        if(strcmp(argv[1], "create") == 0){
            if(argv[2] != nullptr){

                /*************************/
                /* Show the help command */
                /*************************/
                if(strcmp(argv[2], "-h") == 0 || strcmp(argv[2], "--help") == 0){
                    std::cout 
                        << "\nCreate a new C++ project (can be used as a module in an other project too)"
                        << "\n\nUsage: bscxx create [project_name]"
                        << "\n";
                }
                
                /*****************************/
                /* If creating a new project */
                /*****************************/ 
                else{
                    core::InitializeGit();
                    core::CreateMainCmakeListsFile("./");
                    core::CreateFolder("src");
                    core::CreateFolder("include");
                    core::CreateFolder("test");
                    core::CreateMainFile("./src");
                    core::CreateTestMainFile("./test");
                    core::CreateSecondaryCMakeListsFile("./src", argv[2]);
                    core::CreateTestCMakeListsFile("./test", argv[2]);
                    core::CreateFolder("bscxx_modules");
                    core::UpdateDependenciesFile();
                }
            }

            else{
                /***********/
                /* Default */
                /***********/ 
                std::cout 
                    << "\nError: project name can't be empty"
                    << "\n\nUsage: bscxx create [project_name]\n";
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
                        << "\n\t--local \tAdd a new module from a local folder"
                        << "\n\t--github\tAdd a new module from a github repository"
                        << "\n\t--zip   \tAdd a new module from a zip url"
                        << "\n";
                }

                /*****************************/
                /* If adding a github module */
                /*****************************/
                else if(strcmp(argv[2], "--github") == 0){
                    if(argv[3] != nullptr){

                        /*************************/
                        /* Show the help command */
                        /*************************/
                        if(strcmp(argv[3], "-h") == 0 || strcmp(argv[3], "--help") == 0){
                            std::cout 
                                << "\nAdd a C++ module from github in the project's dependencies (located in ./bscxx_modules)"
                                << "\n\nNote: if the github project's url is \"https://github.com/Elkantor/bscxx\" for example"
                                << " the github_path is \"Elkantor/bscxx\".\n"
                                << "\n\nUsage: bscxx add --github [github_path]"
                                << "\n";
                        }

                        /*************************/
                        /* Add the github module */
                        /*************************/
                        else{
                            std::string module_name;
                            core::CreateFolder("bscxx_modules");
                            if(!core::AddGithubModule(argv[3], "bscxx_modules/", &module_name)){
                                return false;
                            }
                            core::AddModuleHeadersToMainCMakeListsFile("bscxx_modules/" + module_name);
                            core::AddModuleSourceFilesToSecondaryCMakeListsFile(module_name, "src");
                            core::AddModuleSourceFilesToSecondaryCMakeListsFile(module_name, "test");
                            core::UpdateDependenciesFile();
                            std::cout << "Module correclty added, project updated.\n";
                        }
                    }
                }

                /*****************************/
                /* If adding a local module */
                /*****************************/
                else if(strcmp(argv[2], "--local") == 0){
                    if(argv[3] != nullptr){
                        std::string module_name;
                        core::CreateFolder("bscxx_modules");
                        if(!core::AddLocalModule(argv[3], "./bscxx_modules/", &module_name)){
                            std::cout << "No module available for this path.\n";
                            return false;
                        }
                        core::AddModuleHeadersToMainCMakeListsFile("bscxx_modules/" + module_name);
                        core::AddModuleSourceFilesToSecondaryCMakeListsFile(module_name, "src");
                        core::AddModuleSourceFilesToSecondaryCMakeListsFile(module_name, "test");
                        core::UpdateDependenciesFile();
                        std::cout << "Module correclty added, project updated.\n";
                    }
                }

                /***********************************/
                /* If adding a zip module from url */
                /***********************************/
                else if(strcmp(argv[2], "--zip") == 0){
                    if(argv[3] != nullptr){
                        std::string module_name;
                        core::CreateFolder("bscxx_modules");
                        if(!core::AddZipModule(argv[3], "bscxx_modules/", &module_name)){
                            std::cout << "Error: Not a bscxx module repository.\n";
                            return false;
                        }
                        core::AddModuleHeadersToMainCMakeListsFile("bscxx_modules/" + module_name);
                        core::AddModuleSourceFilesToSecondaryCMakeListsFile(module_name, "src");
                        core::AddModuleSourceFilesToSecondaryCMakeListsFile(module_name, "test");
                        core::UpdateDependenciesFile();
                        std::cout << "Module correclty added, project updated.\n";
                    }
                }
            }
        }

        /****************************/
        /* Install the dependencies */
        /****************************/
        else if(strcmp(argv[1], "install") == 0){
            if(argv[2] != nullptr){
                /*************************/
                /* Show the help command */
                /*************************/
                if(strcmp(argv[2], "-h") == 0 || strcmp(argv[2], "--help") == 0){
                    std::cout 
                        << "\nDownload and add to the project all the modules present in the dependencies.bscxx file."
                        << "\n\nUsage: bscxx install"
                        << "\n";
                }
            }
            else{
                core::DownloadModules();
            }
        }

        /********************************/
        /* Update the dependencies file */
        /********************************/
        else if(strcmp(argv[1], "update") == 0){
            if(argv[2] != nullptr){
                /*************************/
                /* Show the help command */
                /*************************/
                if(strcmp(argv[2], "-h") == 0 || strcmp(argv[2], "--help") == 0){
                    std::cout 
                        << "\nUpdate the git repository url of this project."
                        << "\n\nUsage: bscxx update"
                        << "\n";
                        return 0;
                }
            }
            else{
                core::UpdateGitUrlProject();
            }
        }

        /*****************************************/
        /* Show the tree of modules dependencies */
        /*****************************************/
        else if(strcmp(argv[1], "tree") == 0){
            /***********************************/
            /* If it's in the project's folder */
            /***********************************/
            if(argv[2] == nullptr){
                core::ShowTreeDependenciesModule(".");
            }
            else{
                /*************************/
                /* Show the help command */
                /*************************/
                if(strcmp(argv[2], "-h") == 0 || strcmp(argv[2], "--help") == 0){
                    std::cout 
                        << "\nGenerate and show a tree of the dependencies used in this project, by scanning your code."
                        << "\n\nUsage: bscxx tree | bscxx tree [path_bscxx_project]"
                        << "\n";
                }

                /*********************************/
                /* If it's for a specific module */
                /*********************************/
                else{
                    core::ShowTreeDependenciesModule(argv[2]);
                }
            }
        }

        /*****************************/
        /* Show the list of packages */
        /*****************************/
        else if(strcmp(argv[1], "list") == 0){
            /***********************************/
            /* If it's in the project's folder */
            /***********************************/
            if(argv[2] == nullptr){
                core::ShowListDependenciesModules(".");
            }

            else{
                /*************************/
                /* Show the help command */
                /*************************/
                if(strcmp(argv[2], "-h") == 0 || strcmp(argv[2], "--help") == 0){
                    std::cout 
                        << "\nList all the modules installed, and show the used and unused modules, by scanning your code."
                        << "\n\nUsage: bscxx list | bscxx list [path_to_project]"
                        << "\n";
                }
                /*********************************/
                /* If it's for a specific module */
                /*********************************/
                else{
                    core::ShowListDependenciesModules(argv[2]);
                }
            }
        }

        /**************************************************/
        /* Remove a package from the project dependencies */
        /**************************************************/
        else if(strcmp(argv[1], "remove") == 0 
            || strcmp(argv[1], "-rm") == 0){
            if(argv[2] != nullptr){
                std::string module_name;
                module_name = argv[2];

                /*************************/
                /* Show the help command */
                /*************************/
                if(strcmp(argv[2], "-h") == 0 || strcmp(argv[2], "--help") == 0){
                    std::cout 
                        << "\nRemove a C++ module from the project's dependencies (located in ./bscxx_modules)"
                        << "\nUsage: bscxx remove [module_name]\n";
                }

                /*******************/
                /* Remove a module */
                /*******************/
                else{
                    core::RemoveFolder("bscxx_modules/" + module_name);
                    core::RemoveModuleHeadersFromMainCMakeListsFile("bscxx_modules/" + module_name);
                    core::RemoveModuleSourceFilesToSecondaryCmakeListsFile(module_name, "src");
                    core::RemoveModuleSourceFilesToSecondaryCmakeListsFile(module_name, "test");
                    core::UpdateDependenciesFile();  
                }
            }
        }

        /************************/
        /* Show the app version */
        /************************/
        else if(strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0){
            std::cout << "\nBSCXX version 0.1.3\n";
        }

        /*****************/
        /* Show the help */
        /*****************/
        else if(strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0){
            std::cout 
                << "\n\nCommands:"
                << "\n\ncreate\tCreate a new C++ project"
                << "\nadd\tAdd a C++ module to the project dependencies"
                << "\nremove\tRemove a C++ module from the project dependencies"
                << "\ninstall\tInstall the modules dependencies"
                << "\ntree\tGenerate the tree of the modules used as dependencies"
                << "\nlist\tList all the modules installed in this project and show if they are used or not"
                << "\nupdate\tUpdate the url of this project in the dependencies file (dependencies.bscxx)\n"
                << "\nSee the help for more details : bscxx [command] --help\n";
        }

        /***********/
        /* Default */
        /***********/ 
        else {
            std::cout 
                << "\nError: bscxx '" << argv[1] << "' is not a valid command."
                << "\n\nAvailable commands:"
                << "\n\ncreate\tCreate a new C++ project"
                << "\nadd\tAdd a C++ module in the project dependencies"
                << "\nremove\tRemove a C++ module from the project dependencies"
                << "\ninstall\tInstall the modules dependencies"
                << "\ntree\tGenerate the tree of the modules used as dependencies"
                << "\nlist\tList all the modules installed in this project and show if they are used or not"
                << "\nupdate\tUpdate the url of this project in the dependencies file (dependencies.bscxx)\n"
                << "\nSee the help for more details : bscxx [command] --help\n";
        }
    }

    return 0;
};