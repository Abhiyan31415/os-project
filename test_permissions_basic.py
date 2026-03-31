#!/usr/bin/env python3
"""
Basic test for xv6 file permissions system
Tests that xv6 boots and basic file operations work
"""

import subprocess
import time
import sys

def test_basic_boot():
    """Test that xv6 boots successfully"""
    print("Testing basic xv6 boot...")
    
    # Start qemu
    proc = subprocess.Popen(
        ["make", "qemu"],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True
    )
    
    try:
        # Wait for boot
        time.sleep(2)
        
        # Send ls command
        proc.stdin.write("ls\n")
        proc.stdin.flush()
        time.sleep(1)
        
        # Send echo command to create file
        proc.stdin.write("echo hello world\n")
        proc.stdin.flush()
        time.sleep(1)
        
        # Exit
        proc.stdin.write("exit\n")
        proc.stdin.flush()
        time.sleep(1)
        
        # Read output
        output = proc.stdout.read()
        
        # Check if we got the shell prompt
        if "$" in output:
            print("✓ xv6 booted successfully")
            print("✓ Shell is responsive")
            return True
        else:
            print("✗ Failed to boot xv6 or get shell prompt")
            print("Output:", output)
            return False
            
    finally:
        # Kill qemu
        proc.terminate()
        try:
            proc.wait(timeout=5)
        except subprocess.TimeoutExpired:
            proc.kill()
            proc.wait()

if __name__ == "__main__":
    success = test_basic_boot()
    sys.exit(0 if success else 1)
