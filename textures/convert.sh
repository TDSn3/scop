if ! command -v magick &> /dev/null; then
    echo "Error: ImageMagick is not installed."
fi

if [[ $# -ne 1 ]]; then
    echo "Usage: $0 input.<png/jpg>"
    exit 1
fi

input="$1"
output="${input%.*}.ppm"

magick "$input" "$output"

echo "Converted: \"$input\" into \"$output\""
