#include <iostream>
#include <algorithm>

class SimpleText {
public:
    virtual void render(const std::string& text) {
        std::cout << text;
    }
};

class DecoratedText : public SimpleText {
public:    
    SimpleText* __simple_text;
    DecoratedText(SimpleText* simple_text) : __simple_text(simple_text) {}
};

class ItalicText : public DecoratedText {
public:
    ItalicText(SimpleText* simple_text) : DecoratedText(simple_text) {}
    void render(const std::string& text) override {
        std::cout << "<i>";
        __simple_text->render(text);
        std::cout << "</i>";
    }
};

class BoldText : public DecoratedText {
public:
    BoldText(SimpleText* simple_text) : DecoratedText(simple_text) {}
    void render(const std::string& text) override {
        std::cout << "<b>";
        __simple_text->render(text);
        std::cout << "</b>";
    }
};

class Paragraph : public DecoratedText {
public:
    Paragraph(SimpleText* simple_text) : DecoratedText(simple_text) {}
    void render(const std::string& text) override {
        std::cout << "<p>";
        __simple_text->render(text);
        std::cout << "</p>";
    }
};

class Reversed : public DecoratedText {
public:
    Reversed(SimpleText* simple_text) : DecoratedText(simple_text) {}
    void render(const std::string& text) override {
        std::string reversed = text;
        std::reverse(reversed.begin(), reversed.end());
        __simple_text->render(reversed);
    }
};

class Link : public DecoratedText {
public:
    Link(SimpleText* simple_text) : DecoratedText(simple_text) {}
    void render(const std::string& url, const std::string& text) {
        std::cout << "<a href=\"" << url << "\">";
        __simple_text->render(text);
        std::cout << "</a>";
    }
};

int main() {

    SimpleText text1;
    text1.render("Hello HTML");
    std::cout << std::endl;

    ItalicText i_text(&text1);
    i_text.render("Italic text");
    std::cout << std::endl;

    BoldText b_text(&text1);
    b_text.render("Bold text");
    std::cout << std::endl;

    auto text_block = new Reversed(new SimpleText());
    text_block->render("Hello world");
    std::cout << std::endl;

    auto text = new ItalicText(new BoldText(new SimpleText()));
    text->render("Decorated text");
    std::cout << std::endl;

    auto text_block1 = new Link(new SimpleText());
    text_block1->render("netology.ru", "Hello world");
    std::cout << std::endl;

    return 0;
}