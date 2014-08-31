BCON format Description
=======================

    ROOT		::= 	TLIST LIST TEND
    		|	TMAP MAP TEND
    
    LIST		::=	LELT LIST
    		|	""
    
    MAP		::=	MELT MAP
    		|	""
    
    LELT		::=	TNULL
    		|	TTRUE
    		|	TFALSE
    		|	TBYTE int8
    		|	TINT32 int32
    		|	TINT64 int64
    		|	TUINT32 uint32
    		|	TUINT64 uint64
    		|	TDOUBLE double
    		|	TTIMESTAMP int64
    		|	TLIST LIST TEND
    		|	TMAP MAP TEND
    		|	TDATA6 byte*
    		|	TSTRING6 byte*
    		|	TDATA12 byte byte+
    		|	TSTRING12 byte byte+
    		|	TDATA20 byte{2} byte+
    		|	TSTRING20 byte{2} byte+
    		|	TDATA36 byte{4} byte+
    		|	TSTRING36 byte{4} byte+
    
    MELT		::=	TNULL KEY
    		|	TTRUE KEY
    		|	TFALSE KEY
    		|	TBYTE int8 KEY
    		|	TINT32 int32 KEY
    		|	TINT64 int64 KEY
    		|	TUINT32 uint32 KEY
    		|	TUINT64 uint64 KEY
    		|	TDOUBLE double KEY
    		|	TTIMESTAMP int64 KEY
    		|	TLIST LIST TEND KEY
    		|	TMAP MAP TEND KEY
    		|	TDATA6 byte* KEY
    		|	TSTRING6 byte* KEY
    		|	TDATA12 byte byte+ KEY
    		|	TSTRING12 byte byte+ KEY
    		|	TDATA20 byte{2} byte+ KEY
    		|	TSTRING20 byte{2} byte+ KEY
    		|	TDATA36 byte{4} byte+ KEY
    		|	TSTRING36 byte{4} byte+ KEY
    
    KEY		::=	byte* "\x00"
    
    TEND		::=	"\x00" (0b00000000)	End of array or map
    TNULL		::=	"\x01" (0b00000001)	Null value
    TTRUE		::=	"\x02" (0b00000010)	True boolean
    TFALSE		::=	"\x03" (0b00000011)	False boolean
    TBYTE		::=	"\x04" (0b00000100)	byte value (int8, on 1 byte)
    TINT32		::=	"\x05" (0b00000101)	integer value (int32, on 4 bytes)
    TINT64		::=	"\x06" (0b00000110)	long value (int64, on 8 bytes)
    TUINT32		::=	"\x07" (0b00000111)	unsigned integer value (uint32 on 4 bytes)
    TUINT64		::=	"\x08" (0b00001000)	unsigned long value (int64 on 8 bytes)
    TDOUBLE		::=	"\x0A" (0b00001010)	8 bytes (64-bit IEEE 754 floating point)
    TTIMESTAMP	::=	"\x0B" (0b00001011)	Timestamp
    TMAP		::=	"\x0C" (0b00001100)	Map begin
    TLIST		::=	"\x0D" (0b00001101)	List begin
    TDATA6		::=	"\xAX" (0b10XXXXXX)	data from 0 to 2^6-1 (63) bytes (the length is coded on bits 0-5)
    TSTRING6	::=	"\xBX" (0b11XXXXXX)	string from 0 to 2^6-1 (63) characters (the length is coded on bits 0-5)
    TDATA12		::=	"\x1X" (0b0001XXXX)	data from 2^6 (64) to 2^12-1 (4095) bytes (the length is coded on bits 0-3 and the following byte)
    TDATA20		::=	"\x2X" (0b0010XXXX)	data from 2^12 (4096) to 2^20-1 bytes (the length is coded on bits 0-3 and 2 the following byte)
    TDATA36		::=	"\x3X" (0b0011XXXX)	data from 2^20 to 2^36-1 bytes (the length is coded on bits 0-3 and the 4 following byte)
    TSTRING12	::=	"\x5X" (0b0101XXXX)	string from 2^6 (64) to 2^12-1 (4095) characters (the length is coded on bits 0-3 and the following byte)
    TSTRING20	::=	"\x6X" (0b0110XXXX)	string from 2^12 (4096) to 2^20-1 characters (the length is coded on bits 0-3 and 2 the following byte)
    TSTRING36	::=	"\x7X" (0b0111XXXX)	string from 2^20 to 2^36-1 characters (the length is coded on bits 0-3 and the 4 following byte)
