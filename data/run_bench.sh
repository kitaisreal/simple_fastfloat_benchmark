for file in `ls -I "*.sh"`; do
    echo "File: $file"
    benchmark -f "$file"
done
