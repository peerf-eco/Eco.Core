"""Load benchmark for a single `Eco.Test` method call from a Python host.

Runs twice — calling the native `dll` component (Python -> C) and the Python
`EcoTest` component through the `Eco.ACOM2Python` bridge (Python -> C -> Python)
— so the bridge overhead shows up as the ratio between the `[c]` and `[py]`
runs. Each run reports throughput, latency percentiles (p50 / p99 / p99.9) and
the `tracemalloc` heap growth. Results are written to file.

Test Classes:
    TestCallOverhead: Measures `Addition` call latency, throughput and memory.
"""

import gc
import tracemalloc
from collections.abc import Generator
from pathlib import Path
from time import perf_counter

import pytest

from eco_python2acom.types.pointer import Ptr
from tests.integration.structures import IEcoTest

ITERATIONS = 1_000_000
REPORT_PATH = Path(__file__).resolve().parent.parent / "report.md"

# Accumulates one row per back-end across the parameterised runs
results: list[dict[str, object]] = []


def percentile(samples: list[float], fraction: float) -> float:
    """Return the nearest-rank percentile of the latency samples.

    Args:
        samples: Latency samples in microseconds.
        fraction: Percentile position between 0 and 1.

    Returns:
        The sample at the requested percentile.
    """
    ordered = sorted(samples)
    rank = max(0, min(len(ordered) - 1, round(fraction * (len(ordered) - 1))))
    return ordered[rank]


def render_report() -> str:
    """Render the accumulated results as a Markdown report.

    Returns:
        The report text: one table row per back-end plus the bridge ratio.
    """
    lines = [
        "# Eco.Test Call Benchmark",
        "",
        f"`Addition(2, 3)` x {ITERATIONS:,} calls, from a Python host:",
        "",
        "- **c** — calling the native `dll` component (Python -> C).",
        "- **py** — calling the Python `EcoTest` component through the "
        "`Eco.ACOM2Python` bridge (Python -> C -> Python).",
        "",
        "| Back-end | Throughput (calls/s) | p50 (us) | p99 (us) | p99.9 (us) | Heap growth (KiB) |",
        "| --- | ---: | ---: | ---: | ---: | ---: |",
    ]
    for row in results:
        lines.append(
            f"| {row['backend']} | {row['throughput']:,.0f} | {row['p50']:.2f} | "
            f"{row['p99']:.2f} | {row['p999']:.2f} | {row['heap_kib']:.1f} |"
        )

    by_name = {row["backend"]: row for row in results}
    if "c" in by_name and "py" in by_name:
        ratio = by_name["py"]["p50"] / by_name["c"]["p50"] if by_name["c"]["p50"] else 0.0
        lines += [
            "",
            f"**Bridge overhead:** a call into the Python component costs ~{ratio:.1f}x "
            "a call into the C component — the price of dispatching back into the interpreter.",
        ]

    lines.append("")
    return "\n".join(lines)


@pytest.fixture(scope="session", autouse=True)
def write_report() -> Generator[None, None, None]:
    """Session fixture that writes the accumulated report after all runs."""
    yield
    if results:
        REPORT_PATH.write_text(render_report(), encoding="utf-8")


@pytest.mark.benchmark
class TestCallOverhead:
    """Measures the latency, throughput and memory of a single `Addition` call."""

    def test_call_overhead(self, eco_test: Ptr[IEcoTest], request: pytest.FixtureRequest) -> None:
        """Time many `Addition` calls, recording latency, throughput and heap growth."""
        call = eco_test.obj.Addition

        # Warm up so first-call effects (caching, lazy init) don't skew samples
        for _ in range(1000):
            call(2, 3)

        # Time the calls, collecting per-call latency
        samples: list[float] = []
        gc.collect()
        tracemalloc.start()
        baseline, _ = tracemalloc.get_traced_memory()

        start = perf_counter()
        for _ in range(ITERATIONS):
            tick = perf_counter()
            call(2, 3)
            samples.append((perf_counter() - tick) * 1_000_000.0)
        elapsed = perf_counter() - start

        # Measure only what the calls retain
        throughput = ITERATIONS / elapsed
        p50, p99, p999 = (percentile(samples, q) for q in (0.50, 0.99, 0.999))
        del samples
        gc.collect()
        retained, _ = tracemalloc.get_traced_memory()
        tracemalloc.stop()

        backend = request.node.callspec.params["eco_test"]
        results.append(
            {
                "backend": backend,
                "throughput": throughput,
                "p50": p50,
                "p99": p99,
                "p999": p999,
                "heap_kib": (retained - baseline) / 1024.0,
            }
        )

        # Sanity: the call works, and the calls retain no per-call memory (no leak)
        assert call(2, 3) == 5
        assert (retained - baseline) < 64 * 1024
