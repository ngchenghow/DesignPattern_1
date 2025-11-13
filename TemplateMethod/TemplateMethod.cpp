#include <iostream>
using namespace std;

// -----------------------------------
// Abstract class: defines the template
// -----------------------------------
class Game
{
public:
    // Template Method (final algorithm)
    void play()
    {
        initialize();
        startPlay();
        endPlay();
    }

    virtual ~Game() {}

protected:
    virtual void initialize() = 0;
    virtual void startPlay() = 0;
    virtual void endPlay() = 0;
};

// -----------------------------------
// Concrete Class 1
// -----------------------------------
class Football : public Game
{
protected:
    void initialize() override
    {
        cout << "Football Game Initialized." << endl;
    }

    void startPlay() override
    {
        cout << "Football Game Started. Enjoy!" << endl;
    }

    void endPlay() override
    {
        cout << "Football Game Finished!" << endl;
    }
};

// -----------------------------------
// Concrete Class 2
// -----------------------------------
class Basketball : public Game
{
protected:
    void initialize() override
    {
        cout << "Basketball Game Initialized." << endl;
    }

    void startPlay() override
    {
        cout << "Basketball Game Started. Enjoy!" << endl;
    }

    void endPlay() override
    {
        cout << "Basketball Game Finished!" << endl;
    }
};

// -----------------------------------
// Main
// -----------------------------------
int main()
{
    Game* game;

    cout << "=== Play Football ===" << endl;
    game = new Football();
    game->play();
    delete game;

    cout << "\n=== Play Basketball ===" << endl;
    game = new Basketball();
    game->play();
    delete game;

    return 0;
}
