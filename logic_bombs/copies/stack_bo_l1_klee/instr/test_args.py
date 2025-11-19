
import sys
print(f"Number of arguments: {len(sys.argv)}")
for i, arg in enumerate(sys.argv):
    print(f"Argument {i}: {repr(arg)}")
    print(f"Argument {i} bytes: {[b for b in arg.encode('latin1')]}")
