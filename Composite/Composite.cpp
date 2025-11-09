// Composite_FileSystem_Remove.cpp  (C++17)
// Composite pattern with working recursive remove.

#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <stdexcept>
#include <algorithm>

class Node {
public:
    explicit Node(std::string name) : name_(std::move(name)) {}
    virtual ~Node() = default;

    const std::string& name() const { return name_; }

    virtual std::size_t size_bytes() const = 0;
    virtual void print(int indent = 0) const = 0;

    virtual void add(std::unique_ptr<Node> /*child*/) {
        throw std::logic_error("Cannot add child to a leaf: " + name_);
    }
    virtual bool remove(const std::string& /*child_name*/) { return false; }

private:
    std::string name_;
};

class File : public Node {
public:
    File(std::string name, std::size_t bytes)
        : Node(std::move(name)), bytes_(bytes) {
    }

    std::size_t size_bytes() const override { return bytes_; }

    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ')
            << "- " << name() << " (" << bytes_ << " B)\n";
    }

private:
    std::size_t bytes_;
};

class Folder : public Node {
public:
    explicit Folder(std::string name) : Node(std::move(name)) {}

    std::size_t size_bytes() const override {
        std::size_t total = 0;
        for (const auto& c : children_) total += c->size_bytes();
        return total;
    }

    void print(int indent = 0) const override {
        std::cout << std::string(indent, ' ')
            << "+ " << name() << " [" << size_bytes() << " B]\n";
        for (const auto& c : children_) c->print(indent + 2);
    }

    void add(std::unique_ptr<Node> child) override {
        children_.push_back(std::move(child));
    }

    // Recursive removal: try direct children; if not found, ask subfolders.
    bool remove(const std::string& child_name) override {
        auto it = std::remove_if(children_.begin(), children_.end(),
            [&](const std::unique_ptr<Node>& p) { return p->name() == child_name; });
        if (it != children_.end()) {
            children_.erase(it, children_.end());
            return true;
        }
        // Recurse into subfolders
        for (auto& c : children_) {
            if (auto* f = dynamic_cast<Folder*>(c.get())) {
                if (f->remove(child_name)) return true;
            }
        }
        return false;
    }

private:
    std::vector<std::unique_ptr<Node>> children_;
};

std::unique_ptr<Node> make_file(const std::string& name, std::size_t bytes) {
    return std::make_unique<File>(name, bytes);
}
std::unique_ptr<Folder> make_folder(const std::string& name) {
    return std::make_unique<Folder>(name);
}

int main() {
    auto root = make_folder("root");
    auto docs = make_folder("docs");
    docs->add(make_file("report.pdf", 1200));
    docs->add(make_file("notes.txt", 300));

    auto images = make_folder("images");
    images->add(make_file("logo.png", 500));
    images->add(make_file("banner.jpg", 2048));

    root->add(std::move(docs));
    root->add(std::move(images));
    root->add(make_file("readme.md", 100));

    std::cout << "== Before ==\n";
    root->print();
    std::cout << "Total: " << root->size_bytes() << " bytes\n\n";

    std::cout << "Removing banner.jpg...\n";
    bool ok = root->remove("banner.jpg");
    std::cout << (ok ? "Removed.\n\n" : "Not found.\n\n");

    std::cout << "== After ==\n";
    root->print();
    std::cout << "Total: " << root->size_bytes() << " bytes\n";
    return 0;
}
