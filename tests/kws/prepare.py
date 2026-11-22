"""Convert one Vela raw Shared_Sram KWS payload into a guest fixture header."""

import argparse
from pathlib import Path

import numpy as np


def array(name, data):
    rows = (data[start : start + 16] for start in range(0, len(data), 16))
    body = "\n".join(
        "    " + ", ".join(f"0x{v:02x}" for v in row) + "," for row in rows
    )
    return f"NPU_MODEL static const uint8_t {name}[] = {{\n{body}\n}};\n"


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("payload", type=Path)
    parser.add_argument("header", type=Path)
    args = parser.parse_args()
    with np.load(args.payload, allow_pickle=False) as data:
        size = int(data["scratch_size"])
        offsets = {}
        for kind, length in (("input", 490), ("output", 12)):
            (offset,) = data[f"{kind}_offset"]
            if (
                data[f"{kind}_region"].tolist() != [1]
                or data[f"{kind}_elem_size"].tolist() != [1]
                or len(data[f"{kind}_shape"]) != 1
                or int(np.prod(data[f"{kind}_shape"][0])) != length
                or not 0 <= offset <= size - length
            ):
                raise ValueError(f"Unexpected {kind} layout")
            offsets[kind] = int(offset)
        if (
            int(data["weight_region"]),
            int(data["scratch_region"]),
            int(data["scratch_fast_size"]),
        ) != (0, 1, 0):
            raise ValueError("Expected a single Shared_Sram command stream")
        text = array("commands", data["cmd_data"].tobytes()) + array(
            "weights", data["weight_data"].tobytes()
        )
        args.header.write_text(
            text + f"enum {{ ARENA_SIZE = {size}, INPUT_OFFSET = {offsets['input']}, "
            f"OUTPUT_OFFSET = {offsets['output']} }};\n"
        )


if __name__ == "__main__":
    main()
