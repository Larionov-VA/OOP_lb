#pragma once

class Item {
private:
    int countOfItem;
public:
    virtual int getCountOfItem() = 0;
    virtual bool useItem() = 0;
};