"""Unit tests for `eco_python2acom.runtime.logging` module.

This module tests the `EcoFormatter` line layout, the `configure` setup
helper, and the `at_level` context manager / decorator. The package-wide
`eco_logger` is re-configured before every test so handlers and level do not
leak across tests.

Test Classes:
    TestEcoFormatter: Tests for `EcoFormatter` output and ANSI coloring.
    TestConfigure: Tests for `configure` handler setup and reconfiguration.
    TestAtLevel: Tests for the `at_level` context manager / decorator.
"""

import logging
import logging.handlers
from collections.abc import Generator
from pathlib import Path
from unittest.mock import patch

import pytest

from eco_python2acom.runtime.logging import (
    AnsiColor,
    Defaults,
    EcoFormatter,
    at_level,
    configure,
    eco_logger,
)


def make_record(
    name: str = Defaults.LOGGER_NAME,
    level: int = logging.INFO,
    msg: str = "hello",
    func: str = "func",
) -> logging.LogRecord:
    """Build a minimal `LogRecord` for formatter tests."""
    record = logging.LogRecord(
        name=name,
        level=level,
        pathname="test.py",
        lineno=1,
        msg=msg,
        args=None,
        exc_info=None,
        func=func,
    )
    return record


@pytest.fixture(autouse=True)
def reset_logger() -> Generator[None, None, None]:
    """Fixture restoring `eco_logger` to its post-import state between tests."""
    previous_level = eco_logger.level
    previous_handlers = list(eco_logger.handlers)
    yield
    eco_logger.setLevel(previous_level)
    for handler in list(eco_logger.handlers):
        eco_logger.removeHandler(handler)
    for handler in previous_handlers:
        eco_logger.addHandler(handler)


@pytest.mark.unit
class TestEcoFormatter:
    """Tests for `EcoFormatter` output.

    Verifies the rendered line shape, scope rewriting, and ANSI coloring.
    """

    @pytest.fixture
    def formatter(self) -> EcoFormatter:
        """Fixture returning default logging formatter."""
        return EcoFormatter(use_color=False)

    def test_root_package_scope_renders_as_core(self, formatter: EcoFormatter) -> None:
        """Verifies a record from the root package logger uses `core` as its scope."""
        line = formatter.format(make_record(name=Defaults.LOGGER_NAME))

        assert "core::func()" in line

    def test_subpackage_scope_is_stripped_of_prefix(self, formatter: EcoFormatter) -> None:
        """Verifies the `eco_python2acom.` prefix is stripped from a subpackage scope."""
        line = formatter.format(make_record(name=f"{Defaults.LOGGER_NAME}.types.guid"))

        assert "types.guid::func()" in line
        assert Defaults.LOGGER_NAME not in line

    def test_foreign_scope_is_left(self, formatter: EcoFormatter) -> None:
        """Verifies a logger outside the package keeps its full name."""
        line = formatter.format(make_record(name="other.lib"))

        assert "other.lib::func()" in line

    def test_layout_matches_expected_format(self, formatter: EcoFormatter) -> None:
        """Verifies the layout is `<timestamp> [LEVEL] <scope>::<func>() ---> <msg>`."""
        line = formatter.format(
            make_record(name=Defaults.LOGGER_NAME, level=logging.WARNING, msg="msg", func="func")
        )

        # Layout: "2026-05-17 12:34:56 [WARNING] core::func() ---> msg"
        assert " [WARNING] core::func() ---> msg" in line

    def test_color_wraps_line_when_use_color_true(self) -> None:
        """Verifies the rendered line is wrapped in the level-specific ANSI color."""
        line = EcoFormatter(use_color=True).format(make_record(level=logging.ERROR))

        assert line.startswith(AnsiColor.RED)
        assert line.endswith(AnsiColor.RESET)

    def test_color_omitted_when_use_color_false(self, formatter: EcoFormatter) -> None:
        """Verifies no ANSI escapes are emitted when colors are disabled."""
        line = formatter.format(make_record(level=logging.ERROR))

        assert AnsiColor.RED not in line
        assert AnsiColor.RESET not in line

    def test_unknown_level_is_not_colored(self) -> None:
        """Verifies a level without a registered color renders without escapes."""
        line = EcoFormatter(use_color=True).format(make_record(level=logging.NOTSET))

        assert AnsiColor.RESET not in line


