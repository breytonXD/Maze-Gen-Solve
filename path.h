#ifndef PATH_H
#define PATH_H
#include <ostream>

struct PathNode {
    int x;
    int y;
    PathNode* next;
    PathNode(int x, int y) : x(x), y(y), next(nullptr) {}
};

class Path {
    public:
        Path();
        ~Path();

        void append(int x, int y);
        int size() const;
        bool empty() const;

        const PathNode* head() const;
        void printTrace(std::ostream& os) const;

    private:
        PathNode* head_;
        PathNode* tail_;
        int count_;

};

#endif
