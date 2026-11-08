#!/bin/sh
# Раскладка для yambar через xkb-monitor: us/ru/ro -> EN/RU/RO
"$HOME/.local/bin/xkb-monitor" -n | while IFS= read -r l; do
    case "$l" in
        us) l=EN ;;
        *)  l=$(printf '%s' "$l" | tr '[:lower:]' '[:upper:]') ;;
    esac
    printf 'layout|string|%s\n\n' "$l"
done
