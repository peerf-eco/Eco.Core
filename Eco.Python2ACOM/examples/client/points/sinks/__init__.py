"""Client-side sinks for the `Eco.BinarySearch` connection-point example.

    - `trace`: `EcoBinarySearchTraceSink` — subscribes to both outgoing
      interfaces (events + stats) on their two connection points.
    - `chart`: `EcoBinarySearchChartSink` — subscribes only to the events point
      and animates the search as a bar chart.
"""
