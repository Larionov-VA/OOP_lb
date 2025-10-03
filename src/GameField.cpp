#include "GameField.hpp"


GameField::GameField(unsigned width = 10, unsigned height = 10) {
    if (width > MAX_FIELD_SIZE || height > MAX_FIELD_SIZE) {
        throw std::range_error("Max size of field is 25");
    }
    if (width < MIN_FIELD_SIZE || height < MIN_FIELD_SIZE) {
        throw std::range_error("Min size of field is 10");
    }
    widthField = width;
    heightField = height;
    generateFieldCells();
}

void GameField::generateFieldCells() {
    for (unsigned i = 0; i < widthField * heightField; ++i) {
        FieldCell new_cell{i, i % widthField, i / widthField};
        cells.push_back(new_cell);
    }
    int randomPoint1 = 0;
    int randomPoint2 = 0;
    int counter = 0;
    do{
        std::srand(std::time(0));
        randomPoint1 = (std::rand() + counter++) % (widthField * heightField);
        randomPoint2 = (std::rand() + counter++) % (widthField * heightField);
    } while (cells[randomPoint1].getDistance(cells[randomPoint2]) < (int)(widthField + heightField)/3);
    // cells[randomPoint1].setAvaible(true);
    // cells[randomPoint2].setAvaible(true);

    FieldCell cell1 = cells[randomPoint1];
    FieldCell cell2 = cells[randomPoint2];
    FieldCell cell3 = cells[randomPoint2];
    int cell1_X = cell1.getCoord().first;
    int cell1_Y = cell1.getCoord().second;
    int cell2_X = cell2.getCoord().first;
    int cell2_Y = cell2.getCoord().second;

    for (unsigned i = 0; i < widthField * heightField; ++i) {
        FieldCell currentCell = cells[i];
        int dist1 = cell1.getDistance(currentCell);
        int dist2 = cell2.getDistance(currentCell);
        if (dist1 == dist2 && dist1 > (int)(widthField + heightField)/4) {
            cells[i].setAvaible(true);
            cell3 = cells[i];
            break;
        }
    }
    int cell3_X = cell3.getCoord().first;
    int cell3_Y = cell3.getCoord().second;    
    for (unsigned i = 0; i < widthField * heightField; ++i) {
        FieldCell currentCell = cells[i];
        if (currentCell.getDistance(cell1) < (int)(widthField + heightField)/6 || currentCell.getDistance(cell2) < (int)(widthField + heightField)/6 || currentCell.getDistance(cell3) < (int)(widthField + heightField)/4) {
            cells[i].setAvaible(true);
        }
    }
    for (unsigned i = 0; i < widthField * heightField; ++i) {
        FieldCell currentCell = cells[i];
        int currentCell_X = currentCell.getCoord().first;
        int currentCell_Y = currentCell.getCoord().second;
        int dx = cell2_X - cell1_X;
        int dy = cell2_Y - cell1_Y;
        int left = (currentCell_X - cell1_X) * dy;
        int right = (currentCell_Y - cell1_Y) * dx;
        if (abs(left - right)  <= abs(dx) + abs(dy) + widthField + heightField) {
            cells[i].setAvaible(true);
        }
    }
    for (unsigned i = 0; i < widthField * heightField; ++i) {
        FieldCell currentCell = cells[i];
        int currentCell_X = currentCell.getCoord().first;
        int currentCell_Y = currentCell.getCoord().second;
        int dx = cell3_X - cell1_X;
        int dy = cell3_Y - cell1_Y;
        int left = (currentCell_X - cell1_X) * dy;
        int right = (currentCell_Y - cell1_Y) * dx;
        if (abs(left - right)  <= abs(dx) + abs(dy) + widthField + heightField) {
            cells[i].setAvaible(true);
        }
    }
    std::unique_ptr<Entity> p = std::make_unique<Player>();
    infoMap[randomPoint1] = std::move(p);
    // std::cout << "here";Entity::Type::PLAYER
    // std::cout << (int)infoMap[randomPoint1]->getType();
    // std::cout << (int)Entity::Type::PLAYER;
    // cells[randomPoint1].setID(0);
}


void GameField::show() {
    for (unsigned i = 0; i < widthField * heightField; ++i) {
        if (cells[i].isCellAvaible()) {
            // std::cout << (int)infoMap[i]->getType();
            if (infoMap[i]) {
                if (infoMap[i]->getType() == Entity::Type::PLAYER) {
                    std::cout << "P";
                }
            }
            else {
                std::cout << "-";
            }
            // std::cout << "-";
        }
        else {
            std::cout << "X";
            
        }
        // std::cout << std::setw(3) << cells[i].getID();
        if ((i + 1) % widthField == 0) {
            std::cout << '\n';
        }
        else {
            std::cout << "  ";
        }
    }
}