def crc16(data: bytes):

	poly= ~0x1021
	data = bytearray(data)
	crc = 0xFFFF
	for b in data:
		cur_byte = 0xFF & b
		for _ in range(0, 8):
			if (crc & 0x0001) ^ (cur_byte & 0x0001):
				crc = (crc >> 1) ^ poly
			else:
				crc >>= 1
			cur_byte >>= 1
	crc = (~crc & 0xFFFF)
	crc = (crc << 8) | ((crc >> 8) & 0xFF)

	return crc & 0xFFFF

print(crc16(b'\x9e\x00\x02\x04\x0c'))

# def crc16(data: bytes, poly=0x8408):
#     '''
#     CRC-16-CCITT Algorithm
#     '''
#     data = bytearray(data)
#     crc = 0xFFFF
#     for b in data:
#         cur_byte = 0xFF & b
#         for _ in range(0, 8):
#             if (crc & 0x0001) ^ (cur_byte & 0x0001):
#                 crc = (crc >> 1) ^ poly
#             else:
#                 crc >>= 1
#             cur_byte >>= 1
#     crc = (~crc & 0xFFFF)
#     crc = (crc << 8) | ((crc >> 8) & 0xFF)
    
#     return crc & 0xFFFF
# print(crc16(b'\x9e\x02\x02\x03\x0c\x01\x00\x00\x00\x0d'))
# POLYNOMIAL = 0x1021
# PRESET = 0

# def _initial(c):
#     crc = 0
#     c = c << 8
#     for j in range(8):
#         if (crc ^ c) & 0x8000:
#             crc = (crc << 1) ^ POLYNOMIAL
#         else:
#             crc = crc << 1
#         c = c << 1
#     return crc

# _tab = [ _initial(i) for i in range(256) ]

# def _update_crc(crc, c):
#     cc = 0xff & c

#     tmp = (crc >> 8) ^ cc
#     crc = (crc << 8) ^ _tab[tmp & 0xff]
#     crc = crc & 0xffff
#     print (crc)

#     return crc

# def crc(str):
#     crc = PRESET
#     for c in str:
#         crc = _update_crc(crc, ord(c))
#     return crc

# def crcb(*i):
#     crc = PRESET
#     for c in i:
#         crc = _update_crc(crc, c)
#     return crc



