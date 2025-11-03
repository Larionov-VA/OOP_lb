#include <fstream>


class FileHandler {
private:
    std::fstream file_;
public:
    explicit FileHandler(const std::string& filename, std::ios::openmode mode);
    std::fstream& stream();
    ~FileHandler();
};
