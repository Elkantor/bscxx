#include <core/common.h>
#include <core/generator.h>
#include <create_project.h>

int main(int argc, char* argv[]){ 

    std::cout << "Performing tests...\n";
    int tests_passed = 0;

    /**************************************/
    /* Test the creation of a new project */
    /**************************************/ 
    if(!test::CreateDefaultProject("abc", &tests_passed)){
        std::cout << "\n-- " << tests_passed << " tests passed. --\n";
        std::cout << "Error when creating the default project structure.\n";
        return EXIT_FAILURE;
    }else{
        std::cout << "\n-- " << tests_passed << " tests passed. --\n";
    }
    /**************************************/


    return 0;
}