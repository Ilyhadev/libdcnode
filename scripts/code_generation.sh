#!/bin/bash

set -e

# This script regenerates C headers from DSDL files
# Run this ONCE when you add/modify .uavcan files

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"

DSDL_ROOT="${PROJECT_ROOT}/dsdl"
OUTPUT_DIR="${PROJECT_ROOT}/include/libdcnode/serialization/uavcan"
DRONECAN_DSDLC="${PROJECT_ROOT}/Libs/dronecan_dsdlc"

# Check if dronecan_dsdlc exists
if [ ! -d "$DRONECAN_DSDLC" ]; then
    echo "ERROR: dronecan_dsdlc not found at $DRONECAN_DSDLC"
fi

# Create output directory
mkdir -p "$OUTPUT_DIR"

# Run code generator
echo "Generating C code from DSDL files..."
echo "  DSDL root: $DSDL_ROOT"
echo "  Output dir: $OUTPUT_DIR"

python3 "$DRONECAN_DSDLC/dronecan_dsdlc.py" \
    -O "$OUTPUT_DIR" \
    "$DSDL_ROOT"

echo "Code generation complete!"
echo ""
echo "Generated files:"
find "$OUTPUT_DIR" -name "*.h" -o -name "*.c" | sort
