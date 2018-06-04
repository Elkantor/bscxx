#include <core/common.h>
#include <core/generator.h>
#include <create_project.h>
#include <addons/console_color.h>

int main(int argc, char* argv[]){ 

    std::cout << "Performing tests...\n";
    int tests_passed = 0;

    /**************************************/
    /* Test the creation of a new project */
    /**************************************/ 
    if(!test::CreateDefaultProject("abc", &tests_passed)){
        std::cout << green << "\n-- " << tests_passed << " tests passed. --" << white << "\n";
        std::cout << red << "Error when creating the default project structure.\n";
        return EXIT_FAILURE;
    }else{
        std::cout << green << "\n-- " << tests_passed << " tests passed. --" << white << "\n";
    }
    /**************************************/


    return 0;
}