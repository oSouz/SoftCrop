"""Checks only the newly built Souz DLL, not the proprietary reference."""
import ctypes as c,sys,json,struct,re
from pathlib import Path
class Info(c.Structure):
 _fields_=[('major',c.c_uint32),('minor',c.c_uint32),('id',c.c_char*4),('name',c.c_char*16),('kind',c.c_uint32)]
dll=c.WinDLL(str(Path(sys.argv[1]).resolve()))
f=dll.plugMain; f.argtypes=[c.c_uint32,c.c_uint64,c.c_void_p]; f.restype=c.c_uint64
info=c.cast(f(0,0,None),c.POINTER(Info)).contents
assert (info.major,info.minor,info.id)==(2,1,b'SZC1')
assert f(34,0,None)==0
try:
 assert f(4,0,None)==9
 expected=['Crop Left','Crop Top','Crop Right','Crop Bottom','Soft Left','Soft Top','Soft Right','Soft Bottom','Corner Blend']
 params=[]
 for i,name in enumerate(expected):
  actual=c.string_at(f(5,i,None),16).split(b'\0')[0].decode()
  default=struct.unpack('<f',struct.pack('<I',f(6,i,None)&0xffffffff))[0]
  assert actual==name and default==0.0,(i,actual,default)
  params.append({'name':actual,'default':default})
 class Thumbnail(c.Structure):
  _fields_=[('width',c.c_uint32),('height',c.c_uint32),('pixels',c.c_void_p)]
 thumb=Thumbnail()
 assert f(42,c.addressof(thumb),None)==0
 assert (thumb.width,thumb.height)==(160,120)
 pixels=(c.c_ubyte*(160*120*4))()
 thumb.pixels=c.cast(pixels,c.c_void_p).value
 assert f(42,c.addressof(thumb),None)==0
 words=struct.unpack('<'+'I'*(160*120),bytes(pixels))
 assert all((word >> 24)==255 for word in words)
 header=(Path(__file__).resolve().parents[1]/'src/SouzThumbnail.h').read_text()
 expected_pixels=[int(word,16) for word in re.findall(r'0x([0-9A-F]{8})u',header)]
 assert list(words)==expected_pixels
 assert max(pixels)>min(pixels)
 print(json.dumps({'api':'2.1','id':'SZC1','name':info.name.decode(),'parameters':params,'thumbnail':'160x120; all pixels match embedded image; PASS','result':'PASS'},indent=2))
finally:
 assert f(2,0,None)==0
