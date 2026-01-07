#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
SVG_DIR="$SCRIPT_DIR/modules/"
OUT="$SCRIPT_DIR/svg_modules.hpp"

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

cat > "$OUT" <<'HDR'
// Auto-generated from *.svg

#pragma once

#include <string>

namespace core {
namespace svg {
namespace assets {

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

echo "}}}" >> "$OUT"

echo "Generated header: $OUT"

