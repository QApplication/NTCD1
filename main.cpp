#include "FabricType.h"

int main(int argc, char *argv[])
{
    if (argc < 1)       // no args
        return EXIT_FAILURE;


    // ini file
    std::string ini_path;

    if (argc == 1) {
        std::filesystem::path p(std::filesystem::current_path().generic_string()
                                + std::filesystem::path::preferred_separator
                                + "param.ini")
            ;       // def path and filename
        if (!std::filesystem::exists(p)) {
            std::cout << "No setting File at path: " << p << std::endl
                      << "run: " << argv[0] << "  /path-to-setting/file-name.ini" << std::endl;
            return EXIT_FAILURE;
        }

        ini_path = p.generic_string();
    }

    if (argc == 2) {
        ini_path = argv[1];
        std::filesystem::path p(ini_path);       // def path and filename
        if (!std::filesystem::exists(p)) {
            std::cout << "File settings not finding." << std::endl;
            return EXIT_FAILURE;
        }
    }


    // set param from

    FabricType ft(ini_path);
    ft.run();

    return EXIT_SUCCESS;
}
