# Test data

Run `python3 tools/generate_data.py` from the project root to recreate the input
files. Compile `std.cpp`, then run
`python3 tools/generate_answers.py /path/to/std` to recreate the `.ans` files.

The directories map to the statement's subtasks. The two small groups contain
boundary and hand-built logic traps; the large groups include maximum-size
chain, star, balanced, random, comb, and broom trees. Weights vary from 1 to 100,
except the maximum-weight chain used to expose 32-bit overflow.
