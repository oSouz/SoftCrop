"""Static PE inspection; never loads or executes the input DLL. Python stdlib only."""
import sys, struct, hashlib, json, re
from pathlib import Path
b=Path(sys.argv[1]).read_bytes()
def u16(o): return struct.unpack_from('<H',b,o)[0]
def u32(o): return struct.unpack_from('<I',b,o)[0]
assert b[:2]==b'MZ'
pe=u32(60); assert b[pe:pe+4]==b'PE\0\0'
opt=pe+24; magic=u16(opt); directories=opt+(112 if magic==0x20b else 96)
sections=[]
for i in range(u16(pe+6)):
 o=opt+u16(pe+20)+40*i
 sections.append((u32(o+12),max(u32(o+8),u32(o+16)),u32(o+20)))
def offset(rva):
 for va,size,raw in sections:
  if va<=rva<va+size: return raw+rva-va
 if rva<u32(opt+60): return rva
 raise ValueError('RVA outside sections')
def cstr(o): return b[o:b.index(b'\0',o)].decode('ascii',errors='replace')
exports=[]; erva=u32(directories)
if erva:
 e=offset(erva); names=offset(u32(e+32))
 exports=[cstr(offset(u32(names+4*i))) for i in range(u32(e+24))]
imports=[]; irva=u32(directories+8)
if irva:
 o=offset(irva)
 while any(b[o:o+20]):
  imports.append(cstr(offset(u32(o+12)))); o+=20
strings=[m.group().decode('ascii') for m in re.finditer(rb'[\x20-\x7e]{4,}',b)]
wanted=['SoftCropper','Haute Technique','cropL','cropT','cropR','cropB','sizeL','sizeT','sizeR','sizeB','Crop Left','Crop Top','Crop Right','Crop Bottom','Soft Left','Soft Top','Soft Right','Soft Bottom','Style','Multiply','style']
r={'file':Path(sys.argv[1]).name,'size':len(b),'sha256':hashlib.sha256(b).hexdigest(),
 'machine':hex(u16(pe+4)),'optional_header':hex(magic),'timestamp_raw':u32(pe+8),
 'certificate_table_present':bool(u32(directories+32)), 'exports':exports,'imports':imports,
 'selected_exact_strings':[s for s in wanted if s in strings]}
print(json.dumps(r,indent=2))
