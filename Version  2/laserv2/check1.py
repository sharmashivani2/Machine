def checksum256(_str):
	return reduce(lambda x,y:x+y, map(ord, _str)) % 256
def hor_ang(ag):
    data1 = hex(ag)[2:]
    if(len(data1)<4):
        datal = data1[1:]
        datah = '0'+data1[:1]
    else:
        datal = data1[2:]
        datah = data1[:2]
    final = ' '+datah+' '+datal+' 01'
    return final
def ver_ang(ag):
    data1 = hex(ag)[2:]
    if(len(data1)<4):
        datal = data1[1:]
        datah = '0'+data1[:1]
    else:
        datal = data1[2:]
        datah = data1[:2]
    final = ' '+datah+' '+datal+' 01'
    return final
# final = hor_ang(4500)
dt1 = 'ff 01 00 4b'
fdata = dt1+hor_ang(1000)
fdata2 = bytearray.fromhex(fdata)
print(fdata)
_checksum = hex(checksum256(str(fdata2)))[2:]
if len(_checksum) is 1:
	_corrected_checksum = '0' + _checksum
else:
	_corrected_checksum = _checksum
final = fdata[:-2]+_corrected_checksum
print("final ",final)


