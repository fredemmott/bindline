---
title: Rationale
---

# The Problems Being Solved

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
  auto f = std::bind_front(
    [](auto weakSelf, auto weakOtherThing]) -> MyCoro {
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
    },
    weak_from_this(),
    otherThing->weak_from_this());
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
    } | bind_refs_front(this, otherThing);
}
```
