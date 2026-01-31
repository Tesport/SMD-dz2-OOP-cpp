#include <iostream>
#include <vector>
#include <string>


// 1
class MyString {
private:
    char* data;

public:
    MyString() {
        data = new char[1];
        data[0] = '\0';
    }

    MyString(const char* str) {
        if (str) {
            data = new char[strlen(str) + 1];
            strcpy_s(data, strlen(str) + 1, str);
        }
        else {
            data = new char[1];
            data[0] = '\0';
        }
    }

    MyString(const MyString& other) {
        data = new char[strlen(other.data) + 1];
        strcpy_s(data, strlen(other.data) + 1, other.data);
    }

    ~MyString() {
        delete[] data;
    }

    int length() const {
        return strlen(data);
    }

    void concat(const MyString& other) {
        char* temp = new char[length() + other.length() + 1];
        strcpy_s(temp, length() + other.length() + 1, data);
        strcat_s(temp, length() + other.length() + 1, other.data);
        delete[] data;
        data = temp;
    }

    int compare(const MyString& other) const {
        return strcmp(data, other.data);
    }

    void insert(const MyString& other, int pos) {
        if (pos < 0 || pos > length()) return;

        size_t len1 = length();
        size_t len2 = other.length();
        size_t newSize = len1 + len2 + 1;
        char* temp = new char[newSize];
        strncpy_s(temp, newSize, data, pos);
        temp[pos] = '\0';
        strcat_s(temp, newSize, other.data);
        strcat_s(temp, newSize, data + pos);

        delete[] data;
        data = temp;
    }

    void print() const {
        std::cout << data << std::endl;
    }

    MyString operator+(const MyString& other) const {
        MyString result(data);
        result.concat(other);
        return result;
    }

    MyString& operator+=(const MyString& other) {
        concat(other);
        return *this;
    }

    char& operator[](int index) {
        return data[index];
    }

    const char& operator[](int index) const {
        return data[index];
    }
};

// 2
template <typename T>
class List {
private:
    struct Node {
        T data;
        Node* next;
        Node* prev;
        Node(const T& val) : data(val), next(nullptr), prev(nullptr) {}
    };

    Node* head;
    Node* tail;
    size_t sz;

public:
    List() : head(nullptr), tail(nullptr), sz(0) {}

    List(const List& other) : head(nullptr), tail(nullptr), sz(0) {
        for (Node* curr = other.head; curr; curr = curr->next) {
            push_back(curr->data);
        }
    }

    ~List() {
        clear();
    }

    List& operator=(const List& other) {
        if (this != &other) {
            clear();
            for (Node* curr = other.head; curr; curr = curr->next) {
                push_back(curr->data);
            }
        }
        return *this;
    }

    void push_back(const T& value) {
        Node* node = new Node(value);
        if (!head) {
            head = tail = node;
        }
        else {
            tail->next = node;
            node->prev = tail;
            tail = node;
        }
        ++sz;
    }

    void push_front(const T& value) {
        Node* node = new Node(value);
        if (!head) {
            head = tail = node;
        }
        else {
            node->next = head;
            head->prev = node;
            head = node;
        }
        ++sz;
    }

    void pop_back() {
        if (!tail) return;
        Node* temp = tail;
        tail = tail->prev;
        if (tail) tail->next = nullptr;
        else head = nullptr;
        delete temp;
        --sz;
    }

    void pop_front() {
        if (!head) return;
        Node* temp = head;
        head = head->next;
        if (head) head->prev = nullptr;
        else tail = nullptr;
        delete temp;
        --sz;
    }

    size_t size() const {
        return sz;
    }

    bool empty() const {
        return sz == 0;
    }

    void clear() {
        Node* curr = head;
        while (curr) {
            Node* next = curr->next;
            delete curr;
            curr = next;
        }
        head = tail = nullptr;
        sz = 0;
    }

    class Iterator {
    private:
        Node* node;
    public:
        Iterator(Node* n) : node(n) {}
        T& operator*() { return node->data; }
        Iterator& operator++() { node = node->next; return *this; }
        Iterator operator++(int) { Iterator tmp = *this; node = node->next; return tmp; }
        Iterator& operator--() { node = node->prev; return *this; }
        Iterator operator--(int) { Iterator tmp = *this; node = node->prev; return tmp; }
        bool operator==(const Iterator& other) const { return node == other.node; }
        bool operator!=(const Iterator& other) const { return node != other.node; }
    };

    Iterator begin() { return Iterator(head); }
    Iterator end() { return Iterator(nullptr); }
};

// 3
struct Point {
    double x, y;
    Point(double xx = 0, double yy = 0) : x(xx), y(yy) {}
};

class Shape {
public:
    virtual double area() const = 0;
    virtual double perimeter() const = 0;
    virtual void translate(double dx, double dy) = 0;
    virtual void rotate(double angle) = 0;
    virtual void print() const = 0;
    virtual std::string getType() const = 0;
    virtual ~Shape() {}
};

