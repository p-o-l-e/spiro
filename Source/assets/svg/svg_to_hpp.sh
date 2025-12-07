
#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SVG_DIR="$SCRIPT_DIR"
AWK_SCRIPT="$SCRIPT_DIR/svg_to_hpp.awk"

if [[ ! -d "$SVG_DIR" ]]; then
  echo "Error: svg/ folder not found" >&2
  exit 1
fi

for f in "$SVG_DIR"/*.svg; do
  base="$(basename "$f" .svg)"
  out="$SVG_DIR/${base}.hpp"
  echo "Generating $out"
  awk -v ns="$base" -f "$AWK_SCRIPT" "$f" > "$out"
done

