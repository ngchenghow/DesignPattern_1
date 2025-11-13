#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

class Mediator;

// ---------------- Base Component ----------------
class Component {
protected:
    Mediator* mediator;
public:
    Component(Mediator* m) : mediator(m) {}
    virtual ~Component() = default;
};

// ---------------- Components ----------------

// TextBox
class TextBox : public Component {
public:
    string text;

    TextBox(Mediator* m) : Component(m) {}

    void setText(const string& t) {
        text = t;
        cout << "TextBox set to: " << text << endl;
    }
};

// Base Button
class Button : public Component {
public:
    Button(Mediator* m) : Component(m) {}
    virtual void click() = 0;
};

// Different button types
class ButtonClear : public Button {
public:
    ButtonClear(Mediator* m) : Button(m) {}
    void click() override;
};

class ButtonUpper : public Button {
public:
    ButtonUpper(Mediator* m) : Button(m) {}
    void click() override;
};

class ButtonPrint : public Button {
public:
    ButtonPrint(Mediator* m) : Button(m) {}
    void click() override;
};

// ---------------- Mediator ----------------
class Mediator {
public:
    TextBox* textBox;
    ButtonClear* btnClear;
    ButtonUpper* btnUpper;
    ButtonPrint* btnPrint;

    void notify(Component* sender, const string& event) {
        if (sender == btnClear && event == "click") {
            cout << "Mediator: clear text\n";
            textBox->text = "";
        }
        else if (sender == btnUpper && event == "click") {
            cout << "Mediator: uppercase text\n";
            transform(textBox->text.begin(), textBox->text.end(),
                textBox->text.begin(), ::toupper);
        }
        else if (sender == btnPrint && event == "click") {
            cout << "Mediator: print text => " << textBox->text << endl;
        }
    }
};

// Button implementations
void ButtonClear::click() {
    cout << "ButtonClear clicked.\n";
    mediator->notify(this, "click");
}

void ButtonUpper::click() {
    cout << "ButtonUpper clicked.\n";
    mediator->notify(this, "click");
}

void ButtonPrint::click() {
    cout << "ButtonPrint clicked.\n";
    mediator->notify(this, "click");
}

// ---------------- Main Demo ----------------
int main() {
    Mediator mediator;

    TextBox text(&mediator);
    ButtonClear clearBtn(&mediator);
    ButtonUpper upperBtn(&mediator);
    ButtonPrint printBtn(&mediator);

    mediator.textBox = &text;
    mediator.btnClear = &clearBtn;
    mediator.btnUpper = &upperBtn;
    mediator.btnPrint = &printBtn;

    text.setText("Hello World");

    upperBtn.click();   // convert to uppercase
    printBtn.click();   // print
    clearBtn.click();   // clear text
    printBtn.click();   // print empty

    return 0;
}
