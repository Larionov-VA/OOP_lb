#include <FileHandler.hpp>


explicit FileHandler::FileHandler(const std::string& filename, std::ios::openmode mode) : file_(filename, mode) {
    if (!file_.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }
}


FileHandler::~FileHandler() {
    if (file_.is_open()) file_.close();
}

std::fstream& FileHandler::stream() {
    return file_;
}