
#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SVG_DIR="$SCRIPT_DIR"
OUT="$SCRIPT_DIR/svg_assets.hpp"

if [[ ! -d "$SVG_DIR" ]]; then
  echo "Error: directory '$SVG_DIR' not found next to script" >&2
  exit 1
fi

sanitize_id() {
  local name="$1"
  name="${name%.*}"                         # strip extension
  name="$(echo "$name" | tr 'A-Z' 'a-z')"   # lowercase
  name="$(echo "$name" | sed 's/[^a-z0-9]/_/g')" # replace non-alnum
  [[ "$name" =~ ^[0-9] ]] && name="_$name"  # prefix if starts with digit
  echo "$name"
}

# Begin header
cat > "$OUT" <<'HDR'
// Auto-generated from svg/ folder next to this script.
// Do not edit manually.
#pragma once

#include <string>

namespace svg_assets {

HDR

shopt -s nullglob
for f in "$SVG_DIR"/*.svg "$SVG_DIR"/*.SVG; do
  base="$(basename "$f")"
  id="$(sanitize_id "$base")"

  {
    echo "inline const std::string ${id} = R\"SVG("
    cat "$f"
    echo ")SVG\";"
    echo
  } >> "$OUT"
done

echo "} // namespace svg_assets" >> "$OUT"

echo "Generated header: $OUT"

