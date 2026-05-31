# Eco.Test Call Benchmark

`Addition(2, 3)` x 1,000,000 calls, from a Python host:

- **c** — calling the native `dll` component (Python -> C).
- **py** — calling the Python `EcoTest` component through the `Eco.ACOM2Python` bridge (Python -> C -> Python).

| Back-end | Throughput (calls/s) | p50 (us) | p99 (us) | p99.9 (us) | Heap growth (KiB) |
| --- | ---: | ---: | ---: | ---: | ---: |
| c | 32,924 | 25.50 | 46.20 | 95.40 | 0.1 |
| py | 19,944 | 45.50 | 74.40 | 247.40 | 0.1 |

**Bridge overhead:** a call into the Python component costs ~1.8x a call into the C component — the price of dispatching back into the interpreter.
