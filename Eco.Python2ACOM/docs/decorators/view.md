# View Decorator

Use this decorator to group the implementation of one ACOM interface inside a `@component`. A view is a namespace — its methods are wired into the component's vtable for that interface, with `self` bound to the parent component instance. The first declared `@view` answers `IID_IEcoUnknown` queries.

::: eco_python2acom.decorators.server.view.view
