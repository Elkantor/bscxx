#include <core/common.h>
#include <core/generator.h>
#include <create_project.h>

int main(int argc, char* argv[]){ 

    std::cout << "Performing tests...\n";

    /**************************************/
    /* Test the creation of a new project */
    /**************************************/ 
    if(!test::CreateDefaultProject("abc")){
        std::cout << "Error when creating the default project structure.\n";
        return EXIT_FAILURE;
    }
    /**************************************/


    
    return 0;
}