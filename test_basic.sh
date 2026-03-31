#!/bin/bash
# Test basic functionality of xv6 with file permissions

# Boot xv6 and run basic commands
timeout 30 make qemu <<'EOF'
ls
echo hello > testfile
cat testfile
ls -l
EOF
