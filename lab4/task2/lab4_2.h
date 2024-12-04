#ifndef FINALLY_H
#define FINALLY_H

#include <utility>

template <typename F>
class Finally {
public:
    explicit Finally(F&& f) 
        : func_(std::forward<F>(f)), active_(true) {}

    Finally(Finally&& other) noexcept 
        : func_(std::move(other.func_)), active_(other.active_) {
        other.active_ = false;
    }

    ~Finally() {
        if (active_) {
            func_();
        }
    }

    Finally(const Finally&) = delete;
    Finally& operator=(const Finally&) = delete;

private:
    F func_;
    bool active_;
};

template <typename F>
Finally<F> make_finally(F&& f) {
    return Finally<F>(std::forward<F>(f));
}

#endif // FINALLY_H
