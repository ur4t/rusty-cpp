
BASE_DIR=$(dirname $(readlink -f "$0"))
SRC_DIR="$BASE_DIR/src"
MERGED_HEADER="$BASE_DIR/rusty.hpp"

rm -f "$MERGED_HEADER"

echo "// SPDX-License-Identifier: MIT" >> "$MERGED_HEADER"
echo "// Copyright(c) 2021 ur4t" >> "$MERGED_HEADER"
echo >> "$MERGED_HEADER"
echo "#ifndef __RUSTY_HPP__" >> "$MERGED_HEADER"
echo "#define __RUSTY_HPP__" >> "$MERGED_HEADER"

for i in $(grep "#include" "$SRC_DIR/rusty.hpp" | sed "s/#include \"\(.*\)\"/\1/g"); do
    sed "/__RUSTY_.*_HPP__/d;/License-Identifier/d;/Copyright/d" "$SRC_DIR/$i" >> "$MERGED_HEADER"
done

sed "/__RUSTY_HPP__/d;/#include/d;/License-Identifier/d;/Copyright/d" "$SRC_DIR/rusty.hpp" >> "$MERGED_HEADER"
echo "#endif // __RUSTY_HPP__" >> "$MERGED_HEADER"

# remove additional blank lines
sed -i "/^$/N;/^\n$/D" "$MERGED_HEADER"