@pytest.mark.unit
class TestConfigure:
    """Tests for `configure` setup helper.

    Verifies handler installation, reconfiguration, and the rotating-file path.
    """

    def test_default_configures_package_logger(self) -> None:
        """Verifies `configure()` reconfigures the package-wide logger."""
        result = configure()

        assert result is eco_logger
        assert eco_logger.propagate is False
        assert eco_logger.level == Defaults.LEVEL

    def test_named_logger_is_used_when_name_passed(self) -> None:
        """Verifies `configure(name=...)` reconfigures the logger of that name."""
        target = configure(name="test")

        assert target is logging.getLogger("test")
        assert target is not eco_logger

    def test_existing_handlers_are_replaced_on_reconfigure(self) -> None:
        """Verifies that calling `configure` twice leaves the logger with a single handler."""
        configure()
        configure()

        assert len(eco_logger.handlers) == 1

    def test_console_handler_used_without_log_file(self) -> None:
        """Verifies the default sink is a `StreamHandler` (console)."""
        configure(use_color=False)

        assert len(eco_logger.handlers) == 1
        handler = next(iter(eco_logger.handlers))
        assert isinstance(handler, logging.StreamHandler)
        assert isinstance(handler.formatter, EcoFormatter)

    @pytest.mark.parametrize(
        ["isatty", "expected"],
        [(True, True), (False, False)],
        ids=["tty", "no-tty"],
    )
    def test_use_color_none_falls_back_to_tty_check(self, isatty: bool, expected: bool) -> None:
        """Verifies `configure(use_color=None, ...)` enables color iff `sys.stdout.isatty()` is true."""
        with patch("sys.stdout.isatty", return_value=isatty):
            configure(use_color=None)
            handler = next(iter(eco_logger.handlers))
            assert handler.formatter._use_color is expected

    def test_log_file_installs_rotating_file_handler(self, tmp_path: Path) -> None:
        """Verifies `configure(log_file=...)` installs a `RotatingFileHandler` with given rotation."""
        log_path = tmp_path / "eco.log"
        configure(log_file=str(log_path), max_bytes=2048, backup_count=5)

        assert len(eco_logger.handlers) == 1
        handler = next(iter(eco_logger.handlers))
        assert isinstance(handler, logging.handlers.RotatingFileHandler)
        assert handler.maxBytes == 2048
        assert handler.backupCount == 5
        assert handler.formatter._use_color is False
        handler.close()  # Release the file handle

    def test_level_is_applied(self) -> None:
        """Verifies the `level` argument is applied to the target logger."""
        configure(level=logging.DEBUG)

        assert eco_logger.level == logging.DEBUG


@pytest.mark.unit
class TestAtLevel:
    """Tests for the `at_level` context manager / decorator.

    Verifies that the level is changed inside the scope and restored on exit.
    """

    def test_context_manager_applies_and_restores_level(self) -> None:
        """Verifies the level is set on enter and restored on exit."""
        configure(level=logging.WARNING)

        with at_level(logging.DEBUG) as target:
            assert target is eco_logger
            assert eco_logger.level == logging.DEBUG

        assert eco_logger.level == logging.WARNING

    def test_level_restored_on_exception(self) -> None:
        """Verifies the previous level is restored when the `with` block raises."""
        configure(level=logging.WARNING)

        with pytest.raises(RuntimeError):
            with at_level(logging.DEBUG):
                raise RuntimeError("error")

        assert eco_logger.level == logging.WARNING

    def test_decorator_form_applies_inside_function(self) -> None:
        """Verifies the decorator form changes the level inside the wrapped function."""
        configure(level=logging.WARNING)
        observed: list[int] = []

        @at_level(logging.DEBUG)
        def function() -> None:
            observed.append(eco_logger.level)

        function()

        assert observed == [logging.DEBUG]
        assert eco_logger.level == logging.WARNING

    def test_custom_target_logger_is_used(self) -> None:
        """Verifies `at_level(target=...)` reconfigures the specified logger."""
        other = logging.getLogger("test")
        other.setLevel(logging.WARNING)
        configure(level=logging.WARNING)

        with at_level(logging.DEBUG, target=other):
            assert other.level == logging.DEBUG
            assert eco_logger.level == logging.WARNING

        assert other.level == logging.WARNING
