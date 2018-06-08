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

    /***************************************/
    /* Test to build & run the default application */
    /***************************************/
    try{
        std::string command;
        command = "mkdir build && cd build ";
        command += " && cmake ..";
        command += " && cmake --build . --config Release";
        command += " && \"src/bin/release/abc.exe\" > log_project.txt";
        command += " && cd ..";
        system(command.c_str());

        std::string line;
        std::string body;
        std::ifstream infile("log_project.txt", std::ios::in);
        if (!infile) {
            std::cerr << red << "Error when trying to open the log_project.txt file.\n";
            return EXIT_FAILURE;
        }
        while(!infile.eof()){
            std::getline(infile, line);
            body += line;
        }
        infile.close();
        std::cout << "\n" << body << "\n";
    }catch(int e){
        return EXIT_FAILURE;
    }

    return 0;
}