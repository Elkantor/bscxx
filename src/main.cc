#include <core/common.h>
#include <core/generater.h>

int main(int argc, char* argv[]){
    // 

    std::cout << "\n\t[Create a new project]\n\n";
    if(argv[1] != nullptr){
        /**
        *   Create a new project
        **/ 
        if(argv[2] != nullptr){
            core::CreateCmakeListsFile();
            core::CreateFolder("src");
            core::CreateFolder("include");
            core::CreateFolder("lib");
            core::CreateFolder("test");
            core::CreateMainFile();
        }
    }

    return 0;
};