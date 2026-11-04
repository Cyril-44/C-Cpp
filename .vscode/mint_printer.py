import gdb

class MintPrinter:
    def __init__(self, val):
        self.val = val

    def to_string(self):
        return str(int(self.val["val"]))

def lookup_type(val):
    if str(val.type.strip_typedefs()) == "Mint":
        return MintPrinter(val)
    return None

gdb.pretty_printers.append(lookup_type)
