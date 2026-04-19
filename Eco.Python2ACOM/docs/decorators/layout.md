# Layout Decorators

Use these decorators to define C-compatible data structures for EcoOS interop.

::: eco_python2acom.decorators.layout.model

::: eco_python2acom.decorators.layout.union

Use `stub` to forward-declare a class before its full definition — required for self-referencing fields (e.g. _linked lists_) and cyclic references between classes.

::: eco_python2acom.decorators.layout.stub
