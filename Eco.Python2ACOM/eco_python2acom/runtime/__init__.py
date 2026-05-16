"""EcoOS runtime package.

This package provides the core runtime machinery for the Python2ACOM bridge:
bootstrapping the ACOM environment, loading C component libraries, querying
platform facilities, and routing log output.

Modules:
    loader: Load EcoOS component libraries and extract factories.
    logging: Logger and helpers for the package-wide log stream.
    platform: Platform-specific helpers.
    system: `EcoSystem` — root ACOM component that boots the runtime.
    utils: Helpers for library filename and GUID parsing.
"""
