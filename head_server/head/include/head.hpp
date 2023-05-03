#pragma once

#include <shared.hpp>

class Head {
private:
    int WriteInDB();

    int FindInDB();
public:
    Head();

    Head(const std::string& server, const std::string& port);

    Head(const Head &);

    Head& operator=(const Head &); 

    Head (Head&& other);

    Head& operator=(Head&& other);

    ~Head();

    int Run();
};
