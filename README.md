# Bindline

WORK IN PROGRESS, along with these docs. Do not use or ask for help.

Bindline is a suite of several *header-only libraries* which are designed to avoid common mistakes with asynchronous code, e.g. event handlers and coroutines.

- `FredEmmott::bindline`: a function composition library
- `FredEmmott::weak_refs`: concepts and functional helpers for working with strong and weak references, e.g. `std::shared_ptr()` and `std::weak_ptr()`
- `FredEmmott::cppwinrt`: concepts and functional helpers for working with [C++/WinRT](https://aka.ms/cppwinrt)

These libraries can be used separately - in particular, Windows and C++/WinRT are not required - but are designed to work together:

C++20 is required, but more features are available with C++23.

```C++
auto f = &MyClass::some_func
  // Store an `std::shared_ptr` or `winrt::weak_ref`, but when invoking:
  // 1. convert back to a strong ref
  // 2. return without invoking if the ref is stale (i.e. `nullptr`)
  | bind_refs_front(this)
  // Automatically switch thread when invoked
  | bind_winrt_context(winrt_apartment_context_or_dispatcher_queue())
  // if some_func() takes N parameters, require invocing with N + 2
  // parameters; this can be useful for event handlers
  | drop_n_back<2>();

f(args...);
```

Compile-and-runtime validation is also included for common errors:

```c++
// This is fine:
f | bind_refs_front(this) | bind_winrt_context(ctx);

/* This is an error:
 *
 * functions on the right wrap functions on the left, and you probably want to
 * promote the weak ref to a strong ref _after_ switching threads:
 */
f | bind_winrt_context(ctx) | bind_refs_front(this);

// Nope, I know what I'm doing:
f | bind_winrt_context(ctx) | bind_suppress_validation(bind_refs_front(this));
```

These libraries are designed to be cleanly usable via `use namespace`, but this is not required.

## Why

- `FredEmmott::weak_refs` makes asynchronous code safer and easier to write, especially when combining multiple kinds of strong vs weak references (e.g. if your project uses `std::shared_ptr` and `std::weak_ptr`, but also uses C++/WinRT strong vs weak refs).
- `FredEmmott::cppwinrt` solves common needs with C++/WinRT, especially event handlers
- `FredEmmott::bindline` makes combining the two - along with other functional needs - concise and convenient

This is unsafe if `this` can be destroyed between enqueing the event and it executing:

```c++
// UNSAFE:
foo.enqueue([this]() {
    // ... do stuff ..
});
```

For types using `std::enable_shared_from_this`, this can be made safe with `std::bind_front()`:

```c++
foo.enqueue(
    std::bind_front(
        [](auto weak) {
            auto self = weak.lock();
            if (!self) {
                return;
            }
            // ... do stuff ...
        },
        this->weak_from_this(),
    )
);
```

... or with *bindline* combined with *weak_refs*:

```c++
// Just with weak_refs:

foo.enqueue(
    bind_refs_front(
        [](auto self) { /* ... */ },
        this
    )
);

// bindline + weak_refs:
foo.enqueue([](auto self) { /* ... */ } | bind_refs_front(this));
```

These examples are unsafe:

```c++
MyCoro ex1(auto otherThing) {
  // UNSAFE. Safe-ish if MyCoro::initial_suspend() returns an std::suspend_never();
  this->foo();
  // safe:
  otherThing->bar();
  co_await do_other_stuff();
  // UNSAFE: may have been destructed while doing other stuff
  this->foo(); 
  co_return;
};

void ex2() {
  auto f = [this, otherThing]() -> MyCoro {
    // Safe-ish if MyCoro::initial_suspend() returns an std::suspend_never();
    this->foo(); // UNSAFE
    otherThing->bar(); // UNSAFE
    co_await do_other_stuff();
    // UNSAFE:
    // - the object may no longer exist
    // - lambda captures are only part of the lambda object, not part of the coroutine state;
    //   see https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rcoro-capture
    this->foo(); // UNSAFE
    otherThing->bar(); // UNSAFE
    co_return;
  };
}
```

To make `ex2()` safe with `std::bind_front()`:

```c++
void ex3() {
  auto f = [](auto weakSelf, auto weakOtherThing]) -> MyCoro {
    auto self = weakSelf.lock();
    auto otherThing = weakOtherThing.lock();
    if (!(self && otherThing)) {
        co_return;
    }
    
    self->foo();
    otherThing->bar();

    self.reset();
    otherThing.reset();
    co_await do_other_stuff();
    auto self = weakSelf.lock();
    auto otherThing = weakOtherThing.lock();
    if (!(self && otherThing)) {
        co_return;
    }

    self->foo();
    otherThing->bar();
  };
}
```

... or with bindline and weak_refs:

```c++
void ex4() {
    auto f = [](auto self, auto otherThing) -> MyCoro {
        self->foo();
        otherThing->bar();

        {
            strong_ref_reseater reseater { &self, &otherThing };
            co_await do_other_thing();
            if (!reseater.reseat()) {
                co_return;
            }
        }

        self->foo();
        otherThing->bar();
    };
}
``