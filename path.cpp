#include "path.h"

Path::Path() : head_(nullptr), tail_(nullptr), count_(0) {}

Path::~Path() {
    PathNode* cur = head_;
    while (cur != nullptr) {
        PathNode* next = cur->next;
        delete cur;
        cur = next;
    }
}

void Path::append(int x, int y) {
    PathNode* node = new PathNode(x, y);
    if (head_ == nullptr) {
        head_ = node;
        tail_ = node;
    } else {
        tail_->next = node;
        tail_ = node;
    }
    count_++;
}

int Path::size() const {
    return count_;
}

bool Path::empty() const {
    return head_ == nullptr;
}

const PathNode* Path::head() const {
    return head_;
}

void Path::printTrace(std::ostream& os) const {
    const PathNode* cur = head_;
    while (cur != nullptr) {
        os << "(" << cur->x << "," << cur->y << ")";
        if (cur->next != nullptr) {
            os << " -> ";
        }
        cur = cur->next;
    }
    os << "\n";
}