#include <header.hpp>

int main (const int argc, const char **argv) {
    ScopApplication app(argc, argv);

    try {
        app.run();
    } catch (const exception &e) {
        cerr << e.what() << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
