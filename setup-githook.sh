#!/bin/sh

BASE_DIR=$(dirname $(readlink -f "$0"))
HOOK="$BASE_DIR/.git/hooks/pre-commit"

echo "#!/bin/sh" > "$HOOK"
echo 'BASE_DIR=$(dirname $(readlink -f "$0"))' >> "$HOOK"
echo 'sh "$BASE_DIR/../../merge-headers.sh"' >> "$HOOK"
echo 'git add "$BASE_DIR/../../rusty.hpp"' >> "$HOOK"

chmod +x "$HOOK"
