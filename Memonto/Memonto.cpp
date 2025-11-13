#include <iostream>
#include <string>
#include <vector>

// ===================== Memento =====================
// Stores the internal state of the Originator.
// It is usually immutable from the outside.
class Memento
{
public:
    explicit Memento(std::string state)
        : state_(std::move(state)) {
    }

    const std::string& getState() const
    {
        return state_;
    }

private:
    std::string state_;
};

// ===================== Originator =====================
// The object whose state we want to save/restore.
class Editor
{
public:
    void type(const std::string& words)
    {
        text_ += words;
    }

    void show() const
    {
        std::cout << "Editor text: \"" << text_ << "\"\n";
    }

    // Create a Memento that holds current state.
    Memento save() const
    {
        return Memento(text_);
    }

    // Restore state from a Memento.
    void restore(const Memento& m)
    {
        text_ = m.getState();
    }

private:
    std::string text_;
};

// ===================== Caretaker =====================
// Manages a list/stack of Mementos (history).
class History
{
public:
    void push(const Memento& m)
    {
        history_.push_back(m);
    }

    bool canUndo() const
    {
        return !history_.empty();
    }

    Memento pop()
    {
        if (history_.empty())
        {
            // In real code, you should throw or handle this properly.
            return Memento("");
        }

        Memento m = history_.back();
        history_.pop_back();
        return m;
    }

private:
    std::vector<Memento> history_;
};

// ===================== Demo =====================
int main()
{
    Editor editor;
    History history;

    editor.type("Hello");
    editor.show();
    history.push(editor.save()); // save #1

    editor.type(", world");
    editor.show();
    history.push(editor.save()); // save #2

    editor.type("!!!");
    editor.show(); // current

    std::cout << "\n--- Undo 1 ---\n";
    if (history.canUndo())
    {
        editor.restore(history.pop());
        editor.show();
    }

    std::cout << "\n--- Undo 2 ---\n";
    if (history.canUndo())
    {
        editor.restore(history.pop());
        editor.show();
    }

    std::cout << "\n--- Undo 3 (no more history) ---\n";
    if (history.canUndo())
    {
        editor.restore(history.pop());
        editor.show();
    }
    else
    {
        std::cout << "Nothing to undo.\n";
    }

    return 0;
}