class Circle : public Shape {
private:
    Point center;
    double radius;
public:
    Circle(double x, double y, double r) : center(x, y), radius(r) {}
    double area() const override { return 3.14 * radius * radius; }
    double perimeter() const override { return 2 * 3.14 * radius; }
    void translate(double dx, double dy) override { center.x += dx; center.y += dy; }
    void rotate(double) override {}
    void print() const override {
        std::cout << "Circle: center=(" << center.x << "," << center.y << "), radius=" << radius
            << ", area=" << area() << ", perimeter=" << perimeter() << std::endl;
    }
    std::string getType() const override { return "Circle"; }
};

class Polygon : public Shape {
protected:
    std::vector<Point> vertices;
public:
    Polygon(const std::vector<Point>& v) : vertices(v) {}
    double perimeter() const override {
        double p = 0;
        int n = vertices.size();
        for (int i = 0; i < n; i++) {
            double dx = vertices[i].x - vertices[(i + 1) % n].x;
            double dy = vertices[i].y - vertices[(i + 1) % n].y;
            p += std::sqrt(dx * dx + dy * dy);
        }
        return p;
    }
    void translate(double dx, double dy) override {
        for (auto& p : vertices) { p.x += dx; p.y += dy; }
    }
    void rotate(double angle) override {
        double rad = angle * 3.14 / 180.0;
        for (auto& p : vertices) {
            double x_new = p.x * std::cos(rad) - p.y * std::sin(rad);
            double y_new = p.x * std::sin(rad) + p.y * std::cos(rad);
            p.x = x_new; p.y = y_new;
        }
    }
};

class Triangle : public Polygon {
public:
    Triangle(const Point& a, const Point& b, const Point& c) : Polygon({ a,b,c }) {}
    double area() const override {
        double x0 = vertices[0].x, y0 = vertices[0].y;
        double x1 = vertices[1].x, y1 = vertices[1].y;
        double x2 = vertices[2].x, y2 = vertices[2].y;
        return std::abs((x0 * (y1 - y2) + x1 * (y2 - y0) + x2 * (y0 - y1)) / 2.0);
    }
    void print() const override {
        std::cout << "Triangle: area=" << area() << ", perimeter=" << perimeter() << std::endl;
    }
    std::string getType() const override { return "Triangle"; }
};

class Tetragon : public Polygon {
public:
    Tetragon(const std::vector<Point>& v) : Polygon(v) {}
    double area() const override {
        double a = 0;
        int n = vertices.size();
        for (int i = 0; i < n; i++) {
            a += (vertices[i].x * vertices[(i + 1) % n].y - vertices[(i + 1) % n].x * vertices[i].y);
        }
        return std::abs(a) / 2.0;
    }
    void print() const override {
        std::cout << "Tetragon: area=" << area() << ", perimeter=" << perimeter() << std::endl;
    }
    std::string getType() const override { return "Tetragon"; }
};

class Parallelogram : public Tetragon {
protected:
    double base, height, side;
public:
    Parallelogram(double b, double h, double s)
        : Tetragon({ {0,0},{b,0},{b,s},{0,s} }), base(b), height(h), side(s) {
    }
    double area() const override { return base * height; }
    double perimeter() const override { return 2 * (base + side); }
    void print() const override {
        std::cout << "Parallelogram: area=" << area() << ", perimeter=" << perimeter() << std::endl;
    }
    std::string getType() const override { return "Parallelogram"; }
};

class Rectangle : virtual public Parallelogram {
public:
    Rectangle(double w, double h) : Parallelogram(w, h, h) {}
    void print() const override {
        std::cout << "Rectangle: area=" << area() << ", perimeter=" << perimeter() << std::endl;
    }
    std::string getType() const override { return "Rectangle"; }
};

class Rhomb : virtual public Parallelogram {
public:
    Rhomb(double side, double height) : Parallelogram(side, height, side) {}
    void print() const override {
        std::cout << "Rhomb: area=" << area() << ", perimeter=" << perimeter() << std::endl;
    }
    std::string getType() const override { return "Rhomb"; }
};

class Square : public Rectangle, public Rhomb {
private:
    double side;
public:
    Square(double s) : Parallelogram(s, s, s), Rectangle(s, s), Rhomb(s, s), side(s) {}
    double area() const override { return side * side; }
    double perimeter() const override { return 4 * side; }
    void print() const override {
        std::cout << "Square: area=" << area() << ", perimeter=" << perimeter() << std::endl;
    }
    std::string getType() const override { return "Square"; }
};
int main() {
    // 1
    MyString s1("Heheheha ");
    MyString s2("Aboba");
    MyString s3 = s1 + s2;
    s3.print();
    s1 += s2;
    s1.print();
    std::cout << s1.length() << std::endl;
    s1.insert(MyString("n't"), 8);
    s1.print();
    std::cout << s1[1] << std::endl;
    
    // 2
    List<int> lst;
    lst.push_back(1);
    lst.push_back(2);
    lst.push_back(3);

    for (auto it = lst.begin(); it != lst.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    List<int> lst2 = lst;
    lst2.push_front(0);

    for (auto it = lst2.begin(); it != lst2.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // 3
    Circle c(0, 0, 5);
    c.print();

    Triangle t({ 0,0 }, { 3,0 }, { 0,4 });
    t.print();
    t.translate(1, 2);
    t.rotate(45);
    t.print();

    Rectangle r(4, 6);
    r.print();

    Rhomb rh(5, 4);
    rh.print();

    Square sq(5);
    sq.print();
    return 0;
}
