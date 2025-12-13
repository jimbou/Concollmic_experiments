
import sys
print(f"argc: {len(sys.argv)}")
for i, arg in enumerate(sys.argv):
    print(f"argv[{i}]: {arg}")
