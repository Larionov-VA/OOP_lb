#pragma once

class FieldCell {
private:
    char c;
public:
    FieldCell();
    ~FieldCell();
    char getData() const{
        return c;
    }
};
