#include "Tetrominos.h"
#include "TaskUtils.h"

class I : public Block
{
public:
    int xBlockOffset = Block::xBlockOffset;
    I() : Block(4)
    {
        matrix[1][0] = 'I';
        matrix[1][1] = 'I';
        matrix[1][2] = 'I';
        matrix[1][3] = 'I';
    }

    void RotateRight() override
    {
        RotateLeft();
        RotateLeft();
        RotateLeft();
    }
};

class J : public Block
{
public:
    int xBlockOffset = Block::xBlockOffset;
    J() : Block(3)
    {
        matrix[1][0] = 'J';
        matrix[1][1] = 'J';
        matrix[1][2] = 'J';
        matrix[0][0] = 'J';
    }
};

class L : public Block
{
public:
    int xBlockOffset = Block::xBlockOffset;
    L() : Block(3)
    {
        matrix[1][0] = 'L';
        matrix[1][1] = 'L';
        matrix[1][2] = 'L';
        matrix[0][2] = 'L';
    }
};

class O : public Block
{
public:
    int xBlockOffset = Block::xBlockOffset;
    O() : Block(2)
    {
        matrix[0][0] = 'O';
        matrix[0][1] = 'O';
        matrix[1][0] = 'O';
        matrix[1][1] = 'O';
    }

    void RotateRight() override {}
    void RotateLeft() override {}
};

class S : public Block
{
public:
    int xBlockOffset = Block::xBlockOffset;
    S() : Block(3)
    {
        matrix[0][1] = 'S';
        matrix[0][2] = 'S';
        matrix[1][0] = 'S';
        matrix[1][1] = 'S';
    }
};

class T : public Block
{
public:
    int xBlockOffset = Block::xBlockOffset;
    T() : Block(3)
    {
        matrix[0][1] = 'T';
        matrix[1][0] = 'T';
        matrix[1][1] = 'T';
        matrix[1][2] = 'T';
    }
};

class Z : public Block
{
public:
    int xBlockOffset = Block::xBlockOffset;
    Z() : Block(3)
    {
        matrix[0][0] = 'Z';
        matrix[0][1] = 'Z';
        matrix[1][1] = 'Z';
        matrix[1][2] = 'Z';
    }
};

std::unique_ptr<Block> ReturnObj()
{
    const char letters[] = { 'I', 'O', 'L', 'J', 'S', 'Z', 'T' };
    switch (letters[TaskUtils::RandomInt(7)])
    {
    case 'I': return std::make_unique<I>();
    case 'J': return std::make_unique<J>();
    case 'L': return std::make_unique<L>();
    case 'O': return std::make_unique<O>();
    case 'S': return std::make_unique<S>();
    case 'Z': return std::make_unique<Z>();
    case 'T': return std::make_unique<T>();
    default: return nullptr;
    }
}