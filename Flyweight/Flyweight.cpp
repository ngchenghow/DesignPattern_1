// Flyweight_CharacterGlyph.cpp  (C++17)
// Build: g++ -std=c++17 Flyweight_CharacterGlyph.cpp -o flyweight && ./flyweight
// VS: cl /std:c++17 Flyweight_CharacterGlyph.cpp

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <tuple>

// ===== Flyweight (intrinsic state) =====
class Glyph {
public:
    Glyph(char ch, std::string font) : ch_(ch), font_(std::move(font)) {}
    // Render uses extrinsic state (x, y, color) provided by the caller.
    void Render(int x, int y, const std::string& color) const {
        std::cout << "Draw '" << ch_ << "'"
            << " font=" << font_
            << " at(" << x << "," << y << ")"
            << " color=" << color
            << "  [flyweight@" << this << "]\n";
    }
    char ch()   const { return ch_; }
    const std::string& font() const { return font_; }
private:
    char ch_;
    std::string font_;
};

// ===== Flyweight Factory =====
class GlyphFactory {
public:
    // Get or create a shared flyweight by (char,font)
    std::shared_ptr<const Glyph> Get(char ch, const std::string& font) {
        Key key{ ch, font };
        if (auto it = pool_.find(key); it != pool_.end()) {
            return it->second;
        }
        auto g = std::make_shared<Glyph>(ch, font);
        pool_.emplace(std::move(key), g);
        return g;
    }

    size_t Count() const { return pool_.size(); }

private:
    struct Key {
        char ch;
        std::string font;
        bool operator==(const Key& other) const {
            return ch == other.ch && font == other.font;
        }
    };
    struct KeyHash {
        size_t operator()(const Key& k) const {
            // Combine hashes of char and string
            size_t h1 = std::hash<char>{}(k.ch);
            size_t h2 = std::hash<std::string>{}(k.font);
            return h1 ^ (h2 + 0x9e3779b97f4a7c15ULL + (h1 << 6) + (h1 >> 2));
        }
    };

    std::unordered_map<Key, std::shared_ptr<const Glyph>, KeyHash> pool_;
};

// ===== Client object holding extrinsic state =====
struct DrawOp {
    std::shared_ptr<const Glyph> glyph; // shared flyweight
    int x, y;                            // extrinsic
    std::string color;                   // extrinsic
    void Run() const { glyph->Render(x, y, color); }
};

int main() {
    GlyphFactory factory;

    // Simulate a small document: "ABBA" in two fonts/colors at many positions.
    std::string text = "ABBA";
    std::vector<DrawOp> ops;

    // Create many draw ops; factory should reuse only needed glyphs.
    for (int row = 0; row < 3; ++row) {
        for (int i = 0; i < (int)text.size(); ++i) {
            char ch = text[i];
            // Alternate fonts/colors to show distinct flyweights per (char,font)
            std::string font = (i % 2 == 0 ? "Consolas-14" : "Consolas-Bold-14");
            std::string color = (row % 2 == 0 ? "black" : "blue");

            auto glyph = factory.Get(ch, font); // shared instance
            ops.push_back({ glyph, 10 + i * 12, 20 + row * 16, color });
        }
    }

    // Render
    for (const auto& op : ops) op.Run();

    // Show how many unique flyweights were actually created
    std::cout << "\nUnique flyweights created: " << factory.Count() << "\n";

    // Optional: demonstrate same pointer reused for same (char,font)
    auto g1 = factory.Get('A', "Consolas-14");
    auto g2 = factory.Get('A', "Consolas-14");
    auto g3 = factory.Get('A', "Consolas-Bold-14");
    std::cout << "g1 @" << g1.get() << "  g2 @" << g2.get()
        << "  (same)\n";
    std::cout << "g3 @" << g3.get() << "  (different font => different flyweight)\n";

    return 0;
}
