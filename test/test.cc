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
        std::cout << red << "Error when creating the default project structure.\n";
        return EXIT_FAILURE;
    }

    /***********************************************/
    /* Test to build & run the default application */
    /***********************************************/
    if(!test::BuildAndRunDefaultProject("abc", &test_passed)){
        std::cout << red << "Error when building and running the default project.\n";
        return EXIT_FAILURE;
    }

    std::cout << green << "\n-- " << tests_passed << " tests passed. --" << white << "\n";

    return 0;
}